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
Lock lock(cCriticalSection);
return ref new ListIterator(this, upos);
}

UINT64 List::Count::get()
{
Lock lock(cCriticalSection);
return cVector.GetCount();
}

ListIterator^ List::First()
{
Lock lock(cCriticalSection);
return ref new ListIterator(this);
}

Platform::Object^ List::GetAt(UINT64 upos)
{
Lock lock(cCriticalSection);
return cVector.GetAt(upos);
}

ListIterator^ List::Last()
{
Lock lock(cCriticalSection);
UINT64 ucount=cVector.GetCount();
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
cVector.Append(hobj);
}

VOID List::Clear()
{
Lock lock(cCriticalSection);
if(uItCount>0)
	throw ref new Platform::AccessDeniedException();
cVector.Clear();
}

VOID List::InsertAt(UINT64 upos, Object^ hobj)
{
Lock lock(cCriticalSection);
if(uItCount>0)
	throw ref new Platform::AccessDeniedException();
cVector.InsertAt(upos, hobj);
}

VOID List::RemoveAt(UINT64 upos)
{
Lock lock(cCriticalSection);
if(uItCount>0)
	throw ref new Platform::AccessDeniedException();
cVector.RemoveAt(upos);
}

}
