//=====================
// CatalogIterator.cpp
//=====================

#include "CatalogIterator.h"


//=======
// Using
//=======

#include "Catalog.h"


//===========
// Namespace
//===========

namespace Clusters {
	namespace Details {
		namespace Catalog {


//==================
// Con-/Destructors
//==================

CatalogIterator::CatalogIterator(Clusters::Catalog^ hcatalog):
cIt(&hcatalog->cIndex, 0),
hCatalog(hcatalog)
{
hCatalog->uItCount++;
}

CatalogIterator::CatalogIterator(Clusters::Catalog^ hcatalog, UINT64 upos):
cIt(&hcatalog->cIndex, ToSIZET(upos)),
hCatalog(hcatalog)
{
hCatalog->uItCount++;
}

CatalogIterator::CatalogIterator(Clusters::Catalog^ hcatalog, Platform::Guid uid):
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
ScopedLock lock(hCatalog->cCriticalSection);
return cIt.GetCurrentId();
}

Platform::Object^ CatalogIterator::CurrentItem::get()
{
ScopedLock lock(hCatalog->cCriticalSection);
return cIt.GetCurrentItem();
}

VOID CatalogIterator::CurrentItem::set(Object^ hitem)
{
ScopedLock lock(hCatalog->cCriticalSection);
cIt.SetCurrentItem(hitem);
}

bool CatalogIterator::HasCurrent::get()
{
ScopedLock lock(hCatalog->cCriticalSection);
return cIt.HasCurrent();
}


//==============
// Modification
//==============

VOID CatalogIterator::RemoveCurrent()
{
ScopedLock lock(hCatalog->cCriticalSection);
if(hCatalog->uItCount>1)
	throw ref new Platform::AccessDeniedException();
cIt.RemoveCurrent();
}


//============
// Navigation
//============

bool CatalogIterator::Find(Platform::Guid uid)
{
ScopedLock lock(hCatalog->cCriticalSection);
return cIt.Find(uid);
}

bool CatalogIterator::MoveNext()
{
ScopedLock lock(hCatalog->cCriticalSection);
return cIt.MoveNext();
}

bool CatalogIterator::MovePrevious()
{
ScopedLock lock(hCatalog->cCriticalSection);
return cIt.MovePrevious();
}

UINT64 CatalogIterator::Position::get()
{
ScopedLock lock(hCatalog->cCriticalSection);
return cIt.GetPosition();
}

VOID CatalogIterator::Position::set(UINT64 upos)
{
ScopedLock lock(hCatalog->cCriticalSection);
cIt.SetPosition(ToSIZET(upos));
}


//==========================
// Con-/Destructors Private
//==========================

CatalogIterator::~CatalogIterator()
{
ScopedLock lock(hCatalog->cCriticalSection);
hCatalog->uItCount--;
}

}}}