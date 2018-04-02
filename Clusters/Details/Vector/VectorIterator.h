//==================
// VectorIterator.h
//==================

#pragma once


//=======
// Using
//=======

#include "..\Cluster\Iterator.h"
#include "VectorParentGroup.h"


//===========
// Namespace
//===========

namespace Clusters {
	namespace Details {
		namespace Vector {


//============================
// Base-Class Iterator Vector
//============================

template <class ITEM, UINT _GroupSize, BOOL _ReadOnly>
class VectorIteratorBase: public ::Clusters::Details::Cluster::IteratorReadWrite<ITEM, VectorGroup<ITEM>, VectorItemGroup<ITEM, _GroupSize>, VectorParentGroup<ITEM, _GroupSize>, _ReadOnly>
{
protected:
	// Using
	using GROUP=VectorGroup<ITEM>;
	using ITEMGROUP=VectorItemGroup<ITEM, _GroupSize>;
	using PARENTGROUP=VectorParentGroup<ITEM, _GroupSize>;
	using BASE=::Clusters::Details::Cluster::IteratorReadWrite<ITEM, GROUP, ITEMGROUP, PARENTGROUP, _ReadOnly>;

	// Con-/Destructors
	using BASE::BASE;
};


//=================
// Iterator Vector
//=================

// Iterator Vector Read-Only
template <class ITEM, UINT _GroupSize, BOOL _ReadOnly>
class VectorIterator: public VectorIteratorBase<ITEM, _GroupSize, _ReadOnly>
{
private:
	// Using
	using IT_R=VectorIterator<ITEM, _GroupSize, true>;
	using IT_W=VectorIterator<ITEM, _GroupSize, false>;
	using CLUSTER=::Clusters::Details::Cluster::Cluster<ITEM, GROUP, ITEMGROUP, PARENTGROUP, IT_R, IT_W>;

public:
	// Con-/Destructors
	VectorIterator(IT_R const& It): VectorIteratorBase(It) {}
	VectorIterator(CLUSTER const* Cluster, UINT64 Position): VectorIteratorBase(Cluster, Position) {}
	
	// Access
	ITEM const& GetCurrent()const { return *pCurrent; }
};

// Iterator Vector Read-Write
template <class ITEM, UINT _GroupSize>
class VectorIterator<ITEM, _GroupSize, false>: public VectorIteratorBase<ITEM, _GroupSize, false>
{
private:
	// Using
	using IT_R=VectorIterator<ITEM, _GroupSize, true>;
	using IT_W=VectorIterator<ITEM, _GroupSize, false>;
	using CLUSTER=::Clusters::Details::Cluster::Cluster<ITEM, GROUP, ITEMGROUP, PARENTGROUP, IT_R, IT_W>;

public:
	// Con-/Destructors
	VectorIterator(CLUSTER* Cluster, UINT64 Position): VectorIteratorBase(Cluster, Position) {}
	
	// Access
	ITEM& GetCurrent() { return *pCurrent; }
	ITEM const& GetCurrent()const { return *pCurrent; }
	
	// Modification
	VOID SetCurrent(ITEM const& Item) { *pCurrent=Item; }
};

// Iterator Pointer-Vector Read-Only
template <class ITEM, UINT _GroupSize>
class VectorIterator<ITEM*, _GroupSize, true>: public VectorIteratorBase<ITEM*, _GroupSize, true>
{
private:
	// Using
	using IT_R=VectorIterator<ITEM*, _GroupSize, true>;
	using IT_W=VectorIterator<ITEM*, _GroupSize, false>;
	using CLUSTER=::Clusters::Details::Cluster::Cluster<ITEM*, GROUP, ITEMGROUP, PARENTGROUP, IT_R, IT_W>;

public:
	// Con-/Destructors
	VectorIterator(IT_R const& It): VectorIteratorBase(It) {}
	VectorIterator(CLUSTER const* Cluster, UINT64 Position): VectorIteratorBase(Cluster, Position) {}

	// Access
	inline ITEM* GetCurrent()const { return *pCurrent; }
};

// Iterator Pointer-Vector Read-Write
template <class ITEM, UINT _GroupSize>
class VectorIterator<ITEM*, _GroupSize, false>: public VectorIteratorBase<ITEM*, _GroupSize, false>
{
private:
	// Using
	using IT_R=VectorIterator<ITEM*, _GroupSize, true>;
	using IT_W=VectorIterator<ITEM*, _GroupSize, false>;
	using CLUSTER=::Clusters::Details::Cluster::Cluster<ITEM*, GROUP, ITEMGROUP, PARENTGROUP, IT_R, IT_W>;

public:
	// Con-/Destructors
	VectorIterator(CLUSTER* Cluster, UINT64 Position): VectorIteratorBase(Cluster, Position) {}

	// Access
	inline ITEM* GetCurrent()const { return *pCurrent; }
	
	// Modification
	VOID SetCurrent(ITEM* Item) { PointerAssign(pCurrent, Item); }
};

#ifdef __cplusplus_winrt
// Iterator Handle-Vector Read-Only
template <class ITEM, UINT _GroupSize>
class VectorIterator<ITEM^, _GroupSize, true>: public VectorIteratorBase<ITEM^, _GroupSize, true>
{
private:
	// Using
	using IT_R=VectorIterator<ITEM^, _GroupSize, true>;
	using IT_W=VectorIterator<ITEM^, _GroupSize, false>;
	using CLUSTER=::Clusters::Details::Cluster::Cluster<ITEM^, GROUP, ITEMGROUP, PARENTGROUP, IT_R, IT_W>;

public:
	// Con-/Destructors
	VectorIterator(IT_R const& It): VectorIteratorBase(It) {}
	VectorIterator(CLUSTER const* Cluster, UINT64 Position): VectorIteratorBase(Cluster, Position) {}

	// Access
	inline ITEM^ GetCurrent()const { return *pCurrent; }
};

#endif

#ifdef __cplusplus_winrt
// Iterator Handle-Vector Read-Write
template <class ITEM, UINT _GroupSize>
class VectorIterator<ITEM^, _GroupSize, false>: public VectorIteratorBase<ITEM^, _GroupSize, false>
{
private:
	// Using
	using IT_R=VectorIterator<ITEM^, _GroupSize, true>;
	using IT_W=VectorIterator<ITEM^, _GroupSize, false>;
	using CLUSTER=::Clusters::Details::Cluster::Cluster<ITEM^, GROUP, ITEMGROUP, PARENTGROUP, IT_R, IT_W>;

public:
	// Con-/Destructors
	VectorIterator(CLUSTER* Cluster, UINT64 Position): VectorIteratorBase(Cluster, Position) {}

	// Access
	inline ITEM^ GetCurrent()const { return *pCurrent; }
	
	// Modification
	VOID SetCurrent(ITEM^ Item) { *pCurrent=Item; }
};
#endif

// Iterator String-Vector Read-Only
template <class CHAR, BOOL _Alloc, UINT _GroupSize>
class VectorIterator<String<CHAR, _Alloc>, _GroupSize, true>: public VectorIteratorBase<String<CHAR, _Alloc>, _GroupSize, true>
{
private:
	// Using
	using IT_R=VectorIterator<String<CHAR, _Alloc>, _GroupSize, true>;
	using IT_W=VectorIterator<String<CHAR, _Alloc>, _GroupSize, false>;
	using CLUSTER=::Clusters::Details::Cluster::Cluster<String<CHAR, _Alloc>, GROUP, ITEMGROUP, PARENTGROUP, IT_R, IT_W>;

public:
	// Con-/Destructors
	VectorIterator(IT_R const& It): VectorIteratorBase(It) {}
	VectorIterator(CLUSTER const* Cluster, UINT64 Position): VectorIteratorBase(Cluster, Position) {}

	// Access
	inline CHAR const* GetCurrent()const { return pCurrent->Get(); }
};

// Iterator String-Vector Read-Write
template <class CHAR, UINT _GroupSize>
class VectorIterator<String<CHAR, true>, _GroupSize, false>: public VectorIteratorBase<String<CHAR, true>, _GroupSize, false>
{
private:
	// Using
	using IT_R=VectorIterator<String<CHAR, true>, _GroupSize, true>;
	using IT_W=VectorIterator<String<CHAR, true>, _GroupSize, false>;
	using CLUSTER=::Clusters::Details::Cluster::Cluster<String<CHAR, true>, GROUP, ITEMGROUP, PARENTGROUP, IT_R, IT_W>;

public:
	// Con-/Destructors
	VectorIterator(CLUSTER* Cluster, UINT64 Position): VectorIteratorBase(Cluster, Position) {}

	// Access
	inline CHAR const* GetCurrent()const { return pCurrent->Get(); }
	
	// Modification
	inline UINT SetCurrent(CHAR const* Value, UINT Length=0) { return pCurrent->Set(Value, Length); }
};

// Iterator Shared String-Vector Read-Write
template <class CHAR, UINT _GroupSize>
class VectorIterator<String<CHAR, false>, _GroupSize, false>: public VectorIteratorBase<String<CHAR, false>, _GroupSize, false>
{
private:
	// Using
	using IT_R=VectorIterator<String<CHAR, false>, _GroupSize, true>;
	using IT_W=VectorIterator<String<CHAR, false>, _GroupSize, false>;
	using CLUSTER=::Clusters::Details::Cluster::Cluster<String<CHAR, false>, GROUP, ITEMGROUP, PARENTGROUP, IT_R, IT_W>;

public:
	// Con-/Destructors
	VectorIterator(CLUSTER* Cluster, UINT64 Position): VectorIteratorBase(Cluster, Position) {}

	// Access
	inline CHAR const* GetCurrent()const { return pCurrent->Get(); }
	
	// Modification
	inline VOID SetCurrent(CHAR const* Value) { pCurrent->Set(Value); }
};

}}}
