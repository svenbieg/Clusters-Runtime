//===================
// CatalogIterator.h
//===================

#pragma once


//=======
// Using
//=======

#include "clusters\index.h"
#include "Guid.h"

namespace WFM=Windows::Foundation::Metadata;


//======================
// Forward-Declarations
//======================

namespace Clusters
{
ref class Catalog;
}


//===========
// Namespace
//===========

namespace Clusters {


//==================
// Iterator Catalog
//==================

[Platform::MTAThreadAttribute]
[WFM::MarshalingBehaviorAttribute(WFM::MarshalingType::Agile)]
[WFM::WebHostHiddenAttribute]
public ref class CatalogIterator sealed
{
public:
	// Access
	property Platform::Guid CurrentId { Platform::Guid get(); }
	property Object^ CurrentItem { Object^ get(); VOID set(Object^ Item); }
	property bool HasCurrent { bool get(); }

	// Modification
	VOID RemoveCurrent();

	// Navigation
	bool Find(Platform::Guid Id);
	bool MoveNext();
	bool MovePrevious();
	property UINT64 Position { UINT64 get(); VOID set(UINT64 Position); }

internal:
	// Con-/Destructors
	CatalogIterator(Clusters::Catalog^ Catalog);
	CatalogIterator(Clusters::Catalog^ Catalog, UINT64 Position);
	CatalogIterator(Clusters::Catalog^ Catalog, Platform::Guid Id);

private:
	// Con-/Destructors
	CatalogIterator(): cIt(nullptr, 0) {}
	~CatalogIterator();

	// Common
	clusters::index<Guid, Handle<Object>>::iterator cIt;
	Clusters::Catalog^ hCatalog;
};

}