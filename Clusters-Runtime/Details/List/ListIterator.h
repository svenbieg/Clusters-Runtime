//================
// ListIterator.h
//================

#pragma once


//=======
// Using
//=======

#include "Clusters\List.h"

namespace WFM=Windows::Foundation::Metadata;

namespace Clusters
{
namespace Runtime
	{
	ref class List;
	}
}


//===========
// Namespace
//===========

namespace Clusters {
	namespace Runtime {
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
private:
	// Using
	using Object=Platform::Object;
	using ScopedLock=Concurrency::critical_section::scoped_lock;

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
	ListIterator(Clusters::Runtime::List^ List);
	ListIterator(Clusters::Runtime::List^ List, UINT64 Position);

private:
	// Con-/Destructors
	ListIterator(): cIt(nullptr, 0) {}
	~ListIterator();

	// Common
	Clusters::List<Object^>::IteratorReadWrite cIt;
	Clusters::Runtime::List^ hList;
};

}}}}
