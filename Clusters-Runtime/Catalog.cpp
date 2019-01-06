//=============
// Catalog.cpp
//=============

#include "pch.h"


//=======
// Using
//=======

#include "Catalog.h"

using CatalogIterator=::Clusters::Details::Catalog::CatalogIterator;


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
return cIndex.Contains(uid);
}

UINT64 Catalog::Count::get()
{
ScopedLock lock(cCriticalSection);
return cIndex.GetCount();
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
return cIndex.Get(uid);
}

CatalogIterator^ Catalog::Last()
{
ScopedLock lock(cCriticalSection);
UINT64 ucount=cIndex.GetCount();
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
return cIndex.Add(uid, hvalue);
}

VOID Catalog::Clear()
{
ScopedLock lock(cCriticalSection);
if(uItCount>0)
	throw ref new Platform::AccessDeniedException();
cIndex.Clear();
}

bool Catalog::Remove(Platform::Guid uid)
{
ScopedLock lock(cCriticalSection);
if(uItCount>0)
	throw ref new Platform::AccessDeniedException();
return cIndex.Remove(uid);
}

VOID Catalog::Set(Platform::Guid uid, Object^ hvalue)
{
ScopedLock lock(cCriticalSection);
if(uItCount>0)
	throw ref new Platform::AccessDeniedException();
cIndex.Set(uid, hvalue);
}

}