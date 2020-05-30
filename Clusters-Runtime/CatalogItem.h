//===============
// CatalogItem.h
//===============

#pragma once


//=======
// Using
//=======

namespace WFM=Windows::Foundation::Metadata;


//===========
// Namespace
//===========

namespace Clusters {


//======================
// Forward-Declarations
//======================

ref class Catalog;


//==============
// Catalog-Item
//==============

[Platform::MTAThreadAttribute]
[WFM::MarshalingBehaviorAttribute(WFM::MarshalingType::Agile)]
[WFM::WebHostHiddenAttribute]
public ref class CatalogItem sealed
{
internal:
	// Con-/Destructors
	CatalogItem(Catalog^ Catalog, Platform::Guid Id, Object^ Value);

public:
	// Common
	property Platform::Guid Id { Platform::Guid get(); }
	property Object^ Value { Object^ get(); VOID set(Object^ Value); }

private:
	// Common
	Catalog^ hCatalog;
	Object^ hValue;
	Platform::Guid uId;
};

}