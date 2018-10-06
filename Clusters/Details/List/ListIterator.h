//================
// ListIterator.h
//================

#pragma once


//=======
// Using
//=======

#include "..\Cluster\Iterator.h"
#include "ListParentGroup.h"


//===========
// Namespace
//===========

namespace Clusters {
	namespace Details {
		namespace List {


//==========================
// Base-Class Iterator List
//==========================

template <class ITEM, UINT _GroupSize, BOOL _ReadOnly>
class ListIteratorBase: public ::Clusters::Details::Cluster::IteratorShared<ITEM, ListGroup<ITEM>, ListItemGroup<ITEM, _GroupSize>, ListParentGroup<ITEM, _GroupSize>, _ReadOnly>
{
protected:
	// Using
	using GROUP=ListGroup<ITEM>;
	using ITEMGROUP=ListItemGroup<ITEM, _GroupSize>;
	using PARENTGROUP=ListParentGroup<ITEM, _GroupSize>;
	using BASE=::Clusters::Details::Cluster::IteratorShared<ITEM, GROUP, ITEMGROUP, PARENTGROUP, _ReadOnly>;

	// Con-/Destructors
	using BASE::BASE;
};


//===============
// Iterator List
//===============

// Iterator List Read-Only
template <class ITEM, UINT _GroupSize, BOOL _ReadOnly>
class ListIterator: public ListIteratorBase<ITEM, _GroupSize, _ReadOnly>
{
private:
	// Using
	using IT_R=ListIterator<ITEM, _GroupSize, true>;
	using IT_W=ListIterator<ITEM, _GroupSize, false>;
	using CLUSTER=::Clusters::Details::Cluster::Cluster<ITEM, GROUP, ITEMGROUP, PARENTGROUP, IT_R, IT_W>;

public:
	// Con-/Destructors
	ListIterator(IT_R const& It): ListIteratorBase(It) {}
	ListIterator(IT_W const& It): ListIteratorBase(It) {}
	ListIterator(CLUSTER const* Cluster, UINT64 Position): ListIteratorBase(Cluster, Position) {}
	
	// Access
	inline ITEM GetCurrent()const { return *pCurrent; }
};

// Iterator List Read-Write
template <class ITEM, UINT _GroupSize>
class ListIterator<ITEM, _GroupSize, false>: public ListIteratorBase<ITEM, _GroupSize, false>
{
private:
	// Using
	using IT_R=ListIterator<ITEM, _GroupSize, true>;
	using IT_W=ListIterator<ITEM, _GroupSize, false>;
	using CLUSTER=::Clusters::Details::Cluster::Cluster<ITEM, GROUP, ITEMGROUP, PARENTGROUP, IT_R, IT_W>;

public:
	// Con-/Destructors
	ListIterator(IT_W const& It): ListIteratorBase(It) {}
	ListIterator(CLUSTER* Cluster, UINT64 Position): ListIteratorBase(Cluster, Position) {}
	
	// Access
	ITEM& GetCurrent() { return *pCurrent; }
	ITEM const& GetCurrent()const { return *pCurrent; }
	
	// Modification
	VOID SetCurrent(ITEM const& Item) { *pCurrent=Item; }
};

// Iterator Pointer-List Read-Only
template <class ITEM, UINT _GroupSize>
class ListIterator<ITEM*, _GroupSize, true>: public ListIteratorBase<ITEM*, _GroupSize, true>
{
private:
	// Using
	using IT_R=ListIterator<ITEM*, _GroupSize, true>;
	using IT_W=ListIterator<ITEM*, _GroupSize, false>;
	using CLUSTER=::Clusters::Details::Cluster::Cluster<ITEM*, GROUP, ITEMGROUP, PARENTGROUP, IT_R, IT_W>;

public:
	// Con-/Destructors
	ListIterator(IT_R const& It): ListIteratorBase(It) {}
	ListIterator(IT_W const& It): ListIteratorBase(It) {}
	ListIterator(CLUSTER const* Cluster, UINT64 Position): ListIteratorBase(Cluster, Position) {}

	// Access
	inline ITEM* GetCurrent()const { return *pCurrent; }
};

// Iterator Pointer-List Read-Write
template <class ITEM, UINT _GroupSize>
class ListIterator<ITEM*, _GroupSize, false>: public ListIteratorBase<ITEM*, _GroupSize, false>
{
private:
	// Using
	using IT_R=ListIterator<ITEM*, _GroupSize, true>;
	using IT_W=ListIterator<ITEM*, _GroupSize, false>;
	using CLUSTER=::Clusters::Details::Cluster::Cluster<ITEM*, GROUP, ITEMGROUP, PARENTGROUP, IT_R, IT_W>;

public:
	// Con-/Destructors
	ListIterator(IT_W const& It): ListIteratorBase(It) {}
	ListIterator(CLUSTER* Cluster, UINT64 Position): ListIteratorBase(Cluster, Position) {}

	// Access
	inline ITEM* GetCurrent()const { return *pCurrent; }
	
	// Modification
	VOID SetCurrent(ITEM* Item) { PointerAssign(pCurrent, Item); }
};

#ifdef __cplusplus_winrt
// Iterator Handle-List Read-Only
template <class ITEM, UINT _GroupSize>
class ListIterator<ITEM^, _GroupSize, true>: public ListIteratorBase<ITEM^, _GroupSize, true>
{
private:
	// Using
	using IT_R=ListIterator<ITEM^, _GroupSize, true>;
	using IT_W=ListIterator<ITEM^, _GroupSize, false>;
	using CLUSTER=::Clusters::Details::Cluster::Cluster<ITEM^, GROUP, ITEMGROUP, PARENTGROUP, IT_R, IT_W>;

public:
	// Con-/Destructors
	ListIterator(IT_R const& It): ListIteratorBase(It) {}
	ListIterator(IT_W const& It): ListIteratorBase(It) {}
	ListIterator(CLUSTER const* Cluster, UINT64 Position): ListIteratorBase(Cluster, Position) {}

	// Access
	inline ITEM^ GetCurrent()const { return *pCurrent; }
};

#endif

#ifdef __cplusplus_winrt
// Iterator Handle-List Read-Write
template <class ITEM, UINT _GroupSize>
class ListIterator<ITEM^, _GroupSize, false>: public ListIteratorBase<ITEM^, _GroupSize, false>
{
private:
	// Using
	using IT_R=ListIterator<ITEM^, _GroupSize, true>;
	using IT_W=ListIterator<ITEM^, _GroupSize, false>;
	using CLUSTER=::Clusters::Details::Cluster::Cluster<ITEM^, GROUP, ITEMGROUP, PARENTGROUP, IT_R, IT_W>;

public:
	// Con-/Destructors
	ListIterator(IT_W const& It): ListIteratorBase(It) {}
	ListIterator(CLUSTER* Cluster, UINT64 Position): ListIteratorBase(Cluster, Position) {}

	// Access
	inline ITEM^ GetCurrent()const { return *pCurrent; }
	
	// Modification
	VOID SetCurrent(ITEM^ Item) { *pCurrent=Item; }
};
#endif

// Iterator String-List Read-Only
template <class CHAR, BOOL _Alloc, UINT _GroupSize>
class ListIterator<String<CHAR, _Alloc>, _GroupSize, true>: public ListIteratorBase<String<CHAR, _Alloc>, _GroupSize, true>
{
private:
	// Using
	using IT_R=ListIterator<String<CHAR, _Alloc>, _GroupSize, true>;
	using IT_W=ListIterator<String<CHAR, _Alloc>, _GroupSize, false>;
	using CLUSTER=::Clusters::Details::Cluster::Cluster<String<CHAR, _Alloc>, GROUP, ITEMGROUP, PARENTGROUP, IT_R, IT_W>;

public:
	// Con-/Destructors
	ListIterator(IT_R const& It): ListIteratorBase(It) {}
	ListIterator(IT_W const& It): ListIteratorBase(It) {}
	ListIterator(CLUSTER const* Cluster, UINT64 Position): ListIteratorBase(Cluster, Position) {}

	// Access
	inline CHAR const* GetCurrent()const { return pCurrent->Get(); }
};

// Iterator String-List Read-Write
template <class CHAR, UINT _GroupSize>
class ListIterator<String<CHAR, true>, _GroupSize, false>: public ListIteratorBase<String<CHAR, true>, _GroupSize, false>
{
private:
	// Using
	using IT_R=ListIterator<String<CHAR, true>, _GroupSize, true>;
	using IT_W=ListIterator<String<CHAR, true>, _GroupSize, false>;
	using CLUSTER=::Clusters::Details::Cluster::Cluster<String<CHAR, true>, GROUP, ITEMGROUP, PARENTGROUP, IT_R, IT_W>;

public:
	// Con-/Destructors
	ListIterator(IT_W const& It): ListIteratorBase(It) {}
	ListIterator(CLUSTER* Cluster, UINT64 Position): ListIteratorBase(Cluster, Position) {}

	// Access
	inline CHAR const* GetCurrent()const { return pCurrent->Get(); }
	
	// Modification
	inline UINT SetCurrent(CHAR const* Value, UINT Length=0) { return pCurrent->Set(Value, Length); }
};

// Iterator Shared String-List Read-Write
template <class CHAR, UINT _GroupSize>
class ListIterator<String<CHAR, false>, _GroupSize, false>: public ListIteratorBase<String<CHAR, false>, _GroupSize, false>
{
private:
	// Using
	using IT_R=ListIterator<String<CHAR, false>, _GroupSize, true>;
	using IT_W=ListIterator<String<CHAR, false>, _GroupSize, false>;
	using CLUSTER=::Clusters::Details::Cluster::Cluster<String<CHAR, false>, GROUP, ITEMGROUP, PARENTGROUP, IT_R, IT_W>;

public:
	// Con-/Destructors
	ListIterator(IT_W const& It): ListIteratorBase(It) {}
	ListIterator(CLUSTER* Cluster, UINT64 Position): ListIteratorBase(Cluster, Position) {}

	// Access
	inline CHAR const* GetCurrent()const { return pCurrent->Get(); }
	
	// Modification
	inline VOID SetCurrent(CHAR const* Value) { pCurrent->Set(Value); }
};

}}}
