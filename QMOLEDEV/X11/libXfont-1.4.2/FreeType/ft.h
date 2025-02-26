/*
Copyright (c) 1997 by Mark Leisher
Copyright (c) 1998-2002 by Juliusz Chroboczek

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

/* $XFree86: xc/lib/font/FreeType/ft.h,v 1.21 2003/05/29 03:05:08 dawes Exp $ */

#ifndef _FT_H_
#define _FT_H_

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_SIZES_H
#include FT_TRUETYPE_IDS_H
#include FT_TRUETYPE_TABLES_H
#include FT_TYPE1_TABLES_H
#include FT_SFNT_NAMES_H
#include FT_BDF_H
#include FT_XFREE86_H
#include <X11/Xfuncproto.h>

#undef DEBUG_TRUETYPE

#ifdef DEBUG_TRUETYPE
#define MUMBLE(s) (ErrorF((s)))
#define MUMBLE1(s,x) (ErrorF((s),(x)))
#else
#define MUMBLE(s)
#define MUMBLE1(s,x)
#endif

#undef MAX
#define MAX(h,i) ((h) > (i) ? (h) : (i))
#define ADJUSTMAX(m,v) if((v)>(m)) (m)=(v)
#undef MIN
#define MIN(l,o) ((l) < (o) ? (l) : (o))
#define ADJUSTMIN(m,v) if ((v)<(m)) (m)=(v)

/* When comparing floating point values, we want to ignore small errors. */
#define NEGLIGIBLE ((double)0.001)
/* Are x and y significantly different? */
#define DIFFER(x,y) (fabs((x)-(y))>=NEGLIGIBLE*fabs(x))
/* Is x significantly different from 0 w.r.t. y? */
#define DIFFER0(x,y) (fabs(x)>=NEGLIGIBLE*fabs(y))

#define ABS(x) ((x) >= 0 ? (x) : -(x))

/* Two to the sixteenth power, as a double. */
#define TWO_SIXTEENTH ((double)(1<<16))
#define TWO_SIXTH ((double)(1<<6))

/* Data structures used across files */

typedef struct _FTMapping
{
    int named;
    FT_CharMap cmap;
    int base;
    struct _FontMap *mapping;     /* allow inclusion without fontenc.h */
} FTMappingRec, *FTMappingPtr;

/* Prototypes */

/* ftfuncs.c */

#if 0
void FreeTypeRegisterFontFileFunctions(void);
#endif

/* ftenc.c */

int FTPickMapping(char*, int, char*, FT_Face, FTMappingPtr);
unsigned FTRemap(FT_Face face, FTMappingPtr, unsigned code);

/* fttools.c */

int FTtoXReturnCode(int);
int FTGetEnglishName(FT_Face, int, char *, int);
int FTcheckForTTCName(char*, char**, int*);

extern void ErrorF(const char*, ...);

#endif /* _FT_H_ */
