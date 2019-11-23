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


//==================
// Con-/Destructors
//==================

MapIterator::MapIterator(Clusters::Map^ hmap):
cIt(&hmap->cList, 0),
hMap(hmap)
{
hMap->uItCount++;
}

MapIterator::MapIterator(Clusters::Map^ hmap, UINT64 upos):
cIt(&hmap->cList, SIZE_T(upos)),
hMap(hmap)
{
if(upos>MAXSIZE_T)
	throw ref new Platform::InvalidArgumentException();
hMap->uItCount++;
}

MapIterator::MapIterator(Clusters::Map^ hmap, String^ hkey):
cIt(&hmap->cList, 0, hkey),
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
return cIt.get_current_id();
}

Platform::Object^ MapIterator::CurrentValue::get()
{
ScopedLock lock(hMap->cCriticalSection);
return cIt.get_current_item();
}

VOID MapIterator::CurrentValue::set(Object^ hvalue)
{
ScopedLock lock(hMap->cCriticalSection);
cIt.set_current_item(hvalue);
}

bool MapIterator::HasCurrent::get()
{
ScopedLock lock(hMap->cCriticalSection);
return cIt.has_current();
}


//==============
// Modification
//==============

VOID MapIterator::RemoveCurrent()
{
ScopedLock lock(hMap->cCriticalSection);
if(hMap->uItCount>1)
	throw ref new Platform::AccessDeniedException();
cIt.remove_current();
}


//============
// Navigation
//============

bool MapIterator::Find(String^ hkey)
{
ScopedLock lock(hMap->cCriticalSection);
return cIt.find(hkey);
}

bool MapIterator::MoveNext()
{
ScopedLock lock(hMap->cCriticalSection);
return cIt.move_next();
}

bool MapIterator::MovePrevious()
{
ScopedLock lock(hMap->cCriticalSection);
return cIt.move_previous();
}

UINT64 MapIterator::Position::get()
{
ScopedLock lock(hMap->cCriticalSection);
return cIt.get_position();
}

VOID MapIterator::Position::set(UINT64 upos)
{
if(upos>MAXSIZE_T)
	throw ref new Platform::InvalidArgumentException();
ScopedLock lock(hMap->cCriticalSection);
cIt.set_position(SIZE_T(upos));
}


//==========================
// Con-/Destructors Private
//==========================

MapIterator::~MapIterator()
{
ScopedLock lock(hMap->cCriticalSection);
hMap->uItCount--;
}

}