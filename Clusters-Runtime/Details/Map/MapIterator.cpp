//=================
// MapIterator.cpp
//=================

#include "pch.h"


//=======
// Using
//=======

#include "Map.h"


//===========
// Namespace
//===========

namespace Clusters {
	namespace Details {
		namespace Map {


//==================
// Con-/Destructors
//==================

MapIterator::MapIterator(Clusters::Map^ hmap):
cIt(&hmap->cIndex, 0),
hMap(hmap)
{
hMap->uItCount++;
}

MapIterator::MapIterator(Clusters::Map^ hmap, UINT64 upos):
cIt(&hmap->cIndex, ToSIZET(upos)),
hMap(hmap)
{
hMap->uItCount++;
}

MapIterator::MapIterator(Clusters::Map^ hmap, String^ hkey):
cIt(&hmap->cIndex, hkey),
hMap(hmap)
{
hMap->uItCount++;
}


//========
// Access
//========

Platform::String^ MapIterator::CurrentKey::get()
{
ScopedLock lock(hMap->cCriticalSection);
return cIt.GetCurrentId();
}

Platform::Object^ MapIterator::CurrentValue::get()
{
ScopedLock lock(hMap->cCriticalSection);
return cIt.GetCurrentItem();
}

VOID MapIterator::CurrentValue::set(Object^ hvalue)
{
ScopedLock lock(hMap->cCriticalSection);
cIt.SetCurrentItem(hvalue);
}

bool MapIterator::HasCurrent::get()
{
ScopedLock lock(hMap->cCriticalSection);
return cIt.HasCurrent();
}


//==============
// Modification
//==============

VOID MapIterator::RemoveCurrent()
{
ScopedLock lock(hMap->cCriticalSection);
if(hMap->uItCount>1)
	throw ref new Platform::AccessDeniedException();
cIt.RemoveCurrent();
}


//============
// Navigation
//============

bool MapIterator::Find(String^ hkey)
{
ScopedLock lock(hMap->cCriticalSection);
return cIt.Find(hkey);
}

bool MapIterator::MoveNext()
{
ScopedLock lock(hMap->cCriticalSection);
return cIt.MoveNext();
}

bool MapIterator::MovePrevious()
{
ScopedLock lock(hMap->cCriticalSection);
return cIt.MovePrevious();
}

UINT64 MapIterator::Position::get()
{
ScopedLock lock(hMap->cCriticalSection);
return cIt.GetPosition();
}

VOID MapIterator::Position::set(UINT64 upos)
{
ScopedLock lock(hMap->cCriticalSection);
cIt.SetPosition(ToSIZET(upos));
}


//==========================
// Con-/Destructors Private
//==========================

MapIterator::~MapIterator()
{
ScopedLock lock(hMap->cCriticalSection);
hMap->uItCount--;
}

}}}