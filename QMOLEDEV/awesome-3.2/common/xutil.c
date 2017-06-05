/*
 * common/xutil.c - X-related useful functions
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

#include "common/util.h"

#include <xcb/xcb.h>
#include <xcb/xcb_atom.h>
#include <xcb/xcb_icccm.h>

#include "common/xutil.h"
#include "common/atoms.h"
#include "common/tokenize.h"

/** Get the string value of an atom.
 * \param conn X connection.
 * \param w Window.
 * \param atom The atom.
 * \param text Buffer to fill.
 * \param len Length of the filled buffer.
 * \return True on sucess, false on failure.
 */
bool
xutil_text_prop_get(xcb_connection_t *conn, xcb_window_t w, xcb_atom_t atom,
                    char **text, ssize_t *len)
{
    xcb_get_text_property_reply_t reply;

    p_clear(&reply, 1);

    if(!xcb_get_text_property_reply(conn,
                                    xcb_get_text_property_unchecked(conn, w,
                                                                    atom),
                                    &reply, NULL) ||
       !reply.name_len || reply.format != 8)
    {
        xcb_get_text_property_reply_wipe(&reply);
        return false;
    }

    if(text && len)
    {
        /* Check whether the returned property value is just an ascii
         * string, an UTF-8 string or just some random multibyte in any other
         * encoding. */
        if(reply.encoding == STRING
           || reply.encoding == UTF8_STRING
           || reply.encoding == COMPOUND_TEXT)
        {
            *text = p_new(char, reply.name_len + 1);
            /* Use memcpy() because the property name is not be \0
             * terminated */
            memcpy(*text, reply.name, reply.name_len);
            (*text)[reply.name_len] = '\0';
            *len = reply.name_len;
        }
        else
        {
            *text = NULL;
            *len = 0;
        }
    }

    xcb_get_text_property_reply_wipe(&reply);
    return true;
}

/** Get the lock masks (shiftlock, numlock, capslock).
 * \param connection The X connection.
 * \param cookie The cookie of the request.
 * \param keysyms Key symbols.
 * \param numlockmask Numlock mask.
 * \param shiftlockmask Shiftlock mask.
 * \param capslockmask Capslock mask.
 */
void
xutil_lock_mask_get(xcb_connection_t *connection,
                    xcb_get_modifier_mapping_cookie_t cookie,
                    xcb_key_symbols_t *keysyms,
                    unsigned int *numlockmask,
                    unsigned int *shiftlockmask,
                    unsigned int *capslockmask)
{
    xcb_get_modifier_mapping_reply_t *modmap_r;
    xcb_keycode_t *modmap, kc;
    unsigned int mask;
    int i, j;

    modmap_r = xcb_get_modifier_mapping_reply(connection, cookie, NULL);
    modmap = xcb_get_modifier_mapping_keycodes(modmap_r);

    for(i = 0; i < 8; i++)
        for(j = 0; j < modmap_r->keycodes_per_modifier; j++)
        {
            kc = modmap[i * modmap_r->keycodes_per_modifier + j];
            mask = (1 << i);

            if(numlockmask != NULL
               && kc == xcb_key_symbols_get_keycode(keysyms, XK_Num_Lock))
                *numlockmask = mask;
            else if(shiftlockmask != NULL
                    && kc == xcb_key_symbols_get_keycode(keysyms, XK_Shift_Lock))
                *shiftlockmask = mask;
            else if(capslockmask != NULL
                    && kc == xcb_key_symbols_get_keycode(keysyms, XK_Caps_Lock))
                *capslockmask = mask;
        }

    p_delete(&modmap_r);
}

/* Number of different errors */
#define ERRORS_NBR 256

void
xutil_error_handler_catch_all_set(xcb_event_handlers_t *evenths,
                                  xcb_generic_error_handler_t handler,
                                  void *data)
{
    int err_num;
    for(err_num = 0; err_num < ERRORS_NBR; err_num++)
	xcb_event_set_error_handler(evenths, err_num, handler, data);
}

const char *
xutil_label_error[] =
{
    "Success",
    "BadRequest",
    "BadValue",
    "BadWindow",
    "BadPixmap",
    "BadAtom",
    "BadCursor",
    "BadFont",
    "BadMatch",
    "BadDrawable",
    "BadAccess",
    "BadAlloc",
    "BadColor",
    "BadGC",
    "BadIDChoice",
    "BadName",
    "BadLength",
    "BadImplementation",
};

const char *
xutil_label_request[] =
{
    "None",
    "CreateWindow",
    "ChangeWindowAttributes",
    "GetWindowAttributes",
    "DestroyWindow",
    "DestroySubwindows",
    "ChangeSaveSet",
    "ReparentWindow",
    "MapWindow",
    "MapSubwindows",
    "UnmapWindow",
    "UnmapSubwindows",
    "ConfigureWindow",
    "CirculateWindow",
    "GetGeometry",
    "QueryTree",
    "InternAtom",
    "GetAtomName",
    "ChangeProperty",
    "DeleteProperty",
    "GetProperty",
    "ListProperties",
    "SetSelectionOwner",
    "GetSelectionOwner",
    "ConvertSelection",
    "SendEvent",
    "GrabPointer",
    "UngrabPointer",
    "GrabButton",
    "UngrabButton",
    "ChangeActivePointerGrab",
    "GrabKeyboard",
    "UngrabKeyboard",
    "GrabKey",
    "UngrabKey",
    "AllowEvents",
    "GrabServer",
    "UngrabServer",
    "QueryPointer",
    "GetMotionEvents",
    "TranslateCoords",
    "WarpPointer",
    "SetInputFocus",
    "GetInputFocus",
    "QueryKeymap",
    "OpenFont",
    "CloseFont",
    "QueryFont",
    "QueryTextExtents",
    "ListFonts",
    "ListFontsWithInfo",
    "SetFontPath",
    "GetFontPath",
    "CreatePixmap",
    "FreePixmap",
    "CreateGC",
    "ChangeGC",
    "CopyGC",
    "SetDashes",
    "SetClipRectangles",
    "FreeGC",
    "ClearArea",
    "CopyArea",
    "CopyPlane",
    "PolyPoint",
    "PolyLine",
    "PolySegment",
    "PolyRectangle",
    "PolyArc",
    "FillPoly",
    "PolyFillRectangle",
    "PolyFillArc",
    "PutImage",
    "GetImage",
    "PolyText",
    "PolyText",
    "ImageText",
    "ImageText",
    "CreateColormap",
    "FreeColormap",
    "CopyColormapAndFree",
    "InstallColormap",
    "UninstallColormap",
    "ListInstalledColormaps",
    "AllocColor",
    "AllocNamedColor",
    "AllocColorCells",
    "AllocColorPlanes",
    "FreeColors",
    "StoreColors",
    "StoreNamedColor",
    "QueryColors",
    "LookupColor",
    "CreateCursor",
    "CreateGlyphCursor",
    "FreeCursor",
    "RecolorCursor",
    "QueryBestSize",
    "QueryExtension",
    "ListExtensions",
    "ChangeKeyboardMapping",
    "GetKeyboardMapping",
    "ChangeKeyboardControl",
    "GetKeyboardControl",
    "Bell",
    "ChangePointerControl",
    "GetPointerControl",
    "SetScreenSaver",
    "GetScreenSaver",
    "ChangeHosts",
    "ListHosts",
    "SetAccessControl",
    "SetCloseDownMode",
    "KillClient",
    "RotateProperties",
    "ForceScreenSaver",
    "SetPointerMapping",
    "GetPointerMapping",
    "SetModifierMapping",
    "GetModifierMapping",
    "major 120",
    "major 121",
    "major 122",
    "major 123",
    "major 124",
    "major 125",
    "major 126",
    "NoOperation",
};

bool
xutil_error_init(const xcb_generic_error_t *e, xutil_error_t *err)
{
    if(e->response_type != 0)
        /* This is not an error, this _should_ not happen */
        return false;

    /*
     * Get the request code,  taken from 'xcb-util/wm'. I can't figure
     * out  how it  works BTW,  seems to  get a  byte in  'pad' member
     * (second byte in second element of the array)
     */
    err->request_code = *((uint8_t *) e + 10);

    /* Extensions  generally provide  their  own requests  so we  just
     * store the request code */
    if(err->request_code >= (sizeof(xutil_label_request) / sizeof(char *)))
        a_asprintf(&err->request_label, "%d", err->request_code);
    else
        err->request_label = a_strdup(xutil_label_request[err->request_code]);

    /* Extensions may also define their  own errors, so just store the
     * error_code */
    if(e->error_code >= (sizeof(xutil_label_error) / sizeof(char *)))
        a_asprintf(&err->error_label, "%d", e->error_code);
    else
        err->error_label = a_strdup(xutil_label_error[e->error_code]);

    return true;
}

xcb_keysym_t
xutil_key_mask_fromstr(const char *keyname, size_t len)
{
    switch(a_tokenize(keyname, len))
    {
      case A_TK_SHIFT:   return XCB_MOD_MASK_SHIFT;
      case A_TK_LOCK:    return XCB_MOD_MASK_LOCK;
      case A_TK_CTRL:
      case A_TK_CONTROL: return XCB_MOD_MASK_CONTROL;
      case A_TK_MOD1:    return XCB_MOD_MASK_1;
      case A_TK_MOD2:    return XCB_MOD_MASK_2;
      case A_TK_MOD3:    return XCB_MOD_MASK_3;
      case A_TK_MOD4:    return XCB_MOD_MASK_4;
      case A_TK_MOD5:    return XCB_MOD_MASK_5;
      default:           return XCB_NO_SYMBOL;
    }
}

/** Permit to use mouse with many more buttons */
#ifndef XCB_BUTTON_INDEX_6
#define XCB_BUTTON_INDEX_6 6
#endif
#ifndef XCB_BUTTON_INDEX_7
#define XCB_BUTTON_INDEX_7 7
#endif
#ifndef XCB_BUTTON_INDEX_8
#define XCB_BUTTON_INDEX_8 8
#endif
#ifndef XCB_BUTTON_INDEX_9
#define XCB_BUTTON_INDEX_9 9
#endif

/** Link a name to a mouse button symbol */
typedef struct
{
    int id;
    unsigned int button;
} mouse_button_t;

/** Lookup for a mouse button from its index.
 * \param button Mouse button index.
 * \return Mouse button or 0 if not found.
 */
unsigned int
xutil_button_fromint(int button)
{
    /** List of button name and corresponding X11 mask codes */
    static const mouse_button_t mouse_button_list[] =
    {
        { 1, XCB_BUTTON_INDEX_1 },
        { 2, XCB_BUTTON_INDEX_2 },
        { 3, XCB_BUTTON_INDEX_3 },
        { 4, XCB_BUTTON_INDEX_4 },
        { 5, XCB_BUTTON_INDEX_5 },
        { 6, XCB_BUTTON_INDEX_6 },
        { 7, XCB_BUTTON_INDEX_7 },
        { 8, XCB_BUTTON_INDEX_8 },
        { 9, XCB_BUTTON_INDEX_9 }
    };

    if(button >= 1 && button <= countof(mouse_button_list))
        return mouse_button_list[button - 1].button;

    return 0;
}

/** Convert a root window a physical screen ID.
 * \param connection The connection to the X server.
 * \param root Root window.
 * \return A physical screen number.
 */
int
xutil_root2screen(xcb_connection_t *connection, xcb_window_t root)
{
    xcb_screen_iterator_t iter;
    int phys_screen = 0;

    for(iter = xcb_setup_roots_iterator(xcb_get_setup(connection));
        iter.rem && iter.data->root != root; xcb_screen_next(&iter), ++phys_screen);

    return phys_screen;
}

// vim: filetype=c:expandtab:shiftwidth=4:tabstop=8:softtabstop=4:encoding=utf-8:textwidth=80
