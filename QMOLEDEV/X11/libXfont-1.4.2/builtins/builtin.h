/*
 * Id: builtin.h,v 1.2 1999/11/02 06:16:47 keithp Exp $
 *
 * Copyright 1999 SuSE, Inc.
 *
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the name of SuSE not be used in advertising or
 * publicity pertaining to distribution of the software without specific,
 * written prior permission.  SuSE makes no representations about the
 * suitability of this software for any purpose.  It is provided "as is"
 * without express or implied warranty.
 *
 * SuSE DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING ALL
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL SuSE
 * BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION
 * OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN 
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 * Author:  Keith Packard, SuSE, Inc.
 */
/* $XFree86: xc/lib/font/builtins/builtin.h,v 1.3 1999/12/30 02:29:49 robin Exp $ */

#include <X11/Xdefs.h>
#include <font.h>
#include "fontxlfd.h"
#include "fntfil.h"
#include "fntfilio.h"
#include "fntfilst.h"

typedef struct _BuiltinFile {
    const char  *name;
    int		len;
    const char  *bits;
} BuiltinFileRec, *BuiltinFilePtr;

typedef struct _BuiltinDir {
    const char	*file_name;
    const char	*font_name;
} BuiltinDirRec, *BuiltinDirPtr;

typedef struct _BuiltinAlias {
    const char	*alias_name;
    const char	*font_name;
} BuiltinAliasRec, *BuiltinAliasPtr;

extern const BuiltinFileRec	builtin_files[];
extern const int		builtin_files_count;

extern const BuiltinDirRec	builtin_dir[];
extern const int		builtin_dir_count;

extern const BuiltinAliasRec	builtin_alias[];
extern const int		builtin_alias_count;

FontFilePtr
BuiltinFileOpen (char *);

int
BuiltinFileClose (FontFilePtr );

int
BuiltinReadDirectory (char *directory, FontDirectoryPtr *pdir);

void
BuiltinRegisterFontFileFunctions(void);

int
BuiltinOpenBitmap (FontPathElementPtr	fpe,
		   FontPtr		*ppFont,
		   int			flags,
		   FontEntryPtr		entry,
		   char			*fileName,
		   fsBitmapFormat	format,
		   fsBitmapFormatMask	fmask,
		   FontPtr		non_cacheable_font);
