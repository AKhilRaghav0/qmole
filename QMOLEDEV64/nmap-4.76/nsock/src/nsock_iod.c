
/***************************************************************************
 * nsock_iod.c -- This contains the functions relating to nsock_iod (and   *
 * its nsock internal manifistation -- nsockiod.  This is is similar to a  *
 * file descriptor in that you create it and then use it to initiate       *
 * connections, read/write data, etc.                                      *
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

/* $Id: nsock_iod.c 9270 2008-08-02 03:44:44Z kris $ */

#include "nsock.h"
#include "nsock_internal.h"
#include "gh_list.h"
#include "netutils.h"

#if HAVE_PCAP
#include "nsock_pcap.h"
#endif

#include <string.h>

/* nsock_iod is like a "file descriptor" for the nsock library.  You
   use it to request events.  And here is how you create an nsock_iod.
   nsi_new returns NULL if the iod cannot be allocated.  Pass NULL as
   userdata if you don't want to immediately associate any user data
   with the iod. */
nsock_iod nsi_new(nsock_pool nsockp, void *userdata) {
  return nsi_new2(nsockp, -1, userdata);
}

/* This version allows you to associate an existing sd with the msi
   so that you can read/write it using the nsock infrastructure.  For example,
   you may want to watch for data from STDIN_FILENO at the same time as you
   read/wrtie various sockets. Ths sd is dup()ed, so you may close or
   otherwise manipulate your copy.  The duped copy will be destroyed when the
   nsi is destroyed 
*/
nsock_iod nsi_new2(nsock_pool nsockp, int sd, void *userdata) {
  mspool *nsp = (mspool *) nsockp;
  msiod *nsi;

  nsi = (msiod *) gh_list_pop(&nsp->free_iods);
  if (!nsi) nsi = (msiod * ) safe_malloc(sizeof(msiod));

  memset(nsi, 0, sizeof(*nsi));

  if (sd == -1) {  
    nsi->sd = -1;
    nsi->state = NSIOD_STATE_INITIAL;
  } else {
    nsi->sd = dup(sd);
    if (nsi->sd == -1) {
      free(nsi);
      return NULL;
    }
    unblock_socket(nsi->sd);
    nsi->state = NSIOD_STATE_UNKNOWN;
  }

  nsi->locallen = 0;
    
  nsi->userdata = userdata;
  nsi->nsp = (mspool *) nsockp;
  nsi->events_pending = 0;

  nsi->ipopts = NULL;
  nsi->ipoptslen = 0;

#if HAVE_OPENSSL
  nsi->ssl_session = NULL;
#endif

  nsi->id = nsp->next_iod_serial++;
  if (nsi->id == 0) nsi->id = nsp->next_iod_serial++;

  /* The nsp keeps track of active msiods so it can delete them 
     if it is deleted */
  nsi->entry_in_nsp_active_iods = gh_list_append(&nsi->nsp->active_iods, nsi);

  return (nsock_iod) nsi;
}


/* If msiod_new returned success, you must free the iod when you are
   done with it to conserve memory (and in some cases, sockets).
   After this call, nsockiod may no longer be used -- you need to
   create a new one with nsi_new().  pending_response tells what to do
   with any events that are pending on this nsock_iod.  This can be
   NSOCK_PENDING_NOTIFY (send a KILL notification to each event),
   NSOCK_PENDING_SILENT (do not send notification to the killed
   events), or NSOCK_PENDING_ERROR (print an error message and quiit
   the program) */
void nsi_delete(nsock_iod nsockiod, int pending_response) {
  msiod *nsi = (msiod *) nsockiod;
  gh_list *elist_ar[3];
  int elist;
  gh_list_elem *currev_elem, *next_elem;
  msevent *currev;
  assert(nsi);

  if (nsi->state == NSIOD_STATE_DELETED) {
    fatal("nsi_delete() called on nsock_iod which appears to have already been deleted");
  }

  if (nsi->events_pending > 0) {
    /* shit -- they killed the msiod while an event was still pending
       on it.  Maybe I should store the pending events in the msiod.
       On the other hand, this should be a pretty rare occurance and
       so I'll save space and hassle by just locating the events here
       by searching through the active events list */
    if (pending_response == NSOCK_PENDING_ERROR)
      fatal("nsi_delete called with argument NSOCK_PENDING_ERROR on a nsock_iod that has %d pending event(s) associated with it", nsi->events_pending);
    assert(pending_response == NSOCK_PENDING_NOTIFY ||
	   pending_response == NSOCK_PENDING_SILENT);
    elist_ar[0] = &(nsi->nsp->evl.read_events);
    elist_ar[1] = &(nsi->nsp->evl.write_events);
    elist_ar[2] = &(nsi->nsp->evl.connect_events);
    for(elist = 0; elist < 3 && nsi->events_pending > 0; elist++) {
      currev_elem = GH_LIST_FIRST_ELEM(elist_ar[elist]);
      while(currev_elem) {
	currev = (msevent *) GH_LIST_ELEM_DATA(currev_elem);
	next_elem = GH_LIST_ELEM_NEXT(currev_elem);
	if (currev->iod == nsi) {
	  /* OK - we found an event pending on this IOD.  Kill it. */
	  /* printf("Found an outstanding event (out of %d), removing\n", nsi->events_pending); */
	  msevent_cancel(nsi->nsp, currev, elist_ar[elist], currev_elem, pending_response == NSOCK_PENDING_NOTIFY);
	}
	if (nsi->events_pending == 0)
	  break;
	currev_elem = next_elem;
      }
    }
  }
  
  if (nsi->events_pending != 0)
    fatal("Trying to delete NSI, but could not find %d of the purportedly pending events on that IOD.\n", nsi->events_pending);

#if HAVE_OPENSSL
  /* Close any SSL resources */
  if (nsi->ssl) {
    /* No longer free session because copy nsi stores is not reference counted */
    /*    if (nsi->ssl_session)
	  SSL_SESSION_free(nsi->ssl_session); 
	  nsi->ssl_session = NULL; */
    if (SSL_shutdown(nsi->ssl) == -1) {
      if (nsi->nsp->tracelevel > 1)
	nsock_trace(nsi->nsp, 
		    "nsi_delete(): SSL shutdown failed (%s) on NSI %li",
		    ERR_reason_error_string(SSL_get_error(nsi->ssl, -1)), 
		    nsi->id);
    }
    /* I don't really care if the SSL_shutdown() succeeded politely.  I could
       make the SD blocking temporarily for this, but I'm hoping it will
       succeed 95% of the time because we can usually write to a socket. */
    SSL_free(nsi->ssl);
    nsi->ssl = NULL;

  }
#endif

  if (nsi->sd >= 0) {
    close(nsi->sd);
    nsi->sd = -1;
  }

  nsi->state = NSIOD_STATE_DELETED;
  nsi->userdata = NULL;

  if (nsi->ipoptslen)
    free(nsi->ipopts);

#if HAVE_PCAP
  if(nsi->pcap){
    mspcap *mp = (mspcap *) nsi->pcap;
    if(mp->pt){
      pcap_close(mp->pt);
      mp->pt=NULL;
    }
    if(mp->pcap_desc){
      // Should I close pcap_desc or pcap_close does this for me?
      mp->pcap_desc = -1;
    }
    if(mp->pcap_device){
    	free(mp->pcap_device);
    	mp->pcap_device = NULL;
    }
    free(mp);
    nsi->pcap = NULL;
  }
#endif

  gh_list_remove_elem(&nsi->nsp->active_iods, nsi->entry_in_nsp_active_iods);
  gh_list_prepend(&nsi->nsp->free_iods, nsi);

}

/* Returns the ID of an nsock_iod .  This ID is always unique amongst
   ids for a given nspool (unless you blow through billions of them). */
unsigned long nsi_id(nsock_iod nsockiod) {
  assert(nsockiod);
  return ((msiod *)nsockiod)->id;
}


/* Returns the SSL_SESSION of an nsock_iod, and increments it's usage count */
nsock_ssl_session nsi_get1_ssl_session(nsock_iod nsockiod) {
#if HAVE_OPENSSL
  return SSL_get1_session(((msiod *)nsockiod)->ssl);
#else
  return NULL;
#endif
}

/* Returns the SSL_SESSION without incrementing usage count */
nsock_ssl_session nsi_get0_ssl_session(nsock_iod nsockiod) {
#if HAVE_OPENSSL
  return SSL_get0_session(((msiod *)nsockiod)->ssl);
#else
  return NULL;
#endif
}

/* sets the ssl session of an nsock_iod, increments usage count.  The
 session should not have been set yet (as no freeing is done) */
#if HAVE_OPENSSL
void nsi_set_ssl_session(msiod *iod, SSL_SESSION *sessid) {
  if (sessid) {
    iod->ssl_session = sessid;

    /* No reference counting for the copy stored briefly in nsiod */
  }
}
#endif

/* Sometimes it is useful to store a pointer to information inside
   the msiod so you can retrieve it during a callback. */
void nsi_setud(nsock_iod nsockiod, void *data) {
  assert(nsockiod);
  ((msiod *)nsockiod)->userdata = data;
}

/* And the function above wouldn't make much sense if we didn't have a way
   to retrieve that data ... */
void *nsi_getud(nsock_iod nsockiod) {
  assert(nsockiod);
  return ((msiod *)nsockiod)->userdata;
}

  /* Returns 1 if an NSI is communicating via SSL, 0 otherwise */
int nsi_checkssl(nsock_iod nsockiod) {
  return ((msiod *)nsockiod)->ssl? 1 : 0;
}

/* Returns the remote peer port (or -1 if unavailable).  Note the
   return value is a whole int so that -1 can be distinguished from
   65535.  Port is returned in host byte order. */
int nsi_peerport(msiod *nsi) {
  int fam;
  if (nsi->peerlen <= 0)
    return -1;

  fam = ((struct sockaddr_in *) &nsi->peer)->sin_family;

  if (fam == AF_INET)
    return ntohs(((struct sockaddr_in *) &nsi->peer)->sin_port);
#if HAVE_IPV6
  else if (fam == AF_INET6)
    return ntohs(((struct sockaddr_in6 *) &nsi->peer)->sin6_port);
#endif

  return -1;
}

/* Sets the local address to bind to before connect() */
int nsi_set_localaddr(nsock_iod nsi, struct sockaddr_storage *ss, size_t sslen)
{
	msiod *iod = (msiod *) nsi;

	assert(iod);

	if (sslen > sizeof(iod->local))
		return -1;

	memcpy(&iod->local, ss, sslen);
	iod->locallen = sslen;
	return 0;
}

/* Sets IPv4 options to apply before connect().  It makes a copy of the 
 * options, so you can free() yours if necessary.  This copy is freed
 * when the iod is destroyed
 */
int nsi_set_ipoptions(nsock_iod nsi, void *opts, size_t optslen)
{
	msiod *iod = (msiod *) nsi;

	assert(iod);

	if (optslen > 44)
		return -1;

	iod->ipopts = safe_malloc(optslen);
	memcpy(iod->ipopts, opts, optslen);
	iod->ipoptslen = optslen;
	return 0;
}

/* I didn't want to do this.  Its an ugly hack, but I suspect it will
   be neccessary.  I certainly can't reproduce in nsock EVERYTHING you
   might want to do with a socket.  So I'm offering you this function
   to obtain the socket descriptor which is (usually) wrapped in a
   nsock_iod).  You can do "reasonable" things with it, like setting
   socket receive buffers.  But don't create havok by closing the
   descriptor!  If the descriptor you get back is -1, the iod does not
   currently possess a valid descriptor */
int nsi_getsd(nsock_iod nsockiod) {
  assert(nsockiod);
  return ((msiod *)nsockiod)->sd;
}
