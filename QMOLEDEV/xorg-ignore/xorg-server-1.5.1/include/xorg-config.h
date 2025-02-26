/* include/xorg-config.h.  Generated from xorg-config.h.in by configure.  */
/* xorg-config.h.in: not at all generated.                      -*- c -*-
 * 
 * This file differs from xorg-server.h.in in that -server is installed
 * with the rest of the SDK for external drivers/modules to use, whereas
 * -config is for internal use only (i.e. building the DDX).
 *
 */

#ifndef _XORG_CONFIG_H_
#define _XORG_CONFIG_H_

#include <dix-config.h>
#include <xkb-config.h>

/* Building Xorg server. */
/* #undef XORGSERVER */

/* Current X.Org version. */
/* #undef XORG_VERSION_CURRENT */

/* Need XFree86 libc-replacement typedefs. */
/* #undef NEED_XF86_TYPES */

/* Need XFree86 libc-replacement functions. */
/* #undef NEED_XF86_PROTOTYPES */

/* Name of X server. */
/* #undef __XSERVERNAME__ */

/* URL to go to for support. */
/* #undef __VENDORDWEBSUPPORT__ */

/* Built-in output drivers. */
/* #undef DRIVERS */

/* Built-in input drivers. */
/* #undef IDRIVERS */

/* Path to configuration file. */
/* #undef XF86CONFIGFILE */

/* Path to configuration file. */
/* #undef __XCONFIGFILE__ */

/* Path to loadable modules. */
/* #undef DEFAULT_MODULE_PATH */

/* Path to installed libraries. */
/* #undef DEFAULT_LIBRARY_PATH */

/* Path to server log file. */
/* #undef DEFAULT_LOGPREFIX */

/* Building DRI-capable DDX. */
/* #undef XF86DRI */

/* Build DRI2 extension */
/* #undef DRI2 */

/* Solaris 8 or later? */
/* #undef __SOL8__ */

/* Define to 1 if you have the `walkcontext' function (used on Solaris for
   xorg_backtrace in hw/xfree86/common/xf86Events.c */
/* #undef HAVE_WALKCONTEXT */

/* Define to 1 if unsigned long is 64 bits. */
/* #undef _XSERVER64 */

/* Building vgahw module */
/* #undef WITH_VGAHW */

/* Define to 1 if NetBSD built-in MTRR support is available */
/* #undef HAS_MTRR_BUILTIN */

/* Define to 1 if BSD MTRR support is available */
/* #undef HAS_MTRR_SUPPORT */

/* NetBSD PIO alpha IO */
/* #undef USE_ALPHA_PIO */

/* BSD AMD64 iopl */
/* #undef USE_AMD64_IOPL */

/* BSD /dev/io */
/* #undef USE_DEV_IO */

/* BSD i386 iopl */
/* #undef USE_I386_IOPL */

/* System is BSD-like */
/* #undef CSRG_BASED */

/* System has PC console */
/* #undef PCCONS_SUPPORT */

/* System has PCVT console */
/* #undef PCVT_SUPPORT */

/* System has syscons console */
/* #undef SYSCONS_SUPPORT */

/* System has wscons console */
/* #undef WSCONS_SUPPORT */

/* System has /dev/xf86 aperture driver */
/* #undef HAS_APERTURE_DRV */

/* Has backtrace support */
#define HAVE_BACKTRACE 1

/* Name of the period field in struct kbd_repeat */
/* #undef LNX_KBD_PERIOD_NAME */

/* Have execinfo.h */
#define HAVE_EXECINFO_H 1

/* Have pci_system_init_dev_mem() */
/* #undef HAVE_PCI_SYSTEM_INIT_DEV_MEM */

/* Have pci_enable_device */
/* #undef HAVE_PCI_DEVICE_ENABLE */

/* Path to text files containing PCI IDs */
#define PCI_TXT_IDS_PATH ""

#endif /* _XORG_CONFIG_H_ */
