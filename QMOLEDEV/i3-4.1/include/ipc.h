/*
 * vim:ts=4:sw=4:expandtab
 *
 * i3 - an improved dynamic tiling window manager
 * © 2009-2011 Michael Stapelberg and contributors (see also: LICENSE)
 *
 * ipc.c: UNIX domain socket IPC (initialization, client handling, protocol).
 *
 */
#ifndef _IPC_H
#define _IPC_H

#include <ev.h>
#include <stdbool.h>
#include <yajl/yajl_gen.h>
#include <yajl/yajl_parse.h>

#include "data.h"
#include "tree.h"

#include "i3/ipc.h"

extern char *current_socketpath;

typedef struct ipc_client {
        int fd;

        /* The events which this client wants to receive */
        int num_events;
        char **events;

        TAILQ_ENTRY(ipc_client) clients;
} ipc_client;

/*
 * Callback type for the different message types.
 *
 * message is the raw packet, as received from the UNIX domain socket. size
 * is the remaining size of bytes for this packet.
 *
 * message_size is the size of the message as the sender specified it.
 * message_type is the type of the message as the sender specified it.
 *
 */
typedef void(*handler_t)(int, uint8_t*, int, uint32_t, uint32_t);

/* Macro to declare a callback */
#define IPC_HANDLER(name) \
        static void handle_ ## name (int fd, uint8_t *message, \
                                     int size, uint32_t message_size, \
                                     uint32_t message_type)

/**
 * Handler for activity on the listening socket, meaning that a new client
 * has just connected and we should accept() him. Sets up the event handler
 * for activity on the new connection and inserts the file descriptor into
 * the list of clients.
 *
 */
void ipc_new_client(EV_P_ struct ev_io *w, int revents);

/**
 * Creates the UNIX domain socket at the given path, sets it to non-blocking
 * mode, bind()s and listen()s on it.
 *
 */
int ipc_create_socket(const char *filename);

/**
 * Sends the specified event to all IPC clients which are currently connected
 * and subscribed to this kind of event.
 *
 */
void ipc_send_event(const char *event, uint32_t message_type, const char *payload);

/**
 * Calls shutdown() on each socket and closes it. This function to be called
 * when exiting or restarting only!
 *
 */
void ipc_shutdown();

void dump_node(yajl_gen gen, Con *con, bool inplace_restart);

#endif
