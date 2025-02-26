// XmbFontImp.cc for FbTk fluxbox toolkit
// Copyright (c) 2002 - 2006 Henrik Kinnunen (fluxgen at fluxbox dot org)
//
// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.

// $Id: XmbFontImp.cc 4199 2006-02-16 06:53:05Z mathias $

#include "XmbFontImp.hh"

#include "App.hh"
#include "StringUtil.hh"
#include "FbDrawable.hh"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif //HAVE_CONFIG_H

#ifdef HAVE_SETLOCALE
#include <locale.h>
#endif // HAVE_SETLOCALE

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif // _GNU_SOURCE

#ifdef HAVE_CSTDIO
  #include <cstdio>
#else
  #include <stdio.h>
#endif
#ifdef HAVE_CSTDARG
  #include <cstdarg>
#else
  #include <stdarg.h>
#endif
#include <iostream>
#ifdef HAVE_CSTRING
  #include <cstring>
#else
  #include <string.h>
#endif

using namespace std;

namespace {

const char *getFontSize(const char *pattern, int *size) {
    const char *p;
    const char *p2=0;
    int n=0;

    for (p=pattern; 1; p++) {
        if (!*p) {
            if (p2!=0 && n>1 && n<72) {
                *size = n; return p2+1;
            } else {
                *size = 16; return 0;
            }
        } else if (*p=='-') {
            if (n>1 && n<72 && p2!=0) {
                *size = n;
                return p2+1;
            }
            p2=p; n=0;
        } else if (*p>='0' && *p<='9' && p2!=0) {
            n *= 10;
            n += *p-'0';
        } else {
            p2=0; n=0;
        }
    }
}

const char *getFontElement(const char *pattern, char *buf, int bufsiz, ...) {
    const char *p, *v;
    char *p2;
    va_list va;

    va_start(va, bufsiz);
    buf[bufsiz-1] = 0;
    buf[bufsiz-2] = '*';
    while((v = va_arg(va, char *)) != 0) {
        p = FbTk::StringUtil::strcasestr(pattern, v);
        if (p) {
            strncpy(buf, p+1, bufsiz-2);
            p2 = strchr(buf, '-');
            if (p2) *p2=0;
            va_end(va);
            return p;
        }
    }
    va_end(va);
    strncpy(buf, "*", bufsiz);
    return 0;
}

XFontSet createFontSet(const char *fontname, bool& utf8mode) {
    Display *display = FbTk::App::instance()->display();
    XFontSet fs;
    char **missing, *def = "-";
    int nmissing;
    std::string orig_locale = "";

#ifdef HAVE_SETLOCALE
    if (utf8mode) {
        orig_locale = setlocale(LC_CTYPE, NULL);
        setlocale(LC_CTYPE, "UTF-8");
    }
#endif // HAVE_SETLOCALE
    fs = XCreateFontSet(display,
                        fontname, &missing, &nmissing, &def);

    if (fs && (! nmissing)) {
#ifdef HAVE_SETLOCALE
        if (utf8mode)
            setlocale(LC_CTYPE, orig_locale.c_str());
#endif // HAVE_SETLOCALE
        return fs;
    }

#ifdef HAVE_SETLOCALE
    if (! fs) {
        if (nmissing)
            XFreeStringList(missing);

        setlocale(LC_CTYPE, "C");
        fs = XCreateFontSet(display, fontname,
                            &missing, &nmissing, &def);
        setlocale(LC_CTYPE, orig_locale.c_str());
    }
    if (utf8mode)
        setlocale(LC_CTYPE, orig_locale.c_str());
#endif // HAVE_SETLOCALE

    utf8mode = false;
    
    return fs;
}

};
namespace FbTk {

XmbFontImp::XmbFontImp(const char *filename, bool utf8) : m_fontset(0), m_setextents(0), m_utf8mode(utf8) {
    if (filename != 0)
        load(filename);
}

XmbFontImp::~XmbFontImp() {
    if (m_fontset != 0)
        XFreeFontSet(App::instance()->display(), m_fontset);
}

bool XmbFontImp::load(const std::string &fontname) {
    if (fontname.empty())
        return false;

    XFontSet set = createFontSet(fontname.c_str(), m_utf8mode);
    if (set == 0)
        return false;

    if (m_fontset != 0)
        XFreeFontSet(App::instance()->display(), m_fontset);

    m_fontset = set;
    m_setextents = XExtentsOfFontSet(m_fontset);

    return true;
}

void XmbFontImp::drawText(const FbDrawable &w, int screen, GC gc, const char *text,
                          size_t len, int x, int y) const {

    if (m_fontset == 0)
        return;

#ifdef X_HAVE_UTF8_STRING
    if (m_utf8mode) {
        Xutf8DrawString(w.display(), w.drawable(), m_fontset,
                        gc, x, y,
                        text, len);
    } else
#endif //X_HAVE_UTF8_STRING
    {
        XmbDrawString(w.display(), w.drawable(), m_fontset,
                      gc, x, y,
                      text, len);
    }
}

unsigned int XmbFontImp::textWidth(const char * const text, unsigned int len) const {
    if (m_fontset == 0)
        return 0;

    XRectangle ink, logical;
#ifdef X_HAVE_UTF8_STRING
    if (m_utf8mode) {
        Xutf8TextExtents(m_fontset, text, len,
                         &ink, &logical);
        if (logical.width != 0)
            return logical.width;
    }
#endif // X_HAVE_UTF8_STRING

    XmbTextExtents(m_fontset, text, len,
                   &ink, &logical);
    return logical.width;
}

unsigned int XmbFontImp::height() const {
    if (m_fontset == 0)
        return 0;
    return m_setextents->max_ink_extent.height;
}

}; // end namespace FbTk

