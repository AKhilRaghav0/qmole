/* xmlrpc_config.h is generated from xmlrpc_config.h.in by 'configure'.

   This file just uses plain AC_SUBST substitution, the same as
   Makefile.config.  Wherever you see , that gets replaced by the
   value of 'configure' variable XXX.

   Logical macros are 0 or 1 instead of the more traditional defined and
   undefined.  That's so we can distinguish when compiling code between
   "false" and some problem with the code.
*/


/* We hope to replace xmlrpc_amconfig.h some day with something that 
   doesn't require a whole special set of software to build, to make
   xmlrpc-c approachable by dumber developers.
*/
#include "xmlrpc_amconfig.h"

#ifndef __xmlrpc_config_h__
#define __xmlrpc_config_h__


#define VA_LIST_IS_ARRAY 0

#define HAS_VA_COPY 1

#define HAVE_LIBWWW_SSL 

#define ATTR_UNUSED __attribute__((__unused__))

#define HAVE_UNICODE_WCHAR ON

#define DIRECTORY_SEPARATOR "/"

/* #undef HAVE_PTHREADS */

/*  Xmlrpc-c code uses __inline__ to declare functions that should
    be compiled as inline code.  GNU C recognizes the __inline__ keyword.
    Others recognize 'inline' or '__inline' or nothing at all to say
    a function should be inlined.

    We could make 'configure' simply do a trial compile to figure out
    which one, but for now, this approximation is easier:
*/
#ifdef _WIN32
# define __inline__ __inline
#else
# ifndef __GNUC__
#  ifndef __inline__
#   ifdef __sgi
#    define __inline__ __inline
#   else
#    define __inline__
#   endif
#  endif
# endif
#endif

/* A timeout in milliseconds. */
typedef unsigned long timeout_t;

#if !defined(WIN32) && defined(_WIN32)
#  define WIN32
#endif
#if defined(WIN32)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
  #if !defined (vsnprintf)
#define vsnprintf _vsnprintf
  #endif
  #if !defined (snprintf)
#define snprintf _snprintf
  #endif
#include <time.h>
#include <winsock2.h>
#include <direct.h>  /* for _chdir() */


/*
__inline BOOL setenv(const char* name, const char* value, int i) 
{
  return (SetEnvironmentVariable(name, value) != 0) ? TRUE : FALSE;
}
*/
#endif

#endif
