/*
 * common/xutil.h - X-related useful functions header
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

#ifndef AWESOME_COMMON_XUTIL_H
#define AWESOME_COMMON_XUTIL_H

#include <xcb/xcb.h>
#include <xcb/xcb_keysyms.h>
#include <xcb/xcb_aux.h>
#include <xcb/xcb_event.h>

/* XCB doesn't provide keysyms definition */
#include <X11/keysym.h>

#include "array.h"

#define XUTIL_MASK_CLEAN(mask) (mask & ~(globalconf.numlockmask | XCB_MOD_MASK_LOCK))

/* X error codes */

/* Everything's okay */
#define XUTIL_SUCCESS 0
/* Bad request code */
#define XUTIL_BAD_REQUEST 1
/* Int parameter out of range */
#define XUTIL_BAD_VALUE 2
/* Parameter not a Window */
#define XUTIL_BAD_WINDOW 3
/* Parameter not a Pixmap */
#define XUTIL_BAD_PIXMAP 4
/* Parameter not an Atom */
#define XUTIL_BAD_ATOM 5
/* Parameter not a Cursor */
#define XUTIL_BAD_CURSOR 6
/* Parameter not a Font */
#define XUTIL_BAD_FONT 7
/* Parameter mismatch */
#define XUTIL_BAD_MATCH 8
/* Parameter not a Pixmap or Window */
#define XUTIL_BAD_DRAWABLE 9
/* Depending on context:
   - key/button already grabbed
   - attempt to free an illegal
     cmap entry
   - attempt to store into a read-only
     color map entry.
   - attempt to modify the access control
     list from other than the local host.
*/
#define XUTIL_BAD_ACCESS 10
/* Insufficient resources */
#define XUTIL_BAD_ALLOC 11
/* No such colormap */
#define XUTIL_BAD_COLOR 12
/* Parameter not a GC */
#define XUTIL_BAD_GC 13
/* Choice not in range or already used */
#define XUTIL_BAD_ID_CHOICE 14
/* Font or color name doesn't exist */
#define XUTIL_BAD_NAME 15
/* Request length incorrect */
#define XUTIL_BAD_LENGTH 16
/* Server is defective */
#define XUTIL_BAD_IMPLEMENTATION 17

bool xutil_text_prop_get(xcb_connection_t *, xcb_window_t, xcb_atom_t, char **, ssize_t *);

void xutil_lock_mask_get(xcb_connection_t *, xcb_get_modifier_mapping_cookie_t,
                         xcb_key_symbols_t *, unsigned int *, unsigned int *,
                         unsigned int *);

/** Set the same handler for all errors */
void xutil_error_handler_catch_all_set(xcb_event_handlers_t *,
                                       xcb_generic_error_handler_t, void *);

typedef struct
{
    uint8_t request_code;
    char *request_label;
    char *error_label;
} xutil_error_t;

bool xutil_error_init(const xcb_generic_error_t *, xutil_error_t *);
xcb_keysym_t xutil_key_mask_fromstr(const char *, size_t);
unsigned int xutil_button_fromint(int);

static inline void
xutil_error_wipe(xutil_error_t *err)
{
    p_delete(&err->error_label);
    p_delete(&err->request_label);
}

/* Get the informations about the screen.
 * \param c X connection.
 * \param screen Screen number.
 * \return Screen informations (must not be freed!).
 */
static inline xcb_screen_t *
xutil_screen_get(xcb_connection_t *c, int screen)
{
    xcb_screen_t *s;

    if(xcb_connection_has_error(c))
        fatal("X connection invalid");

    s = xcb_aux_get_screen(c, screen);

    assert(s);

    return s;
}

int xutil_root2screen(xcb_connection_t *, xcb_window_t);

#endif
// vim: filetype=c:expandtab:shiftwidth=4:tabstop=8:softtabstop=4:encoding=utf-8:textwidth=80
