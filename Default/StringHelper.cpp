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

template <class T, class C>
BOOL StringContainsChar(T const* pstr, C c, UINT ustart, UINT ufind, BOOL bcs)
{
if(!pstr||!c)
	return false;
UINT ulen=StringLength(pstr);
UINT uend=ulen;
if(ufind>0)
	uend=MIN(ustart+ufind, ulen);
for(UINT u=ustart; u<uend; u++)
	{
	if(CharCompare(pstr[u], c, bcs)==0)
		return true;
	}
return false;
}

template <class T, class V>
BOOL StringContainsString(T const* pstr, V const* pfind, UINT ustart, UINT ufind, BOOL bcs)
{
if(!pstr||!pfind)
	return false;
UINT ulen=StringLength(pstr);
UINT uend=ulen;
if(ufind>0)
	uend=MIN(ustart+ufind, ulen);
UINT ufindlen=StringLength(pfind);
if(!ufindlen||uend<ufindlen-1)
	return false;
uend-=ufindlen-1;
for(UINT u=ustart; u<uend; u++)
	{
	if(StringCompare(&pstr[u], pfind, ufindlen, bcs)==0)
		return true;
	}
return false;
}

BOOL StringContains(LPCSTR pstr, CHAR c, UINT ustart, UINT ulen, BOOL bcs)
{
return StringContainsChar<CHAR, CHAR>(pstr, c, ustart, ulen, bcs);
}

BOOL StringContains(LPCSTR pstr, WCHAR c, UINT ustart, UINT ulen, BOOL bcs)
{
return StringContainsChar<CHAR, WCHAR>(pstr, c, ustart, ulen, bcs);
}

BOOL StringContains(LPCSTR pstr, LPCSTR pfind, UINT ustart, UINT ulen, BOOL bcs)
{
return StringContainsString<CHAR, CHAR>(pstr, pfind, ustart, ulen, bcs);
}

BOOL StringContains(LPCSTR pstr, LPCWSTR pfind, UINT ustart, UINT ulen, BOOL bcs)
{
return StringContainsString<CHAR, WCHAR>(pstr, pfind, ustart, ulen, bcs);
}

BOOL StringContains(LPCWSTR pstr, CHAR c, UINT ustart, UINT ulen, BOOL bcs)
{
return StringContainsChar<WCHAR, CHAR>(pstr, c, ustart, ulen, bcs);
}

BOOL StringContains(LPCWSTR pstr, WCHAR c, UINT ustart, UINT ulen, BOOL bcs)
{
return StringContainsChar<WCHAR, WCHAR>(pstr, c, ustart, ulen, bcs);
}

BOOL StringContains(LPCWSTR pstr, LPCSTR pfind, UINT ustart, UINT ulen, BOOL bcs)
{
return StringContainsString<WCHAR, CHAR>(pstr, pfind, ustart, ulen, bcs);
}

BOOL StringContains(LPCWSTR pstr, LPCWSTR pfind, UINT ustart, UINT ulen, BOOL bcs)
{
return StringContainsString<WCHAR, WCHAR>(pstr, pfind, ustart, ulen, bcs);
}

template <class T1, class T2>
UINT StringCount(const T1* pstr, const T2* pfind, UINT ustart, UINT ufind, BOOL bcs)
{
UINT ulen=StringLength(pstr);
UINT ufindlen=StringLength(pfind);
if(!ulen||!ufindlen||ulen<ufindlen)
	return 0;
UINT uend=ulen;
if(ufind>0)
	uend=MIN(ustart+ufind, ulen);
uend-=ufindlen-1;
UINT ucount=0;
for(UINT u=ustart; u<uend; u++)
	{
	if(StringCompare(&pstr[u], pfind, ufindlen, bcs)==0)
		{
		ucount++;
		u+=ufindlen-1;
		}
	}
return ucount;
}

UINT StringCount(LPCSTR pstr, LPCSTR pfind, UINT ustart, UINT ulen, BOOL bcs)
{
return StringCount<CHAR, CHAR>(pstr, pfind, ustart, ulen, bcs);
}

UINT StringCount(LPCSTR pstr, LPCWSTR pfind, UINT ustart, UINT ulen, BOOL bcs)
{
return StringCount<CHAR, WCHAR>(pstr, pfind, ustart, ulen, bcs);
}

UINT StringCount(LPCWSTR pstr, LPCSTR pfind, UINT ustart, UINT ulen, BOOL bcs)
{
return StringCount<WCHAR, CHAR>(pstr, pfind, ustart, ulen, bcs);
}

UINT StringCount(LPCWSTR pstr, LPCWSTR pfind, UINT ustart, UINT ulen, BOOL bcs)
{
return StringCount<WCHAR, WCHAR>(pstr, pfind, ustart, ulen, bcs);
}

template <class T1, class T2>
BOOL StringFind(const T1* pstr, const T2* pfind, UINT* ppos, UINT umax, BOOL bcs)
{
UINT ulen=StringLength(pstr, umax);
UINT ufindlen=StringLength(pfind);
if(!ulen||!ufindlen||ulen<ufindlen)
	return false;
UINT upos=*ppos;
UINT uend=ulen-ufindlen+1;
for(; upos<uend; upos++)
	{
	if(StringCompare(&pstr[upos], pfind, ufindlen, bcs)==0)
		{
		*ppos=upos;
		return true;
		}
	}
return false;
}

BOOL StringFind(LPCSTR pstr, LPCSTR pfind, UINT* ppos, UINT umax, BOOL bcs)
{
return StringFind<CHAR, CHAR>(pstr, pfind, ppos, umax, bcs);
}

BOOL StringFind(LPCSTR pstr, LPCWSTR pfind, UINT* ppos, UINT umax, BOOL bcs)
{
return StringFind<CHAR, WCHAR>(pstr, pfind, ppos, umax, bcs);
}

BOOL StringFind(LPCWSTR pstr, LPCSTR pfind, UINT* ppos, UINT umax, BOOL bcs)
{
return StringFind<WCHAR, CHAR>(pstr, pfind, ppos, umax, bcs);
}

BOOL StringFind(LPCWSTR pstr, LPCWSTR pfind, UINT* ppos, UINT umax, BOOL bcs)
{
return StringFind<WCHAR, WCHAR>(pstr, pfind, ppos, umax, bcs);
}

UINT StringLength(LPCSTR pstr, UINT umax)
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

template <class T1, class T2>
INT StringCompare(const T1* pstr1, UINT umax1, const T2* pstr2, UINT umax2, BOOL bcs)
{
UINT ulen1=StringLength(pstr1);
UINT ulen2=StringLength(pstr2);
if(!ulen1&&!ulen2)
	return 0;
if(!ulen1)
	return -1;
if(!ulen2)
	return 1;
UINT umax=umax1;
if(umax2>0&&umax2<umax)
	umax=umax2;
UINT ulen=umax;
if(ulen==0)
	ulen=MIN(ulen1+1, ulen2+1);
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
		INT i=CharCompare(pstr1[upos1], pstr2[upos2], bcs);
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
if(umax)
	return 0;
if(pstr1[upos1]==0)
	{
	if(pstr2[upos2]==0)
		return 0;
	return -1;
	}
return 1;
}

INT StringCompare(LPCSTR pstr1, LPCSTR pstr2, UINT ulen, BOOL bcs)
{
return StringCompare<CHAR, CHAR>(pstr1, ulen, pstr2, ulen, bcs);
}

INT StringCompare(LPCSTR pstr1, LPCWSTR pstr2, UINT ulen, BOOL bcs)
{
return StringCompare<CHAR, WCHAR>(pstr1, ulen, pstr2, ulen, bcs);
}

INT StringCompare(LPCWSTR pstr1, LPCSTR pstr2, UINT ulen, BOOL bcs)
{
return StringCompare<WCHAR, CHAR>(pstr1, ulen, pstr2, ulen, bcs);
}

INT StringCompare(LPCWSTR pstr1, LPCWSTR pstr2, UINT ulen, BOOL bcs)
{
return StringCompare<WCHAR, WCHAR>(pstr1, ulen, pstr2, ulen, bcs);
}

INT StringCompare(LPCSTR pstr1, UINT ulen1, LPCSTR pstr2, UINT ulen2, BOOL bcs)
{
return StringCompare<CHAR, CHAR>(pstr1, ulen1, pstr2, ulen2, bcs);
}

INT StringCompare(LPCSTR pstr1, UINT ulen1, LPCWSTR pstr2, UINT ulen2, BOOL bcs)
{
return StringCompare<CHAR, WCHAR>(pstr1, ulen1, pstr2, ulen2, bcs);
}

INT StringCompare(LPCWSTR pstr1, UINT ulen1, LPCSTR pstr2, UINT ulen2, BOOL bcs)
{
return StringCompare<WCHAR, CHAR>(pstr1, ulen1, pstr2, ulen2, bcs);
}

INT StringCompare(LPCWSTR pstr1, UINT ulen1, LPCWSTR pstr2, UINT ulen2, BOOL bcs)
{
return StringCompare<WCHAR, WCHAR>(pstr1, ulen1, pstr2, ulen2, bcs);
}


//============
// Assignment
//============

template <class T, class V>
UINT StringAssign(T** ppstr, UINT* psize, V const* passign, UINT ucopy)
{
UINT ulen=StringLength(passign, ucopy);
if(ulen==0)
	return StringClear(ppstr, psize);
T* pstr=*ppstr;
UINT usize=0;
if(psize)
	usize=*psize;
if(ulen+1>usize)
	{
	if(pstr!=nullptr)
		delete pstr;
	usize=ulen+1;
	pstr=new T[usize];
	*ppstr=pstr;
	if(psize)
		*psize=usize;
	}
return StringCopy(pstr, usize, passign, ulen);
}

UINT StringAssign(LPSTR* ppstr, UINT* psize, LPCSTR passign, UINT ulen)
{
return StringAssign<CHAR, CHAR>(ppstr, psize, passign, ulen);
}

UINT StringAssign(LPSTR* ppstr, UINT* psize, LPCWSTR passign, UINT ulen)
{
return StringAssign<CHAR, WCHAR>(ppstr, psize, passign, ulen);
}

UINT StringAssign(LPWSTR* ppstr, UINT* psize, LPCSTR passign, UINT ulen)
{
return StringAssign<WCHAR, CHAR>(ppstr, psize, passign, ulen);
}

UINT StringAssign(LPWSTR* ppstr, UINT* psize, LPCWSTR passign, UINT ulen)
{
return StringAssign<WCHAR, WCHAR>(ppstr, psize, passign, ulen);
}

template <class DST, class SRC>
UINT StringCopy(DST* pdst, UINT usize, SRC const* psrc, UINT ucopy)
{
if(!pdst||!usize)
	return 0;
UINT uend=usize;
if(ucopy)
	uend=MIN(uend, ucopy);
if(!psrc)
	{
	pdst[0]=0;
	return 0;
	}
UINT upos=0;
for(; upos<uend; upos++)
	{
	if(psrc[upos]==0)
		break;
	CharAssign(pdst[upos], psrc[upos]);
	}
if(upos<usize)
	pdst[upos]=0;
return upos;
}

UINT StringCopy(LPSTR pdst, UINT usize, LPCSTR psrc, UINT ucopy)
{
return StringCopy<CHAR, CHAR>(pdst, usize, psrc, ucopy);
}

UINT StringCopy(LPSTR pdst, UINT usize, LPCWSTR psrc, UINT ucopy)
{
return StringCopy<CHAR, WCHAR>(pdst, usize, psrc, ucopy);
}

UINT StringCopy(LPWSTR pdst, UINT usize, LPCSTR psrc, UINT ucopy)
{
return StringCopy<WCHAR, CHAR>(pdst, usize, psrc, ucopy);
}

UINT StringCopy(LPWSTR pdst, UINT usize, LPCWSTR psrc, UINT ucopy)
{
return StringCopy<WCHAR, WCHAR>(pdst, usize, psrc, ucopy);
}


//==============
// Modification
//==============

template <class T, class V>
UINT StringAppend(T** ppstr, UINT* psize, const V* pappend, UINT ucopy)
{
T* pstr=*ppstr;
UINT usize=0;
if(psize)
	usize=*psize;
UINT ulen=StringLength(pstr);
UINT uappendlen=StringLength(pappend, ucopy);
UINT unewlen=ulen+uappendlen;
if(unewlen+1>usize)
	{
	StringSetSize(ppstr, psize, unewlen+1);
	pstr=*ppstr;
	usize=unewlen+1;
	}
StringCopy(&pstr[ulen], usize-ulen, pappend, ucopy);
return unewlen;
}

UINT StringAppend(LPSTR* ppstr, UINT* psize, LPCSTR pappend, UINT ulen)
{
return StringAppend<CHAR, CHAR>(ppstr, psize, pappend, ulen);
}

UINT StringAppend(LPSTR* ppstr, UINT* psize, LPCWSTR pappend, UINT ulen)
{
return StringAppend<CHAR, WCHAR>(ppstr, psize, pappend, ulen);
}

UINT StringAppend(LPWSTR* ppstr, UINT* psize, LPCSTR pappend, UINT ulen)
{
return StringAppend<WCHAR, CHAR>(ppstr, psize, pappend, ulen);
}

UINT StringAppend(LPWSTR* ppstr, UINT* psize, LPCWSTR pappend, UINT ulen)
{
return StringAppend<WCHAR, WCHAR>(ppstr, psize, pappend, ulen);
}

template <class T>
UINT StringClear(T** ppstr, UINT* psize)
{
T* pstr=*ppstr;
if(pstr!=nullptr)
	{
	delete pstr;
	*ppstr=nullptr;
	}
if(psize)
	*psize=0;
return 0;
}

UINT StringClear(LPSTR* ppstr, UINT* psize)
{
return StringClear<CHAR>(ppstr, psize);
}

UINT StringClear(LPWSTR* ppstr, UINT* psize)
{
return StringClear<WCHAR>(ppstr, psize);
}

template <class T, class V>
UINT StringInsert(T** ppstr, UINT* psize, const V* pinsert, UINT upos, UINT ucopy)
{
T* pstr=*ppstr;
UINT ulen=StringLength(pstr);
if(upos>ulen)
	throw(EINVAL);
UINT uinsertlen=StringLength(pinsert, ucopy);
if(uinsertlen==0)
	return 0;
UINT usize=0;
if(psize)
	usize=*psize;
UINT unewlen=ulen+uinsertlen;
if(unewlen+1>usize)
	{
	usize=unewlen+1;
	T* pnew=new T[usize];
	StringCopy(pnew, usize, pstr, upos);
	StringCopy(&pnew[ulen], usize-ulen, pinsert, uinsertlen);
	if(ulen>upos)
		StringCopy(&pnew[upos+uinsertlen], usize-upos-uinsertlen, &pstr[upos], ulen-upos);
	delete pstr;
	pstr=pnew;
	*ppstr=pstr;
	if(psize)
		*psize=usize;
	return uinsertlen;
	}
MoveMemory(&pstr[upos+uinsertlen], &pstr[upos], (ulen-upos)*sizeof(T));
StringCopy(&pstr[upos], uinsertlen, pinsert, uinsertlen);
return uinsertlen;
}

UINT StringInsert(LPSTR* ppstr, UINT* psize, LPCSTR pinsert, UINT upos, UINT ulen)
{
return StringInsert<CHAR, CHAR>(ppstr, psize, pinsert, upos, ulen);
}

UINT StringInsert(LPSTR* ppstr, UINT* psize, LPCWSTR pinsert, UINT upos, UINT ulen)
{
return StringInsert<CHAR, WCHAR>(ppstr, psize, pinsert, upos, ulen);
}

UINT StringInsert(LPWSTR* ppstr, UINT* psize, LPCSTR pinsert, UINT upos, UINT ulen)
{
return StringInsert<WCHAR, CHAR>(ppstr, psize, pinsert, upos, ulen);
}

UINT StringInsert(LPWSTR* ppstr, UINT* psize, LPCWSTR pinsert, UINT upos, UINT ulen)
{
return StringInsert<WCHAR, WCHAR>(ppstr, psize, pinsert, upos, ulen);
}

template <class T>
UINT StringLowerCase(T* pstr, UINT ustart, UINT ufind)
{
UINT ulen=StringLength(pstr);
	return ulen;
UINT uend=ulen;
if(ufind>0)
	uend=MIN(ustart+ufind, ulen);
for(UINT u=ustart; u<uend; u++)
	pstr[u]=CharToSmall(pstr[u]);
return ulen;
}

UINT StringLowerCase(LPSTR pstr, UINT ustart, UINT ulen)
{
return StringLowerCase<CHAR>(pstr, ustart, ulen);
}

UINT StringLowerCase(LPWSTR pstr, UINT ustart, UINT ulen)
{
return StringLowerCase<WCHAR>(pstr, ustart, ulen);
}

template <class T>
UINT StringRemove(T* pstr, UINT upos, UINT usize)
{
UINT ulen=StringLength(pstr);
if(!usize)
	return ulen;
if(upos>=ulen)
	return ulen;
UINT umove=(UINT)((ulen-upos-usize+1)*sizeof(T));
MoveMemory(&pstr[upos], &pstr[upos+usize], umove);
return ulen-usize;
}

UINT StringRemove(LPSTR pstr, UINT upos, UINT usize)
{
return StringRemove<CHAR>(pstr, upos, usize);
}

UINT StringRemove(LPWSTR pstr, UINT upos, UINT usize)
{
return StringRemove<WCHAR>(pstr, upos, usize);
}

template <class T, class V>
UINT StringReplace(T** ppstr, UINT* psize, const V* pfind, const V* pinsert, UINT ustart, UINT ufind, BOOL bcs, BOOL ball)
{
T* pstr=*ppstr;
UINT ulen=StringLength(pstr);
if(ustart>=ulen)
	return ulen;
UINT ufindlen=StringLength(pfind);
if(ufindlen==0)
	return ulen;
UINT uinsertlen=StringLength(pinsert);
UINT usearchlen=ulen-ustart;
if(ufind>0)
	usearchlen=MIN(ufind, usearchlen);
UINT ucount=StringCount(&pstr[ustart], pfind, usearchlen, bcs);
if(!ucount)
	return ulen;
if(!ball)
	ucount=1;
INT idiflen=(INT)uinsertlen-ufindlen;
UINT unewlen=ulen+idiflen*ucount;
UINT unewsize=unewlen+1;
T* pnew=new T[unewsize];
StringCopy(pnew, ustart, pstr, ustart);
UINT upos=ustart;
UINT unewpos=ustart;
for(UINT u=0; u<ucount; u++)
	{
	if(StringCompare(&pstr[upos], pfind, ufindlen, bcs)==0)
		{
		StringCopy(&pnew[unewpos], uinsertlen+1, pinsert, uinsertlen);
		upos+=ufindlen;
		unewpos+=uinsertlen;
		continue;
		}
	pnew[unewpos]=pstr[upos];
	upos++;
	unewpos++;
	}
StringCopy(&pnew[unewpos], unewsize-unewpos, &pstr[upos], ulen-upos);
pnew[unewlen]=0;
UINT usize=0;
if(psize)
	usize=*psize;
if(unewsize>usize)
	{
	delete pstr;
	*ppstr=pnew;
	if(psize)
		*psize=unewsize;
	return unewlen;
	}
StringCopy(pstr, usize, pnew);
return unewlen;
}

UINT StringReplace(LPSTR* ppstr, UINT* psize, LPCSTR pfind, LPCSTR pinsert, UINT ustart, UINT ulen, BOOL bcs, BOOL ball)
{
return StringReplace<CHAR, CHAR>(ppstr, psize, pfind, pinsert, ustart, ulen, bcs, ball);
}

UINT StringReplace(LPSTR* ppstr, UINT* psize, LPCWSTR pfind, LPCWSTR pinsert, UINT ustart, UINT ulen, BOOL bcs, BOOL ball)
{
return StringReplace<CHAR, WCHAR>(ppstr, psize, pfind, pinsert, ustart, ulen, bcs, ball);
}

UINT StringReplace(LPWSTR* ppstr, UINT* psize, LPCSTR pfind, LPCSTR pinsert, UINT ustart, UINT ulen, BOOL bcs, BOOL ball)
{
return StringReplace<WCHAR, CHAR>(ppstr, psize, pfind, pinsert, ustart, ulen, bcs, ball);
}

UINT StringReplace(LPWSTR* ppstr, UINT* psize, LPCWSTR pfind, LPCWSTR pinsert, UINT ustart, UINT ulen, BOOL bcs, BOOL ball)
{
return StringReplace<WCHAR, WCHAR>(ppstr, psize, pfind, pinsert, ustart, ulen, bcs, ball);
}

template <class T>
VOID StringSetSize(T** ppstr, UINT* psize, UINT unewsize)
{
UINT usize=0;
if(psize)
	usize=*psize;
if(unewsize==usize)
	return;
if(unewsize==0)
	{
	StringClear(ppstr, psize);
	return;
	}
T* pstr=*ppstr;
T* pnew=new T[unewsize];
if(pstr)
	{
	StringCopy(pnew, unewsize, pstr);
	pnew[unewsize-1]=0;
	delete pstr;
	}
else
	{
	pnew[0]=0;
	}
*ppstr=pnew;
if(psize)
	*psize=unewsize;
}

VOID StringSetSize(LPSTR* ppstr, UINT* psize, UINT unewsize)
{
StringSetSize<CHAR>(ppstr, psize, unewsize);
}

VOID StringSetSize(LPWSTR* ppstr, UINT* psize, UINT unewsize)
{
StringSetSize<WCHAR>(ppstr, psize, unewsize);
}

template <class T>
UINT StringUpperCase(T* pstr, UINT ustart, UINT ufind)
{
UINT ulen=StringLength(pstr);
if(ustart>ulen)
	return ulen;
UINT uend=ulen;
if(ufind>0)
	uend=MIN(ustart+ufind, ulen);
for(UINT u=ustart; u<uend; u++)
	pstr[u]=CharToCapital(pstr[u]);
return ulen;
}

UINT StringUpperCase(LPSTR pstr, UINT ustart, UINT ulen)
{
return StringLowerCase<CHAR>(pstr, ustart, ulen);
}

UINT StringUpperCase(LPWSTR pstr, UINT ustart, UINT ulen)
{
return StringLowerCase<WCHAR>(pstr, ustart, ulen);
}


//============
// Conversion
//============

template <class T>
BOOL StringIsNumeric(const T* pstr, UINT* plen)
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

BOOL StringIsNumeric(LPCSTR pstr, UINT* plen)
{
return StringIsNumeric<CHAR>(pstr, plen);
}

BOOL StringIsNumeric(LPCWSTR pstr, UINT* plen)
{
return StringIsNumeric<WCHAR>(pstr, plen);
}

template <class T, class P>
P StringToFloat(const T* pstr, UINT* plen)
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
P fvalue=0;
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
		fvalue+=((P)pstr[upos]-'0')/(FLOAT)umul;
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
	P fex=0;
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

DOUBLE StringToDouble(LPCSTR pstr, UINT* plen)
{
return StringToFloat<CHAR, DOUBLE>(pstr, plen);
}

DOUBLE StringToDouble(LPCWSTR pstr, UINT* plen)
{
return StringToFloat<WCHAR, DOUBLE>(pstr, plen);
}

FLOAT StringToFloat(LPCSTR pstr, UINT* plen)
{
return StringToFloat<CHAR, FLOAT>(pstr, plen);
}

FLOAT StringToFloat(LPCWSTR pstr, UINT* plen)
{
return StringToFloat<WCHAR, FLOAT>(pstr, plen);
}

INT StringToInt(LPCSTR pstr, UINT* plen)
{
return (INT)StringToFloat<CHAR, FLOAT>(pstr, plen);
}

INT StringToInt(LPCWSTR pstr, UINT* plen)
{
return (INT)StringToFloat<WCHAR, FLOAT>(pstr, plen);
}

INT64 StringToInt64(LPCSTR pstr, UINT* plen)
{
return (INT64)StringToFloat<CHAR, DOUBLE>(pstr, plen);
}

INT64 StringToInt64(LPCWSTR pstr, UINT* plen)
{
return (INT64)StringToFloat<WCHAR, DOUBLE>(pstr, plen);
}
