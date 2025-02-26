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


#ifndef __SN_COMMON_H__
#define __SN_COMMON_H__

#include <libsn/sn-util.h>
#include <X11/Xlib.h>

SN_BEGIN_DECLS

typedef struct SnDisplay SnDisplay;

typedef void (* SnDisplayErrorTrapPush) (SnDisplay *display,
                                         Display   *xdisplay);
typedef void (* SnDisplayErrorTrapPop)  (SnDisplay *display,
                                         Display   *xdisplay);

SnDisplay* sn_display_new             (Display                *xdisplay,
                                       SnDisplayErrorTrapPush  push_trap_func,
                                       SnDisplayErrorTrapPop   pop_trap_func);

void       sn_display_ref             (SnDisplay              *display);
void       sn_display_unref           (SnDisplay              *display);
Display*   sn_display_get_x_display   (SnDisplay              *display);

sn_bool_t  sn_display_process_event   (SnDisplay              *display,
                                       XEvent                 *xevent);

void       sn_display_error_trap_push (SnDisplay              *display);
void       sn_display_error_trap_pop  (SnDisplay              *display);



SN_END_DECLS

#endif /* __SN_COMMON_H__ */
