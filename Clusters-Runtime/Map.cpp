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
Lock lock(cCriticalSection);
return ref new MapIterator(this, upos);
}

bool Map::Contains(String^ hkey)
{
Lock lock(cCriticalSection);
return cIndex.Contains(hkey);
}

UINT64 Map::Count::get()
{
Lock lock(cCriticalSection);
return cIndex.GetCount();
}

MapIterator^ Map::Find(String^ hkey)
{
Lock lock(cCriticalSection);
return ref new MapIterator(this, hkey);
}

MapIterator^ Map::First()
{
Lock lock(cCriticalSection);
return ref new MapIterator(this);
}

Platform::Object^ Map::Get(String^ hkey)
{
Lock lock(cCriticalSection);
return cIndex.Get(hkey);
}

MapIterator^ Map::Last()
{
Lock lock(cCriticalSection);
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
Lock lock(cCriticalSection);
if(uItCount>0)
	throw ref new Platform::AccessDeniedException();
auto pitem=cIndex.Add(hkey, hvalue);
return pitem!=nullptr;
}

VOID Map::Clear()
{
Lock lock(cCriticalSection);
if(uItCount>0)
	throw ref new Platform::AccessDeniedException();
cIndex.Clear();
}

bool Map::Remove(String^ hkey)
{
Lock lock(cCriticalSection);
if(uItCount>0)
	throw ref new Platform::AccessDeniedException();
return cIndex.Remove(hkey);
}

VOID Map::RemoveAt(UINT64 upos)
{
Lock lock(cCriticalSection);
if(uItCount>0)
	throw ref new Platform::AccessDeniedException();
cIndex.RemoveAt(upos);
}

VOID Map::Set(String^ hkey, Object^ hvalue)
{
Lock lock(cCriticalSection);
if(uItCount>0)
	throw ref new Platform::AccessDeniedException();
cIndex.Set(hkey, hvalue);
}

}
