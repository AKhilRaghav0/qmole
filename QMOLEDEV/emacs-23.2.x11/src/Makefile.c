# Makefile for GNU Emacs.
# Copyright (C) 1985, 1987, 1988, 1993, 1994, 1995, 1999, 2000, 2001, 2002,
#               2003, 2004, 2005, 2006, 2007, 2008, 2009, 2010
#               Free Software Foundation, Inc.

# This file is part of GNU Emacs.

# GNU Emacs is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.

# GNU Emacs is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with GNU Emacs.  If not, see <http://www.gnu.org/licenses/>.


# Note that this file is edited by msdos/sed1v2.inp for MSDOS.  That
# script may need modifying in sync with changes made here.  Try to
# avoid shell-ism because the DOS build has to use the DOS shell.

# Don't try to replace the cpp processing using autoconf facilities,
# says rms.
# Replacing a particular part of the conditionals to work via autoconf
# is OK.
# Some of the conditionals might be dead now.  Finding them and
# deleting them would be fine.


# Here are the things that we expect ../configure to edit.
# We use $(srcdir) explicitly in dependencies so as not to depend on VPATH.
srcdir=/home/chris/emacs-23.2.x11/src
VPATH=/home/chris/emacs-23.2.x11/src
CC=/usr/bin/gcc
CPP=/usr/bin/gcc -E -no-cpp-precomp
CFLAGS=-g -O2 -Wdeclaration-after-statement -Wno-pointer-sign  
CPPFLAGS=  
LDFLAGS=  
LN_S=ln -s
EXEEXT=
version=23.2
# Substitute an assignment for the MAKE variable, because
# BSD doesn't have it as a default.

# Don't use LIBS.  configure puts stuff in it that either shouldn't be
# linked with Emacs or is duplicated by the cpp stuff below.
# LIBS = -lintl -lncurses -lm -lXext -lXmu -lXt -lSM -lICE -lX11  
LIBOBJS = 

lispsource = ${srcdir}/../lisp/
libsrc = ../lib-src/
etc = ../etc/
oldXMenudir = ../oldXMenu/
lwlibdir = ../lwlib/
lispdir = ../lisp/

# Configuration files for .o files to depend on.
M_FILE = ${srcdir}/m/arm.h
S_FILE = ${srcdir}/s/darwin.h
config_h = config.h $(M_FILE) $(S_FILE)

bootstrap_exe = ../src/bootstrap-emacs${EXEEXT}

OTHER_FILES = 

# ========================== start of cpp stuff =======================
/* From here on, comments must be done in C syntax.  */

C_SWITCH_SYSTEM=

/* just to be sure the sh is used */
SHELL=/bin/sh

#define THIS_IS_MAKEFILE
#define NOT_C_CODE
#include "config.h"

#ifdef AUTO_DEPEND
DEPFLAGS = -MMD -MF deps/$*.d
#endif

/* Do not let the file name mktime.c get messed up.  */
#ifdef mktime
#undef mktime
#endif

/* Use HAVE_X11 as an alias for X11 in this file
   to avoid problems with X11 as a subdirectory name
   in -I and other such options which pass through this file. */

#ifdef X11
#define HAVE_X11
#undef X11
#endif

/* On some machines #define register is done in config;
   do not let it interfere with this file.  */
#undef register

/* GNU libc requires ORDINARY_LINK so that its own crt0 is used.
   GNU/Linux is an exception because it uses a funny variant of GNU libc.  */
#ifdef __GNU_LIBRARY__
#ifndef GNU_LINUX
#define ORDINARY_LINK
#endif
#endif

/* Some machines do not find the standard C libraries in the usual place.  */
#ifndef ORDINARY_LINK
#ifndef LIB_STANDARD
#define LIB_STANDARD -lc
#endif
#else
#ifndef LIB_STANDARD
#define LIB_STANDARD
#endif
#endif

/* Under GNUstep, putting libc on the link line causes problems. */
#ifdef NS_IMPL_GNUSTEP
#undef LIB_STANDARD
#define LIB_STANDARD
#endif

/* Unless inhibited or changed, use -lg to link for debugging.  */
#ifndef LIBS_DEBUG
#define LIBS_DEBUG -lg
#endif

/* Some s/SYSTEM.h files define this to request special libraries.  */
#ifndef LIBS_SYSTEM
#define LIBS_SYSTEM
#endif

/* Some m/MACHINE.h files define this to request special libraries.  */
#ifndef LIBS_MACHINE
#define LIBS_MACHINE
#endif

#ifndef LIB_MATH
#  define LIB_MATH -lm
#endif /* LIB_MATH */

/* Some s/SYSTEM.h files define this to request special switches in ld.  */
#ifndef LD_SWITCH_SYSTEM
#if !defined (__GNUC__) && ((defined (BSD_SYSTEM) && !defined (COFF)))
#define LD_SWITCH_SYSTEM -X
#else /* (defined (BSD_SYSTEM) && !defined (COFF)) */
#define LD_SWITCH_SYSTEM
#endif /* (defined (BSD_SYSTEM) && !defined (COFF)) */
#endif /* LD_SWITCH_SYSTEM */

/* This holds special options for linking temacs
   that should be used for linking anything else.  */
#ifndef LD_SWITCH_SYSTEM_TEMACS
#define LD_SWITCH_SYSTEM_TEMACS
#endif

/* Some s/SYSTEM.h files define this to request special switches
   for compiling temacs.  */
#ifndef C_SWITCH_SYSTEM_TEMACS
#define C_SWITCH_SYSTEM_TEMACS
#endif

/* Some m/MACHINE.h files define this to request special switches in ld.  */
#ifndef LD_SWITCH_MACHINE
#define LD_SWITCH_MACHINE
#endif

/* This holds special options for linking temacs
   that should be used for linking anything else.  */
#ifndef LD_SWITCH_MACHINE_TEMACS
#define LD_SWITCH_MACHINE_TEMACS
#endif

/* Some m/MACHINE.h files define this to request special switches in cc.  */
#ifndef C_SWITCH_MACHINE
#define C_SWITCH_MACHINE
#endif

/* Some s/SYSTEM.h files define this to request special switches in cc.  */
#ifndef C_SWITCH_SYSTEM
#define C_SWITCH_SYSTEM
#endif

/* These macros are for switches specifically related to X Windows.  */
#ifndef C_SWITCH_X_MACHINE
#define C_SWITCH_X_MACHINE
#endif

#ifndef C_SWITCH_X_SYSTEM
#define C_SWITCH_X_SYSTEM
#endif

#ifndef C_SWITCH_X_SITE
#define C_SWITCH_X_SITE
#endif

#ifndef LD_SWITCH_X_SITE
#define LD_SWITCH_X_SITE
#endif

#ifndef LD_SWITCH_X_DEFAULT
#define LD_SWITCH_X_DEFAULT
#endif

#ifndef ORDINARY_LINK

#ifndef CRT0_COMPILE
#define CRT0_COMPILE $(CC) -c $(ALL_CFLAGS)
#endif

#ifndef START_FILES
#ifdef NO_REMAP
#define START_FILES pre-crt0.o /lib/crt0.o
#else /* ! defined (NO_REMAP) */
#define START_FILES ecrt0.o
#endif /* ! defined (NO_REMAP) */
#endif /* START_FILES */
STARTFILES = START_FILES

#else /* ORDINARY_LINK */

/* config.h might want to force START_FILES anyway */
#ifdef START_FILES
STARTFILES = START_FILES
#endif /* START_FILES */

#endif /* not ORDINARY_LINK */


#ifdef NS_IMPL_GNUSTEP
/* Pull in stuff from GNUstep-make. */
FOUNDATION_LIB=gnu
GUI_LIB=gnu
include /Additional/base.make
include /Additional/gui.make
shared=no
#endif

#ifdef HAVE_DBUS
DBUS_CFLAGS = 
DBUS_LIBS = 
DBUS_OBJ = dbusbind.o
#endif

#ifdef HAVE_GCONF
GCONF_CFLAGS = 
GCONF_LIBS = 
#endif

/* DO NOT use -R.  There is a special hack described in lastfile.c
   which is used instead.  Some initialized data areas are modified
   at initial startup, then labeled as part of the text area when
   Emacs is dumped for the first time, and never changed again. */

/* -Demacs is needed to make some files produce the correct version
   for use in Emacs.

   -DHAVE_CONFIG_H is needed for some other files to take advantage of
   the information in ``config.h''.  */

/* C_SWITCH_X_SITE must come before C_SWITCH_X_MACHINE and C_SWITCH_X_SYSTEM
   since it may have -I options that should override those two.  */
ALL_CFLAGS=-Demacs -DHAVE_CONFIG_H $(MYCPPFLAGS) -I. -I${srcdir} C_SWITCH_MACHINE C_SWITCH_SYSTEM C_SWITCH_X_SITE C_SWITCH_X_MACHINE C_SWITCH_X_SYSTEM C_SWITCH_SYSTEM_TEMACS ${CFLAGS_SOUND} ${RSVG_CFLAGS} ${DBUS_CFLAGS} ${GCONF_CFLAGS} ${CFLAGS}     ${DEPFLAGS}
ALL_OBJC_CFLAGS=$(ALL_CFLAGS) 

.SUFFIXES: .m
.c.o:
#ifdef AUTO_DEPEND
	@-test -d deps || mkdir deps
#endif
	$(CC) -c $(CPPFLAGS) $(ALL_CFLAGS) $<
.m.o:
#ifdef AUTO_DEPEND
	@-test -d deps || mkdir deps
#endif
	$(CC) -c $(CPPFLAGS) $(ALL_OBJC_CFLAGS) $<

#ifndef LIBX11_SYSTEM
#define LIBX11_SYSTEM
#endif

#ifndef LIB_X11_LIB
#define LIB_X11_LIB -lX11
#endif

#ifdef HAVE_X_WINDOWS
XMENU_OBJ = xmenu.o
XOBJ= xterm.o xfns.o xselect.o xrdb.o fontset.o xsmfns.o fringe.o image.o \
      xsettings.o xgselect.o

#ifdef HAVE_MENUS

#ifdef USE_GTK
GTK_OBJ= gtkutil.o
#endif

/* The X Menu stuff is present in the X10 distribution, but missing
   from X11.  If we have X10, just use the installed library;
   otherwise, use our own copy.  */
#ifdef HAVE_X11
#ifdef USE_X_TOOLKIT
OLDXMENU=${lwlibdir}liblw.a
LIBXMENU= $(OLDXMENU)
#else /* not USE_X_TOOLKIT */
OLDXMENU= ${oldXMenudir}libXMenu11.a
LIBXMENU= $(OLDXMENU)
#endif /* not USE_X_TOOLKIT */
#else /* not HAVE_X11 */
LIBXMENU= -lXMenu
#endif /* not HAVE_X11 */

#else /* not HAVE_MENUS */

/* Otherwise, do not worry about the menu library at all.  */
LIBXMENU=
#endif /* not HAVE_MENUS */

#ifdef USE_X_TOOLKIT
# define LUCID
# if HAVE_XAW3D
LUCID_LIBW= -lXaw3d
# else
LUCID_LIBW= -lXaw
# endif
# if defined (HAVE_MOTIF_2_1) && defined (HAVE_LIBXP)
#  define LIB_MOTIF_EXTRA -lXp
# else
#  define LIB_MOTIF_EXTRA
# endif
# ifdef LIB_MOTIF
MOTIF_LIBW= LIB_MOTIF LIB_MOTIF_EXTRA
# else
MOTIF_LIBW= -lXm LIB_MOTIF_EXTRA
# endif
LIBW=$(LUCID_LIBW)

#ifdef HAVE_X11XTR6
#ifdef NEED_LIBW
LIBXTR6 = -lSM -lICE -lw
#else
LIBXTR6 = -lSM -lICE
#endif
#endif

#ifndef LIBXMU
#define LIBXMU -lXmu
#endif

LIBXT= $(LIBW) LIBXMU -lXt $(LIBXTR6) -lXext

#else /* not USE_X_TOOLKIT */

#ifdef USE_GTK
LIBW=
OLDXMENU=
LIBXMENU=
#endif /* USE_GTK */

#ifdef HAVE_X_SM
LIBXT=$(LIBW) -lSM -lICE
#else
LIBXT=$(LIBW)
#endif
#endif /* not USE_X_TOOLKIT */

#if HAVE_XFT
XFT_LIBS=
#endif /* HAVE_XFT */

#if HAVE_XPM
#ifndef LIBXPM
#define LIBXPM -lXpm
#endif /* not defined LIBXPM */
#else /* not HAVE_XPM */
#define LIBXPM
#endif /* not HAVE_XPM */

#if HAVE_JPEG
#ifndef LIBJPEG
#define LIBJPEG -ljpeg
#endif /* not defined LIBJPEG */
#else /* not HAVE_JPEG */
#define LIBJPEG
#endif /* not HAVE_JPEG */

#if HAVE_PNG
#ifndef LIBPNG
#define LIBPNG -lpng -lz -lm
#endif /* not defined LIBPNG */
#else /* not HAVE_PNG */
#define LIBPNG
#endif /* not HAVE_PNG */

#if HAVE_TIFF
#ifndef LIBTIFF
#define LIBTIFF -ltiff
#endif /* not defined LIBTIFF */
#else /* not HAVE_TIFF */
#define LIBTIFF
#endif /* not HAVE_TIFF */

#if HAVE_GIF
#ifndef LIBGIF
#define LIBGIF -lgif
#endif /* not defined LIBGIF */
#else /* not HAVE_GIF */
#define LIBGIF
#endif /* not HAVE_GIF */

#ifdef HAVE_X11
/* LD_SWITCH_X_DEFAULT comes after everything else that specifies
   options for where to find X libraries, but before those libraries.  */
X11_LDFLAGS = LD_SWITCH_X_SITE LD_SWITCH_X_DEFAULT
LIBX= $(LIBXMENU) $(X11_LDFLAGS) $(LIBXT) LIBTIFF LIBJPEG LIBPNG LIBGIF LIBXPM LIB_X11_LIB LIBX11_SYSTEM $(XFT_LIBS)
#else /* not HAVE_X11 */
LIBX= $(LIBXMENU) LD_SWITCH_X_SITE
#endif /* not HAVE_X11 */
#endif /* not HAVE_X_WINDOWS */

#if HAVE_GPM
#ifndef LIBGPM
#define LIBGPM -lgpm
#endif /* not defined LIBGPM */
#else /* not HAVE_GPM */
#define LIBGPM
#endif /* not HAVE_GPM */

#if HAVE_LIBRESOLV
#ifndef LIBRESOLV
#define LIBRESOLV -lresolv
#endif /* not defined LIBRESOLV */
#else /* not HAVE_LIBRESOLV */
#define LIBRESOLV
#endif /* not HAVE_LIBRESOLV */

LIBSOUND= 
CFLAGS_SOUND= 

RSVG_LIBS= 
RSVG_CFLAGS= 

#ifndef ORDINARY_LINK
/* Fix linking if compiled with GCC.  */
#ifdef __GNUC__

#ifdef LINKER
#define LINKER_WAS_SPECIFIED
#endif

/* Versions of GCC >= 2.0 put their library, libgcc.a, in obscure
   places that are difficult to figure out at make time.  Fortunately,
   these same versions allow you to pass arbitrary flags on to the
   linker, so there is no reason not to use it as a linker.

   Well, it is not quite perfect.  The "-nostdlib" keeps GCC from
   searching for libraries in its internal directories, so we have to
   ask GCC explicitly where to find libgcc.a.  */

#ifndef LINKER
#define LINKER $(CC) -nostdlib
#endif

#ifndef LIB_GCC
/* Ask GCC where to find libgcc.a.  */
#define LIB_GCC `$(CC) -print-libgcc-file-name`
#endif /* not LIB_GCC */

GNULIB_VAR = LIB_GCC

#ifndef LINKER_WAS_SPECIFIED
/* GCC passes any argument prefixed with -Xlinker directly to the
   linker.  See prefix-args.c for an explanation of why we do not do
   this with the shell''s ``for'' construct.
   Note that some people do not have '.'  in their paths, so we must
   use ./prefix-args.  */
#define YMF_PASS_LDFLAGS(flags) `./prefix-args -Xlinker flags`
#else
#define YMF_PASS_LDFLAGS(flags) flags
#endif

#else /* not __GNUC__ */
GNULIB_VAR =

#endif /* not __GNUC__ */
#endif /* not ORDINARY_LINK */

#ifdef ORDINARY_LINK
LD = $(CC)
#else
#ifdef LINKER
LD=LINKER
#else /* not LINKER */
LD=ld
#endif /* not LINKER */
#endif /* not ORDINARY_LINK */

/* Flags to pass to LD only for temacs.  */
/* Do not split this line with a backslash.  That can cause trouble with
   some cpps.  */
TEMACS_LDFLAGS = LD_SWITCH_SYSTEM LD_SWITCH_SYSTEM_TEMACS LD_SWITCH_MACHINE LD_SWITCH_MACHINE_TEMACS

/* A macro which other sections of Makefile can redefine to munge the
   flags before they are passed to LD.  This is helpful if you have
   redefined LD to something odd, like "gcc".
   (The YMF prefix is a holdover from the old name "ymakefile".)
  */
#ifndef YMF_PASS_LDFLAGS
#define YMF_PASS_LDFLAGS(flags) flags
#endif

/* Allow config.h to specify a replacement file for unexec.c.  */
#ifndef UNEXEC
#define UNEXEC unexec.o
#endif

INTERVALS_H = dispextern.h intervals.h composite.h

GETLOADAVG_LIBS = 

#ifdef MSDOS
#ifdef HAVE_X_WINDOWS
MSDOS_OBJ = dosfns.o msdos.o xmenu.o
#else
MSDOS_OBJ = dosfns.o msdos.o w16select.o xmenu.o
#endif
#endif

#ifdef CYGWIN
CYGWIN_OBJ = sheap.o
#endif

#ifdef HAVE_NS
ns_appdir=/
ns_appbindir=/
ns_appresdir=/
ns_appsrc=
/* Object files for NeXTstep */
NS_OBJ= nsterm.o nsfns.o nsmenu.o nsselect.o nsimage.o nsfont.o \
	fontset.o fringe.o image.o
#endif  /* HAVE_NS */

#ifdef HAVE_WINDOW_SYSTEM
#ifdef HAVE_X_WINDOWS
#if defined (HAVE_XFT)
FONT_DRIVERS = xfont.o ftfont.o xftfont.o ftxfont.o
#elif defined (HAVE_FREETYPE)
FONT_DRIVERS = xfont.o ftfont.o ftxfont.o
#else /* ! defined (HAVE_XFT) && ! defined (HAVE_FREETYPE) */
FONT_DRIVERS = xfont.o
#endif /* ! defined (HAVE_XFT) && ! defined (HAVE_FREETYPE) */
#endif /* HAVE_X_WINDOWS */
#endif /* HAVE_WINDOW_SYSTEM */

/* lastfile must follow all files
   whose initialized data areas should be dumped as pure by dump-emacs.  */
obj=    dispnew.o frame.o scroll.o xdisp.o menu.o $(XMENU_OBJ) window.o \
	charset.o coding.o category.o ccl.o character.o chartab.o \
	cm.o term.o terminal.o xfaces.o $(XOBJ) $(GTK_OBJ) $(DBUS_OBJ) \
	emacs.o keyboard.o macros.o keymap.o sysdep.o \
	buffer.o filelock.o insdel.o marker.o \
	minibuf.o fileio.o dired.o filemode.o \
	cmds.o casetab.o casefiddle.o indent.o search.o regex.o undo.o \
	alloc.o data.o doc.o editfns.o callint.o \
	eval.o floatfns.o fns.o font.o print.o lread.o \
	syntax.o UNEXEC bytecode.o \
	process.o callproc.o \
	region-cache.o sound.o atimer.o \
	doprnt.o strftime.o intervals.o textprop.o composite.o md5.o \
	$(MSDOS_OBJ) $(NS_OBJ) $(CYGWIN_OBJ) $(FONT_DRIVERS)

/* Object files used on some machine or other.
   These go in the DOC file on all machines
   in case they are needed there.  */
SOME_MACHINE_OBJECTS = dosfns.o msdos.o \
  xterm.o xfns.o xmenu.o xselect.o xrdb.o xsmfns.o fringe.o image.o \
  fontset.o dbusbind.o \
  nsterm.o nsfns.o nsmenu.o nsselect.o nsimage.o nsfont.o \
  w32.o w32console.o w32fns.o w32heap.o w32inevt.o \
  w32menu.o w32proc.o w32reg.o w32select.o w32term.o w32xfns.o $(FONT_DRIVERS)


#ifdef TERMINFO
/* Used to be -ltermcap here.  If your machine needs that,
   define LIBS_TERMCAP in the m/MACHINE.h file.  */
#ifndef LIBS_TERMCAP
#define LIBS_TERMCAP -lcurses
#endif /* LIBS_TERMCAP */
termcapobj = terminfo.o
#else /* ! defined (TERMINFO) */
#ifndef LIBS_TERMCAP
#define LIBS_TERMCAP
termcapobj = termcap.o tparam.o
#else /* LIBS_TERMCAP */
termcapobj = tparam.o
#endif /* LIBS_TERMCAP */
#endif /* ! defined (TERMINFO) */


#ifndef SYSTEM_MALLOC

#ifndef DOUG_LEA_MALLOC
gmallocobj = gmalloc.o
#endif

#ifdef REL_ALLOC
rallocobj = ralloc.o
#endif

mallocobj = $(gmallocobj) $(rallocobj) vm-limit.o

#endif /* SYSTEM_MALLOC */


#ifdef USE_X_TOOLKIT
widgetobj= widget.o
#else /* not USE_X_TOOLKIT */
widgetobj=
#endif /* not USE_X_TOOLKIT */


/* define otherobj as list of object files that make-docfile
   should not be told about.  */
#ifdef CYGWIN
/* Cygwin differs because of its unexec(). */
otherobj= $(termcapobj) $(gmallocobj) $(rallocobj) lastfile.o vm-limit.o $(widgetobj) $(LIBOBJS)
#else
otherobj= $(termcapobj) lastfile.o $(mallocobj) $(widgetobj) $(LIBOBJS)
#endif

#ifdef HAVE_MOUSE
#define MOUSE_SUPPORT ${lispsource}mouse.elc \
  ${lispsource}select.elc ${lispsource}scroll-bar.elc
#else
#ifdef HAVE_GPM
#define MOUSE_SUPPORT ${lispsource}mouse.elc
#else
#define MOUSE_SUPPORT
#endif
#endif

#ifdef MSDOS
#define MSDOS_SUPPORT ${lispsource}ls-lisp.elc ${lispsource}disp-table.elc \
 ${lispsource}dos-fns.elc ${lispsource}dos-w32.elc ${lispsource}dos-vars.elc \
 ${lispsource}term/internal.elc ${lispsource}term/pc-win.elc

#else
#define MSDOS_SUPPORT
#endif

#ifdef HAVE_WINDOW_SYSTEM
#ifdef HAVE_X_WINDOWS
#define WINDOW_SUPPORT ${lispsource}fringe.elc ${lispsource}image.elc \
 ${lispsource}international/fontset.elc ${lispsource}dnd.elc \
 ${lispsource}tool-bar.elc ${lispsource}mwheel.elc ${lispsource}x-dnd.elc \
 ${lispsource}term/common-win.elc ${lispsource}term/x-win.elc \
 ${lispsource}font-setting.elc
#else
#define WINDOW_SUPPORT ${lispsource}fringe.elc ${lispsource}image.elc \
 ${lispsource}international/fontset.elc ${lispsource}dnd.elc \
 ${lispsource}tool-bar.elc ${lispsource}mwheel.elc
#endif
#else
#define WINDOW_SUPPORT
#endif

#ifdef WINDOWSNT
#define WINNT_SUPPORT ${lispsource}ls-lisp.elc ${lispsource}disp-table.elc \
  ${lispsource}dos-w32.elc ${lispsource}w32-vars.elc \
  ${lispsource}w32-fns.elc ${lispsource}term/common-win.elc \
  ${lispsource}term/w32-win.elc
#else
#define WINNT_SUPPORT
#endif

#ifdef HAVE_WINDOW_SYSTEM
#define TOOLTIP_SUPPORT ${lispsource}tooltip.elc
#else
#define TOOLTIP_SUPPORT
#endif

#ifdef HAVE_NS
#define NS_SUPPORT ${lispsource}emacs-lisp/easymenu.elc \
  ${lispsource}term/ns-win.elc
#else
#define NS_SUPPORT
#endif

/* This is the platform-specific list of Lisp files loaded into the
   dumped Emacs.  It is arranged like this because it is easier to generate
   it semi-mechanically from loadup.el this way.

   Note that this list should not include lisp files which might not
   be present, like site-load.el and site-init.el; this makefile
   expects them all to be either present or buildable.

   Files which are loaded unconditionally (i.e., on all platforms) should
   also be in shortlisp.  Files which are loaded conditionally (i.e., only
   on some platforms) should instead be in SOME_MACHINE_LISP.  */

/* Place loaddefs.el first, so it gets generated first, since it is on
   the critical path (relevant in parallel compilations).  */

lisp= \
	${lispsource}loaddefs.el \
	${lispsource}abbrev.elc \
	${lispsource}buff-menu.elc \
	${lispsource}button.elc \
	${lispsource}emacs-lisp/byte-run.elc \
	${lispsource}composite.elc \
	${lispsource}cus-face.elc \
	${lispsource}cus-start.elc \
	${lispsource}custom.elc \
	${lispsource}emacs-lisp/backquote.elc \
	${lispsource}emacs-lisp/lisp-mode.elc \
	${lispsource}emacs-lisp/lisp.elc \
	${lispsource}env.elc \
	${lispsource}faces.elc \
	${lispsource}files.elc \
	${lispsource}format.elc \
	${lispsource}facemenu.elc \
	MOUSE_SUPPORT \
	${lispsource}emacs-lisp/float-sup.elc \
	${lispsource}frame.elc \
	${lispsource}help.elc \
	${lispsource}indent.elc \
	${lispsource}isearch.elc \
	${lispsource}rfn-eshadow.elc \
	${lispsource}loadup.el \
	${lispsource}bindings.elc \
	${lispsource}emacs-lisp/map-ynp.elc \
	${lispsource}menu-bar.elc \
	${lispsource}international/mule.elc \
	${lispsource}international/mule-conf.elc \
	${lispsource}international/mule-cmds.elc \
	${lispsource}international/characters.elc \
	${lispsource}international/charprop.el \
	${lispsource}case-table.elc \
	${lispsource}language/chinese.elc \
	${lispsource}language/cyrillic.elc \
	${lispsource}language/indian.elc \
	${lispsource}language/sinhala.el \
	${lispsource}language/english.el \
	${lispsource}language/ethiopic.elc \
	${lispsource}language/european.elc \
	${lispsource}language/czech.el \
	${lispsource}language/slovak.el \
	${lispsource}language/romanian.el \
	${lispsource}language/greek.el \
	${lispsource}language/hebrew.el \
	${lispsource}language/japanese.el \
	${lispsource}language/korean.el \
	${lispsource}language/lao.el \
	${lispsource}language/cham.el \
	${lispsource}language/tai-viet.el \
	${lispsource}language/thai.el \
	${lispsource}language/tibetan.elc \
	${lispsource}language/vietnamese.elc \
	${lispsource}language/misc-lang.el \
	${lispsource}language/utf-8-lang.el \
	${lispsource}language/georgian.el \
	${lispsource}language/khmer.el \
	${lispsource}language/burmese.el \
	${lispsource}paths.el \
	${lispsource}register.elc \
	${lispsource}replace.elc \
	${lispsource}simple.elc \
	${lispsource}minibuffer.elc \
	${lispsource}startup.elc \
	${lispsource}subr.elc \
	${lispsource}term/tty-colors.elc \
	${lispsource}font-core.elc \
	${lispsource}emacs-lisp/syntax.elc \
	${lispsource}font-lock.elc \
	${lispsource}jit-lock.elc \
	${lispsource}textmodes/fill.elc \
	${lispsource}textmodes/page.elc \
	${lispsource}textmodes/paragraphs.elc \
	${lispsource}textmodes/text-mode.elc \
	${lispsource}emacs-lisp/timer.elc \
	${lispsource}jka-cmpr-hook.elc \
	${lispsource}vc-hooks.elc \
	${lispsource}ediff-hook.elc \
	${lispsource}epa-hook.elc \
	TOOLTIP_SUPPORT \
	MSDOS_SUPPORT \
	WINNT_SUPPORT \
	WINDOW_SUPPORT \
	NS_SUPPORT \
	${lispsource}widget.elc \
	${lispsource}window.elc \
	${lispsource}version.el

/* List of relative names for those files from $lisp that are loaded
   unconditionally (i.e. on all platforms).  Files from $lisp that
   are only loaded on some platforms should instead be placed in
   SOME_MACHINE_LISP.  The only reason this variable exists is to prevent
   the make-docfile command-line getting too long for some systems.  */
shortlisp= \
	../lisp/loaddefs.el \
	../lisp/abbrev.elc \
	../lisp/buff-menu.elc \
	../lisp/button.elc \
	../lisp/emacs-lisp/byte-run.elc \
	../lisp/composite.elc \
	../lisp/cus-face.elc \
	../lisp/cus-start.elc \
	../lisp/custom.elc \
	../lisp/emacs-lisp/backquote.elc \
	../lisp/emacs-lisp/lisp-mode.elc \
	../lisp/emacs-lisp/lisp.elc \
	../lisp/facemenu.elc \
	../lisp/faces.elc \
	../lisp/files.elc \
	../lisp/emacs-lisp/float-sup.elc \
	../lisp/format.elc \
	../lisp/frame.elc \
	../lisp/help.elc \
	../lisp/indent.elc \
	../lisp/isearch.elc \
	../lisp/rfn-eshadow.elc \
	../lisp/loadup.el \
	../lisp/bindings.elc \
	../lisp/emacs-lisp/map-ynp.elc \
	../lisp/env.elc \
	../lisp/international/mule.elc \
	../lisp/international/mule-conf.elc \
	../lisp/international/mule-cmds.elc \
	../lisp/international/characters.elc \
	../lisp/case-table.elc \
	../lisp/language/chinese.elc \
	../lisp/language/cyrillic.elc \
	../lisp/language/indian.elc \
	../lisp/language/sinhala.el \
	../lisp/language/english.el \
	../lisp/language/ethiopic.elc \
	../lisp/language/european.elc \
	../lisp/language/czech.el \
	../lisp/language/slovak.el \
	../lisp/language/romanian.el \
	../lisp/language/greek.el \
	../lisp/language/hebrew.el \
	../lisp/language/japanese.el \
	../lisp/language/korean.el \
	../lisp/language/lao.el \
	../lisp/language/cham.el \
	../lisp/language/tai-viet.el \
	../lisp/language/thai.el \
	../lisp/language/tibetan.elc \
	../lisp/language/vietnamese.elc \
	../lisp/language/misc-lang.el \
	../lisp/language/utf-8-lang.el \
	../lisp/language/georgian.el \
	../lisp/language/khmer.el \
	../lisp/language/burmese.el \
	../lisp/menu-bar.elc \
	../lisp/paths.el \
	../lisp/register.elc \
	../lisp/replace.elc \
	../lisp/simple.elc \
	../lisp/minibuffer.elc \
	../lisp/startup.elc \
	../lisp/subr.elc \
	../lisp/term/tty-colors.elc \
	../lisp/font-core.elc \
	../lisp/emacs-lisp/syntax.elc \
	../lisp/font-lock.elc \
	../lisp/jit-lock.elc \
	../lisp/textmodes/fill.elc \
	../lisp/textmodes/page.elc \
	../lisp/textmodes/paragraphs.elc \
	../lisp/textmodes/text-mode.elc \
	../lisp/emacs-lisp/timer.elc \
	../lisp/vc-hooks.elc \
	../lisp/jka-cmpr-hook.elc \
	../lisp/ediff-hook.elc \
	../lisp/epa-hook.elc \
	../lisp/widget.elc \
	../lisp/window.elc \
	../lisp/version.el

/* Like $shortlisp, but includes only those files from $lisp that are loaded
   conditionally (i.e., only on some platforms).  */
SOME_MACHINE_LISP = ../lisp/mouse.elc \
  ../lisp/select.elc ../lisp/scroll-bar.elc \
  ../lisp/ls-lisp.elc ../lisp/dos-fns.elc \
  ../lisp/w32-fns.elc ../lisp/dos-w32.elc \
  ../lisp/disp-table.elc ../lisp/dos-vars.elc \
  ../lisp/tooltip.elc ../lisp/image.elc \
  ../lisp/fringe.elc ../lisp/dnd.elc \
  ../lisp/mwheel.elc ../lisp/tool-bar.elc \
  ../lisp/x-dnd.elc ../lisp/font-setting.elc \
  ../lisp/international/ccl.elc \
  ../lisp/international/fontset.elc \
  ../lisp/mouse.elc \
  ../lisp/term/common-win.elc \
  ../lisp/term/x-win.elc \
  ../lisp/term/pc-win.elc ../lisp/term/internal.elc \
  ../lisp/term/ns-win.elc ../lisp/term/w32-win.elc \
  ../lisp/emacs-lisp/easymenu.elc

/* Construct full set of libraries to be linked.
   Note that SunOS needs -lm to come before -lc; otherwise, you get
   duplicated symbols.  If the standard libraries were compiled
   with GCC, we might need gnulib again after them.  */

LIBES = $(LOADLIBES) $(LIBS) $(LIBX) $(LIBSOUND) $(RSVG_LIBS) $(DBUS_LIBS) \
   LIBGPM LIBRESOLV LIBS_SYSTEM LIBS_MACHINE LIBS_TERMCAP \
   LIBS_DEBUG $(GETLOADAVG_LIBS) ${GCONF_LIBS} \
       \
   $(GNULIB_VAR) LIB_MATH LIB_STANDARD $(GNULIB_VAR)

#ifdef HAVE_SHM
RUN_TEMACS = `/bin/pwd`/temacs -nl
#else
RUN_TEMACS = `/bin/pwd`/temacs
#endif

all: emacs${EXEEXT} $(OTHER_FILES)

emacs${EXEEXT}: temacs${EXEEXT} ${etc}DOC ${lisp}
#ifdef CANNOT_DUMP
	rm -f emacs${EXEEXT}
	ln temacs${EXEEXT} emacs${EXEEXT}
	-EMACSLOADPATH=${lispsource} ./emacs -q -batch -f list-load-path-shadows
#else
	LC_ALL=C $(RUN_TEMACS) -batch -l loadup dump
	@: This new Emacs is as functional and more efficient then
	@: bootstrap-emacs, so let us replace it.
	-ln -f emacs${EXEEXT} bootstrap-emacs${EXEEXT}
	-./emacs -q -batch -f list-load-path-shadows
#endif /* ! defined (CANNOT_DUMP) */

/* We run make-docfile twice because the command line may get too long
   on some systems.  */
/* ${SOME_MACHINE_OBJECTS} comes before ${obj} because some files may
   or may not be included in ${obj}, but they are always included in
   ${SOME_MACHINE_OBJECTS}.  Since a file is processed when it is mentioned
   for the first time, this prevents any variation between configurations
   in the contents of the DOC file.
   Likewise for ${SOME_MACHINE_LISP}.  */
/* Most of this Makefile refers to Lisp files via ${lispsource}, so
   we also use ${lisp} rather than ${shortlisp} for the dependency since
   the Makefile uses string equality to decide when we talk about identical
   files.  Apparently we pass ${shortlisp} rather than ${lisp} to make-docfile
   only in order to reduce the command line length.  --Stef  */
${etc}DOC: ${libsrc}make-docfile${EXEEXT} ${obj} ${lisp} ${SOME_MACHINE_LISP}
	-rm -f ${etc}DOC
	${libsrc}make-docfile -d ${srcdir} ${SOME_MACHINE_OBJECTS} ${obj} > ${etc}DOC
	${libsrc}make-docfile -a ${etc}DOC -d ${srcdir} ${SOME_MACHINE_LISP} ${shortlisp}

${libsrc}make-docfile${EXEEXT}:
	cd ${libsrc}; ${MAKE} ${MFLAGS} make-docfile${EXEEXT}

buildobj.h: Makefile
	echo "#define BUILDOBJ \"${obj} ${otherobj} " "\"" > buildobj.h

temacs${EXEEXT}: $(LOCALCPP) $(STARTFILES) stamp-oldxmenu ${obj} ${otherobj} prefix-args${EXEEXT}
#ifdef NS_IMPL_GNUSTEP
	$(CC) -rdynamic YMF_PASS_LDFLAGS ( ${TEMACS_LDFLAGS} \
	-L -lgnustep-gui -lgnustep-base \
	-lobjc $(CONFIG_SYSTEM_LIBS) -lpthread ) -o temacs \
	${obj} ${otherobj} ${LIBES}
#else
	$(LD) YMF_PASS_LDFLAGS (${STARTFLAGS} ${TEMACS_LDFLAGS}) $(LDFLAGS) \
    -o temacs ${STARTFILES} ${obj} ${otherobj}  \
    ${LIBES}
#endif

/* We do not use ALL_LDFLAGS because LD_SWITCH_SYSTEM and LD_SWITCH_MACHINE
   often contain options that have to do with using Emacs''s crt0,
   which are only good with temacs.  */
prefix-args${EXEEXT}: prefix-args.o $(config_h)
	$(CC) $(LDFLAGS) prefix-args.o -o prefix-args

#if defined (HAVE_X_WINDOWS) && defined (HAVE_X11) && defined (HAVE_MENUS) && ! defined (USE_GTK)

/* We use stamp-xmenu with these two deps
   to both ensure that lwlib gets remade based on its dependencies
   in its own makefile,
   and remake temacs if lwlib gets changed by this.  */
stamp-oldxmenu: ${OLDXMENU} ../src/$(OLDXMENU)
	touch stamp-oldxmenu
/* Supply an ordering for parallel make.  */
../src/$(OLDXMENU): ${OLDXMENU}

/* Encode the values of these two macros in Make variables,
   so we can use $(...) to substitute their values within "...".  */
C_SWITCH_MACHINE_1 = C_SWITCH_MACHINE
C_SWITCH_SYSTEM_1 = C_SWITCH_SYSTEM
C_SWITCH_X_SITE_1 = C_SWITCH_X_SITE
C_SWITCH_X_MACHINE_1 = C_SWITCH_X_MACHINE
C_SWITCH_X_SYSTEM_1 = C_SWITCH_X_SYSTEM

#ifdef USE_X_TOOLKIT
$(OLDXMENU): really-lwlib

really-lwlib:
	cd ${lwlibdir}; ${MAKE} ${MFLAGS} \
    CC='${CC}' CFLAGS='${CFLAGS}' MAKE='${MAKE}' \
    "C_SWITCH_X_SITE=$(C_SWITCH_X_SITE_1)" \
    "C_SWITCH_X_MACHINE=$(C_SWITCH_X_MACHINE_1)" \
    "C_SWITCH_X_SYSTEM=$(C_SWITCH_X_SYSTEM_1)" \
    "C_SWITCH_MACHINE=$(C_SWITCH_MACHINE_1)" \
    "C_SWITCH_SYSTEM=$(C_SWITCH_SYSTEM_1)"
	@true  /* make -t should not create really-lwlib.  */
.PHONY: really-lwlib
#else /* not USE_X_TOOLKIT */
$(OLDXMENU): really-oldXMenu

really-oldXMenu:
	cd ${oldXMenudir}; ${MAKE} ${MFLAGS} \
    CC='${CC}' CFLAGS='${CFLAGS}' MAKE='${MAKE}' \
    "C_SWITCH_X_SITE=$(C_SWITCH_X_SITE_1)" \
    "C_SWITCH_X_MACHINE=$(C_SWITCH_X_MACHINE_1)" \
    "C_SWITCH_X_SYSTEM=$(C_SWITCH_X_SYSTEM_1)" \
    "C_SWITCH_MACHINE=$(C_SWITCH_MACHINE_1)" \
    "C_SWITCH_SYSTEM=$(C_SWITCH_SYSTEM_1)"
	@true  /* make -t should not create really-oldXMenu.  */
.PHONY: really-oldXMenu
#endif /* not USE_X_TOOLKIT */
#else /* not (HAVE_X_WINDOWS && HAVE_X11 && HAVE_MENUS && ! USE_GTK) */

/* We don''t really need this, but satisfy the dependency.  */
stamp-oldxmenu:
	touch stamp-oldxmenu
#endif /* not (HAVE_X_WINDOWS && HAVE_X11 && HAVE_MENUS && ! USE_GTK) */

../config.status:: epaths.in
	@echo "The file epaths.h needs to be set up from epaths.in."
	@echo "Please run the `configure' script again."
	exit 1

../config.status:: config.in
	@echo "The file config.h needs to be set up from config.in."
	@echo "Please run the `configure' script again."
	exit 1

ecrt0.o: ecrt0.c $(config_h)
#ifdef AUTO_DEPEND
	@-test -d deps || mkdir deps
#endif
	CRT0_COMPILE ${srcdir}/ecrt0.c
doc.o: buildobj.h

#ifndef AUTO_DEPEND

atimer.o: atimer.c atimer.h syssignal.h systime.h lisp.h blockinput.h \
 $(config_h)
buffer.o: buffer.c buffer.h region-cache.h commands.h window.h \
   $(INTERVALS_H) blockinput.h atimer.h systime.h character.h \
   indent.h keyboard.h coding.h keymap.h frame.h lisp.h $(config_h)
callint.o: callint.c window.h commands.h buffer.h keymap.h \
   keyboard.h dispextern.h systime.h coding.h composite.h lisp.h $(config_h)
callproc.o: callproc.c epaths.h buffer.h commands.h lisp.h $(config_h) \
	process.h systty.h syssignal.h character.h coding.h ccl.h msdos.h \
        composite.h w32.h blockinput.h atimer.h systime.h frame.h termhooks.h \
        buffer.h
casefiddle.o: casefiddle.c syntax.h commands.h buffer.h character.h \
   composite.h keymap.h lisp.h $(config_h)
casetab.o: casetab.c buffer.h character.h lisp.h $(config_h)
category.o: category.c category.h buffer.h charset.h keymap.h	\
	character.h lisp.h $(config_h)
ccl.o: ccl.c ccl.h charset.h character.h coding.h composite.h lisp.h $(config_h)
character.o: character.c character.h buffer.h charset.h composite.h disptab.h \
   lisp.h $(config_h)
charset.o: charset.c charset.h character.h buffer.h coding.h composite.h \
   disptab.h lisp.h $(config_h)
chartab.o: charset.h character.h ccl.h lisp.h $(config_h)
coding.o: coding.c coding.h ccl.h buffer.h character.h charset.h composite.h \
	window.h dispextern.h frame.h termhooks.h lisp.h $(config_h)
cm.o: cm.c frame.h cm.h termhooks.h termchar.h dispextern.h lisp.h $(config_h)
cmds.o: cmds.c syntax.h buffer.h character.h commands.h window.h lisp.h $(config_h) \
	msdos.h dispextern.h keyboard.h keymap.h systime.h coding.h frame.h \
        composite.h
pre-crt0.o: pre-crt0.c
dbusbind.o: dbusbind.c termhooks.h frame.h keyboard.h lisp.h $(config_h)
dired.o: dired.c commands.h buffer.h lisp.h $(config_h) character.h charset.h \
   coding.h regex.h systime.h blockinput.h atimer.h composite.h
dispnew.o: dispnew.c systime.h commands.h process.h frame.h coding.h \
   window.h buffer.h termchar.h termopts.h termhooks.h cm.h \
   disptab.h indent.h $(INTERVALS_H) \
   xterm.h blockinput.h atimer.h character.h msdos.h keyboard.h \
   syssignal.h lisp.h $(config_h)
doc.o: doc.c lisp.h $(config_h) epaths.h buffer.h keyboard.h keymap.h \
   character.h systime.h coding.h composite.h
doprnt.o: doprnt.c character.h lisp.h $(config_h)
dosfns.o: buffer.h termchar.h termhooks.h frame.h blockinput.h window.h \
   msdos.h dosfns.h dispextern.h charset.h coding.h atimer.h systime.h \
   lisp.h $(config_h)
editfns.o: editfns.c window.h buffer.h systime.h $(INTERVALS_H) character.h \
   coding.h frame.h blockinput.h atimer.h lisp.h $(config_h)
emacs.o: emacs.c commands.h systty.h syssignal.h blockinput.h process.h \
   termhooks.h buffer.h atimer.h systime.h $(INTERVALS_H) lisp.h $(config_h) \
   window.h dispextern.h keyboard.h keymap.h frame.h coding.h
fileio.o: fileio.c window.h buffer.h systime.h $(INTERVALS_H) character.h \
   coding.h msdos.h blockinput.h atimer.h lisp.h $(config_h) frame.h commands.h
filelock.o: filelock.c buffer.h character.h coding.h systime.h composite.h \
   lisp.h $(config_h)
filemode.o: filemode.c $(config_h)
frame.o: frame.c xterm.h window.h frame.h termhooks.h commands.h keyboard.h \
   blockinput.h atimer.h systime.h buffer.h character.h fontset.h font.h    \
   msdos.h dosfns.h dispextern.h w32term.h termchar.h coding.h composite.h \
   lisp.h $(config_h) termhooks.h ccl.h
fringe.o: fringe.c dispextern.h frame.h window.h buffer.h termhooks.h \
   blockinput.h atimer.h systime.h lisp.h $(config_h)
font.o: font.c dispextern.h frame.h window.h ccl.h character.h charset.h \
   font.h lisp.h $(config_h) buffer.h composite.h fontset.h xterm.h
ftfont.o: dispextern.h frame.h character.h charset.h composite.h font.h \
   lisp.h $(config_h) blockinput.h atimer.h systime.h coding.h fontset.h \
   ccl.h ftfont.h
fontset.o: fontset.c fontset.h ccl.h buffer.h character.h \
   charset.h frame.h keyboard.h termhooks.h font.h lisp.h $(config_h) \
   blockinput.h atimer.h systime.h coding.h $(INTERVALS_H) window.h xterm.h
getloadavg.o: getloadavg.c $(config_h)
gtkutil.o: gtkutil.c gtkutil.h xterm.h lisp.h frame.h lisp.h $(config_h) \
  blockinput.h window.h atimer.h systime.h termhooks.h keyboard.h \
  charset.h coding.h syssignal.h dispextern.h composite.h
image.o: image.c frame.h window.h dispextern.h blockinput.h atimer.h \
   systime.h xterm.h w32term.h w32gui.h font.h epaths.h character.h coding.h \
   nsterm.h nsgui.h lisp.h $(config_h) composite.h termhooks.h ccl.h
indent.o: indent.c frame.h window.h indent.h buffer.h lisp.h $(config_h) termchar.h \
   termopts.h disptab.h region-cache.h character.h category.h \
   keyboard.h systime.h coding.h $(INTERVALS_H)
insdel.o: insdel.c window.h buffer.h $(INTERVALS_H) blockinput.h character.h \
   dispextern.h atimer.h systime.h region-cache.h lisp.h $(config_h)
keyboard.o: keyboard.c termchar.h termhooks.h termopts.h buffer.h character.h \
   commands.h frame.h window.h macros.h disptab.h keyboard.h syssignal.h \
   systime.h syntax.h $(INTERVALS_H) blockinput.h atimer.h composite.h \
   xterm.h puresize.h msdos.h keymap.h w32term.h nsterm.h coding.h \
   lisp.h $(config_h)
keymap.o: keymap.c buffer.h commands.h keyboard.h termhooks.h blockinput.h \
   atimer.h systime.h puresize.h character.h charset.h $(INTERVALS_H) keymap.h window.h \
   coding.h frame.h lisp.h $(config_h)
lastfile.o: lastfile.c $(config_h)
macros.o: macros.c window.h buffer.h commands.h macros.h keyboard.h \
	dispextern.h lisp.h $(config_h) systime.h coding.h composite.h
gmalloc.o: gmalloc.c $(config_h)
ralloc.o: ralloc.c lisp.h $(config_h)
vm-limit.o: vm-limit.c mem-limits.h lisp.h $(config_h)
marker.o: marker.c buffer.h character.h lisp.h $(config_h)
md5.o: md5.c md5.h $(config_h)
minibuf.o: minibuf.c syntax.h frame.h window.h keyboard.h systime.h \
   buffer.h commands.h character.h msdos.h $(INTERVALS_H) keymap.h \
   termhooks.h lisp.h $(config_h) coding.h
mktime.o: mktime.c $(config_h)
msdos.o: msdos.c msdos.h dosfns.h systime.h termhooks.h dispextern.h frame.h \
   termopts.h termchar.h character.h coding.h ccl.h disptab.h window.h \
   keyboard.h $(INTERVALS_H) buffer.h commands.h blockinput.h atimer.h lisp.h $(config_h)
nsfns.o: nsfns.m charset.h nsterm.h nsgui.h frame.h window.h buffer.h \
  dispextern.h nsgui.h fontset.h $(INTERVALS_H) keyboard.h blockinput.h \
  atimer.h systime.h epaths.h termhooks.h coding.h systime.h lisp.h $(config_h)
nsfont.o: nsterm.h dispextern.h frame.h lisp.h lisp.h $(config_h)
nsimage.o: nsimage.m nsterm.h lisp.h $(config_h)
nsmenu.o: nsmenu.m termhooks.h frame.h window.h dispextern.h \
  nsgui.h keyboard.h blockinput.h atimer.h systime.h buffer.h \
  nsterm.h lisp.h $(config_h)
nsterm.o: nsterm.m blockinput.h atimer.h systime.h syssignal.h nsterm.h \
  nsgui.h frame.h charset.h ccl.h dispextern.h fontset.h termhooks.h \
  termopts.h termchar.h disptab.h buffer.h window.h keyboard.h \
  $(INTERVALS_H) process.h coding.h lisp.h $(config_h)
nsselect.o: nsselect.m blockinput.h nsterm.h nsgui.h frame.h lisp.h $(config_h)
process.o: process.c process.h buffer.h window.h termhooks.h termopts.h \
   commands.h syssignal.h systime.h systty.h syswait.h frame.h dispextern.h \
   blockinput.h atimer.h charset.h coding.h ccl.h msdos.h composite.h \
   keyboard.h lisp.h $(config_h) character.h xgselect.h sysselect.h
regex.o: regex.c syntax.h buffer.h lisp.h $(config_h) regex.h category.h character.h
region-cache.o: region-cache.c buffer.h region-cache.h lisp.h $(config_h)
scroll.o: scroll.c termchar.h dispextern.h frame.h msdos.h keyboard.h \
   termhooks.h lisp.h $(config_h) systime.h coding.h composite.h window.h
search.o: search.c regex.h commands.h buffer.h region-cache.h syntax.h \
   blockinput.h atimer.h systime.h category.h character.h charset.h \
   $(INTERVALS_H) \
   lisp.h $(config_h)
sound.o: sound.c dispextern.h syssignal.h lisp.h $(config_h) atimer.h systime.h
strftime.o: strftime.c $(config_h)
syntax.o: syntax.c syntax.h buffer.h commands.h category.h character.h \
   keymap.h regex.h $(INTERVALS_H) lisp.h $(config_h)
sysdep.o: sysdep.c syssignal.h systty.h systime.h syswait.h blockinput.h \
   process.h dispextern.h termhooks.h termchar.h termopts.h coding.h \
   frame.h atimer.h window.h msdos.h dosfns.h keyboard.h cm.h lisp.h $(config_h) \
   composite.h
term.o: term.c termchar.h termhooks.h termopts.h lisp.h $(config_h) cm.h frame.h \
   disptab.h keyboard.h character.h charset.h coding.h ccl.h xterm.h \
   msdos.h window.h keymap.h blockinput.h atimer.h systime.h systty.h \
   syssignal.h $(INTERVALS_H) buffer.h
termcap.o: termcap.c lisp.h $(config_h)
terminal.o: terminal.c frame.h termchar.h termhooks.h charset.h coding.h \
   keyboard.h lisp.h $(config_h) dispextern.h composite.h systime.h
terminfo.o: terminfo.c lisp.h $(config_h)
tparam.o: tparam.c lisp.h $(config_h)
undo.o: undo.c buffer.h commands.h window.h dispextern.h lisp.h $(config_h)
unexaix.o: unexaix.c lisp.h $(config_h)
unexalpha.o: unexalpha.c $(config_h)
unexcw.o: unexcw.c lisp.h $(config_h)
unexec.o: unexec.c lisp.h $(config_h)
unexelf.o: unexelf.c $(config_h)
unexhp9k800.o: unexhp9k800.c $(config_h)
unexmacosx.o: unexmacosx.c $(config_h)
unexsol.o: unexsol.c lisp.h $(config_h)
unexw32.o: unexw32.c $(config_h)
w16select.o: w16select.c dispextern.h frame.h blockinput.h atimer.h systime.h \
   msdos.h buffer.h charset.h coding.h composite.h lisp.h $(config_h)
widget.o: widget.c xterm.h frame.h dispextern.h widgetprv.h \
   $(srcdir)/../lwlib/lwlib.h lisp.h $(config_h)
window.o: window.c indent.h commands.h frame.h window.h buffer.h termchar.h \
   disptab.h keyboard.h msdos.h coding.h termhooks.h \
   keymap.h blockinput.h atimer.h systime.h $(INTERVALS_H) \
   xterm.h w32term.h nsterm.h lisp.h $(config_h)
xdisp.o: xdisp.c macros.h commands.h process.h indent.h buffer.h dispextern.h \
   coding.h termchar.h frame.h window.h disptab.h termhooks.h character.h     \
   charset.h lisp.h $(config_h) keyboard.h $(INTERVALS_H) region-cache.h xterm.h    \
   w32term.h nsterm.h msdos.h composite.h fontset.h ccl.h \
   blockinput.h atimer.h systime.h keymap.h font.h
xfaces.o: xfaces.c dispextern.h frame.h xterm.h buffer.h blockinput.h	\
   window.h character.h charset.h msdos.h dosfns.h composite.h atimer.h	\
   systime.h keyboard.h fontset.h w32term.h nsterm.h coding.h ccl.h \
   $(INTERVALS_H) termchar.h termhooks.h font.h lisp.h $(config_h)
xfns.o: xfns.c buffer.h frame.h window.h keyboard.h xterm.h dispextern.h \
   $(srcdir)/../lwlib/lwlib.h blockinput.h atimer.h systime.h epaths.h \
   character.h charset.h coding.h gtkutil.h lisp.h $(config_h) termhooks.h \
   fontset.h termchar.h font.h xsettings.h $(INTERVALS_H) ccl.h
xfont.o: dispextern.h xterm.h frame.h blockinput.h character.h charset.h \
   font.h lisp.h $(config_h) atimer.h systime.h fontset.h ccl.h
xftfont.o: dispextern.h xterm.h frame.h blockinput.h character.h charset.h \
   font.h lisp.h $(config_h) atimer.h systime.h fontset.h ccl.h ftfont.h
ftxfont.o: dispextern.h xterm.h frame.h blockinput.h character.h charset.h \
   font.h lisp.h $(config_h) atimer.h systime.h fontset.h ccl.h
menu.o: menu.c lisp.h keyboard.h keymap.h frame.h termhooks.h blockinput.h \
   dispextern.h $(srcdir)/../lwlib/lwlib.h xterm.h gtkutil.h menu.h \
   lisp.h $(config_h) systime.h coding.h composite.h window.h atimer.h
xmenu.o: xmenu.c xterm.h termhooks.h window.h dispextern.h frame.h buffer.h \
   charset.h keyboard.h $(srcdir)/../lwlib/lwlib.h blockinput.h atimer.h \
   systime.h gtkutil.h msdos.h coding.h menu.h lisp.h $(config_h) composite.h \
   keymap.h sysselect.h
xterm.o: xterm.c xterm.h termhooks.h termopts.h termchar.h window.h buffer.h \
   dispextern.h frame.h disptab.h blockinput.h atimer.h systime.h syssignal.h \
   keyboard.h emacs-icon.h character.h charset.h ccl.h fontset.h composite.h \
   coding.h process.h gtkutil.h font.h fontset.h lisp.h $(config_h) \
   xsettings.h intervals.h keymap.h xgselect.h sysselect.h
xselect.o: xselect.c process.h dispextern.h frame.h xterm.h blockinput.h \
  buffer.h atimer.h systime.h termhooks.h lisp.h $(config_h) keyboard.h \
  coding.h composite.h
xgselect.o: xgselect.h systime.h sysselect.h lisp.h $(config_h)
xrdb.o: xrdb.c lisp.h $(config_h) epaths.h
xsmfns.o: xsmfns.c lisp.h $(config_h) systime.h sysselect.h termhooks.h xterm.h \
  lisp.h termopts.h frame.h dispextern.h
xsettings.o: xterm.h xsettings.h lisp.h frame.h termhooks.h $(config_h) \
  dispextern.h keyboard.h systime.h coding.h composite.h blockinput.h atimer.h \
  termopts.h
/* The files of Lisp proper */

alloc.o: alloc.c process.h frame.h window.h buffer.h  puresize.h syssignal.h \
 keyboard.h blockinput.h atimer.h systime.h character.h lisp.h $(config_h) \
 $(INTERVALS_H) termhooks.h
bytecode.o: bytecode.c buffer.h syntax.h character.h window.h dispextern.h \
  frame.h xterm.h lisp.h $(config_h)
data.o: data.c buffer.h puresize.h character.h syssignal.h keyboard.h frame.h \
   termhooks.h systime.h coding.h composite.h dispextern.h font.h ccl.h \
   lisp.h $(config_h)
eval.o: eval.c commands.h keyboard.h blockinput.h atimer.h systime.h \
  dispextern.h lisp.h $(config_h) coding.h composite.h xterm.h
floatfns.o: floatfns.c syssignal.h lisp.h $(config_h)
fns.o: fns.c commands.h lisp.h $(config_h) frame.h buffer.h character.h keyboard.h \
 keymap.h window.h dispextern.h $(INTERVALS_H) coding.h md5.h \
 blockinput.h atimer.h systime.h xterm.h
print.o: print.c process.h frame.h window.h buffer.h keyboard.h character.h \
   lisp.h $(config_h) termchar.h $(INTERVALS_H) msdos.h termhooks.h \
   blockinput.h atimer.h systime.h font.h charset.h coding.h ccl.h
lread.o: lread.c commands.h keyboard.h buffer.h epaths.h character.h \
 charset.h lisp.h $(config_h) $(INTERVALS_H) termhooks.h coding.h msdos.h \
 systime.h frame.h blockinput.h atimer.h

/* Text properties support */
composite.o: composite.c buffer.h character.h coding.h font.h ccl.h \
	 frame.h termhooks.h $(INTERVALS_H) window.h lisp.h $(config_h)
intervals.o: intervals.c buffer.h $(INTERVALS_H) keyboard.h puresize.h \
	keymap.h lisp.h $(config_h) systime.h coding.h
textprop.o: textprop.c buffer.h window.h $(INTERVALS_H) \
	lisp.h $(config_h)

#endif /* ! AUTO_DEPEND */

/* System-specific programs to be made.
   OTHER_FILES
   select which of these should be compiled.  */

#ifdef HAVE_NS
/* In fact, every .o file depends directly or indirectly on dispextern.h
   and hence nsgui.h under NS.  But the ones that actually use stuff there
   are more limited.  */
dispnew.o font.o fontset.o frame.o fringe.o image.o keyboard.o menu.o \
   window.o xdisp.o xfaces.o: nsgui.h

${ns_appdir}: ${ns_appsrc}
	rm -fr ${ns_appdir}
	mkdir -p ${ns_appdir}
	( cd ${ns_appsrc} ; tar cfh - . ) | ( cd ${ns_appdir} ; tar xf - )
	( cd ${ns_appdir} ; for subdir in `find . -type d ! -name CVS -print` ; do \
		chmod a+rx $${subdir} ; \
		rm -rf $${subdir}/CVS ; \
		rm -f  $${subdir}/.cvsignore ; done ; )

${ns_appbindir}Emacs: emacs${EXEEXT}
	mkdir -p ${ns_appbindir}
	cp -f emacs${EXEEXT} ${ns_appbindir}Emacs

ns-app: ${ns_appdir} ${ns_appbindir}Emacs

#endif /* HAVE_NS */

mostlyclean:
	rm -f temacs${EXEEXT} prefix-args${EXEEXT} core *.core \#* *.o libXMenu11.a liblw.a
	rm -f ../etc/DOC
	rm -f bootstrap-emacs${EXEEXT} emacs-${version}${EXEEXT}
	rm -f buildobj.h
clean: mostlyclean
	rm -f emacs-*.*.*${EXEEXT} emacs${EXEEXT}
	-rm -rf deps
#ifdef HAVE_NS
	rm -fr ${ns_appdir}
#endif
/* bootstrap-clean is used to clean up just before a bootstrap.
   It should remove all files generated during a compilation/bootstrap,
   but not things like config.status or TAGS.  */
bootstrap-clean: clean
	rm -f epaths.h config.h Makefile.c config.stamp stamp-oldxmenu ../etc/DOC-*
	if test -f ./.gdbinit; then \
	  mv ./.gdbinit ./.gdbinit.save; \
	  if test -f "${srcdir}/.gdbinit"; then rm -f ./.gdbinit.save; \
	  else mv ./.gdbinit.save ./.gdbinit; fi; \
	fi
/**/# This is used in making a distribution.
/**/# Do not use it on development directories!
distclean: bootstrap-clean
	rm -f Makefile
maintainer-clean: distclean
	@echo "This command is intended for maintainers to use;"
	@echo "it deletes files that may require special tools to rebuild."
	rm -f TAGS
versionclean:
	-rm -f emacs${EXEEXT} emacs-*.*.*${EXEEXT} ../etc/DOC*
extraclean: distclean
	-rm -f *~ \#* m/?*~ s/?*~

/* Arrange to make a tags table TAGS-LISP for ../lisp,
   plus TAGS for the C files, which includes ../lisp/TAGS by reference.  */

ctagsfiles1 = [xyzXYZ]*.[hcm]
ctagsfiles2 = [a-wA-W]*.[hcm]

TAGS: $(srcdir)/$(ctagsfiles1) $(srcdir)/$(ctagsfiles2)
	../lib-src/etags --include=TAGS-LISP --include=${lwlibdir}/TAGS \
	  --regex='/[ 	]*DEFVAR_[A-Z_ 	(]+"\([^"]+\)"/' \
	  $(srcdir)/$(ctagsfiles1) $(srcdir)/$(ctagsfiles2)
frc:
TAGS-LISP: frc
	$(MAKE) -f ${lispdir}Makefile TAGS-LISP ETAGS=../lib-src/etags

$(lwlibdir)TAGS:
	(cd $(lwlibdir); $(MAKE) -f $(lwlibdir)Makefile tags ETAGS=../lib-src/etags)

tags: TAGS TAGS-LISP $(lwlibdir)TAGS
.PHONY: tags


/* Bootstrapping.  */
/* Bootstrapping right is difficult because of the circular dependencies.
   Furthermore, we have to deal with the fact that many compilation targets
   such as loaddefs.el or *.elc can typically be produced by any old
   Emacs executable, so we would like to avoid rebuilding them whenever
   we build a new Emacs executable.
   To solve the circularity, we use 2 different Emacs executables,
   "emacs" is the main target and "bootstrap-emacs" is the one used
   to build the *.elc and loaddefs.el files.
   To solve the freshness issue, we used to use a third file "witness-emacs"
   which was used to witness the fact that there is a bootstrap-emacs
   executable, and then have dependencies on witness-emacs rather than
   bootstrap-emacs, but that lead to problems in parallel builds (because
   witness-emacs needed to be free from dependencies (to avoid rebuilding
   it), so it was compiled in parallel, leading typically to having 2
   processes dumping bootstrap-emacs at the same time).
   So instead, we replace the witness-emacs dependencies by conditional
   bootstrap-dependencies (via ${BOOTSTRAPEMACS}).  Of course, since we do
   not want to rely on GNU Make features, we have to rely on an external
   script to do the conditional part of the dependency
   (i.e. see the ${SUBDIR} rule ../Makefile.in).  */

.SUFFIXES: .elc .el

/* These suffix rules do not allow additional dependencies, sadly, so
   instead of adding a $(BOOTSTRAPEMACS) dependency here, we add it
   separately below.
   With GNU Make, we would just say "%.el : %.elc $(BOOTSTRAPEMACS)"  */
.el.elc:
	@cd ../lisp; $(MAKE) $(MFLAGS) compile-onefile \
	                     THEFILE=$< EMACS=${bootstrap_exe}

/* Since the .el.elc rule cannot specify an extra dependency, we do it here.  */
${lisp} ${SOME_MACHINE_LISP}: $(BOOTSTRAPEMACS)

${lispsource}loaddefs.el: $(BOOTSTRAPEMACS)
	cd ../lisp; $(MAKE) $(MFLAGS) autoloads EMACS=${bootstrap_exe}

/* Dump an Emacs executable named bootstrap-emacs containing the
   files from loadup.el in source form.  */

bootstrap-emacs${EXEEXT}: temacs${EXEEXT}
	cd ../lisp; $(MAKE) $(MFLAGS) update-subdirs
#ifdef CANNOT_DUMP
	ln -f temacs${EXEEXT} bootstrap-emacs${EXEEXT}
#else
	$(RUN_TEMACS) --batch --load loadup bootstrap
	mv -f emacs${EXEEXT} bootstrap-emacs${EXEEXT}
#endif /* ! defined (CANNOT_DUMP) */
	@: Compile some files earlier to speed up further compilation.
	cd ../lisp; $(MAKE) $(MFLAGS) compile-first EMACS=${bootstrap_exe}

#ifdef AUTO_DEPEND
ALLOBJS=$(STARTFILES) ${obj} ${otherobj} prefix-args.o
-include $(ALLOBJS:%.o=deps/%.d)
#endif
