// FbPixmap.cc for FbTk - Fluxbox ToolKit
// Copyright (c) 2003 - 2006 Henrik Kinnunen (fluxgen at fluxbox dot org)
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

// $Id: FbPixmap.cc 4199 2006-02-16 06:53:05Z mathias $

#include "FbPixmap.hh"
#include "App.hh"
#include "GContext.hh"
#include "Transparent.hh"

#include <X11/Xutil.h>
#include <X11/Xatom.h>
#include <iostream>
#include <string>

using namespace std;

namespace FbTk {

Pixmap *FbPixmap::m_root_pixmaps = 0;

const char* FbPixmap::root_prop_ids[] = {
    "_XROOTPMAP_ID",
    "_XSETROOT_ID",
    0
};

// same number as in root_prop_ids
Atom FbPixmap::root_prop_atoms[] = {
    None,
    None,
    None
};


FbPixmap::FbPixmap():m_pm(0),
                     m_width(0), m_height(0),
                     m_depth(0) {
}

FbPixmap::FbPixmap(const FbPixmap &the_copy):FbDrawable(), m_pm(0),
                                             m_width(0), m_height(0),
                                             m_depth(0){
    copy(the_copy);
}

FbPixmap::FbPixmap(Pixmap pm):m_pm(0),
                              m_width(0), m_height(0),
                              m_depth(0) {
    if (pm == 0)
        return;
    // assign X pixmap to this
    (*this) = pm;
}

FbPixmap::FbPixmap(const FbDrawable &src,
                   unsigned int width, unsigned int height,
                   int depth):m_pm(0),
                              m_width(0), m_height(0),
                              m_depth(0) {

    create(src.drawable(), width, height, depth);
}

FbPixmap::FbPixmap(Drawable src,
                   unsigned int width, unsigned int height,
                   int depth):m_pm(0),
                              m_width(0), m_height(0),
                              m_depth(0) {

    create(src, width, height, depth);
}

FbPixmap::~FbPixmap() {
    free();
}

FbPixmap &FbPixmap::operator = (const FbPixmap &the_copy) {
    copy(the_copy);
    return *this;
}

FbPixmap &FbPixmap::operator = (Pixmap pm) {
    // free pixmap before we set new
    free();

    if (pm == 0)
        return *this;

    // get width, height and depth for the pixmap
    Window root;
    int x, y;
    unsigned int border_width, bpp;
    XGetGeometry(display(),
                 pm,
                 &root,
                 &x, &y,
                 &m_width, &m_height,
                 &border_width,
                 &bpp);

    m_depth = bpp;

    m_pm = pm;

    return *this;
}

void FbPixmap::copy(const FbPixmap &the_copy) {

    bool create_new = false;

    if (the_copy.width() != width() ||
        the_copy.height() != height() ||
        the_copy.depth() != depth() ||
        drawable() == 0)
        create_new = true;

    if (create_new)
        free();

    if (the_copy.drawable() != 0) {
        if (create_new) {
            create(the_copy.drawable(),
                   the_copy.width(), the_copy.height(),
                   the_copy.depth());
        }

        if (drawable()) {
            GContext gc(drawable());

            copyArea(the_copy.drawable(),
                     gc.gc(),
                     0, 0,
                     0, 0,
                     width(), height());
        }
    }
}

// screen doesn't count if depth is "zero"...
void FbPixmap::copy(Pixmap pm, int depth, int screen_num) {
    free();
    if (pm == 0)
        return;

    // get width, height and depth for the pixmap
    Window root;
    int x, y;
    unsigned int border_width, bpp;
    unsigned int new_width, new_height;

    XGetGeometry(display(),
                 pm,
                 &root,
                 &x, &y,
                 &new_width, &new_height,
                 &border_width,
                 &bpp);

    if (depth == 0)
        depth = bpp;

    // create new pixmap and copy area
    create(root, new_width, new_height, depth);

    GC gc = XCreateGC(display(), drawable(), 0, 0);

    if (depth == bpp) {
        XCopyArea(display(), pm, drawable(), gc,
                  0, 0,
                  width(), height(),
                  0, 0);
    } else {
        XSetForeground(display(), gc, Color("black", screen_num).pixel());
        XSetBackground(display(), gc, Color("white", screen_num).pixel());
        XCopyPlane(display(), pm, drawable(), gc,
                   0, 0,
                   width(), height(),
                   0, 0, 1);
    }

    XFreeGC(display(), gc);
}

void FbPixmap::rotate() {

    // make an image copy
    XImage *src_image = XGetImage(display(), drawable(),
                                  0, 0, // pos
                                  width(), height(), // size
                                  ~0, // plane mask
                                  ZPixmap); // format
    // reverse height/width for new pixmap
    FbPixmap new_pm(drawable(), height(), width(), depth());

    GContext gc(drawable());

    // copy new area
    for (unsigned int y = 0; y < height(); ++y) {
        for (unsigned int x = 0; x < width(); ++x) {
            gc.setForeground(XGetPixel(src_image, x, y));
            // revers coordinates
            XDrawPoint(display(), new_pm.drawable(), gc.gc(), y, x);
        }
    }

    XDestroyImage(src_image);
    // free old pixmap and set new from new_pm
    free();

    m_width = new_pm.width();
    m_height = new_pm.height();
    m_depth = new_pm.depth();
    m_pm = new_pm.release();
}

void FbPixmap::scale(unsigned int dest_width, unsigned int dest_height) {

    if (drawable() == 0 ||
        (dest_width == width() && dest_height == height()))
        return;

    XImage *src_image = XGetImage(display(), drawable(),
                                  0, 0, // pos
                                  width(), height(), // size
                                  ~0, // plane mask
                                  ZPixmap); // format
    if (src_image == 0)
        return;

    // create new pixmap with dest size
    FbPixmap new_pm(drawable(), dest_width, dest_height, depth());

    GContext gc(drawable());
    // calc zoom
    float zoom_x = static_cast<float>(width())/static_cast<float>(dest_width);
    float zoom_y = static_cast<float>(height())/static_cast<float>(dest_height);

    // start scaling
    float src_x = 0, src_y = 0;
    for (unsigned int tx=0; tx < dest_width; ++tx, src_x += zoom_x) {
        src_y = 0;
        for (unsigned int ty=0; ty < dest_height; ++ty, src_y += zoom_y) {
            gc.setForeground(XGetPixel(src_image,
                                       static_cast<int>(src_x),
                                       static_cast<int>(src_y)));
            XDrawPoint(display(), new_pm.drawable(), gc.gc(), tx, ty);
        }
    }

    XDestroyImage(src_image);

    // free old pixmap and set new from new_pm
    free();

    m_width = new_pm.width();
    m_height = new_pm.height();
    m_depth = new_pm.depth();
    m_pm = new_pm.release();
}

void FbPixmap::tile(unsigned int dest_width, unsigned int dest_height) {
    if (drawable() == 0 ||
        (dest_width == width() && dest_height == height()))
        return;

    FbPixmap new_pm(drawable(), width(), height(), depth());

    new_pm.copy(m_pm, 0, 0);

    resize(dest_width, dest_height);

    FbTk::GContext gc(*this);

    gc.setTile(new_pm);
    gc.setFillStyle(FillTiled);

    fillRectangle(gc.gc(), 0, 0, dest_width, dest_height);

}



void FbPixmap::resize(unsigned int width, unsigned int height) {
    FbPixmap pm(drawable(), width, height, depth());
    *this = pm.release();
}

Pixmap FbPixmap::release() {
    Pixmap ret = m_pm;
    m_pm = 0;
    m_width = 0;
    m_height = 0;
    m_depth = 0;
    return ret;
}

void FbPixmap::rootwinPropertyNotify(int screen_num, Atom atom) {
    if (!FbTk::Transparent::haveRender()) 
        return;

    checkAtoms();
    for (int i=0; root_prop_ids[i] != 0; ++i) {
        if (root_prop_atoms[i] == atom) {
            Pixmap root_pm = None;
            Atom real_type;
            int real_format;
            unsigned long items_read, items_left;
            unsigned long *data;

            unsigned int prop = 0;
            if (XGetWindowProperty(display(),
                                   RootWindow(display(), i),
                                   root_prop_atoms[i],
                                   0l, 1l,
                                   False, XA_PIXMAP,
                                   &real_type, &real_format,
                                   &items_read, &items_left,
                                   (unsigned char **) &data) == Success) {
                if (real_format == 32 && items_read == 1) {
                    root_pm = (Pixmap) (*data);
                }
                XFree(data);
                if (root_pm != None)
                    setRootPixmap(screen_num, root_pm);
            }
            break;
        }
    }
}

void FbPixmap::setRootPixmap(int screen_num, Pixmap pm) {
    if (!m_root_pixmaps) {
        m_root_pixmaps = new Pixmap[ScreenCount(display())];
    }

    m_root_pixmaps[screen_num] = pm;
}

Pixmap FbPixmap::getRootPixmap(int screen_num, bool force_update) {
    /*
      if (!FbTk::Transparent::haveRender()) 
      return None;
    */

    // check and see if if we have the pixmaps in cache
    if (m_root_pixmaps && !force_update)
        return m_root_pixmaps[screen_num];

    // else setup pixmap cache        
    int numscreens = ScreenCount(display());
    for (int i=0; i < numscreens; ++i) {
        Atom real_type;
        int real_format;
        unsigned long items_read, items_left;
        unsigned long *data;

        unsigned int prop = 0;

        static bool print_error = true; // print error_message only once
        static const char* error_message = {
            "\n\n !!! WARNING WARNING WARNING WARNING !!!!!\n"
            "   if you experience problems with transparency:\n"
            "   you are using a wallpapersetter that \n"
            "   uses _XSETROOT_ID .. which we do not support.\n"
            "   consult 'fbsetbg -i' or try any other wallpapersetter\n"
            "   that uses _XROOTPMAP_ID !\n"
            " !!! WARNING WARNING WARNING WARNING !!!!!!\n\n"
        };

        Pixmap root_pm = None;
        for (prop = 0; root_prop_ids[prop]; prop++) {
            checkAtoms();
            if (XGetWindowProperty(display(),
                                   RootWindow(display(), i),
                                   root_prop_atoms[i],
                                   0l, 1l,
                                   False, XA_PIXMAP,
                                   &real_type, &real_format,
                                   &items_read, &items_left,
                                   (unsigned char **) &data) == Success) {
                if (real_format == 32 && items_read == 1) {
                    if (print_error && strcmp(root_prop_ids[prop], "_XSETROOT_ID") == 0) {
                        cerr<<error_message;
                        print_error = false;
                    } else
                        root_pm = (Pixmap) (*data);
                }
                XFree(data);
                if (root_pm != None)
                    break;
            }
        }
        setRootPixmap(i, root_pm);
    }

    return m_root_pixmaps[screen_num];
}

void FbPixmap::checkAtoms() {
    for (int i=0; root_prop_ids[i] != 0; ++i) {
        if (root_prop_atoms[i] == None) {
            root_prop_atoms[i] = XInternAtom(display(), root_prop_ids[i], False);
        }
    }
}

void FbPixmap::free() {
    if (m_pm != 0) {
        XFreePixmap(display(), m_pm);
        m_pm = 0;
    }
    m_width = 0;
    m_height = 0;
    m_depth = 0;
}

void FbPixmap::create(Drawable src,
                      unsigned int width, unsigned int height,
                      int depth) {
    if (src == 0)
        return;

    m_pm = XCreatePixmap(display(),
                         src, width, height, depth);
    if (m_pm == 0)
        return;

    m_width = width;
    m_height = height;
    m_depth = depth;
}

}; // end namespace FbTk
