
/***************************************************************************
 * nsock.h -- public interface definitions for the nsock parallel socket   *
 * event library                                                           *
 *                                                                         *
 ***********************IMPORTANT NSOCK LICENSE TERMS***********************
 *                                                                         *
 * The nsock parallel socket event library is (C) 1999-2008 Insecure.Com   *
 * LLC This library is free software; you may redistribute and/or          *
 * modify it under the terms of the GNU General Public License as          *
 * published by the Free Software Foundation; Version 2.  This guarantees  *
 * your right to use, modify, and redistribute this software under certain *
 * conditions.  If this license is unacceptable to you, Insecure.Com LLC   *
 * may be willing to sell alternative licenses (contact                    *
 * sales@insecure.com ).                                                   *
 *                                                                         *
 * As a special exception to the GPL terms, Insecure.Com LLC grants        *
 * permission to link the code of this program with any version of the     *
 * OpenSSL library which is distributed under a license identical to that  *
 * listed in the included COPYING.OpenSSL file, and distribute linked      *
 * combinations including the two. You must obey the GNU GPL in all        *
 * respects for all of the code used other than OpenSSL.  If you modify    *
 * this file, you may extend this exception to your version of the file,   *
 * but you are not obligated to do so.                                     *
 *                                                                         * 
 * If you received these files with a written license agreement stating    *
 * terms other than the (GPL) terms above, then that alternative license   *
 * agreement takes precedence over this comment.                          *
 *                                                                         *
 * Source is provided to this software because we believe users have a     *
 * right to know exactly what a program is going to do before they run it. *
 * This also allows you to audit the software for security holes (none     *
 * have been found so far).                                                *
 *                                                                         *
 * Source code also allows you to port Nmap to new platforms, fix bugs,    *
 * and add new features.  You are highly encouraged to send your changes   *
 * to fyodor@insecure.org for possible incorporation into the main         *
 * distribution.  By sending these changes to Fyodor or one of the         *
 * insecure.org development mailing lists, it is assumed that you are      *
 * offering Fyodor and Insecure.Com LLC the unlimited, non-exclusive right *
 * to reuse, modify, and relicense the code.  Nmap will always be          *
 * available Open Source, but this is important because the inability to   *
 * relicense code has caused devastating problems for other Free Software  *
 * projects (such as KDE and NASM).  We also occasionally relicense the    *
 * code to third parties as discussed above.  If you wish to specify       *
 * special license conditions of your contributions, just say so when you  *
 * send them.                                                              *
 *                                                                         *
 * This program is distributed in the hope that it will be useful, but     *
 * WITHOUT ANY WARRANTY; without even the implied warranty of              *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU       *
 * General Public License v2.0 for more details                            *
 * (http://www.gnu.org/licenses/gpl-2.0.html).                             *
 *                                                                         *
 ***************************************************************************/

/* $Id: nsock.h 8885 2008-07-14 19:37:26Z kris $ */

/* Would you like to include pcap support in nsock?
 * Pcap support code is currently unstable, so we give
 * you a choice. In future this #define will be removed.*/
#define HAVE_PCAP 1

#ifndef NSOCK_H
#define NSOCK_H

/* Keep assert() defined for security reasons */
#undef NDEBUG

#include <sys/types.h>
#ifndef WIN32
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/time.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* The read calls will generally return after reading at least this
 * much data so that the caller can process it and so that the
 * connection spewing data doesn't monopolize resources.  The caller
 * can always initiate another read request to ask for more.
 */
#define NSOCK_READ_CHUNK_SIZE 0x8FFFF

/*********************  TYPEDEFS ********************/
/* nsock_pool, nsock_iod, and nsock_event are opaque objects that should 
   only be accessed using the appropriate accessor functions (described 
   below).  
 */

/* An nsock_pool aggregates and manages events and i/o descriptors */
typedef void *nsock_pool;

/* nsock_iod is an I/O descriptor -- you create it and then use it to
   make calls to do connect()s, read()s, write()s, etc.  A single IOD
   can handle multiple event calls, but only one at a time.  Also the
   event calls must be in a "reasonable" order.  For example, you
   might start with nsock_connect_tcp() followed by a bunch of
   nsock_read* and nsock_write* calls.  Then you either destroy the
   iod for good with nsi_delete() and allocate a new one via nsi_new
   for your next connection. */
typedef void *nsock_iod;

/* An event is created when you do various calls (for reading, writing,
   connecting, timers, etc) and is provided back to you in the callback
   when the call completes/fails. It is automatically destroyed after
   the callback 
*/
typedef void *nsock_event;

/* Provided by calls which (internally) create an nsock_event.  This allows
   you to cancel the event */
typedef unsigned long nsock_event_id;

/* This is used to save SSL sessionids between SSL connections */
typedef void *nsock_ssl_session;

/********************   PROTOTYPES *******************/


/* Here is the all important looping function that tells the event
   engine to start up and begin processing events.  It will continue
   until all events have been delivered (including new ones started
   from event handlers), or the msec_timeout is reached, or a major
   error has occured.  Use -1 if you don't want to set a maximum time
   for it to run.  A timeout of 0 will return after 1 non-blocking
   loop.  The nsock loop can be restarted again after it returns.  For
   example you could do a series of 15 second runs, allowing you to do
   other stuff between them.  Or you could just schedule a timer to
   call you back every 15 seconds.*/
enum nsock_loopstatus { NSOCK_LOOP_NOEVENTS = 2, NSOCK_LOOP_TIMEOUT, NSOCK_LOOP_ERROR };
enum nsock_loopstatus nsock_loop(nsock_pool nsp, int msec_timeout);

/* This next function returns the errno style error code -- which is only
   valid if the status is NSOCK_LOOP_ERROR was returned by nsock_loop() */
int nsp_geterrorcode(nsock_pool nsp);

/* Every nsp has an ID that is unique across the program execution */
unsigned long nsp_getid(nsock_pool nsp);

/* Note that nsi_get1_ssl_session will increment the usage count
 * of the SSL_SESSION, since nsock does a free when the nsi is
 * destroyed.  It's up to any calling function/etc to do a
 * SSL_SESSION_free() on it.  nsi_get0_ssl_session doesn't
 * increment, and is for informational purposes only.
 */
nsock_ssl_session nsi_get1_ssl_session(nsock_iod nsockiod);
nsock_ssl_session nsi_get0_ssl_session(nsock_iod nsockiod);

/* Sometimes it is useful to store a pointer to information inside
   the NSP so you can retrieve it during a callback. */
void nsp_setud(nsock_pool nsp, void *data);

/* And the function above wouldn't make much sense if we didn't have a way
   to retrieve that data ... */
void *nsp_getud(nsock_pool nsp);

/* Sets a trace/debug level.  Zero (the default) turns tracing off,
   while higher numbers are more verbose.  This is generally only used
   for debugging purposes.  Trace logs are printed to stdout.  The
   initial value is set in nsp_new().  A level of 1 or 2 is usually
   sufficient, but 10 will ensure you get everything.  The basetime
   can be NULL to print trace lines with the current time, otherwise
   the difference between the current time and basetime will be used
   (the time program execution starts would be a good candidate) */
void nsp_settrace(nsock_pool nsp, int tracelevel, const struct timeval *basetime);

/* And here is how you create an nsock_pool.  This allocates, initializes,
   and returns an nsock_pool event aggregator.  In the case of error,
   NULL will be returned.  If you do not wish to immediately associate
   any userdata, pass in NULL. */
nsock_pool nsp_new(void *userdata);

/* If nsp_new returned success, you must free the nsp when you are
   done with it to conserve memory (and in some cases, sockets).
   After this call, nsp may no longer be used.  Any pending events are
   sent an NSE_STATUS_KILL callback and all outstanding iods are
   deleted. */
void nsp_delete(nsock_pool nsp);

/* nsock_event handles a single event.  Its ID is generally returned when
   the event is created, and the event itself is included in callbacks */
/* IF YOU ADD NEW NSE_TYPES YOU MUST INCREASE TYPE_CODE_NUM_BITS SO THAT
   IT IS ALWAYS log2(maximum_nse_type_value + 1) */
#define TYPE_CODE_NUM_BITS 3
enum nse_type { 
	NSE_TYPE_CONNECT=0,
	NSE_TYPE_CONNECT_SSL=1,
	NSE_TYPE_READ=2,
	NSE_TYPE_WRITE=3, 
	NSE_TYPE_TIMER=4,
	NSE_TYPE_PCAP_READ=5,
	NSE_TYPE_MAX=6,
};  /* At some point I was considering a NSE_TYPE_START and NSE_TYPE_CUSTOM */

/* Find the type of an event that spawned a callback */
enum nse_type nse_type(nsock_event nse);

/* Takes an nse_type (as returned by nse_type() and returns a static
   string name that you can use for printing, etc. */
const char *nse_type2str(enum nse_type type);

/* Did the event succeed?  What is the status? */
enum nse_status { NSE_STATUS_NONE = 0, /* User should never see this */
                  NSE_STATUS_SUCCESS, /* Everything went A-OK! */
                  NSE_STATUS_ERROR, /* Uh-oh!  Problem, check the
				       nse_errorcode() */
		  NSE_STATUS_TIMEOUT, /* The async call surpassed the
					 timeout you specified */
		  NSE_STATUS_CANCELLED, /* Someone cancelled the
                                           event. (by calling
                                           nsock_event_cancel. */
		  NSE_STATUS_KILL, /* The event has been killed, this
				      generally means the nspool is
				      being deleted -- you should free
				      up any resources you have
				      allocated and exit.  Don't you
				      dare make any more async nsock calls!  */
		  NSE_STATUS_EOF /* We got EOF and NO DATA -- if we got data
				    first, SUCCESS is reported (see nse_eof() 
				 */
                 }; 

enum nse_status nse_status(nsock_event nse);

/* Takes an nse_status (as returned by nse_status() and returns a static
   string name that you can use for printing, etc. */
const char *nse_status2str(enum nse_status status);

/* This next function tells whether we received an EOF when we
   were reading.  It is generally a better way to check for EOF
   than looking at the status because sometimes we read some data
   before getting the EOF, in which SUCCESS is returned (although
   another read attempt would return a status of EOF).  nse_eof
   returns nonzero if we have reached EOF, zero if we have NOT
   reach EOF.  */
int nse_eof(nsock_event nse);

/* This next function returns the errno style error code -- which is only
   valid if the status is NSE_STATUS_ERROR (this is a normal errno style
   errorcode */
int nse_errorcode(nsock_event nse);

/* Every event has an ID which will be unique throughout the program's execution (for a given nsock_pool) unless you blow through 500,000,000 of them */
nsock_event_id nse_id(nsock_event nse);

/* If you did a read request, and the result was STATUS_SUCCESS, this 
   function provides the buffer that was read in as well as the number
   of chars read.  The buffer should not be modified or free'd .  It is not 
   guaranteed to be NUL-terminated and it may even contain nuls */
char *nse_readbuf(nsock_event nse, int *nbytes);

/* Obtains the nsock_iod (see below) associated with the event.  Note that 
   some events (such as timers) don't have an nsock_iod associated with them
*/
nsock_iod nse_iod(nsock_event nse);


/* nsock_iod is like a "file descriptor" for the nsock library.  You
   use it to request events.  And here is how you create an nsock_iod.
   nsi_new returns NULL if the iod cannot be allocated.  Pass NULL as
   userdata if you don't want to immediately associate any user data
   with the iod. */
nsock_iod nsi_new(nsock_pool nsockp, void *userdata);

/* This version allows you to associate an existing sd with the msi
   so that you can read/write it using the nsock infrastructure.  For example,
   you may want to watch for data from STDIN_FILENO at the same time as you
   read/wrtie various sockets. Ths sd is dup()ed, so you may close or
   otherwise manipulate your copy.  The duped copy will be destroyed when the
   nsi is destroyed 
*/
nsock_iod nsi_new2(nsock_pool nsockp, int sd, void *userdata);

/* If msiod_new returned success, you must free the iod when you are
   done with it to conserve memory (and in some cases, sockets).
   After this call, nsockiod may no longer be used -- you need to
   create a new one with nsi_new().  pending_response tells what to do
   with any events that are pending on this nsock_iod.  This can be
   NSOCK_PENDING_NOTIFY (send a KILL notification to each event),
   NSOCK_PENDING_SILENT (do not send notification to the killed
   events), or NSOCK_PENDING_ERROR (print an error message and quiit
   the program) */
#define NSOCK_PENDING_NOTIFY 1
#define NSOCK_PENDING_SILENT 2
#define NSOCK_PENDING_ERROR 4
void nsi_delete(nsock_iod nsockiod, int pending_response);

/* Sometimes it is useful to store a pointer to information inside
   the nsiod so you can retrieve it during a callback. */
void nsi_setud(nsock_iod nsiod, void *data);

/* And the function above wouldn't make much sense if we didn't have a way
   to retrieve that data ... */
void *nsi_getud(nsock_iod nsiod);

/* I didn't want to do this.  Its an ugly hack, but I suspect it will
   be neccessary.  I certainly can't reproduce in nsock EVERYTHING you
   might want to do with a socket.  So I'm offering you this function
   to obtain the socket descriptor which is (usually) wrapped in a
   nsock_iod).  You can do "reasonable" things with it, like setting
   socket receive buffers.  But don't create havok by closing the
   descriptor!  If the descriptor you get back is -1, the iod does not
   currently possess a valid descriptor */
int nsi_getsd(nsock_iod nsiod);

/* Returns the ID of an nsock_iod .  This ID is always unique amongst
   ids for a given nspool (unless you blow through billions of them). */
unsigned long nsi_id(nsock_iod nsockiod);

  /* Returns 1 if an NSI is communicating via SSL, 0 otherwise */
int nsi_checkssl(nsock_iod nsockiod);

/* Sets the local address to bind to before connect() */
int nsi_set_localaddr(nsock_iod nsi, struct sockaddr_storage *ss, size_t sslen);

/* Sets IPv4 options to apply before connect().  It makes a copy of the 
 * options, so you can free() yours if necessary.  This copy is freed
 * when the iod is destroyed
 */
int nsi_set_ipoptions(nsock_iod nsi, void *ipopts, size_t ipoptslen);

/* Returns that host/port/protocol information for the last
   communication (or comm. attempt) this nsi has been involved with.
   By "involved" with I mean interactions like establishing (or trying
   to) a connection or sending a UDP datagram through an unconnected
   nsock_iod.  AF is the address family (AF_INET or AF_INET6), Protocl
   is IPPROTO_TCP or IPPROTO_UDP.  Pass NULL for information you do
   not need.  If ANY of the information you requested is not
   available, 0 will be returned and the unavailable sockets are
   zeroed.  If protocol or af is requested but not available, it will
   be set to -1 (and 0 returned).  The pointers you pass in must be
   NULL or point to allocated address space.  The sockaddr members
   should actually be sockaddr_storage, sockaddr_in6, or sockaddr_in
   with the socklen of them set appropriately (eg
   sizeof(sockaddr_storage) if that is what you are passing). */
int nsi_getlastcommunicationinfo(nsock_iod ms_iod, int *protocol,
				 int *af, struct sockaddr *local, 
				 struct sockaddr *remote, size_t socklen);

/* EVENT CREATION FUNCTIONS -- These functions request asynchronous
   notification of completion of an event.  The handler will never be
   synchronously called back during the event creation call (that
   causes too many hard to debug errors and plus we don't want people
   to have to deal with callbacks until they actually call nsock_loop */

/* These functions generally take a common 5 initial parameters:

   nsock_pool mst -- the is the nsock_pool describing the events you have 
         scheduled, etc

   nsock_iod  nsiod -- The I/O Descriptor that should be used in the 
         request.  Note that timer events don't have this argument since
         they don't use an iod.  You can obtain it in the callback from
	 the nsock_event. 

   nsock_ev_handler handler -- This is the function you want the
	 system to call when your event is triggered (or times out, or
	 hits an error, etc.).  The function should be of this form:
	 void funcname(nsock_pool nsp, nsock_event nse, void *userdata)

   int timeout_msecs -- The timeout for the request in milliseconds.
         If the request hasn't completed (or in a few cases started)
         within the timeout specified, the handler will be called with
         a TIMEOUT status and the request will be aborted.

   void *userdata -- The nsock_event that comes back can
         optionally have a pointer associated with it.  You can set
         that pointer here.  If you don't want one, just pass NULL.

   These functions return an nsock_event_id which can be used to cancel
   the event if neccessary.
*/

typedef void (*nsock_ev_handler)(nsock_pool, nsock_event, void *);

/* Request a TCP connection to another system (by IP address).  The
   in_addr is normal network byte order, but the port number should be
   given in HOST BYTE ORDER.  ss should be a sockaddr_storage,
   sockaddr_in6, or sockaddr_in as appropriate (just like what you
   would pass to connect).  sslen should be the sizeof the structure
   you are passing in. */
nsock_event_id nsock_connect_tcp(nsock_pool nsp, nsock_iod nsiod, 
				 nsock_ev_handler handler, int timeout_msecs, 
				 void *userdata, struct sockaddr *ss,
				 size_t sslen, unsigned short port);

/* Request a UDP "connection" to another system (by IP address).  The
   in_addr is normal network byte order, but the port number should be
   given in HOST BYTE ORDER.  Since this is UDP, no packets are
   actually sent.  The destination IP and port are just associated
   with the nsiod (an actual OS connect() call is made).  You can then
   use the normal nsock write calls on the socket.  There is no
   timeout since this call always calls your callback at the next
   opportunity.  The advantages to having a connected UDP socket (as
   opposed to just specifying an address with sendto() are that we can
   now use a consistent set of write/read calls for TCP/UDP, received
   packets from the non-partner are automatically dropped by the OS,
   and the OS can provide asynchronous errors (see Unix Network
   Programming pp224).  ss should be a sockaddr_storage,
   sockaddr_in6, or sockaddr_in as appropriate (just like what you
   would pass to connect).  sslen should be the sizeof the structure
   you are passing in. */
nsock_event_id nsock_connect_udp(nsock_pool nsp, nsock_iod nsiod, 
				 nsock_ev_handler handler, void *userdata, 
				 struct sockaddr *ss, size_t sslen,
				 unsigned short port);

/* Request an SSL over TCP connection to another system (by IP
   address).  The in_addr is normal network byte order, but the port
   number should be given in HOST BYTE ORDER.  This function will call
   back only after it has made the TCP connection AND done the initial
   SSL negotiation.  From that point on, you use the normal read/write
   calls and decryption will happen transparently. ss should be a
   sockaddr_storage, sockaddr_in6, or sockaddr_in as appropriate (just
   like what you would pass to connect).  sslen should be the sizeof
   the structure you are passing in. */
nsock_event_id nsock_connect_ssl(nsock_pool nsp, nsock_iod nsiod, 
				 nsock_ev_handler handler, int timeout_msecs, 
				 void *userdata, struct sockaddr *ss, 
				 size_t sslen, unsigned short port,
				 nsock_ssl_session ssl_session);

/* Request ssl connection over already established TCP connection.
   nsiod must be socket that is already connected to target
   using nsock_connect_tcp.
   All parameters have the same meaning as in 'nsock_connect_ssl' */
nsock_event_id nsock_reconnect_ssl(nsock_pool nsp, nsock_iod nsiod,
				   nsock_ev_handler handler, int timeout_msecs,
                                   void *userdata, nsock_ssl_session ssl_session);

/* Read up to nlines lines (terminated with \n, which of course
   inclues \r\n), or until EOF, or until the timeout, whichever comes
   first.  Note that NSE_STATUS_SUCCESS will be returned in the case
   of EOF or tiemout if at least 1 char has been read.  Also note that
   you may get more than 'nlines' back -- we just stop once "at least"
   'nlines' is read */
nsock_event_id nsock_readlines(nsock_pool nsp, nsock_iod nsiod, 
			 nsock_ev_handler handler, int timeout_msecs,
			 void *userdata, int nlines);

/* Same as above, except it tries to read at least 'nbytes' instead of 
   'nlines'. */
nsock_event_id nsock_readbytes(nsock_pool nsp, nsock_iod nsiod,
			 nsock_ev_handler handler, int timeout_msecs, 
			 void *userdata, int nbytes);

/* The simplest read function -- returns NSE_STATUS_SUCCESS when it
   reads anything, otherwise it returns timeout, eof, or error as
   appropriate */
nsock_event_id nsock_read(nsock_pool nsp, nsock_iod nsiod, nsock_ev_handler handler, 
		    int timeout_msecs, void *userdata);

/* Write some data to the socket.  If the write is not COMPLETED within timeout_msecs , NSE_STATUS_TIMEOUT will be returned.  If you are supplying NUL-terminated data, you can optionally pass -1 for datalen and nsock_write will figure out the length itself */
nsock_event_id nsock_write(nsock_pool nsp, nsock_iod nsiod, 
		     nsock_ev_handler handler, int timeout_msecs, 
		     void *userdata, const char *data, int datalen);

/* Same as nsock_write except you can use a printf-style format and you can only use this for ASCII strings */
nsock_event_id nsock_printf(nsock_pool nsp, nsock_iod nsiod, 
		      nsock_ev_handler handler, int timeout_msecs, 
		      void *userdata, char *format, ... );

/* Send back an NSE_TYPE_TIMER after the number of milliseconds specified.  Of course it can also return due to error, cancellation, etc. */
nsock_event_id nsock_timer_create(nsock_pool nsp, nsock_ev_handler handler, 
			    int timeout_msecs, void *userdata);

/* Cancel an event (such as a timer or read request).  If notify is
 nonzero, the requester will be sent an event CANCELLED status back to
 the given handler.  But in some cases there is no need to do this
 (like if the function deleting it is the one which created it), in
 which case 0 can be passed to skip the step.  This function returns
 zero if the event is not found, nonzero otherwise */
int nsock_event_cancel(nsock_pool ms_pool, nsock_event_id id, int notify );

/* Grab the latest time as recorded by the nsock library, which does
   so at least once per event loop (in main_loop).  Not only does this
   function (generally) avoid a system call, but in many circumstances
   it is better to use nsock's time rather than the system time.  If
   nsock has never obtained the time when you call it, it will do so
   before returning */
const struct timeval *nsock_gettimeofday();


#ifdef HAVE_PCAP
/*
 * Open pcap device and connect it to nsp. Other parameters have the
 * same meaning as for pcap_open_live in pcap(3).
 * 	device   : pcap-style device name
 * 	snaplen  : size of packet to be copied to handler
 * 	promisc  : whether to open device in promiscous mode
 * 	bpf_fmt	 : berkeley filter 
 * return value: NULL if everything was okay, or error string if error occurred
 * [sorry Fyodor for breaking the API, but it's just simpler]
 * */
char *nsock_pcap_open(nsock_pool nsp, nsock_iod nsiod, 
		const char *pcap_device, int snaplen, int promisc,
		const char *bpf_fmt, ...);

/*
 * Requests exacly one packet to be captured.from pcap. 
 * See nsock_read() for parameters description.
 * */
nsock_event_id nsock_pcap_read_packet(nsock_pool nsp, nsock_iod nsiod, 
			 nsock_ev_handler handler, int timeout_msecs,
			 void *userdata);

/*
 * Gets packet data. This should be called after succesfull receiving of packet
 * to get packet.  If you're not interested in some values, just pass NULL
 * instead of valid pointer.
 * l3_data is just after l2_data in buffer. Feel free to treat l2_data as one
 * buffer with size of (l2_len + l3_len). 
 * Ts time is fixed for systems that don't support proper timing, like Windows.
 * So TS is pointing to time when packet was received or to the time _after_.
 * As a result you'll get longer times than you should, but it's safer to
 * think that host is a bit further. 
 * */
void nse_readpcap(nsock_event nsee,
	const unsigned char **l2_data, size_t *l2_len,
	const unsigned char **l3_data, size_t *l3_len,
	size_t *packet_len, struct timeval *ts); 

/* Well. Just pcap-style datalink. Like DLT_EN10MB or DLT_SLIP. Check in pcap(3) manpage. */
int nsi_pcap_linktype(nsock_iod nsiod);

/* Is this nsiod a pcap descriptor? */
int nsi_is_pcap(nsock_iod nsiod);

#endif /* HAVE_PCAP */

#ifdef __cplusplus
} /* End of 'extern "C"' */
#endif

#endif /* NSOCK_H */








