/* config.h.  Generated by configure.  */
/* config.h.in.  Generated from configure.ac by autoheader.  */

/* actions.c RCS ID */
#define ACTIONS_IDENT "actions.c 51650 2010-08-26 01:34:13Z lucas"

/* Authors */
#define AUTHORS "Michael Jennings (mej@eterm.org) "

/* Define for automatic character encoding. */
#define AUTO_ENCODING 1

/* Define if backing store is desired. */
#define BACKING_STORE 

/* The date of the build. */
#define BUILD_DATE "Tue Jan 17 07:18:57 EST 2012"

/* buttons.c RCS ID */
#define BUTTONS_IDENT "buttons.c 51650 2010-08-26 01:34:13Z lucas"

/* command.c RCS ID */
#define COMMAND_IDENT "command.c 59413 2011-05-15 21:19:59Z mej"

/* Config file/theme search path. */
#define CONFIG_SEARCH_PATH "~/.Eterm/themes:~/.Eterm:/usr/local/etc/Eterm/themes:/etc/Eterm/themes:/usr/local/share/Eterm/themes:/usr/local/etc/Eterm:/etc/Eterm:/usr/local/share/Eterm"

/* Define for dbx support. */
/* #undef DBX */

/* Debugging level to compile in. */
#define DEBUG 4

/* Default font index. */
#define DEF_FONT_IDX 2

/* draw.c RCS ID */
#define DRAW_IDENT "draw.c 51650 2010-08-26 01:34:13Z lucas"

/* Defined to enable title/icon name reporting escape sequences. */
/* #undef ENABLE_NAME_REPORTING_ESCAPES */

/* Define to enable code profiling macros. */
/* #undef ENABLE_PROFILE */

/* Define for Escreen support. */
#define ESCREEN 

/* Define to enable Escreen startup effects. */
/* #undef ESCREEN_FX */

/* events.c RCS ID */
#define EVENTS_IDENT "events.c 51650 2010-08-26 01:34:13Z lucas"

/* e.c RCS ID */
#define E_IDENT "e.c 51650 2010-08-26 01:34:13Z lucas"

/* Font 0 */
#define FONT0 "5x7"

/* Font 1 */
#define FONT1 "6x10"

/* Font 2 */
#define FONT2 "fixed"

/* Font 3 */
#define FONT3 "8x13"

/* Font 4 */
#define FONT4 "9x15"

/* font.c RCS ID */
#define FONT_IDENT "font.c 51650 2010-08-26 01:34:13Z lucas"

/* Define to force Backspace to send ^H. */
#define FORCE_BACKSPACE 

/* Define to force Backspace to send ^?. */
/* #undef FORCE_DELETE */

/* Define for gdb support. */
#define GDB "/usr/bin/gdb"

/* gdb command script. */
#define GDB_CMD_FILE "/usr/local/share/Eterm/gdb.scr"

/* Define for Greek keyboard support. */
/* #undef GREEK_SUPPORT */

/* grkelot.c RCS ID */
#define GRKELOT_IDENT "grkelot.c 51650 2010-08-26 01:34:13Z lucas"

/* Define to 1 if you have the `atexit' function. */
#define HAVE_ATEXIT 1

/* Define to 1 if you have the <bsd/signal.h> header file. */
/* #undef HAVE_BSD_SIGNAL_H */

/* Define for /dev/ptc support. */
/* #undef HAVE_DEV_PTC */

/* Define for /dev/ptmx support. */
#define HAVE_DEV_PTMX 

/* Define to 1 if you have the <dlfcn.h> header file. */
#define HAVE_DLFCN_H 1

/* Define to 1 if you have the <fcntl.h> header file. */
#define HAVE_FCNTL_H 1

/* Defined if user has a sense of humor. */
#define HAVE_HUMOR 1

/* Define to 1 if you have the <inttypes.h> header file. */
#define HAVE_INTTYPES_H 1

/* Define if LibAST is present. */
#define HAVE_LIBAST 1

/* Define if Imlib2 is present. */
#define HAVE_LIBIMLIB2 

/* Define to 1 if you have the `Kenny' library (-lKenny). */
/* #undef HAVE_LIBKENNY */

/* Define to 1 if you have the `m' library (-lm). */
#define HAVE_LIBM 1

/* Define to 1 if you have the `utempter' library (-lutempter). */
/* #undef HAVE_LIBUTEMPTER */

/* Define to 1 if you have the `X11' library (-lX11). */
#define HAVE_LIBX11 1

/* Define to 1 if you have the `Xext' library (-lXext). */
#define HAVE_LIBXEXT 1

/* Define to 1 if you have the <malloc.h> header file. */
/* #undef HAVE_MALLOC_H */

/* Define to 1 if you have the `memmem' function. */
/* #undef HAVE_MEMMEM */

/* Define to 1 if you have the `memmove' function. */
#define HAVE_MEMMOVE 1

/* Define to 1 if you have the <memory.h> header file. */
#define HAVE_MEMORY_H 1

/* Define for 32-bit MMX support. */
/* #undef HAVE_MMX */

/* Define to 1 if you have the `nl_langinfo' function. */
#define HAVE_NL_LANGINFO 1

/* Define for posix_openpt() support. */
#define HAVE_POSIX_OPENPT 

/* Define to 1 if you have the `putenv' function. */
#define HAVE_PUTENV 1

/* Define to 1 if you have the <regexp.h> header file. */
/* #undef HAVE_REGEXP_H */

/* Define to 1 if you have the <regex.h> header file. */
#define HAVE_REGEX_H 1

/* Define if the system supports saved UID and GID. */
/* #undef HAVE_SAVED_UIDS */

/* Define for SCO-style pty support. */
/* #undef HAVE_SCO_PTYS */

/* Define to 1 if you have the `seteuid' function. */
#define HAVE_SETEUID 1

/* Define to 1 if you have the `setresgid' function. */
/* #undef HAVE_SETRESGID */

/* Define to 1 if you have the `setresuid' function. */
/* #undef HAVE_SETRESUID */

/* Define to 1 if you have the `setutent' function. */
/* #undef HAVE_SETUTENT */

/* Define to 1 if you have the `snprintf' function. */
#define HAVE_SNPRINTF 1

/* Defined if libc snprintf is buggy. */
/* #undef HAVE_SNPRINTF_BUG */

/* Define for 64-bit SSE2 support. */
/* #undef HAVE_SSE2 */

/* Define to 1 if you have the <stdarg.h> header file. */
#define HAVE_STDARG_H 1

/* Define to 1 if you have the <stdint.h> header file. */
#define HAVE_STDINT_H 1

/* Define to 1 if you have the <stdlib.h> header file. */
#define HAVE_STDLIB_H 1

/* Define to 1 if you have the `strcasechr' function. */
/* #undef HAVE_STRCASECHR */

/* Define to 1 if you have the `strcasepbrk' function. */
/* #undef HAVE_STRCASEPBRK */

/* Define to 1 if you have the `strcasestr' function. */
#define HAVE_STRCASESTR 1

/* Define to 1 if you have the <strings.h> header file. */
#define HAVE_STRINGS_H 1

/* Define to 1 if you have the <string.h> header file. */
#define HAVE_STRING_H 1

/* Define to 1 if you have the `strrev' function. */
/* #undef HAVE_STRREV */

/* Define to 1 if you have the `strsep' function. */
#define HAVE_STRSEP 1

/* Define to 1 if you have the <sys/byteorder.h> header file. */
/* #undef HAVE_SYS_BYTEORDER_H */

/* Define to 1 if you have the <sys/ioctl.h> header file. */
#define HAVE_SYS_IOCTL_H 1

/* Define to 1 if you have the <sys/select.h> header file. */
#define HAVE_SYS_SELECT_H 1

/* Define to 1 if you have the <sys/sockio.h> header file. */
#define HAVE_SYS_SOCKIO_H 1

/* Define to 1 if you have the <sys/stat.h> header file. */
#define HAVE_SYS_STAT_H 1

/* Define to 1 if you have the <sys/time.h> header file. */
#define HAVE_SYS_TIME_H 1

/* Define to 1 if you have the <sys/types.h> header file. */
#define HAVE_SYS_TYPES_H 1

/* Define to 1 if you have <sys/wait.h> that is POSIX.1 compatible. */
#define HAVE_SYS_WAIT_H 1

/* Define to 1 if you have the <termios.h> header file. */
#define HAVE_TERMIOS_H 1

/* Define to 1 if you have the <unistd.h> header file. */
#define HAVE_UNISTD_H 1

/* Define to 1 if you have the `unsetenv' function. */
#define HAVE_UNSETENV 1

/* Define to 1 if you have the `usleep' function. */
#define HAVE_USLEEP 1

/* Define to 1 if you have the <utmpx.h> header file. */
#define HAVE_UTMPX_H 1

/* Define if we have X11/extensions/XRes.h */
#define HAVE_X11_EXTENSIONS_XRES_H 

/* Define to 1 if you have the <X11/Sunkeysym.h> header file. */
#define HAVE_X11_SUNKEYSYM_H 1

/* Define to 1 if you have the <X11/Xlib.h> header file. */
#define HAVE_X11_XLIB_H 1

/* Define to 1 if you have the <X11/Xlocale.h> header file. */
#define HAVE_X11_XLOCALE_H 1

/* Define to 1 if you have the <X11/X.h> header file. */
#define HAVE_X11_X_H 1

/* Define if we have the XResource extension. */
#define HAVE_XRES_EXT 

/* Define if X shaped window extension is available. */
#define HAVE_X_SHAPE_EXT 

/* Define to 1 if you have the `_exit' function. */
#define HAVE__EXIT 1

/* Define for Pablo I/O tracing support. */
/* #undef IOTRACE */

/* Define for specific Delete escape sequence. */
/* #undef KS_DELETE */

/* Emulate Linux console for End key. */
/* #undef KS_END */

/* Emulate Linux console for Home key. */
/* #undef KS_HOME */

/* Defined as 1 if LibAST has Imlib2 support. */
/* #undef LIBAST_IMLIB2_SUPPORT */

/* Defined as 1 if LibAST has MMX support. */
/* #undef LIBAST_MMX_SUPPORT */

/* Defined as 1 if LibAST has X11 support. */
#define LIBAST_X11_SUPPORT 1

/* main.c RCS ID */
#define MAIN_IDENT "main.c 38480 2009-01-06 09:08:48Z mej"

/* menus.c RCS ID */
#define MENUS_IDENT "menus.c 51650 2010-08-26 01:34:13Z lucas"

/* Multibyte Font 0 */
#define MFONT0 "none"

/* Multibyte Font 1 */
#define MFONT1 "none"

/* Multibyte Font 2 */
#define MFONT2 "none"

/* Multibyte Font 3 */
#define MFONT3 "none"

/* Multibyte Font 4 */
#define MFONT4 "none"

/* misc.c RCS ID */
#define MISC_IDENT "misc.c 51650 2010-08-26 01:34:13Z lucas"

/* Define for built-in mousewheel support. */
#define MOUSEWHEEL 

/* Multi-byte character encoding style. */
#define MULTICHAR_ENCODING "none"

/* Define for multi-byte character set support. */
/* #undef MULTI_CHARSET */

/* Define if uid hack needed to open X display. */
/* #undef NEED_LINUX_HACK */

/* Define to use the X server value for Delete. */
/* #undef NO_DELETE_KEY */

/* Define to disable stack trace on abnormal termination. */
/* #undef NO_STACK_TRACE */

/* Define for support of the screen text windowing system. */
#define NS_HAVE_SCREEN 1

/* Define for support of the twin text windowing system. */
/* #undef NS_HAVE_TWIN */

/* options.c RCS ID */
#define OPTIONS_IDENT "options.c 51650 2010-08-26 01:34:13Z lucas"

/* Name of package */
#define PACKAGE "Eterm"

/* Define to the address where bug reports for this package should be sent. */
#define PACKAGE_BUGREPORT ""

/* Define to the full name of this package. */
#define PACKAGE_NAME ""

/* Define to the full name and version of this package. */
#define PACKAGE_STRING ""

/* Define to the one symbol short name of this package. */
#define PACKAGE_TARNAME ""

/* Define to the version of this package. */
#define PACKAGE_VERSION ""

/* pixmap.c RCS ID */
#define PIXMAP_IDENT "pixmap.c 59414 2011-05-15 21:24:19Z mej"

/* Define for pseudo-transparency support. */
#define PIXMAP_OFFSET 

/* Define if pixmap support is desired. */
#define PIXMAP_SUPPORT 

/* Package data directory. */
#define PKGDATADIR "/usr/local/share/Eterm"

/* Define for pstack support. */
/* #undef PSTACK */

/* Group name for pty ownership. */
#define PTY_GRP_NAME "wheel"

/* Define as the return type of signal handlers (`int' or `void'). */
#define RETSIGTYPE void

/* screen.c RCS ID */
#define SCREEN_IDENT "screen.c 51650 2010-08-26 01:34:13Z lucas"

/* scrollbar.c RCS ID */
#define SCROLLBAR_IDENT "scrollbar.c 51650 2010-08-26 01:34:13Z lucas"

/* startup.c RCS ID */
#define STARTUP_IDENT "startup.c 51650 2010-08-26 01:34:13Z lucas"

/* Define to 1 if you have the ANSI C header files. */
#define STDC_HEADERS 1

/* Define if WM cannot handle icon pixmaps. */
/* #undef STRICT_ICCCM */

/* system.c RCS ID */
#define SYSTEM_IDENT "system.c 38480 2009-01-06 09:08:48Z mej"

/* term.c RCS ID */
#define TERM_IDENT "term.c 51650 2010-08-26 01:34:13Z lucas"

/* timer.c RCS ID */
#define TIMER_IDENT "timer.c 38480 2009-01-06 09:08:48Z mej"

/* Define to 1 if you can safely include both <sys/time.h> and <time.h>. */
#define TIME_WITH_SYS_TIME 1

/* Define if we have X11R6 XIM. */
#define USE_X11R6_XIM 

/* Define for X Input Method support. */
#define USE_XIM 

/* utmp.c RCS ID */
#define UTMP_IDENT "utmp.c 51650 2010-08-26 01:34:13Z lucas"

/* Define for utmp logging support. */
/* #undef UTMP_SUPPORT */

/* Version number of package */
#define VERSION "0.9.6"

/* windows.c RCS ID */
#define WINDOWS_IDENT "windows.c 51650 2010-08-26 01:34:13Z lucas"

/* Define if using the dmalloc debugging malloc package */
/* #undef WITH_DMALLOC */

/* Define to 1 if your processor stores words with the most significant byte
   first (like Motorola and SPARC, unlike Intel and VAX). */
/* #undef WORDS_BIGENDIAN */

/* Define to 1 if the X Window System is missing or not being used. */
/* #undef X_DISPLAY_MISSING */

/* Define to 1 if on AIX 3.
   System headers sometimes define this.
   We just want to avoid a redefinition error message.  */
#ifndef _ALL_SOURCE
/* # undef _ALL_SOURCE */
#endif

/* Define to 1 if on MINIX. */
/* #undef _MINIX */

/* Define to 2 if the system does not provide POSIX.1 features except with
   this defined. */
/* #undef _POSIX_1_SOURCE */

/* Define to 1 if you need to in order for `stat' and other things to work. */
/* #undef _POSIX_SOURCE */

/* Define to empty if `const' does not conform to ANSI C. */
/* #undef const */

/* Define to `int' if <sys/types.h> doesn't define. */
/* #undef gid_t */

/* Define to `__inline__' or `__inline' if that's what the C compiler
   calls it, or to nothing if 'inline' is not supported under any name.  */
#ifndef __cplusplus
/* #undef inline */
#endif

/* Define to `int' if <sys/types.h> does not define. */
/* #undef mode_t */

/* Define to `long' if <sys/types.h> does not define. */
/* #undef off_t */

/* Define to `int' if <sys/types.h> does not define. */
/* #undef pid_t */

/* Define to `int' if <sys/types.h> doesn't define. */
/* #undef uid_t */
