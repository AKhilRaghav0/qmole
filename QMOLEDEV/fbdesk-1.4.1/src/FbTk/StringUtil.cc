// StringUtil.cc for fluxbox
// Copyright (c) 2001 - 2006 Henrik Kinnunen (fluxgen at fluxbox dot org)
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

// $Id: StringUtil.cc 4199 2006-02-16 06:53:05Z mathias $

#include "StringUtil.hh"


#ifdef HAVE_CSTDIO
  #include <cstdio>
#else
  #include <stdio.h>
#endif
#ifdef HAVE_CSTDLIB
  #include <cstdlib>
#else
  #include <stdlib.h>
#endif
#ifdef HAVE_CCTYPE
  #include <cctype>
#else
  #include <ctype.h>
#endif
#ifdef HAVE_CASSERT
  #include <cassert>
#else
  #include <assert.h>
#endif


#include <memory>
#include <algorithm>
#include <string>

using std::string;
using std::transform;

namespace FbTk {

namespace StringUtil {

/**
   Takes a pointer to string *s as an argument,
   creates a new string n, copies s to n and
   returns a pointer to n.
*/
char *strdup(const char *s) {
    int l = strlen(s) + 1;
    char *n = new char[l];
    strncpy(n, s, l);
    return n;
}

/**
   Tries to find a string in another and
   ignoring the case of the characters
   Returns 0 on success else pointer to str.
*/
const char *strcasestr(const char *str, const char *ptn) {
    const char *s2, *p2;
    for( ; *str; str++) {
        for(s2=str, p2=ptn; ; s2++,p2++) {
            // check if we reached the end of ptn, if so, return str
            if (!*p2)
                return str;
            // check if the chars match(ignoring case)
            if (toupper(*s2) != toupper(*p2))
                break;
        }
    }
    return 0;
}

/**
   if ~ then expand it to home of user
   returns expanded filename
*/
string expandFilename(const string &filename) {
    string retval;
    size_t pos = filename.find_first_not_of(" \t");
    if (pos != string::npos && filename[pos] == '~') {
        retval = getenv("HOME");
        if (pos != filename.size()) {
            // copy from the character after '~'
            retval += static_cast<const char *>(filename.c_str() + pos + 1);
        }
    } else
        return filename; //return unmodified value

    return retval;
}

/**
   @return string from last "." to end of string
*/
string findExtension(const string &filename) {
    //get start of extension
    string::size_type start_pos = filename.find_last_of(".");
    if (start_pos == string::npos && start_pos != filename.size())
        return "";
    // return from last . to end of string
    return filename.substr(start_pos + 1);
}

string replaceString(const string &original,
                     const char *findthis,
                     const char *replace) {
    int i=0;
    const int size_of_replace = strlen(replace);
    const int size_of_find = strlen(findthis);
    string ret_str(original);
    while (i < ret_str.size()) {
        i = ret_str.find(findthis, i);
        if (i == string::npos)
            break;
        // erase old string and insert replacement
        ret_str.erase(i, size_of_find);
        ret_str.insert(i, replace);
        // jump to next position after insert
        i += size_of_replace;
    }

    return ret_str;
}

/**
   Parses a string between "first" and "last" characters
   and ignoring ok_chars as whitespaces. The value is
   returned in "out".
   Returns negative value on error and this value is the position
   in the in-string where the error occured.
   Returns positive value on success and this value is
   for the position + 1 in the in-string where the "last"-char value
   was found.
*/
int getStringBetween(string& out, const char *instr, const char first, const char last,
                     const char *ok_chars, bool allow_nesting) {
    assert(first);
    assert(last);
    assert(instr);

    string::size_type i = 0;
    string::size_type total_add=0; //used to add extra if there is a \last to skip
    string in(instr);

    // eat leading whitespace
    i = in.find_first_not_of(ok_chars);
    if (i == string::npos)
        return -in.size();   // nothing left but whitespace

    if (in[i]!=first)
        return -i; //return position to error

    // find the end of the token
    string::size_type j = i, k;
    int nesting = 0;
    while (1) {
        k = in.find_first_of(first, j+1);
        j = in.find_first_of(last, j+1);
        if (j==string::npos)
            return -in.size(); //send negative size

        if (allow_nesting && k < j && in[k-1] != '\\') {
            nesting++;
            j = k;
            continue;
        }
        //we found the last char, check so it doesn't have a '\' before
        if (j>1 && in[j-1] != '\\') {
            if (allow_nesting && nesting > 0) nesting--;
            else
                break;
        } else if (j>1 && !allow_nesting) { // we leave escapes if we're allowing nesting
            in.erase(j-1, 1); //remove the '\'
            j--;
            total_add++; //save numchars removed so we can calculate totalpos
        }
    }

    out = in.substr(i+1, j-i-1); //copy the string between first and last
    //return value to last character
    return (j+1+total_add);
}

string toLower(const string &conv) {
    string ret = conv;
    transform(ret.begin(), ret.end(), ret.begin(), tolower);
    return ret;
}

string toUpper(const string &conv) {
    string ret = conv;
    transform(ret.begin(), ret.end(), ret.begin(), toupper);
    return ret;
}

string basename(const string &filename) {
    string::size_type first_pos = filename.find_last_of("/");
    if (first_pos != string::npos)
        return filename.substr(first_pos + 1);
    return filename;
}

string::size_type removeFirstWhitespace(string &str) {
    string::size_type first_pos = str.find_first_not_of(" \t");
    if (first_pos != string::npos)
        str.erase(0, first_pos);
    return first_pos;
}


string::size_type removeTrailingWhitespace(string &str) {
    // strip trailing whitespace
    string::size_type first_pos = str.find_last_not_of(" \t");
    if (first_pos != string::npos) {
        string::size_type last_pos = str.find_first_of(" \t", first_pos);
        while (last_pos != string::npos) {
            str.erase(last_pos);
            last_pos = str.find_first_of(" \t", last_pos);
        }
    }
    return first_pos;
}

}; // end namespace StringUtil

}; // end namespace FbTk
