//===================
// CatalogIterator.h
//===================

#pragma once


//=======
// Using
//=======

#include "Clusters-Templates\Index.h"
#include "..\Guid.h"

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
	namespace Details {
		namespace Catalog {


//==================
// Iterator Catalog
//==================

[Platform::MTAThreadAttribute]
[WFM::MarshalingBehaviorAttribute(WFM::MarshalingType::Agile)]
[WFM::WebHostHiddenAttribute]
public ref class CatalogIterator sealed
{
private:
	// Using
	using Object=Platform::Object;
	using ScopedLock=Concurrency::critical_section::scoped_lock;

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
	Clusters::Templates::Index<Guid, Object^>::Iterator cIt;
	Clusters::Catalog^ hCatalog;
};

}}}