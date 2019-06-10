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
cIt(&hcatalog->cIndex, SIZE_T(upos)),
hCatalog(hcatalog)
{
if(upos>MAXSIZE_T)
	throw ref new Platform::InvalidArgumentException();
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
return cIt.get_current_id();
}

Platform::Object^ CatalogIterator::CurrentItem::get()
{
ScopedLock lock(hCatalog->cCriticalSection);
return cIt.get_current_item();
}

VOID CatalogIterator::CurrentItem::set(Object^ hitem)
{
ScopedLock lock(hCatalog->cCriticalSection);
cIt.set_current_item(hitem);
}

bool CatalogIterator::HasCurrent::get()
{
ScopedLock lock(hCatalog->cCriticalSection);
return cIt.has_current();
}


//==============
// Modification
//==============

VOID CatalogIterator::RemoveCurrent()
{
ScopedLock lock(hCatalog->cCriticalSection);
if(hCatalog->uItCount>1)
	throw ref new Platform::AccessDeniedException();
cIt.remove_current();
}


//============
// Navigation
//============

bool CatalogIterator::Find(Platform::Guid uid)
{
ScopedLock lock(hCatalog->cCriticalSection);
return cIt.find(uid);
}

bool CatalogIterator::MoveNext()
{
ScopedLock lock(hCatalog->cCriticalSection);
return cIt.move_next();
}

bool CatalogIterator::MovePrevious()
{
ScopedLock lock(hCatalog->cCriticalSection);
return cIt.move_previous();
}

UINT64 CatalogIterator::Position::get()
{
ScopedLock lock(hCatalog->cCriticalSection);
return cIt.get_position();
}

VOID CatalogIterator::Position::set(UINT64 upos)
{
if(upos>MAXSIZE_T)
	throw ref new Platform::InvalidArgumentException();
ScopedLock lock(hCatalog->cCriticalSection);
cIt.set_position(SIZE_T(upos));
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