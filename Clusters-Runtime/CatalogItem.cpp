//=================
// CatalogItem.cpp
//=================

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

CatalogItem::CatalogItem(Catalog^ hcatalog, Platform::Guid uid, Object^ hvalue):
hCatalog(hcatalog),
hValue(hvalue),
uId(uid)
{}


//========
// Common
//========

Platform::Guid CatalogItem::Id::get()
{
return uId;
}

Object^ CatalogItem::Value::get()
{
return hValue;
}

VOID CatalogItem::Value::set(Object^ hobj)
{
hValue=hobj;
hCatalog->Set(uId, hValue);
}

}