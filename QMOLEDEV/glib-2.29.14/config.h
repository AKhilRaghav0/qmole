/* config.h.  Generated from config.h.in by configure.  */
/* config.h.in.  Generated from configure.ac by autoheader.  */

/* Define if building universal (internal helper macro) */
/* #undef AC_APPLE_UNIVERSAL_BUILD */

/* define if asm blocks can use numeric local labels */
/* #undef ASM_NUMERIC_LABELS */

/* poll doesn't work on devices */
#define BROKEN_POLL 1

/* Define to one of `_getb67', `GETB67', `getb67' for Cray-2 and Cray-YMP
   systems. This function is required for `alloca.c' support on those systems.
   */
/* #undef CRAY_STACKSEG_END */

/* Define to 1 if using `alloca.c'. */
/* #undef C_ALLOCA */

/* Whether to disable memory pools */
/* #undef DISABLE_MEM_POOLS */

/* Whether to enable GC friendliness by default */
/* #undef ENABLE_GC_FRIENDLY_DEFAULT */

/* always defined to indicate that i18n is enabled */
#define ENABLE_NLS 1

/* include GRegex */
#define ENABLE_REGEX /**/

/* Define the gettext package to be used */
#define GETTEXT_PACKAGE "glib20"

/* Define to the GLIB binary age */
#define GLIB_BINARY_AGE 2914

/* Byte contents of gmutex */
#define GLIB_BYTE_CONTENTS_GMUTEX -89,-85,-86,50,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0

/* Define to the GLIB interface age */
#define GLIB_INTERFACE_AGE 0

/* Define the location where the catalogs will be installed */
#define GLIB_LOCALE_DIR "/usr/local/share/locale"

/* Define to the GLIB major version */
#define GLIB_MAJOR_VERSION 2

/* Define to the GLIB micro version */
#define GLIB_MICRO_VERSION 14

/* Define to the GLIB minor version */
#define GLIB_MINOR_VERSION 29

/* The size of gmutex, as computed by sizeof. */
#define GLIB_SIZEOF_GMUTEX 44

/* The size of system_thread, as computed by sizeof. */
#define GLIB_SIZEOF_SYSTEM_THREAD 4

/* alpha atomic implementation */
/* #undef G_ATOMIC_ALPHA */

/* arm atomic implementation */
#define G_ATOMIC_ARM 1

/* cris atomic implementation */
/* #undef G_ATOMIC_CRIS */

/* crisv32 atomic implementation */
/* #undef G_ATOMIC_CRISV32 */

/* i486 atomic implementation */
/* #undef G_ATOMIC_I486 */

/* ia64 atomic implementation */
/* #undef G_ATOMIC_IA64 */

/* powerpc atomic implementation */
/* #undef G_ATOMIC_POWERPC */

/* s390 atomic implementation */
/* #undef G_ATOMIC_S390 */

/* sparcv9 atomic implementation */
/* #undef G_ATOMIC_SPARCV9 */

/* x86_64 atomic implementation */
/* #undef G_ATOMIC_X86_64 */

/* Have inline keyword */
#define G_HAVE_INLINE 1

/* Have __inline keyword */
#define G_HAVE___INLINE 1

/* Have __inline__ keyword */
#define G_HAVE___INLINE__ 1

/* Source file containing theread implementation */
#define G_THREAD_SOURCE "gthread-posix.c"

/* A 'va_copy' style function */
#define G_VA_COPY va_copy

/* 'va_lists' cannot be copies as values */
/* #undef G_VA_COPY_AS_ARRAY */

/* Define to 1 if you have `alloca', as a function or macro. */
#define HAVE_ALLOCA 1

/* Define to 1 if you have <alloca.h> and it should be used (not on Ultrix).
   */
#define HAVE_ALLOCA_H 1

/* Define to 1 if you have the <arpa/nameser_compat.h> header file. */
#define HAVE_ARPA_NAMESER_COMPAT_H 1

/* Define to 1 if you have the `atexit' function. */
#define HAVE_ATEXIT 1

/* Define to 1 if you have the <attr/xattr.h> header file. */
/* #undef HAVE_ATTR_XATTR_H */

/* Define to 1 if you have the `bind_textdomain_codeset' function. */
#define HAVE_BIND_TEXTDOMAIN_CODESET 1

/* Define if you have a version of the snprintf function with semantics as
   specified by the ISO C99 standard. */
#define HAVE_C99_SNPRINTF 1

/* Define if you have a version of the vsnprintf function with semantics as
   specified by the ISO C99 standard. */
#define HAVE_C99_VSNPRINTF 1

/* define to 1 if Carbon is available */
/* #undef HAVE_CARBON */

/* Define to 1 if you have the `chown' function. */
#define HAVE_CHOWN 1

/* Define to 1 if you have the `clock_gettime' function. */
/* #undef HAVE_CLOCK_GETTIME */

/* Have nl_langinfo (CODESET) */
#define HAVE_CODESET 1

/* Define to 1 if you have the <crt_externs.h> header file. */
/* #undef HAVE_CRT_EXTERNS_H */

/* Define if dbus-1 is available */
#define HAVE_DBUS1 1

/* Define to 1 if you have the `dcgettext' function. */
#define HAVE_DCGETTEXT 1

/* Define to 1 if you have the <dirent.h> header file. */
#define HAVE_DIRENT_H 1

/* Define to 1 if you have the <dlfcn.h> header file. */
#define HAVE_DLFCN_H 1

/* Define to 1 if you don't have `vprintf' but do have `_doprnt.' */
/* #undef HAVE_DOPRNT */

/* define for working do while(0) macros */
/* #undef HAVE_DOWHILE_MACROS */

/* Define to 1 if using dtrace probes. */
/* #undef HAVE_DTRACE */

/* Define to 1 if you have the `endmntent' function. */
/* #undef HAVE_ENDMNTENT */

/* Define to 1 if you have the `endservent' function. */
#define HAVE_ENDSERVENT 1

/* we have the eventfd(2) system call */
/* #undef HAVE_EVENTFD */

/* Define if we have FAM */
/* #undef HAVE_FAM */

/* Define to 1 if you have the <fam.h> header file. */
/* #undef HAVE_FAM_H */

/* Define if we have FAMNoExists in fam */
/* #undef HAVE_FAM_NO_EXISTS */

/* Define to 1 if you have the `fchmod' function. */
#define HAVE_FCHMOD 1

/* Define to 1 if you have the `fchown' function. */
#define HAVE_FCHOWN 1

/* Define to 1 if you have the `fdwalk' function. */
/* #undef HAVE_FDWALK */

/* Define to 1 if you have the <float.h> header file. */
#define HAVE_FLOAT_H 1

/* Define to 1 if you have the <fstab.h> header file. */
#define HAVE_FSTAB_H 1

/* Define to 1 if you have the `fsync' function. */
#define HAVE_FSYNC 1

/* we have the futex(2) system call */
/* #undef HAVE_FUTEX */

/* Whether you have gcov */
/* #undef HAVE_GCOV */

/* Define to 1 if you have the `getcwd' function. */
#define HAVE_GETCWD 1

/* Define to 1 if you have the `getc_unlocked' function. */
#define HAVE_GETC_UNLOCKED 1

/* Define to 1 if you have the `getgrgid' function. */
#define HAVE_GETGRGID 1

/* Define to 1 if you have the `getmntent_r' function. */
/* #undef HAVE_GETMNTENT_R */

/* Define to 1 if you have the `getmntinfo' function. */
#define HAVE_GETMNTINFO 1

/* Define to 1 if you have the `getprotobyname_r' function. */
/* #undef HAVE_GETPROTOBYNAME_R */

/* Define to 1 if you have the `getpwuid' function. */
#define HAVE_GETPWUID 1

/* Define if the GNU gettext() function is already present or preinstalled. */
#define HAVE_GETTEXT 1

/* Define to 1 if you have the `gmtime_r' function. */
#define HAVE_GMTIME_R 1

/* define to use system printf */
#define HAVE_GOOD_PRINTF 1

/* Define to 1 if you have the <grp.h> header file. */
#define HAVE_GRP_H 1

/* Define to 1 if you have the `hasmntopt' function. */
/* #undef HAVE_HASMNTOPT */

/* Define to 1 if you have the `inotify_init1' function. */
/* #undef HAVE_INOTIFY_INIT1 */

/* define to support printing 64-bit integers with format I64 */
/* #undef HAVE_INT64_AND_I64 */

/* Define if you have the 'intmax_t' type in <stdint.h> or <inttypes.h>. */
#define HAVE_INTMAX_T 1

/* Define to 1 if you have the <inttypes.h> header file. */
#define HAVE_INTTYPES_H 1

/* Define if <inttypes.h> exists, doesn't clash with <sys/types.h>, and
   declares uintmax_t. */
#define HAVE_INTTYPES_H_WITH_UINTMAX 1

/* Define if you have <langinfo.h> and nl_langinfo(CODESET). */
#define HAVE_LANGINFO_CODESET 1

/* Define to 1 if you have the `lchmod' function. */
#define HAVE_LCHMOD 1

/* Define to 1 if you have the `lchown' function. */
#define HAVE_LCHOWN 1

/* Define if your <locale.h> file defines LC_MESSAGES. */
#define HAVE_LC_MESSAGES 1

/* Define to 1 if you have the <limits.h> header file. */
#define HAVE_LIMITS_H 1

/* Define to 1 if you have the `link' function. */
#define HAVE_LINK 1

/* Define to 1 if you have the <linux/magic.h> header file. */
/* #undef HAVE_LINUX_MAGIC_H */

/* Define to 1 if you have the <locale.h> header file. */
#define HAVE_LOCALE_H 1

/* Define to 1 if you have the `localtime_r' function. */
#define HAVE_LOCALTIME_R 1

/* Define if you have the 'long double' type. */
#define HAVE_LONG_DOUBLE 1

/* Define if you have the 'long long' type. */
#define HAVE_LONG_LONG 1

/* define if system printf can print long long */
#define HAVE_LONG_LONG_FORMAT 1

/* Define to 1 if you have the `lstat' function. */
#define HAVE_LSTAT 1

/* Define to 1 if you have the <malloc.h> header file. */
/* #undef HAVE_MALLOC_H */

/* Define to 1 if you have the `memalign' function. */
/* #undef HAVE_MEMALIGN */

/* Define to 1 if you have the `memmem' function. */
/* #undef HAVE_MEMMEM */

/* Define to 1 if you have the `memmove' function. */
#define HAVE_MEMMOVE 1

/* Define to 1 if you have the <memory.h> header file. */
/* #undef HAVE_MEMORY_H */

/* Define to 1 if you have the `mmap' function. */
#define HAVE_MMAP 1

/* Define to 1 if you have the <mntent.h> header file. */
/* #undef HAVE_MNTENT_H */

/* Have a monotonic clock */
/* #undef HAVE_MONOTONIC_CLOCK */

/* Define to 1 if you have the <netdb.h> header file. */
#define HAVE_NETDB_H 1

/* Have non-POSIX function getgrgid_r */
/* #undef HAVE_NONPOSIX_GETGRGID_R */

/* Have non-POSIX function getpwuid_r */
/* #undef HAVE_NONPOSIX_GETPWUID_R */

/* Define to 1 if you have the `on_exit' function. */
/* #undef HAVE_ON_EXIT */

/* Define to 1 if you have the `pipe2' function. */
/* #undef HAVE_PIPE2 */

/* Define to 1 if you have the `poll' function. */
#define HAVE_POLL 1

/* Have POSIX function getgrgid_r */
#define HAVE_POSIX_GETGRGID_R 1

/* Have POSIX function getpwuid_r */
#define HAVE_POSIX_GETPWUID_R 1

/* Define to 1 if you have the `posix_memalign' function. */
#define HAVE_POSIX_MEMALIGN 1

/* Have function pthread_attr_setstacksize */
#define HAVE_PTHREAD_ATTR_SETSTACKSIZE 1

/* Define to 1 if the system has the type `ptrdiff_t'. */
/* #undef HAVE_PTRDIFF_T */

/* Define to 1 if you have the <pwd.h> header file. */
#define HAVE_PWD_H 1

/* Define to 1 if you have the 'qsort_r' function */
/* #undef HAVE_QSORT_R */

/* Define to 1 if you have the `readlink' function. */
#define HAVE_READLINK 1

/* Define to 1 if you have the <sched.h> header file. */
#define HAVE_SCHED_H 1

/* Define to 1 if libselinux is available */
/* #undef HAVE_SELINUX */

/* Define to 1 if you have the <selinux/selinux.h> header file. */
/* #undef HAVE_SELINUX_SELINUX_H */

/* Define to 1 if you have the `setenv' function. */
#define HAVE_SETENV 1

/* Define to 1 if you have the `setlocale' function. */
#define HAVE_SETLOCALE 1

/* Define to 1 if you have the `setmntent' function. */
/* #undef HAVE_SETMNTENT */

/* Define to 1 if you have the `setresuid' function. */
/* #undef HAVE_SETRESUID */

/* Define to 1 if you have the `setreuid' function. */
#define HAVE_SETREUID 1

/* Define to 1 if you have the `snprintf' function. */
#define HAVE_SNPRINTF 1

/* Define to 1 if you have the `splice' function. */
/* #undef HAVE_SPLICE */

/* Define to 1 if you have the `statfs' function. */
/* #undef HAVE_STATFS */

/* Define to 1 if you have the `statvfs' function. */
#define HAVE_STATVFS 1

/* Define to 1 if you have the <stddef.h> header file. */
#define HAVE_STDDEF_H 1

/* Define to 1 if you have the <stdint.h> header file. */
#define HAVE_STDINT_H 1

/* Define if <stdint.h> exists, doesn't clash with <sys/types.h>, and declares
   uintmax_t. */
#define HAVE_STDINT_H_WITH_UINTMAX 1

/* Define to 1 if you have the <stdlib.h> header file. */
#define HAVE_STDLIB_H 1

/* Define to 1 if you have the `stpcpy' function. */
#define HAVE_STPCPY 1

/* Define to 1 if you have the `strcasecmp' function. */
#define HAVE_STRCASECMP 1

/* Define to 1 if you have the `strerror' function. */
#define HAVE_STRERROR 1

/* Define to 1 if you have the <strings.h> header file. */
/* #undef HAVE_STRINGS_H */

/* Define to 1 if you have the <string.h> header file. */
#define HAVE_STRING_H 1

/* Have functions strlcpy and strlcat */
#define HAVE_STRLCPY 1

/* Define to 1 if you have the `strncasecmp' function. */
#define HAVE_STRNCASECMP 1

/* Define to 1 if you have the `strndup' function. */
/* #undef HAVE_STRNDUP */

/* Define to 1 if you have the `strsignal' function. */
#define HAVE_STRSIGNAL 1

/* Define to 1 if `f_bavail' is a member of `struct statfs'. */
#define HAVE_STRUCT_STATFS_F_BAVAIL 1

/* Define to 1 if `f_fstypename' is a member of `struct statfs'. */
#define HAVE_STRUCT_STATFS_F_FSTYPENAME 1

/* Define to 1 if `f_basetype' is a member of `struct statvfs'. */
/* #undef HAVE_STRUCT_STATVFS_F_BASETYPE */

/* Define to 1 if `st_atimensec' is a member of `struct stat'. */
/* #undef HAVE_STRUCT_STAT_ST_ATIMENSEC */

/* Define to 1 if `st_atim.tv_nsec' is a member of `struct stat'. */
/* #undef HAVE_STRUCT_STAT_ST_ATIM_TV_NSEC */

/* Define to 1 if `st_blksize' is a member of `struct stat'. */
#define HAVE_STRUCT_STAT_ST_BLKSIZE 1

/* Define to 1 if `st_blocks' is a member of `struct stat'. */
#define HAVE_STRUCT_STAT_ST_BLOCKS 1

/* Define to 1 if `st_ctimensec' is a member of `struct stat'. */
/* #undef HAVE_STRUCT_STAT_ST_CTIMENSEC */

/* Define to 1 if `st_ctim.tv_nsec' is a member of `struct stat'. */
/* #undef HAVE_STRUCT_STAT_ST_CTIM_TV_NSEC */

/* Define to 1 if `st_mtimensec' is a member of `struct stat'. */
/* #undef HAVE_STRUCT_STAT_ST_MTIMENSEC */

/* Define to 1 if `st_mtim.tv_nsec' is a member of `struct stat'. */
/* #undef HAVE_STRUCT_STAT_ST_MTIM_TV_NSEC */

/* Define to 1 if `tm_gmtoff' is a member of `struct tm'. */
#define HAVE_STRUCT_TM_TM_GMTOFF 1

/* Define to 1 if `__tm_gmtoff' is a member of `struct tm'. */
/* #undef HAVE_STRUCT_TM___TM_GMTOFF */

/* Define to 1 if you have the `symlink' function. */
#define HAVE_SYMLINK 1

/* Define to 1 if you have the <sys/inotify.h> header file. */
/* #undef HAVE_SYS_INOTIFY_H */

/* Define to 1 if you have the <sys/mkdev.h> header file. */
/* #undef HAVE_SYS_MKDEV_H */

/* Define to 1 if you have the <sys/mntctl.h> header file. */
/* #undef HAVE_SYS_MNTCTL_H */

/* Define to 1 if you have the <sys/mnttab.h> header file. */
/* #undef HAVE_SYS_MNTTAB_H */

/* Define to 1 if you have the <sys/mount.h> header file. */
#define HAVE_SYS_MOUNT_H 1

/* Define to 1 if you have the <sys/param.h> header file. */
#define HAVE_SYS_PARAM_H 1

/* Define to 1 if you have the <sys/poll.h> header file. */
#define HAVE_SYS_POLL_H 1

/* Define to 1 if you have the <sys/prctl.h> header file. */
/* #undef HAVE_SYS_PRCTL_H */

/* Define to 1 if you have the <sys/resource.h> header file. */
#define HAVE_SYS_RESOURCE_H 1

/* found fd_set in sys/select.h */
#define HAVE_SYS_SELECT_H 1

/* Define to 1 if you have the <sys/statfs.h> header file. */
/* #undef HAVE_SYS_STATFS_H */

/* Define to 1 if you have the <sys/statvfs.h> header file. */
#define HAVE_SYS_STATVFS_H 1

/* Define to 1 if you have the <sys/stat.h> header file. */
/* #undef HAVE_SYS_STAT_H */

/* Define to 1 if you have the <sys/sysctl.h> header file. */
#define HAVE_SYS_SYSCTL_H 1

/* Define to 1 if you have the <sys/times.h> header file. */
#define HAVE_SYS_TIMES_H 1

/* Define to 1 if you have the <sys/time.h> header file. */
#define HAVE_SYS_TIME_H 1

/* Define to 1 if you have the <sys/types.h> header file. */
#define HAVE_SYS_TYPES_H 1

/* Define to 1 if you have the <sys/uio.h> header file. */
#define HAVE_SYS_UIO_H 1

/* Define to 1 if you have the <sys/vfstab.h> header file. */
/* #undef HAVE_SYS_VFSTAB_H */

/* Define to 1 if you have the <sys/vfs.h> header file. */
/* #undef HAVE_SYS_VFS_H */

/* Define to 1 if you have the <sys/vmount.h> header file. */
/* #undef HAVE_SYS_VMOUNT_H */

/* Define to 1 if you have the <sys/wait.h> header file. */
#define HAVE_SYS_WAIT_H 1

/* Define to 1 if you have the <sys/xattr.h> header file. */
#define HAVE_SYS_XATTR_H 1

/* Define to 1 if you have the `timegm' function. */
#define HAVE_TIMEGM 1

/* Define to 1 if you have the <unistd.h> header file. */
#define HAVE_UNISTD_H 1

/* Define if your printf function family supports positional parameters as
   specified by Unix98. */
#define HAVE_UNIX98_PRINTF 1

/* Define to 1 if you have the `unsetenv' function. */
#define HAVE_UNSETENV 1

/* Define to 1 if you have the `utimes' function. */
#define HAVE_UTIMES 1

/* Define to 1 if you have the `valloc' function. */
#define HAVE_VALLOC 1

/* Define to 1 if you have the <values.h> header file. */
/* #undef HAVE_VALUES_H */

/* Define to 1 if you have the `vasprintf' function. */
#define HAVE_VASPRINTF 1

/* Define to 1 if you have the `vprintf' function. */
#define HAVE_VPRINTF 1

/* Define to 1 if you have the `vsnprintf' function. */
#define HAVE_VSNPRINTF 1

/* Define if you have the 'wchar_t' type. */
#define HAVE_WCHAR_T 1

/* Define to 1 if you have the `wcslen' function. */
#define HAVE_WCSLEN 1

/* Define if you have the 'wint_t' type. */
#define HAVE_WINT_T 1

/* Have a working bcopy */
/* #undef HAVE_WORKING_BCOPY */

/* Define to 1 if you have the <wspiapi.h> header file. */
/* #undef HAVE_WSPIAPI_H */

/* Define to 1 if xattr is available */
#define HAVE_XATTR 1

/* Define to 1 if xattr API uses XATTR_NOFOLLOW */
#define HAVE_XATTR_NOFOLLOW 1

/* Define to 1 if you have the `_NSGetEnviron' function. */
#define HAVE__NSGETENVIRON 1

/* Define to the sub-directory in which libtool stores uninstalled libraries.
   */
#define LT_OBJDIR ".libs/"

/* Do we cache iconv descriptors */
#define NEED_ICONV_CACHE 1

/* didn't find fd_set */
/* #undef NO_FD_SET */

/* Define to 1 if your C compiler doesn't accept -c and -o together. */
/* #undef NO_MINUS_C_MINUS_O */

/* global 'sys_errlist' not found */
/* #undef NO_SYS_ERRLIST */

/* global 'sys_siglist' not found */
/* #undef NO_SYS_SIGLIST */

/* global 'sys_siglist' not declared */
/* #undef NO_SYS_SIGLIST_DECL */

/* Define to the address where bug reports for this package should be sent. */
#define PACKAGE_BUGREPORT "http://bugzilla.gnome.org/enter_bug.cgi?product=glib"

/* Define to the full name of this package. */
#define PACKAGE_NAME "glib"

/* Define to the full name and version of this package. */
#define PACKAGE_STRING "glib 2.29.14"

/* Define to the one symbol short name of this package. */
#define PACKAGE_TARNAME "glib"

/* Define to the home page for this package. */
#define PACKAGE_URL ""

/* Define to the version of this package. */
#define PACKAGE_VERSION "2.29.14"

/* Maximum POSIX RT priority */
#define POSIX_MAX_PRIORITY sched_get_priority_max(SCHED_OTHER)

/* define if posix_memalign() can allocate any size */
#define POSIX_MEMALIGN_WITH_COMPLIANT_ALLOCS 1

/* Minimum POSIX RT priority */
#define POSIX_MIN_PRIORITY sched_get_priority_min(SCHED_OTHER)

/* The POSIX RT yield function */
#define POSIX_YIELD_FUNC sched_yield()

/* whether realloc (NULL,) works */
#define REALLOC_0_WORKS 1

/* Define if you have correct malloc prototypes */
#define SANE_MALLOC_PROTOS 1

/* The size of `char', as computed by sizeof. */
#define SIZEOF_CHAR 1

/* The size of `int', as computed by sizeof. */
#define SIZEOF_INT 4

/* The size of `long', as computed by sizeof. */
#define SIZEOF_LONG 4

/* The size of `long long', as computed by sizeof. */
#define SIZEOF_LONG_LONG 8

/* The size of `short', as computed by sizeof. */
#define SIZEOF_SHORT 2

/* The size of `size_t', as computed by sizeof. */
#define SIZEOF_SIZE_T 4

/* The size of `void *', as computed by sizeof. */
#define SIZEOF_VOID_P 4

/* The size of `__int64', as computed by sizeof. */
#define SIZEOF___INT64 0

/* If using the C implementation of alloca, define if you know the
   direction of stack growth for your system; otherwise it will be
   automatically deduced at runtime.
	STACK_DIRECTION > 0 => grows toward higher addresses
	STACK_DIRECTION < 0 => grows toward lower addresses
	STACK_DIRECTION = 0 => direction of growth unknown */
/* #undef STACK_DIRECTION */

/* Number of arguments to statfs() */
/* #undef STATFS_ARGS */

/* Define to 1 if you have the ANSI C header files. */
#define STDC_HEADERS 1

/* Using GNU libiconv */
#define USE_LIBICONV_GNU 1

/* Using a native implementation of iconv in a separate library */
/* #undef USE_LIBICONV_NATIVE */

/* Define to use statfs() */
/* #undef USE_STATFS */

/* Define to use statvfs() */
#define USE_STATVFS 1

/* using the system-supplied PCRE library */
/* #undef USE_SYSTEM_PCRE */

/* Define WORDS_BIGENDIAN to 1 if your processor stores words with the most
   significant byte first (like Motorola and SPARC, unlike Intel). */
#if defined AC_APPLE_UNIVERSAL_BUILD
# if defined __BIG_ENDIAN__
#  define WORDS_BIGENDIAN 1
# endif
#else
# ifndef WORDS_BIGENDIAN
/* #  undef WORDS_BIGENDIAN */
# endif
#endif

/* Number of bits in a file offset, on hosts where this is settable. */
/* #undef _FILE_OFFSET_BITS */

/* Make all glibc extensions visible */
/* #undef _GNU_SOURCE */

/* Define for large files, on AIX-style hosts. */
/* #undef _LARGE_FILES */

/* Needed to get declarations for msg_control and msg_controllen on Solaris */
/* #undef _XOPEN_SOURCE */

/* Needed to get declarations for msg_control and msg_controllen on Solaris */
/* #undef _XOPEN_SOURCE_EXTENDED */

/* Needed to get declarations for msg_control and msg_controllen on Solaris */
/* #undef __EXTENSIONS__ */

/* Define to empty if `const' does not conform to ANSI C. */
/* #undef const */

/* Define to long or long long if <inttypes.h> and <stdint.h> don't define. */
/* #undef intmax_t */

/* Define to empty if the C compiler doesn't support this keyword. */
/* #undef signed */

/* Define to `unsigned int' if <sys/types.h> does not define. */
/* #undef size_t */
