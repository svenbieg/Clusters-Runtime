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
	namespace Runtime {
		namespace Details {
			namespace Map {


//==================
// Con-/Destructors
//==================

MapIterator::MapIterator(Clusters::Runtime::Map^ hmap):
cIt(&hmap->cIndex, 0),
hMap(hmap)
{
hMap->uItCount++;
}

MapIterator::MapIterator(Clusters::Runtime::Map^ hmap, UINT64 upos):
cIt(&hmap->cIndex, upos),
hMap(hmap)
{
hMap->uItCount++;
}

MapIterator::MapIterator(Clusters::Runtime::Map^ hmap, String^ hkey):
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
Lock lock(hMap->cCriticalSection);
return cIt.GetCurrentId();
}

Platform::Object^ MapIterator::CurrentValue::get()
{
Lock lock(hMap->cCriticalSection);
return cIt.GetCurrentItem();
}

VOID MapIterator::CurrentValue::set(Object^ hvalue)
{
Lock lock(hMap->cCriticalSection);
cIt.SetCurrentItem(hvalue);
}

bool MapIterator::HasCurrent::get()
{
Lock lock(hMap->cCriticalSection);
return cIt.HasCurrent();
}


//==============
// Modification
//==============

VOID MapIterator::RemoveCurrent()
{
Lock lock(hMap->cCriticalSection);
if(hMap->uItCount>1)
	throw ref new Platform::InvalidArgumentException();
cIt.RemoveCurrent();
}


//============
// Navigation
//============

bool MapIterator::Find(String^ hkey)
{
Lock lock(hMap->cCriticalSection);
return cIt.Find(hkey);
}

bool MapIterator::MoveNext()
{
Lock lock(hMap->cCriticalSection);
return cIt.MoveNext();
}

bool MapIterator::MovePrevious()
{
Lock lock(hMap->cCriticalSection);
return cIt.MovePrevious();
}

UINT64 MapIterator::Position::get()
{
Lock lock(hMap->cCriticalSection);
return cIt.GetPosition();
}

VOID MapIterator::Position::set(UINT64 upos)
{
Lock lock(hMap->cCriticalSection);
cIt.SetPosition(upos);
}


//==========================
// Con-/Destructors Private
//==========================

MapIterator::~MapIterator()
{
Lock lock(hMap->cCriticalSection);
hMap->uItCount--;
}

}}}}
