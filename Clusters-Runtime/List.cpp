//==========
// List.cpp
//==========

#include "pch.h"


//=======
// Using
//=======

#include "List.h"

using ListIterator=Clusters::Details::List::ListIterator;


//===========
// Namespace
//===========

namespace Clusters {


//========
// Access
//========

ListIterator^ List::At(UINT64 upos)
{
ScopedLock lock(cCriticalSection);
return ref new ListIterator(this, upos);
}

UINT64 List::Count::get()
{
ScopedLock lock(cCriticalSection);
return cList.GetCount();
}

ListIterator^ List::First()
{
ScopedLock lock(cCriticalSection);
return ref new ListIterator(this);
}

Platform::Object^ List::GetAt(UINT64 upos)
{
ScopedLock lock(cCriticalSection);
return cList.GetAt(upos);
}

ListIterator^ List::Last()
{
ScopedLock lock(cCriticalSection);
UINT64 ucount=cList.GetCount();
if(ucount==0)
	throw ref new Platform::OutOfBoundsException();
return ref new ListIterator(this, ucount-1);
}


//==============
// Modification
//==============

VOID List::Append(Object^ hobj)
{
ScopedLock lock(cCriticalSection);
if(uItCount>0)
	throw ref new Platform::AccessDeniedException();
cList.Append(hobj);
}

VOID List::Clear()
{
ScopedLock lock(cCriticalSection);
if(uItCount>0)
	throw ref new Platform::AccessDeniedException();
cList.Clear();
}

VOID List::InsertAt(UINT64 upos, Object^ hobj)
{
ScopedLock lock(cCriticalSection);
if(uItCount>0)
	throw ref new Platform::AccessDeniedException();
cList.InsertAt(upos, hobj);
}

VOID List::RemoveAt(UINT64 upos)
{
ScopedLock lock(cCriticalSection);
if(uItCount>0)
	throw ref new Platform::AccessDeniedException();
cList.RemoveAt(upos);
}

}