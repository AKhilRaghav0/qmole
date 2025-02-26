// TextBox.cc for FbTk - fluxbox toolkit
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
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.

// $Id: TextBox.cc 4199 2006-02-16 06:53:05Z mathias $

#include "TextBox.hh"
#include "Font.hh"
#include "EventManager.hh"
#include "App.hh"
#include "KeyUtil.hh"

#ifdef HAVE_CCTYPE
  #include <cctype>
#else
  #include <ctype.h>
#endif
#include <X11/keysym.h>
#include <X11/Xutil.h>

#include <iostream>

namespace FbTk {

TextBox::TextBox(int screen_num,
                 const Font &font, const std::string &text):
    FbWindow(screen_num, 0, 0, 1, 1, ExposureMask | KeyPressMask | ButtonPressMask),
    m_font(&font),
    m_text(text),
    m_gc(0),
    m_cursor_pos(0),
    m_start_pos(0),
    m_end_pos(0) {

    FbTk::EventManager::instance()->add(*this, *this);
}

TextBox::TextBox(const FbWindow &parent, 
                 const Font &font, const std::string &text):
    FbWindow(parent, 0, 0, 1, 1, ExposureMask | KeyPressMask | ButtonPressMask),
    m_font(&font),
    m_text(text),
    m_gc(0),
    m_cursor_pos(0),
    m_start_pos(0),
    m_end_pos(0) {

    FbTk::EventManager::instance()->add(*this, *this);
}

TextBox::~TextBox() {

}

void TextBox::setText(const std::string &text) {
    m_text = text;
    m_start_pos = 0;
    cursorEnd();
    adjustStartPos();
}

void TextBox::setFont(const Font &font) {
    m_font = &font;
}

void TextBox::setGC(GC gc) {
    m_gc = gc;
}

void TextBox::setInputFocus() {
    XSetInputFocus(FbTk::App::instance()->display(),
                   window(),
                   RevertToParent,
                   CurrentTime);
}

void TextBox::cursorHome() {
    m_start_pos = m_cursor_pos = 0;
    adjustEndPos();
}

void TextBox::cursorEnd() {
    m_end_pos = text().size();
    adjustStartPos();
    m_cursor_pos = m_end_pos - m_start_pos;
}

void TextBox::cursorForward() {
    if (m_start_pos + cursorPosition() < m_end_pos)
        m_cursor_pos++;
    else if (m_end_pos < text().size()) {
        m_cursor_pos++;
        m_end_pos++;
        adjustStartPos();
    }
}

void TextBox::enter() {

}

void TextBox::cursorBackward() {
    if (cursorPosition())
        m_cursor_pos--;
    else if (m_start_pos) {
        m_start_pos--;
        adjustEndPos();
    }
}

void TextBox::backspace() {
    if (m_start_pos || cursorPosition()) {
        m_text.erase(m_start_pos + cursorPosition() - 1, 1);
        if (cursorPosition())
            setCursorPosition(cursorPosition() - 1);
        else
            m_start_pos--;
        adjustEndPos();
    }
}

void TextBox::deleteForward() {
    if (m_start_pos + m_cursor_pos < m_end_pos) {
        m_text.erase(m_start_pos + m_cursor_pos, 1);
        adjustEndPos();
    }
}

void TextBox::insertText(const std::string &val) {
    m_text.insert(m_start_pos + cursorPosition(), val);
    m_cursor_pos += val.size();
    m_end_pos += val.size();
 
    adjustPos();
}

void TextBox::killToEnd() {
    if (cursorPosition() >= 0 && cursorPosition() < static_cast<signed>(text().size())) {
        m_text.erase(cursorPosition());
        setText(m_text);
    }
}

void TextBox::clear() {
    FbWindow::clear();
    // center text by default
    int center_pos = (height() + font().ascent())/2;
    if (gc() == 0)
        setGC(DefaultGC(FbTk::App::instance()->display(), screenNumber()));

    font().drawText(*this, screenNumber(), 
                    gc(),                     
                    text().c_str() + m_start_pos, 
                    m_end_pos - m_start_pos, 
                    0, center_pos); // pos

    // draw cursor position
    int cursor_pos = font().textWidth(text().c_str() + m_start_pos, m_cursor_pos) + 1;
    drawLine(gc(), cursor_pos, center_pos, cursor_pos, center_pos - font().height());
}

void TextBox::moveResize(int x, int y,
                         unsigned int width, unsigned int height) {
    FbWindow::moveResize(x, y, width, height);
    clear();
}

void TextBox::resize(unsigned int width, unsigned int height) {
    FbWindow::resize(width, height);
    clear();
}

void TextBox::exposeEvent(XExposeEvent &event) {
    clear();
}

void TextBox::buttonPressEvent(XButtonEvent &event) {
    setInputFocus();
    if (event.window == window()) {
        std::string::size_type i;
        std::string::size_type click_pos = m_end_pos;
        int delta = width();
        int tmp = 0;
        for(i = m_start_pos; i <= m_end_pos; i++) {
            tmp = abs(event.x - font().textWidth(m_text.c_str() + m_start_pos, i - m_start_pos));

            if (tmp < delta) {
                delta = tmp;
                click_pos = i;
            }
        }
        m_cursor_pos = click_pos - m_start_pos;
        clear();
    }
}

void TextBox::keyPressEvent(XKeyEvent &event) {
    
    event.state = KeyUtil::instance().cleanMods(event.state);

    KeySym ks;
    char keychar[1];
    XLookupString(&event, keychar, 1, &ks, 0);
    // a modifier key by itself doesn't do anything
    if (IsModifierKey(ks)) return;

    if (FbTk::KeyUtil::instance().isolateModifierMask(event.state)) { // handle keybindings with state
        if ((event.state & ControlMask) == ControlMask) {

            switch (ks) {
            case XK_b:
                cursorBackward();
                break;
            case XK_f:
                cursorForward();
                break;
            case XK_a:
                cursorHome();
                break;
            case XK_e:
                cursorEnd();
                break;
            case XK_d:
                deleteForward();
                break;
            case XK_k:
                killToEnd();
                break;
            case XK_c:
                cursorHome();
                m_text = "";
                m_start_pos = 0;
                m_cursor_pos = 0;
                m_end_pos = 0;
                break;
            case XK_Left: {
                    int pos = findEmptySpaceLeft();
                    if (pos < m_start_pos){
                        m_start_pos  = pos;
                        m_cursor_pos = 0;
                    } else if (m_start_pos > 0) {
                        m_cursor_pos = pos - m_start_pos;
                    } else {
                        m_cursor_pos = pos;
                    }
                    adjustPos();
                }
                break;
            case XK_Right:
                if (m_text.size() && m_cursor_pos < m_text.size()){
                    int pos = findEmptySpaceRight() - m_start_pos;
                    if (m_start_pos + pos <= m_end_pos)
                        m_cursor_pos = pos;
                    else if (m_end_pos < text().size()) {
                        m_cursor_pos = pos;
                        m_end_pos = pos;
                    }

                    adjustPos();

                }
                break;

            case XK_BackSpace: {
                    int pos = findEmptySpaceLeft();
                    m_text.erase(pos, m_cursor_pos - pos + m_start_pos);

                    if (pos < m_start_pos){
                        m_start_pos  = pos;
                        m_cursor_pos = 0;
                    } else if (m_start_pos > 0) {
                        m_cursor_pos = pos - m_start_pos;
                    } else {
                        m_cursor_pos = pos;
                    }
                    adjustPos();
                }
                break;
            case XK_Delete: {
                    if (!m_text.size() || m_cursor_pos >= m_text.size())
                        break;
                    int pos = findEmptySpaceRight();
                    m_text.erase(m_cursor_pos + m_start_pos, pos - (m_cursor_pos + m_start_pos));
                    adjustPos();
                }
                break;
            }
        } else if ((event.state & ShiftMask)== ShiftMask || 
                   (event.state & 0x80) == 0x80) { // shif and altgr
            if (isprint(keychar[0])) {
                std::string val;
                val += keychar[0];
                insertText(val);
            }
        }

    } else { // no state

        switch (ks) {
        case XK_Return:
            enter();
            break;
        case XK_BackSpace:
            backspace();
            break;
        case XK_Home:
            cursorHome();
            break;
        case XK_End:
            cursorEnd();
            break;
        case XK_Left:
            cursorBackward();
            break;
        case XK_Right:
            cursorForward();
            break;
        case XK_Delete:
            deleteForward();
            break;
        default:
            switch (ks) {
            case XK_KP_Insert:
                keychar[0] = '0';
                break;
            case XK_KP_End:
                keychar[0] = '1';
                break;
            case XK_KP_Down:
                keychar[0] = '2';
                break;
            case XK_KP_Page_Down:
                keychar[0] = '3';
                break;
            case XK_KP_Left:
                keychar[0] = '4';
                break;
            case XK_KP_Begin:
                keychar[0] = '5';
                break;
            case XK_KP_Right:
                keychar[0] = '6';
                break;
            case XK_KP_Home:
                keychar[0] = '7';
                break;
            case XK_KP_Up:
                keychar[0] = '8';
                break;
            case XK_KP_Page_Up:
                keychar[0] = '9';
                break;
            case XK_KP_Delete:
                keychar[0] = ',';
                break;
            };
            if (isprint(keychar[0])) {
                std::string val;
                val += keychar[0];
                insertText(val);
            }
        }
    }
    clear();
}

void TextBox::setCursorPosition(int pos) {
    m_cursor_pos = pos < 0 ? 0 : pos;
    if (m_cursor_pos > text().size())
        cursorEnd();
}

void TextBox::adjustEndPos() {
    m_end_pos = text().size();
    int text_width = font().textWidth(text().c_str() + m_start_pos, m_end_pos - m_start_pos);
    while (text_width > static_cast<signed>(width())) {
        m_end_pos--;
        text_width = font().textWidth(text().c_str() + m_start_pos, m_end_pos - m_start_pos);
    }
}

void TextBox::adjustStartPos() {
    
    int text_width = font().textWidth(text().c_str(), m_end_pos);
    if (text_width < static_cast<signed>(width()))
        return;

    int start_pos = 0;
    while (text_width > static_cast<signed>(width())) {
        start_pos++;
        text_width = font().textWidth(text().c_str() + start_pos, m_end_pos - start_pos);
    }

    // adjust cursorPosition() according relative to change to m_start_pos
    m_cursor_pos -= start_pos - m_start_pos;
    m_start_pos = start_pos;
}

int TextBox::findEmptySpaceLeft(){

    // found the first left space symbol
    int pos = m_text.rfind(' ', (m_start_pos + m_cursor_pos) > 0 ? 
                                 m_start_pos + m_cursor_pos - 1 : 0);

    // do we have one more space symbol near?
    int next_pos = -1;
    while (pos > 0 && (next_pos = m_text.rfind(' ', pos - 1)) > -1){
        if (next_pos + 1 < pos)
            break;
        pos = next_pos;
    }
    if (pos < 0)  
        pos = 0;

    return pos;

}
int TextBox::findEmptySpaceRight(){

    // found the first right space symbol
    int pos = m_text.find(' ', m_start_pos + m_cursor_pos);

    // do we have one more space symbol near?
    int next_pos = -1;
    while (pos > -1 && pos < m_text.size() && (next_pos = m_text.find(' ', pos + 1)) > -1 ){

        if (next_pos - 1 > pos)
            break;
        pos = next_pos;
    }
    if (pos < 0)
        pos = m_text.size() - 1;

    return pos + 1; // (+1) - sets cursor at the right.

}
void TextBox::adjustPos(){
    if (m_start_pos + cursorPosition() < m_end_pos)
        adjustEndPos();
    else
        adjustStartPos();

}
}; // end namespace FbTk
