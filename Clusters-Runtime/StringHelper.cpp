//==================
// StringHelper.cpp
//==================

#include "pch.h"


//=======
// Using
//=======

#include "CharHelper.h"
#include "StringHelper.h"


//========
// Common
//========

UINT StringLength(LPCWSTR pstr, UINT umax)
{
if(!pstr)
	return 0;
if(!umax)
	umax=MAXUINT;
for(UINT u=0; u<umax; u++)
	{
	if(pstr[u]==0)
		return u;
	}
return umax;
}


//============
// Comparison
//============

INT StringCompare(LPCWSTR pstr1, LPCWSTR pstr2)
{
UINT ulen1=StringLength(pstr1);
UINT ulen2=StringLength(pstr2);
if(!ulen1&&!ulen2)
	return 0;
if(!ulen1)
	return -1;
if(!ulen2)
	return 1;
UINT ulen=min(ulen1+1, ulen2+1);
UINT upos1=0;
UINT upos2=0;
for(; upos1<ulen&&upos2<ulen; )
	{
	if(!pstr1[upos1])
		{
		if(!pstr2[upos2])
			return 0;
		return -1;
		}
	if(!pstr2[upos2])
		return 1;
	if(CharIsAlpha(pstr1[upos1]))
		{
		if(!CharIsAlpha(pstr2[upos2]))
			return 1;
		INT i=CharCompare(pstr1[upos1], pstr2[upos2], false);
		if(i==0)
			{
			upos1++;
			upos2++;
			continue;
			}
		return i;
		}
	if(CharIsAlpha(pstr2[upos2]))
		return -1;
	UINT unum1=0;
	BOOL bnum1=StringIsNumeric(&pstr1[upos1], &unum1);
	UINT unum2=0;
	BOOL bnum2=StringIsNumeric(&pstr2[upos2], &unum2);
	if(bnum1)
		{
		if(bnum2)
			{
			DOUBLE d1=StringToDouble(&pstr1[upos1]);
			DOUBLE d2=StringToDouble(&pstr2[upos2]);
			if(d1>d2)
				return 1;
			if(d2>d1)
				return -1;
			upos1+=unum1;
			upos2+=unum2;
			continue;
			}
		return -1;
		}
	if(bnum2)
		return 1;
	if(pstr1[upos1]>pstr2[upos2])
		return 1;
	if(pstr2[upos2]>pstr1[upos1])
		return -1;
	upos1++;
	upos2++;
	}
if(pstr1[upos1]==0)
	{
	if(pstr2[upos2]==0)
		return 0;
	return -1;
	}
return 1;
}


//============
// Conversion
//============

BOOL StringIsNumeric(LPCWSTR pstr, UINT* plen)
{
if(!pstr)
	return false;
UINT umax=0;
if(plen)
	umax=*plen;
UINT ulen=StringLength(pstr, umax);
UINT upos=0;
BOOL bneg=false;
for(UINT upos=0; upos<ulen; upos++)
	{
	if(pstr[upos]!='-')
		break;
	bneg=!bneg;
	}
BOOL bhex=false;
if(upos+1<ulen)
	{
	if(pstr[upos]=='0')
		{
		if(pstr[upos+1]=='x')
			{
			bhex=true;
			upos+=2;
			}
		}
	}
if(upos==ulen)
	return false;
BOOL bpt=false;
if(pstr[upos]=='.')
	{
	if(bhex)
		return false;
	bpt=true;
	upos++;
	}
if(upos==ulen)
	return false;
if(!CharIsDigit(pstr[upos], bhex))
	return false;
for(upos++; upos<ulen; upos++)
	{
	if(!CharIsDigit(pstr[upos], bhex))
		break;
	}
if(upos==ulen||bhex||bpt)
	{
	if(plen)
		*plen=upos;
	return true;
	}
if(pstr[upos]=='.')
	{
	upos++;
	for(; upos<ulen; upos++)
		{
		if(!CharIsDigit(pstr[upos]))
			break;
		}
	}
if(upos==ulen)
	{
	if(plen)
		*plen=upos;
	return true;
	}
if(pstr[upos]=='e'||pstr[upos]=='E')
	{
	upos++;
	if(pstr[upos]=='-')
		upos++;
	if(!CharIsDigit(pstr[upos]))
		return false;
	for(upos++; upos<ulen; upos++)
		{
		if(!CharIsDigit(pstr[upos]))
			break;
		}
	}
if(plen)
	*plen=upos;
return true;
}

DOUBLE StringToDouble(LPCWSTR pstr, UINT* plen)
{
if(!pstr)
	return 0;
UINT umax=0;
if(plen)
	umax=*plen;
UINT ulen=StringLength(pstr, umax);
UINT upos=0;
BOOL bneg=false;
for(UINT upos=0; upos<ulen; upos++)
	{
	if(pstr[upos]!='-')
		break;
	bneg=!bneg;
	}
BOOL bhex=false;
UINT umul=10;
if(upos+1<ulen)
	{
	if(pstr[upos]=='0')
		{
		if(pstr[upos+1]=='x')
			{
			bhex=true;
			umul=16;
			upos+=2;
			}
		}
	}
if(upos==ulen)
	return 0;
BOOL bpt=false;
if(pstr[upos]=='.')
	{
	if(bhex)
		return 0;
	bpt=true;
	upos++;
	}
if(upos==ulen)
	return 0;
if(!CharIsDigit(pstr[upos], bhex))
	return 0;
DOUBLE fvalue=0;
for(; upos<ulen; upos++)
	{
	if(!CharIsDigit(pstr[upos], bhex))
		break;
	WCHAR wc=pstr[upos]-'0';
	if(bhex)
		{
		if(wc>9)
			wc-=8;
		}
	fvalue*=(FLOAT)umul;
	fvalue+=(FLOAT)wc;
	}
if(upos==ulen||bhex||bpt)
	{
	if(bneg)
		fvalue*=-1;
	if(plen)
		*plen=upos;
	return fvalue;
	}
if(pstr[upos]=='.')
	{
	upos++;
	for(; upos<ulen; upos++)
		{
		if(!CharIsDigit(pstr[upos]))
			break;
		fvalue+=(DOUBLE)(pstr[upos]-'0')/umul;
		umul*=10;
		}
	}
if(upos==ulen)
	{
	if(bneg)
		fvalue*=-1;
	if(plen)
		*plen=upos;
	return fvalue;
	}
if(pstr[upos]=='e'||pstr[upos]=='E')
	{
	upos++;
	BOOL bdiv=false;
	if(pstr[upos]=='-')
		{
		upos++;
		bdiv=true;
		}
	if(!CharIsDigit(pstr[upos]))
		return 0;
	DOUBLE fex=0;
	for(upos++; upos<ulen; upos++)
		{
		if(!CharIsDigit(pstr[upos]))
			break;
		fex*=10;
		fex+=(FLOAT)(pstr[upos]-'0');
		}
	fex*=10;
	if(bdiv)
		fex=1/fex;
	fvalue*=fex;
	}
if(bneg)
	fvalue*=-1;
if(plen)
	*plen=upos;
return fvalue;
}
