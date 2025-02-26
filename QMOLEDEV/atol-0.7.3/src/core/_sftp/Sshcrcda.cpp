////////////////////////////////////////////////////////////////////////////
// Atol file manager project <http://atol.sf.net>
//
// This code is licensed under BSD license.See "license.txt" for more details.
//
// File: TOFIX
////////////////////////////////////////////////////////////////////////////

/*	$OpenBSD: deattack.c,v 1.14 2001/06/23 15:12:18 itojun Exp $	*/

/*
 * Cryptographic attack detector for ssh - source code
 *
 * Copyright (c) 1998 CORE SDI S.A., Buenos Aires, Argentina.
 *
 * All rights reserved. Redistribution and use in source and binary
 * forms, with or without modification, are permitted provided that
 * this copyright notice is retained.
 *
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES ARE DISCLAIMED. IN NO EVENT SHALL CORE SDI S.A. BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY OR
 * CONSEQUENTIAL DAMAGES RESULTING FROM THE USE OR MISUSE OF THIS
 * SOFTWARE.
 *
 * Ariel Futoransky <futo@core-sdi.com>
 * <http://www.core-sdi.com>
 * 
 * Modified for use in PuTTY by Simon Tatham
 */

#include <assert.h>
#include "misc.h"
#include "ssh.h"
#include "macros.h"

typedef unsigned char uchar;
typedef unsigned short uint16;

/* SSH Constants */
#define SSH_MAXBLOCKS	(32 * 1024)
#define SSH_BLOCKSIZE	(8)

/* Hashing constants */
#define HASH_MINSIZE	(8 * 1024)
#define HASH_ENTRYSIZE	(sizeof(uint16))
#define HASH_FACTOR(x)	((x)*3/2)
#define HASH_UNUSEDCHAR	(0xff)
#define HASH_UNUSED	(0xffff)
#define HASH_IV     	(0xfffe)

#define HASH_MINBLOCKS	(7*SSH_BLOCKSIZE)

/* Hash function (Input keys are cipher results) */
#define HASH(x)		GET_32BIT_MSB_FIRST(x)

#define CMP(a, b)	(memcmp(a, b, SSH_BLOCKSIZE))

uchar ONE[4] = { 1, 0, 0, 0 };
uchar ZERO[4] = { 0, 0, 0, 0 };

static void crc_update(uint32 *a, void *b)
{
    *a = crc32_update(*a, b, 4);
}

/* detect if a block is used in a particular pattern */
static int check_crc(uchar *S, uchar *buf, uint32 len, uchar *IV)
{
    uint32 crc;
    uchar *c;

    crc = 0;
    if (IV && !CMP(S, IV)) {
        crc_update(&crc, ONE);
        crc_update(&crc, ZERO);
    }
    for (c = buf; c < buf + len; c += SSH_BLOCKSIZE) {
        if (!CMP(S, c)) {
            crc_update(&crc, ONE);
            crc_update(&crc, ZERO);
        } else {
            crc_update(&crc, ZERO);
            crc_update(&crc, ZERO);
        }
    }
    return (crc == 0);
}

/* Detect a crc32 compensation attack on a packet */
int detect_attack(CSshSession &session, uchar *buf, uint32 len, uchar *IV)
{
    static uint16 *h = (uint16 *) NULL;
    static uint32 n = HASH_MINSIZE / HASH_ENTRYSIZE;
    register uint32 i, j;
    uint32 l;
    register uchar *c;
    uchar *d;

    assert(!(len > (SSH_MAXBLOCKS * SSH_BLOCKSIZE) ||
             len % SSH_BLOCKSIZE != 0));
    for (l = n; l < HASH_FACTOR(len / SSH_BLOCKSIZE); l = l << 2)
        ;

    if (h == NULL) {
        logevent(session, "Installing CRC compensation attack detector");
        n = l;
        h = (uint16 *) smalloc(n * HASH_ENTRYSIZE);
    } else {
        if (l > n) {
            n = l;
            h = (uint16 *) srealloc(h, n * HASH_ENTRYSIZE);
        }
    }

    if (len <= HASH_MINBLOCKS) {
        for (c = buf; c < buf + len; c += SSH_BLOCKSIZE) {
            if (IV && (!CMP(c, IV))) {
                if ((check_crc(c, buf, len, IV)))
                    return 1;          /* attack detected */
                else
                    break;
            }
            for (d = buf; d < c; d += SSH_BLOCKSIZE) {
                if (!CMP(c, d)) {
                    if ((check_crc(c, buf, len, IV)))
                        return 1;      /* attack detected */
                    else
                        break;
                }
            }
        }
        return 0;                      /* ok */
    }
    memset(h, HASH_UNUSEDCHAR, n * HASH_ENTRYSIZE);

    if (IV)
        h[HASH(IV) & (n - 1)] = HASH_IV;

    for (c = buf, j = 0; c < (buf + len); c += SSH_BLOCKSIZE, j++) {
        for (i = HASH(c) & (n - 1); h[i] != HASH_UNUSED;
             i = (i + 1) & (n - 1)) {
            if (h[i] == HASH_IV) {
                if (!CMP(c, IV)) {
                    if (check_crc(c, buf, len, IV))
                        return 1;      /* attack detected */
                    else
                        break;
                }
            } else if (!CMP(c, buf + h[i] * SSH_BLOCKSIZE)) {
                if (check_crc(c, buf, len, IV))
                    return 1;          /* attack detected */
                else
                    break;
            }
        }
        h[i] = j;
    }
    return 0;                          /* ok */
}
