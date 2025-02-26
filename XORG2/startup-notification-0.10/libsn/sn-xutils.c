/*
 * Copyright (C) 2002 Red Hat, Inc.
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <config.h>
#include "sn-internals.h"
#include <X11/Xutil.h>
#include <X11/Xatom.h>

#include <xcb/xcb.h>
#include <xcb/xcb_atom.h>

Atom
sn_internal_atom_get (SnDisplay  *display,
                      const char *atom_name)
{
    switch (sn_internal_display_get_type (display))
    {
     case SN_DISPLAY_TYPE_XLIB:
      return XInternAtom (sn_display_get_x_display (display),
                          atom_name,
                          False);
     case SN_DISPLAY_TYPE_XCB:
      return xcb_atom_get (sn_display_get_x_connection (display),
                           atom_name);
    }
    return None;
}

void
sn_internal_set_utf8_string (SnDisplay  *display,
                             Window      xwindow,
                             const char *property,
                             const char *str)
{
  sn_display_error_trap_push (display);

  switch (sn_internal_display_get_type (display))
  {
    case SN_DISPLAY_TYPE_XLIB:
      XChangeProperty (sn_display_get_x_display (display),
                       xwindow,
                       sn_internal_atom_get (display, property),
                       sn_internal_atom_get (display, "UTF8_STRING"),
                       8, PropModeReplace, (unsigned char*) str,
                       strlen (str));
      break;
     case SN_DISPLAY_TYPE_XCB:
      xcb_change_property (sn_display_get_x_connection (display),
                           XCB_PROP_MODE_REPLACE,
                           xwindow,
                           sn_internal_atom_get (display, property),
                           sn_internal_atom_get (display, "UTF8_STRING"),
                           8, strlen (str), str);
      break;
  }

  sn_display_error_trap_pop (display);
}
