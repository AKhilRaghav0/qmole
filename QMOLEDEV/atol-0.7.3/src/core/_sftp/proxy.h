////////////////////////////////////////////////////////////////////////////
// Atol file manager project <http://atol.sf.net>
//
// This code is licensed under BSD license.See "license.txt" for more details.
//
// File: TOFIX
////////////////////////////////////////////////////////////////////////////

/*
 * Network proxy abstraction in PuTTY
 *
 * A proxy layer, if necessary, wedges itself between the
 * network code and the higher level backend.
 *
 * Supported proxies: HTTP CONNECT, generic telnet
 * In progress: SOCKS
 */

#ifndef PUTTY_PROXY_H
#define PUTTY_PROXY_H

#define PROXY_ERROR_GENERAL 8000
#define PROXY_ERROR_UNEXPECTED 8001

typedef struct Socket_proxy_tag * Proxy_Socket;

struct Socket_proxy_tag {
    struct socket_function_table *fn;
    /* the above variable absolutely *must* be the first in this structure */

    char * error;

    Socket sub_socket;
    Plug plug;
    SockAddr remote_addr;
    int remote_port;

    bufchain pending_output_data;
    bufchain pending_oob_output_data;
    int pending_flush;
    bufchain pending_input_data;

#define PROXY_STATE_NEW    -1
#define PROXY_STATE_ACTIVE  0

    int state; /* proxy states greater than 0 are implementation
		* dependent, but represent various stages/states
		* of the initialization/setup/negotiation with the
		* proxy server.
		*/
    int freeze; /* should we freeze the underlying socket when
		 * we are done with the proxy negotiation? this
		 * simply caches the value of sk_set_frozen calls.
		 */

#define PROXY_CHANGE_NEW      -1
#define PROXY_CHANGE_CLOSING   0
#define PROXY_CHANGE_SENT      1
#define PROXY_CHANGE_RECEIVE   2
#define PROXY_CHANGE_ACCEPTING 3

    /* something has changed (a call from the sub socket
     * layer into our Proxy Plug layer, or we were just
     * created, etc), so the proxy layer needs to handle
     * this change (the type of which is the second argument)
     * and further the proxy negotiation process.
     */

    int (*negotiate) (CSshSession &, Proxy_Socket /* this */, int /* change type */);

    /* current arguments of plug handlers
     * (for use by proxy's negotiate function)
     */

    /* closing */
    char *closing_error_msg;
    int closing_error_code;
    int closing_calling_back;

    /* receive */
    int receive_urgent;
    char *receive_data;
    int receive_len;

    /* sent */
    int sent_bufsize;

    /* accepting */
    void *accepting_sock;

};

typedef struct Plug_proxy_tag * Proxy_Plug;

struct Plug_proxy_tag {
    struct plug_function_table *fn;
    /* the above variable absolutely *must* be the first in this structure */

    Proxy_Socket proxy_socket;

};

void proxy_activate (CSshSession &session, Proxy_Socket);
int proxy_http_negotiate (CSshSession &session, Proxy_Socket, int);
int proxy_telnet_negotiate (CSshSession &session, Proxy_Socket, int);
int proxy_socks4_negotiate (CSshSession &session, Proxy_Socket, int);
int proxy_socks5_negotiate (CSshSession &session, Proxy_Socket, int);

#endif
