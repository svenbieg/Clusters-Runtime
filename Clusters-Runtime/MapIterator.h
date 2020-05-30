//===============
// MapIterator.h
//===============

#pragma once


//=======
// Using
//=======

#include "Clusters\slist.h"
#include "MapItem.h"

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


//==============
// Iterator Map
//==============

[Platform::MTAThreadAttribute]
[WFM::MarshalingBehaviorAttribute(WFM::MarshalingType::Agile)]
[WFM::WebHostHiddenAttribute]
public ref class MapIterator sealed: public IIterator<MapItem^>
{
internal:
	// Con-/Destructors
	MapIterator(Clusters::Map^ Map);
	MapIterator(Clusters::Map^ Map, UINT64 Position);
	MapIterator(Clusters::Map^ Map, String^ Key);

public:
	// Access
	virtual property MapItem^ Current { MapItem^ get(); }
	virtual UINT GetMany(Platform::WriteOnlyArray<MapItem^>^ items);
	virtual property bool HasCurrent { bool get(); }

	// Modification
	VOID RemoveCurrent();

	// Navigation
	bool Find(String^ Key);
	virtual bool MoveNext();
	bool MovePrevious();
	property UINT64 Position { UINT64 get(); VOID set(UINT64 Position); }

private:
	// Con-/Destructors
	MapIterator(): cIt(nullptr, 0) {}
	~MapIterator();

	// Common
	Clusters::slist<Handle<String>, Handle<Object>>::iterator cIt;
	Clusters::Map^ hMap;
};

}