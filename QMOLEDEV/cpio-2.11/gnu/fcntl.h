/* DO NOT EDIT! GENERATED AUTOMATICALLY! */
/* -*- buffer-read-only: t -*- vi: set ro: */
/* DO NOT EDIT! GENERATED AUTOMATICALLY! */
/* Like <fcntl.h>, but with non-working flags defined to 0.

   Copyright (C) 2006-2010 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* written by Paul Eggert */

#if __GNUC__ >= 3
#pragma GCC system_header
#endif

#if defined __need_system_fcntl_h
/* Special invocation convention.  */

#include <sys/types.h>
#ifndef __GLIBC__ /* Avoid namespace pollution on glibc systems.  */
# include <sys/stat.h>
#endif
#include_next <fcntl.h>

#else
/* Normal invocation convention.  */

#ifndef _GL_FCNTL_H

#include <sys/types.h>
#ifndef __GLIBC__ /* Avoid namespace pollution on glibc systems.  */
# include <sys/stat.h>
#endif
/* The include_next requires a split double-inclusion guard.  */
#include_next <fcntl.h>

#ifndef _GL_FCNTL_H
#define _GL_FCNTL_H

#ifndef __GLIBC__ /* Avoid namespace pollution on glibc systems.  */
# include <unistd.h>
#endif


/* The definitions of _GL_FUNCDECL_RPL etc. are copied here.  */
#ifndef _GL_CXXDEFS_H
#define _GL_CXXDEFS_H

/* The three most frequent use cases of these macros are:

   * For providing a substitute for a function that is missing on some
     platforms, but is declared and works fine on the platforms on which
     it exists:

       #if @GNULIB_FOO@
       # if !@HAVE_FOO@
       _GL_FUNCDECL_SYS (foo, ...);
       # endif
       _GL_CXXALIAS_SYS (foo, ...);
       _GL_CXXALIASWARN (foo);
       #elif defined GNULIB_POSIXCHECK
       ...
       #endif

   * For providing a replacement for a function that exists on all platforms,
     but is broken/insufficient and needs to be replaced on some platforms:

       #if @GNULIB_FOO@
       # if @REPLACE_FOO@
       #  if !(defined __cplusplus && defined GNULIB_NAMESPACE)
       #   undef foo
       #   define foo rpl_foo
       #  endif
       _GL_FUNCDECL_RPL (foo, ...);
       _GL_CXXALIAS_RPL (foo, ...);
       # else
       _GL_CXXALIAS_SYS (foo, ...);
       # endif
       _GL_CXXALIASWARN (foo);
       #elif defined GNULIB_POSIXCHECK
       ...
       #endif

   * For providing a replacement for a function that exists on some platforms
     but is broken/insufficient and needs to be replaced on some of them and
     is additionally either missing or undeclared on some other platforms:

       #if @GNULIB_FOO@
       # if @REPLACE_FOO@
       #  if !(defined __cplusplus && defined GNULIB_NAMESPACE)
       #   undef foo
       #   define foo rpl_foo
       #  endif
       _GL_FUNCDECL_RPL (foo, ...);
       _GL_CXXALIAS_RPL (foo, ...);
       # else
       #  if !@HAVE_FOO@   or   if !@HAVE_DECL_FOO@
       _GL_FUNCDECL_SYS (foo, ...);
       #  endif
       _GL_CXXALIAS_SYS (foo, ...);
       # endif
       _GL_CXXALIASWARN (foo);
       #elif defined GNULIB_POSIXCHECK
       ...
       #endif
*/

/* _GL_EXTERN_C declaration;
   performs the declaration with C linkage.  */
#if defined __cplusplus
# define _GL_EXTERN_C extern "C"
#else
# define _GL_EXTERN_C extern
#endif

/* _GL_FUNCDECL_RPL (func, rettype, parameters_and_attributes);
   declares a replacement function, named rpl_func, with the given prototype,
   consisting of return type, parameters, and attributes.
   Example:
     _GL_FUNCDECL_RPL (open, int, (const char *filename, int flags, ...)
                                  _GL_ARG_NONNULL ((1)));
 */
#define _GL_FUNCDECL_RPL(func,rettype,parameters_and_attributes) \
  _GL_FUNCDECL_RPL_1 (rpl_##func, rettype, parameters_and_attributes)
#define _GL_FUNCDECL_RPL_1(rpl_func,rettype,parameters_and_attributes) \
  _GL_EXTERN_C rettype rpl_func parameters_and_attributes

/* _GL_FUNCDECL_SYS (func, rettype, parameters_and_attributes);
   declares the system function, named func, with the given prototype,
   consisting of return type, parameters, and attributes.
   Example:
     _GL_FUNCDECL_SYS (open, int, (const char *filename, int flags, ...)
                                  _GL_ARG_NONNULL ((1)));
 */
#define _GL_FUNCDECL_SYS(func,rettype,parameters_and_attributes) \
  _GL_EXTERN_C rettype func parameters_and_attributes

/* _GL_CXXALIAS_RPL (func, rettype, parameters);
   declares a C++ alias called GNULIB_NAMESPACE::func
   that redirects to rpl_func, if GNULIB_NAMESPACE is defined.
   Example:
     _GL_CXXALIAS_RPL (open, int, (const char *filename, int flags, ...));
 */
#define _GL_CXXALIAS_RPL(func,rettype,parameters) \
  _GL_CXXALIAS_RPL_1 (func, rpl_##func, rettype, parameters)
#if defined __cplusplus && defined GNULIB_NAMESPACE
# define _GL_CXXALIAS_RPL_1(func,rpl_func,rettype,parameters) \
    namespace GNULIB_NAMESPACE                                \
    {                                                         \
      rettype (*const func) parameters = ::rpl_func;          \
    }                                                         \
    _GL_EXTERN_C int _gl_cxxalias_dummy
#else
# define _GL_CXXALIAS_RPL_1(func,rpl_func,rettype,parameters) \
    _GL_EXTERN_C int _gl_cxxalias_dummy
#endif

/* _GL_CXXALIAS_SYS (func, rettype, parameters);
   declares a C++ alias called GNULIB_NAMESPACE::func
   that redirects to the system provided function func, if GNULIB_NAMESPACE
   is defined.
   Example:
     _GL_CXXALIAS_SYS (open, int, (const char *filename, int flags, ...));
 */
#if defined __cplusplus && defined GNULIB_NAMESPACE
  /* If we were to write
       rettype (*const func) parameters = ::func;
     like above in _GL_CXXALIAS_RPL_1, the compiler could optimize calls
     better (remove an indirection through a 'static' pointer variable),
     but then the _GL_CXXALIASWARN macro below would cause a warning not only
     for uses of ::func but also for uses of GNULIB_NAMESPACE::func.  */
# define _GL_CXXALIAS_SYS(func,rettype,parameters) \
    namespace GNULIB_NAMESPACE                     \
    {                                              \
      static rettype (*func) parameters = ::func;  \
    }                                              \
    _GL_EXTERN_C int _gl_cxxalias_dummy
#else
# define _GL_CXXALIAS_SYS(func,rettype,parameters) \
    _GL_EXTERN_C int _gl_cxxalias_dummy
#endif

/* _GL_CXXALIAS_SYS_CAST (func, rettype, parameters);
   is like  _GL_CXXALIAS_SYS (func, rettype, parameters);
   except that the C function func may have a slightly different declaration.
   A cast is used to silence the "invalid conversion" error that would
   otherwise occur.  */
#if defined __cplusplus && defined GNULIB_NAMESPACE
# define _GL_CXXALIAS_SYS_CAST(func,rettype,parameters) \
    namespace GNULIB_NAMESPACE                          \
    {                                                   \
      static rettype (*func) parameters =               \
        reinterpret_cast<rettype(*)parameters>(::func); \
    }                                                   \
    _GL_EXTERN_C int _gl_cxxalias_dummy
#else
# define _GL_CXXALIAS_SYS_CAST(func,rettype,parameters) \
    _GL_EXTERN_C int _gl_cxxalias_dummy
#endif

/* _GL_CXXALIAS_SYS_CAST2 (func, rettype, parameters, rettype2, parameters2);
   is like  _GL_CXXALIAS_SYS (func, rettype, parameters);
   except that the C function is picked among a set of overloaded functions,
   namely the one with rettype2 and parameters2.  Two consecutive casts
   are used to silence the "cannot find a match" and "invalid conversion"
   errors that would otherwise occur.  */
#if defined __cplusplus && defined GNULIB_NAMESPACE
# define _GL_CXXALIAS_SYS_CAST2(func,rettype,parameters,rettype2,parameters2) \
    namespace GNULIB_NAMESPACE                                                \
    {                                                                         \
      static rettype (*func) parameters =                                     \
        reinterpret_cast<rettype(*)parameters>(                               \
          reinterpret_cast<rettype2(*)parameters2>(::func));                  \
    }                                                                         \
    _GL_EXTERN_C int _gl_cxxalias_dummy
#else
# define _GL_CXXALIAS_SYS_CAST(func,rettype,parameters) \
    _GL_EXTERN_C int _gl_cxxalias_dummy
#endif

/* _GL_CXXALIASWARN (func);
   causes a warning to be emitted when ::func is used but not when
   GNULIB_NAMESPACE::func is used.  */
#if defined __cplusplus && defined GNULIB_NAMESPACE
# define _GL_CXXALIASWARN(func) \
   _GL_CXXALIASWARN1 (func, GNULIB_NAMESPACE)
# define _GL_CXXALIASWARN1(func,namespace) \
   _GL_CXXALIASWARN2 (func, namespace)
# define _GL_CXXALIASWARN2(func,namespace) \
   _GL_WARN_ON_USE (func, \
                    "The symbol ::" #func " refers to the system function. " \
                    "Use " #namespace "::" #func " instead.")
#else
# define _GL_CXXALIASWARN(func) \
    _GL_EXTERN_C int _gl_cxxalias_dummy
#endif

#endif /* _GL_CXXDEFS_H */

/* The definition of _GL_ARG_NONNULL is copied here.  */
/* _GL_ARG_NONNULL((n,...,m)) tells the compiler and static analyzer tools
   that the values passed as arguments n, ..., m must be non-NULL pointers.
   n = 1 stands for the first argument, n = 2 for the second argument etc.  */
#ifndef _GL_ARG_NONNULL
# if (__GNUC__ == 3 && __GNUC_MINOR__ >= 3) || __GNUC__ > 3
#  define _GL_ARG_NONNULL(params) __attribute__ ((__nonnull__ params))
# else
#  define _GL_ARG_NONNULL(params)
# endif
#endif

/* The definition of _GL_WARN_ON_USE is copied here.  */
#ifndef _GL_WARN_ON_USE

# if 4 < __GNUC__ || (__GNUC__ == 4 && 3 <= __GNUC_MINOR__)
/* A compiler attribute is available in gcc versions 4.3.0 and later.  */
#  define _GL_WARN_ON_USE(function, message) \
extern __typeof__ (function) function __attribute__ ((__warning__ (message)))

# else /* Unsupported.  */
#  define _GL_WARN_ON_USE(function, message) \
extern int _gl_warn_on_use
# endif
#endif


/* Declare overridden functions.  */

#if 1
# if 1
#  if !(defined __cplusplus && defined GNULIB_NAMESPACE)
#   undef fcntl
#   define fcntl rpl_fcntl
#  endif
_GL_FUNCDECL_RPL (fcntl, int, (int fd, int action, ...));
_GL_CXXALIAS_RPL (fcntl, int, (int fd, int action, ...));
# else
#  if !1
_GL_FUNCDECL_SYS (fcntl, int, (int fd, int action, ...));
#  endif
_GL_CXXALIAS_SYS (fcntl, int, (int fd, int action, ...));
# endif
_GL_CXXALIASWARN (fcntl);
#elif defined GNULIB_POSIXCHECK
# undef fcntl
# if HAVE_RAW_DECL_FCNTL
_GL_WARN_ON_USE (fcntl, "fcntl is not always POSIX compliant - "
                 "use gnulib module fcntl for portability");
# endif
#endif

#if 1
# if 1
#  if !(defined __cplusplus && defined GNULIB_NAMESPACE)
#   undef open
#   define open rpl_open
#  endif
_GL_FUNCDECL_RPL (open, int, (const char *filename, int flags, ...)
                             _GL_ARG_NONNULL ((1)));
_GL_CXXALIAS_RPL (open, int, (const char *filename, int flags, ...));
# else
_GL_CXXALIAS_SYS (open, int, (const char *filename, int flags, ...));
# endif
_GL_CXXALIASWARN (open);
#elif defined GNULIB_POSIXCHECK
# undef open
/* Assume open is always declared.  */
_GL_WARN_ON_USE (open, "open is not always POSIX compliant - "
                 "use gnulib module open for portability");
#endif

#if 1
# if 0
#  if !(defined __cplusplus && defined GNULIB_NAMESPACE)
#   undef openat
#   define openat rpl_openat
#  endif
_GL_FUNCDECL_RPL (openat, int,
                  (int fd, char const *file, int flags, /* mode_t mode */ ...)
                  _GL_ARG_NONNULL ((2)));
_GL_CXXALIAS_RPL (openat, int,
                  (int fd, char const *file, int flags, /* mode_t mode */ ...));
# else
#  if !0
_GL_FUNCDECL_SYS (openat, int,
                  (int fd, char const *file, int flags, /* mode_t mode */ ...)
                  _GL_ARG_NONNULL ((2)));
#  endif
_GL_CXXALIAS_SYS (openat, int,
                  (int fd, char const *file, int flags, /* mode_t mode */ ...));
# endif
_GL_CXXALIASWARN (openat);
#elif defined GNULIB_POSIXCHECK
# undef openat
# if HAVE_RAW_DECL_OPENAT
_GL_WARN_ON_USE (openat, "openat is not portable - "
                 "use gnulib module openat for portability");
# endif
#endif


/* Fix up the FD_* macros, only known to be missing on mingw.  */

#ifndef FD_CLOEXEC
# define FD_CLOEXEC 1
#endif

/* Fix up the supported F_* macros.  Intentionally leave other F_*
   macros undefined.  Only known to be missing on mingw.  */

#ifndef F_DUPFD_CLOEXEC
# define F_DUPFD_CLOEXEC 0x40000000
/* Witness variable: 1 if gnulib defined F_DUPFD_CLOEXEC, 0 otherwise.  */
# define GNULIB_defined_F_DUPFD_CLOEXEC 1
#else
# define GNULIB_defined_F_DUPFD_CLOEXEC 0
#endif

#ifndef F_DUPFD
# define F_DUPFD 1
#endif

#ifndef F_GETFD
# define F_GETFD 2
#endif

/* Fix up the O_* macros.  */

#if !defined O_DIRECT && defined O_DIRECTIO
/* Tru64 spells it `O_DIRECTIO'.  */
# define O_DIRECT O_DIRECTIO
#endif

#if !defined O_CLOEXEC && defined O_NOINHERIT
/* Mingw spells it `O_NOINHERIT'.  Intentionally leave it
   undefined if not available.  */
# define O_CLOEXEC O_NOINHERIT
#endif

#ifndef O_DIRECT
# define O_DIRECT 0
#endif

#ifndef O_DIRECTORY
# define O_DIRECTORY 0
#endif

#ifndef O_DSYNC
# define O_DSYNC 0
#endif

#ifndef O_NDELAY
# define O_NDELAY 0
#endif

#ifndef O_NOATIME
# define O_NOATIME 0
#endif

#ifndef O_NONBLOCK
# define O_NONBLOCK O_NDELAY
#endif

#ifndef O_NOCTTY
# define O_NOCTTY 0
#endif

#ifndef O_NOFOLLOW
# define O_NOFOLLOW 0
#endif

#ifndef O_NOLINKS
# define O_NOLINKS 0
#endif

#ifndef O_RSYNC
# define O_RSYNC 0
#endif

#ifndef O_SYNC
# define O_SYNC 0
#endif

#ifndef O_TTY_INIT
# define O_TTY_INIT 0
#endif

/* For systems that distinguish between text and binary I/O.
   O_BINARY is usually declared in fcntl.h  */
#if !defined O_BINARY && defined _O_BINARY
  /* For MSC-compatible compilers.  */
# define O_BINARY _O_BINARY
# define O_TEXT _O_TEXT
#endif

#if defined __BEOS__ || defined __HAIKU__
  /* BeOS 5 and Haiku have O_BINARY and O_TEXT, but they have no effect.  */
# undef O_BINARY
# undef O_TEXT
#endif

#ifndef O_BINARY
# define O_BINARY 0
# define O_TEXT 0
#endif

/* Fix up the AT_* macros.  */

/* Work around a bug in Solaris 9 and 10: AT_FDCWD is positive.  Its
   value exceeds INT_MAX, so its use as an int doesn't conform to the
   C standard, and GCC and Sun C complain in some cases.  If the bug
   is present, undef AT_FDCWD here, so it can be redefined below.  */
#if 0 < AT_FDCWD && AT_FDCWD == 0xffd19553
# undef AT_FDCWD
#endif

/* Use the same bit pattern as Solaris 9, but with the proper
   signedness.  The bit pattern is important, in case this actually is
   Solaris with the above workaround.  */
#ifndef AT_FDCWD
# define AT_FDCWD (-3041965)
#endif

/* Use the same values as Solaris 9.  This shouldn't matter, but
   there's no real reason to differ.  */
#ifndef AT_SYMLINK_NOFOLLOW
# define AT_SYMLINK_NOFOLLOW 4096
#endif

#ifndef AT_REMOVEDIR
# define AT_REMOVEDIR 1
#endif

/* Solaris 9 lacks these two, so just pick unique values.  */
#ifndef AT_SYMLINK_FOLLOW
# define AT_SYMLINK_FOLLOW 2
#endif

#ifndef AT_EACCESS
# define AT_EACCESS 4
#endif


#endif /* _GL_FCNTL_H */
#endif /* _GL_FCNTL_H */
#endif
