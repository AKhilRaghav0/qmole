/** Copyright 2011 Thorsten Wißmann. All rights reserved.
 *
 * This software is licensed under the "Simplified BSD License".
 * See LICENSE for details */


#ifndef __CLIENTLIST_H_
#define __CLIENTLIST_H_

#include <X11/Xlib.h>
#include <X11/Xproto.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#include <glib.h>
#include <stdbool.h>

#include "layout.h"

typedef struct HSClient {
    Window      window;
    XRectangle  last_size;
    HSTag*      tag;
    XRectangle  float_size;
    bool        urgent;
    bool        fullscreen;
    bool        pseudotile; // only move client but don't resize (if possible)
    int         pid;
} HSClient;

void clientlist_init();
void clientlist_destroy();

void clientlist_foreach(GHFunc func, gpointer data);

void window_focus(Window window);
void window_unfocus(Window window);
void window_unfocus_last();

void reset_client_colors();
void reset_client_settings();

// adds a new client to list of managed client windows
HSClient* manage_client(Window win);
void unmanage_client(Window win);

void window_enforce_last_size(Window in);

// destroys a special client
void destroy_client(HSClient* client);

HSClient* get_client_from_window(Window window);
HSClient* get_current_client();
XRectangle client_outer_floating_rect(HSClient* client);

void client_setup_border(HSClient* client, bool focused);
void client_resize(HSClient* client, XRectangle rect);
void client_resize_tiling(HSClient* client, XRectangle rect);
void client_resize_floating(HSClient* client, HSMonitor* m);
void client_clear_urgent(HSClient* client);
void client_update_wm_hints(HSClient* client);
int window_close_current();

void client_set_fullscreen(HSClient* client, bool state);
void client_set_pseudotile(HSClient* client, bool state);
// sets a client property, depending on argv[0]
int client_set_property_command(int argc, char** argv);
bool is_window_class_ignored(char* window_class);
bool is_window_ignored(Window win);

void window_show(Window win);
void window_hide(Window win);
void window_set_visible(Window win, bool visible);


#endif


