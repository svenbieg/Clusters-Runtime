//=======
// Map.h
//=======

#pragma once


//=======
// Using
//=======

#include "clusters\index.h"
#include "MapIterator.h"

namespace WFM=Windows::Foundation::Metadata;


//===========
// Namespace
//===========

namespace Clusters {


//=====
// Map
//=====

[Platform::MTAThreadAttribute]
[WFM::MarshalingBehaviorAttribute(WFM::MarshalingType::Agile)]
[WFM::WebHostHiddenAttribute]
public ref class Map sealed
{
internal:
	// Friends
	friend MapIterator;

public:
	// Con-/Destructors
	Map(): uItCount(0) {}
	Map(Map^ Map): cIndex(Map->cIndex), uItCount(0) {}

	// Access
	MapIterator^ At(UINT64 Position);
	bool Contains(String^ Key);
	property UINT64 Count { UINT64 get(); }
	MapIterator^ Find(String^ Key);
	MapIterator^ First();
	Object^ Get(String^ Key);
	MapIterator^ Last();

	// Modification
	bool Add(String^ Key, Object^ Value);
	VOID Clear();
	bool Remove(String^ Key);
	VOID Set(String^ Key, Object^ Value);

private:
	// Common
	CriticalSection cCriticalSection;
	clusters::index<Handle<String>, Handle<Object>> cIndex;
	UINT uItCount;
};

}