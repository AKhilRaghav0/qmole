// XftFontImp.cc  Xft font implementation for FbTk
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

//$Id: XftFontImp.cc 4199 2006-02-16 06:53:05Z mathias $

#include "XftFontImp.hh"
#include "App.hh"
#include "FbDrawable.hh"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif //HAVE_CONFIG_H

namespace FbTk {

XftFontImp::XftFontImp(const char *name, bool utf8):m_xftfont(0),
                                                    m_utf8mode(utf8) {
    if (name != 0)
        load(name);
}

XftFontImp::~XftFontImp() {
    if (m_xftfont != 0)
        XftFontClose(App::instance()->display(), m_xftfont);
}

bool XftFontImp::load(const std::string &name) {
    //Note: assumes screen 0 for now, changes on draw if needed

    Display *disp = App::instance()->display();

    XftFont *newxftfont = XftFontOpenXlfd(disp, 0, name.c_str());
    if (newxftfont == 0) {
        newxftfont = XftFontOpenName(disp, 0, name.c_str());
        if (newxftfont == 0)
            return false;
    }
    
    // destroy old font and set new
    if (m_xftfont != 0)
        XftFontClose(disp, m_xftfont);

    m_xftfont = newxftfont;

    return true;
}

void XftFontImp::drawText(const FbDrawable &w, int screen, GC gc, const char *text, size_t len, int x, int y) const {
    if (m_xftfont == 0)
        return;
    XftDraw *draw = XftDrawCreate(w.display(),
                                  w.drawable(),
                                  DefaultVisual(w.display(), screen),
                                  DefaultColormap(w.display(), screen));

    XGCValues gc_val;

    // get foreground pixel value and convert it to XRenderColor value
    // TODO: we should probably check return status
    XGetGCValues(w.display(), gc, GCForeground, &gc_val);

    // get red, green, blue values
    XColor xcol;
    xcol.pixel = gc_val.foreground;
    XQueryColor(w.display(), DefaultColormap(w.display(), screen), &xcol);

    // convert xcolor to XftColor
    XRenderColor rendcol;
    rendcol.red = xcol.red;
    rendcol.green = xcol.green;
    rendcol.blue = xcol.blue;
    rendcol.alpha = 0xFFFF;
    XftColor xftcolor;
    XftColorAllocValue(w.display(), 
                       DefaultVisual(w.display(), screen), 
                       DefaultColormap(w.display(), screen),
                       &rendcol, &xftcolor);

    // draw string
#ifdef HAVE_XFT_UTF8_STRING
    if (m_utf8mode) {
        // check the string size,
        // if the size is zero we use the XftDrawString8 function instead.
        XGlyphInfo ginfo;
        XftTextExtentsUtf8(w.display(),
                           m_xftfont,
                           (XftChar8 *)text, len,
                           &ginfo);
        if (ginfo.xOff != 0) {
            XftDrawStringUtf8(draw,
                              &xftcolor,
                              m_xftfont,
                              x, y,
                              (XftChar8 *)(text), len);
            XftColorFree(w.display(), 
                         DefaultVisual(w.display(), screen),
                         DefaultColormap(w.display(), screen), &xftcolor);
            XftDrawDestroy(draw);
            return;
        }
    }
#endif // HAVE_XFT_UTF8_STRING

    XftDrawString8(draw,
                   &xftcolor,
                   m_xftfont,
                   x, y,
                   (XftChar8 *)(text), len);


    XftColorFree(w.display(), 
                 DefaultVisual(w.display(), screen),
                 DefaultColormap(w.display(), screen), &xftcolor);
    XftDrawDestroy(draw);
}

unsigned int XftFontImp::textWidth(const char * const text, unsigned int len) const {
    if (m_xftfont == 0)
        return 0;

    XGlyphInfo ginfo;
    Display* disp = App::instance()->display();

#ifdef HAVE_XFT_UTF8_STRING
    if (m_utf8mode) {
        XftTextExtentsUtf8(disp,
                           m_xftfont,
                           (XftChar8 *)text, len,
                           &ginfo);
        if (ginfo.xOff != 0)
            return ginfo.xOff;

        // the utf8 failed, try normal extents
    }
#endif  //HAVE_XFT_UTF8_STRING

    XftTextExtents8(disp,
                    m_xftfont,
                    (XftChar8 *)text, len,
                    &ginfo);

    return ginfo.xOff;
}

unsigned int XftFontImp::height() const {
    if (m_xftfont == 0)
        return 0;
    return m_xftfont->height;
    //m_xftfont->ascent + m_xftfont->descent;
    // curiously, fonts seem to have a smaller height, but the "height"
    // is specified within the actual font, so it must be right, right?
}

}; // end namespace FbTk
