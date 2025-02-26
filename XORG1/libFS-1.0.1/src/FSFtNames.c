/* $Xorg: FSFtNames.c,v 1.4 2001/02/09 02:03:25 xorgcvs Exp $ */
/*
 * Copyright 1990 Network Computing Devices;
 * Portions Copyright 1987 by Digital Equipment Corporation
 *
 * Permission to use, copy, modify, distribute, and sell this software 
 * and its documentation for any purpose is hereby granted without fee, 
 * provided that the above copyright notice appear in all copies and 
 * that both that copyright notice and this permission notice appear 
 * in supporting documentation, and that the names of Network Computing 
 * Devices or Digital not be used in advertising or publicity pertaining 
 * to distribution of the software without specific, written prior 
 * permission. Network Computing Devices or Digital make no representations 
 * about the suitability of this software for any purpose.  It is provided 
 * "as is" without express or implied warranty.
 *
 * NETWORK COMPUTING DEVICES AND  DIGITAL DISCLAIM ALL WARRANTIES WITH
 * REGARD TO THIS SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES OF 
 * MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL NETWORK COMPUTING DEVICES
 * OR DIGITAL BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES 
 * OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, 
 * WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, 
 * ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS 
 * SOFTWARE.
 */
/* $XFree86: xc/lib/FS/FSFtNames.c,v 1.6tsi Exp $ */

/*

Copyright 1987, 1994, 1998  The Open Group

Permission to use, copy, modify, distribute, and sell this software and its
documentation for any purpose is hereby granted without fee, provided that
the above copyright notice appear in all copies and that both that
copyright notice and this permission notice appear in supporting
documentation.

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
OPEN GROUP BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

Except as contained in this notice, the name of The Open Group shall not be
used in advertising or otherwise to promote the sale, use or other dealings
in this Software without prior written authorization from The Open Group.

*/

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "FSlibint.h"

char      **
FSListFonts(
    FSServer	*svr,
    char	*pattern,
    int		 maxNames,
    int		*actualCount)
{
    long        nbytes;
    int         i,
                length;
    char      **flist;
    char       *c;
    fsListFontsReply rep;
    fsListFontsReq *req;
    long        rlen;

    GetReq(ListFonts, req);
    req->maxNames = maxNames;
    nbytes = req->nbytes = pattern ? strlen(pattern) : 0;
    req->length += (nbytes + 3) >> 2;
    _FSSend(svr, pattern, nbytes);
    if (!_FSReply(svr, (fsReply *) & rep,
	  (SIZEOF(fsListFontsReply) - SIZEOF(fsGenericReply)) >> 2, fsFalse))
	return (char **) 0;

    if (rep.nFonts
#if (SIZE_MAX >> 2) <= UINT_MAX
	&& rep.nFonts <= SIZE_MAX / sizeof(char *)
	&& rep.length <= (SIZE_MAX >> 2)
#endif
	) {
	flist = (char **) FSmalloc((unsigned) rep.nFonts * sizeof(char *));
	rlen = (rep.length << 2) - SIZEOF(fsListFontsReply);
	c = (char *) FSmalloc((unsigned) (rlen + 1));

	if ((!flist) || (!c)) {
	    if (flist)
		FSfree((char *) flist);
	    if (c)
		FSfree(c);
	    _FSEatData(svr, (unsigned long) rlen);
	    SyncHandle();
	    return (char **) NULL;
	}
	_FSReadPad(svr, c, rlen);
	/* unpack */
	length = *(unsigned char *)c;
	for (i = 0; i < rep.nFonts; i++) {
	    flist[i] = c + 1;
	    c += length + 1;
	    length = *(unsigned char *)c;
	    *c = '\0';
	}
    } else {

	flist = (char **) NULL;
    }

    *actualCount = rep.nFonts;
    SyncHandle();
    return flist;

}

int FSFreeFontNames(char **list)
{
    if (list) {
	FSfree(list[0] - 1);
	FSfree((char *) list);
    }
    return 1;
}
