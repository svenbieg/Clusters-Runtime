//=============
// Catalog.cpp
//=============

#include "pch.h"


//=======
// Using
//=======

#include "Catalog.h"

using CatalogIterator=Clusters::Details::Catalog::CatalogIterator;


//===========
// Namespace
//===========

namespace Clusters {


//========
// Access
//========

CatalogIterator^ Catalog::At(UINT64 upos)
{
Lock lock(cCriticalSection);
return ref new CatalogIterator(this, upos);
}

bool Catalog::Contains(Platform::Guid uid)
{
Lock lock(cCriticalSection);
return cIndex.Contains(uid);
}

UINT64 Catalog::Count::get()
{
Lock lock(cCriticalSection);
return cIndex.GetCount();
}

CatalogIterator^ Catalog::Find(Platform::Guid uid)
{
Lock lock(cCriticalSection);
return ref new CatalogIterator(this, uid);
}

CatalogIterator^ Catalog::First()
{
Lock lock(cCriticalSection);
return ref new CatalogIterator(this);
}

Platform::Object^ Catalog::Get(Platform::Guid uid)
{
Lock lock(cCriticalSection);
return cIndex.Get(uid);
}

CatalogIterator^ Catalog::Last()
{
Lock lock(cCriticalSection);
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
Lock lock(cCriticalSection);
if(uItCount>0)
	throw ref new Platform::AccessDeniedException();
auto pitem=cIndex.Add(uid, hvalue);
return pitem!=nullptr;
}

VOID Catalog::Clear()
{
Lock lock(cCriticalSection);
if(uItCount>0)
	throw ref new Platform::AccessDeniedException();
cIndex.Clear();
}

bool Catalog::Remove(Platform::Guid uid)
{
Lock lock(cCriticalSection);
if(uItCount>0)
	throw ref new Platform::AccessDeniedException();
return cIndex.Remove(uid);
}

VOID Catalog::RemoveAt(UINT64 upos)
{
Lock lock(cCriticalSection);
if(uItCount>0)
	throw ref new Platform::AccessDeniedException();
cIndex.RemoveAt(upos);
}

VOID Catalog::Set(Platform::Guid uid, Object^ hvalue)
{
Lock lock(cCriticalSection);
if(uItCount>0)
	throw ref new Platform::AccessDeniedException();
cIndex.Set(uid, hvalue);
}

}
