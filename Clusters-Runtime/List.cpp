//==========
// List.cpp
//==========

#include "pch.h"


//=======
// Using
//=======

#include "List.h"

using ListIterator=Clusters::Runtime::Details::List::ListIterator;


//===========
// Namespace
//===========

namespace Clusters {
	namespace Runtime {


//========
// Access
//========

ListIterator^ List::At(UINT64 upos)
{
Lock lock(cCriticalSection);
return ref new ListIterator(this, upos);
}

UINT64 List::Count::get()
{
Lock lock(cCriticalSection);
return cList.GetCount();
}

ListIterator^ List::First()
{
Lock lock(cCriticalSection);
return ref new ListIterator(this);
}

Platform::Object^ List::GetAt(UINT64 upos)
{
Lock lock(cCriticalSection);
return cList.GetAt(upos);
}

ListIterator^ List::Last()
{
Lock lock(cCriticalSection);
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
Lock lock(cCriticalSection);
if(uItCount>0)
	throw ref new Platform::AccessDeniedException();
cList.Append(hobj);
}

VOID List::Clear()
{
Lock lock(cCriticalSection);
if(uItCount>0)
	throw ref new Platform::AccessDeniedException();
cList.Clear();
}

VOID List::InsertAt(UINT64 upos, Object^ hobj)
{
Lock lock(cCriticalSection);
if(uItCount>0)
	throw ref new Platform::AccessDeniedException();
cList.InsertAt(upos, hobj);
}

VOID List::RemoveAt(UINT64 upos)
{
Lock lock(cCriticalSection);
if(uItCount>0)
	throw ref new Platform::AccessDeniedException();
cList.RemoveAt(upos);
}

}}
