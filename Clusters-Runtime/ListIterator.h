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


//===============
// Iterator List
//===============

[Platform::MTAThreadAttribute]
[WFM::MarshalingBehaviorAttribute(WFM::MarshalingType::Agile)]
[WFM::WebHostHiddenAttribute]
public ref class ListIterator sealed: public IIterator<Object^>
{
public:
	// Access
	virtual property Object^ Current { Object^ get(); }
	virtual UINT GetMany(Platform::WriteOnlyArray<Object^>^ items);
	virtual property bool HasCurrent { bool get(); }

	// Modification
	VOID RemoveCurrent();

	// Navigation
	virtual bool MoveNext();
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
	Clusters::list<Handle<Object>>::iterator cIt;
	Clusters::List^ hList;
};

}