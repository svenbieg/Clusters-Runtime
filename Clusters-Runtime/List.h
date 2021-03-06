//========
// List.h
//========

#pragma once


//=======
// Using
//=======

#include "ListIterator.h"

namespace WFM=Windows::Foundation::Metadata;


//===========
// Namespace
//===========

namespace Clusters {


//======
// List
//======

[Platform::MTAThreadAttribute]
[WFM::MarshalingBehaviorAttribute(WFM::MarshalingType::Agile)]
[WFM::WebHostHiddenAttribute]
public ref class List sealed: public IIterable<Object^>
{
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
	virtual IIterator<Object^>^ First();
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
	Clusters::list<Handle<Object>> cList;
	UINT uItCount;
};

}