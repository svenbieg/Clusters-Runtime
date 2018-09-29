//=====================
// CatalogIterator.cpp
//=====================

#include "pch.h"


//=======
// Using
//=======

#include "Catalog.h"


//===========
// Namespace
//===========

namespace Clusters {
	namespace Runtime {
		namespace Details {
			namespace Catalog {


//==================
// Con-/Destructors
//==================

CatalogIterator::CatalogIterator(Clusters::Runtime::Catalog^ hcatalog):
cIt(&hcatalog->cIndex, 0),
hCatalog(hcatalog)
{
hCatalog->uItCount++;
}

CatalogIterator::CatalogIterator(Clusters::Runtime::Catalog^ hcatalog, UINT64 upos):
cIt(&hcatalog->cIndex, upos),
hCatalog(hcatalog)
{
hCatalog->uItCount++;
}

CatalogIterator::CatalogIterator(Clusters::Runtime::Catalog^ hcatalog, Platform::Guid uid):
cIt(&hcatalog->cIndex, 0, uid),
hCatalog(hcatalog)
{
hCatalog->uItCount++;
}


//========
// Access
//========

Platform::Guid CatalogIterator::CurrentId::get()
{
Lock lock(hCatalog->cCriticalSection);
return cIt.GetCurrentId();
}

Platform::Object^ CatalogIterator::CurrentItem::get()
{
Lock lock(hCatalog->cCriticalSection);
return cIt.GetCurrentItem();
}

VOID CatalogIterator::CurrentItem::set(Object^ hitem)
{
Lock lock(hCatalog->cCriticalSection);
cIt.SetCurrentItem(hitem);
}

bool CatalogIterator::HasCurrent::get()
{
Lock lock(hCatalog->cCriticalSection);
return cIt.HasCurrent();
}


//==============
// Modification
//==============

VOID CatalogIterator::RemoveCurrent()
{
Lock lock(hCatalog->cCriticalSection);
if(hCatalog->uItCount>1)
	throw ref new Platform::InvalidArgumentException();
cIt.RemoveCurrent();
}


//============
// Navigation
//============

bool CatalogIterator::Find(Platform::Guid uid)
{
Lock lock(hCatalog->cCriticalSection);
return cIt.Find(uid);
}

bool CatalogIterator::MoveNext()
{
Lock lock(hCatalog->cCriticalSection);
return cIt.MoveNext();
}

bool CatalogIterator::MovePrevious()
{
Lock lock(hCatalog->cCriticalSection);
return cIt.MovePrevious();
}

UINT64 CatalogIterator::Position::get()
{
Lock lock(hCatalog->cCriticalSection);
return cIt.GetPosition();
}

VOID CatalogIterator::Position::set(UINT64 upos)
{
Lock lock(hCatalog->cCriticalSection);
cIt.SetPosition(upos);
}


//==========================
// Con-/Destructors Private
//==========================

CatalogIterator::~CatalogIterator()
{
Lock lock(hCatalog->cCriticalSection);
hCatalog->uItCount--;
}

}}}}
