//=========
// Map.cpp
//=========

#include "Map.h"


//=======
// Using
//=======

using MapIterator=::Clusters::Details::Map::MapIterator;


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
return cIndex.contains(hkey);
}

UINT64 Map::Count::get()
{
ScopedLock lock(cCriticalSection);
return cIndex.get_count();
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
return cIndex.get(hkey);
}

MapIterator^ Map::Last()
{
ScopedLock lock(cCriticalSection);
UINT64 ucount=cIndex.get_count();
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
return cIndex.add(hkey, hvalue);
}

VOID Map::Clear()
{
ScopedLock lock(cCriticalSection);
if(uItCount>0)
	throw ref new Platform::AccessDeniedException();
cIndex.clear();
}

bool Map::Remove(String^ hkey)
{
ScopedLock lock(cCriticalSection);
if(uItCount>0)
	throw ref new Platform::AccessDeniedException();
return cIndex.remove(hkey);
}

VOID Map::Set(String^ hkey, Object^ hvalue)
{
ScopedLock lock(cCriticalSection);
if(uItCount>0)
	throw ref new Platform::AccessDeniedException();
cIndex.set(hkey, hvalue);
}

}