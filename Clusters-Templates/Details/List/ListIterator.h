//================
// ListIterator.h
//================

#pragma once


//=======
// Using
//=======

#include "../Cluster/Iterator.h"
#include "ListParentGroup.h"


//===========
// Namespace
//===========

namespace Clusters {
	namespace Templates {
		namespace Details {
			namespace List {


//===============
// Iterator List
//===============

// Iterator List Read-Only
template <class _Item, UINT _GroupSize, BOOL _ReadOnly>
class ListIterator: public ::Clusters::Templates::Details::Cluster::IteratorShared<_Item, ListGroup<_Item>, ListItemGroup<_Item, _GroupSize>, ListParentGroup<_Item, _GroupSize>, _ReadOnly>
{
private:
	// Using
	using _ListGroup=ListGroup<_Item>;
	using _ListItemGroup=ListItemGroup<_Item, _GroupSize>;
	using _ListParentGroup=ListParentGroup<_Item, _GroupSize>;
	using _It=ListIterator<_Item, _GroupSize, _ReadOnly>;
	using _ItR=ListIterator<_Item, _GroupSize, true>;
	using _ItW=ListIterator<_Item, _GroupSize, false>;
	using _IteratorShared=::Clusters::Templates::Details::Cluster::IteratorShared<_Item, _ListGroup, _ListItemGroup, _ListParentGroup, _ReadOnly>;
	using _Cluster=::Clusters::Templates::Details::Cluster::Cluster<_Item, _ListGroup, _ListItemGroup, _ListParentGroup, _ItR, _ItW>;

public:
	// Con-/Destructors
	ListIterator(_ItR const& It): _IteratorShared(It) {}
	ListIterator(_ItW const& It): _IteratorShared(It) {}
	ListIterator(_Cluster const* Cluster, SIZE_T Position): _IteratorShared(Cluster) { this->SetPosition(Position); }

	// Access
	inline _Item GetCurrent()const { return *(this->pCurrent); }

	// Assignment
	inline _It& operator=(_It const& It) { this->template Assign<_It>(It); return *this; }
};

// Iterator List Read-Write
template <class _Item, UINT _GroupSize>
class ListIterator<_Item, _GroupSize, false>: public ::Clusters::Templates::Details::Cluster::IteratorShared<_Item, ListGroup<_Item>, ListItemGroup<_Item, _GroupSize>, ListParentGroup<_Item, _GroupSize>, false>
{
private:
	// Using
	using _ListGroup=ListGroup<_Item>;
	using _ListItemGroup=ListItemGroup<_Item, _GroupSize>;
	using _ListParentGroup=ListParentGroup<_Item, _GroupSize>;
	using _It=ListIterator<_Item, _GroupSize, false>;
	using _ItR=ListIterator<_Item, _GroupSize, true>;
	using _ItW=ListIterator<_Item, _GroupSize, false>;
	using _IteratorShared=::Clusters::Templates::Details::Cluster::IteratorShared<_Item, _ListGroup, _ListItemGroup, _ListParentGroup, false>;
	using _Cluster=::Clusters::Templates::Details::Cluster::Cluster<_Item, _ListGroup, _ListItemGroup, _ListParentGroup, _ItR, _ItW>;

public:
	// Con-/Destructors
	ListIterator(_ItW const& It): _IteratorShared(It) {}
	ListIterator(_Cluster* Cluster, SIZE_T Position): _IteratorShared(Cluster) { this->SetPosition(Position); }
	
	// Access
	_Item& GetCurrent() { return *(this->pCurrent); }
	_Item const& GetCurrent()const { return *(this->pCurrent); }

	// Assignment
	inline _It& operator=(_It const& It) { this->template Assign<_It>(It); return *this; }

	// Modification
	VOID SetCurrent(_Item const& Item) { *(this->pCurrent)=Item; }
};

// Iterator Pointer-List Read-Only
template <class _Item, UINT _GroupSize>
class ListIterator<_Item*, _GroupSize, true>: public ::Clusters::Templates::Details::Cluster::IteratorShared<_Item*, ListGroup<_Item*>, ListItemGroup<_Item*, _GroupSize>, ListParentGroup<_Item*, _GroupSize>, true>
{
private:
	// Using
	using _ListGroup=ListGroup<_Item*>;
	using _ListItemGroup=ListItemGroup<_Item*, _GroupSize>;
	using _ListParentGroup=ListParentGroup<_Item*, _GroupSize>;
	using _It=ListIterator<_Item*, _GroupSize, true>;
	using _ItR=ListIterator<_Item*, _GroupSize, true>;
	using _ItW=ListIterator<_Item*, _GroupSize, false>;
	using _IteratorShared=::Clusters::Templates::Details::Cluster::IteratorShared<_Item*, _ListGroup, _ListItemGroup, _ListParentGroup, true>;
	using _Cluster=::Clusters::Templates::Details::Cluster::Cluster<_Item*, _ListGroup, _ListItemGroup, _ListParentGroup, _ItR, _ItW>;

public:
	// Con-/Destructors
	ListIterator(_ItR const& It): _IteratorShared(It) {}
	ListIterator(_ItW const& It): _IteratorShared(It) {}
	ListIterator(_Cluster const* Cluster, SIZE_T Position): _IteratorShared(Cluster) { this->SetPosition(Position); }

	// Access
	inline _Item* GetCurrent()const { return *(this->pCurrent); }

	// Assignment
	inline _It& operator=(_It const& It) { this->template Assign<_It>(It); return *this; }
};

// Iterator Pointer-List Read-Write
template <class _Item, UINT _GroupSize>
class ListIterator<_Item*, _GroupSize, false>: public ::Clusters::Templates::Details::Cluster::IteratorShared<_Item*, ListGroup<_Item*>, ListItemGroup<_Item*, _GroupSize>, ListParentGroup<_Item*, _GroupSize>, false>
{
private:
	// Using
	using _ListGroup=ListGroup<_Item*>;
	using _ListItemGroup=ListItemGroup<_Item*, _GroupSize>;
	using _ListParentGroup=ListParentGroup<_Item*, _GroupSize>;
	using _It=ListIterator<_Item*, _GroupSize, false>;
	using _ItR=ListIterator<_Item*, _GroupSize, true>;
	using _ItW=ListIterator<_Item*, _GroupSize, false>;
	using _IteratorShared=::Clusters::Templates::Details::Cluster::IteratorShared<_Item*, _ListGroup, _ListItemGroup, _ListParentGroup, false>;
	using _Cluster=::Clusters::Templates::Details::Cluster::Cluster<_Item*, _ListGroup, _ListItemGroup, _ListParentGroup, _ItR, _ItW>;

public:
	// Con-/Destructors
	ListIterator(_ItW const& It): _IteratorShared(It) {}
	ListIterator(_Cluster* Cluster, SIZE_T Position): _IteratorShared(Cluster) { this->SetPosition(Position); }

	// Access
	inline _Item* GetCurrent()const { return *(this->pCurrent); }

	// Assignment
	inline _It& operator=(_It const& It) { this->template Assign<_It>(It); return *this; }

	// Modification
	VOID SetCurrent(_Item* Item) { PointerAssign(this->pCurrent, Item); }
};

#ifdef __cplusplus_winrt
// Iterator Handle-List Read-Only
template <class _Item, UINT _GroupSize>
class ListIterator<_Item^, _GroupSize, true>: public ::Clusters::Templates::Details::Cluster::IteratorShared<_Item^, ListGroup<_Item^>, ListItemGroup<_Item^, _GroupSize>, ListParentGroup<_Item^, _GroupSize>, true>
{
private:
	// Using
	using _ListGroup=ListGroup<_Item^>;
	using _ListItemGroup=ListItemGroup<_Item^, _GroupSize>;
	using _ListParentGroup=ListParentGroup<_Item^, _GroupSize>;
	using _It=ListIterator<_Item^, _GroupSize, true>;
	using _ItR=ListIterator<_Item^, _GroupSize, true>;
	using _ItW=ListIterator<_Item^, _GroupSize, false>;
	using _IteratorShared=::Clusters::Templates::Details::Cluster::IteratorShared<_Item^, _ListGroup, _ListItemGroup, _ListParentGroup, true>;
	using _Cluster=::Clusters::Templates::Details::Cluster::Cluster<_Item^, _ListGroup, _ListItemGroup, _ListParentGroup, _ItR, _ItW>;

public:
	// Con-/Destructors
	ListIterator(_ItR const& It): _IteratorShared(It) {}
	ListIterator(_ItW const& It): _IteratorShared(It) {}
	ListIterator(_Cluster const* Cluster, SIZE_T Position): _IteratorShared(Cluster) { this->SetPosition(Position); }

	// Access
	inline _Item^ GetCurrent()const { return *(this->pCurrent); }

	// Assignment
	inline _It& operator=(_It const& It) { this->template Assign<_It>(It); return *this; }
};

#endif

#ifdef __cplusplus_winrt
// Iterator Handle-List Read-Write
template <class _Item, UINT _GroupSize>
class ListIterator<_Item^, _GroupSize, false>: public ::Clusters::Templates::Details::Cluster::IteratorShared<_Item^, ListGroup<_Item^>, ListItemGroup<_Item^, _GroupSize>, ListParentGroup<_Item^, _GroupSize>, false>
{
private:
	// Using
	using _ListGroup=ListGroup<_Item^>;
	using _ListItemGroup=ListItemGroup<_Item^, _GroupSize>;
	using _ListParentGroup=ListParentGroup<_Item^, _GroupSize>;
	using _It=ListIterator<_Item^, _GroupSize, false>;
	using _ItR=ListIterator<_Item^, _GroupSize, true>;
	using _ItW=ListIterator<_Item^, _GroupSize, false>;
	using _IteratorShared=::Clusters::Templates::Details::Cluster::IteratorShared<_Item^, _ListGroup, _ListItemGroup, _ListParentGroup, false>;
	using _Cluster=::Clusters::Templates::Details::Cluster::Cluster<_Item^, _ListGroup, _ListItemGroup, _ListParentGroup, _ItR, _ItW>;

public:
	// Con-/Destructors
	ListIterator(_ItW const& It): _IteratorShared(It) {}
	ListIterator(_Cluster* Cluster, SIZE_T Position): _IteratorShared(Cluster) { this->SetPosition(Position); }

	// Access
	inline _Item^ GetCurrent()const { return *(this->pCurrent); }

	// Assignment
	inline _It& operator=(_It const& It) { this->template Assign<_It>(It); return *this; }

	// Modification
	VOID SetCurrent(_Item^ Item) { *(this->pCurrent)=Item; }
};
#endif

// Iterator String-List Read-Only
template <class _Char, BOOL _Alloc, UINT _GroupSize>
class ListIterator<StringItem<_Char, _Alloc>, _GroupSize, true>: public ::Clusters::Templates::Details::Cluster::IteratorShared<StringItem<_Char, _Alloc>, ListGroup<StringItem<_Char, _Alloc>>, ListItemGroup<StringItem<_Char, _Alloc>, _GroupSize>, ListParentGroup<StringItem<_Char, _Alloc>, _GroupSize>, true>
{
private:
	// Using
	using _ListGroup=ListGroup<StringItem<_Char, _Alloc>>;
	using _ListItemGroup=ListItemGroup<StringItem<_Char, _Alloc>, _GroupSize>;
	using _ListParentGroup=ListParentGroup<StringItem<_Char, _Alloc>, _GroupSize>;
	using _It=ListIterator<StringItem<_Char, _Alloc>, _GroupSize, true>;
	using _ItR=ListIterator<StringItem<_Char, _Alloc>, _GroupSize, true>;
	using _ItW=ListIterator<StringItem<_Char, _Alloc>, _GroupSize, false>;
	using _IteratorShared=::Clusters::Templates::Details::Cluster::IteratorShared<StringItem<_Char, _Alloc>, _ListGroup, _ListItemGroup, _ListParentGroup, true>;
	using _Cluster=::Clusters::Templates::Details::Cluster::Cluster<StringItem<_Char, _Alloc>, _ListGroup, _ListItemGroup, _ListParentGroup, _ItR, _ItW>;

public:
	// Con-/Destructors
	ListIterator(_ItR const& It): _IteratorShared(It) {}
	ListIterator(_ItW const& It): _IteratorShared(It) {}
	ListIterator(_Cluster const* Cluster, SIZE_T Position): _IteratorShared(Cluster) { this->SetPosition(Position); }

	// Access
	inline _Char const* GetCurrent()const { return this->pCurrent->Get(); }

	// Assignment
	inline _It& operator=(_It const& It) { this->template Assign<_It>(It); return *this; }
};

// Iterator String-List Read-Write
template <class _Char, UINT _GroupSize>
class ListIterator<StringItem<_Char, true>, _GroupSize, false>: public ::Clusters::Templates::Details::Cluster::IteratorShared<StringItem<_Char, true>, ListGroup<StringItem<_Char, true>>, ListItemGroup<StringItem<_Char, true>, _GroupSize>, ListParentGroup<StringItem<_Char, true>, _GroupSize>, false>
{
private:
	// Using
	using _ListGroup=ListGroup<StringItem<_Char, true>>;
	using _ListItemGroup=ListItemGroup<StringItem<_Char, true>, _GroupSize>;
	using _ListParentGroup=ListParentGroup<StringItem<_Char, true>, _GroupSize>;
	using _It=ListIterator<StringItem<_Char, true>, _GroupSize, false>;
	using _ItR=ListIterator<StringItem<_Char, true>, _GroupSize, true>;
	using _ItW=ListIterator<StringItem<_Char, true>, _GroupSize, false>;
	using _IteratorShared=::Clusters::Templates::Details::Cluster::IteratorShared<StringItem<_Char, true>, _ListGroup, _ListItemGroup, _ListParentGroup, false>;
	using _Cluster=::Clusters::Templates::Details::Cluster::Cluster<StringItem<_Char, true>, _ListGroup, _ListItemGroup, _ListParentGroup, _ItR, _ItW>;

public:
	// Con-/Destructors
	ListIterator(_ItW const& It): _IteratorShared(It) {}
	ListIterator(_Cluster* Cluster, SIZE_T Position): _IteratorShared(Cluster) { this->SetPosition(Position); }

	// Access
	inline _Char const* GetCurrent()const { return this->pCurrent->Get(); }

	// Assignment
	inline _It& operator=(_It const& It) { this->template Assign<_It>(It); return *this; }

	// Modification
	inline UINT SetCurrent(_Char const* Value, UINT Length=0) { return this->pCurrent->Set(Value, Length); }
};

// Iterator Shared String-List Read-Write
template <class _Char, UINT _GroupSize>
class ListIterator<StringItem<_Char, false>, _GroupSize, false>: public ::Clusters::Templates::Details::Cluster::IteratorShared<StringItem<_Char, false>, ListGroup<StringItem<_Char, false>>, ListItemGroup<StringItem<_Char, false>, _GroupSize>, ListParentGroup<StringItem<_Char, false>, _GroupSize>, false>
{
private:
	// Using
	using _ListGroup=ListGroup<StringItem<_Char, false>>;
	using _ListItemGroup=ListItemGroup<StringItem<_Char, false>, _GroupSize>;
	using _ListParentGroup=ListParentGroup<StringItem<_Char, false>, _GroupSize>;
	using _It=ListIterator<StringItem<_Char, false>, _GroupSize, false>;
	using _ItR=ListIterator<StringItem<_Char, false>, _GroupSize, true>;
	using _ItW=ListIterator<StringItem<_Char, false>, _GroupSize, false>;
	using _IteratorShared=::Clusters::Templates::Details::Cluster::IteratorShared<StringItem<_Char, false>, _ListGroup, _ListItemGroup, _ListParentGroup, false>;
	using _Cluster=::Clusters::Templates::Details::Cluster::Cluster<StringItem<_Char, false>, _ListGroup, _ListItemGroup, _ListParentGroup, _ItR, _ItW>;

public:
	// Con-/Destructors
	ListIterator(_ItW const& It): _IteratorShared(It) {}
	ListIterator(_Cluster* Cluster, SIZE_T Position): _IteratorShared(Cluster) { this->SetPosition(Position); }

	// Access
	inline _Char const* GetCurrent()const { return this->pCurrent->Get(); }

	// Assignment
	inline _It& operator=(_It const& It) { this->template Assign<_It>(It); return *this; }

	// Modification
	inline VOID SetCurrent(_Char const* Value) { this->pCurrent->Set(Value); }
};

}}}}
