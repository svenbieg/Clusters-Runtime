//==================
// BufferHelper.cpp
//==================

#include "pch.h"


//=======
// Using
//=======

#include "BufferHelper.h"


//============
// Assignment
//============

SIZE_T BufferAssign(BYTE** ppbuf, SIZE_T* psize, VOID const* psrc, SIZE_T usrcsize)
{
ASSERT(ppbuf&&psize);
operator delete(*ppbuf);
if(!psrc||!usrcsize)
	{
	*psize=0;
	return 0;
	}
*ppbuf=(BYTE*)operator new(usrcsize);
CopyMemory(*ppbuf, psrc, usrcsize);
*psize=usrcsize;
return usrcsize;
}


//==============
// Modification
//==============

SIZE_T BufferAppend(BYTE** ppbuf, SIZE_T* psize, VOID const* psrc, SIZE_T usrcsize)
{
SIZE_T usize=*psize;
SIZE_T unewsize=usize+usrcsize;
BufferSetSize(ppbuf, psize, unewsize);
BYTE* pbuf=*ppbuf;
CopyMemory(&pbuf[usize], psrc, usrcsize);
return unewsize;
}

SIZE_T BufferInsert(BYTE** ppbuf, SIZE_T* psize, SIZE_T upos, VOID const* psrc, SIZE_T usrcsize)
{
SIZE_T usize=*psize;
SIZE_T unewsize=usize+usrcsize;
BYTE* pnew=(BYTE*)operator new(unewsize);
BYTE* pbuf=*ppbuf;
CopyMemory(pnew, pbuf, upos);
CopyMemory(&pnew[upos], psrc, usrcsize);
CopyMemory(&pnew[upos+usrcsize], &pbuf[upos], usize-upos);
operator delete(pbuf);
*ppbuf=pnew;
return usrcsize;
}

SIZE_T BufferRemove(BYTE** ppbuf, SIZE_T* psize, SIZE_T upos, SIZE_T urem)
{
BYTE* pbuf=*ppbuf;
SIZE_T usize=*psize;
ASSERT(upos+urem<=usize);
MoveMemory(&pbuf[upos], &pbuf[upos+urem], usize-urem-upos);
SIZE_T unewsize=usize-urem;
BufferSetSize(ppbuf, psize, unewsize);
return unewsize;
}

VOID BufferSetSize(BYTE** ppbuf, SIZE_T* psize, SIZE_T unewsize)
{
ASSERT(ppbuf&&psize);
if(unewsize==*psize)
	return;
BYTE* pbuf=*ppbuf;
if(!pbuf)
	{
	if(!unewsize)
		return;
	*ppbuf=(BYTE*)operator new(unewsize);
	*psize=unewsize;
	return;
	}
*ppbuf=(BYTE*)operator new(unewsize);
SIZE_T ucopy=min(*psize, unewsize);
CopyMemory(*ppbuf, pbuf, ucopy);
operator delete(pbuf);
*psize=unewsize;
}

SIZE_T BufferWrite(BYTE** ppbuf, SIZE_T* psize, SIZE_T upos, VOID const* psrc, SIZE_T usrcsize)
{
SIZE_T usize=*psize;
SIZE_T uend=upos+usrcsize;
if(uend>usize)
	BufferSetSize(ppbuf, psize, uend);
BYTE* pbuf=*ppbuf;
CopyMemory(&pbuf[upos], psrc, usrcsize);
return usrcsize;
}
