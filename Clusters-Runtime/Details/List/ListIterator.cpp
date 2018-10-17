//==================
// ListIterator.cpp
//==================

#include "pch.h"


//=======
// Using
//=======

#include "List.h"

using Object=Platform::Object;


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
cIt(&hlist->cList, upos),
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
return cIt.GetCurrent();
}

bool ListIterator::HasCurrent::get()
{
ScopedLock lock(hList->cCriticalSection);
return cIt.HasCurrent();
}


//==============
// Modification
//==============

VOID ListIterator::RemoveCurrent()
{
ScopedLock lock(hList->cCriticalSection);
if(hList->uItCount>1)
	throw ref new Platform::AccessDeniedException();
cIt.RemoveCurrent();
}


//============
// Navigation
//============

bool ListIterator::MoveNext()
{
ScopedLock lock(hList->cCriticalSection);
return cIt.MoveNext();
}

bool ListIterator::MovePrevious()
{
ScopedLock lock(hList->cCriticalSection);
return cIt.MovePrevious();
}

UINT64 ListIterator::Position::get()
{
ScopedLock lock(hList->cCriticalSection);
return cIt.GetPosition();
}

VOID ListIterator::Position::set(UINT64 upos)
{
ScopedLock lock(hList->cCriticalSection);
cIt.SetPosition(upos);
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