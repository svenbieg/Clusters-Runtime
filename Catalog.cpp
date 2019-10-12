//=============
// Catalog.cpp
//=============

#include "pch.h"


//=======
// Using
//=======

#include "Catalog.h"


//===========
// Namespace
//===========

namespace Clusters {


//========
// Access
//========

CatalogIterator^ Catalog::At(UINT64 upos)
{
ScopedLock lock(cCriticalSection);
return ref new CatalogIterator(this, upos);
}

bool Catalog::Contains(Platform::Guid uid)
{
ScopedLock lock(cCriticalSection);
return cIndex.contains(uid);
}

UINT64 Catalog::Count::get()
{
ScopedLock lock(cCriticalSection);
return cIndex.get_count();
}

CatalogIterator^ Catalog::Find(Platform::Guid uid)
{
ScopedLock lock(cCriticalSection);
return ref new CatalogIterator(this, uid);
}

CatalogIterator^ Catalog::First()
{
ScopedLock lock(cCriticalSection);
return ref new CatalogIterator(this);
}

Platform::Object^ Catalog::Get(Platform::Guid uid)
{
ScopedLock lock(cCriticalSection);
return cIndex.get(uid);
}

CatalogIterator^ Catalog::Last()
{
ScopedLock lock(cCriticalSection);
UINT64 ucount=cIndex.get_count();
if(ucount==0)
	throw ref new Platform::InvalidArgumentException();
return ref new CatalogIterator(this, ucount-1);
}


//==============
// Modification
//==============

bool Catalog::Add(Platform::Guid uid, Object^ hvalue)
{
ScopedLock lock(cCriticalSection);
if(uItCount>0)
	throw ref new Platform::AccessDeniedException();
return cIndex.add(uid, hvalue);
}

VOID Catalog::Clear()
{
ScopedLock lock(cCriticalSection);
if(uItCount>0)
	throw ref new Platform::AccessDeniedException();
cIndex.clear();
}

bool Catalog::Remove(Platform::Guid uid)
{
ScopedLock lock(cCriticalSection);
if(uItCount>0)
	throw ref new Platform::AccessDeniedException();
return cIndex.remove(uid);
}

VOID Catalog::Set(Platform::Guid uid, Object^ hvalue)
{
ScopedLock lock(cCriticalSection);
if(uItCount>0)
	throw ref new Platform::AccessDeniedException();
cIndex.set(uid, hvalue);
}

}