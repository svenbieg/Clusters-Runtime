//=========
// Map.cpp
//=========

#include "pch.h"


//=======
// Using
//=======

#include "Map.h"


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
return cList.contains(hkey);
}

UINT64 Map::Count::get()
{
ScopedLock lock(cCriticalSection);
return cList.get_count();
}

MapIterator^ Map::Find(String^ hkey)
{
ScopedLock lock(cCriticalSection);
return ref new MapIterator(this, hkey);
}

IIterator<MapItem^>^ Map::First()
{
ScopedLock lock(cCriticalSection);
return ref new MapIterator(this);
}

Platform::Object^ Map::Get(String^ hkey)
{
ScopedLock lock(cCriticalSection);
return cList.get(hkey);
}

MapIterator^ Map::Last()
{
ScopedLock lock(cCriticalSection);
UINT64 ucount=cList.get_count();
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
return cList.add(hkey, hvalue);
}

VOID Map::Clear()
{
ScopedLock lock(cCriticalSection);
if(uItCount>0)
	throw ref new Platform::AccessDeniedException();
cList.clear();
}

bool Map::Remove(String^ hkey)
{
ScopedLock lock(cCriticalSection);
if(uItCount>0)
	throw ref new Platform::AccessDeniedException();
return cList.remove(hkey);
}

VOID Map::Set(String^ hkey, Object^ hvalue)
{
ScopedLock lock(cCriticalSection);
cList.set(hkey, hvalue);
}

}