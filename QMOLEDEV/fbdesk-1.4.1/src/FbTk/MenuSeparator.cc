// MenuSeparator.cc for FbTk - Fluxbox ToolKit
// Copyright (c) 2004 - 2006 Henrik Kinnunen (fluxgen at fluxbox dot org)
//                and Simon Bowden (rathnor at users.sourceforge.net)
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

// $Id: MenuSeparator.cc 4199 2006-02-16 06:53:05Z mathias $

#include "MenuSeparator.hh"

#include "GContext.hh"
#include "MenuTheme.hh"
#include "FbDrawable.hh"

namespace FbTk {

void MenuSeparator::draw(FbDrawable &drawable, 
                         const MenuTheme &theme,
                         bool highlight, bool draw_foreground, bool draw_background,
                         int x, int y,
                         unsigned int width, unsigned int height) const {

    if (draw_background) {
        const GContext &tgc =
            (highlight ? theme.hiliteTextGC() :
             (isEnabled() ? theme.frameTextGC() : theme.disableTextGC() ) );

        drawable.drawRectangle(tgc.gc(),
                               x + theme.bevelWidth() + height + 1, y + height / 2,
                               width - ((theme.bevelWidth() + height) * 2) - 1, 0);
    }
}

}

