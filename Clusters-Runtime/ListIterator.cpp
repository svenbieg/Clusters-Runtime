//==================
// ListIterator.cpp
//==================

#include "pch.h"


//=======
// Using
//=======

#include "List.h"


//===========
// Namespace
//===========

namespace Clusters {


//==================
// Con-/Destructors
//==================

ListIterator::ListIterator(Clusters::List^ hlist):
cIt(&hlist->cList, 0),
hList(hlist)
{
hList->uItCount++;
}

ListIterator::ListIterator(Clusters::List^ hlist, UINT64 upos):
cIt(&hlist->cList, SIZE_T(upos)),
hList(hlist)
{
if(upos>MAXSIZE_T)
	throw ref new Platform::InvalidArgumentException();
hList->uItCount++;
}


//========
// Access
//========

Object^ ListIterator::Current::get()
{
ScopedLock lock(hList->cCriticalSection);
return cIt.get_current();
}

UINT ListIterator::GetMany(Platform::WriteOnlyArray<Object^>^ hitems)
{
if(!hitems)
	throw ref new Platform::InvalidArgumentException();
ScopedLock lock(hList->cCriticalSection);
UINT64 utotal=hList->Count;
UINT ucount=hitems->Length;
UINT64 uread=min(utotal, ucount);
UINT upos=0;
for(cIt.set_position(0); cIt.has_current(); cIt.move_next())
	{
	Object^ hobj=cIt.get_current();
	hitems->set(upos++, hobj);
	if(upos==uread)
		break;
	}
return upos;
}

bool ListIterator::HasCurrent::get()
{
ScopedLock lock(hList->cCriticalSection);
return cIt.has_current();
}


//==============
// Modification
//==============

VOID ListIterator::RemoveCurrent()
{
ScopedLock lock(hList->cCriticalSection);
if(hList->uItCount>1)
	throw ref new Platform::AccessDeniedException();
cIt.remove_current();
}


//============
// Navigation
//============

bool ListIterator::MoveNext()
{
ScopedLock lock(hList->cCriticalSection);
return cIt.move_next();
}

bool ListIterator::MovePrevious()
{
ScopedLock lock(hList->cCriticalSection);
return cIt.move_previous();
}

UINT64 ListIterator::Position::get()
{
ScopedLock lock(hList->cCriticalSection);
return cIt.get_position();
}

VOID ListIterator::Position::set(UINT64 upos)
{
if(upos>MAXSIZE_T)
	throw ref new Platform::InvalidArgumentException();
ScopedLock lock(hList->cCriticalSection);
cIt.set_position(SIZE_T(upos));
}


//==========================
// Con-/Destructors Private
//==========================

ListIterator::~ListIterator()
{
ScopedLock lock(hList->cCriticalSection);
hList->uItCount--;
}

}