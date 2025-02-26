/* src/config.h.  Generated from config.in by configure.  */
/* src/config.in.  Generated from configure.in by autoheader.  */

/* GNU Emacs site configuration template file.
   Copyright (C) 1988, 1993, 1994, 1999, 2000, 2001, 2002, 2004, 2005,
     2006, 2007, 2008, 2009, 2010  Free Software Foundation, Inc.

This file is part of GNU Emacs.

GNU Emacs is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

GNU Emacs is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GNU Emacs.  If not, see <http://www.gnu.org/licenses/>.  */


/* No code in Emacs #includes config.h twice, but some bits of code
   intended to work with other packages as well (like gmalloc.c)
   think they can include it as many times as they like.  */
#ifndef EMACS_CONFIG_H
#define EMACS_CONFIG_H


/* Generate dependencies with gcc. */
#define AUTO_DEPEND 1

/* Define to 1 if the mktime function is broken. */
/* #undef BROKEN_MKTIME */

/* Define to one of `_getb67', `GETB67', `getb67' for Cray-2 and Cray-YMP
   systems. This function is required for `alloca.c' support on those systems.
   */
/* #undef CRAY_STACKSEG_END */

/* Define to 1 if using `alloca.c'. */
/* #undef C_ALLOCA */

/* Define to 1 if using `getloadavg.c'. */
/* #undef C_GETLOADAVG */

/* Define C_SWITCH_X_SITE to contain any special flags your compiler may need
   to deal with X Windows. For instance, if you've defined HAVE_X_WINDOWS
   above and your X include files aren't in a place that your compiler can
   find on its own, you might want to add "-I/..." or something similar. */
#define C_SWITCH_X_SITE 

/* Define to 1 for DGUX with <sys/dg_sys_info.h>. */
/* #undef DGUX */

/* Define to 1 if you are using the GNU C Library. */
/* #undef DOUG_LEA_MALLOC */

/* Define to the canonical Emacs configuration name. */
#define EMACS_CONFIGURATION "arm-apple-darwin10.4.0"

/* Define to the options passed to configure. */
#define EMACS_CONFIG_OPTIONS " '--without-dbus' 'CC=/usr/bin/gcc'"

/* Enable expensive run-time checking of data types? */
/* #undef ENABLE_CHECKING */

/* Define this to check for errors in cons list. */
/* #undef GC_CHECK_CONS_LIST */

/* Define this temporarily to hunt a bug. If defined, the size of strings is
   redundantly recorded in sdata structures so that it can be compared to the
   sizes recorded in Lisp strings. */
/* #undef GC_CHECK_STRING_BYTES */

/* Define this to check the string free list. */
/* #undef GC_CHECK_STRING_FREE_LIST */

/* Define this to check for short string overrun. */
/* #undef GC_CHECK_STRING_OVERRUN */

/* Define to 1 if the `getloadavg' function needs to be run setuid or setgid.
   */
/* #undef GETLOADAVG_PRIVILEGED */

/* Define to 1 if the `getpgrp' function requires zero arguments. */
#define GETPGRP_VOID 1

/* Define to 1 if gettimeofday accepts only one argument. */
/* #undef GETTIMEOFDAY_ONE_ARGUMENT */

/* Define to 1 if you want to use the GNU memory allocator. */
/* #undef GNU_MALLOC */

/* Define to 1 if the file /usr/lpp/X11/bin/smt.exp exists. */
/* #undef HAVE_AIX_SMT_EXP */

/* Define to 1 if you have the `alarm' function. */
#define HAVE_ALARM 1

/* Define to 1 if you have `alloca', as a function or macro. */
#define HAVE_ALLOCA 1

/* Define to 1 if you have <alloca.h> and it should be used (not on Ultrix).
   */
#define HAVE_ALLOCA_H 1

/* Define to 1 if ALSA is available. */
/* #undef HAVE_ALSA */

/* Define to 1 if you have the `bcmp' function. */
#define HAVE_BCMP 1

/* Define to 1 if you have the `bcopy' function. */
#define HAVE_BCOPY 1

/* Define to 1 if you have the `bzero' function. */
#define HAVE_BZERO 1

/* Define to 1 if you have the `cbrt' function. */
#define HAVE_CBRT 1

/* Define to 1 if you have the `cfmakeraw' function. */
#define HAVE_CFMAKERAW 1

/* Define to 1 if you have the `cfsetspeed' function. */
#define HAVE_CFSETSPEED 1

/* Define to 1 if you have the `closedir' function. */
#define HAVE_CLOSEDIR 1

/* Define to 1 if you have the <coff.h> header file. */
/* #undef HAVE_COFF_H */

/* Define to 1 if you have the <com_err.h> header file. */
/* #undef HAVE_COM_ERR_H */

/* Define to 1 if you have /usr/lib/crti.o. */
/* #undef HAVE_CRTIN */

/* Define to 1 if using D-Bus. */
/* #undef HAVE_DBUS */

/* Define to 1 if you have the `dbus_watch_get_unix_fd' function. */
/* #undef HAVE_DBUS_WATCH_GET_UNIX_FD */

/* Define to 1 if you have the declaration of `sys_siglist', and to 0 if you
   don't. */
#define HAVE_DECL_SYS_SIGLIST 0

/* Define to 1 if you have the declaration of `tzname', and to 0 if you don't.
   */
/* #undef HAVE_DECL_TZNAME */

/* Define to 1 if you have the declaration of `__sys_siglist', and to 0 if you
   don't. */
#define HAVE_DECL___SYS_SIGLIST 0

/* Define to 1 if you have the <des.h> header file. */
/* #undef HAVE_DES_H */

/* Define to 1 if dynamic ptys are supported. */
/* #undef HAVE_DEV_PTMX */

/* Define to 1 if you have the `difftime' function. */
#define HAVE_DIFFTIME 1

/* Define to 1 if you have the `dup2' function. */
#define HAVE_DUP2 1

/* Define to 1 if you have the `euidaccess' function. */
/* #undef HAVE_EUIDACCESS */

/* Define to 1 if you have the <fcntl.h> header file. */
#define HAVE_FCNTL_H 1

/* Define to 1 if you have the `fmod' function. */
#define HAVE_FMOD 1

/* Define to 1 if you have the `fork' function. */
#define HAVE_FORK 1

/* Define to 1 if you have the `fpathconf' function. */
#define HAVE_FPATHCONF 1

/* Define to 1 if using the freetype and fontconfig libraries. */
/* #undef HAVE_FREETYPE */

/* Define to 1 if you have the `frexp' function. */
#define HAVE_FREXP 1

/* Define to 1 if fseeko (and presumably ftello) exists and is declared. */
#define HAVE_FSEEKO 1

/* Define to 1 if you have the `fsync' function. */
#define HAVE_FSYNC 1

/* Define to 1 if you have the `ftime' function. */
#define HAVE_FTIME 1

/* Define to 1 if you have the `gai_strerror' function. */
#define HAVE_GAI_STRERROR 1

/* Define to 1 if using GConf. */
/* #undef HAVE_GCONF */

/* Define to 1 if you have the `gdk_display_open' function. */
/* #undef HAVE_GDK_DISPLAY_OPEN */

/* Define to 1 if you have the `getaddrinfo' function. */
#define HAVE_GETADDRINFO 1

/* Define to 1 if you have the `getcwd' function. */
#define HAVE_GETCWD 1

/* Define to 1 if you have the `getdelim' function. */
/* #undef HAVE_GETDELIM */

/* Define to 1 if you have the `getdomainname' function. */
#define HAVE_GETDOMAINNAME 1

/* Define to 1 if you have the `gethostname' function. */
#define HAVE_GETHOSTNAME 1

/* Define to 1 if you have the `getline' function. */
/* #undef HAVE_GETLINE */

/* Define to 1 if you have the `getloadavg' function. */
#define HAVE_GETLOADAVG 1

/* Define to 1 if you have the <getopt.h> header file. */
#define HAVE_GETOPT_H 1

/* Define to 1 if you have the `getopt_long_only' function. */
#define HAVE_GETOPT_LONG_ONLY 1

/* Define to 1 if you have the `getpagesize' function. */
#define HAVE_GETPAGESIZE 1

/* Define to 1 if you have the `getpeername' function. */
#define HAVE_GETPEERNAME 1

/* Define to 1 if you have the `getpt' function. */
/* #undef HAVE_GETPT */

/* Define to 1 if you have the `getrlimit' function. */
#define HAVE_GETRLIMIT 1

/* Define to 1 if you have the `getrusage' function. */
#define HAVE_GETRUSAGE 1

/* Define to 1 if you have the `getsockname' function. */
#define HAVE_GETSOCKNAME 1

/* Define to 1 if you have the `getsockopt' function. */
#define HAVE_GETSOCKOPT 1

/* Define to 1 if you have the `gettimeofday' function. */
#define HAVE_GETTIMEOFDAY 1

/* Define to 1 if you have the `getwd' function. */
#define HAVE_GETWD 1

/* Define to 1 if you have the `get_current_dir_name' function. */
/* #undef HAVE_GET_CURRENT_DIR_NAME */

/* Define to 1 if you have a gif library (default -lgif; otherwise specify
   with LIBGIF). */
#define HAVE_GIF 1

/* Define to 1 if you have the gpm library (-lgpm). */
/* #undef HAVE_GPM */

/* Define to 1 if you have the `grantpt' function. */
#define HAVE_GRANTPT 1

/* Define to 1 if you have GTK and pthread (-lpthread). */
/* #undef HAVE_GTK_AND_PTHREAD */

/* Define to 1 if GTK has both file selection and chooser dialog. */
/* #undef HAVE_GTK_FILE_BOTH */

/* Define to 1 if you have the `gtk_file_chooser_dialog_new' function. */
/* #undef HAVE_GTK_FILE_CHOOSER_DIALOG_NEW */

/* Define to 1 if you have the `gtk_file_selection_new' function. */
/* #undef HAVE_GTK_FILE_SELECTION_NEW */

/* Define to 1 if you have the `gtk_main' function. */
/* #undef HAVE_GTK_MAIN */

/* Define to 1 if GTK can handle more than one display. */
/* #undef HAVE_GTK_MULTIDISPLAY */

/* Define to 1 if netdb.h declares h_errno. */
#define HAVE_H_ERRNO 1

/* Define to 1 if you have the `index' function. */
#define HAVE_INDEX 1

/* Define to 1 if you have inet sockets. */
#define HAVE_INET_SOCKETS 1

/* Define to 1 if you have the <inttypes.h> header file. */
#define HAVE_INTTYPES_H 1

/* Define to 1 if you have the jpeg library (-ljpeg). */
#define HAVE_JPEG 1

/* Define to 1 if you have the <kerberosIV/des.h> header file. */
/* #undef HAVE_KERBEROSIV_DES_H */

/* Define to 1 if you have the <kerberosIV/krb.h> header file. */
/* #undef HAVE_KERBEROSIV_KRB_H */

/* Define to 1 if you have the <kerberos/des.h> header file. */
/* #undef HAVE_KERBEROS_DES_H */

/* Define to 1 if you have the <kerberos/krb.h> header file. */
/* #undef HAVE_KERBEROS_KRB_H */

/* Define to 1 if `e_text' is member of `krb5_error'. */
/* #undef HAVE_KRB5_ERROR_E_TEXT */

/* Define to 1 if `text' is member of `krb5_error'. */
/* #undef HAVE_KRB5_ERROR_TEXT */

/* Define to 1 if you have the <krb5.h> header file. */
/* #undef HAVE_KRB5_H */

/* Define to 1 if you have the <krb.h> header file. */
/* #undef HAVE_KRB_H */

/* Define if you have <langinfo.h> and nl_langinfo(CODESET). */
#define HAVE_LANGINFO_CODESET 1

/* Define to 1 if the directory /usr/lib64 exists. */
/* #undef HAVE_LIB64_DIR */

/* Define to 1 if you have the `com_err' library (-lcom_err). */
/* #undef HAVE_LIBCOM_ERR */

/* Define to 1 if you have the `crypto' library (-lcrypto). */
/* #undef HAVE_LIBCRYPTO */

/* Define to 1 if you have the `des' library (-ldes). */
/* #undef HAVE_LIBDES */

/* Define to 1 if you have the `des425' library (-ldes425). */
/* #undef HAVE_LIBDES425 */

/* Define to 1 if you have the `dgc' library (-ldgc). */
/* #undef HAVE_LIBDGC */

/* Define to 1 if you have the `dnet' library (-ldnet). */
/* #undef HAVE_LIBDNET */

/* Define to 1 if you have the hesiod library (-lhesiod). */
/* #undef HAVE_LIBHESIOD */

/* Define to 1 if you have the `intl' library (-lintl). */
#define HAVE_LIBINTL 1

/* Define to 1 if you have the `k5crypto' library (-lk5crypto). */
/* #undef HAVE_LIBK5CRYPTO */

/* Define to 1 if you have the `krb' library (-lkrb). */
/* #undef HAVE_LIBKRB */

/* Define to 1 if you have the `krb4' library (-lkrb4). */
/* #undef HAVE_LIBKRB4 */

/* Define to 1 if you have the `krb5' library (-lkrb5). */
/* #undef HAVE_LIBKRB5 */

/* Define to 1 if you have the `kstat' library (-lkstat). */
/* #undef HAVE_LIBKSTAT */

/* Define to 1 if you have the `lockfile' library (-llockfile). */
/* #undef HAVE_LIBLOCKFILE */

/* Define to 1 if you have the `m' library (-lm). */
#define HAVE_LIBM 1

/* Define to 1 if you have the `mail' library (-lmail). */
/* #undef HAVE_LIBMAIL */

/* Define to 1 if you have the `ncurses' library (-lncurses). */
#define HAVE_LIBNCURSES 1

/* Define to 1 if using libotf. */
/* #undef HAVE_LIBOTF */

/* Define to 1 if you have the <libpng/png.h> header file. */
/* #undef HAVE_LIBPNG_PNG_H */

/* Define to 1 if you have the `pthreads' library (-lpthreads). */
/* #undef HAVE_LIBPTHREADS */

/* Define to 1 if you have the resolv library (-lresolv). */
#define HAVE_LIBRESOLV 1

/* Define to 1 if you have the `Xext' library (-lXext). */
#define HAVE_LIBXEXT 1

/* Define to 1 if you have the `Xmu' library (-lXmu). */
#define HAVE_LIBXMU 1

/* Define to 1 if you have the Xp library (-lXp). */
/* #undef HAVE_LIBXP */

/* Define to 1 if you have the <limits.h> header file. */
#define HAVE_LIMITS_H 1

/* Define to 1 if you have the <linux/version.h> header file. */
/* #undef HAVE_LINUX_VERSION_H */

/* Define to 1 if you have the <locale.h> header file. */
#define HAVE_LOCALE_H 1

/* Define to 1 if you have the `logb' function. */
#define HAVE_LOGB 1

/* Define to 1 if you support file names longer than 14 characters. */
#define HAVE_LONG_FILE_NAMES 1

/* Define to 1 if you have the `lrand48' function. */
#define HAVE_LRAND48 1

/* Define to 1 if using libm17n-flt. */
/* #undef HAVE_M17N_FLT */

/* Define to 1 if you have the <machine/soundcard.h> header file. */
/* #undef HAVE_MACHINE_SOUNDCARD_H */

/* Define to 1 if you have the <mach/mach.h> header file. */
/* #undef HAVE_MACH_MACH_H */

/* Define to 1 if you have the <maillock.h> header file. */
/* #undef HAVE_MAILLOCK_H */

/* Define to 1 if you have the <malloc/malloc.h> header file. */
#define HAVE_MALLOC_MALLOC_H 1

/* Define to 1 if you have the `mblen' function. */
#define HAVE_MBLEN 1

/* Define to 1 if you have the `mbrlen' function. */
#define HAVE_MBRLEN 1

/* Define to 1 if you have the `mbsinit' function. */
#define HAVE_MBSINIT 1

/* Define to 1 if <wchar.h> declares mbstate_t. */
#define HAVE_MBSTATE_T 1

/* Define to 1 if you have the `memcmp' function. */
#define HAVE_MEMCMP 1

/* Define to 1 if you have the `memcpy' function. */
#define HAVE_MEMCPY 1

/* Define to 1 if you have the `memmove' function. */
#define HAVE_MEMMOVE 1

/* Define to 1 if you have the <memory.h> header file. */
#define HAVE_MEMORY_H 1

/* Define to 1 if you have the `mempcpy' function. */
/* #undef HAVE_MEMPCPY */

/* Define to 1 if you have the `memset' function. */
#define HAVE_MEMSET 1

/* Define to 1 if you have mouse menus. (This is automatic if you use X, but
   the option to specify it remains.) It is also defined with other window
   systems that support xmenu.c. */
#define HAVE_MENUS 1

/* Define to 1 if you have the `mkdir' function. */
#define HAVE_MKDIR 1

/* Define to 1 if you have the `mkstemp' function. */
#define HAVE_MKSTEMP 1

/* Define to 1 if you have the `mktime' function. */
#define HAVE_MKTIME 1

/* Define to 1 if you have a working `mmap' system call. */
#define HAVE_MMAP 1

/* Define to 1 if you have Motif 2.1 or newer. */
/* #undef HAVE_MOTIF_2_1 */

/* Define to 1 if you have the `mremap' function. */
/* #undef HAVE_MREMAP */

/* Define to 1 if you have the <net/if.h> header file. */
#define HAVE_NET_IF_H 1

/* Define to 1 if you have the <nlist.h> header file. */
/* #undef HAVE_NLIST_H */

/* Define to 1 if you are using the NeXTstep API, either GNUstep or Cocoa on
   Mac OS X. */
/* #undef HAVE_NS */

/* Define to 1 if libotf has OTF_get_variation_glyphs. */
/* #undef HAVE_OTF_GET_VARIATION_GLYPHS */

/* Define to 1 if personality LINUX32 can be set. */
/* #undef HAVE_PERSONALITY_LINUX32 */

/* Define to 1 if you have the png library (-lpng). */
#define HAVE_PNG 1

/* Define to 1 if you have the <png.h> header file. */
#define HAVE_PNG_H 1

/* Define to 1 if you have the `posix_memalign' function. */
#define HAVE_POSIX_MEMALIGN 1

/* Define to 1 if you have the `pstat_getdynamic' function. */
/* #undef HAVE_PSTAT_GETDYNAMIC */

/* Define to 1 if you have the <pthread.h> header file. */
/* #undef HAVE_PTHREAD_H */

/* Define to 1 if you have the <pty.h> header file. */
/* #undef HAVE_PTY_H */

/* Define to 1 if you have the <pwd.h> header file. */
#define HAVE_PWD_H 1

/* Define to 1 if you have the `random' function. */
#define HAVE_RANDOM 1

/* Define to 1 if you have the `recvfrom' function. */
#define HAVE_RECVFROM 1

/* Define to 1 if you have the `rename' function. */
#define HAVE_RENAME 1

/* Define to 1 if res_init is available. */
#define HAVE_RES_INIT 1

/* Define to 1 if you have the `rindex' function. */
#define HAVE_RINDEX 1

/* Define to 1 if you have the `rint' function. */
#define HAVE_RINT 1

/* Define to 1 if you have the `rmdir' function. */
#define HAVE_RMDIR 1

/* Define to 1 if using librsvg. */
/* #undef HAVE_RSVG */

/* Define to 1 if you have the `select' function. */
#define HAVE_SELECT 1

/* Define to 1 if you have the `sendto' function. */
#define HAVE_SENDTO 1

/* Define to 1 if you have the `setitimer' function. */
#define HAVE_SETITIMER 1

/* Define to 1 if you have the `setlocale' function. */
#define HAVE_SETLOCALE 1

/* Define to 1 if you have the `setpgid' function. */
#define HAVE_SETPGID 1

/* Define to 1 if you have the `setrlimit' function. */
#define HAVE_SETRLIMIT 1

/* Define to 1 if you have the `setsid' function. */
#define HAVE_SETSID 1

/* Define to 1 if you have the `setsockopt' function. */
#define HAVE_SETSOCKOPT 1

/* Define to 1 if you have the `shutdown' function. */
#define HAVE_SHUTDOWN 1

/* Define to 1 if the system has the type `size_t'. */
#define HAVE_SIZE_T 1

/* Define to 1 if you have the <soundcard.h> header file. */
/* #undef HAVE_SOUNDCARD_H */

/* Define to 1 if `speed_t' is declared by <termios.h>. */
#define HAVE_SPEED_T 1

/* Define to 1 if you have the <stdint.h> header file. */
#define HAVE_STDINT_H 1

/* Define to 1 if you have the <stdio_ext.h> header file. */
/* #undef HAVE_STDIO_EXT_H */

/* Define to 1 if you have the <stdlib.h> header file. */
#define HAVE_STDLIB_H 1

/* Define to 1 if you have the `strerror' function. */
#define HAVE_STRERROR 1

/* Define to 1 if you have the `strftime' function. */
#define HAVE_STRFTIME 1

/* Define to 1 if you have the <strings.h> header file. */
#define HAVE_STRINGS_H 1

/* Define to 1 if you have the <string.h> header file. */
#define HAVE_STRING_H 1

/* Define to 1 if you have the `strsignal' function. */
#define HAVE_STRSIGNAL 1

/* Define to 1 if `ifr_addr' is member of `struct ifreq'. */
#define HAVE_STRUCT_IFREQ_IFR_ADDR 1

/* Define to 1 if `ifr_broadaddr' is member of `struct ifreq'. */
#define HAVE_STRUCT_IFREQ_IFR_BROADADDR 1

/* Define to 1 if `ifr_flags' is member of `struct ifreq'. */
#define HAVE_STRUCT_IFREQ_IFR_FLAGS 1

/* Define to 1 if `ifr_hwaddr' is member of `struct ifreq'. */
/* #undef HAVE_STRUCT_IFREQ_IFR_HWADDR */

/* Define to 1 if `ifr_netmask' is member of `struct ifreq'. */
/* #undef HAVE_STRUCT_IFREQ_IFR_NETMASK */

/* Define to 1 if `n_un.n_name' is member of `struct nlist'. */
/* #undef HAVE_STRUCT_NLIST_N_UN_N_NAME */

/* Define to 1 if `tm_zone' is member of `struct tm'. */
#define HAVE_STRUCT_TM_TM_ZONE 1

/* Define to 1 if `struct utimbuf' is declared by <utime.h>. */
#define HAVE_STRUCT_UTIMBUF 1

/* Define to 1 if you have the `sync' function. */
#define HAVE_SYNC 1

/* Define to 1 if you have the `sysinfo' function. */
/* #undef HAVE_SYSINFO */

/* Define to 1 if you have the <sys/ioctl.h> header file. */
#define HAVE_SYS_IOCTL_H 1

/* Define to 1 if you have the <sys/mman.h> header file. */
#define HAVE_SYS_MMAN_H 1

/* Define to 1 if you have the <sys/param.h> header file. */
#define HAVE_SYS_PARAM_H 1

/* Define to 1 if you have the <sys/resource.h> header file. */
#define HAVE_SYS_RESOURCE_H 1

/* Define to 1 if you have the <sys/select.h> header file. */
#define HAVE_SYS_SELECT_H 1

/* Define to 1 if you have the <sys/socket.h> header file. */
#define HAVE_SYS_SOCKET_H 1

/* Define to 1 if you have the <sys/soundcard.h> header file. */
/* #undef HAVE_SYS_SOUNDCARD_H */

/* Define to 1 if you have the <sys/stat.h> header file. */
#define HAVE_SYS_STAT_H 1

/* Define to 1 if you have the <sys/systeminfo.h> header file. */
/* #undef HAVE_SYS_SYSTEMINFO_H */

/* Define to 1 if you have the <sys/timeb.h> header file. */
#define HAVE_SYS_TIMEB_H 1

/* Define to 1 if you have the <sys/time.h> header file. */
#define HAVE_SYS_TIME_H 1

/* Define to 1 if you have the <sys/types.h> header file. */
#define HAVE_SYS_TYPES_H 1

/* Define to 1 if you have the <sys/un.h> header file. */
#define HAVE_SYS_UN_H 1

/* Define to 1 if you have the <sys/utsname.h> header file. */
#define HAVE_SYS_UTSNAME_H 1

/* Define to 1 if you have the <sys/vlimit.h> header file. */
/* #undef HAVE_SYS_VLIMIT_H */

/* Define to 1 if you have <sys/wait.h> that is POSIX.1 compatible. */
#define HAVE_SYS_WAIT_H 1

/* Define to 1 if you have the <sys/_mbstate_t.h> header file. */
/* #undef HAVE_SYS__MBSTATE_T_H */

/* Define to 1 if you have the <termcap.h> header file. */
/* #undef HAVE_TERMCAP_H */

/* Define to 1 if you have the <termios.h> header file. */
#define HAVE_TERMIOS_H 1

/* Define to 1 if you have the <term.h> header file. */
#define HAVE_TERM_H 1

/* Define to 1 if you have the tiff library (-ltiff). */
#define HAVE_TIFF 1

/* Define to 1 if `struct timeval' is declared by <sys/time.h>. */
#define HAVE_TIMEVAL 1

/* Define to 1 if `tm_gmtoff' is member of `struct tm'. */
#define HAVE_TM_GMTOFF 1

/* Define to 1 if your `struct tm' has `tm_zone'. Deprecated, use
   `HAVE_STRUCT_TM_TM_ZONE' instead. */
#define HAVE_TM_ZONE 1

/* Define to 1 if you have the `touchlock' function. */
/* #undef HAVE_TOUCHLOCK */

/* Define to 1 if you don't have `tm_zone' but do have the external array
   `tzname'. */
/* #undef HAVE_TZNAME */

/* Define to 1 if you have the `tzset' function. */
#define HAVE_TZSET 1

/* Define to 1 if you have the `ualarm' function. */
#define HAVE_UALARM 1

/* Define to 1 if you have the <unistd.h> header file. */
#define HAVE_UNISTD_H 1

/* Define to 1 if you have the `utimes' function. */
#define HAVE_UTIMES 1

/* Define to 1 if you have the <utime.h> header file. */
#define HAVE_UTIME_H 1

/* Define to 1 if you have the <utmp.h> header file. */
/* #undef HAVE_UTMP_H */

/* Define to 1 if you have the `vfork' function. */
#define HAVE_VFORK 1

/* Define to 1 if you have the <vfork.h> header file. */
/* #undef HAVE_VFORK_H */

/* Define to 1 if `fork' works. */
#define HAVE_WORKING_FORK 1

/* Define to 1 if `vfork' works. */
#define HAVE_WORKING_VFORK 1

/* Define to 1 if you want to use version 11 of X windows. Otherwise, Emacs
   expects to use version 10. */
#define HAVE_X11 1

/* Define to 1 if you have the X11R6 or newer version of Xlib. */
#define HAVE_X11R6 1

/* Define to 1 if you have the X11R6 or newer version of Xt. */
#define HAVE_X11XTR6 1

/* Define to 1 if you have the Xaw3d library (-lXaw3d). */
/* #undef HAVE_XAW3D */

/* Define to 1 if you have the Xft library. */
/* #undef HAVE_XFT */

/* Define to 1 if XIM is available */
#define HAVE_XIM 1

/* Define to 1 if you have the XkbGetKeyboard function. */
#define HAVE_XKBGETKEYBOARD 1

/* Define to 1 if you have the Xpm libary (-lXpm). */
#define HAVE_XPM 1

/* Define to 1 if you have the `XrmSetDatabase' function. */
#define HAVE_XRMSETDATABASE 1

/* Define to 1 if you have the `XScreenNumberOfScreen' function. */
#define HAVE_XSCREENNUMBEROFSCREEN 1

/* Define to 1 if you have the `XScreenResourceString' function. */
#define HAVE_XSCREENRESOURCESTRING 1

/* Define to 1 if you have the `XSetWMProtocols' function. */
#define HAVE_XSETWMPROTOCOLS 1

/* Define to 1 if you have the SM library (-lSM). */
#define HAVE_X_SM 1

/* Define to 1 if you want to use the X window system. */
#define HAVE_X_WINDOWS 1

/* Define to 1 if you have the `__fpending' function. */
/* #undef HAVE___FPENDING */

/* Define to support using a Hesiod database to find the POP server. */
/* #undef HESIOD */

/* Define to support Kerberos-authenticated POP mail retrieval. */
/* #undef KERBEROS */

/* Define to use Kerberos 5 instead of Kerberos 4. */
/* #undef KERBEROS5 */

/* Define LD_SWITCH_X_SITE to contain any special flags your loader may need
   to deal with X Windows. For instance, if you've defined HAVE_X_WINDOWS
   above and your X libraries aren't in a place that your loader can find on
   its own, you might want to add "-L/..." or something similar. */
#define LD_SWITCH_X_SITE 

/* Define LD_SWITCH_X_SITE_AUX with an -R option in case it's needed (for
   Solaris, for example). */
#define LD_SWITCH_X_SITE_AUX 

/* Compiler option to link with the gif library (if not -lgif). */
/* #undef LIBGIF */

/* Define to 1 if localtime caches TZ. */
/* #undef LOCALTIME_CACHE */

/* Define to support POP mail retrieval. */
#define MAIL_USE_POP 1

/* Define to 1 if your `struct nlist' has an `n_un' member. Obsolete, depend
   on `HAVE_STRUCT_NLIST_N_UN_N_NAME */
/* #undef NLIST_NAME_UNION */

/* Define to 1 if you don't have struct exception in math.h. */
#define NO_MATHERR 1

/* Define to 1 if `NSInteger' is defined. */
/* #undef NS_HAVE_NSINTEGER */

/* Define to 1 if you are using NS windowing under MacOS X. */
/* #undef NS_IMPL_COCOA */

/* Define to 1 if you are using NS windowing under GNUstep. */
/* #undef NS_IMPL_GNUSTEP */

/* Define to the address where bug reports for this package should be sent. */
#define PACKAGE_BUGREPORT ""

/* Define to the full name of this package. */
#define PACKAGE_NAME "emacs"

/* Define to the full name and version of this package. */
#define PACKAGE_STRING "emacs 23.2"

/* Define to the one symbol short name of this package. */
#define PACKAGE_TARNAME "emacs"

/* Define to the version of this package. */
#define PACKAGE_VERSION "23.2"

/* Define as `void' if your compiler accepts `void *'; otherwise define as
   `char'. */
#define POINTER_TYPE void

/* Define to 1 if the C compiler supports function prototypes. */
#define PROTOTYPES 1

/* Define REL_ALLOC if you want to use the relocating allocator for buffer
   space. */
/* #undef REL_ALLOC */

/* Define as the return type of signal handlers (`int' or `void'). */
#define RETSIGTYPE void

/* If using the C implementation of alloca, define if you know the
   direction of stack growth for your system; otherwise it will be
   automatically deduced at runtime.
	STACK_DIRECTION > 0 => grows toward higher addresses
	STACK_DIRECTION < 0 => grows toward lower addresses
	STACK_DIRECTION = 0 => direction of growth unknown */
/* #undef STACK_DIRECTION */

/* Define to 1 if you have the ANSI C header files. */
#define STDC_HEADERS 1

/* Define to 1 on System V Release 4. */
/* #undef SVR4 */

/* Process async input synchronously. */
#define SYNC_INPUT 1

/* Define to 1 if you can safely include both <sys/time.h> and <time.h>. */
#define TIME_WITH_SYS_TIME 1

/* Define to 1 if your <sys/time.h> declares `struct tm'. */
/* #undef TM_IN_SYS_TIME */

/* Define to 1 for Encore UMAX. */
/* #undef UMAX */

/* Define to 1 for Encore UMAX 4.3 that has <inq_status/cpustats.h> instead of
   <sys/cpustats.h>. */
/* #undef UMAX4_3 */

/* Define to the unexec source file name. */
#define UNEXEC_SRC 

/* Define to 1 if using GTK. */
/* #undef USE_GTK */

/* Define to 1 if using the Lucid X toolkit. */
#define USE_LUCID 1

/* Define to 1 if using the Motif X toolkit. */
/* #undef USE_MOTIF */

/* Define to 1 if we should use toolkit scroll bars. */
/* #undef USE_TOOLKIT_SCROLL_BARS */

/* Define to 1 if we should use XIM, if it is available. */
#define USE_XIM 1

/* Define to 1 if using an X toolkit. */
#define USE_X_TOOLKIT 1

/* Define this to check for malloc buffer overrun. */
/* #undef XMALLOC_OVERRUN_CHECK */

/* Define to the type of the 6th arg of XRegisterIMInstantiateCallback, either
   XPointer or XPointer*. */
#define XRegisterIMInstantiateCallback_arg6 XPointer

/* Number of bits in a file offset, on hosts where this is settable. */
/* #undef _FILE_OFFSET_BITS */

/* Define to 1 to make fseeko visible on some hosts (e.g. glibc 2.2). */
/* #undef _LARGEFILE_SOURCE */

/* Define for large files, on AIX-style hosts. */
/* #undef _LARGE_FILES */

/* Define to 1 if on MINIX. */
/* #undef _MINIX */

/* Define to 2 if the system does not provide POSIX.1 features except with
   this defined. */
/* #undef _POSIX_1_SOURCE */

/* Define to 1 if you need to in order for `stat' and other things to work. */
/* #undef _POSIX_SOURCE */

/* Enable extensions on AIX 3, Interix.  */
#ifndef _ALL_SOURCE
# define _ALL_SOURCE 1
#endif
/* Enable GNU extensions on systems that have them.  */
#ifndef _GNU_SOURCE
# define _GNU_SOURCE 1
#endif
/* Enable threading extensions on Solaris.  */
#ifndef _POSIX_PTHREAD_SEMANTICS
# define _POSIX_PTHREAD_SEMANTICS 1
#endif
/* Enable extensions on HP NonStop.  */
#ifndef _TANDEM_SOURCE
# define _TANDEM_SOURCE 1
#endif
/* Enable general extensions on Solaris.  */
#ifndef __EXTENSIONS__
# define __EXTENSIONS__ 1
#endif


/* Define to rpl_ if the getopt replacement functions and variables should be
   used. */
#define __GETOPT_PREFIX rpl_

/* Define like PROTOTYPES; this can be used by system headers. */
#define __PROTOTYPES 1

/* Define to compiler's equivalent of C99 restrict keyword. Don't define if
   equivalent is `__restrict'. */
/* #undef __restrict */

/* Define to compiler's equivalent of C99 restrict keyword in array
   declarations. Define as empty for no equivalent. */
#define __restrict_arr __restrict

/* Define to the used machine dependent file. */
#define config_machfile "m/arm.h"

/* Define to the used os dependent file. */
#define config_opsysfile "s/darwin.h"

/* Define to empty if `const' does not conform to ANSI C. */
/* #undef const */

/* Define to a type if <wchar.h> does not define. */
/* #undef mbstate_t */

/* Define to `int' if <sys/types.h> does not define. */
/* #undef pid_t */

/* Define to any substitute for sys_siglist. */
/* #undef sys_siglist */

/* Define as `fork' if `vfork' does not work. */
/* #undef vfork */

/* Define to empty if the keyword `volatile' does not work. Warning: valid
   code using `volatile' can become incorrect without. Disable with care. */
/* #undef volatile */


/* If we're using X11/Carbon/GNUstep, define some consequences.  */
#if defined(HAVE_X_WINDOWS) || defined(HAVE_NS)
#define HAVE_WINDOW_SYSTEM
#define HAVE_MOUSE
#endif

/* Define AMPERSAND_FULL_NAME if you use the convention
   that & in the full name stands for the login id.  */
/* Turned on June 1996 supposing nobody will mind it.  */
#define AMPERSAND_FULL_NAME

/* Define HAVE_SOUND if we have sound support.  We know it works
   and compiles only on the specified platforms.   For others,
   it probably doesn't make sense to try.  */

#if defined __FreeBSD__ || defined __NetBSD__ || defined __linux__
#ifdef HAVE_MACHINE_SOUNDCARD_H
#define HAVE_SOUND 1
#endif
#ifdef HAVE_SYS_SOUNDCARD_H
#define HAVE_SOUND 1
#endif
#ifdef HAVE_SOUNDCARD_H
#define HAVE_SOUND 1
#endif
#ifdef HAVE_ALSA
#define HAVE_SOUND 1
#endif
#endif /* __FreeBSD__ || __NetBSD__ || __linux__  */

/* If using GNU, then support inline function declarations. */
/* Don't try to switch on inline handling as detected by AC_C_INLINE
   generally, because even if non-gcc compilers accept `inline', they
   may reject `extern inline'.  */
#if defined (__GNUC__) && defined (OPTIMIZE)
#define INLINE __inline__
#else
#define INLINE
#endif

/* `subprocesses' should be defined if you want to
   have code for asynchronous subprocesses
   (as used in M-x compile and M-x shell).
   Only MSDOS does not support this (it overrides
   this in its config_opsysfile below).  */

#define subprocesses

/* Include the os and machine dependent files.  */
#include config_opsysfile
#include config_machfile

/* Set up some defines, C and LD flags for NeXTstep interface on GNUstep.
  (There is probably a better place to do this, but right now the Cocoa
   side does this in s/darwin.h and we cannot
   parallel this exactly since GNUstep is multi-OS. */
#ifdef HAVE_NS
# ifdef NS_IMPL_GNUSTEP
/* See also .m.o rule in Makefile.in */
/* FIXME: are all these flags really needed?  Document here why.  */
#  define C_SWITCH_X_SYSTEM -D_REENTRANT -fPIC -fno-strict-aliasing
/* GNUstep needs a bit more pure memory.  Of the existing knobs,
SYSTEM_PURESIZE_EXTRA seems like the least likely to cause problems.  */
#  define SYSTEM_PURESIZE_EXTRA 30000
# endif /* NS_IMPL_GNUSTEP */
#endif /* HAVE_NS */

/* SIGTYPE is the macro we actually use.  */
#ifndef SIGTYPE
#define SIGTYPE RETSIGTYPE
#endif

#ifdef emacs /* Don't do this for lib-src.  */
/* Tell regex.c to use a type compatible with Emacs.  */
#define RE_TRANSLATE_TYPE Lisp_Object
#define RE_TRANSLATE(TBL, C) CHAR_TABLE_TRANSLATE (TBL, C)
#ifdef make_number
/* If make_number is a macro, use it.  */
#define RE_TRANSLATE_P(TBL) (!EQ (TBL, make_number (0)))
#else
/* If make_number is a function, avoid it.  */
#define RE_TRANSLATE_P(TBL) (!(INTEGERP (TBL) && XINT (TBL) == 0))
#endif
#endif

/* Avoid link-time collision with system mktime if we will use our own.  */
#if ! HAVE_MKTIME || BROKEN_MKTIME
#define mktime emacs_mktime
#endif

#define my_strftime nstrftime	/* for strftime.c */

/* The rest of the code currently tests the CPP symbol BSTRING.
   Override any claims made by the system-description files.
   Note that on some SCO version it is possible to have bcopy and not bcmp.  */
/* #undef BSTRING */
#if defined (HAVE_BCOPY) && defined (HAVE_BCMP)
#define BSTRING
#endif

/* Some of the files of Emacs which are intended for use with other
   programs assume that if you have a config.h file, you must declare
   the type of getenv.

   This declaration shouldn't appear when alloca.s or Makefile.in
   includes config.h.  */
#ifndef NOT_C_CODE
extern char *getenv ();
#endif

/* These default definitions are good for almost all machines.
   The exceptions override them in m/MACHINE.h.  */

#ifndef BITS_PER_CHAR
#define BITS_PER_CHAR 8
#endif

#ifndef BITS_PER_SHORT
#define BITS_PER_SHORT 16
#endif

/* Note that lisp.h uses this in a preprocessor conditional, so it
   would not work to use sizeof.  That being so, we do all of them
   without sizeof, for uniformity's sake.  */
#ifndef BITS_PER_INT
#define BITS_PER_INT 32
#endif

#ifndef BITS_PER_LONG
#ifdef _LP64
#define BITS_PER_LONG 64
#else
#define BITS_PER_LONG 32
#endif
#endif

/* Define if the compiler supports function prototypes.  It may do so
   but not define __STDC__ (e.g. DEC C by default) or may define it as
   zero.  */
#define PROTOTYPES 1
/* For mktime.c:  */
#ifndef __P
# if defined PROTOTYPES
#  define __P(args) args
# else
#  define __P(args) ()
# endif  /* GCC.  */
#endif /* __P */

/* Don't include "string.h" or <stdlib.h> in non-C code.  */
#ifndef NOT_C_CODE
#ifdef HAVE_STRING_H
#include "string.h"
#endif
#ifdef HAVE_STRINGS_H
#include "strings.h"  /* May be needed for bcopy & al. */
#endif
#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif
#ifndef __GNUC__
# ifdef HAVE_ALLOCA_H
#  include <alloca.h>
# else /* AIX files deal with #pragma.  */
#  ifndef alloca /* predefined by HP cc +Olibcalls */
char *alloca ();
#  endif
# endif /* HAVE_ALLOCA_H */
#endif /* __GNUC__ */
#ifndef HAVE_SIZE_T
typedef unsigned size_t;
#endif
#endif /* NOT_C_CODE */

/* Define HAVE_X_I18N if we have usable i18n support.  */

#ifdef HAVE_X11R6
#define HAVE_X_I18N
#elif !defined X11R5_INHIBIT_I18N
#define HAVE_X_I18N
#endif

/* Define HAVE_X11R6_XIM if we have usable X11R6-style XIM support.  */

#if defined HAVE_X11R6 && !defined INHIBIT_X11R6_XIM
#define HAVE_X11R6_XIM
#endif

#if defined __GNUC__ && (__GNUC__ > 2 \
                         || (__GNUC__ == 2 && __GNUC_MINOR__ >= 5))
#define NO_RETURN	__attribute__ ((__noreturn__))
#else
#define NO_RETURN	/* nothing */
#endif

/* These won't be used automatically yet.  We also need to know, at least,
   that the stack is continuous.  */
#ifdef __GNUC__
#  ifndef GC_SETJMP_WORKS
  /* GC_SETJMP_WORKS is nearly always appropriate for GCC --
     see NON_SAVING_SETJMP in the target descriptions.  */
  /* Exceptions (see NON_SAVING_SETJMP in target description) are
     SCO5 non-ELF (but Emacs specifies ELF) and SVR3 on x86.
     Fixme: Deal with SVR3.  */
#    define GC_SETJMP_WORKS 1
#  endif
#  ifndef GC_LISP_OBJECT_ALIGNMENT
#    define GC_LISP_OBJECT_ALIGNMENT (__alignof__ (Lisp_Object))
#  endif
#endif

#ifndef HAVE_BCOPY
#define bcopy(a,b,s) memcpy (b,a,s)
#endif
#ifndef HAVE_BZERO
#define bzero(a,s) memset (a,0,s)
#endif
#ifndef HAVE_BCMP
#define BCMP memcmp
#endif

#endif /* EMACS_CONFIG_H */

/*
Local Variables:
mode: c
End:
*/

