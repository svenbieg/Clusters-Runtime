//===============
// MapIterator.h
//===============

#pragma once
#include "pch.h"


//=======
// Using
//=======

#include "Clusters\Index.h"

namespace WFM=Windows::Foundation::Metadata;


//======================
// Forward-Declarations
//======================

namespace Clusters
{
ref class Map;
}


//===========
// Namespace
//===========

namespace Clusters {
	namespace Details {
		namespace Map {


//==============
// Iterator Map
//==============

[Platform::MTAThreadAttribute]
[WFM::MarshalingBehaviorAttribute(WFM::MarshalingType::Agile)]
[WFM::WebHostHiddenAttribute]
public ref class MapIterator sealed
{
private:
	// Using
	using Lock=Concurrency::critical_section::scoped_lock;
	using Object=Platform::Object;
	using String=Platform::String;

public:
	// Access
	property String^ CurrentKey { String^ get(); }
	property Object^ CurrentValue { Object^ get(); VOID set(Object^ Value); }
	property bool HasCurrent { bool get(); }

	// Modification
	VOID RemoveCurrent();

	// Navigation
	bool Find(String^ Key);
	bool MoveNext();
	bool MovePrevious();
	property UINT64 Position { UINT64 get(); VOID set(UINT64 Position); }

internal:
	// Con-/Destructors
	MapIterator(Clusters::Map^ Map);
	MapIterator(Clusters::Map^ Map, UINT64 Position);
	MapIterator(Clusters::Map^ Map, String^ Key);

private:
	// Con-/Destructors
	MapIterator(): cIt(nullptr, 0) {}
	~MapIterator();

	// Common
	Clusters::Index<String^, Object^>::IteratorReadWrite cIt;
	Clusters::Map^ hMap;
};

}}}
