/*
 * Copyright 1990,91 by Thomas Roell, Dinkelscherben, Germany.
 *
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the name of Thomas Roell not be used in
 * advertising or publicity pertaining to distribution of the software without
 * specific, written prior permission.  Thomas Roell makes no representations
 * about the suitability of this software for any purpose.  It is provided
 * "as is" without express or implied warranty.
 *
 * THOMAS ROELL DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE,
 * INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO
 * EVENT SHALL THOMAS ROELL BE LIABLE FOR ANY SPECIAL, INDIRECT OR
 * CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE,
 * DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
 * TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 *
 */
/*
 * Copyright (c) 1994-2003 by The XFree86 Project, Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE COPYRIGHT HOLDER(S) OR AUTHOR(S) BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * Except as contained in this notice, the name of the copyright holder(s)
 * and author(s) shall not be used in advertising or otherwise to promote
 * the sale, use or other dealings in this Software without prior written
 * authorization from the copyright holder(s) and author(s).
 */

/* [JCH-96/01/21] Extended std reverse map to four buttons. */

#ifdef HAVE_XORG_CONFIG_H
#include <xorg-config.h>
#endif

#include <X11/X.h>
#include <X11/Xpoll.h>
#include <X11/Xproto.h>
#include "misc.h"

#include "compiler.h"

#include "xf86.h"
#include "xf86Priv.h"
#define XF86_OS_PRIVS
#include "xf86_OSlib.h"
#include "atKeynames.h"


#ifdef XFreeXDGA
#include "dgaproc.h"
#endif

#ifdef XINPUT
#include <X11/extensions/XI.h>
#include <X11/extensions/XIproto.h>
#else
#include "inputstr.h"
#endif
#include "xf86Xinput.h"

#include "mi.h"
#include "mipointer.h"

#ifdef XF86BIGFONT
#define _XF86BIGFONT_SERVER_
#include <X11/extensions/xf86bigfont.h>
#endif

#ifdef XKB
extern Bool noXkbExtension;
#endif

#ifdef DPMSExtension
#define DPMS_SERVER
#include <X11/extensions/dpms.h>
#include "dpmsproc.h"
#endif

#define XE_POINTER  1
#define XE_KEYBOARD 2

#define EqEnqueue(pDev, ev) { \
    int __sigstate = xf86BlockSIGIO (); \
    mieqEnqueue (pDev, ev); \
    xf86UnblockSIGIO(__sigstate); \
}

/*
 * The first of many hacks to get VT switching to work under
 * Solaris 2.1 for x86. The basic problem is that Solaris is supposed
 * to be SVR4. It is for the most part, except where the video interface
 * is concerned.  These hacks work around those problems.
 * See the comments for Linux, and SCO.
 *
 * This is a toggling variable:
 *  FALSE = No VT switching keys have been pressed last time around
 *  TRUE  = Possible VT switch Pending
 * (DWH - 12/2/93)
 *
 * This has been generalised to work with Linux and *BSD+syscons (DHD)
 */

_X_EXPORT Bool VTSwitchEnabled = TRUE;	/* Allows run-time disabling for
                                         *BSD and for avoiding VT
                                         switches when using the DRI
                                         automatic full screen mode.*/

extern fd_set EnabledDevices;

#ifdef XF86PM
extern void (*xf86OSPMClose)(void);
#endif

static void xf86VTSwitch(void);

/*
 * Allow arbitrary drivers or other XFree86 code to register with our main
 * Wakeup handler.
 */
typedef struct x_IHRec {
    int			fd;
    InputHandlerProc	ihproc;
    pointer		data;
    Bool		enabled;
    struct x_IHRec *	next;
} IHRec, *IHPtr;

static IHPtr InputHandlers = NULL;


Bool
LegalModifier(unsigned int key, DeviceIntPtr pDev)
{
    return TRUE;
}

/*
 * TimeSinceLastInputEvent --
 *      Function used for screensaver purposes by the os module. Returns the
 *      time in milliseconds since there last was any input.
 */

int
TimeSinceLastInputEvent()
{
  if (xf86Info.lastEventTime == 0) {
    xf86Info.lastEventTime = GetTimeInMillis();
  }
  return GetTimeInMillis() - xf86Info.lastEventTime;
}



/*
 * SetTimeSinceLastInputEvent --
 *      Set the lastEventTime to now.
 */

_X_EXPORT void
SetTimeSinceLastInputEvent()
{
  xf86Info.lastEventTime = GetTimeInMillis();
}



/*
 * ProcessInputEvents --
 *      Retrieve all waiting input events and pass them to DIX in their
 *      correct chronological order. Only reads from the system pointer
 *      and keyboard.
 */

void
ProcessInputEvents ()
{
  int x, y;
#ifdef INHERIT_LOCK_STATE
  static int generation = 0;
#endif

    /*
     * With INHERIT_LOCK_STATE defined, the initial state of CapsLock, NumLock
     * and ScrollLock will be set to match that of the VT the server is
     * running on.
     */
#ifdef INHERIT_LOCK_STATE
    if (generation != serverGeneration) {
      xEvent kevent;
      DevicePtr pKeyboard = xf86Info.pKeyboard;
      extern unsigned int xf86InitialCaps, xf86InitialNum, xf86InitialScroll;

      generation = serverGeneration;
      kevent.u.keyButtonPointer.time = GetTimeInMillis();
      kevent.u.keyButtonPointer.rootX = 0;
      kevent.u.keyButtonPointer.rootY = 0;
      kevent.u.u.type = KeyPress;


      if (xf86InitialCaps) {
        kevent.u.u.detail = xf86InitialCaps;
        (* pKeyboard->processInputProc)(&kevent, (DeviceIntPtr)pKeyboard, 1);
        xf86InitialCaps = 0;
      }
      if (xf86InitialNum) {
        kevent.u.u.detail = xf86InitialNum;
        (* pKeyboard->processInputProc)(&kevent, (DeviceIntPtr)pKeyboard, 1);
        xf86InitialNum = 0;
      }
      if (xf86InitialScroll) {
        kevent.u.u.detail = xf86InitialScroll;
        (* pKeyboard->processInputProc)(&kevent, (DeviceIntPtr)pKeyboard, 1);
        xf86InitialScroll = 0;
      }
    }
#endif

  xf86Info.inputPending = FALSE;

  mieqProcessInputEvents();
  miPointerUpdateSprite(inputInfo.pointer);

  miPointerGetPosition(inputInfo.pointer, &x, &y);
  xf86SetViewport(xf86Info.currentScreen, x, y);
}

void
xf86GrabServerCallback(CallbackListPtr *callbacks, pointer data, pointer args)
{
    ServerGrabInfoRec *grab = (ServerGrabInfoRec*)args;

    xf86Info.grabInfo.server.client = grab->client;
    xf86Info.grabInfo.server.grabstate = grab->grabstate;
}

/*
 * Handle keyboard events that cause some kind of "action"
 * (i.e., server termination, video mode changes, VT switches, etc.)
 */
void
xf86ProcessActionEvent(ActionEvent action, void *arg)
{
#ifdef DEBUG
    ErrorF("ProcessActionEvent(%d,%x)\n", (int) action, arg);
#endif
    switch (action) {
    case ACTION_TERMINATE:
	if (!xf86Info.dontZap) {
#ifdef XFreeXDGA
	    DGAShutdown();
#endif
	    GiveUp(0);
	}
	break;
    case ACTION_NEXT_MODE:
	if (!xf86Info.dontZoom)
	    xf86ZoomViewport(xf86Info.currentScreen,  1);
	break;
    case ACTION_PREV_MODE:
	if (!xf86Info.dontZoom)
	    xf86ZoomViewport(xf86Info.currentScreen, -1);
	break;
    case ACTION_DISABLEGRAB:
	if (!xf86Info.grabInfo.disabled && xf86Info.grabInfo.allowDeactivate) {
	  if (inputInfo.pointer && inputInfo.pointer->grab != NULL &&
	      inputInfo.pointer->DeactivateGrab)
	    inputInfo.pointer->DeactivateGrab(inputInfo.pointer);
	  if (inputInfo.keyboard && inputInfo.keyboard->grab != NULL &&
	      inputInfo.keyboard->DeactivateGrab)
	    inputInfo.keyboard->DeactivateGrab(inputInfo.keyboard);
	}
	break;
    case ACTION_CLOSECLIENT:
	if (!xf86Info.grabInfo.disabled && xf86Info.grabInfo.allowClosedown) {
	  ClientPtr pointer, keyboard, server;

	  pointer = keyboard = server = NULL;
	  if (inputInfo.pointer && inputInfo.pointer->grab != NULL)
	    pointer = clients[CLIENT_ID(inputInfo.pointer->grab->resource)];
	  if (inputInfo.keyboard && inputInfo.keyboard->grab != NULL) {
	    keyboard = clients[CLIENT_ID(inputInfo.keyboard->grab->resource)];
	    if (keyboard == pointer)
	      keyboard = NULL;
	  }
	  if ((xf86Info.grabInfo.server.grabstate == SERVER_GRABBED) &&
	      (((server = xf86Info.grabInfo.server.client) == pointer) ||
	       (server == keyboard)))
	      server = NULL;

	  if (pointer)
	    CloseDownClient(pointer);
	  if (keyboard)
	    CloseDownClient(keyboard);
	  if (server)
	    CloseDownClient(server);
	}
	break;
#if !defined(__SOL8__) && !defined(sgi) && \
    (!defined(sun) || defined(__i386__)) && defined(VT_ACTIVATE)
    case ACTION_SWITCHSCREEN:
	if (VTSwitchEnabled && !xf86Info.dontVTSwitch && arg) {
	    int vtno = *((int *) arg);
#if defined(__SCO__) || defined(__UNIXWARE__)
	    vtno--;
#endif
#if defined(QNX4)
	    xf86Info.vtRequestsPending = vtno;
#else
	    if (ioctl(xf86Info.consoleFd, VT_ACTIVATE, vtno) < 0)
		ErrorF("Failed to switch consoles (%s)\n", strerror(errno));
#endif
	}
	break;
    case ACTION_SWITCHSCREEN_NEXT:
	if (VTSwitchEnabled && !xf86Info.dontVTSwitch) {
/* Shouldn't this be true for (sun) && (i386) && (SVR4) ? */
#if defined(__SCO__) || defined(__UNIXWARE__)
	    if (ioctl(xf86Info.consoleFd, VT_ACTIVATE, xf86Info.vtno) < 0)
#else
	    if (ioctl(xf86Info.consoleFd, VT_ACTIVATE, xf86Info.vtno + 1) < 0)
#endif
#if defined (__SCO__) || (defined(sun) && defined (__i386__) && defined (SVR4)) || defined(__UNIXWARE__)
		if (ioctl(xf86Info.consoleFd, VT_ACTIVATE, 0) < 0)
#else
		if (ioctl(xf86Info.consoleFd, VT_ACTIVATE, 1) < 0)
#endif
		    ErrorF("Failed to switch consoles (%s)\n", strerror(errno));
	}
	break;
    case ACTION_SWITCHSCREEN_PREV:
	if (VTSwitchEnabled && !xf86Info.dontVTSwitch && xf86Info.vtno > 0) {
	    if (ioctl(xf86Info.consoleFd, VT_ACTIVATE, xf86Info.vtno - 1) < 0)
		ErrorF("Failed to switch consoles (%s)\n", strerror(errno));
	}
	break;
#endif
    case ACTION_MESSAGE:
        {
            char *retstr, *message = (char *) arg;
	    ScrnInfoPtr pScr = XF86SCRNINFO(xf86Info.currentScreen);

#ifdef DEBUG
            ErrorF("ActionMessage: '%s'\n", message);
#endif
	    /* Okay the message made it to the ddx.  The common layer */
	    /* can check for relevant messages here and react to any  */
	    /* that have a global effect.  For example:               */
	    /*                                                        */
	    /* if (!strcmp(message, "foo") {                          */
	    /*      do_foo(); break                                   */
	    /* }                                                      */
	    /*                                                        */
	    /* otherwise fallback to sending a key event message to   */
	    /* the current screen's driver:                           */
	    if (*pScr->HandleMessage != NULL) {
		(void) (*pScr->HandleMessage)(pScr->scrnIndex,
			"KeyEventMessage", message, &retstr);
	    }
        }
	break;
    default:
	break;
    }
}

#define ModifierIsSet(k) ((modifiers & (k)) == (k))

_X_EXPORT Bool
xf86CommonSpecialKey(int key, Bool down, int modifiers)
{
  if ((!ModifierIsSet(ShiftMask)) &&
      (((ModifierIsSet(ControlMask | AltMask)) ||
        (ModifierIsSet(ControlMask | AltLangMask))))) {
      switch (key) {
	
      case KEY_BackSpace:
	xf86ProcessActionEvent(ACTION_TERMINATE, NULL);
	break;

      /*
       * Check grabs
       */
      case KEY_KP_Divide:
	xf86ProcessActionEvent(ACTION_DISABLEGRAB, NULL);
	break;
      case KEY_KP_Multiply:
	xf86ProcessActionEvent(ACTION_CLOSECLIENT, NULL);
	break;
	
	/*
	 * The idea here is to pass the scancode down to a list of
	 * registered routines. There should be some standard conventions
	 * for processing certain keys.
	 */
      case KEY_KP_Minus:   /* Keypad - */
	if (down) xf86ProcessActionEvent(ACTION_PREV_MODE, NULL);
	if (!xf86Info.dontZoom) return TRUE;
	break;
	
      case KEY_KP_Plus:   /* Keypad + */
	if (down) xf86ProcessActionEvent(ACTION_NEXT_MODE, NULL);
	if (!xf86Info.dontZoom) return TRUE;
	break;
      }
  }
  return FALSE;
}

/*
 * xf86Wakeup --
 *      Os wakeup handler.
 */

/* ARGSUSED */
void
xf86Wakeup(pointer blockData, int err, pointer pReadmask)
{
#if !defined(__QNX__)
    fd_set* LastSelectMask = (fd_set*)pReadmask;
    fd_set devicesWithInput;
    InputInfoPtr pInfo;

    if (err >= 0) {

	XFD_ANDSET(&devicesWithInput, LastSelectMask, &EnabledDevices);
	if (XFD_ANYSET(&devicesWithInput)) {
	    pInfo = xf86InputDevs;
	    while (pInfo) {
		if (pInfo->read_input && pInfo->fd >= 0 &&
		    (FD_ISSET(pInfo->fd, &devicesWithInput) != 0)) {
		    int sigstate = xf86BlockSIGIO();

		    pInfo->read_input(pInfo);
		    xf86UnblockSIGIO(sigstate);
		    /*
		     * Remove the descriptior from the set because more than one
		     * device may share the same file descriptor.
		     */
		    FD_CLR(pInfo->fd, &devicesWithInput);
		}
		pInfo = pInfo->next;
	    }
	}
    }
#else   /* __QNX__ */

    InputInfoPtr pInfo;

    pInfo = xf86InputDevs;
    while (pInfo) {
		if (pInfo->read_input && pInfo->fd >= 0) {
		    int sigstate = xf86BlockSIGIO();

		    pInfo->read_input(pInfo);
		    xf86UnblockSIGIO(sigstate);
		    /*
		     * Must break here because more than one device may share
		     * the same file descriptor.
		     */
		    break;
		}
		pInfo = pInfo->next;
    }

#endif  /* __QNX__ */

    if (err >= 0) { /* we don't want the handlers called if select() */
	IHPtr ih;   /* returned with an error condition, do we?      */
	
	for (ih = InputHandlers; ih; ih = ih->next) {
	    if (ih->enabled && ih->fd >= 0 && ih->ihproc &&
		(FD_ISSET(ih->fd, ((fd_set *)pReadmask)) != 0)) {
		ih->ihproc(ih->fd, ih->data);
	    }
	}
    }

    if (xf86VTSwitchPending()) xf86VTSwitch();

    if (xf86Info.inputPending) ProcessInputEvents();
}


/*
 * xf86SigioReadInput --
 *    signal handler for the SIGIO signal.
 */
static void
xf86SigioReadInput(int fd,
		   void *closure)
{
    int errno_save = errno;
    int sigstate = xf86BlockSIGIO();
    InputInfoPtr pInfo = (InputInfoPtr) closure;

    pInfo->read_input(pInfo);

    xf86UnblockSIGIO(sigstate);
    errno = errno_save;
}

/*
 * xf86AddEnabledDevice --
 *
 */
_X_EXPORT void
xf86AddEnabledDevice(InputInfoPtr pInfo)
{
    if (!xf86InstallSIGIOHandler (pInfo->fd, xf86SigioReadInput, pInfo)) {
	AddEnabledDevice(pInfo->fd);
    }
}

/*
 * xf86RemoveEnabledDevice --
 *
 */
_X_EXPORT void
xf86RemoveEnabledDevice(InputInfoPtr pInfo)
{
    if (!xf86RemoveSIGIOHandler (pInfo->fd)) {
	RemoveEnabledDevice(pInfo->fd);
    }
}

static int *xf86SignalIntercept = NULL;

_X_EXPORT void
xf86InterceptSignals(int *signo)
{
    if ((xf86SignalIntercept = signo))
	*signo = -1;
}

static void (*xf86SigIllHandler)(void) = NULL;

_X_EXPORT void
xf86InterceptSigIll(void (*sigillhandler)(void))
{
    xf86SigIllHandler = sigillhandler;
}

#ifdef HAVE_BACKTRACE
#include <execinfo.h>

static __inline__ void xorg_backtrace(void)
{
    void *array[32]; /* deeper nesting than this means something's wrong */
    size_t size, i;
    char **strings;
    ErrorF("\nBacktrace:\n");
    size = backtrace(array, 32);
    strings = backtrace_symbols(array, size);
    for (i = 0; i < size; i++)
        ErrorF("%d: %s\n", i, strings[i]);
    free(strings);
}

#else /* not glibc or glibc < 2.1 */

# if defined(sun) && defined(__SVR4)
#  define HAVE_PSTACK
# endif

# if defined(HAVE_WALKCONTEXT) /* Solaris 9 & later */

# include <ucontext.h>
# include <signal.h>
# include <dlfcn.h>
# include <sys/elf.h>

#ifdef _LP64
# define ElfSym Elf64_Sym
#else
# define ElfSym Elf32_Sym
#endif

/* Called for each frame on the stack to print it's contents */
static int xorg_backtrace_frame(uintptr_t pc, int signo, void *arg)
{
    Dl_info dlinfo;
    ElfSym *dlsym;
    char header[32];
    int depth = *((int *) arg);

    if (signo) {
	char signame[SIG2STR_MAX];

	if (sig2str(signo, signame) != 0) {
	    strcpy(signame, "unknown");
	}

	ErrorF("** Signal %d (%s)\n", signo, signame);
    }

    snprintf(header, sizeof(header), "%d: 0x%lx", depth, pc);
    *((int *) arg) = depth + 1;

    /* Ask system dynamic loader for info on the address */
    if (dladdr1((void *) pc, &dlinfo, (void **) &dlsym, RTLD_DL_SYMENT)) {
	unsigned long offset = pc - (uintptr_t) dlinfo.dli_saddr;
	const char *symname;
	
	if (offset < dlsym->st_size) { /* inside a function */
	    symname = dlinfo.dli_sname;
	} else { /* found which file it was in, but not which function */
	    symname = "<section start>";
	    offset = pc - (uintptr_t)dlinfo.dli_fbase;
	}
	ErrorF("%s: %s:%s+0x%lx\n", header, dlinfo.dli_fname,
	       symname, offset);

    } else {
	/* Couldn't find symbol info from system dynamic loader, should
	 * probably poke elfloader here, but haven't written that code yet,
	 * so we just print the pc.
	 */
	ErrorF("%s\n", header);
    }

    return 0;
}
# endif /* HAVE_WALKCONTEXT */

# ifdef HAVE_PSTACK
static int xorg_backtrace_pstack(void) {
    pid_t kidpid;
    int pipefd[2];

    if (pipe(pipefd) != 0) {
	return -1;
    }

    kidpid = fork1();

    if (kidpid == -1) {
	/* ERROR */
	return -1;
    } else if (kidpid == 0) {
	/* CHILD */
	char parent[16];
	
	seteuid(0);
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	dup2(pipefd[1],STDOUT_FILENO);
	closefrom(STDERR_FILENO);

	snprintf(parent, sizeof(parent), "%d", getppid());
	execle("/usr/bin/pstack", "pstack", parent, NULL);
	exit(1);
    } else {
	/* PARENT */
	char btline[256];
	int kidstat;
	int bytesread;
	int done = 0;
	
	close(pipefd[1]);

	while (!done) {
	    bytesread = read(pipefd[0], btline, sizeof(btline) - 1);

	    if (bytesread > 0) {
		btline[bytesread] = 0;
		ErrorF("%s", btline);
	    }
	    else if ((bytesread < 0) ||
		     ((errno != EINTR) && (errno != EAGAIN)))
		done = 1;
	}
	close(pipefd[0]);
	waitpid(kidpid, &kidstat, 0);
	if (kidstat != 0)
	    return -1;
    }
    return 0;
}
# endif /* HAVE_PSTACK */


# if defined(HAVE_PSTACK) || defined(HAVE_WALKCONTEXT)

static __inline__ void xorg_backtrace(void) {

    ErrorF("\nBacktrace:\n");

#  ifdef HAVE_PSTACK
/* First try fork/exec of pstack - otherwise fall back to walkcontext
   pstack is preferred since it can print names of non-exported functions */

    if (xorg_backtrace_pstack() < 0)
#  endif	
    {
#  ifdef HAVE_WALKCONTEXT
	ucontext_t u;
	int depth = 1;
	
	if (getcontext(&u) == 0)
	    walkcontext(&u, xorg_backtrace_frame, &depth);
	else
#  endif
	    Error("Failed to get backtrace info");
    }
    ErrorF("\n");	
}

# else

/* Default fallback if we can't find any way to get a backtrace */
static __inline__ void xorg_backtrace(void) { return; }

# endif
#endif

/*
 * xf86SigHandler --
 *    Catch unexpected signals and exit or continue cleanly.
 */
void
xf86SigHandler(int signo)
{
  if ((signo == SIGILL) && xf86SigIllHandler) {
    (*xf86SigIllHandler)();
    /* Re-arm handler just in case we unexpectedly return here */
    (void) signal(signo, xf86SigHandler);
    return;
  }

  if (xf86SignalIntercept && (*xf86SignalIntercept < 0)) {
    *xf86SignalIntercept = signo;
    /* Re-arm handler just in case */
    (void) signal(signo, xf86SigHandler);
    return;
  }

  signal(signo,SIG_IGN);
  xf86Info.caughtSignal = TRUE;
#ifdef XF86BIGFONT
  XF86BigfontCleanup();
#endif

  xorg_backtrace();

  FatalError("Caught signal %d.  Server aborting\n", signo);
}

static void
xf86ReleaseKeys(DeviceIntPtr pDev)
{
    KeyClassPtr keyc = NULL;
    KeySym *map = NULL;
    xEvent ke;
    int i = 0, j = 0, nevents = 0;

    if (!pDev || !pDev->key)
        return;

    keyc = pDev->key;
    map = keyc->curKeySyms.map;

    /*
     * Hmm... here is the biggest hack of every time !
     * It may be possible that a switch-vt procedure has finished BEFORE
     * you released all keys neccessary to do this. That peculiar behavior
     * can fool the X-server pretty much, cause it assumes that some keys
     * were not released. TWM may stuck alsmost completly....
     * OK, what we are doing here is after returning from the vt-switch
     * exeplicitely unrelease all keyboard keys before the input-devices
     * are reenabled.
     */

    for (i = keyc->curKeySyms.minKeyCode, map = keyc->curKeySyms.map;
         i < keyc->curKeySyms.maxKeyCode;
         i++, map += keyc->curKeySyms.mapWidth) {
        if (KeyPressed(i)) {
            switch (*map) {
            /* Don't release the lock keys */
            case XK_Caps_Lock:
            case XK_Shift_Lock:
            case XK_Num_Lock:
            case XK_Scroll_Lock:
            case XK_Kana_Lock:
                break;
            default:
                if (pDev == inputInfo.keyboard) {
                    ke.u.keyButtonPointer.time = GetTimeInMillis();
                    ke.u.keyButtonPointer.rootX = 0;
                    ke.u.keyButtonPointer.rootY = 0;
                    ke.u.u.type = KeyRelease;
                    ke.u.u.detail = i;
                    (*pDev->public.processInputProc) (&ke, pDev, 1);
                }
                else {
		    int sigstate = xf86BlockSIGIO ();
                    nevents = GetKeyboardEvents(xf86Events, pDev, KeyRelease, i);
                    for (j = 0; j < nevents; j++)
                        mieqEnqueue(pDev, xf86Events + j);
		    xf86UnblockSIGIO(sigstate);
                }
                break;
            }
        }
    }
}

/*
 * xf86VTSwitch --
 *      Handle requests for switching the vt.
 */
static void
xf86VTSwitch()
{
  int i, prevSIGIO;
  InputInfoPtr pInfo;
  IHPtr ih;

#ifdef DEBUG
  ErrorF("xf86VTSwitch()\n");
#endif

#ifdef XFreeXDGA
  if(!DGAVTSwitch())
	return;
#endif

  /*
   * Since all screens are currently all in the same state it is sufficient
   * check the first.  This might change in future.
   */
  if (xf86Screens[0]->vtSema) {

#ifdef DEBUG
    ErrorF("xf86VTSwitch: Leaving, xf86Exiting is %s\n",
	   BOOLTOSTRING((dispatchException & DE_TERMINATE) ? TRUE : FALSE));
#endif
#ifdef DPMSExtension
    if (DPMSPowerLevel != DPMSModeOn)
	DPMSSet(serverClient, DPMSModeOn);
#endif
    for (i = 0; i < xf86NumScreens; i++) {
      if (!(dispatchException & DE_TERMINATE))
	if (xf86Screens[i]->EnableDisableFBAccess)
	  (*xf86Screens[i]->EnableDisableFBAccess) (i, FALSE);
    }

    /*
     * Keep the order: Disable Device > LeaveVT
     *                        EnterVT > EnableDevice
     */
    pInfo = xf86InputDevs;
    while (pInfo) {
      if (pInfo->dev)
          DisableDevice(pInfo->dev);
      pInfo = pInfo->next;
    }
    xf86EnterServerState(SETUP);
    for (i = 0; i < xf86NumScreens; i++)
	xf86Screens[i]->LeaveVT(i, 0);

    for (ih = InputHandlers; ih; ih = ih->next)
      xf86DisableInputHandler(ih);
    xf86AccessLeave();      /* We need this here, otherwise */
    xf86AccessLeaveState(); /* console won't be restored    */

    if (!xf86VTSwitchAway()) {
      /*
       * switch failed
       */

#ifdef DEBUG
      ErrorF("xf86VTSwitch: Leave failed\n");
#endif
      prevSIGIO = xf86BlockSIGIO();
      xf86AccessEnter();
      xf86EnterServerState(SETUP);
      for (i = 0; i < xf86NumScreens; i++) {
	if (!xf86Screens[i]->EnterVT(i, 0))
	  FatalError("EnterVT failed for screen %d\n", i);
      }
      xf86EnterServerState(OPERATING);
      if (!(dispatchException & DE_TERMINATE)) {
	for (i = 0; i < xf86NumScreens; i++) {
	  if (xf86Screens[i]->EnableDisableFBAccess)
	    (*xf86Screens[i]->EnableDisableFBAccess) (i, TRUE);
	}
      }
      dixSaveScreens(serverClient, SCREEN_SAVER_FORCER, ScreenSaverReset);

      pInfo = xf86InputDevs;
      while (pInfo) {
        if (pInfo->dev) {
            xf86ReleaseKeys(pInfo->dev);
            EnableDevice(pInfo->dev);
        }
	pInfo = pInfo->next;
      }
      /* XXX HACK */
      xf86ReleaseKeys(inputInfo.keyboard);
      for (ih = InputHandlers; ih; ih = ih->next)
        xf86EnableInputHandler(ih);

      xf86UnblockSIGIO(prevSIGIO);

    } else {
#ifdef XF86PM
	  if (xf86OSPMClose)
	      xf86OSPMClose();
	  xf86OSPMClose = NULL;
#endif

	for (i = 0; i < xf86NumScreens; i++) {
 	    /*
 	     * zero all access functions to
 	     * trap calls when switched away.
 	     */
	    xf86Screens[i]->vtSema = FALSE;
	    xf86Screens[i]->access = NULL;
	    xf86Screens[i]->busAccess = NULL;
	}
	if (xorgHWAccess)
	    xf86DisableIO();
    }
  } else {

#ifdef DEBUG
    ErrorF("xf86VTSwitch: Entering\n");
#endif
    if (!xf86VTSwitchTo()) return;

    prevSIGIO = xf86BlockSIGIO();
#ifdef XF86PM
    xf86OSPMClose = xf86OSPMOpen();
#endif

    if (xorgHWAccess)
	xf86EnableIO();
    xf86AccessEnter();
    xf86EnterServerState(SETUP);
    for (i = 0; i < xf86NumScreens; i++) {
      xf86Screens[i]->vtSema = TRUE;
      if (!xf86Screens[i]->EnterVT(i, 0))
	  FatalError("EnterVT failed for screen %d\n", i);
    }
    xf86EnterServerState(OPERATING);
    for (i = 0; i < xf86NumScreens; i++) {
      if (xf86Screens[i]->EnableDisableFBAccess)
	(*xf86Screens[i]->EnableDisableFBAccess)(i, TRUE);
    }

    /* Turn screen saver off when switching back */
    dixSaveScreens(serverClient, SCREEN_SAVER_FORCER, ScreenSaverReset);

    pInfo = xf86InputDevs;
    while (pInfo) {
      if (pInfo->dev) {
          xf86ReleaseKeys(pInfo->dev);
          EnableDevice(pInfo->dev);
      }
      pInfo = pInfo->next;
    }
    /* XXX HACK */
    xf86ReleaseKeys(inputInfo.keyboard);

    for (ih = InputHandlers; ih; ih = ih->next)
      xf86EnableInputHandler(ih);

    xf86UnblockSIGIO(prevSIGIO);
  }
}


/* Input handler registration */

static pointer
addInputHandler(int fd, InputHandlerProc proc, pointer data)
{
    IHPtr ih;

    if (fd < 0 || !proc)
	return NULL;

    ih = xcalloc(sizeof(*ih), 1);
    if (!ih)
	return NULL;

    ih->fd = fd;
    ih->ihproc = proc;
    ih->data = data;
    ih->enabled = TRUE;

    ih->next = InputHandlers;
    InputHandlers = ih;

    return ih;
}

_X_EXPORT pointer
xf86AddInputHandler(int fd, InputHandlerProc proc, pointer data)
{
    IHPtr ih = addInputHandler(fd, proc, data);

    if (ih)
        AddEnabledDevice(fd);
    return ih;
}

_X_EXPORT pointer
xf86AddGeneralHandler(int fd, InputHandlerProc proc, pointer data)
{
    IHPtr ih = addInputHandler(fd, proc, data);

    if (ih)
        AddGeneralSocket(fd);
    return ih;
}

static void
removeInputHandler(IHPtr ih)
{
    IHPtr p;

    if (ih == InputHandlers)
	InputHandlers = ih->next;
    else {
	p = InputHandlers;
	while (p && p->next != ih)
	    p = p->next;
	if (ih)
	    p->next = ih->next;
    }
    xfree(ih);
}

_X_EXPORT int
xf86RemoveInputHandler(pointer handler)
{
    IHPtr ih;
    int fd;

    if (!handler)
	return -1;

    ih = handler;
    fd = ih->fd;

    if (ih->fd >= 0)
	RemoveEnabledDevice(ih->fd);
    removeInputHandler(ih);

    return fd;
}

_X_EXPORT int
xf86RemoveGeneralHandler(pointer handler)
{
    IHPtr ih;
    int fd;

    if (!handler)
	return -1;

    ih = handler;
    fd = ih->fd;

    if (ih->fd >= 0)
	RemoveGeneralSocket(ih->fd);
    removeInputHandler(ih);

    return fd;
}

_X_EXPORT void
xf86DisableInputHandler(pointer handler)
{
    IHPtr ih;

    if (!handler)
	return;

    ih = handler;
    ih->enabled = FALSE;
    if (ih->fd >= 0)
	RemoveEnabledDevice(ih->fd);
}

_X_EXPORT void
xf86DisableGeneralHandler(pointer handler)
{
    IHPtr ih;

    if (!handler)
	return;

    ih = handler;
    ih->enabled = FALSE;
    if (ih->fd >= 0)
	RemoveGeneralSocket(ih->fd);
}

_X_EXPORT void
xf86EnableInputHandler(pointer handler)
{
    IHPtr ih;

    if (!handler)
	return;

    ih = handler;
    ih->enabled = TRUE;
    if (ih->fd >= 0)
	AddEnabledDevice(ih->fd);
}

_X_EXPORT void
xf86EnableGeneralHandler(pointer handler)
{
    IHPtr ih;

    if (!handler)
	return;

    ih = handler;
    ih->enabled = TRUE;
    if (ih->fd >= 0)
	AddGeneralSocket(ih->fd);
}

/*
 * As used currently by the DRI, the return value is ignored.
 */
_X_EXPORT Bool
xf86EnableVTSwitch(Bool new)
{
    static Bool def = TRUE;
    Bool old;

    old = VTSwitchEnabled;
    if (!new) {
	/* Disable VT switching */
	def = VTSwitchEnabled;
	VTSwitchEnabled = FALSE;
    } else {
	/* Restore VT switching to default */
	VTSwitchEnabled = def;
    }
    return old;
}

void
xf86ReloadInputDevs(int sig)
{
  InputInfoPtr pInfo;

  signal(sig, (void(*)(int))xf86ReloadInputDevs);

  pInfo = xf86InputDevs;
  while (pInfo) {
    DisableDevice(pInfo->dev);
    EnableDevice(pInfo->dev);
    pInfo = pInfo->next;
  }

  return;
}

_X_EXPORT void
DDXRingBell(int volume, int pitch, int duration) {
    xf86OSRingBell(volume, pitch, duration);
}
