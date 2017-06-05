/*
 * key.c - Key bindings configuration management
 *
 * Copyright © 2008 Julien Danjou <julien@danjou.info>
 * Copyright © 2008 Pierre Habouzit <madcoder@debian.org>
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

/* XStringToKeysym() */
#include <X11/Xlib.h>

#include "structs.h"

extern awesome_t globalconf;

static void
key_delete(keyb_t **kbp)
{
    luaL_unref(globalconf.L, LUA_REGISTRYINDEX, (*kbp)->press);
    luaL_unref(globalconf.L, LUA_REGISTRYINDEX, (*kbp)->release);
    p_delete(kbp);
}

DO_RCNT(keyb_t, key, key_delete)
ARRAY_FUNCS(keyb_t *, key, key_unref)
DO_LUA_NEW(static, keyb_t, key, "key", key_ref)
DO_LUA_GC(keyb_t, key, "key", key_unref)

static void
keybindings_init(keybindings_t *kb)
{
    key_array_init(&kb->by_code);
    key_array_init(&kb->by_sym);
}

static void
keybindings_wipe(keybindings_t *kb)
{
    key_array_wipe(&kb->by_code);
    key_array_wipe(&kb->by_sym);
}

static int
key_ev_cmp(xcb_keysym_t keysym, xcb_keycode_t keycode,
                  unsigned long mod, const keyb_t *k)
{
    if (k->keysym) {
        if (k->keysym != keysym)
            return k->keysym > keysym ? 1 : -1;
    }
    if (k->keycode) {
        if (k->keycode != keycode)
            return k->keycode > keycode ? 1 : -1;
    }
    return k->mod == mod ? 0 : (k->mod > mod ? 1 : -1);
}

static int
key_cmp(const keyb_t *k1, const keyb_t *k2)
{
    assert ((k1->keysym && k2->keysym) || (k1->keycode && k2->keycode));
    assert ((!k1->keysym && !k2->keysym) || (!k1->keycode && !k2->keycode));

    if (k1->keysym != k2->keysym)
        return k2->keysym > k1->keysym ? 1 : -1;
    if (k1->keycode != k2->keycode)
        return k2->keycode > k1->keycode ? 1 : -1;
    return k1->mod == k2->mod ? 0 : (k2->mod > k1->mod ? 1 : -1);
}

/** Grab key on a window.
 * \param win The window.
 * \param k The key.
 */
static void
window_grabkey(xcb_window_t win, keyb_t *k)
{
    xcb_keycode_t kc;

    if((kc = k->keycode)
       || (k->keysym
           && (kc = xcb_key_symbols_get_keycode(globalconf.keysyms, k->keysym))))
    {
        xcb_grab_key(globalconf.connection, true, win,
                     k->mod, kc, XCB_GRAB_MODE_ASYNC, XCB_GRAB_MODE_ASYNC);
        xcb_grab_key(globalconf.connection, true, win,
                     k->mod | XCB_MOD_MASK_LOCK, kc, XCB_GRAB_MODE_ASYNC, XCB_GRAB_MODE_ASYNC);
        xcb_grab_key(globalconf.connection, true, win,
                     k->mod | globalconf.numlockmask, kc, XCB_GRAB_MODE_ASYNC, XCB_GRAB_MODE_ASYNC);
        xcb_grab_key(globalconf.connection, true, win,
                     k->mod | globalconf.numlockmask | XCB_MOD_MASK_LOCK, kc, XCB_GRAB_MODE_ASYNC,
                     XCB_GRAB_MODE_ASYNC);
    }
}

void
window_grabkeys(xcb_window_t win, keybindings_t *keys)
{
    for(int i = 0; i < keys->by_code.len; i++)
        window_grabkey(win, keys->by_code.tab[i]);

    for(int i = 0; i < keys->by_sym.len; i++)
        window_grabkey(win, keys->by_sym.tab[i]);
}

static void
key_register(keybindings_t *keys, keyb_t *k)
{
    key_array_t *arr = k->keysym ? &keys->by_sym : &keys->by_code;
    int l = 0, r = arr->len;

    key_ref(&k);

    while (l < r) {
        int i = (r + l) / 2;
        switch (key_cmp(k, arr->tab[i])) {
          case -1: /* k < arr->tab[i] */
            r = i;
            break;
          case 0: /* k == arr->tab[i] */
            key_unref(&arr->tab[i]);
            arr->tab[i] = k;
            return;
          case 1: /* k > arr->tab[i] */
            l = i + 1;
            break;
        }
    }

    key_array_splice(arr, r, 0, &k, 1);
}

/** Return the keysym from keycode.
 * \param detail The keycode received.
 * \param state The modifier state.
 * \return A keysym.
 */
xcb_keysym_t
key_getkeysym(xcb_keycode_t detail, uint16_t state)
{
    xcb_keysym_t k0, k1;

    /* 'col' (third parameter) is used to get the proper KeySym
     * according to modifier (XCB doesn't provide an equivalent to
     * XLookupString()).
     *
     * If Mod5 is ON we look into second group.
     */
    if(state & XCB_MOD_MASK_5)
    {
        k0 = xcb_key_symbols_get_keysym(globalconf.keysyms, detail, 2);
        k1 = xcb_key_symbols_get_keysym(globalconf.keysyms, detail, 3);
    }
    else
    {
        k0 = xcb_key_symbols_get_keysym(globalconf.keysyms, detail, 0);
        k1 = xcb_key_symbols_get_keysym(globalconf.keysyms, detail, 1);
    }

    /* If the second column does not exists use the first one. */
    if(k1 == XCB_NONE)
        k1 = k0;

    /* The numlock modifier is on and the second KeySym is a keypad
     * KeySym */
    if((state & globalconf.numlockmask) && xcb_is_keypad_key(k1))
    {
        /* The Shift modifier is on, or if the Lock modifier is on and
         * is interpreted as ShiftLock, use the first KeySym */
        if((state & XCB_MOD_MASK_SHIFT) ||
           (state & XCB_MOD_MASK_LOCK && (state & globalconf.shiftlockmask)))
            return k0;
        else
            return k1;
    }

    /* The Shift and Lock modifers are both off, use the first
     * KeySym */
    else if(!(state & XCB_MOD_MASK_SHIFT) && !(state & XCB_MOD_MASK_LOCK))
        return k0;

    /* The Shift modifier is off and the Lock modifier is on and is
     * interpreted as CapsLock */
    else if(!(state & XCB_MOD_MASK_SHIFT) &&
            (state & XCB_MOD_MASK_LOCK && (state & globalconf.capslockmask)))
        /* The first Keysym is used but if that KeySym is lowercase
         * alphabetic, then the corresponding uppercase KeySym is used
         * instead */
        return k1;

    /* The Shift modifier is on, and the Lock modifier is on and is
     * interpreted as CapsLock */
    else if((state & XCB_MOD_MASK_SHIFT) &&
            (state & XCB_MOD_MASK_LOCK && (state & globalconf.capslockmask)))
        /* The second Keysym is used but if that KeySym is lowercase
         * alphabetic, then the corresponding uppercase KeySym is used
         * instead */
        return k1;

    /* The Shift modifer is on, or the Lock modifier is on and is
     * interpreted as ShiftLock, or both */
    else if((state & XCB_MOD_MASK_SHIFT) ||
            (state & XCB_MOD_MASK_LOCK && (state & globalconf.shiftlockmask)))
        return k1;

    return XCB_NO_SYMBOL;
}


keyb_t *
key_find(keybindings_t *keys, const xcb_key_press_event_t *ev)
{
    const key_array_t *arr = &keys->by_sym;
    int l, r, mod = XUTIL_MASK_CLEAN(ev->state);
    xcb_keysym_t keysym;

    /* get keysym ignoring shift and mod5 */
    keysym = key_getkeysym(ev->detail, ev->state & ~(XCB_MOD_MASK_SHIFT | XCB_MOD_MASK_5 | XCB_MOD_MASK_LOCK));

  again:
    l = 0;
    r = arr->len;
    while (l < r)
    {
        int i = (r + l) / 2;
        switch (key_ev_cmp(keysym, ev->detail, mod, arr->tab[i]))
        {
          case -1: /* ev < arr->tab[i] */
            r = i;
            break;
          case 0: /* ev == arr->tab[i] */
            return arr->tab[i];
          case 1: /* ev > arr->tab[i] */
            l = i + 1;
            break;
        }
    }
    if(arr != &keys->by_code)
    {
        arr = &keys->by_code;
        goto again;
    }
    return NULL;
}

static void
luaA_keystore(keyb_t *key, const char *str, ssize_t len)
{
    if(len)
    {
        if(*str != '#')
        {
            key->keysym = XStringToKeysym(str);
            if (!key->keysym)
            {
                if (len == 1)
                    key->keysym = *str;
                else
                    warn("there's no keysym named \"%s\"", str);
            }
        }
        else
            key->keycode = atoi(str + 1);
    }
}

/** Define a global key binding. This key binding will always be available.
 * \param L The Lua VM state.
 *
 * \luastack
 * \lparam A table with modifier keys.
 * \lparam A key name.
 * \lparam A function to execute on key press.
 * \lparam A function to execute on key release.
 * \lreturn The key.
 */
int
luaA_key_new(lua_State *L)
{
    size_t i, len;
    keyb_t *k;
    const char *key;
    luaA_ref press = LUA_REFNIL, release = LUA_REFNIL;

    /* arg 2 is key mod table */
    luaA_checktable(L, 2);
    /* arg 3 is key */
    key = luaL_checklstring(L, 3, &len);

    if(!lua_isnil(L, 4))
        luaA_registerfct(L, 4, &press);

    if(lua_gettop(L) == 5 && !lua_isnil(L, 5))
        luaA_registerfct(L, 5, &release);

    /* get the last arg as function */
    k = p_new(keyb_t, 1);
    luaA_keystore(k, key, len);

    k->press = press;
    k->release = release;

    len = lua_objlen(L, 2);
    for(i = 1; i <= len; i++)
    {
        size_t blen;
        lua_rawgeti(L, 2, i);
        key = luaL_checklstring(L, -1, &blen);
        k->mod |= xutil_key_mask_fromstr(key, blen);
    }

    return luaA_key_userdata_new(L, k);
}

/** Set a key array with a Lua table.
 * \param L The Lua VM state.
 * \param idx The index of the Lua table.
 * \param keys The array key to fill.
 */
void
luaA_key_array_set(lua_State *L, int idx, keybindings_t *keys)
{
    luaA_checktable(L, idx);

    keybindings_wipe(keys);
    keybindings_init(keys);

    lua_pushnil(L);
    while(lua_next(L, idx))
    {
        keyb_t **k = luaA_checkudata(L, -1, "key");
        key_register(keys, *k);
        lua_pop(L, 1);
    }
}

/** Push an array of key as an Lua table onto the stack.
 * \param L The Lua VM state.
 * \param keys The key array to push.
 * \return The number of elements pushed on stack.
 */
int
luaA_key_array_get(lua_State *L, keybindings_t *keys)
{
    luaA_otable_new(L);
    for(int i = 0; i < keys->by_code.len; i++)
    {
        luaA_key_userdata_new(L, keys->by_code.tab[i]);
        lua_rawseti(L, -2, i + 1);
    }
    for(int i = 0; i < keys->by_sym.len; i++)
    {
        luaA_key_userdata_new(L, keys->by_sym.tab[i]);
        lua_rawseti(L, -2, i + 1);
    }
    return 1;
}

/** Add a global key binding. This key binding will always be available.
 * \param L The Lua VM state.
 *
 * \luastack
 * \lvalue A key.
 */
static int
luaA_key_add(lua_State *L)
{
    luaA_deprecate(L, "root.keys");
    keyb_t **k = luaA_checkudata(L, 1, "key");
    int nscreen = xcb_setup_roots_length(xcb_get_setup(globalconf.connection));

    key_register(&globalconf.keys, *k);

    for(int phys_screen = 0; phys_screen < nscreen; phys_screen++)
    {
        xcb_screen_t *s = xutil_screen_get(globalconf.connection, phys_screen);
        xcb_ungrab_key(globalconf.connection, XCB_GRAB_ANY, s->root, XCB_BUTTON_MASK_ANY);
        window_grabkeys(s->root, &globalconf.keys);
    }

    return 0;
}

/** Remove a global key binding.
 * \param L The Lua VM state.
 *
 * \luastack
 * \lvalue A key.
 */
static int
luaA_key_remove(lua_State *L)
{
    luaA_deprecate(L, "root.keys");
    return 0;
}

const struct luaL_reg awesome_key_methods[] =
{
    { "__call", luaA_key_new },
    { NULL, NULL }
};
const struct luaL_reg awesome_key_meta[] =
{
    { "add", luaA_key_add },
    { "remove", luaA_key_remove },
    { "__tostring", luaA_key_tostring },
    { "__gc", luaA_key_gc },
    { NULL, NULL },
};

