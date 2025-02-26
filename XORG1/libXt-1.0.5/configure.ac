dnl 
dnl  $Id$
dnl 
dnl  Copyright © 2003 Keith Packard, Noah Levitt
dnl 
dnl  Permission to use, copy, modify, distribute, and sell this software and its
dnl  documentation for any purpose is hereby granted without fee, provided that
dnl  the above copyright notice appear in all copies and that both that
dnl  copyright notice and this permission notice appear in supporting
dnl  documentation, and that the name of Keith Packard not be used in
dnl  advertising or publicity pertaining to distribution of the software without
dnl  specific, written prior permission.  Keith Packard makes no
dnl  representations about the suitability of this software for any purpose.  It
dnl  is provided "as is" without express or implied warranty.
dnl 
dnl  KEITH PACKARD DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE,
dnl  INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO
dnl  EVENT SHALL KEITH PACKARD BE LIABLE FOR ANY SPECIAL, INDIRECT OR
dnl  CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE,
dnl  DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
dnl  TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
dnl  PERFORMANCE OF THIS SOFTWARE.
dnl
dnl Process this file with autoconf to create configure.

AC_PREREQ([2.57])

dnl
dnl
AC_INIT(libXt, 1.0.5, [https://bugs.freedesktop.org/enter_bug.cgi?product=xorg], libXt)
AM_INIT_AUTOMAKE([dist-bzip2])
AM_MAINTAINER_MODE

AM_CONFIG_HEADER(config.h)

# Check for progs
AC_PROG_CC
AC_PROG_LIBTOOL

PKG_CHECK_MODULES(XT, sm x11 xproto kbproto)

# Needed for including Xalloca.h
AC_CHECK_HEADER([alloca.h], AC_DEFINE(INCLUDE_ALLOCA_H, 1, [Define to 1 if Xalloca.h should include <alloca.h>]))

# Map function checks to old Imake #defines
AC_CHECK_FUNC(poll, AC_DEFINE(USE_POLL,1,
        [Define to 1 if you have the `poll' function.]))
AC_CHECK_FUNC(snprintf, AC_DEFINE(USE_SNPRINTF,1,
        [Define to 1 if you have the `snprintf' function.]))

# Options
AC_ARG_ENABLE(xkb, AC_HELP_STRING([--disable-xkb], [Disable XKB support]),
	XKB="$enableval", XKB="yes")
if test "x$XKB" = "xyes" ; then
	AC_DEFINE(XKB, 1, [Define to 1 to use XKB for keysym resolution.])
fi

AC_ARG_ENABLE(install-makestrs,
	AC_HELP_STRING([--disable-install-makestrs], [Disable installing makestrs]),
	INSTALL_MAKESTRS="$enableval",
	INSTALL_MAKESTRS="yes")
AM_CONDITIONAL([INSTALL_MAKESTRS], [test "x$INSTALL_MAKESTRS" = xyes])

# Replaces XFileSearchPathDefault from Imake configs
XFILESEARCHPATHDEFAULT='$(sysconfdir)/X11/%L/%T/%N%C%S:$(sysconfdir)/X11/%l/%T/%N%C%S:$(sysconfdir)/X11/%T/%N%C%S:$(sysconfdir)/X11/%L/%T/%N%S:$(sysconfdir)/X11/%l/%T/%N%S:$(sysconfdir)/X11/%T/%N%S:$(datadir)/X11/%L/%T/%N%C%S:$(datadir)/X11/%l/%T/%N%C%S:$(datadir)/X11/%T/%N%C%S:$(datadir)/X11/%L/%T/%N%S:$(datadir)/X11/%l/%T/%N%S:$(datadir)/X11/%T/%N%S:$(libdir)/X11/%L/%T/%N%C%S:$(libdir)/X11/%l/%T/%N%C%S:$(libdir)/X11/%T/%N%C%S:$(libdir)/X11/%L/%T/%N%S:$(libdir)/X11/%l/%T/%N%S:$(libdir)/X11/%T/%N%S'

AC_ARG_WITH(xfile-search-path,
	AC_HELP_STRING([--with-xfile-search-path=<path>],
		       [Set path to search for app-defaults and other files]),
	[XFILESEARCHPATHDEFAULT="$withval"], [])
AC_SUBST([XFILESEARCHPATHDEFAULT])

AC_ARG_WITH(appdefaultdir,
	AC_HELP_STRING([--with-appdefaultdir=<path>],
		       [Set app-default directory (default: ${datadir}/X11/app-defaults)]),
	[appdefaultdir="$withval"],
	[appdefaultdir=${datadir}/X11/app-defaults])
AC_DEFINE_DIR([appdefaultdir], [appdefaultdir], [app default data directory])

# Replacement for Imake ToolkitStringsABIOptions, controls string tables
# generated by util/string.list in StringDefs.h & Shell.h
case $host_os in
	solaris*)
		case $host_cpu in
		    sparc*) STRINGSABIOPTIONS="-sparcabi -solarisabinames" ;;
	   		 *) STRINGSABIOPTIONS="-intelabi -solarisabinames" ;;
		esac
		;;
	sco* | svr4*)
		case $host_cpu in
		    i*86) STRINGSABIOPTIONS="-intelabi" ;;
		       *) STRINGSABIOPTIONS="" ;;
		esac
		;;
esac
AC_SUBST(STRINGSABIOPTIONS)

# Man page processing requirements
XORG_PROG_RAWCPP
XORG_MANPAGE_SECTIONS

# Check render configuration, strip extra digits from package version to
# find the required protocol version

XORG_CHECK_MALLOC_ZERO
XORG_RELEASE_VERSION

AC_OUTPUT([Makefile
	   src/Makefile
	   util/Makefile
	   include/Makefile
	   man/Makefile
           xt.pc])
