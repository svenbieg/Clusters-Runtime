//==================
// ResourceHelper.h
//==================

#pragma once


//============
// Ressources
//============

IMAGE_RESOURCE_DATA_ENTRY* GetResourceData(IMAGE_RESOURCE_DIRECTORY* Parent, UINT Id);
IMAGE_RESOURCE_DATA_ENTRY* GetResourceData(IMAGE_RESOURCE_DIRECTORY* Parent, LPCTSTR Name);
IMAGE_RESOURCE_DIRECTORY* GetResourceDirectory(IMAGE_RESOURCE_DIRECTORY* Parent, UINT Id);
IMAGE_RESOURCE_DIRECTORY* GetResourceDirectory(IMAGE_RESOURCE_DIRECTORY* Parent, LPCTSTR Name);
IMAGE_RESOURCE_DIRECTORY* GetResourceRoot();


//=========
// Strings
//=========

UINT GetResourceString(UINT Id, LPCTSTR* String);
UINT GetResourceString(UINT Id, LPTSTR String, UINT MaxLen);


//=======
// Icons
//=======

BITMAPINFO* GetResourceIcon(UINT Id, UINT Size);
