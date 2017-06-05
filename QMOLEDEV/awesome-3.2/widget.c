/*
 * widget.c - widget managing
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

#include <math.h>

#include <xcb/xcb.h>
#include <xcb/xcb_atom.h>

#include "mouse.h"
#include "widget.h"
#include "wibox.h"
#include "common/atoms.h"

extern awesome_t globalconf;

DO_LUA_NEW(extern, widget_t, widget, "widget", widget_ref)
DO_LUA_GC(widget_t, widget, "widget", widget_unref)
DO_LUA_EQ(widget_t, widget, "widget")

#include "widgetgen.h"

/** Delete a widget structure.
 * \param widget The widget to destroy.
 */
void
widget_delete(widget_t **widget)
{
    if((*widget)->destructor)
        (*widget)->destructor(*widget);
    button_array_wipe(&(*widget)->buttons);
    luaL_unref(globalconf.L, LUA_REGISTRYINDEX, (*widget)->mouse_enter);
    luaL_unref(globalconf.L, LUA_REGISTRYINDEX, (*widget)->mouse_leave);
    p_delete(widget);
}

/** Compute offset for drawing the first pixel of a widget.
 * \param barwidth The wibox width.
 * \param widgetwidth The widget width.
 * \param alignment The widget alignment on wibox.
 * \return The x coordinate to draw at.
 */
int
widget_calculate_offset(int barwidth, int widgetwidth, int offset, int alignment)
{
    switch(alignment)
    {
      case AlignLeft:
      case AlignFlex:
      case AlignFixed:
        return offset;
    }
    return barwidth - offset - widgetwidth;
}

/** Get a widget node from a wibox by coords.
 * \param Container position.
 * \param widgets The widget list.
 * \param width The container width.
 * \param height The container height.
 * \param x X coordinate of the widget.
 * \param y Y coordinate of the widget.
 * \return A widget.
 */
widget_t *
widget_getbycoords(position_t position, widget_node_array_t *widgets,
                   int width, int height, int16_t *x, int16_t *y)
{
    int tmp;

    /* Need to transform coordinates like it was top/bottom */
    switch(position)
    {
      case Right:
        tmp = *y;
        *y = width - *x;
        *x = tmp;
        break;
      case Left:
        tmp = *y;
        *y = *x;
        *x = height - tmp;
        break;
      default:
        break;
    }
    for(int i = 0; i < widgets->len; i++)
    {
        widget_node_t *w = &widgets->tab[i];
        if(w->widget->isvisible &&
           *x >= w->geometry.x && *x < w->geometry.x + w->geometry.width
           && *y >= w->geometry.y && *y < w->geometry.y + w->geometry.height)
            return w->widget;
    }

    return NULL;
}

/** Convert a Lua table to a list of widget nodet.
 * \param L The Lua VM state.
 * \param widgets The linked list of widget node.
 */
void
luaA_table2widgets(lua_State *L, widget_node_array_t *widgets)
{
    if(lua_istable(L, -1))
    {
        lua_pushnil(L);
        while(luaA_next(L, -2))
        {
            luaA_table2widgets(L, widgets);
            lua_pop(L, 1); /* remove value */
        }
    }
    else
    {
        widget_t **widget = luaA_toudata(L, -1, "widget");
        if(widget)
        {
            widget_node_t w;
            p_clear(&w, 1);
            w.widget = widget_ref(widget);
            widget_node_array_append(widgets, w);
        }
    }
}

/** Render a list of widgets.
 * \param wibox The wibox.
 * \todo Remove GC.
 */
void
widget_render(wibox_t *wibox)
{
    draw_context_t *ctx = &wibox->sw.ctx;
    int left = 0, right = 0;
    area_t rectangle = { 0, 0, 0, 0 };

    rectangle.width = ctx->width;
    rectangle.height = ctx->height;

    if(ctx->bg.alpha != 0xffff)
    {
        int x = wibox->sw.geometry.x, y = wibox->sw.geometry.y;
        xcb_get_property_reply_t *prop_r;
        char *data;
        xcb_pixmap_t rootpix;
        xcb_get_property_cookie_t prop_c;
        xcb_screen_t *s = xutil_screen_get(globalconf.connection, ctx->phys_screen);
        prop_c = xcb_get_property_unchecked(globalconf.connection, false, s->root, _XROOTPMAP_ID,
                                            PIXMAP, 0, 1);
        if((prop_r = xcb_get_property_reply(globalconf.connection, prop_c, NULL)))
        {
            if(prop_r->value_len
               && (data = xcb_get_property_value(prop_r))
               && (rootpix = *(xcb_pixmap_t *) data))
               switch(wibox->sw.orientation)
               {
                 case North:
                   draw_rotate(ctx,
                               rootpix, ctx->pixmap,
                               s->width_in_pixels, s->height_in_pixels,
                               ctx->width, ctx->height,
                               M_PI_2,
                               y + ctx->width,
                               - x);
                   break;
                 case South:
                   draw_rotate(ctx,
                               rootpix, ctx->pixmap,
                               s->width_in_pixels, s->height_in_pixels,
                               ctx->width, ctx->height,
                               - M_PI_2,
                               - y,
                               x + ctx->height);
                   break;
                 case East:
                   xcb_copy_area(globalconf.connection, rootpix,
                                 wibox->sw.pixmap, wibox->sw.gc,
                                 x, y,
                                 0, 0,
                                 ctx->width, ctx->height);
                   break;
               }
            p_delete(&prop_r);
        }
    }

    widget_node_array_t *widgets = &wibox->widgets;

    /* compute geometry */
    for(int i = 0; i < widgets->len; i++)
        if(widgets->tab[i].widget->align == AlignLeft && widgets->tab[i].widget->isvisible)
        {
            widgets->tab[i].geometry = widgets->tab[i].widget->geometry(widgets->tab[i].widget,
                                                                        wibox->screen, ctx->height,
                                                                        ctx->width - (left + right));
            widgets->tab[i].geometry.x = left;
            left += widgets->tab[i].geometry.width;
        }

    for(int i = widgets->len - 1; i >= 0; i--)
        if(widgets->tab[i].widget->align == AlignRight && widgets->tab[i].widget->isvisible)
        {
            widgets->tab[i].geometry = widgets->tab[i].widget->geometry(widgets->tab[i].widget,
                                                                        wibox->screen, ctx->height,
                                                                        ctx->width - (left + right));
            right += widgets->tab[i].geometry.width;
            widgets->tab[i].geometry.x = ctx->width - right;
        }

    /* save left value */
    int fake_left = left;

    /* compute width of flex or fixed aligned widgets */
    int flex = 0;
    for(int i = 0; i < widgets->len; i++)
        if(widgets->tab[i].widget->align & (AlignFlex | AlignFixed)
          && widgets->tab[i].widget->isvisible)
        {
            if(widgets->tab[i].widget->align_supported & AlignFlex
              && widgets->tab[i].widget->align == AlignFlex)
                flex++;
            else
                fake_left += widgets->tab[i].widget->geometry(widgets->tab[i].widget,
                                                              wibox->screen, ctx->height,
                                                              ctx->width - (fake_left + right)).width;
        }

    /* now compute everybody together! */
    int flex_rendered = 0;
    for(int i = 0; i < widgets->len; i++)
        if(widgets->tab[i].widget->align & (AlignFlex | AlignFixed)
          && widgets->tab[i].widget->isvisible)
        {
            if(widgets->tab[i].widget->align_supported & AlignFlex
              && widgets->tab[i].widget->align == AlignFlex)
            {
                int width = (ctx->width - (right + fake_left)) / flex;
                /* give last pixels to last flex to be rendered */
                if(flex_rendered == flex - 1)
                    width += (ctx->width - (right + fake_left)) % flex;
                widgets->tab[i].geometry = widgets->tab[i].widget->geometry(widgets->tab[i].widget,
                                                                            wibox->screen, ctx->height,
                                                                            width);
                flex_rendered++;
            }
            else
                widgets->tab[i].geometry = widgets->tab[i].widget->geometry(widgets->tab[i].widget,
                                                                            wibox->screen, ctx->height,
                                                                            ctx->width - (left + right));
            widgets->tab[i].geometry.x = left;
            left += widgets->tab[i].geometry.width;
        }

    /* draw everything! */
    draw_rectangle(ctx, rectangle, 1.0, true, &ctx->bg);

    for(int i = 0; i < widgets->len; i++)
        if(widgets->tab[i].widget->isvisible)
        {
            widgets->tab[i].geometry.y = 0;
            widgets->tab[i].widget->draw(widgets->tab[i].widget,
                                         ctx, widgets->tab[i].geometry,
                                         wibox->screen, wibox);
        }

    switch(wibox->sw.orientation)
    {
        case South:
          draw_rotate(ctx, ctx->pixmap, wibox->sw.pixmap,
                      ctx->width, ctx->height,
                      ctx->height, ctx->width,
                      M_PI_2, ctx->height, 0);
          break;
        case North:
          draw_rotate(ctx, ctx->pixmap, wibox->sw.pixmap,
                      ctx->width, ctx->height,
                      ctx->height, ctx->width,
                      - M_PI_2, 0, ctx->width);
          break;
        case East:
          break;
    }
}

/** Invalidate widgets which should be refresh depending on their types.
 * \param screen Virtual screen number.
 * \param type Widget type to invalidate.
 */
void
widget_invalidate_bytype(int screen, widget_constructor_t *type)
{
    for(int i = 0; i < globalconf.screens[screen].wiboxes.len; i++)
    {
        wibox_t *wibox = globalconf.screens[screen].wiboxes.tab[i];

        for(int j = 0; j < wibox->widgets.len; j++)
            if(wibox->widgets.tab[j].widget->type == type)
            {
                wibox->need_update = true;
                break;
            }
    }
}

/** Set a wibox needs update because it has widget, or redraw a titlebar.
 * \todo Probably needs more optimization.
 * \param widget The widget to look for.
 */
void
widget_invalidate_bywidget(widget_t *widget)
{
    for(int screen = 0; screen < globalconf.nscreen; screen++)
        for(int i = 0; i < globalconf.screens[screen].wiboxes.len; i++)
        {
            wibox_t *wibox = globalconf.screens[screen].wiboxes.tab[i];
            if(!wibox->need_update)
                for(int j = 0; j < wibox->widgets.len; j++)
                    if(wibox->widgets.tab[j].widget == widget)
                    {
                        wibox->need_update = true;
                        break;
                    }
         }

    for(client_t *c = globalconf.clients; c; c = c->next)
        if(c->titlebar && !c->titlebar->need_update)
            for(int j = 0; j < c->titlebar->widgets.len; j++)
                if(c->titlebar->widgets.tab[j].widget == widget)
                {
                    c->titlebar->need_update = true;
                    break;
                }
}

/** Create a new widget.
 * \param L The Lua VM state.
 *
 * \luastack
 * \lparam A table with at least a type value. Optional attributes
 * are: align.
 * \lreturn A brand new widget.
 */
static int
luaA_widget_new(lua_State *L)
{
    const char *align, *type;
    widget_t *w;
    widget_constructor_t *wc;
    size_t len;

    luaA_checktable(L, 2);

    type = luaA_getopt_lstring(L, 2, "type", NULL, &len);

    if((wc = name_func_lookup(type, len, WidgetList)))
    {
        w = p_new(widget_t, 1);
        wc(w);
    }
    else
    {
        luaA_warn(L, "unkown widget type: %s", type);
        return 0;
    }

    w->type = wc;

    align = luaA_getopt_lstring(L, 2, "align", "left", &len);
    w->align_supported |= AlignLeft | AlignRight | AlignFixed;
    w->align = draw_align_fromstr(align, len);

    /* Set visible by default. */
    w->isvisible = true;

    w->mouse_enter = w->mouse_leave = LUA_REFNIL;

    return luaA_widget_userdata_new(L, w);
}

/** Get or set mouse buttons bindings to a widget.
 * \param L The Lua VM state.
 *
 * \luastack
 * \lvalue A widget.
 * \lparam An array of mouse button bindings objects, or nothing.
 * \return The array of mouse button bindings objects of this widget.
 */
static int
luaA_widget_buttons(lua_State *L)
{
    widget_t **widget = luaA_checkudata(L, 1, "widget");
    button_array_t *buttons = &(*widget)->buttons;

    if(lua_gettop(L) == 2)
    {
        luaA_button_array_set(L, 2, buttons);
        return 1;
    }

    return luaA_button_array_get(L, buttons);
}

/** Generic widget.
 * \param L The Lua VM state.
 * \return The number of elements pushed on stack.
 * \luastack
 * \lfield align The widget alignment.
 * \lfield visible The widget visibility.
 * \lfield mouse_enter A function to execute when the mouse enter the widget.
 * \lfield mouse_leave A function to execute when the mouse leave the widget.
 */
static int
luaA_widget_index(lua_State *L)
{
    size_t len;
    widget_t **widget = luaA_checkudata(L, 1, "widget");
    const char *buf = luaL_checklstring(L, 2, &len);
    awesome_token_t token;

    if(luaA_usemetatable(L, 1, 2))
        return 1;

    switch((token = a_tokenize(buf, len)))
    {
      case A_TK_ALIGN:
        lua_pushstring(L, draw_align_tostr((*widget)->align));
        return 1;
      case A_TK_VISIBLE:
        lua_pushboolean(L, (*widget)->isvisible);
        return 1;
      case A_TK_MOUSE_ENTER:
        if((*widget)->mouse_enter != LUA_REFNIL)
            lua_rawgeti(L, LUA_REGISTRYINDEX, (*widget)->mouse_enter);
        else
            return 0;
        return 1;
      case A_TK_MOUSE_LEAVE:
        if((*widget)->mouse_leave != LUA_REFNIL)
            lua_rawgeti(L, LUA_REGISTRYINDEX, (*widget)->mouse_leave);
        else
            return 0;
        return 1;
      default:
        break;
    }

    return (*widget)->index ? (*widget)->index(L, token) : 0;
}

/** Generic widget newindex.
 * \param L The Lua VM state.
 * \return The number of elements pushed on stack.
 */
static int
luaA_widget_newindex(lua_State *L)
{
    size_t len;
    widget_t **widget = luaA_checkudata(L, 1, "widget");
    const char *buf = luaL_checklstring(L, 2, &len);
    awesome_token_t token;

    switch((token = a_tokenize(buf, len)))
    {
      case A_TK_ALIGN:
        buf = luaL_checklstring(L, 3, &len);
        (*widget)->align = draw_align_fromstr(buf, len);
        break;
      case A_TK_VISIBLE:
        (*widget)->isvisible = luaA_checkboolean(L, 3);
        break;
      case A_TK_MOUSE_ENTER:
        luaA_registerfct(L, 3, &(*widget)->mouse_enter);
        return 0;
      case A_TK_MOUSE_LEAVE:
        luaA_registerfct(L, 3, &(*widget)->mouse_leave);
        return 0;
      default:
        return (*widget)->newindex ? (*widget)->newindex(L, token) : 0;
    }

    widget_invalidate_bywidget(*widget);

    return 0;
}

const struct luaL_reg awesome_widget_methods[] =
{
    { "__call", luaA_widget_new },
    { NULL, NULL }
};
const struct luaL_reg awesome_widget_meta[] =
{
    { "buttons", luaA_widget_buttons },
    { "__index", luaA_widget_index },
    { "__newindex", luaA_widget_newindex },
    { "__gc", luaA_widget_gc },
    { "__eq", luaA_widget_eq },
    { "__tostring", luaA_widget_tostring },
    { NULL, NULL }
};

// vim: filetype=c:expandtab:shiftwidth=4:tabstop=8:softtabstop=4:encoding=utf-8:textwidth=80
