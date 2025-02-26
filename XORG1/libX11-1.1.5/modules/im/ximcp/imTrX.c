/* $Xorg: imTrX.c,v 1.4 2000/08/17 19:45:15 cpqbld Exp $ */
/******************************************************************

           Copyright 1992 by Sun Microsystems, Inc.
           Copyright 1992, 1993, 1994 by FUJITSU LIMITED

Permission to use, copy, modify, distribute, and sell this software
and its documentation for any purpose is hereby granted without fee,
provided that the above copyright notice appear in all copies and
that both that copyright notice and this permission notice appear
in supporting documentation, and that the name of Sun Microsystems, Inc.
and FUJITSU LIMITED not be used in advertising or publicity pertaining to
distribution of the software without specific, written prior permission.
Sun Microsystems, Inc. and FUJITSU LIMITED makes no representations about
the suitability of this software for any purpose.
It is provided "as is" without express or implied warranty.

Sun Microsystems Inc. AND FUJITSU LIMITED DISCLAIMS ALL WARRANTIES WITH
REGARD TO THIS SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY
AND FITNESS, IN NO EVENT SHALL Sun Microsystems, Inc. AND FUJITSU LIMITED
BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR
IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.

  Author: Hideki Hiura (hhiura@Sun.COM) Sun Microsystems, Inc.
          Takashi Fujiwara     FUJITSU LIMITED 
                               fujiwara@a80.tech.yk.fujitsu.co.jp

******************************************************************/
/* $XFree86: xc/lib/X11/imTrX.c,v 1.3 2003/04/13 19:22:21 dawes Exp $ */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <string.h>
#include <X11/Xatom.h>
#define NEED_EVENTS
#include "Xlibint.h"
#include "Xlcint.h"
#include "Ximint.h"
#include "XimTrInt.h"
#include "XimTrX.h"

Private Bool
_XimXRegisterDispatcher(
    Xim			 im,
    Bool		 (*callback)(
				     Xim, INT16, XPointer, XPointer
				     ),
    XPointer		 call_data)
{
    XIntrCallbackPtr	 rec;
    XSpecRec		*spec = (XSpecRec *)im->private.proto.spec;

    if (!(rec = (XIntrCallbackPtr)Xmalloc(sizeof(XIntrCallbackRec))))
        return False;

    rec->func       = callback;
    rec->call_data  = call_data;
    rec->next       = spec->intr_cb;
    spec->intr_cb   = rec;
    return True;
}

Private void
_XimXFreeIntrCallback(
    Xim			 im)
{
    XSpecRec		*spec = (XSpecRec *)im->private.proto.spec;
    register XIntrCallbackPtr rec, next;

    for (rec = spec->intr_cb; rec;) {
	next = rec->next;
	Xfree(rec);
	rec = next;
    }
    return;
}

Private Bool
_XimXCallDispatcher(Xim im, INT16 len, XPointer data)
{
    register XIntrCallbackRec	*rec;
    XSpecRec		*spec = (XSpecRec *)im->private.proto.spec;

    for (rec = spec->intr_cb; rec; rec = rec->next) {
	if ((*rec->func)(im, len, data, rec->call_data))
	    return True;
    }
    return False;
}

Private Bool
_XimXFilterWaitEvent(
    Display	*d,
    Window	 w,
    XEvent	*ev,
    XPointer	 arg)
{
    Xim		 im = (Xim)arg;
    XSpecRec	*spec = (XSpecRec *)im->private.proto.spec;
    Bool ret;

    spec->ev = (XPointer)ev;
    ret = _XimFilterWaitEvent(im);

    /* 
     * If ev is a pointer to a stack variable, there could be
     * a coredump later on if the pointer is dereferenced.
     * Therefore, reset to NULL to force reinitialization in
     * _XimXRead().
     * 
     * Keep in mind _XimXRead may be called again when the stack
     * is very different.
     */
     spec->ev = (XPointer)NULL;

     return ret;
}

Private Bool
_CheckConnect(
    Display	*display,
    XEvent	*event,
    XPointer	 xim)
{
    Xim		 im = (Xim)xim;
    XSpecRec	*spec = (XSpecRec *)im->private.proto.spec;

    if ((event->type == ClientMessage)
     && (event->xclient.message_type == spec->imconnectid)) {
	return True;
    }
    return False;
}

Private Bool
_XimXConnect(Xim im)
{
    XEvent	 event;
    XSpecRec	*spec = (XSpecRec *)im->private.proto.spec;
    CARD32	 major_code;
    CARD32	 minor_code;

    if (!(spec->lib_connect_wid = XCreateSimpleWindow(im->core.display,
		DefaultRootWindow(im->core.display), 0, 0, 1, 1, 1, 0, 0))) {
	return False;
    }

    event.xclient.type         = ClientMessage;
    event.xclient.display      = im->core.display;
    event.xclient.window       = im->private.proto.im_window;
    event.xclient.message_type = spec->imconnectid;
    event.xclient.format       = 32;
    event.xclient.data.l[0]    = (CARD32)spec->lib_connect_wid;
    event.xclient.data.l[1]    = spec->major_code;
    event.xclient.data.l[2]    = spec->minor_code;
    event.xclient.data.l[3]    = 0;
    event.xclient.data.l[4]    = 0;

    if(event.xclient.data.l[1] == 1 || event.xclient.data.l[1] == 2) {
	XWindowAttributes	 atr;
	long			 event_mask;

	XGetWindowAttributes(im->core.display, spec->lib_connect_wid, &atr);
	event_mask = atr.your_event_mask | PropertyChangeMask;
	XSelectInput(im->core.display, spec->lib_connect_wid, event_mask);
	_XRegisterFilterByType(im->core.display, spec->lib_connect_wid,
			PropertyNotify, PropertyNotify,
			 _XimXFilterWaitEvent, (XPointer)im);
    }

    XSendEvent(im->core.display, im->private.proto.im_window,
		 False, NoEventMask, &event);
    XFlush(im->core.display);

    for (;;) {
	XIfEvent(im->core.display, &event, _CheckConnect, (XPointer)im);
	if (event.xclient.type != ClientMessage) {
	    return False;
	}
	if (event.xclient.message_type == spec->imconnectid)
	    break;
    }

    spec->ims_connect_wid = (Window)event.xclient.data.l[0];
    major_code = (CARD32)event.xclient.data.l[1];
    minor_code = (CARD32)event.xclient.data.l[2];

    if (((major_code == 0) && (minor_code <= 2)) ||
	((major_code == 1) && (minor_code == 0)) ||
	((major_code == 2) && (minor_code <= 1))) {
	spec->major_code = major_code;
	spec->minor_code = minor_code;
    }
    if (((major_code == 0) && (minor_code == 2)) ||
	((major_code == 2) && (minor_code == 1))) {
	spec->BoundarySize = (CARD32)event.xclient.data.l[3];
    }
	
    /* ClientMessage Event Filter */
    _XRegisterFilterByType(im->core.display, spec->lib_connect_wid,
			ClientMessage, ClientMessage,
			 _XimXFilterWaitEvent, (XPointer)im);
    return True;
}

Private Bool
_XimXShutdown(Xim im)
{
    XSpecRec	*spec = (XSpecRec *)im->private.proto.spec;

    if (!spec)
	return True;

    /* ClientMessage Event Filter */
    _XUnregisterFilter(im->core.display,
	    ((XSpecRec *)im->private.proto.spec)->lib_connect_wid,
	    _XimXFilterWaitEvent, (XPointer)im);
    XDestroyWindow(im->core.display,
	    ((XSpecRec *)im->private.proto.spec)->lib_connect_wid);
    _XimXFreeIntrCallback(im);
    Xfree(spec);
    im->private.proto.spec = 0;
    return True;
}

Private char *
_NewAtom(
    char	*atomName)
{
    static int	 sequence = 0;

    (void)sprintf(atomName, "_client%d", sequence);
    sequence = ((sequence < 20) ? sequence + 1 : 0);
    return atomName;
}

Private Bool
_XimXWrite(Xim im, INT16 len, XPointer data)    
{
    Atom	 atom;
    char	 atomName[16];
    XSpecRec	*spec = (XSpecRec *)im->private.proto.spec;
    XEvent	 event;
    CARD8	*p;
    CARD32	 major_code = spec->major_code;
    CARD32	 minor_code = spec->minor_code;
    int		 BoundSize;

    bzero(&event,sizeof(XEvent));
    event.xclient.type         = ClientMessage;
    event.xclient.display      = im->core.display;
    event.xclient.window       = spec->ims_connect_wid;
    if(major_code == 1 && minor_code == 0) {
        BoundSize = 0;
    } else if((major_code == 0 && minor_code == 2) ||
              (major_code == 2 && minor_code == 1)) {
        BoundSize = spec->BoundarySize;
    } else if(major_code == 0 && minor_code == 1) {
        BoundSize = len;
    } else {
        BoundSize = XIM_CM_DATA_SIZE;
    }
    if (len > BoundSize) {
	event.xclient.message_type = spec->improtocolid;
	atom = XInternAtom(im->core.display, _NewAtom(atomName), False);
	XChangeProperty(im->core.display, spec->ims_connect_wid,
			atom, XA_STRING, 8, PropModeAppend,
			(unsigned char *)data, len);
	if(major_code == 0) {
	    event.xclient.format = 32;
	    event.xclient.data.l[0] = (long)len;
	    event.xclient.data.l[1] = (long)atom;
	    XSendEvent(im->core.display, spec->ims_connect_wid,
					False, NoEventMask, &event);
	}
    } else {
	int		 length;

	event.xclient.format = 8;
	for(length = 0 ; length < len ; length += XIM_CM_DATA_SIZE) {
	    p = (CARD8 *)&event.xclient.data.b[0];
	    if((length + XIM_CM_DATA_SIZE) >= len) {
		event.xclient.message_type = spec->improtocolid;
		bzero(p, XIM_CM_DATA_SIZE);
		memcpy((char *)p, (data + length), (len - length));
	    } else {
		event.xclient.message_type = spec->immoredataid;
		memcpy((char *)p, (data + length), XIM_CM_DATA_SIZE);
	    }
	    XSendEvent(im->core.display, spec->ims_connect_wid,
					False, NoEventMask, &event);
	}
    }

    return True;
}

Private Bool
_XimXGetReadData(
    Xim			  im,
    char		 *buf,
    int			  buf_len,
    int			 *ret_len,
    XEvent		 *event)
{
    char		 *data;
    int			  len;

    char		  tmp_buf[XIM_CM_DATA_SIZE];
    XSpecRec		 *spec = (XSpecRec *)im->private.proto.spec;
    unsigned long	  length;
    Atom		  prop;
    int			  return_code;
    Atom		  type_ret;
    int			  format_ret;
    unsigned long	  nitems;
    unsigned long	  bytes_after_ret;
    unsigned char	 *prop_ret;

    if ((event->type == ClientMessage) &&
        !((event->xclient.message_type == spec->improtocolid) ||
          (event->xclient.message_type == spec->immoredataid))) {
         /* This event has nothing to do with us,
          * FIXME should not have gotten here then...
          */
         return False;
    } else if ((event->type == ClientMessage) && (event->xclient.format == 8)) {
        data = event->xclient.data.b;
	if (buf_len >= XIM_CM_DATA_SIZE) {
	    (void)memcpy(buf, data, XIM_CM_DATA_SIZE);
	    *ret_len = XIM_CM_DATA_SIZE;
	} else {
	    (void)memcpy(buf, data, buf_len);
	    len = XIM_CM_DATA_SIZE - buf_len;
	    (void)memcpy(tmp_buf, &data[buf_len], len);
	    bzero(data, XIM_CM_DATA_SIZE);
	    (void)memcpy(data, tmp_buf, len);
	    XPutBackEvent(im->core.display, event);
	    *ret_len = buf_len;
	}
    } else if ((event->type == ClientMessage)
				&& (event->xclient.format == 32)) {
	length = (unsigned long)event->xclient.data.l[0];
	prop   = (Atom)event->xclient.data.l[1];
	return_code = XGetWindowProperty(im->core.display,
		spec->lib_connect_wid, prop, 0L,
		(long)((length + 3)/ 4), True, AnyPropertyType,
		&type_ret, &format_ret, &nitems, &bytes_after_ret, &prop_ret);
	if (return_code != Success || format_ret == 0 || nitems == 0) {
	    if (return_code == Success)
		XFree(prop_ret);
	    return False;
	}
	if (buf_len >= length) {
	    (void)memcpy(buf, prop_ret, (int)nitems);
	    *ret_len  = (int)nitems;
	    if (bytes_after_ret > 0) {
	        XGetWindowProperty(im->core.display,
		    spec->lib_connect_wid, prop, 0L,
		    ((length + bytes_after_ret + 3)/ 4), True, AnyPropertyType,
		    &type_ret, &format_ret, &nitems, &bytes_after_ret,
		    &prop_ret);
	        XChangeProperty(im->core.display, spec->lib_connect_wid, prop,
		    XA_STRING, 8, PropModePrepend, &prop_ret[length],
		    (nitems - length)); 
	    }
	} else {
	    (void)memcpy(buf, prop_ret, buf_len);
	    *ret_len  = buf_len;
	    len = nitems - buf_len;

	    if (bytes_after_ret > 0) {
		XFree(prop_ret);
	        XGetWindowProperty(im->core.display,
		spec->lib_connect_wid, prop, 0L,
		((length + bytes_after_ret + 3)/ 4), True, AnyPropertyType,
		&type_ret, &format_ret, &nitems, &bytes_after_ret, &prop_ret);
	    }
	    XChangeProperty(im->core.display, spec->lib_connect_wid, prop,
		    XA_STRING, 8, PropModePrepend, &prop_ret[buf_len], len); 
	    event->xclient.data.l[0] = (long)len;
	    event->xclient.data.l[1] = (long)prop;
	    XPutBackEvent(im->core.display, event);
	}
	XFree(prop_ret);
    } else if (event->type == PropertyNotify) {
	prop = event->xproperty.atom;
	return_code = XGetWindowProperty(im->core.display,
		spec->lib_connect_wid, prop, 0L,
		1000000L, True, AnyPropertyType,
		&type_ret, &format_ret, &nitems, &bytes_after_ret, &prop_ret);
	if (return_code != Success || format_ret == 0 || nitems == 0) {
	    if (return_code == Success)
		XFree(prop_ret);
	    return False;
	}
	if (buf_len >= nitems) {
	    (void)memcpy(buf, prop_ret, (int)nitems);
	    *ret_len  = (int)nitems;
	} else {
	    (void)memcpy(buf, prop_ret, buf_len);
	    *ret_len  = buf_len;
	    len = nitems - buf_len;
	    XChangeProperty(im->core.display, spec->lib_connect_wid, prop,
		XA_STRING, 8, PropModePrepend, &prop_ret[buf_len], len); 
	}
	XFree(prop_ret);
    }
    return True;
}

Private Bool
_CheckCMEvent(
    Display	*display,
    XEvent	*event,
    XPointer	 xim)
{
    Xim		 im = (Xim)xim;
    XSpecRec	*spec = (XSpecRec *)im->private.proto.spec;
    CARD32	 major_code = spec->major_code;

    if ((event->type == ClientMessage)
     &&((event->xclient.message_type == spec->improtocolid) ||
        (event->xclient.message_type == spec->immoredataid)))
	return True;
    if((major_code == 1 || major_code == 2) &&
       (event->type == PropertyNotify) &&
       (event->xproperty.state == PropertyNewValue))
	return True;
    return False;
}

Private Bool
_XimXRead(Xim im, XPointer recv_buf, int buf_len, int *ret_len)
{
    XEvent	*ev;
    XEvent	 event;
    int		 len;
    XSpecRec	*spec = (XSpecRec *)im->private.proto.spec;
    XPointer	  arg = spec->ev;

    if (!arg) {
	bzero(&event, sizeof(XEvent));
	ev = &event;
	XIfEvent(im->core.display, ev, _CheckCMEvent, (XPointer)im);
    } else {
	ev = (XEvent *)arg;
	spec->ev = (XPointer)NULL;
    }
    if (!(_XimXGetReadData(im, recv_buf, buf_len, &len, ev)))
	return False;
    *ret_len = len;
    return True;
}

Private void
_XimXFlush(Xim im)
{
    XFlush(im->core.display);
    return;
}

Public Bool
_XimXConf(im, address)
    Xim		 im;
    char	*address;
{
    XSpecRec	*spec;

    if (!(spec = (XSpecRec *)Xmalloc(sizeof(XSpecRec))))
	return False;
    bzero(spec, sizeof(XSpecRec));

    spec->improtocolid = XInternAtom(im->core.display, _XIM_PROTOCOL, False);
    spec->imconnectid  = XInternAtom(im->core.display, _XIM_XCONNECT, False);
    spec->immoredataid = XInternAtom(im->core.display, _XIM_MOREDATA, False);
    spec->major_code = MAJOR_TRANSPORT_VERSION;
    spec->minor_code = MINOR_TRANSPORT_VERSION;

    im->private.proto.spec     = (XPointer)spec;
    im->private.proto.connect  = _XimXConnect;
    im->private.proto.shutdown = _XimXShutdown;
    im->private.proto.write    = _XimXWrite;
    im->private.proto.read     = _XimXRead;
    im->private.proto.flush    = _XimXFlush;
    im->private.proto.register_dispatcher  = _XimXRegisterDispatcher;
    im->private.proto.call_dispatcher = _XimXCallDispatcher;

    return True;
}
