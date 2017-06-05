/*
 * screen.c - screen management
 *
 * Copyright © 2007-2008 Julien Danjou <julien@danjou.info>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 */

#include <stdio.h>

#include <xcb/xcb.h>
#include <xcb/xinerama.h>

#include "screen.h"
#include "ewmh.h"
#include "tag.h"
#include "client.h"
#include "widget.h"
#include "wibox.h"

extern awesome_t globalconf;

static inline area_t
screen_xsitoarea(xcb_xinerama_screen_info_t si)
{
    area_t a =
    {
        .x = si.x_org,
        .y = si.y_org,
        .width = si.width,
        .height = si.height
    };
    return a;
}

/** Get screens informations and fill global configuration.
 */
void
screen_scan(void)
{
    /* Check for extension before checking for Xinerama */
    if(xcb_get_extension_data(globalconf.connection, &xcb_xinerama_id)->present)
    {
        xcb_xinerama_is_active_reply_t *xia;
        xia = xcb_xinerama_is_active_reply(globalconf.connection, xcb_xinerama_is_active(globalconf.connection), NULL);
        globalconf.xinerama_is_active = xia->state;
        p_delete(&xia);
    }

    if(globalconf.xinerama_is_active)
    {
        xcb_xinerama_query_screens_reply_t *xsq;
        xcb_xinerama_screen_info_t *xsi;
        int xinerama_screen_number;

        xsq = xcb_xinerama_query_screens_reply(globalconf.connection,
                                               xcb_xinerama_query_screens_unchecked(globalconf.connection),
                                               NULL);

        xsi = xcb_xinerama_query_screens_screen_info(xsq);
        xinerama_screen_number = xcb_xinerama_query_screens_screen_info_length(xsq);

        globalconf.screens = p_new(screen_t, xinerama_screen_number);

        /* now check if screens overlaps (same x,y): if so, we take only the biggest one */
        for(int screen = 0; screen < xinerama_screen_number; screen++)
        {
            bool drop = false;
            for(int screen_to_test = 0; screen_to_test < globalconf.nscreen; screen_to_test++)
                if(xsi[screen].x_org == globalconf.screens[screen_to_test].geometry.x
                   && xsi[screen].y_org == globalconf.screens[screen_to_test].geometry.y)
                    {
                        /* we already have a screen for this area, just check if
                         * it's not bigger and drop it */
                        drop = true;
                        globalconf.screens[screen_to_test].geometry.width =
                            MAX(xsi[screen].width, xsi[screen_to_test].width);
                        globalconf.screens[screen_to_test].geometry.height =
                            MAX(xsi[screen].height, xsi[screen_to_test].height);
                    }
            if(!drop)
            {
                globalconf.screens[globalconf.nscreen].index = screen;
                globalconf.screens[globalconf.nscreen++].geometry = screen_xsitoarea(xsi[screen]);
            }
        }

        /* realloc smaller if xinerama_screen_number != screen registered */
        if(xinerama_screen_number != globalconf.nscreen)
        {
            screen_t *new = p_new(screen_t, globalconf.nscreen);
            memcpy(new, globalconf.screens, globalconf.nscreen * sizeof(screen_t));
            p_delete(&globalconf.screens);
            globalconf.screens = new;
        }

        p_delete(&xsq);
    }
    else
    {
        globalconf.nscreen = xcb_setup_roots_length(xcb_get_setup(globalconf.connection));
        globalconf.screens = p_new(screen_t, globalconf.nscreen);
        for(int screen = 0; screen < globalconf.nscreen; screen++)
        {
            xcb_screen_t *s = xutil_screen_get(globalconf.connection, screen);
            globalconf.screens[screen].index = screen;
            globalconf.screens[screen].geometry.x = 0;
            globalconf.screens[screen].geometry.y = 0;
            globalconf.screens[screen].geometry.width = s->width_in_pixels;
            globalconf.screens[screen].geometry.height = s->height_in_pixels;
        }
    }

    globalconf.screen_focus = globalconf.screens;
}

/** Return the Xinerama screen number where the coordinates belongs to.
 * \param screen The logical screen number.
 * \param x X coordinate
 * \param y Y coordinate
 * \return Screen number or screen param if no match or no multi-head.
 */
int
screen_getbycoord(int screen, int x, int y)
{
    int i;

    /* don't waste our time */
    if(!globalconf.xinerama_is_active)
        return screen;

    for(i = 0; i < globalconf.nscreen; i++)
    {
        screen_t *s = &globalconf.screens[i];
        if((x < 0 || (x >= s->geometry.x && x < s->geometry.x + s->geometry.width))
           && (y < 0 || (y >= s->geometry.y && y < s->geometry.y + s->geometry.height)))
            return i;
    }

    return screen;
}

/** Get screens info.
 * \param screen Screen number.
 * \param wiboxes Wiboxes list to remove.
 * \param padding Padding.
 * \param strut Honor windows strut.
 * \return The screen area.
 */
area_t
screen_area_get(int screen, wibox_array_t *wiboxes,
                padding_t *padding, bool strut)
{
    area_t area = globalconf.screens[screen].geometry;
    uint16_t top = 0, bottom = 0, left = 0, right = 0;

    /* make padding corrections */
    if(padding)
    {
        area.x += padding->left;
        area.y += padding->top;
        area.width -= padding->left + padding->right;
        area.height -= padding->top + padding->bottom;
    }

    if(strut)
    {
        client_t *c;
        for(c = globalconf.clients; c; c = c->next)
            if(client_isvisible(c, screen))
            {
                if(c->strut.top_start_x || c->strut.top_end_x)
                {
                    if(c->strut.top)
                        top = MAX(top, c->strut.top);
                    else
                        top = MAX(top, (c->geometry.y - area.y) + c->geometry.height);
                }
                if(c->strut.bottom_start_x || c->strut.bottom_end_x)
                {
                    if(c->strut.bottom)
                        bottom = MAX(bottom, c->strut.bottom);
                    else
                        bottom = MAX(bottom, (area.y + area.height) - c->geometry.y);
                }
                if(c->strut.left_start_y || c->strut.left_end_y)
                {
                    if(c->strut.left)
                        left = MAX(left, c->strut.left);
                    else
                        left = MAX(left, (c->geometry.x - area.x) + c->geometry.width);
                }
                if(c->strut.right_start_y || c->strut.right_end_y)
                {
                    if(c->strut.right)
                        right = MAX(right, c->strut.right);
                    else
                        right = MAX(right, (area.x + area.width) - c->geometry.x);
                }
            }
    }


    if(wiboxes)
        for(int i = 0; i < wiboxes->len; i++)
        {
            wibox_t *w = wiboxes->tab[i];
            if(w->isvisible)
                switch(w->position)
                {
                  case Top:
                    top = MAX(top, (uint16_t) (w->sw.geometry.y - area.y) + w->sw.geometry.height + 2 * w->sw.border.width);
                    break;
                  case Bottom:
                    bottom = MAX(bottom, (uint16_t) (area.y + area.height) - w->sw.geometry.y);
                    break;
                  case Left:
                    left = MAX(left, (uint16_t) (w->sw.geometry.x - area.x) + w->sw.geometry.width + 2 * w->sw.border.width);
                    break;
                  case Right:
                    right = MAX(right, (uint16_t) (area.x + area.width) - w->sw.geometry.x);
                    break;
                  default:
                    break;
                }
        }

    area.x += left;
    area.y += top;
    area.width -= left + right;
    area.height -= top + bottom;

    return area;
}

/** Get display info.
 * \param phys_screen Physical screen number.
 * \param wiboxes The wiboxes.
 * \param padding Padding.
 * \return The display area.
 */
area_t
display_area_get(int phys_screen, wibox_array_t *wiboxes, padding_t *padding)
{
    xcb_screen_t *s = xutil_screen_get(globalconf.connection, phys_screen);
    area_t area = { .x = 0,
                    .y = 0,
                    .width = s->width_in_pixels,
                    .height = s->height_in_pixels };

    if(wiboxes)
        for(int i = 0; i < wiboxes->len; i++)
        {
            wibox_t *w = wiboxes->tab[i];
            area.y += w->position == Top ? w->sw.geometry.height : 0;
            area.height -= (w->position == Top || w->position == Bottom) ? w->sw.geometry.height : 0;
        }

    /* make padding corrections */
    if(padding)
    {
            area.x += padding->left;
            area.y += padding->top;
            area.width -= padding->left + padding->right;
            area.height -= padding->top + padding->bottom;
    }
    return area;
}

/** This returns the real X screen number for a logical
 * screen if Xinerama is active.
 * \param screen The logical screen.
 * \return The X screen.
 */
int
screen_virttophys(int screen)
{
    if(globalconf.xinerama_is_active)
        return globalconf.default_screen;
    return screen;
}

/** Move a client to a virtual screen.
 * \param c The client to move.
 * \param new_screen The destinatiuon screen number.
 * \param dotag Set to true if we also change tags.
 * \param doresize Set to true if we also move the client to the new x and
 *        y of the new screen.
 */
void
screen_client_moveto(client_t *c, int new_screen, bool dotag, bool doresize)
{
    int i, old_screen = c->screen;
    tag_array_t *old_tags = &globalconf.screens[old_screen].tags,
                *new_tags = &globalconf.screens[new_screen].tags;
    area_t from, to;
    bool wasvisible = client_isvisible(c, c->screen);

    if(new_screen == c->screen)
        return;

    c->screen = new_screen;

    if(c->titlebar)
        c->titlebar->screen = new_screen;

    if(dotag && !c->issticky)
    {
        /* remove old tags */
        for(i = 0; i < old_tags->len; i++)
            untag_client(c, old_tags->tab[i]);

        /* add new tags */
        for(i = 0; i < new_tags->len; i++)
            if(new_tags->tab[i]->selected)
                tag_client(c, new_tags->tab[i]);
    }

    if(wasvisible)
        globalconf.screens[old_screen].need_arrange = true;
    client_need_arrange(c);

    if (!doresize)
        return;

    from = screen_area_get(old_screen, NULL, NULL, false);
    to = screen_area_get(c->screen, NULL, NULL, false);

    area_t new_geometry = c->geometry;

    if(c->isfullscreen)
    {
        new_geometry = to;
        area_t new_f_geometry = c->geometries.fullscreen;

        new_f_geometry.x = to.x + new_f_geometry.x - from.x;
        new_f_geometry.y = to.y + new_f_geometry.y - from.x;

        /* resize the client's original geometry if it doesn't fit the screen */
        if (new_f_geometry.width > to.width)
            new_f_geometry.width = to.width;
        if (new_f_geometry.height > to.height)
            new_f_geometry.height = to.height;

        /* make sure the client is still on the screen */
        if (new_f_geometry.x + new_f_geometry.width > to.x + to.width)
            new_f_geometry.x = to.x + to.width - new_f_geometry.width;
        if (new_f_geometry.y + new_f_geometry.height > to.y + to.height)
            new_f_geometry.y = to.y + to.height - new_f_geometry.height;

        c->geometries.fullscreen = new_f_geometry;
    }
    else
    {
        new_geometry.x = to.x + new_geometry.x - from.x;
        new_geometry.y = to.y + new_geometry.y - from.y;

        /* resize the client if it doesn't fit the new screen */
        if(new_geometry.width > to.width)
           new_geometry.width = to.width;
        if(new_geometry.height > to.height)
           new_geometry.height = to.height;

        /* make sure the client is still on the screen */
        if(new_geometry.x + new_geometry.width > to.x + to.width)
           new_geometry.x = to.x + to.width - new_geometry.width;
        if(new_geometry.y + new_geometry.height > to.y + to.height)
           new_geometry.y = to.y + to.height - new_geometry.height;
    }
    /* move / resize the client */
    client_resize(c, new_geometry, false);
}

/** Screen module.
 * \param L The Lua VM state.
 * \return The number of elements pushed on stack.
 * \luastack
 * \lfield number The screen number, to get a screen.
 */
static int
luaA_screen_module_index(lua_State *L)
{
    int screen = luaL_checknumber(L, 2) - 1;

    luaA_checkscreen(screen);
    lua_pushlightuserdata(L, &globalconf.screens[screen]);
    return luaA_settype(L, "screen");
}

/** Get or set screen tags.
 * \param L The Lua VM state.
 * \return The number of elements pushed on stack.
 * \luastack
 * \lparam None or a table of tags to set to the screen.
 * The table must contains at least one tag.
 * \return A table with all screen tags.
 */
static int
luaA_screen_tags(lua_State *L)
{
    int i;
    screen_t *s = lua_touserdata(L, 1);

    if(!s)
        luaL_typerror(L, 1, "screen");

    if(lua_gettop(L) == 2)
    {
        tag_t **tag;

        luaA_checktable(L, 2);

        /* remove current tags */
        for(i = 0; i < s->tags.len; i++)
            s->tags.tab[i]->screen = SCREEN_UNDEF;

        tag_array_wipe(&s->tags);
        tag_array_init(&s->tags);

        s->need_arrange = true;

        /* push new tags */
        lua_pushnil(L);
        while(lua_next(L, 2))
        {
            tag = luaA_checkudata(L, -1, "tag");
            tag_append_to_screen(*tag, s);
            lua_pop(L, 1);
        }

        /* check there's at least one tag! */
        if(!s->tags.len)
        {
            luaA_warn(L, "screen %d has no tag, taking last resort action and adding default tag\n",
                      s->index);
            tag_append_to_screen(tag_new("default", sizeof("default") - 1), s);
            return 1;
        }
    }
    else
    {
        lua_newtable(L);
        for(i = 0; i < s->tags.len; i++)
        {
            luaA_tag_userdata_new(L, s->tags.tab[i]);
            lua_rawseti(L, -2, i + 1);
        }
    }

    return 1;
}

/** A screen.
 * \param L The Lua VM state.
 * \return The number of elements pushed on stack.
 * \luastack
 * \lfield coords The screen coordinates. Immutable.
 * \lfield workarea The screen workarea, i.e. without wiboxes.
 */
static int
luaA_screen_index(lua_State *L)
{
    size_t len;
    const char *buf;
    screen_t *s;

    if(luaA_usemetatable(L, 1, 2))
        return 1;

    buf = luaL_checklstring(L, 2, &len);
    s = lua_touserdata(L, 1);

    switch(a_tokenize(buf, len))
    {
      case A_TK_GEOMETRY:
        luaA_pusharea(L, s->geometry);
        break;
      case A_TK_WORKAREA:
        luaA_pusharea(L, screen_area_get(s->index, &s->wiboxes, &s->padding, true));
        break;
      default:
        return 0;
    }

    return 1;
}

/** Set or get the screen padding.
 * \param L The Lua VM state.
 * \return The number of elements pushed on stack.
 * \luastack
 * \lparam None or a table with new padding values.
 * \lreturn The screen padding. A table with top, right, left and bottom
 * keys and values in pixel.
 */
static int
luaA_screen_padding(lua_State *L)
{
    screen_t *s = lua_touserdata(L, 1);

    if(!s)
        luaL_typerror(L, 1, "screen");

    if(lua_gettop(L) == 2)
    {
        s->padding = luaA_getopt_padding(L, 2, &s->padding);

        s->need_arrange = true;

        /* All the wiboxes repositioned */
        for(int i = 0; i < s->wiboxes.len; i++)
            wibox_position_update(s->wiboxes.tab[i]);

        ewmh_update_workarea(screen_virttophys(s->index));
    }
    return luaA_pushpadding(L, &s->padding);
}

/** Get the screen count.
 * \param L The Lua VM state.
 * \return The number of elements pushed on stack.
 *
 * \luastack
 * \lreturn The screen count, at least 1.
 */
static int
luaA_screen_count(lua_State *L)
{
    lua_pushnumber(L, globalconf.nscreen);
    return 1;
}

const struct luaL_reg awesome_screen_methods[] =
{
    { "count", luaA_screen_count },
    { "__index", luaA_screen_module_index },
    { NULL, NULL }
};

const struct luaL_reg awesome_screen_meta[] =
{
    { "tags", luaA_screen_tags },
    { "padding", luaA_screen_padding },
    { "__index", luaA_screen_index },
    { NULL, NULL }
};

// vim: filetype=c:expandtab:shiftwidth=4:tabstop=8:softtabstop=4:encoding=utf-8:textwidth=80
