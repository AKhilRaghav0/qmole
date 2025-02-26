/* DO NOT EDIT! GENERATED AUTOMATICALLY! */
/* Provide a netdb.h header file for systems lacking it (read: MinGW).
   Copyright (C) 2008-2011 Free Software Foundation, Inc.
   Written by Simon Josefsson.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU Lesser General Public License as published by
   the Free Software Foundation; either version 2.1, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public License
   along with this program; if not, write to the Free Software Foundation,
   Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.  */

/* This file is supposed to be used on platforms that lack <netdb.h>.
   It is intended to provide definitions and prototypes needed by an
   application.  */

#ifndef _GL_NETDB_H

#if __GNUC__ >= 3
#pragma GCC system_header
#endif


#if 1

/* The include_next requires a split double-inclusion guard.  */
# include_next <netdb.h>

#endif

#ifndef _GL_NETDB_H
#define _GL_NETDB_H

/* Get netdb.h definitions such as struct hostent for MinGW.  */
#include <sys/socket.h>

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
# elif __GNUC__ >= 3 && GNULIB_STRICT_CHECKING
/* Verify the existence of the function.  */
#  define _GL_WARN_ON_USE(function, message) \
extern __typeof__ (function) function
# else /* Unsupported.  */
#  define _GL_WARN_ON_USE(function, message) \
_GL_WARN_EXTERN_C int _gl_warn_on_use
# endif
#endif

/* _GL_WARN_ON_USE_CXX (function, rettype, parameters_and_attributes, "string")
   is like _GL_WARN_ON_USE (function, "string"), except that the function is
   declared with the given prototype, consisting of return type, parameters,
   and attributes.
   This variant is useful for overloaded functions in C++. _GL_WARN_ON_USE does
   not work in this case.  */
#ifndef _GL_WARN_ON_USE_CXX
# if 4 < __GNUC__ || (__GNUC__ == 4 && 3 <= __GNUC_MINOR__)
#  define _GL_WARN_ON_USE_CXX(function,rettype,parameters_and_attributes,msg) \
extern rettype function parameters_and_attributes \
     __attribute__ ((__warning__ (msg)))
# elif __GNUC__ >= 3 && GNULIB_STRICT_CHECKING
/* Verify the existence of the function.  */
#  define _GL_WARN_ON_USE_CXX(function,rettype,parameters_and_attributes,msg) \
extern rettype function parameters_and_attributes
# else /* Unsupported.  */
#  define _GL_WARN_ON_USE_CXX(function,rettype,parameters_and_attributes,msg) \
_GL_WARN_EXTERN_C int _gl_warn_on_use
# endif
#endif

/* _GL_WARN_EXTERN_C declaration;
   performs the declaration with C linkage.  */
#ifndef _GL_WARN_EXTERN_C
# if defined __cplusplus
#  define _GL_WARN_EXTERN_C extern "C"
# else
#  define _GL_WARN_EXTERN_C extern
# endif
#endif

/* Declarations for a platform that lacks <netdb.h>, or where it is
   incomplete.  */

#if 0

# if !1

#  if !GNULIB_defined_struct_addrinfo
/* Structure to contain information about address of a service provider.  */
struct addrinfo
{
  int ai_flags;                 /* Input flags.  */
  int ai_family;                /* Protocol family for socket.  */
  int ai_socktype;              /* Socket type.  */
  int ai_protocol;              /* Protocol for socket.  */
  socklen_t ai_addrlen;         /* Length of socket address.  */
  struct sockaddr *ai_addr;     /* Socket address for socket.  */
  char *ai_canonname;           /* Canonical name for service location.  */
  struct addrinfo *ai_next;     /* Pointer to next in list.  */
};
#   define GNULIB_defined_struct_addrinfo 1
#  endif
# endif

/* Possible values for `ai_flags' field in `addrinfo' structure.  */
# ifndef AI_PASSIVE
#  define AI_PASSIVE    0x0001  /* Socket address is intended for `bind'.  */
# endif
# ifndef AI_CANONNAME
#  define AI_CANONNAME  0x0002  /* Request for canonical name.  */
# endif
# ifndef AI_NUMERICSERV
#  define AI_NUMERICSERV        0x0400  /* Don't use name resolution.  */
# endif

# if 0
#  define AI_NUMERICHOST        0x0004  /* Don't use name resolution.  */
# endif

/* These symbolic constants are required to be present by POSIX, but
   our getaddrinfo replacement doesn't use them (yet).  Setting them
   to 0 on systems that doesn't have them avoids causing problems for
   system getaddrinfo implementations that would be confused by
   unknown values.  */
# ifndef AI_V4MAPPED
#  define AI_V4MAPPED    0 /* 0x0008: IPv4 mapped addresses are acceptable.  */
# endif
# ifndef AI_ALL
#  define AI_ALL         0 /* 0x0010: Return IPv4 mapped and IPv6 addresses. */
# endif
# ifndef AI_ADDRCONFIG
#  define AI_ADDRCONFIG  0 /* 0x0020: Use configuration of this host to choose
                                      returned address type.  */
# endif

/* Error values for `getaddrinfo' function.  */
# ifndef EAI_BADFLAGS
#  define EAI_BADFLAGS    -1    /* Invalid value for `ai_flags' field.  */
#  define EAI_NONAME      -2    /* NAME or SERVICE is unknown.  */
#  define EAI_AGAIN       -3    /* Temporary failure in name resolution.  */
#  define EAI_FAIL        -4    /* Non-recoverable failure in name res.  */
#  define EAI_NODATA      -5    /* No address associated with NAME.  */
#  define EAI_FAMILY      -6    /* `ai_family' not supported.  */
#  define EAI_SOCKTYPE    -7    /* `ai_socktype' not supported.  */
#  define EAI_SERVICE     -8    /* SERVICE not supported for `ai_socktype'.  */
#  define EAI_MEMORY      -10   /* Memory allocation failure.  */
# endif

/* Since EAI_NODATA is deprecated by RFC3493, some systems (at least
   FreeBSD, which does define EAI_BADFLAGS) have removed the definition
   in favor of EAI_NONAME.  */
# if !defined EAI_NODATA && defined EAI_NONAME
#  define EAI_NODATA EAI_NONAME
# endif

# ifndef EAI_OVERFLOW
/* Not defined on mingw32 and Haiku. */
#  define EAI_OVERFLOW    -12   /* Argument buffer overflow.  */
# endif
# ifndef EAI_ADDRFAMILY
/* Not defined on mingw32. */
#  define EAI_ADDRFAMILY  -9    /* Address family for NAME not supported.  */
# endif
# ifndef EAI_SYSTEM
/* Not defined on mingw32. */
#  define EAI_SYSTEM      -11   /* System error returned in `errno'.  */
# endif

# if 0
/* The commented out definitions below are not yet implemented in the
   GNULIB getaddrinfo() replacement, so are not yet needed.

   If they are restored, be sure to protect the definitions with #ifndef.  */
#  ifndef EAI_INPROGRESS
#   define EAI_INPROGRESS       -100    /* Processing request in progress.  */
#   define EAI_CANCELED         -101    /* Request canceled.  */
#   define EAI_NOTCANCELED      -102    /* Request not canceled.  */
#   define EAI_ALLDONE          -103    /* All requests done.  */
#   define EAI_INTR             -104    /* Interrupted by a signal.  */
#   define EAI_IDN_ENCODE       -105    /* IDN encoding failed.  */
#  endif
# endif

# if !1
/* Translate name of a service location and/or a service name to set of
   socket addresses.
   For more details, see the POSIX:2001 specification
   <http://www.opengroup.org/susv3xsh/getaddrinfo.html>.  */
extern int getaddrinfo (const char *restrict nodename,
                        const char *restrict servname,
                        const struct addrinfo *restrict hints,
                        struct addrinfo **restrict res)
     _GL_ARG_NONNULL ((4));
# endif

# if !1
/* Free `addrinfo' structure AI including associated storage.
   For more details, see the POSIX:2001 specification
   <http://www.opengroup.org/susv3xsh/getaddrinfo.html>.  */
extern void freeaddrinfo (struct addrinfo *ai) _GL_ARG_NONNULL ((1));
# endif

# if !1
/* Convert error return from getaddrinfo() to a string.
   For more details, see the POSIX:2001 specification
   <http://www.opengroup.org/susv3xsh/gai_strerror.html>.  */
extern const char *gai_strerror (int ecode);
# endif

# if !1
/* Convert socket address to printable node and service names.
   For more details, see the POSIX:2001 specification
   <http://www.opengroup.org/susv3xsh/getnameinfo.html>.  */
extern int getnameinfo (const struct sockaddr *restrict sa, socklen_t salen,
                        char *restrict node, socklen_t nodelen,
                        char *restrict service, socklen_t servicelen,
                        int flags)
     _GL_ARG_NONNULL ((1));
# endif

/* Possible flags for getnameinfo.  */
# ifndef NI_NUMERICHOST
#  define NI_NUMERICHOST 1
# endif
# ifndef NI_NUMERICSERV
#  define NI_NUMERICSERV 2
# endif

#elif defined GNULIB_POSIXCHECK

# undef getaddrinfo
# if HAVE_RAW_DECL_GETADDRINFO
_GL_WARN_ON_USE (getaddrinfo, "getaddrinfo is unportable - "
                 "use gnulib module getaddrinfo for portability");
# endif

# undef freeaddrinfo
# if HAVE_RAW_DECL_FREEADDRINFO
_GL_WARN_ON_USE (freeaddrinfo, "freeaddrinfo is unportable - "
                 "use gnulib module getaddrinfo for portability");
# endif

# undef gai_strerror
# if HAVE_RAW_DECL_GAI_STRERROR
_GL_WARN_ON_USE (gai_strerror, "gai_strerror is unportable - "
                 "use gnulib module getaddrinfo for portability");
# endif

# undef getnameinfo
# if HAVE_RAW_DECL_GETNAMEINFO
_GL_WARN_ON_USE (getnameinfo, "getnameinfo is unportable - "
                 "use gnulib module getaddrinfo for portability");
# endif

#endif

#endif /* _GL_NETDB_H */
#endif /* _GL_NETDB_H */
