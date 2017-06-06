/** Copyright 2011 Thorsten Wißmann. All rights reserved.
 *
 * This software is licensed under the "Simplified BSD License".
 * See LICENSE for details */

#include "globals.h"
#include "command.h"
#include "utils.h"
#include "ipc-protocol.h"
#include "ipc-server.h"

#include <string.h>
#include <stdio.h>
#include <glib.h>

#include <X11/Xlib.h>
#include <X11/Xproto.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>

// public callable functions
//
void ipc_init() {
}

void ipc_destroy() {
}

void ipc_add_connection(Window window) {
    XSelectInput(g_display, window, PropertyChangeMask);
    // check, if property already exists
    ipc_handle_connection(window, true);
}

bool ipc_handle_connection(Window win, bool try_it) {
    XTextProperty text_prop;
    if (!XGetTextProperty(g_display, win, &text_prop, ATOM(HERBST_IPC_ARGS_ATOM))) {
        if (try_it) {
            // just return without doing anything else
            return false;
        } else {
            fprintf(stderr, "herbstluftwm: Warning: herbstclient window %d does "
                            "not have the Atom \"%s\" set. Ignoring it.\n",
                            (unsigned int)win, HERBST_IPC_ARGS_ATOM);
            return false;
        }
    }
    char** list_return;
    int count;
    if (Success != Xutf8TextPropertyToTextList(g_display, &text_prop, &list_return, &count)) {
        fprintf(stderr, "herbstluftwm: Warning: couldnot parse the %s atom of herbstclient "
                        "window %d to utf8 list\n",
                        HERBST_IPC_ARGS_ATOM, (unsigned int)win);
        XFree(text_prop.value);
        return false;
    }
    // now we do not need any events from this window anymore
    XSelectInput(g_display, win, 0);
    GString* output = g_string_new("");
    int status = call_command(count, list_return, &output);
    // send output back
    XChangeProperty(g_display, win, ATOM(HERBST_IPC_OUTPUT_ATOM),
        ATOM("UTF8_STRING"), 8, PropModeReplace,
        (unsigned char*)output->str, 1+strlen(output->str));
    // and also set the exit status
    XChangeProperty(g_display, win, ATOM(HERBST_IPC_STATUS_ATOM),
        XA_ATOM, 32, PropModeReplace, (unsigned char*)&(status), 1);
    // cleanup
    XFreeStringList(list_return);
    XFree(text_prop.value);
    g_string_free(output, true);
    return true;
}

bool is_ipc_connectable(Window window) {
    XClassHint hint;
    if (0 == XGetClassHint(g_display, window, &hint)) {
        return false;
    }
    bool is_ipc = false;
    if (hint.res_name && hint.res_class &&
        !strcmp(hint.res_class, HERBST_IPC_CLASS)) {
        is_ipc = true;
    }
    if (hint.res_name) XFree(hint.res_name);
    if (hint.res_class) XFree(hint.res_class);
    return is_ipc;
}

