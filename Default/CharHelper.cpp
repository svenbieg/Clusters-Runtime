//================
// CharHelper.cpp
//================

#include "pch.h"


//=======
// Using
//=======

#include "CharHelper.h"
#include "MemoryHelper.h"


//============
// Characters
//============

CHAR cUnknown='_';
WCHAR wcUnknown=L'_';


//=========
// Strings
//=========

CHAR pStrBreakA[]="\n\r\t.:!?,;/|\\";
WCHAR pStrBreakW[]=L"\n\r\t.:!?,;/|\\";
CHAR pStrDigitA[]="0123456789";
WCHAR pStrDigitW[]=L"0123456789";
CHAR pStrHexA[]="0123456789ABCDEFabcdef";
WCHAR pStrHexW[]=L"0123456789ABCDEFabcdef";
CHAR pStrSortA[]="0123456789AaBbCcDdEeFfGgHhIiJjKkLlMmNnOoPpQqRrSsTtUuVvWwXxYyZz";
WCHAR pStrSortW[]=L"0123456789AaBbCcDdEeFfGgHhIiJjKkLlMmNnOoPpQqRrSsTtUuVvWwXxYyZz";
CHAR pStrSpecialA[]="\"*/:<>?\\|";
WCHAR pStrSpecialW[]=L"\"*/:<>?\\|";

#ifdef _WINDOWS
CHAR pStrAlphaA[]="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyzÀÁÂÃÄÅÆÇÈÉÊËÌÍÎÏĞÑÒÓÔÕÖØÙÚÛÜİŞßàáâãäåæçèéêëìíîïğñòóôõöøùúûüışÿ";
WCHAR pStrAlphaW[]=L"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyzÀÁÂÃÄÅÆÇÈÉÊËÌÍÎÏĞÑÒÓÔÕÖØÙÚÛÜİŞßàáâãäåæçèéêëìíîïğñòóôõöøùúûüışÿ";
CHAR pStrCapitalA[]="ABCDEFGHIJKLMNOPQRSTUVWXYZÀÁÂÃÄÅÆÇÈÉÊËÌÍÎÏĞÑÒÓÔÕÖØÙÚÛÜİŞ";
WCHAR pStrCapitalW[]=L"ABCDEFGHIJKLMNOPQRSTUVWXYZÀÁÂÃÄÅÆÇÈÉÊËÌÍÎÏĞÑÒÓÔÕÖØÙÚÛÜİŞ";
CHAR pStrSmallA[]="abcdefghijklmnopqrstuvwxyzàáâãäåæçèéêëìíîïğñòóôõöøùúûüışßÿ";
WCHAR pStrSmallW[]=L"abcdefghijklmnopqrstuvwxyzàáâãäåæçèéêëìíîïğñòóôõöøùúûüışßÿ";
#else
CHAR pStrAlphaA[]="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
WCHAR pStrAlphaW[]=L"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
CHAR pStrCapitalA[]="ABCDEFGHIJKLMNOPQRSTUVWXYZ";
WCHAR pStrCapitalW[]=L"ABCDEFGHIJKLMNOPQRSTUVWXYZ";
CHAR pStrSmallA[]="abcdefghijklmnopqrstuvwxyz";
WCHAR pStrSmallW[]=L"abcdefghijklmnopqrstuvwxyz";
#endif


//========
// Common
//========

BOOL CharIsAlpha(CHAR c)
{
for(UINT u=0; u<ARRAYSIZE(pStrAlphaA); u++)
	{
	if(c==pStrAlphaA[u])
		return true;
	}
return false;
}

BOOL CharIsAlpha(WCHAR c)
{
for(UINT u=0; u<ARRAYSIZE(pStrAlphaW); u++)
	{
	if(c==pStrAlphaW[u])
		return true;
	}
return false;
}

BOOL CharIsBreak(CHAR c)
{
for(UINT u=0; u<ARRAYSIZE(pStrBreakA); u++)
	{
	if(c==pStrBreakA[u])
		return true;
	}
return false;
}

BOOL CharIsBreak(WCHAR c)
{
for(UINT u=0; u<ARRAYSIZE(pStrBreakW); u++)
	{
	if(c==pStrBreakW[u])
		return true;
	}
return false;
}

BOOL CharIsCapital(CHAR c)
{
for(UINT u=0; u<ARRAYSIZE(pStrCapitalA); u++)
	{
	if(c==pStrCapitalA[u])
		return true;
	}
return false;
}

BOOL CharIsCapital(WCHAR c)
{
for(UINT u=0; u<ARRAYSIZE(pStrCapitalW); u++)
	{
	if(c==pStrCapitalW[u])
		return true;
	}
return false;
}

BOOL CharIsDigit(CHAR c, BOOL bhex)
{
if(bhex)
	return CharIsHex(c);
for(UINT u=0; u<ARRAYSIZE(pStrDigitA); u++)
	{
	if(c==pStrDigitA[u])
		return true;
	}
return false;
}

BOOL CharIsDigit(WCHAR c, BOOL bhex)
{
if(bhex)
	return CharIsHex(c);
for(UINT u=0; u<ARRAYSIZE(pStrDigitW); u++)
	{
	if(c==pStrDigitW[u])
		return true;
	}
return false;
}

BOOL CharIsHex(CHAR c)
{
for(UINT u=0; u<ARRAYSIZE(pStrHexA); u++)
	{
	if(c==pStrHexA[u])
		return true;
	}
return false;
}

BOOL CharIsHex(WCHAR c)
{
for(UINT u=0; u<ARRAYSIZE(pStrHexW); u++)
	{
	if(c==pStrHexW[u])
		return true;
	}
return false;
}

BOOL CharIsSmall(CHAR c)
{
for(UINT u=0; u<ARRAYSIZE(pStrSmallA); u++)
	{
	if(c==pStrSmallA[u])
		return true;
	}
return false;
}

BOOL CharIsSmall(WCHAR c)
{
for(UINT u=0; u<ARRAYSIZE(pStrSmallW); u++)
	{
	if(c==pStrSmallW[u])
		return true;
	}
return false;
}

BOOL CharIsSpecial(CHAR c)
{
for(UINT u=0; u<ARRAYSIZE(pStrSpecialA); u++)
	{
	if(c==pStrSpecialA[u])
		return true;
	}
return false;
}

BOOL CharIsSpecial(WCHAR c)
{
for(UINT u=0; u<ARRAYSIZE(pStrSpecialW); u++)
	{
	if(c==pStrSpecialW[u])
		return true;
	}
return false;
}


//============
// Conversion
//============

CHAR pStrAnsi[]="";
WCHAR pStrUnicode[]=L"";

CHAR CharToAnsi(WCHAR c)
{
for(UINT u=0; u<ARRAYSIZE(pStrUnicode); u++)
	{
	if(c==pStrUnicode[u])
		return pStrAnsi[u];
	}
if(c>0xFF)
	return cUnknown;
return (CHAR)c;
}

CHAR CharToCapital(CHAR c)
{
for(UINT u=0; u<ARRAYSIZE(pStrSmallA); u++)
	{
	if(c==pStrSmallA[u])
		return pStrCapitalA[u];
	}
return c;
}

WCHAR CharToCapital(WCHAR c)
{
for(UINT u=0; u<ARRAYSIZE(pStrSmallW); u++)
	{
	if(c==pStrSmallW[u])
		return pStrCapitalW[u];
	}
return c;
}

CHAR CharToSmall(CHAR c)
{
for(UINT u=0; u<ARRAYSIZE(pStrCapitalA); u++)
	{
	if(c==pStrCapitalA[u])
		return pStrSmallA[u];
	}
return c;
}

WCHAR CharToSmall(WCHAR c)
{
for(UINT u=0; u<ARRAYSIZE(pStrCapitalW); u++)
	{
	if(c==pStrCapitalW[u])
		return pStrSmallW[u];
	}
return c;
}

WCHAR CharToUnicode(CHAR c)
{
for(UINT u=0; u<ARRAYSIZE(pStrAnsi); u++)
	{
	if(c==pStrAnsi[u])
		return pStrUnicode[u];
	}
return c;
}


//============
// Comparison
//============

INT CharCompare(CHAR c1, CHAR c2, BOOL bcs)
{
CHAR c1i=c1;
CHAR c2i=c2;
if(!bcs)
	{
	c1i=CharToCapital(c1);
	c2i=CharToCapital(c2);
	}
UINT upos1=0;
for(; upos1<ARRAYSIZE(pStrSortA); upos1++)
	{
	if(c1i==pStrSortA[upos1])
		break;
	}
UINT upos2=0;
for(; upos2<ARRAYSIZE(pStrSortA); upos2++)
	{
	if(c2i==pStrSortA[upos2])
		break;
	}
if(upos1==upos2)
	{
	if(c1i>c2i)
		return 1;
	if(c2i>c1i)
		return -1;
	return 0;
	}
if(upos1>upos2)
	return 1;
if(upos2>upos1)
	return -1;
return 0;
}

INT CharCompare(WCHAR c1, WCHAR c2, BOOL bcs)
{
WCHAR c1i=c1;
WCHAR c2i=c2;
if(!bcs)
	{
	c1i=CharToCapital(c1);
	c2i=CharToCapital(c2);
	}
UINT upos1=0;
for(; upos1<ARRAYSIZE(pStrSortW); upos1++)
	{
	if(c1i==pStrSortW[upos1])
		break;
	}
UINT upos2=0;
for(; upos2<ARRAYSIZE(pStrSortW); upos2++)
	{
	if(c2i==pStrSortW[upos2])
		break;
	}
if(upos1==upos2)
	{
	if(c1i>c2i)
		return 1;
	if(c2i>c1i)
		return -1;
	return 0;
	}
if(upos1>upos2)
	return 1;
if(upos2>upos1)
	return -1;
return 0;
}
