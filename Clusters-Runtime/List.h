//========
// List.h
//========

#pragma once


//=======
// Using
//=======

#include "Clusters\List.h"
#include "Details\List\ListIterator.h"

namespace WFM=Windows::Foundation::Metadata;


//===========
// Namespace
//===========

namespace Clusters {
	namespace Runtime {


//======
// List
//======

[Platform::MTAThreadAttribute]
[WFM::MarshalingBehaviorAttribute(WFM::MarshalingType::Agile)]
[WFM::WebHostHiddenAttribute]
public ref class List sealed
{
private:
	// Using
	using CriticalSection=Concurrency::critical_section;
	using ListIterator=Details::List::ListIterator;
	using Lock=Concurrency::critical_section::scoped_lock;
	using Object=Platform::Object;

internal:
	// Friends
	friend ListIterator;

public:
	// Con-/Destructors
	List(): uItCount(0) {}
	List(List^ List): cList(List->cList), uItCount(0) {}
	
	// Access
	ListIterator^ At(UINT64 Position);
	property UINT64 Count { UINT64 get(); }
	ListIterator^ First();
	Object^ GetAt(UINT64 Position);
	ListIterator^ Last();

	// Modification
	VOID Append(Object^ Item);
	VOID Clear();
	VOID InsertAt(UINT64 Position, Object^ Item);
	VOID RemoveAt(UINT64 Position);

private:
	// Common
	CriticalSection cCriticalSection;
	Clusters::List<Object^> cList;
	UINT uItCount;
};

}}