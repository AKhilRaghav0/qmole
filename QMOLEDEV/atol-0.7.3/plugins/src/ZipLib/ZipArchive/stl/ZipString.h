////////////////////////////////////////////////////////////////////////////////
// This source file is part of the ZipArchive library source distribution and
// is Copyrighted 2000 - 2006 by Tadeusz Dracz (http://www.artpol-software.com/)
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
// 
// For the licensing details see the file License.txt
////////////////////////////////////////////////////////////////////////////////

#ifndef __ZIPSTRING_H__
#define __ZIPSTRING_H__
#include "stdafx.h"

#if defined(_MSC_VER) && (_MSC_VER > 1100) 
	#pragma warning( push, 3 ) // STL requirements
#endif

/**
	If you wish the class to respect your locale (if it is different from 
	English), you need to set it in your program with function 
	std::locale::global(std::locale("")) (to set it to be the same as your 
	system locale) or e.g. std::locale::global(std::locale("French"))
	( to set it to a specified value); setlocale() function is not sufficient here.
*/


#include <string>
#include <algorithm>
#include <stdarg.h>
#include <stdio.h>
#include <cctype>

#include "ZipExport.h"

#ifndef __GNUC__
    #ifndef _vsntprintf 
	#ifdef  _UNICODE
		#define _vsntprintf _vsnwprintf
	#else
		#define _vsntprintf _vsnprintf
	#endif
    #endif
#include <locale>
#else
       #define _vsntprintf vsnprintf
#endif



typedef std::basic_string<TCHAR> stdbs;
/**
	This is not a full MFC - CString replacement.
	For now it contains mostly the methods required by ZipArchive library.
*/
class ZIP_API CZipString : public stdbs
{
	void TrimInternalL(size_type iPos)
	{
		if (iPos == npos)
			erase ();
		if (iPos)
			erase(0, iPos);
	}
	void TrimInternalR(size_type iPos)
	{
		if (iPos == npos)
			erase ();
		erase(++iPos);
	}
#ifndef __GNUC__
	static int zslen(const TCHAR* lpsz)
	{
		if (!lpsz) return 0;

		// we want to take into account the locale stuff (by using standard templates)

		#ifdef _UNICODE
			return (int)std::wstring(lpsz).length();
		#else
			return (int)std::string(lpsz).length();
		#endif
	}
	static TCHAR tl(TCHAR c)
	{
		// use_facet doesn't work here well (doesn't convert all the local characters properly)
		return std::tolower(c, std::locale());
	}
	static TCHAR tu(TCHAR c)
	{
		// use_facet doesn't work here well (doesn't convert all the local characters properly)
		return std::toupper(c, std::locale());
	}
#else
   	static int zslen(const TCHAR* lpsz)
   	{
  		#if (__GNUC__ < 3) // I'm not sure which precisely version should be put here
      		return lpsz ? std::string_char_traits<TCHAR>::length(lpsz) : 0;
		#else
      		return lpsz ? std::char_traits<TCHAR>::length(lpsz) : 0;
		#endif

	}
	static char tl(char c)
	{
		return (char)std::tolower(c);
	}
	static char tu(char c)
	{
		return (char)std::toupper(c);
	}
#endif



public:
	CZipString(){}
	explicit CZipString (TCHAR ch, int nRepeat = 1):stdbs(nRepeat, ch){}
	CZipString( const CZipString& stringSrc ) {assign(stringSrc);}
	CZipString( const stdbs& stringSrc ) {assign(stringSrc);}
	CZipString( LPCTSTR lpsz ){if (!lpsz) Empty(); else assign(lpsz);}
	operator LPCTSTR() const{return c_str();}
	
	int GetLength() const {return (int) size();}
	bool IsEmpty() const {return empty();}
	void Empty() {erase(begin(), end());}
	TCHAR GetAt (int iIndex) const{return at(iIndex);}
	TCHAR operator[] (int iIndex) const{return at(iIndex);}
	void SetAt( int nIndex, TCHAR ch ) {at(nIndex) = ch;}
	LPTSTR GetBuffer(int nMinBufLength)
	{
		if ((int)size() < nMinBufLength)
			resize(nMinBufLength);
      		return empty() ? const_cast<TCHAR*>(data()) : &(at(0));
	}
	void ReleaseBuffer( int nNewLength = -1 ) { resize(nNewLength > -1 ? nNewLength : zslen(c_str()));}
	void TrimLeft( TCHAR chTarget )
	{
		TrimInternalL(find_first_not_of(chTarget));
	}
	void TrimLeft( LPCTSTR lpszTargets )
	{
		TrimInternalL(find_first_not_of(lpszTargets));
	}
	void TrimRight( TCHAR chTarget )
	{
		TrimInternalR(find_last_not_of(chTarget));
	}
	void TrimRight( LPCTSTR lpszTargets )
	{
		TrimInternalR(find_last_not_of(lpszTargets));
	}
	void Format(LPCTSTR lpszFormat, ...)
	{
		va_list arguments;
		va_start (arguments, lpszFormat);
		TCHAR* pBuf = NULL;
		int iCounter = 1, uTotal = 0;
		do 
		{
			int nLen = sizeof(TCHAR) * iCounter*1024;
			pBuf = (TCHAR*)realloc((void*)pBuf, nLen);
			if (!pBuf)
				return;

#if _MSC_VER >= 1400	
			uTotal = _vsntprintf_s(pBuf, nLen, nLen - 1, lpszFormat, arguments);
#else
			uTotal = _vsntprintf(pBuf, nLen - 1, lpszFormat, arguments);
#endif
			
			if (uTotal == -1 || (uTotal == nLen - 1) ) // for some implementations
			{
				pBuf[nLen - 1] = _T('\0');
				if (iCounter == 7)
					break;
			}
			else
			{
				pBuf[uTotal] = _T('\0');
				break;
			}

		} while (true);
		
		va_end (arguments);
	    *this = pBuf;
		free(pBuf);
	}
	void Insert( int nIndex, LPCTSTR pstr ){insert(nIndex, pstr, zslen(pstr));}
	void Insert( int nIndex, TCHAR ch ) {insert(nIndex, 1, ch);}
	int Delete( int nIndex, int nCount = 1 )
	{
		int iSize = (int) size();
		int iToDelete = iSize < nIndex + nCount ? iSize - nIndex : nCount;
		if (iToDelete > 0)
		{
			erase(nIndex, iToDelete);
			iSize -= iToDelete;
		}
		return iSize;
	}
	void MakeLower() 
	{
			std::transform(begin(),end(),begin(),tl);
	}
	void MakeUpper() 
	{
			std::transform(begin(),end(),begin(),tu);
	}
	void MakeReverse()
	{
		std::reverse(begin(), end());

	}
	CZipString Left( int nCount ) const { return substr(0, nCount);}
	CZipString Right( int nCount) const 
	{
		nCount = (int)size() < nCount ? (int)size() : nCount;
		return substr(size() - nCount);
	}
	CZipString Mid( int nFirst ) const {return substr(nFirst);}
	CZipString Mid( int nFirst, int nCount ) const {return substr(nFirst, nCount);}
	int Collate( LPCTSTR lpsz ) const
	{
#ifndef __GNUC__
		#ifdef _UNICODE
			return wcscoll(c_str(), lpsz);
		#else			
			return strcoll(c_str(), lpsz);
		#endif
#else
//       		return compare(lpsz);
			return strcoll(c_str(), lpsz);
#endif
	
	}
	int CollateNoCase( LPCTSTR lpsz ) const
	{
		CZipString s1(c_str()), s2(lpsz);
		s1.MakeLower();
		s2.MakeLower();
		return s1.Collate(s2);
	}
	int Compare( LPCTSTR lpsz ) const
	{
		return compare(lpsz);
	}
	int CompareNoCase( LPCTSTR lpsz ) const
	{
		CZipString s1(c_str()), s2(lpsz);
		s1.MakeLower();
		s2.MakeLower();
		return s1.Compare(s2);
	}
	bool operator != (LPCTSTR lpsz)
	{
		return Compare(lpsz) != 0;
	}
	bool operator == (LPCTSTR lpsz)
	{
		return Compare(lpsz) == 0;
	}
	int Find( TCHAR ch, int nStart = 0) const
	{
		return (int) find(ch, nStart);
	}

	int Find( LPCTSTR pstr, int nStart = 0) const
	{
		return (int) find(pstr, nStart);
	}

	int Replace( TCHAR chOld, TCHAR chNew )
	{
		int iCount = 0;
		for (iterator it = begin(); it != end(); ++it)
			if (*it == chOld)
			{
				*it = chNew;
				iCount++;
			}
		return iCount;
	}



	
};

/**
	A poiter type to point to CZipString to Collate or CollateNoCase
	or Compare or CompareNoCase
*/
typedef int (CZipString::*ZIPSTRINGCOMPARE)( LPCTSTR ) const;

/**
	return a pointer to the function in CZipString structure, 
	used to compare elements depending on the arguments
*/
	ZIP_API ZIPSTRINGCOMPARE GetCZipStrCompFunc(bool bCaseSensitive, bool bCollate = true);


#if defined(_MSC_VER) && (_MSC_VER > 1100)
	#pragma warning( pop)
#endif


#endif //__ZIPSTRING_H__

