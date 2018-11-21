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
	namespace Templates {
		namespace Details {
			namespace List {


//==========================
// Base-Class Iterator List
//==========================

template <class _Item, unsigned int _GroupSize, bool _ReadOnly>
class ListIteratorBase: public ::Clusters::Templates::Details::Cluster::IteratorShared<_Item, ListGroup<_Item>, ListItemGroup<_Item, _GroupSize>, ListParentGroup<_Item, _GroupSize>, _ReadOnly>
{
protected:
	// Using
	using _Group=ListGroup<_Item>;
	using _ItemGroup=ListItemGroup<_Item, _GroupSize>;
	using _ParentGroup=ListParentGroup<_Item, _GroupSize>;
	using _Base=::Clusters::Templates::Details::Cluster::IteratorShared<_Item, _Group, _ItemGroup, _ParentGroup, _ReadOnly>;

	// Con-/Destructors
	using _Base::_Base;
};


//===============
// Iterator List
//===============

// Iterator List Read-Only
template <class _Item, unsigned int _GroupSize, bool _ReadOnly>
class ListIterator: public ListIteratorBase<_Item, _GroupSize, _ReadOnly>
{
private:
	// Using
	using _ItR=ListIterator<_Item, _GroupSize, true>;
	using _ItW=ListIterator<_Item, _GroupSize, false>;
	using _Cluster=::Clusters::Templates::Details::Cluster::Cluster<_Item, _Group, _ItemGroup, _ParentGroup, _ItR, _ItW>;

public:
	// Con-/Destructors
	ListIterator(_ItR const& It): ListIteratorBase(It) {}
	ListIterator(_ItW const& It): ListIteratorBase(It) {}
	ListIterator(_Cluster const* Cluster, size_t Position): ListIteratorBase(Cluster, Position) {}
	
	// Access
	inline _Item GetCurrent()const { return *pCurrent; }
};

// Iterator List Read-Write
template <class _Item, unsigned int _GroupSize>
class ListIterator<_Item, _GroupSize, false>: public ListIteratorBase<_Item, _GroupSize, false>
{
private:
	// Using
	using _ItR=ListIterator<_Item, _GroupSize, true>;
	using _ItW=ListIterator<_Item, _GroupSize, false>;
	using _Cluster=::Clusters::Templates::Details::Cluster::Cluster<_Item, _Group, _ItemGroup, _ParentGroup, _ItR, _ItW>;

public:
	// Con-/Destructors
	ListIterator(_ItW const& It): ListIteratorBase(It) {}
	ListIterator(_Cluster* Cluster, size_t Position): ListIteratorBase(Cluster, Position) {}
	
	// Access
	_Item& GetCurrent() { return *pCurrent; }
	_Item const& GetCurrent()const { return *pCurrent; }
	
	// Modification
	void SetCurrent(_Item const& Item) { *pCurrent=Item; }
};

// Iterator Pointer-List Read-Only
template <class _Item, unsigned int _GroupSize>
class ListIterator<_Item*, _GroupSize, true>: public ListIteratorBase<_Item*, _GroupSize, true>
{
private:
	// Using
	using _ItR=ListIterator<_Item*, _GroupSize, true>;
	using _ItW=ListIterator<_Item*, _GroupSize, false>;
	using _Cluster=::Clusters::Templates::Details::Cluster::Cluster<_Item*, _Group, _ItemGroup, _ParentGroup, _ItR, _ItW>;

public:
	// Con-/Destructors
	ListIterator(_ItR const& It): ListIteratorBase(It) {}
	ListIterator(_ItW const& It): ListIteratorBase(It) {}
	ListIterator(_Cluster const* Cluster, size_t Position): ListIteratorBase(Cluster, Position) {}

	// Access
	inline _Item* GetCurrent()const { return *pCurrent; }
};

// Iterator Pointer-List Read-Write
template <class _Item, unsigned int _GroupSize>
class ListIterator<_Item*, _GroupSize, false>: public ListIteratorBase<_Item*, _GroupSize, false>
{
private:
	// Using
	using _ItR=ListIterator<_Item*, _GroupSize, true>;
	using _ItW=ListIterator<_Item*, _GroupSize, false>;
	using _Cluster=::Clusters::Templates::Details::Cluster::Cluster<_Item*, _Group, _ItemGroup, _ParentGroup, _ItR, _ItW>;

public:
	// Con-/Destructors
	ListIterator(_ItW const& It): ListIteratorBase(It) {}
	ListIterator(_Cluster* Cluster, size_t Position): ListIteratorBase(Cluster, Position) {}

	// Access
	inline _Item* GetCurrent()const { return *pCurrent; }
	
	// Modification
	void SetCurrent(_Item* Item) { PointerAssign(pCurrent, Item); }
};

#ifdef __cplusplus_winrt
// Iterator Handle-List Read-Only
template <class _Item, unsigned int _GroupSize>
class ListIterator<_Item^, _GroupSize, true>: public ListIteratorBase<_Item^, _GroupSize, true>
{
private:
	// Using
	using _ItR=ListIterator<_Item^, _GroupSize, true>;
	using _ItW=ListIterator<_Item^, _GroupSize, false>;
	using _Cluster=::Clusters::Templates::Details::Cluster::Cluster<_Item^, _Group, _ItemGroup, _ParentGroup, _ItR, _ItW>;

public:
	// Con-/Destructors
	ListIterator(_ItR const& It): ListIteratorBase(It) {}
	ListIterator(_ItW const& It): ListIteratorBase(It) {}
	ListIterator(_Cluster const* Cluster, size_t Position): ListIteratorBase(Cluster, Position) {}

	// Access
	inline _Item^ GetCurrent()const { return *pCurrent; }
};

#endif

#ifdef __cplusplus_winrt
// Iterator Handle-List Read-Write
template <class _Item, unsigned int _GroupSize>
class ListIterator<_Item^, _GroupSize, false>: public ListIteratorBase<_Item^, _GroupSize, false>
{
private:
	// Using
	using _ItR=ListIterator<_Item^, _GroupSize, true>;
	using _ItW=ListIterator<_Item^, _GroupSize, false>;
	using _Cluster=::Clusters::Templates::Details::Cluster::Cluster<_Item^, _Group, _ItemGroup, _ParentGroup, _ItR, _ItW>;

public:
	// Con-/Destructors
	ListIterator(_ItW const& It): ListIteratorBase(It) {}
	ListIterator(_Cluster* Cluster, size_t Position): ListIteratorBase(Cluster, Position) {}

	// Access
	inline _Item^ GetCurrent()const { return *pCurrent; }
	
	// Modification
	void SetCurrent(_Item^ Item) { *pCurrent=Item; }
};
#endif

// Iterator String-List Read-Only
template <class _Char, bool _Alloc, unsigned int _GroupSize>
class ListIterator<String<_Char, _Alloc>, _GroupSize, true>: public ListIteratorBase<String<_Char, _Alloc>, _GroupSize, true>
{
private:
	// Using
	using _ItR=ListIterator<String<_Char, _Alloc>, _GroupSize, true>;
	using _ItW=ListIterator<String<_Char, _Alloc>, _GroupSize, false>;
	using _Cluster=::Clusters::Templates::Details::Cluster::Cluster<String<_Char, _Alloc>, _Group, _ItemGroup, _ParentGroup, _ItR, _ItW>;

public:
	// Con-/Destructors
	ListIterator(_ItR const& It): ListIteratorBase(It) {}
	ListIterator(_ItW const& It): ListIteratorBase(It) {}
	ListIterator(_Cluster const* Cluster, size_t Position): ListIteratorBase(Cluster, Position) {}

	// Access
	inline _Char const* GetCurrent()const { return pCurrent->Get(); }
};

// Iterator String-List Read-Write
template <class _Char, unsigned int _GroupSize>
class ListIterator<String<_Char, true>, _GroupSize, false>: public ListIteratorBase<String<_Char, true>, _GroupSize, false>
{
private:
	// Using
	using _ItR=ListIterator<String<_Char, true>, _GroupSize, true>;
	using _ItW=ListIterator<String<_Char, true>, _GroupSize, false>;
	using _Cluster=::Clusters::Templates::Details::Cluster::Cluster<String<_Char, true>, _Group, _ItemGroup, _ParentGroup, _ItR, _ItW>;

public:
	// Con-/Destructors
	ListIterator(_ItW const& It): ListIteratorBase(It) {}
	ListIterator(_Cluster* Cluster, size_t Position): ListIteratorBase(Cluster, Position) {}

	// Access
	inline _Char const* GetCurrent()const { return pCurrent->Get(); }
	
	// Modification
	inline unsigned int SetCurrent(_Char const* Value, unsigned int Length=0) { return pCurrent->Set(Value, Length); }
};

// Iterator Shared String-List Read-Write
template <class _Char, unsigned int _GroupSize>
class ListIterator<String<_Char, false>, _GroupSize, false>: public ListIteratorBase<String<_Char, false>, _GroupSize, false>
{
private:
	// Using
	using _ItR=ListIterator<String<_Char, false>, _GroupSize, true>;
	using _ItW=ListIterator<String<_Char, false>, _GroupSize, false>;
	using _Cluster=::Clusters::Templates::Details::Cluster::Cluster<String<_Char, false>, _Group, _ItemGroup, _ParentGroup, _ItR, _ItW>;

public:
	// Con-/Destructors
	ListIterator(_ItW const& It): ListIteratorBase(It) {}
	ListIterator(_Cluster* Cluster, size_t Position): ListIteratorBase(Cluster, Position) {}

	// Access
	inline _Char const* GetCurrent()const { return pCurrent->Get(); }
	
	// Modification
	inline void SetCurrent(_Char const* Value) { pCurrent->Set(Value); }
};

}}}}
