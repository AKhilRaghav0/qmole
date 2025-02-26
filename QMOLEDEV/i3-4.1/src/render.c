/*
 * vim:ts=4:sw=4:expandtab
 *
 * i3 - an improved dynamic tiling window manager
 * © 2009-2011 Michael Stapelberg and contributors (see also: LICENSE)
 *
 * render.c: Renders (determines position/sizes) the layout tree, updating the
 *           various rects. Needs to be pushed to X11 (see x.c) to be visible.
 *
 */
#include "all.h"

/* change this to 'true' if you want to have additional borders around every
 * container (for debugging purposes) */
static bool show_debug_borders = false;

/*
 * Renders a container with layout L_OUTPUT. In this layout, all CT_DOCKAREAs
 * get the height of their content and the remaining CT_CON gets the rest.
 *
 */
static void render_l_output(Con *con) {
    Con *child, *dockchild;

    int x = con->rect.x;
    int y = con->rect.y;
    int height = con->rect.height;

    /* Find the content container and ensure that there is exactly one. Also
     * check for any non-CT_DOCKAREA clients. */
    Con *content = NULL;
    TAILQ_FOREACH(child, &(con->nodes_head), nodes) {
        if (child->type == CT_CON) {
            if (content != NULL) {
                DLOG("More than one CT_CON on output container\n");
                assert(false);
            }
            content = child;
        } else if (child->type != CT_DOCKAREA) {
            DLOG("Child %p of type %d is inside the OUTPUT con\n", child, child->type);
            assert(false);
        }
    }

    assert(content != NULL);

    /* We need to find out if there is a fullscreen con on the current workspace
     * and take the short-cut to render it directly (the user does not want to
     * see the dockareas in that case) */
    Con *ws = con_get_fullscreen_con(content, CF_OUTPUT);
    Con *fullscreen = con_get_fullscreen_con(ws, CF_OUTPUT);
    if (fullscreen) {
        fullscreen->rect = con->rect;
        x_raise_con(fullscreen);
        render_con(fullscreen, true);
        return;
    }

    /* First pass: determine the height of all CT_DOCKAREAs (the sum of their
     * children) and figure out how many pixels we have left for the rest */
    TAILQ_FOREACH(child, &(con->nodes_head), nodes) {
        if (child->type != CT_DOCKAREA)
            continue;

        child->rect.height = 0;
        TAILQ_FOREACH(dockchild, &(child->nodes_head), nodes)
            child->rect.height += dockchild->geometry.height;

        height -= child->rect.height;
    }

    /* Second pass: Set the widths/heights */
    TAILQ_FOREACH(child, &(con->nodes_head), nodes) {
        if (child->type == CT_CON) {
            child->rect.x = x;
            child->rect.y = y;
            child->rect.width = con->rect.width;
            child->rect.height = height;
        }

        child->rect.x = x;
        child->rect.y = y;
        child->rect.width = con->rect.width;

        child->deco_rect.x = 0;
        child->deco_rect.y = 0;
        child->deco_rect.width = 0;
        child->deco_rect.height = 0;

        y += child->rect.height;

        DLOG("child at (%d, %d) with (%d x %d)\n",
                child->rect.x, child->rect.y, child->rect.width, child->rect.height);
        x_raise_con(child);
        render_con(child, false);
    }
}

/*
 * "Renders" the given container (and its children), meaning that all rects are
 * updated correctly. Note that this function does not call any xcb_*
 * functions, so the changes are completely done in memory only (and
 * side-effect free). As soon as you call x_push_changes(), the changes will be
 * updated in X11.
 *
 */
void render_con(Con *con, bool render_fullscreen) {
    int children = con_num_children(con);
    DLOG("Rendering %snode %p / %s / layout %d / children %d / orient %d\n",
         (render_fullscreen ? "fullscreen " : ""), con, con->name, con->layout,
         children, con->orientation);

    /* Copy container rect, subtract container border */
    /* This is the actually usable space inside this container for clients */
    Rect rect = con->rect;

    /* Display a border if this is a leaf node. For container nodes, we don’t
     * draw borders (except when in debug mode) */
    if (show_debug_borders) {
        rect.x += 2;
        rect.y += 2;
        rect.width -= 2 * 2;
        rect.height -= 2 * 2;
    }

    int x = rect.x;
    int y = rect.y;

    int i = 0;

    con->mapped = true;

    /* if this container contains a window, set the coordinates */
    if (con->window) {
        /* depending on the border style, the rect of the child window
         * needs to be smaller */
        Rect *inset = &(con->window_rect);
        *inset = (Rect){0, 0, con->rect.width, con->rect.height};
        if (!render_fullscreen)
            *inset = rect_add(*inset, con_border_style_rect(con));

        /* Obey x11 border */
        inset->width -= (2 * con->border_width);
        inset->height -= (2 * con->border_width);

        /* Obey the aspect ratio, if any */
        if (con->proportional_height != 0 &&
            con->proportional_width != 0) {
            double new_height = inset->height + 1;
            int new_width = inset->width;

            while (new_height > inset->height) {
                new_height = ((double)con->proportional_height / con->proportional_width) * new_width;

                if (new_height > inset->height)
                    new_width--;
            }
            /* Center the window */
            inset->y += ceil(inset->height / 2) - floor(new_height / 2);
            inset->x += ceil(inset->width / 2) - floor(new_width / 2);

            inset->height = new_height;
            inset->width = new_width;
        }

        if (con->height_increment > 1) {
            int old_height = inset->height;
            inset->height -= (inset->height - con->base_height) % con->height_increment;
            DLOG("Lost %d pixel due to client's height_increment (%d px, base_height = %d)\n",
                old_height - inset->height, con->height_increment, con->base_height);
        }

        if (con->width_increment > 1) {
            int old_width = inset->width;
            inset->width -= (inset->width - con->base_width) % con->width_increment;
            DLOG("Lost %d pixel due to client's width_increment (%d px, base_width = %d)\n",
                old_width - inset->width, con->width_increment, con->base_width);
        }

        DLOG("child will be at %dx%d with size %dx%d\n", inset->x, inset->y, inset->width, inset->height);
    }

    /* Check for fullscreen nodes */
    Con *fullscreen = NULL;
    if (con->type != CT_OUTPUT) {
        fullscreen = con_get_fullscreen_con(con, (con->type == CT_ROOT ? CF_GLOBAL : CF_OUTPUT));
    }
    if (fullscreen) {
        fullscreen->rect = rect;
        x_raise_con(fullscreen);
        render_con(fullscreen, true);
        return;
    }

    /* find the height for the decorations */
    int deco_height = config.font.height + 5;

    /* precalculate the sizes to be able to correct rounding errors */
    int sizes[children];
    if (con->layout == L_DEFAULT && children > 0) {
        assert(!TAILQ_EMPTY(&con->nodes_head));
        Con *child;
        int i = 0, assigned = 0;
        int total = con->orientation == HORIZ ? rect.width : rect.height;
        TAILQ_FOREACH(child, &(con->nodes_head), nodes) {
            double percentage = child->percent > 0.0 ? child->percent : 1.0 / children;
            assigned += sizes[i++] = percentage * total;
        }
        assert(assigned == total ||
                (assigned > total && assigned - total <= children * 2) ||
                (assigned < total && total - assigned <= children * 2));
        int signal = assigned < total ? 1 : -1;
        while (assigned != total) {
            for (i = 0; i < children && assigned != total; ++i) {
                sizes[i] += signal;
                assigned += signal;
            }
        }
    }

    if (con->layout == L_OUTPUT) {
        render_l_output(con);
    } else if (con->type == CT_ROOT) {
        Con *output;
        TAILQ_FOREACH(output, &(con->nodes_head), nodes) {
            render_con(output, false);
        }

        /* We need to render floating windows after rendering all outputs’
         * tiling windows because they need to be on top of *every* output at
         * all times. This is important when the user places floating
         * windows/containers so that they overlap on another output. */
        DLOG("Rendering floating windows:\n");
        TAILQ_FOREACH(output, &(con->nodes_head), nodes) {
            /* Get the active workspace of that output */
            Con *content = output_get_content(output);
            Con *workspace = TAILQ_FIRST(&(content->focus_head));

            Con *child;
            TAILQ_FOREACH(child, &(workspace->floating_head), floating_windows) {
                DLOG("floating child at (%d,%d) with %d x %d\n", child->rect.x, child->rect.y, child->rect.width, child->rect.height);
                x_raise_con(child);
                render_con(child, false);
            }
        }

    } else {

        /* FIXME: refactor this into separate functions: */
    Con *child;
    TAILQ_FOREACH(child, &(con->nodes_head), nodes) {
        assert(children > 0);

        /* default layout */
        if (con->layout == L_DEFAULT) {
            if (con->orientation == HORIZ) {
                child->rect.x = x;
                child->rect.y = y;
                child->rect.width = sizes[i];
                child->rect.height = rect.height;
                x += child->rect.width;
            } else {
                child->rect.x = x;
                child->rect.y = y;
                child->rect.width = rect.width;
                child->rect.height = sizes[i];
                y += child->rect.height;
            }

            /* first we have the decoration, if this is a leaf node */
            if (con_is_leaf(child) && child->border_style == BS_NORMAL) {
                /* TODO: make a function for relative coords? */
                child->deco_rect.x = child->rect.x - con->rect.x;
                child->deco_rect.y = child->rect.y - con->rect.y;

                child->rect.y += deco_height;
                child->rect.height -= deco_height;

                child->deco_rect.width = child->rect.width;
                child->deco_rect.height = deco_height;
            }
        }

        /* stacked layout */
        else if (con->layout == L_STACKED) {
            child->rect.x = x;
            child->rect.y = y;
            child->rect.width = rect.width;
            child->rect.height = rect.height;

            child->deco_rect.x = x - con->rect.x;
            child->deco_rect.y = y - con->rect.y + (i * deco_height);
            child->deco_rect.width = child->rect.width;
            child->deco_rect.height = deco_height;

            if (children > 1 || (child->border_style != BS_1PIXEL && child->border_style != BS_NONE)) {
                child->rect.y += (deco_height * children);
                child->rect.height -= (deco_height * children);
            }
        }

        /* tabbed layout */
        else if (con->layout == L_TABBED) {
            child->rect.x = x;
            child->rect.y = y;
            child->rect.width = rect.width;
            child->rect.height = rect.height;

            child->deco_rect.width = child->rect.width / children;
            child->deco_rect.height = deco_height;
            child->deco_rect.x = x - con->rect.x + i * child->deco_rect.width;
            child->deco_rect.y = y - con->rect.y;

            if (children > 1 || (child->border_style != BS_1PIXEL && child->border_style != BS_NONE)) {
                child->rect.y += deco_height;
                child->rect.height -= deco_height;
            }
        }

        /* dockarea layout */
        else if (con->layout == L_DOCKAREA) {
            child->rect.x = x;
            child->rect.y = y;
            child->rect.width = rect.width;
            child->rect.height = child->geometry.height;

            child->deco_rect.x = 0;
            child->deco_rect.y = 0;
            child->deco_rect.width = 0;
            child->deco_rect.height = 0;
            y += child->rect.height;
        }

        DLOG("child at (%d, %d) with (%d x %d)\n",
                child->rect.x, child->rect.y, child->rect.width, child->rect.height);
        x_raise_con(child);
        render_con(child, false);
        i++;
    }

    /* in a stacking or tabbed container, we ensure the focused client is raised */
    if (con->layout == L_STACKED || con->layout == L_TABBED) {
        TAILQ_FOREACH_REVERSE(child, &(con->focus_head), focus_head, focused)
            x_raise_con(child);
        if ((child = TAILQ_FIRST(&(con->focus_head)))) {
            /* By rendering the stacked container again, we handle the case
             * that we have a non-leaf-container inside the stack. In that
             * case, the children of the non-leaf-container need to be raised
             * aswell. */
            render_con(child, false);
        }

        if (children != 1)
            /* Raise the stack con itself. This will put the stack decoration on
             * top of every stack window. That way, when a new window is opened in
             * the stack, the old window will not obscure part of the decoration
             * (it’s unmapped afterwards). */
            x_raise_con(con);
    }
    }
}
