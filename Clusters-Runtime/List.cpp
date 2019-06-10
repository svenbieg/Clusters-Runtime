//==========
// List.cpp
//==========

#include "List.h"


//=======
// Using
//=======

using ListIterator=::Clusters::Details::List::ListIterator;


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
return cList.get_count();
}

ListIterator^ List::First()
{
ScopedLock lock(cCriticalSection);
return ref new ListIterator(this);
}

Platform::Object^ List::GetAt(UINT64 upos)
{
ScopedLock lock(cCriticalSection);
if(upos>MAXSIZE_T)
	throw ref new Platform::InvalidArgumentException();
return cList.get_at(SIZE_T(upos));
}

ListIterator^ List::Last()
{
ScopedLock lock(cCriticalSection);
UINT64 ucount=cList.get_count();
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
cList.append(hobj);
}

VOID List::Clear()
{
ScopedLock lock(cCriticalSection);
if(uItCount>0)
	throw ref new Platform::AccessDeniedException();
cList.clear();
}

VOID List::InsertAt(UINT64 upos, Object^ hobj)
{
ScopedLock lock(cCriticalSection);
if(uItCount>0)
	throw ref new Platform::AccessDeniedException();
if(upos>MAXSIZE_T)
	throw ref new Platform::InvalidArgumentException();
cList.insert_at(SIZE_T(upos), hobj);
}

VOID List::RemoveAt(UINT64 upos)
{
ScopedLock lock(cCriticalSection);
if(uItCount>0)
	throw ref new Platform::AccessDeniedException();
if(upos>MAXSIZE_T)
	throw ref new Platform::InvalidArgumentException();
cList.remove_at(SIZE_T(upos));
}

}