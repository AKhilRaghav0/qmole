/*
 * Copyright (c) 2002-2007, Communications and Remote Sensing Laboratory, Universite catholique de Louvain (UCL), Belgium
 * Copyright (c) 2002-2007, Professor Benoit Macq
 * Copyright (c) 2001-2003, David Janssens
 * Copyright (c) 2002-2003, Yannick Verschueren
 * Copyright (c) 2003-2007, Francois-Olivier Devaux and Antonin Descampe
 * Copyright (c) 2005, Herve Drolon, FreeImage Team
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS `AS IS'
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include "opj_includes.h"

/** @defgroup BIO BIO - Individual bit input-output stream */
/*@{*/

/** @name Local static functions */
/*@{*/

/**
Write a bit
@param bio BIO handle
@param b Bit to write (0 or 1)
*/
static void opj_bio_putbit(opj_bio_t *bio, OPJ_UINT32 b);
/**
Read a bit
@param bio BIO handle
@return Returns the read bit
*/
static OPJ_UINT32 opj_bio_getbit(opj_bio_t *bio);
/**
Write a byte
@param bio BIO handle
@return Returns OPJ_TRUE if successful, returns OPJ_FALSE otherwise
*/
static OPJ_BOOL opj_bio_byteout(opj_bio_t *bio);
/**
Read a byte
@param bio BIO handle
@return Returns OPJ_TRUE if successful, returns OPJ_FALSE otherwise
*/
static OPJ_BOOL opj_bio_bytein(opj_bio_t *bio);

/*@}*/

/*@}*/

/* 
==========================================================
   local functions
==========================================================
*/

OPJ_BOOL opj_bio_byteout(opj_bio_t *bio) {
	bio->buf = (bio->buf << 8) & 0xffff;
	bio->ct = bio->buf == 0xff00 ? 7 : 8;
	if (bio->bp >= bio->end) {
		return OPJ_FALSE;
	}
	*bio->bp++ = (OPJ_BYTE)(bio->buf >> 8);
	return OPJ_TRUE;
}

OPJ_BOOL opj_bio_bytein(opj_bio_t *bio) {
	bio->buf = (bio->buf << 8) & 0xffff;
	bio->ct = bio->buf == 0xff00 ? 7 : 8;
	if (bio->bp >= bio->end) {
		return OPJ_FALSE;
	}
	bio->buf |= *bio->bp++;
	return OPJ_TRUE;
}

void opj_bio_putbit(opj_bio_t *bio, OPJ_UINT32 b) {
	if (bio->ct == 0) {
		opj_bio_byteout(bio); /* MSD: why not check the return value of this function ? */
	}
	bio->ct--;
	bio->buf |= b << bio->ct;
}

OPJ_UINT32 opj_bio_getbit(opj_bio_t *bio) {
	if (bio->ct == 0) {
		opj_bio_bytein(bio); /* MSD: why not check the return value of this function ? */
	}
	bio->ct--;
	return (bio->buf >> bio->ct) & 1;
}

/* 
==========================================================
   Bit Input/Output interface
==========================================================
*/

opj_bio_t* opj_bio_create(void) {
	opj_bio_t *bio = (opj_bio_t*)opj_malloc(sizeof(opj_bio_t));
	return bio;
}

void opj_bio_destroy(opj_bio_t *bio) {
	if(bio) {
		opj_free(bio);
	}
}

int opj_bio_numbytes(opj_bio_t *bio) {
	return (bio->bp - bio->start);
}

void opj_bio_init_enc(opj_bio_t *bio, OPJ_BYTE *bp, OPJ_UINT32 len) {
	bio->start = bp;
	bio->end = bp + len;
	bio->bp = bp;
	bio->buf = 0;
	bio->ct = 8;
}

void opj_bio_init_dec(opj_bio_t *bio, OPJ_BYTE *bp, OPJ_UINT32 len) {
	bio->start = bp;
	bio->end = bp + len;
	bio->bp = bp;
	bio->buf = 0;
	bio->ct = 0;
}

void opj_bio_write(opj_bio_t *bio, OPJ_UINT32 v, OPJ_UINT32 n) {
	OPJ_INT32 i;
	for (i = n - 1; i >= 0; i--) {
		opj_bio_putbit(bio, (v >> i) & 1);
	}
}

OPJ_UINT32 opj_bio_read(opj_bio_t *bio, OPJ_UINT32 n) {
	OPJ_INT32 i;
    OPJ_UINT32 v;
	v = 0;
	for (i = n - 1; i >= 0; i--) {
		v += opj_bio_getbit(bio) << i;
	}
	return v;
}

OPJ_BOOL opj_bio_flush(opj_bio_t *bio) {
	bio->ct = 0;
	if (! opj_bio_byteout(bio)) {
		return OPJ_FALSE;
	}
	if (bio->ct == 7) {
		bio->ct = 0;
		if (! opj_bio_byteout(bio)) {
			return OPJ_FALSE;
		}
	}
	return OPJ_TRUE;
}

OPJ_BOOL opj_bio_inalign(opj_bio_t *bio) {
	bio->ct = 0;
	if ((bio->buf & 0xff) == 0xff) {
		if (! opj_bio_bytein(bio)) {
			return OPJ_FALSE;
		}
		bio->ct = 0;
	}
	return OPJ_TRUE;
}
