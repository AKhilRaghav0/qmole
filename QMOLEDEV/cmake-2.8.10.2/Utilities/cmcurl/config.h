/* lib/config.h.in.  Generated from configure.ac by autoheader.  */

/* when building libcurl itself */
/* #undef BUILDING_LIBCURL */

/* to disable cookies support */
/* #undef CURL_DISABLE_COOKIES */

/* to disable cryptographic authentication */
/* #undef CURL_DISABLE_CRYPTO_AUTH */

/* to disable DICT */
/* #undef CURL_DISABLE_DICT */

/* to disable FILE */
/* #undef CURL_DISABLE_FILE */

/* to disable FTP */
/* #undef CURL_DISABLE_FTP */

/* to disable HTTP */
/* #undef CURL_DISABLE_HTTP */

/* to disable LDAP */
#define CURL_DISABLE_LDAP 1

/* to disable TELNET */
/* #undef CURL_DISABLE_TELNET */

/* to disable TFTP */
/* #undef CURL_DISABLE_TFTP */

/* to disable verbose strings */
/* #undef CURL_DISABLE_VERBOSE_STRINGS */

/* to make a symbol visible */
/* #undef CURL_EXTERN_SYMBOL */

/* to enable hidden symbols */
/* #undef CURL_HIDDEN_SYMBOLS */

/* when not building a shared library */
/* #undef CURL_STATICLIB */

/* Set to explicitly specify we don't want to use thread-safe functions */
/* #undef DISABLED_THREADSAFE */

/* lber dynamic library file */
/* #undef DL_LBER_FILE */

/* ldap dynamic library file */
/* #undef DL_LDAP_FILE */

/* your Entropy Gathering Daemon socket pathname */
/* #undef EGD_SOCKET */

/* Define if you want to enable IPv6 support */
/* #undef ENABLE_IPV6 */

/* Define to the type qualifier of arg 1 for getnameinfo. */
/* #undef GETNAMEINFO_QUAL_ARG1 */

/* Define to the type of arg 1 for getnameinfo. */
/* #undef GETNAMEINFO_TYPE_ARG1 */

/* Define to the type of arg 2 for getnameinfo. */
/* #undef GETNAMEINFO_TYPE_ARG2 */

/* Define to the type of args 4 and 6 for getnameinfo. */
/* #undef GETNAMEINFO_TYPE_ARG46 */

/* Define to the type of arg 7 for getnameinfo. */
/* #undef GETNAMEINFO_TYPE_ARG7 */

/* Define to 1 if you have the <alloca.h> header file. */
#define HAVE_ALLOCA_H 1

/* Define to 1 if you have the <arpa/inet.h> header file. */
#define HAVE_ARPA_INET_H 1

/* Define to 1 if you have the <arpa/tftp.h> header file. */
/* #undef HAVE_ARPA_TFTP_H */

/* Define to 1 if you have the <assert.h> header file. */
#define HAVE_ASSERT_H 1

/* Define to 1 if you have the `basename' function. */
#define HAVE_BASENAME 1

/* Define to 1 if you have the `closesocket' function. */
/* #undef HAVE_CLOSESOCKET */

/* Define to 1 if you have the `CRYPTO_cleanup_all_ex_data' function. */
/* #undef HAVE_CRYPTO_CLEANUP_ALL_EX_DATA */

/* Define to 1 if you have the <crypto.h> header file. */
/* #undef HAVE_CRYPTO_H */

/* Define to 1 if you have the <des.h> header file. */
/* #undef HAVE_DES_H */

/* disabled non-blocking sockets */
/* #undef HAVE_DISABLED_NONBLOCKING */

/* Define to 1 if you have the <dlfcn.h> header file. */
#define HAVE_DLFCN_H 1

/* Define to 1 if you have the `dlopen' function. */
#define HAVE_DLOPEN 1

/* Define to 1 if you have the `ENGINE_load_builtin_engines' function. */
/* #undef HAVE_ENGINE_LOAD_BUILTIN_ENGINES */

/* Define to 1 if you have the <errno.h> header file. */
#define HAVE_ERRNO_H 1

/* Define to 1 if you have the <err.h> header file. */
/* #undef HAVE_ERR_H */

/* Define to 1 if you have the <fcntl.h> header file. */
#define HAVE_FCNTL_H 1

/* use FIONBIO for non-blocking sockets */
/* #undef HAVE_FIONBIO */

/* Define to 1 if you have the `fork' function. */
#define HAVE_FORK 1

/* Define to 1 if you have the `ftruncate' function. */
#define HAVE_FTRUNCATE 1

/* Define if getaddrinfo exists and works */
#define HAVE_GETADDRINFO 1

/* Define to 1 if you have the `geteuid' function. */
#define HAVE_GETEUID 1

/* Define to 1 if you have the `gethostbyaddr' function. */
#define HAVE_GETHOSTBYADDR 1

/* If you have gethostbyname */
#define HAVE_GETHOSTBYNAME 1

/* Define to 1 if you have the `gethostbyname_r' function. */
/* #undef HAVE_GETHOSTBYNAME_R */

/* gethostbyname_r() takes 3 args */
/* #undef HAVE_GETHOSTBYNAME_R_3 */

/* gethostbyname_r() takes 5 args */
/* #undef HAVE_GETHOSTBYNAME_R_5 */

/* gethostbyname_r() takes 6 args */
/* #undef HAVE_GETHOSTBYNAME_R_6 */

/* Define to 1 if you have the getnameinfo function. */
/* #undef HAVE_GETNAMEINFO */

/* Define to 1 if you have the `getpass_r' function. */
/* #undef HAVE_GETPASS_R */

/* Define to 1 if you have the `getprotobyname' function. */
#define HAVE_GETPROTOBYNAME 1

/* Define to 1 if you have the `getpwuid' function. */
#define HAVE_GETPWUID 1

/* Define to 1 if you have the `getrlimit' function. */
#define HAVE_GETRLIMIT 1

/* Define to 1 if you have the `gettimeofday' function. */
#define HAVE_GETTIMEOFDAY 1

/* Define to 1 if you have the `gmtime_r' function. */
#define HAVE_GMTIME_R 1

/* if you have the gssapi libraries */
/* #undef HAVE_GSSAPI */

/* if you have the GNU gssapi libraries */
/* #undef HAVE_GSSGNU */

/* if you have the Heimdal gssapi libraries */
/* #undef HAVE_GSSHEIMDAL */

/* if you have the MIT gssapi libraries */
/* #undef HAVE_GSSMIT */

/* Define to 1 if you have the `idna_strerror' function. */
/* #undef HAVE_IDNA_STRERROR */

/* Define to 1 if you have the `idn_free' function. */
/* #undef HAVE_IDN_FREE */

/* Define to 1 if you have the <idn-free.h> header file. */
/* #undef HAVE_IDN_FREE_H */

/* Define to 1 if you have the `inet_addr' function. */
#define HAVE_INET_ADDR 1

/* Define to 1 if you have the `inet_ntoa' function. */
#define HAVE_INET_NTOA 1

/* Define to 1 if you have the `inet_ntoa_r' function. */
/* #undef HAVE_INET_NTOA_R */

/* inet_ntoa_r() is declared */
/* #undef HAVE_INET_NTOA_R_DECL */

/* Define to 1 if you have the `inet_pton' function. */
#define HAVE_INET_PTON 1

/* Define to 1 if you have the <inttypes.h> header file. */
#define HAVE_INTTYPES_H 1

/* use ioctlsocket() for non-blocking sockets */
/* #undef HAVE_IOCTLSOCKET */

/* use Ioctlsocket() for non-blocking sockets */
/* #undef HAVE_IOCTLSOCKET_CASE */

/* Define to 1 if you have the <io.h> header file. */
/* #undef HAVE_IO_H */

/* if you have the Kerberos4 libraries (including -ldes) */
/* #undef HAVE_KRB4 */

/* Define to 1 if you have the `krb_get_our_ip_for_realm' function. */
/* #undef HAVE_KRB_GET_OUR_IP_FOR_REALM */

/* Define to 1 if you have the <krb.h> header file. */
/* #undef HAVE_KRB_H */

/* Define to 1 if you have the `dl' library (-ldl). */
/* #undef HAVE_LIBDL */

/* Define to 1 if you have the <libgen.h> header file. */
#define HAVE_LIBGEN_H 1

/* Define to 1 if you have the `idn' library (-lidn). */
/* #undef HAVE_LIBIDN */

/* Define to 1 if you have the `resolv' library (-lresolv). */
/* #undef HAVE_LIBRESOLV */

/* Define to 1 if you have the `resolve' library (-lresolve). */
/* #undef HAVE_LIBRESOLVE */

/* Define to 1 if you have the `socket' library (-lsocket). */
/* #undef HAVE_LIBSOCKET */

/* Define to 1 if you have the `ssh2' library (-lssh2). */
/* #undef HAVE_LIBSSH2 */

/* Define to 1 if you have the <libssh2.h> header file. */
/* #undef HAVE_LIBSSH2_H */

/* if zlib is available */
/* #undef HAVE_LIBZ */

/* Define to 1 if you have the <limits.h> header file. */
#define HAVE_LIMITS_H 1

/* if your compiler supports LL */
#define HAVE_LL 1

/* Define to 1 if you have the <locale.h> header file. */
#define HAVE_LOCALE_H 1

/* Define to 1 if you have the `localtime_r' function. */
#define HAVE_LOCALTIME_R 1

/* if your compiler supports long long */
#define HAVE_LONGLONG 1

/* Define to 1 if you have the malloc.h header file. */
/* #undef HAVE_MALLOC_H */

/* Define to 1 if you have the <memory.h> header file. */
#define HAVE_MEMORY_H 1

/* Define to 1 if you have the MSG_NOSIGNAL flag. */
/* #undef HAVE_MSG_NOSIGNAL */

/* Define to 1 if you have the <netdb.h> header file. */
#define HAVE_NETDB_H 1

/* Define to 1 if you have the <netinet/in.h> header file. */
#define HAVE_NETINET_IN_H 1

/* Define to 1 if you have the <netinet/tcp.h> header file. */
#define HAVE_NETINET_TCP_H 1

/* Define to 1 if you have the <net/if.h> header file. */
#define HAVE_NET_IF_H 1

/* Define to 1 if NI_WITHSCOPEID exists and works. */
/* #undef HAVE_NI_WITHSCOPEID */

/* Defined if no inet_pton() prototype available */
/* #undef HAVE_NO_INET_PTON_PROTO */

/* we have no strerror_r() proto */
/* #undef HAVE_NO_STRERROR_R_DECL */

/* Define to 1 if you have the <openssl/crypto.h> header file. */
/* #undef HAVE_OPENSSL_CRYPTO_H */

/* Define to 1 if you have the <openssl/engine.h> header file. */
/* #undef HAVE_OPENSSL_ENGINE_H */

/* Define to 1 if you have the <openssl/err.h> header file. */
/* #undef HAVE_OPENSSL_ERR_H */

/* Define to 1 if you have the <openssl/pem.h> header file. */
/* #undef HAVE_OPENSSL_PEM_H */

/* Define to 1 if you have the <openssl/pkcs12.h> header file. */
/* #undef HAVE_OPENSSL_PKCS12_H */

/* Define to 1 if you have the <openssl/rsa.h> header file. */
/* #undef HAVE_OPENSSL_RSA_H */

/* Define to 1 if you have the <openssl/ssl.h> header file. */
/* #undef HAVE_OPENSSL_SSL_H */

/* Define to 1 if you have the <openssl/x509.h> header file. */
/* #undef HAVE_OPENSSL_X509_H */

/* use O_NONBLOCK for non-blocking sockets */
#define HAVE_O_NONBLOCK 1

/* Define to 1 if you have the <pem.h> header file. */
/* #undef HAVE_PEM_H */

/* Define to 1 if you have the `perror' function. */
#define HAVE_PERROR 1

/* Define to 1 if you have the `pipe' function. */
#define HAVE_PIPE 1

/* Define to 1 if you have the `poll' function. */
#define HAVE_POLL 1

/* Define to 1 if you have the <process.h> header file. */
/* #undef HAVE_PROCESS_H */

/* Define to 1 if you have the <pwd.h> header file. */
#define HAVE_PWD_H 1

/* Define to 1 if you have the `RAND_egd' function. */
/* #undef HAVE_RAND_EGD */

/* Define to 1 if you have the `RAND_screen' function. */
/* #undef HAVE_RAND_SCREEN */

/* Define to 1 if you have the `RAND_status' function. */
/* #undef HAVE_RAND_STATUS */

/* Define to 1 if you have the recv function. */
#define HAVE_RECV 1

/* Define to 1 if you have the <rsa.h> header file. */
/* #undef HAVE_RSA_H */

/* Define to 1 if you have the select function. */
#define HAVE_SELECT 1

/* Define to 1 if you have the send function. */
#define HAVE_SEND 1

/* Define to 1 if you have the <setjmp.h> header file. */
#define HAVE_SETJMP_H 1

/* Define to 1 if you have the `setlocale' function. */
#define HAVE_SETLOCALE 1

/* Define to 1 if you have the `setrlimit' function. */
#define HAVE_SETRLIMIT 1

/* Define to 1 if you have the <sgtty.h> header file. */
/* #undef HAVE_SGTTY_H */

/* Define to 1 if you have the `sigaction' function. */
#define HAVE_SIGACTION 1

/* Define to 1 if you have the `siginterrupt' function. */
#define HAVE_SIGINTERRUPT 1

/* Define to 1 if you have the `signal' function. */
#define HAVE_SIGNAL 1

/* Define to 1 if you have the <signal.h> header file. */
#define HAVE_SIGNAL_H 1

/* If you have sigsetjmp */
#define HAVE_SIGSETJMP 1

/* Define to 1 if sig_atomic_t is an available typedef. */
#define HAVE_SIG_ATOMIC_T 1

/* Define to 1 if sig_atomic_t is already defined as volatile. */
/* #undef HAVE_SIG_ATOMIC_T_VOLATILE */

/* Define to 1 if you have the `socket' function. */
#define HAVE_SOCKET 1

/* use SO_NONBLOCK for non-blocking sockets */
/* #undef HAVE_SO_NONBLOCK */

/* Define this if you have the SPNEGO library fbopenssl */
/* #undef HAVE_SPNEGO */

/* Define to 1 if you have the <ssl.h> header file. */
/* #undef HAVE_SSL_H */

/* Define to 1 if you have the <stdint.h> header file. */
#define HAVE_STDINT_H 1

/* Define to 1 if you have the <stdio.h> header file. */
#define HAVE_STDIO_H 1

/* Define to 1 if you have the <stdlib.h> header file. */
#define HAVE_STDLIB_H 1

/* Define to 1 if you have the `strcasecmp' function. */
#define HAVE_STRCASECMP 1

/* Define to 1 if you have the `strcmpi' function. */
/* #undef HAVE_STRCMPI */

/* Define to 1 if you have the `strdup' function. */
#define HAVE_STRDUP 1

/* Define to 1 if you have the `stricmp' function. */
/* #undef HAVE_STRICMP */

/* Define to 1 if you have the <strings.h> header file. */
#define HAVE_STRINGS_H 1

/* Define to 1 if you have the <string.h> header file. */
#define HAVE_STRING_H 1

/* Define to 1 if you have the `strlcat' function. */
#define HAVE_STRLCAT 1

/* Define to 1 if you have the `strlcpy' function. */
/* #undef HAVE_STRLCPY */

/* Define to 1 if you have the `strstr' function. */
#define HAVE_STRSTR 1

/* Define to 1 if you have the `strtok_r' function. */
#define HAVE_STRTOK_R 1

/* Define to 1 if you have the `strtoll' function. */
#define HAVE_STRTOLL 1

/* if struct sockaddr_storage is defined */
#define HAVE_STRUCT_SOCKADDR_STORAGE 1

/* Define to 1 if you have the timeval struct. */
#define HAVE_STRUCT_TIMEVAL 1

/* Define to 1 if you have the <sys/filio.h> header file. */
#define HAVE_SYS_FILIO_H 1

/* Define to 1 if you have the <sys/ioctl.h> header file. */
#define HAVE_SYS_IOCTL_H 1

/* Define to 1 if you have the <sys/param.h> header file. */
#define HAVE_SYS_PARAM_H 1

/* Define to 1 if you have the <sys/poll.h> header file. */
#define HAVE_SYS_POLL_H 1

/* Define to 1 if you have the <sys/resource.h> header file. */
#define HAVE_SYS_RESOURCE_H 1

/* Define to 1 if you have the <sys/select.h> header file. */
#define HAVE_SYS_SELECT_H 1

/* Define to 1 if you have the <sys/socket.h> header file. */
#define HAVE_SYS_SOCKET_H 1

/* Define to 1 if you have the <sys/sockio.h> header file. */
#define HAVE_SYS_SOCKIO_H 1

/* Define to 1 if you have the <sys/stat.h> header file. */
#define HAVE_SYS_STAT_H 1

/* Define to 1 if you have the <sys/time.h> header file. */
#define HAVE_SYS_TIME_H 1

/* Define to 1 if you have the <sys/types.h> header file. */
#define HAVE_SYS_TYPES_H 1

/* Define to 1 if you have the <sys/utime.h> header file. */
/* #undef HAVE_SYS_UTIME_H */

/* Define to 1 if you have the <termios.h> header file. */
#define HAVE_TERMIOS_H 1

/* Define to 1 if you have the <termio.h> header file. */
/* #undef HAVE_TERMIO_H */

/* Define to 1 if you have the <time.h> header file. */
#define HAVE_TIME_H 1

/* Define to 1 if you have the <tld.h> header file. */
/* #undef HAVE_TLD_H */

/* Define to 1 if you have the `tld_strerror' function. */
/* #undef HAVE_TLD_STRERROR */

/* Define to 1 if you have the `uname' function. */
#define HAVE_UNAME 1

/* Define to 1 if you have the <unistd.h> header file. */
#define HAVE_UNISTD_H 1

/* Define to 1 if you have the `utime' function. */
#define HAVE_UTIME 1

/* Define to 1 if you have the <utime.h> header file. */
#define HAVE_UTIME_H 1

/* Define to 1 if you have the windows.h header file. */
/* #undef HAVE_WINDOWS_H */

/* Define to 1 if you have the winsock2.h header file. */
/* #undef HAVE_WINSOCK2_H */

/* Define to 1 if you have the winsock.h header file. */
/* #undef HAVE_WINSOCK_H */

/* Define this symbol if your OS supports changing the contents of argv */
/* #undef HAVE_WRITABLE_ARGV */

/* Define to 1 if you have the ws2tcpip.h header file. */
/* #undef HAVE_WS2TCPIP_H */

/* Define to 1 if you have the <x509.h> header file. */
/* #undef HAVE_X509_H */

/* if you have the zlib.h header file */
/* #undef HAVE_ZLIB_H */

/* If you lack a fine basename() prototype */
/* #undef NEED_BASENAME_PROTO */

/* Define to 1 if you need the malloc.h header file even with stdlib.h */
/* #undef NEED_MALLOC_H */

/* need REENTRANT defined */
/* #undef NEED_REENTRANT */

/* cpu-machine-OS */
#define OS "Darwin"

/* Name of package */
#define PACKAGE "curl"

/* Define to the address where bug reports for this package should be sent. */
#define PACKAGE_BUGREPORT "a suitable curl mailing list => http://curl.haxx.se/mail/"

/* Define to the full name of this package. */
#define PACKAGE_NAME "curl"

/* Define to the full name and version of this package. */
#define PACKAGE_STRING "curl-"

/* Define to the one symbol short name of this package. */
#define PACKAGE_TARNAME "curl"

/* Define to the version of this package. */
#define PACKAGE_VERSION "-"

/* a suitable file to read random data from */
#define RANDOM_FILE "/dev/urandom"

/* Define to the type of arg 1 for recv. */
#define RECV_TYPE_ARG1 int

/* Define to the type of arg 2 for recv. */
#define RECV_TYPE_ARG2 void *

/* Define to the type of arg 3 for recv. */
#define RECV_TYPE_ARG3 size_t

/* Define to the type of arg 4 for recv. */
#define RECV_TYPE_ARG4 int

/* Define to the function return type for recv. */
#define RECV_TYPE_RETV ssize_t

/* Define as the return type of signal handlers (`int' or `void'). */
#define RETSIGTYPE void

/* Define to the type of arg 1 for `select'. */
/* #undef SELECT_TYPE_ARG1 */

/* Define to the type of args 2, 3 and 4 for `select'. */
/* #undef SELECT_TYPE_ARG234 */

/* Define to the type of arg 5 for `select'. */
/* #undef SELECT_TYPE_ARG5 */

/* Define to the type qualifier of arg 2 for send. */
#define SEND_QUAL_ARG2 const

/* Define to the type of arg 1 for send. */
#define SEND_TYPE_ARG1 int

/* Define to the type of arg 2 for send. */
#define SEND_TYPE_ARG2 void *

/* Define to the type of arg 3 for send. */
#define SEND_TYPE_ARG3 size_t

/* Define to the type of arg 4 for send. */
#define SEND_TYPE_ARG4 int

/* Define to the function return type for send. */
#define SEND_TYPE_RETV ssize_t

/* The size of `curl_off_t', as computed by sizeof. */
#define SIZEOF_CURL_OFF_T 8

/* The size of `long', as computed by sizeof. */
#define SIZEOF_LONG 4

/* The size of `long long', as computed by sizeof. */
#define SIZEOF_LONG_LONG 8

/* The size of `__int64', as computed by sizeof. */
/* #undef SIZEOF___INT64 */

/* The size of `size_t', as computed by sizeof. */
#define SIZEOF_SIZE_T 4

/* The size of `ssize_t', as computed by sizeof. */
#define SIZEOF_SSIZE_T 4

/* The size of `time_t', as computed by sizeof. */
#define SIZEOF_TIME_T 4

/* Define to 1 if you have the ANSI C header files. */
#define STDC_HEADERS 1

/* Define to 1 if you can safely include both <sys/time.h> and <time.h>. */
#define TIME_WITH_SYS_TIME 1

/* Define if you want to enable ares support */
/* #undef USE_ARES */

/* if GnuTLS is enabled */
/* #undef USE_GNUTLS */

/* if libSSH2 is in use */
/* #undef USE_LIBSSH2 */

/* If you want to build curl with the built-in manual */
/* #undef USE_MANUAL */

/* if OpenSSL is in use */
/* #undef USE_OPENSSL */

/* if SSL is enabled */
/* #undef USE_SSLEAY */

/* to enable SSPI support */
/* #undef USE_WINDOWS_SSPI */

/* Version number of package */
#define VERSION "7.16.1"

/* Define to avoid automatic inclusion of winsock.h */
/* #undef WIN32_LEAN_AND_MEAN */

/* Define to 1 if on AIX 3.
   System headers sometimes define this.
   We just want to avoid a redefinition error message.  */
#ifndef _ALL_SOURCE
/* #undef _ALL_SOURCE */
#endif

/* Number of bits in a file offset, on hosts where this is settable. */
#define _FILE_OFFSET_BITS 64

/* Define for large files, on AIX-style hosts. */
/* #undef _LARGE_FILES */

/* define this if you need it to compile thread-safe code */
/* #undef _THREAD_SAFE */

/* Define to empty if `const' does not conform to ANSI C. */
/* #undef const */

/* type to use in place of in_addr_t if not defined */
/* #undef in_addr_t */

/* Define to `unsigned int' if <sys/types.h> does not define. */
/* #undef size_t */

/* type to use in place of socklen_t if not defined */
/* #undef socklen_t */

/* the signed version of size_t */
#ifndef SIZEOF_SSIZE_T
# if SIZEOF_LONG == SIZEOF_SIZE_T
   typedef long ssize_t;
# elif SIZEOF_LONG_LONG == SIZEOF_SIZE_T
   typedef long long ssize_t;
# elif SIZEOF___INT64 == SIZEOF_SIZE_T
   typedef __int64 ssize_t;
# else
   typedef int ssize_t;
# endif
#endif

/* Special handling of zlib library */
#define CURL_SPECIAL_ZLIB_H "cm_zlib.h"
