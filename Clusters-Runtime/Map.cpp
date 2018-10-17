//=========
// Map.cpp
//=========

#include "pch.h"


//=======
// Using
//=======

#include "Map.h"

using MapIterator=Clusters::Details::Map::MapIterator;


//===========
// Namespace
//===========

namespace Clusters {


//========
// Access
//========

MapIterator^ Map::At(UINT64 upos)
{
ScopedLock lock(cCriticalSection);
return ref new MapIterator(this, upos);
}

bool Map::Contains(String^ hkey)
{
ScopedLock lock(cCriticalSection);
return cIndex.Contains(hkey);
}

UINT64 Map::Count::get()
{
ScopedLock lock(cCriticalSection);
return cIndex.GetCount();
}

MapIterator^ Map::Find(String^ hkey)
{
ScopedLock lock(cCriticalSection);
return ref new MapIterator(this, hkey);
}

MapIterator^ Map::First()
{
ScopedLock lock(cCriticalSection);
return ref new MapIterator(this);
}

Platform::Object^ Map::Get(String^ hkey)
{
ScopedLock lock(cCriticalSection);
return cIndex.Get(hkey);
}

MapIterator^ Map::Last()
{
ScopedLock lock(cCriticalSection);
UINT64 ucount=cIndex.GetCount();
if(ucount==0)
	throw ref new Platform::InvalidArgumentException();
return ref new MapIterator(this, ucount-1);
}


//==============
// Modification
//==============

bool Map::Add(String^ hkey, Object^ hvalue)
{
ScopedLock lock(cCriticalSection);
if(uItCount>0)
	throw ref new Platform::AccessDeniedException();
return cIndex.Add(hkey, hvalue);
}

VOID Map::Clear()
{
ScopedLock lock(cCriticalSection);
if(uItCount>0)
	throw ref new Platform::AccessDeniedException();
cIndex.Clear();
}

bool Map::Remove(String^ hkey)
{
ScopedLock lock(cCriticalSection);
if(uItCount>0)
	throw ref new Platform::AccessDeniedException();
return cIndex.Remove(hkey);
}

VOID Map::Set(String^ hkey, Object^ hvalue)
{
ScopedLock lock(cCriticalSection);
if(uItCount>0)
	throw ref new Platform::AccessDeniedException();
cIndex.Set(hkey, hvalue);
}

}