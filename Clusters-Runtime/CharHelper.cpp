//================
// CharHelper.cpp
//================

#include "pch.h"


//=======
// Using
//=======

#include "CharHelper.h"


//=========
// Strings
//=========

WCHAR pStrAlpha[]=L"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
WCHAR pStrCapital[]=L"ABCDEFGHIJKLMNOPQRSTUVWXYZÀÁÂÃÄÅÆÇÈÉÊËÌÍÎÏĞÑÒÓÔÕÖØÙÚÛÜİŞ";
WCHAR pStrDigit[]=L"0123456789";
WCHAR pStrHex[]=L"0123456789ABCDEFabcdef";
WCHAR pStrSmall[]=L"abcdefghijklmnopqrstuvwxyzàáâãäåæçèéêëìíîïğñòóôõöøùúûüışßÿ";
WCHAR pStrSort[]=L"0123456789AaBbCcDdEeFfGgHhIiJjKkLlMmNnOoPpQqRrSsTtUuVvWwXxYyZz";


//========
// Common
//========

BOOL CharIsAlpha(WCHAR c)
{
for(UINT u=0; u<ARRAYSIZE(pStrAlpha); u++)
	{
	if(c==pStrAlpha[u])
		return true;
	}
return false;
}

BOOL CharIsDigit(WCHAR c, BOOL bhex)
{
if(bhex)
	return CharIsHex(c);
for(UINT u=0; u<ARRAYSIZE(pStrDigit); u++)
	{
	if(c==pStrDigit[u])
		return true;
	}
return false;
}

BOOL CharIsHex(WCHAR c)
{
for(UINT u=0; u<ARRAYSIZE(pStrHex); u++)
	{
	if(c==pStrHex[u])
		return true;
	}
return false;
}


//============
// Conversion
//============

WCHAR CharToCapital(WCHAR c)
{
for(UINT u=0; u<ARRAYSIZE(pStrSmall); u++)
	{
	if(c==pStrSmall[u])
		return pStrCapital[u];
	}
return c;
}


//============
// Comparison
//============

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
for(; upos1<ARRAYSIZE(pStrSort); upos1++)
	{
	if(c1i==pStrSort[upos1])
		break;
	}
UINT upos2=0;
for(; upos2<ARRAYSIZE(pStrSort); upos2++)
	{
	if(c2i==pStrSort[upos2])
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
