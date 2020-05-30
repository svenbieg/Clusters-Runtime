//===========
// MapItem.h
//===========

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

ref class Map;


//==========
// Map-Item
//==========

[Platform::MTAThreadAttribute]
[WFM::MarshalingBehaviorAttribute(WFM::MarshalingType::Agile)]
[WFM::WebHostHiddenAttribute]
public ref class MapItem sealed
{
internal:
	// Con-/Destructors
	MapItem(Map^ Map, String^ Key, Object^ Value);

public:
	// Common
	property String^ Key { String^ get(); }
	property Object^ Value { Object^ get(); VOID set(Object^ Value); }

private:
	// Common
	String^ hKey;
	Map^ hMap;
	Object^ hValue;
};

}