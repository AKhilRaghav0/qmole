/*
 *   stunnel       TLS offloading and load-balancing proxy
 *   Copyright (C) 1998-2015 Michal Trojnara <Michal.Trojnara@mirt.net>
 *
 *   This program is free software; you can redistribute it and/or modify it
 *   under the terms of the GNU General Public License as published by the
 *   Free Software Foundation; either version 2 of the License, or (at your
 *   option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *   See the GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License along
 *   with this program; if not, see <http://www.gnu.org/licenses>.
 *
 *   Linking stunnel statically or dynamically with other modules is making
 *   a combined work based on stunnel. Thus, the terms and conditions of
 *   the GNU General Public License cover the whole combination.
 *
 *   In addition, as a special exception, the copyright holder of stunnel
 *   gives you permission to combine stunnel with free software programs or
 *   libraries that are released under the GNU LGPL and with code included
 *   in the standard release of OpenSSL under the OpenSSL License (or
 *   modified versions of such code, with unchanged license). You may copy
 *   and distribute such a system following the terms of the GNU GPL for
 *   stunnel and the licenses of the other code concerned.
 *
 *   Note that people who make modified versions of stunnel are not obligated
 *   to grant this special exception for their modified versions; it is their
 *   choice whether to do so. The GNU General Public License gives permission
 *   to release a modified version without this exception; this exception
 *   also makes it possible to release a modified version which carries
 *   forward this exception.
 */

#include "common.h"
#include "prototypes.h"

    /* global OpenSSL initalization: compression, engine, entropy */
NOEXPORT void cb_free(void *parent, void *ptr, CRYPTO_EX_DATA *ad,
    int idx, long argl, void *argp);
#ifndef OPENSSL_NO_COMP
NOEXPORT int compression_init(GLOBAL_OPTIONS *);
#endif
NOEXPORT int prng_init(GLOBAL_OPTIONS *);
NOEXPORT int add_rand_file(GLOBAL_OPTIONS *, const char *);

int index_cli, index_opt, index_redirect, index_addr;

int ssl_init(void) { /* init SSL before parsing configuration file */
    SSL_load_error_strings();
    SSL_library_init();
    index_cli=SSL_get_ex_new_index(0, "cli index",
        NULL, NULL, NULL);
    index_opt=SSL_CTX_get_ex_new_index(0, "opt index",
        NULL, NULL, NULL);
    index_redirect=SSL_SESSION_get_ex_new_index(0, "redirect index",
        NULL, NULL, NULL);
    index_addr=SSL_SESSION_get_ex_new_index(0, "addr index",
        NULL, NULL, cb_free);
    if(index_cli<0 || index_opt<0 || index_redirect<0 || index_addr<0) {
        s_log(LOG_ERR, "Application specific data initialization failed");
        return 1;
    }
#ifndef OPENSSL_NO_ENGINE
    ENGINE_load_builtin_engines();
#endif
#ifndef OPENSSL_NO_DH
    dh_params=get_dh2048();
    if(!dh_params) {
        s_log(LOG_ERR, "Failed to get default DH parameters");
        return 1;
    }
#endif /* OPENSSL_NO_DH */
    return 0;
}

NOEXPORT void cb_free(void *parent, void *ptr, CRYPTO_EX_DATA *ad,
        int idx, long argl, void *argp) {
    (void)parent; /* squash the unused parameter warning */
    (void)ad; /* squash the unused parameter warning */
    (void)idx; /* squash the unused parameter warning */
    (void)argl; /* squash the unused parameter warning */
    s_log(LOG_DEBUG, "Deallocating application specific data for %s",
        (char *)argp);
    str_free(ptr);
}

int ssl_configure(GLOBAL_OPTIONS *global) { /* configure global SSL settings */
#ifdef USE_FIPS
    if(FIPS_mode()!=global->option.fips) {
        RAND_set_rand_method(NULL); /* reset RAND methods */
        if(!FIPS_mode_set(global->option.fips)) {
            ERR_load_crypto_strings();
            sslerror("FIPS_mode_set");
            return 1;
        }
    }
    s_log(LOG_NOTICE, "FIPS mode %s",
        global->option.fips ? "enabled" : "disabled");
#endif /* USE_FIPS */
#ifndef OPENSSL_NO_COMP
    if(compression_init(global))
        return 1;
#endif /* OPENSSL_NO_COMP */
    if(prng_init(global))
        return 1;
    s_log(LOG_DEBUG, "PRNG seeded successfully");
    return 0; /* SUCCESS */
}

#ifndef OPENSSL_NO_COMP
NOEXPORT int compression_init(GLOBAL_OPTIONS *global) {
    STACK_OF(SSL_COMP) *methods;

    methods=SSL_COMP_get_compression_methods();
    if(!methods) {
        if(global->compression==COMP_NONE) {
            s_log(LOG_NOTICE, "Failed to get compression methods");
            return 0; /* ignore */
        } else {
            s_log(LOG_ERR, "Failed to get compression methods");
            return 1;
        }
    }

    if(global->compression==COMP_NONE ||
            SSLeay()<0x00908051L /* 0.9.8e-beta1 */) {
        /* delete OpenSSL defaults (empty the SSL_COMP stack) */
        /* cannot use sk_SSL_COMP_pop_free,
         * as it also destroys the stack itself */
        /* only leave the standard RFC 1951 (DEFLATE) algorithm,
         * if any of the private algorithms is enabled */
        /* only allow DEFLATE with OpenSSL 0.9.8 or later
         * with OpenSSL #1468 zlib memory leak fixed */
        while(sk_SSL_COMP_num(methods))
#if OPENSSL_VERSION_NUMBER>=0x10100000L
            /* FIXME: remove when sk_SSL_COMP_pop() works again */
            OPENSSL_free(sk_pop((void *)methods));
#else
            OPENSSL_free(sk_SSL_COMP_pop(methods));
#endif
    }

    if(global->compression==COMP_NONE) {
        s_log(LOG_DEBUG, "Compression disabled");
        return 0; /* success */
    }

    /* also insert the obsolete ZLIB algorithm */
    if(global->compression==COMP_ZLIB) {
        /* 224 - within the private range (193 to 255) */
        COMP_METHOD *meth=COMP_zlib();
#if OPENSSL_VERSION_NUMBER>=0x10100000L
        if(!meth || COMP_get_type(meth)==NID_undef) {
#else
        if(!meth || meth->type==NID_undef) {
#endif
            s_log(LOG_ERR, "ZLIB compression is not supported");
            return 1;
        }
        SSL_COMP_add_compression_method(0xe0, meth);
    }
    s_log(LOG_INFO, "Compression enabled: %d method(s)",
        sk_SSL_COMP_num(methods));
    return 0; /* success */
}
#endif /* OPENSSL_NO_COMP */

NOEXPORT int prng_init(GLOBAL_OPTIONS *global) {
    int totbytes=0;
    char filename[256];
#ifndef USE_WIN32
    int bytes;
#endif

    filename[0]='\0';

    /* if they specify a rand file on the command line we
       assume that they really do want it, so try it first */
    if(global->rand_file) {
        totbytes+=add_rand_file(global, global->rand_file);
        if(RAND_status())
            return 0; /* success */
    }

    /* try the $RANDFILE or $HOME/.rnd files */
    RAND_file_name(filename, 256);
    if(filename[0]) {
        totbytes+=add_rand_file(global, filename);
        if(RAND_status())
            return 0; /* success */
    }

#ifdef RANDOM_FILE
    totbytes+=add_rand_file(global, RANDOM_FILE);
    if(RAND_status())
        return 0; /* success */
#endif

#ifdef USE_WIN32
    RAND_screen();
    if(RAND_status()) {
        s_log(LOG_DEBUG, "Seeded PRNG with RAND_screen");
        return 0; /* success */
    }
    s_log(LOG_DEBUG, "RAND_screen failed to sufficiently seed PRNG");
#elif !defined(OPENSSL_NO_EGD)
    if(global->egd_sock) {
        if((bytes=RAND_egd(global->egd_sock))==-1) {
            s_log(LOG_WARNING, "EGD Socket %s failed", global->egd_sock);
            bytes=0;
        } else {
            totbytes+=bytes;
            s_log(LOG_DEBUG, "Snagged %d random bytes from EGD Socket %s",
                bytes, global->egd_sock);
            return 0; /* OpenSSL always gets what it needs or fails,
                         so no need to check if seeded sufficiently */
        }
    }
#else
    /* try the good-old default /dev/urandom, if available  */
    totbytes+=add_rand_file(global, "/dev/urandom");
    if(RAND_status())
        return 0; /* success */
#endif /* USE_WIN32 */

    /* random file specified during configure */
    s_log(LOG_ERR, "PRNG seeded with %d bytes total", totbytes);
    s_log(LOG_ERR, "PRNG was not seeded with enough random bytes");
    return 1; /* FAILED */
}

NOEXPORT int add_rand_file(GLOBAL_OPTIONS *global, const char *filename) {
    int readbytes;
    int writebytes;
    struct stat sb;

    if(stat(filename, &sb))
        return 0; /* could not stat() file --> return 0 bytes */
    if((readbytes=RAND_load_file(filename, global->random_bytes)))
        s_log(LOG_DEBUG, "Snagged %d random bytes from %s",
            readbytes, filename);
    else
        s_log(LOG_INFO, "Cannot retrieve any random data from %s",
            filename);
    /* write new random data for future seeding if it's a regular file */
    if(global->option.rand_write && S_ISREG(sb.st_mode)) {
        writebytes=RAND_write_file(filename);
        if(writebytes==-1)
            s_log(LOG_WARNING, "Failed to write strong random data to %s - "
                "may be a permissions or seeding problem", filename);
        else
            s_log(LOG_DEBUG, "Wrote %d new random bytes to %s",
                writebytes, filename);
    }
    return readbytes;
}

/* end of ssl.c */
