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


//==================
// Con-/Destructors
//==================

CatalogIterator::CatalogIterator(Clusters::Catalog^ hcatalog):
cIt(&hcatalog->cList, 0),
hCatalog(hcatalog)
{
hCatalog->uItCount++;
}

CatalogIterator::CatalogIterator(Clusters::Catalog^ hcatalog, UINT64 upos):
cIt(&hcatalog->cList, SIZE_T(upos)),
hCatalog(hcatalog)
{
if(upos>MAXSIZE_T)
	throw ref new Platform::InvalidArgumentException();
hCatalog->uItCount++;
}

CatalogIterator::CatalogIterator(Clusters::Catalog^ hcatalog, Platform::Guid uid):
cIt(&hcatalog->cList, 0, uid),
hCatalog(hcatalog)
{
hCatalog->uItCount++;
}


//========
// Access
//========

CatalogItem^ CatalogIterator::Current::get()
{
ScopedLock lock(hCatalog->cCriticalSection);
Platform::Guid uid=cIt.get_current_id();
Object^ hvalue=cIt.get_current_item();
return ref new CatalogItem(hCatalog, uid, hvalue);
}

UINT CatalogIterator::GetMany(Platform::WriteOnlyArray<CatalogItem^>^ hitems)
{
if(!hitems)
	throw ref new Platform::InvalidArgumentException();
ScopedLock lock(hCatalog->cCriticalSection);
UINT64 utotal=hCatalog->Count;
UINT ucount=hitems->Length;
UINT64 uread=min(utotal, ucount);
UINT upos=0;
for(cIt.set_position(0); cIt.has_current(); cIt.move_next())
	{
	Platform::Guid uid=cIt.get_current_id();
	Object^ hvalue=cIt.get_current_item();
	CatalogItem^ hitem=ref new CatalogItem(hCatalog, uid, hvalue);
	hitems->set(upos++, hitem);
	if(upos==uread)
		break;
	}
return upos;
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

}