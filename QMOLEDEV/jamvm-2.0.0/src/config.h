/* src/config.h.  Generated from config.h.in by configure.  */
/* src/config.h.in.  Generated from configure.ac by autoheader.  */

/* GNU Classpath installation directory (prefix) */
#define CLASSPATH_INSTALL_DIR "/usr/local/classpath"

/* Define to one of `_getb67', `GETB67', `getb67' for Cray-2 and Cray-YMP
   systems. This function is required for `alloca.c' support on those systems.
   */
/* #undef CRAY_STACKSEG_END */

/* Define to 1 if using `alloca.c'. */
/* #undef C_ALLOCA */

/* interpreter direct */
#define DIRECT 1

/* Define to 1 if you have `alloca', as a function or macro. */
#define HAVE_ALLOCA 1

/* Define to 1 if you have <alloca.h> and it should be used (not on Ultrix).
   */
#define HAVE_ALLOCA_H 1

/* Define to 1 if you have the <dlfcn.h> header file. */
#define HAVE_DLFCN_H 1

/* Define to 1 if you have the <endian.h> header file. */
/* #undef HAVE_ENDIAN_H */

/* Define to 1 if you have the `getpagesize' function. */
#define HAVE_GETPAGESIZE 1

/* Define to 1 if you have the `gettimeofday' function. */
#define HAVE_GETTIMEOFDAY 1

/* Define to 1 if you have the <inttypes.h> header file. */
#define HAVE_INTTYPES_H 1

/* Define if your <locale.h> file defines LC_MESSAGES. */
#define HAVE_LC_MESSAGES 1

/* Define to 1 if you have the `dl' library (-ldl). */
/* #undef HAVE_LIBDL */

/* Define to 1 if you have the `ffi' library (-lffi). */
#define HAVE_LIBFFI 1

/* Define to 1 if you have the `m' library (-lm). */
#define HAVE_LIBM 1

/* Define to 1 if you have the `pthread' library (-lpthread). */
#define HAVE_LIBPTHREAD 1

/* Define to 1 if you have the `rt' library (-lrt). */
/* #undef HAVE_LIBRT */

/* Define to 1 if you have the `thr' library (-lthr). */
/* #undef HAVE_LIBTHR */

/* Define to 1 if you have the `z' library (-lz). */
#define HAVE_LIBZ 1

/* Define to 1 if you have the <locale.h> header file. */
#define HAVE_LOCALE_H 1

/* Define to 1 if you have the <memory.h> header file. */
#define HAVE_MEMORY_H 1

/* Define to 1 if you have a working `mmap' system call. */
/* #undef HAVE_MMAP */

/* JNI stubs support profiling */
/* #undef HAVE_PROFILE_STUBS */

/* Define to 1 if you have the `setlocale' function. */
#define HAVE_SETLOCALE 1

/* Define to 1 if you have the <stdint.h> header file. */
#define HAVE_STDINT_H 1

/* Define to 1 if you have the <stdlib.h> header file. */
#define HAVE_STDLIB_H 1

/* Define to 1 if you have the <strings.h> header file. */
#define HAVE_STRINGS_H 1

/* Define to 1 if you have the <string.h> header file. */
#define HAVE_STRING_H 1

/* Define to 1 if you have the `strtol' function. */
#define HAVE_STRTOL 1

/* Define to 1 if you have the <sys/param.h> header file. */
#define HAVE_SYS_PARAM_H 1

/* Define to 1 if you have the <sys/stat.h> header file. */
#define HAVE_SYS_STAT_H 1

/* Define to 1 if you have the <sys/time.h> header file. */
#define HAVE_SYS_TIME_H 1

/* Define to 1 if you have the <sys/types.h> header file. */
#define HAVE_SYS_TYPES_H 1

/* __thread supported by compiler */
/* #undef HAVE_TLS */

/* Define to 1 if you have the <unistd.h> header file. */
#define HAVE_UNISTD_H 1

/* interpreter inlining */
#define INLINING 1

/* Installation directory (prefix) */
#define INSTALL_DIR "/usr/local/jamvm"

/* VM support for JSR292 (invokedynamic) */
/* #undef JSR292 */

/* VM support for JSR308 (type annotations) */
/* #undef JSR308 */

/* VM support for JSR335 (lambda expressions) */
/* #undef JSR335 */

/* VM support for JSR901 (method parameter reflection) */
/* #undef JSR901 */

/* Define to the sub-directory in which libtool stores uninstalled libraries.
   */
#define LT_OBJDIR ".libs/"

/* OpenJDK class-library */
/* #undef OPENJDK_VERSION */

/* Name of package */
#define PACKAGE "jamvm"

/* Define to the address where bug reports for this package should be sent. */
#define PACKAGE_BUGREPORT ""

/* Define to the full name of this package. */
#define PACKAGE_NAME "jamvm"

/* Define to the full name and version of this package. */
#define PACKAGE_STRING "jamvm 2.0.0"

/* Define to the one symbol short name of this package. */
#define PACKAGE_TARNAME "jamvm"

/* Define to the home page for this package. */
#define PACKAGE_URL ""

/* Define to the version of this package. */
#define PACKAGE_VERSION "2.0.0"

/* interpreter uses prefetching */
/* #undef PREFETCH */

/* compute relocatability at runtime */
/* #undef RUNTIME_RELOC_CHECKS */

/* java.lang.String uses shared char buffers */
#define SHARED_CHAR_BUFFERS 1

/* If using the C implementation of alloca, define if you know the
   direction of stack growth for your system; otherwise it will be
   automatically deduced at runtime.
	STACK_DIRECTION > 0 => grows toward higher addresses
	STACK_DIRECTION < 0 => grows toward lower addresses
	STACK_DIRECTION = 0 => direction of growth unknown */
/* #undef STACK_DIRECTION */

/* Define to 1 if you have the ANSI C header files. */
#define STDC_HEADERS 1

/* interpreter threaded */
#define THREADED 1

/* Define to 1 if you can safely include both <sys/time.h> and <time.h>. */
#define TIME_WITH_SYS_TIME 1

/* defined if object allocation tracing enabled for debugging */
/* #undef TRACEALLOC */

/* defined if GC compaction phase tracing enabled for debugging */
/* #undef TRACECOMPACT */

/* defined if interpreter direct-mode tracing enabled for debugging */
/* #undef TRACEDIRECT */

/* defined if library loading/unloading tracing enabled for debugging */
/* #undef TRACEDLL */

/* defined if object finalisation tracing enabled for debugging */
/* #undef TRACEFNLZ */

/* defined if gc tracing enabled for debugging */
/* #undef TRACEGC */

/* defined if interpreter inlining tracing enabled for debugging */
/* #undef TRACEINLINING */

/* defined if object locking tracing enabled for debugging */
/* #undef TRACELOCK */

/* defined if thread creation tracing enabled for debugging */
/* #undef TRACETHREAD */

/* interpreter uses caching */
#define USE_CACHE 1

/* use FFI */
#define USE_FFI 1

/* use machine-dependent JNI stubs */
/* #undef USE_MD_STUBS */

/* use zip */
#define USE_ZIP 1

/* Version number of package */
#define VERSION "2.0.0"

/* major version number */
#define VERSION_MAJOR 2

/* micro version number */
#define VERSION_MICRO 0

/* minor version number */
#define VERSION_MINOR 0

/* Define to empty if `const' does not conform to ANSI C. */
/* #undef const */

/* Define to `unsigned int' if <sys/types.h> does not define. */
/* #undef size_t */
