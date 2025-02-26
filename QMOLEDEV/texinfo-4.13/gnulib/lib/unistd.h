/* DO NOT EDIT! GENERATED AUTOMATICALLY! */
/* Substitute for and wrapper around <unistd.h>.
   Copyright (C) 2004-2008 Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software Foundation,
   Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.  */

#ifndef _GL_UNISTD_H

/* The include_next requires a split double-inclusion guard.  */
#if 1
# include_next <unistd.h>
#endif

#ifndef _GL_UNISTD_H
#define _GL_UNISTD_H

/* mingw doesn't define the SEEK_* macros in <unistd.h>.  */
#if !(defined SEEK_CUR && defined SEEK_END && defined SEEK_SET)
# include <stdio.h>
#endif

/* mingw fails to declare _exit in <unistd.h>.  */
#include <stdlib.h>

/* The definition of GL_LINK_WARNING is copied here.  */


/* Declare overridden functions.  */

#ifdef __cplusplus
extern "C" {
#endif


#if 0
# if 0
#  ifndef REPLACE_CHOWN
#   define REPLACE_CHOWN 1
#  endif
#  if REPLACE_CHOWN
/* Change the owner of FILE to UID (if UID is not -1) and the group of FILE
   to GID (if GID is not -1).  Follow symbolic links.
   Return 0 if successful, otherwise -1 and errno set.
   See the POSIX:2001 specification
   <http://www.opengroup.org/susv3xsh/chown.html>.  */
#   define chown rpl_chown
extern int chown (const char *file, uid_t uid, gid_t gid);
#  endif
# endif
#elif defined GNULIB_POSIXCHECK
# undef chown
# define chown(f,u,g) \
    (GL_LINK_WARNING ("chown fails to follow symlinks on some systems and " \
                      "doesn't treat a uid or gid of -1 on some systems - " \
                      "use gnulib module chown for portability"), \
     chown (f, u, g))
#endif


#if 0
# if !1
/* Copy the file descriptor OLDFD into file descriptor NEWFD.  Do nothing if
   NEWFD = OLDFD, otherwise close NEWFD first if it is open.
   Return 0 if successful, otherwise -1 and errno set.
   See the POSIX:2001 specification
   <http://www.opengroup.org/susv3xsh/dup2.html>.  */
extern int dup2 (int oldfd, int newfd);
# endif
#elif defined GNULIB_POSIXCHECK
# undef dup2
# define dup2(o,n) \
    (GL_LINK_WARNING ("dup2 is unportable - " \
                      "use gnulib module dup2 for portability"), \
     dup2 (o, n))
#endif


#if 1
# if !0
/* Set of environment variables and values.  An array of strings of the form
   "VARIABLE=VALUE", terminated with a NULL.  */
#  if 0 && defined __APPLE__ && defined __MACH__
#   include <crt_externs.h>
#   define environ (*_NSGetEnviron ())
#  else
extern char **environ;
#  endif
# endif
#elif defined GNULIB_POSIXCHECK
# undef environ
# define environ \
    (GL_LINK_WARNING ("environ is unportable - " \
                      "use gnulib module environ for portability"), \
     environ)
#endif


#if 0
# if 0

/* Change the process' current working directory to the directory on which
   the given file descriptor is open.
   Return 0 if successful, otherwise -1 and errno set.
   See the POSIX:2001 specification
   <http://www.opengroup.org/susv3xsh/fchdir.html>.  */
extern int fchdir (int /*fd*/);

#  define close rpl_close
extern int close (int);
#  define dup rpl_dup
extern int dup (int);
#  define dup2 rpl_dup2
extern int dup2 (int, int);

# endif
#elif defined GNULIB_POSIXCHECK
# undef fchdir
# define fchdir(f) \
    (GL_LINK_WARNING ("fchdir is unportable - " \
                      "use gnulib module fchdir for portability"), \
     fchdir (f))
#endif


#if 0
# if !1
/* Change the size of the file to which FD is opened to become equal to LENGTH.
   Return 0 if successful, otherwise -1 and errno set.
   See the POSIX:2001 specification
   <http://www.opengroup.org/susv3xsh/ftruncate.html>.  */
extern int ftruncate (int fd, off_t length);
# endif
#elif defined GNULIB_POSIXCHECK
# undef ftruncate
# define ftruncate(f,l) \
    (GL_LINK_WARNING ("ftruncate is unportable - " \
                      "use gnulib module ftruncate for portability"), \
     ftruncate (f, l))
#endif


#if 0
/* Include the headers that might declare getcwd so that they will not
   cause confusion if included after this file.  */
# include <stdlib.h>
# if 0
/* Get the name of the current working directory, and put it in SIZE bytes
   of BUF.
   Return BUF if successful, or NULL if the directory couldn't be determined
   or SIZE was too small.
   See the POSIX:2001 specification
   <http://www.opengroup.org/susv3xsh/getcwd.html>.
   Additionally, the gnulib module 'getcwd' guarantees the following GNU
   extension: If BUF is NULL, an array is allocated with 'malloc'; the array
   is SIZE bytes long, unless SIZE == 0, in which case it is as big as
   necessary.  */
#  define getcwd rpl_getcwd
extern char * getcwd (char *buf, size_t size);
# endif
#elif defined GNULIB_POSIXCHECK
# undef getcwd
# define getcwd(b,s) \
    (GL_LINK_WARNING ("getcwd is unportable - " \
                      "use gnulib module getcwd for portability"), \
     getcwd (b, s))
#endif


#if 0
/* Copies the user's login name to NAME.
   The array pointed to by NAME has room for SIZE bytes.

   Returns 0 if successful.  Upon error, an error number is returned, or -1 in
   the case that the login name cannot be found but no specific error is
   provided (this case is hopefully rare but is left open by the POSIX spec).

   See <http://www.opengroup.org/susv3xsh/getlogin.html>.
 */
# if !1
#  include <stddef.h>
extern int getlogin_r (char *name, size_t size);
# endif
#elif defined GNULIB_POSIXCHECK
# undef getlogin_r
# define getlogin_r(n,s) \
    (GL_LINK_WARNING ("getlogin_r is unportable - " \
                      "use gnulib module getlogin_r for portability"), \
     getlogin_r (n, s))
#endif


#if 0
# if 0
#  define getpagesize rpl_getpagesize
extern int getpagesize (void);
# elif !1
/* This is for POSIX systems.  */
#  if !defined getpagesize && defined _SC_PAGESIZE
#   if ! (defined __VMS && __VMS_VER < 70000000)
#    define getpagesize() sysconf (_SC_PAGESIZE)
#   endif
#  endif
/* This is for older VMS.  */
#  if !defined getpagesize && defined __VMS
#   ifdef __ALPHA
#    define getpagesize() 8192
#   else
#    define getpagesize() 512
#   endif
#  endif
/* This is for BeOS.  */
#  if !defined getpagesize && 0
#   include <OS.h>
#   if defined B_PAGE_SIZE
#    define getpagesize() B_PAGE_SIZE
#   endif
#  endif
/* This is for AmigaOS4.0.  */
#  if !defined getpagesize && defined __amigaos4__
#   define getpagesize() 2048
#  endif
/* This is for older Unix systems.  */
#  if !defined getpagesize && 0
#   include <sys/param.h>
#   ifdef EXEC_PAGESIZE
#    define getpagesize() EXEC_PAGESIZE
#   else
#    ifdef NBPG
#     ifndef CLSIZE
#      define CLSIZE 1
#     endif
#     define getpagesize() (NBPG * CLSIZE)
#    else
#     ifdef NBPC
#      define getpagesize() NBPC
#     endif
#    endif
#   endif
#  endif
# endif
#elif defined GNULIB_POSIXCHECK
# undef getpagesize
# define getpagesize() \
    (GL_LINK_WARNING ("getpagesize is unportable - " \
                      "use gnulib module getpagesize for portability"), \
     getpagesize ())
#endif


#if 0
# if 0
/* Change the owner of FILE to UID (if UID is not -1) and the group of FILE
   to GID (if GID is not -1).  Do not follow symbolic links.
   Return 0 if successful, otherwise -1 and errno set.
   See the POSIX:2001 specification
   <http://www.opengroup.org/susv3xsh/lchown.html>.  */
#  define lchown rpl_lchown
extern int lchown (char const *file, uid_t owner, gid_t group);
# endif
#elif defined GNULIB_POSIXCHECK
# undef lchown
# define lchown(f,u,g) \
    (GL_LINK_WARNING ("lchown is unportable to pre-POSIX.1-2001 " \
                      "systems - use gnulib module lchown for portability"), \
     lchown (f, u, g))
#endif


#if 0
# if 0
/* Set the offset of FD relative to SEEK_SET, SEEK_CUR, or SEEK_END.
   Return the new offset if successful, otherwise -1 and errno set.
   See the POSIX:2001 specification
   <http://www.opengroup.org/susv3xsh/lseek.html>.  */
#  define lseek rpl_lseek
   extern off_t lseek (int fd, off_t offset, int whence);
# endif
#elif defined GNULIB_POSIXCHECK
# undef lseek
# define lseek(f,o,w) \
    (GL_LINK_WARNING ("lseek does not fail with ESPIPE on pipes on some " \
                      "systems - use gnulib module lseek for portability"), \
     lseek (f, o, w))
#endif


#if 0
/* Read the contents of the symbolic link FILE and place the first BUFSIZE
   bytes of it into BUF.  Return the number of bytes placed into BUF if
   successful, otherwise -1 and errno set.
   See the POSIX:2001 specification
   <http://www.opengroup.org/susv3xsh/readlink.html>.  */
# if !1
#  include <stddef.h>
extern int readlink (const char *file, char *buf, size_t bufsize);
# endif
#elif defined GNULIB_POSIXCHECK
# undef readlink
# define readlink(f,b,s) \
    (GL_LINK_WARNING ("readlink is unportable - " \
                      "use gnulib module readlink for portability"), \
     readlink (f, b, s))
#endif


#if 0
/* Pause the execution of the current thread for N seconds.
   Returns the number of seconds left to sleep.
   See the POSIX:2001 specification
   <http://www.opengroup.org/susv3xsh/sleep.html>.  */
# if !1
extern unsigned int sleep (unsigned int n);
# endif
#elif defined GNULIB_POSIXCHECK
# undef sleep
# define sleep(n) \
    (GL_LINK_WARNING ("sleep is unportable - " \
                      "use gnulib module sleep for portability"), \
     sleep (n))
#endif


#ifdef __cplusplus
}
#endif


#endif /* _GL_UNISTD_H */
#endif /* _GL_UNISTD_H */
