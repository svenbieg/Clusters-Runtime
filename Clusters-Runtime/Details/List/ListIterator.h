//================
// ListIterator.h
//================

#pragma once


//=======
// Using
//=======

#include "Clusters\list.h"

namespace WFM=Windows::Foundation::Metadata;

namespace Clusters
{
ref class List;
}


//===========
// Namespace
//===========

namespace Clusters {
	namespace Details {
		namespace List {


//===============
// Iterator List
//===============

[Platform::MTAThreadAttribute]
[WFM::MarshalingBehaviorAttribute(WFM::MarshalingType::Agile)]
[WFM::WebHostHiddenAttribute]
public ref class ListIterator sealed
{
public:
	// Access
	property Object^ Current { Object^ get(); }
	property bool HasCurrent { bool get(); }

	// Modification
	VOID RemoveCurrent();

	// Navigation
	bool MoveNext();
	bool MovePrevious();
	property UINT64 Position { UINT64 get(); VOID set(UINT64 Position); }

internal:
	// Con-/Destructors
	ListIterator(Clusters::List^ List);
	ListIterator(Clusters::List^ List, UINT64 Position);

private:
	// Con-/Destructors
	ListIterator(): cIt(nullptr, 0) {}
	~ListIterator();

	// Common
	clusters::list<Handle<Object>>::iterator cIt;
	Clusters::List^ hList;
};

}}}