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
	namespace Details {
		namespace List {


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
cIt(&hlist->cList, ToSIZET(upos)),
hList(hlist)
{
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
ScopedLock lock(hList->cCriticalSection);
cIt.set_position(ToSIZET(upos));
}


//==========================
// Con-/Destructors Private
//==========================

ListIterator::~ListIterator()
{
ScopedLock lock(hList->cCriticalSection);
hList->uItCount--;
}

}}}