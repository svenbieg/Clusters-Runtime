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
cIt(&hlist->cVector, 0),
hList(hlist)
{
hList->uItCount++;
}

ListIterator::ListIterator(Clusters::List^ hlist, UINT64 upos):
cIt(&hlist->cVector, upos),
hList(hlist)
{
hList->uItCount++;
}


//========
// Access
//========

Object^ ListIterator::Current::get()
{
Lock lock(hList->cCriticalSection);
return cIt.GetCurrent();
}

bool ListIterator::HasCurrent::get()
{
Lock lock(hList->cCriticalSection);
return cIt.HasCurrent();
}


//==============
// Modification
//==============

VOID ListIterator::RemoveCurrent()
{
Lock lock(hList->cCriticalSection);
if(hList->uItCount>1)
	throw ref new Platform::AccessDeniedException();
cIt.RemoveCurrent();
}


//============
// Navigation
//============

bool ListIterator::MoveNext()
{
Lock lock(hList->cCriticalSection);
return cIt.MoveNext();
}

bool ListIterator::MovePrevious()
{
Lock lock(hList->cCriticalSection);
return cIt.MovePrevious();
}

UINT64 ListIterator::Position::get()
{
Lock lock(hList->cCriticalSection);
return cIt.GetPosition();
}

VOID ListIterator::Position::set(UINT64 upos)
{
Lock lock(hList->cCriticalSection);
cIt.SetPosition(upos);
}


//==========================
// Con-/Destructors Private
//==========================

ListIterator::~ListIterator()
{
Lock lock(hList->cCriticalSection);
hList->uItCount--;
}

}}}
