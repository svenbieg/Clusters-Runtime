//===========
// Catalog.h
//===========

#pragma once
#include "pch.h"


//=======
// Using
//=======

#include "Clusters\Index.h"
#include "Default\Guid.h"
#include "Details\Catalog\CatalogIterator.h"

namespace WFM=Windows::Foundation::Metadata;


//===========
// Namespace
//===========

namespace Clusters {


//=========
// Catalog
//=========

[Platform::MTAThreadAttribute]
[WFM::MarshalingBehaviorAttribute(WFM::MarshalingType::Agile)]
[WFM::WebHostHiddenAttribute]
public ref class Catalog sealed
{
private:
	// Using
	using CriticalSection=Concurrency::critical_section;
	using Lock=Concurrency::critical_section::scoped_lock;
	using Object=Platform::Object;
	using CatalogIterator=Details::Catalog::CatalogIterator;

internal:
	// Friends
	friend CatalogIterator;

public:
	// Con-/Destructors
	Catalog(): uItCount(0) {}
	Catalog(Catalog^ Catalog): cIndex(Catalog->cIndex), uItCount(0) {}

	// Access
	CatalogIterator^ At(UINT64 Position);
	bool Contains(Platform::Guid Id);
	property UINT64 Count { UINT64 get(); }
	CatalogIterator^ Find(Platform::Guid Id);
	CatalogIterator^ First();
	Object^ Get(Platform::Guid Id);
	CatalogIterator^ Last();

	// Modification
	bool Add(Platform::Guid Id, Object^ Value);
	VOID Clear();
	bool Remove(Platform::Guid Id);
	VOID RemoveAt(UINT64 Position);
	VOID Set(Platform::Guid Id, Object^ Value);

private:
	// Common
	CriticalSection cCriticalSection;
	Index<Guid, Object^> cIndex;
	UINT uItCount;
};

}
