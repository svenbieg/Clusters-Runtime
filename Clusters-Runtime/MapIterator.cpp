//=================
// MapIterator.cpp
//=================

#include "pch.h"


//=======
// Using
//=======

#include "Map.h"
#include "MapItem.h"


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

MapItem^ MapIterator::Current::get()
{
//ScopedLock lock(hMap->cCriticalSection);
String^ hkey=cIt.get_current_id();
Object^ hvalue=cIt.get_current_item();
return ref new MapItem(hMap, hkey, hvalue);
}

UINT MapIterator::GetMany(Platform::WriteOnlyArray<MapItem^>^ hitems)
{
if(!hitems)
	throw ref new Platform::InvalidArgumentException();
//ScopedLock lock(hMap->cCriticalSection);
UINT64 utotal=hMap->Count;
UINT ucount=hitems->Length;
UINT64 uread=min(utotal, ucount);
UINT upos=0;
for(cIt.set_position(0); cIt.has_current(); cIt.move_next())
	{
	String^ hkey=cIt.get_current_id();
	Object^ hvalue=cIt.get_current_item();
	MapItem^ hitem=ref new MapItem(hMap, hkey, hvalue);
	hitems->set(upos++, hitem);
	if(upos==uread)
		break;
	}
return upos;
}

bool MapIterator::HasCurrent::get()
{
//ScopedLock lock(hMap->cCriticalSection);
return cIt.has_current();
}


//==============
// Modification
//==============

VOID MapIterator::RemoveCurrent()
{
//ScopedLock lock(hMap->cCriticalSection);
if(hMap->uItCount>1)
	throw ref new Platform::AccessDeniedException();
cIt.remove_current();
}


//============
// Navigation
//============

bool MapIterator::Find(String^ hkey)
{
//ScopedLock lock(hMap->cCriticalSection);
return cIt.find(hkey);
}

bool MapIterator::MoveNext()
{
//ScopedLock lock(hMap->cCriticalSection);
return cIt.move_next();
}

bool MapIterator::MovePrevious()
{
//ScopedLock lock(hMap->cCriticalSection);
return cIt.move_previous();
}

UINT64 MapIterator::Position::get()
{
//ScopedLock lock(hMap->cCriticalSection);
return cIt.get_position();
}

VOID MapIterator::Position::set(UINT64 upos)
{
if(upos>MAXSIZE_T)
	throw ref new Platform::InvalidArgumentException();
//ScopedLock lock(hMap->cCriticalSection);
cIt.set_position(SIZE_T(upos));
}


//==========================
// Con-/Destructors Private
//==========================

MapIterator::~MapIterator()
{
//ScopedLock lock(hMap->cCriticalSection);
hMap->uItCount--;
}

}