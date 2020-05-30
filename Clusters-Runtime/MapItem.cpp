//=============
// MapItem.cpp
//=============

#include "pch.h"


//=======
// Using
//=======

#include "Map.h"


//===========
// Namespace
//===========

namespace Clusters {


//==================
// Con-/Destructors
//==================

MapItem::MapItem(Map^ hmap, String^ hkey, Object^ hvalue):
hKey(hkey),
hMap(hmap),
hValue(hvalue)
{}


//========
// Common
//========

String^ MapItem::Key::get()
{
return hKey;
}

Object^ MapItem::Value::get()
{
return hValue;
}

VOID MapItem::Value::set(Object^ hobj)
{
hValue=hobj;
hMap->Set(hKey, hValue);
}

}