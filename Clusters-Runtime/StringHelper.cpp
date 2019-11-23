//==================
// StringHelper.cpp
//==================

#include "pch.h"


//=======
// Using
//=======

#include "StringHelper.h"


//========
// Common
//========

WCHAR pStrSort[]=L"0123456789AaBbCcDdEeFfGgHhIiJjKkLlMmNnOoPpQqRrSsTtUuVvWwXxYyZz";

INT CharCompare(WCHAR c1, WCHAR c2)
{
UINT upos1=0;
for(; upos1<ARRAYSIZE(pStrSort); upos1++)
	{
	if(c1==pStrSort[upos1])
		break;
	}
UINT upos2=0;
for(; upos2<ARRAYSIZE(pStrSort); upos2++)
	{
	if(c2==pStrSort[upos2])
		break;
	}
if(upos1==upos2)
	{
	if(c1>c2)
		return 1;
	if(c2>c1)
		return -1;
	return 0;
	}
if(upos1>upos2)
	return 1;
if(upos2>upos1)
	return -1;
return 0;
}

INT StringCompare(LPCWSTR pstr1, LPCWSTR pstr2)
{
UINT ulen1=wcslen(pstr1);
UINT ulen2=wcslen(pstr2);
if(!ulen1&&!ulen2)
	return 0;
if(!ulen1)
	return -1;
if(!ulen2)
	return 1;
UINT upos1=0;
UINT upos2=0;
for(; upos1<ulen1&&upos2<ulen2; )
	{
	UINT udlen1=ulen1-upos1;
	UINT udlen2=ulen2-upos2;
	DOUBLE d1=StringToDouble(&pstr1[upos1], &udlen1);
	DOUBLE d2=StringToDouble(&pstr2[upos2], &udlen2);
	if(udlen1==0)
		{
		if(udlen2==0)
			{
			INT icmp=CharCompare(pstr1[upos1], pstr2[upos2]);
			if(icmp==0)
				{
				upos1++;
				upos2++;
				continue;
				}
			return icmp;
			}
		return -1;
		}
	if(udlen2==0)
		return 1;
	if(d1>d2)
		return 1;
	if(d2>d1)
		return -1;
	upos1+=udlen1;
	upos2+=udlen2;
	}
if(upos1==ulen1)
	{
	if(upos2==ulen2)
		return 0;
	return -1;
	}
return 1;
}

DOUBLE StringToDouble(LPCWSTR pstr, UINT* plen)
{
WCHAR ptmp[32];
if(plen&&*plen)
	{
	wcscpy_s(ptmp, 32, pstr);
	pstr=ptmp;
	}
LPWSTR pend=nullptr;
DOUBLE d=wcstod(pstr, &pend);
if(plen)
	*plen=(UINT)(pend-pstr)/sizeof(WCHAR);
return d;
}
