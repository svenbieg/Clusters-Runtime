//=================
// IndexIterator.h
//=================

#pragma once


//=======
// Using
//=======

#include "../Cluster/Cluster.h"
#include "../Cluster/Iterator.h"
#include "../StringItem.h"
#include "IndexParentGroup.h"


//===========
// Namespace
//===========

namespace Clusters {
	namespace Templates {
		namespace Details {
			namespace Index {


//===========================
// Base-Class Iterator Index
//===========================

template <class _Id, class _Item, UINT _GroupSize, BOOL _ReadOnly>
class IndexIteratorBase: public ::Clusters::Templates::Details::Cluster::IteratorShared<IndexItem<_Id, _Item>, IndexGroup<_Id, _Item>, IndexItemGroup<_Id, _Item, _GroupSize>, IndexParentGroup<_Id, _Item, _GroupSize>, _ReadOnly>
{
private:
	// Using
	using _IndexItem=IndexItem<_Id, _Item>;
	using _IndexGroup=IndexGroup<_Id, _Item>;
	using _IndexGroupPtr=Conditional<_ReadOnly, _IndexGroup const*, _IndexGroup*>;
	using _IndexItemGroup=IndexItemGroup<_Id, _Item, _GroupSize>;
	using _IndexItemGroupPtr=Conditional<_ReadOnly, _IndexItemGroup const*, _IndexItemGroup*>;
	using _IndexParentGroup=IndexParentGroup<_Id, _Item, _GroupSize>;
	using _IndexParentGroupPtr=Conditional<_ReadOnly, _IndexParentGroup const*, _IndexParentGroup*>;
	using _IteratorShared=::Clusters::Templates::Details::Cluster::IteratorShared<_IndexItem, _IndexGroup, _IndexItemGroup, _IndexParentGroup, _ReadOnly>;
	using _IteratorBase=::Clusters::Templates::Details::Cluster::IteratorBase<_IndexItem, _IndexGroup, _IndexItemGroup, _IndexParentGroup, _ReadOnly>;

protected:
	// Con-/Destructors
	using _IteratorShared::_IteratorShared;

	// Common
	template <class... _Params> BOOL FindInternal(_Params... Id)
		{
		this->pCurrent=nullptr;
		BOOL bfound=true;
		_IndexGroupPtr pgroup=this->pCluster->pRoot;
		this->SetLevelCount(pgroup->GetLevel()+1);
		for(UINT u=0; u<this->uLevelCount-1; u++)
			{
			_IndexParentGroupPtr pparent=(_IndexParentGroupPtr)pgroup;
			INT ipos=pparent->Find(Id...);
			if(ipos<0)
				{
				bfound=false;
				ipos++;
				ipos*=-1;
				}
			this->pIts[u].Group=pgroup;
			this->pIts[u].Position=ipos;
			pgroup=pparent->GetChild(ipos);
			}
		_IndexItemGroupPtr pitemgroup=(_IndexItemGroupPtr)pgroup;
		INT ipos=pitemgroup->Find(Id...);
		if(ipos<0)
			{
			bfound=false;
			ipos++;
			ipos*=-1;
			}
		this->pIts[this->uLevelCount-1].Group=pgroup;
		this->pIts[this->uLevelCount-1].Position=ipos;
		this->pCurrent=pitemgroup->GetAt(ipos);
		return bfound;
		}
};


//================
// Iterator Index
//================

// Iterator Index Read-Only
template <class _Id, class _Item, UINT _GroupSize, BOOL _ReadOnly>
class IndexIterator: public IndexIteratorBase<_Id, _Item, _GroupSize, true>
{
private:
	// Using
	using _It=IndexIterator<_Id, _Item, _GroupSize, _ReadOnly>;
	using _ItR=IndexIterator<_Id, _Item, _GroupSize, true>;
	using _ItW=IndexIterator<_Id, _Item, _GroupSize, false>;
	using _IndexIteratorBase=IndexIteratorBase<_Id, _Item, _GroupSize, true>;
	using _IndexItem=IndexItem<_Id, _Item>;
	using _IndexGroup=IndexGroup<_Id, _Item>;
	using _IndexItemGroup=IndexItemGroup<_Id, _Item, _GroupSize>;
	using _IndexParentGroup=IndexParentGroup<_Id, _Item, _GroupSize>;
	using _Cluster=::Clusters::Templates::Details::Cluster::Cluster<_IndexItem, _IndexGroup, _IndexItemGroup, _IndexParentGroup, _ItR, _ItW>;

public:
	// Con-/Destructors
	IndexIterator(_ItR const& It): _IndexIteratorBase(It) {}
	IndexIterator(_ItW const& It): _IndexIteratorBase(It) {}
	IndexIterator(_Cluster const* Cluster, SIZE_T Position): _IndexIteratorBase(Cluster) { this->SetPosition(Position); }
	IndexIterator(_Cluster const* Cluster, SIZE_T, _Id const& Id): _IndexIteratorBase(Cluster) { this->template FindInternal<_Id const&>(Id); }

	// Access
	inline BOOL Find(_Id const& Id) { return this->template FindInternal<_Id const&>(Id); }
	inline _Id const& GetCurrentId()const { return this->pCurrent->GetId(); }
	inline _Item const& GetCurrentItem()const { return this->pCurrent->GetItem(); }

	// Assignment
	inline _It& operator=(_ItR const& It) { this->template Assign<_ItR>(It); return *this; }
	inline _It& operator=(_ItW const& It) { this->template Assign<_ItW>(It); return *this; }
};

// Iterator Index Read-Write
template <class _Id, class _Item, UINT _GroupSize>
class IndexIterator<_Id, _Item, _GroupSize, false>: public IndexIteratorBase<_Id, _Item, _GroupSize, false>
{
private:
	// Using
	using _It=IndexIterator<_Id, _Item, _GroupSize, false>;
	using _ItR=IndexIterator<_Id, _Item, _GroupSize, true>;
	using _ItW=IndexIterator<_Id, _Item, _GroupSize, false>;
	using _IndexIteratorBase=IndexIteratorBase<_Id, _Item, _GroupSize, false>;
	using _IndexItem=IndexItem<_Id, _Item>;
	using _IndexGroup=IndexGroup<_Id, _Item>;
	using _IndexItemGroup=IndexItemGroup<_Id, _Item, _GroupSize>;
	using _IndexParentGroup=IndexParentGroup<_Id, _Item, _GroupSize>;
	using _Cluster=::Clusters::Templates::Details::Cluster::Cluster<_IndexItem, _IndexGroup, _IndexItemGroup, _IndexParentGroup, _ItR, _ItW>;

public:
	// Con-/Destructors
	IndexIterator(_ItW const& It): _IndexIteratorBase(It) {}
	IndexIterator(_Cluster* Cluster, SIZE_T Position): _IndexIteratorBase(Cluster) { this->SetPosition(Position); }
	IndexIterator(_Cluster* Cluster, SIZE_T, _Id const& Id): _IndexIteratorBase(Cluster) { this->template FindInternal<_Id const&>(Id); }

	// Access
	inline BOOL Find(_Id const& Id) { return this->template FindInternal<_Id const&>(Id); }
	inline _Id const& GetCurrentId()const { return this->pCurrent->GetId(); }
	inline _Item& GetCurrentItem()const { return this->pCurrent->GetItem(); }

	// Assignment
	inline _It& operator=(_ItW const& It) { this->template Assign<_ItW>(It); return *this; }

	// Modification
	inline VOID SetCurrentItem(_Item const& Item) { this->pCurrent->SetItem(Item); }
};

// Iterator Index without Items Read-Only
template <class _Id, UINT _GroupSize>
class IndexIterator<_Id, VOID, _GroupSize, true>: public IndexIteratorBase<_Id, VOID, _GroupSize, true>
{
private:
	// Using
	using _It=IndexIterator<_Id, VOID, _GroupSize, true>;
	using _ItR=IndexIterator<_Id, VOID, _GroupSize, true>;
	using _ItW=IndexIterator<_Id, VOID, _GroupSize, false>;
	using _IndexIteratorBase=IndexIteratorBase<_Id, VOID, _GroupSize, true>;
	using _IndexItem=IndexItem<_Id, VOID>;
	using _IndexGroup=IndexGroup<_Id, VOID>;
	using _IndexItemGroup=IndexItemGroup<_Id, VOID, _GroupSize>;
	using _IndexParentGroup=IndexParentGroup<_Id, VOID, _GroupSize>;
	using _Cluster=::Clusters::Templates::Details::Cluster::Cluster<_IndexItem, _IndexGroup, _IndexItemGroup, _IndexParentGroup, _ItR, _ItW>;

public:
	// Con-/Destructors
	IndexIterator(_ItR const& It): _IndexIteratorBase(It) {}
	IndexIterator(_ItW const& It): _IndexIteratorBase(It) {}
	IndexIterator(_Cluster const* Cluster, SIZE_T Position): _IndexIteratorBase(Cluster) { this->SetPosition(Position); }
	IndexIterator(_Cluster const* Cluster, SIZE_T, _Id const& Id): _IndexIteratorBase(Cluster) { this->template FindInternal<_Id const&>(Id); }

	// Access
	inline BOOL Find(_Id const& Id) { return this->template FindInternal<_Id const&>(Id); }
	inline _Id const& GetCurrent()const { return this->pCurrent->GetId(); }

	// Assignment
	inline _It& operator=(_ItR const& It) { this->template Assign<_ItR>(It); return *this; }
	inline _It& operator=(_ItW const& It) { this->template Assign<_ItW>(It); return *this; }
};

// Iterator Index without Items Read-Write
template <class _Id, UINT _GroupSize>
class IndexIterator<_Id, VOID, _GroupSize, false>: public IndexIteratorBase<_Id, VOID, _GroupSize, false>
{
private:
	// Using
	using _It=IndexIterator<_Id, VOID, _GroupSize, false>;
	using _ItR=IndexIterator<_Id, VOID, _GroupSize, true>;
	using _ItW=IndexIterator<_Id, VOID, _GroupSize, false>;
	using _IndexIteratorBase=IndexIteratorBase<_Id, VOID, _GroupSize, false>;
	using _IndexItem=IndexItem<_Id, VOID>;
	using _IndexGroup=IndexGroup<_Id, VOID>;
	using _IndexItemGroup=IndexItemGroup<_Id, VOID, _GroupSize>;
	using _IndexParentGroup=IndexParentGroup<_Id, VOID, _GroupSize>;
	using _Cluster=::Clusters::Templates::Details::Cluster::Cluster<_IndexItem, _IndexGroup, _IndexItemGroup, _IndexParentGroup, _ItR, _ItW>;

public:
	// Con-/Destructors
	IndexIterator(_ItW const& It): _IndexIteratorBase(It) {}
	IndexIterator(_Cluster* Cluster, SIZE_T Position): _IndexIteratorBase(Cluster) { this->SetPosition(Position); }
	IndexIterator(_Cluster* Cluster, SIZE_T, _Id const& Id): _IndexIteratorBase(Cluster) { this->template FindInternal<_Id const&>(Id); }

	// Access
	inline BOOL Find(_Id const& Id) { return this->template FindInternal<_Id const&>(Id); }
	inline _Id const& GetCurrent()const { return this->pCurrent->GetId(); }

	// Assignment
	inline _It& operator=(_ItW const& It) { this->template Assign<_ItW>(It); return *this; }
};

// Iterator Index with Pointers Read-Only
template <class _Id, class _Item, UINT _GroupSize>
class IndexIterator<_Id, _Item*, _GroupSize, true>: public IndexIteratorBase<_Id, _Item*, _GroupSize, true>
{
private:
	// Using
	using _It=IndexIterator<_Id, _Item*, _GroupSize, true>;
	using _ItR=IndexIterator<_Id, _Item*, _GroupSize, true>;
	using _ItW=IndexIterator<_Id, _Item*, _GroupSize, false>;
	using _IndexIteratorBase=IndexIteratorBase<_Id, _Item*, _GroupSize, true>;
	using _IndexItem=IndexItem<_Id, _Item*>;
	using _IndexGroup=IndexGroup<_Id, _Item*>;
	using _IndexItemGroup=IndexItemGroup<_Id, _Item*, _GroupSize>;
	using _IndexParentGroup=IndexParentGroup<_Id, _Item*, _GroupSize>;
	using _Cluster=::Clusters::Templates::Details::Cluster::Cluster<_IndexItem, _IndexGroup, _IndexItemGroup, _IndexParentGroup, _ItR, _ItW>;

public:
	// Con-/Destructors
	IndexIterator(_ItR const& It): _IndexIteratorBase(It) {}
	IndexIterator(_ItW const& It): _IndexIteratorBase(It) {}
	IndexIterator(_Cluster const* Cluster, SIZE_T Position): _IndexIteratorBase(Cluster) { this->SetPosition(Position); }
	IndexIterator(_Cluster const* Cluster, SIZE_T, _Id const& Id): _IndexIteratorBase(Cluster) { this->template FindInternal<_Id const&>(Id); }

	// Access
	inline BOOL Find(_Id const& Id) { return this->template FindInternal<_Id const&>(Id); }
	inline _Id const& GetCurrentId()const { return this->pCurrent->GetId(); }
	inline _Item* GetCurrentItem()const { return this->pCurrent->GetItem(); }

	// Assignment
	inline _It& operator=(_ItR const& It) { this->template Assign<_ItR>(It); return *this; }
	inline _It& operator=(_ItW const& It) { this->template Assign<_ItW>(It); return *this; }
};

// Iterator Index with Pointers Read-Write
template <class _Id, class _Item, UINT _GroupSize>
class IndexIterator<_Id, _Item*, _GroupSize, false>: public IndexIteratorBase<_Id, _Item*, _GroupSize, false>
{
private:
	// Using
	using _It=IndexIterator<_Id, _Item*, _GroupSize, false>;
	using _ItR=IndexIterator<_Id, _Item*, _GroupSize, true>;
	using _ItW=IndexIterator<_Id, _Item*, _GroupSize, false>;
	using _IndexIteratorBase=IndexIteratorBase<_Id, _Item*, _GroupSize, false>;
	using _IndexItem=IndexItem<_Id, _Item*>;
	using _IndexGroup=IndexGroup<_Id, _Item*>;
	using _IndexItemGroup=IndexItemGroup<_Id, _Item*, _GroupSize>;
	using _IndexParentGroup=IndexParentGroup<_Id, _Item*, _GroupSize>;
	using _Cluster=::Clusters::Templates::Details::Cluster::Cluster<_IndexItem, _IndexGroup, _IndexItemGroup, _IndexParentGroup, _ItR, _ItW>;

public:
	// Con-/Destructors
	IndexIterator(_ItW const& It): _IndexIteratorBase(It) {}
	IndexIterator(_Cluster* Cluster, SIZE_T Position): _IndexIteratorBase(Cluster) { this->SetPosition(Position); }
	IndexIterator(_Cluster* Cluster, SIZE_T, _Id const& Id): _IndexIteratorBase(Cluster) { this->template FindInternal<_Id const&>(Id); }

	// Access
	inline BOOL Find(_Id const& Id) { return this->template FindInternal<_Id const&>(Id); }
	inline _Id const& GetCurrentId()const { return this->pCurrent->GetId(); }
	inline _Item* GetCurrentItem()const { return this->pCurrent->GetItem(); }

	// Assignment
	inline _It& operator=(_ItW const& It) { this->template Assign<_ItW>(It); return *this; }

	// Modification
	inline VOID SetCurrentItem(_Item* Item) { this->pCurrent->SetItem(Item); }
};

#ifdef __cplusplus_winrt
// Iterator Index with Handles Read-Only
template <class _Id, class _Item, UINT _GroupSize>
class IndexIterator<_Id, _Item^, _GroupSize, true>: public IndexIteratorBase<_Id, _Item^, _GroupSize, true>
{
private:
	// Using
	using _It=IndexIterator<_Id, _Item^, _GroupSize, true>;
	using _ItR=IndexIterator<_Id, _Item^, _GroupSize, true>;
	using _ItW=IndexIterator<_Id, _Item^, _GroupSize, false>;
	using _IndexIteratorBase=IndexIteratorBase<_Id, _Item^, _GroupSize, true>;
	using _IndexItem=IndexItem<_Id, _Item^>;
	using _IndexGroup=IndexGroup<_Id, _Item^>;
	using _IndexItemGroup=IndexItemGroup<_Id, _Item^, _GroupSize>;
	using _IndexParentGroup=IndexParentGroup<_Id, _Item^, _GroupSize>;
	using _Cluster=::Clusters::Templates::Details::Cluster::Cluster<_IndexItem, _IndexGroup, _IndexItemGroup, _IndexParentGroup, _ItR, _ItW>;

public:
	// Con-/Destructors
	IndexIterator(_ItR const& It): _IndexIteratorBase(It) {}
	IndexIterator(_ItW const& It): _IndexIteratorBase(It) {}
	IndexIterator(_Cluster const* Cluster, SIZE_T Position): _IndexIteratorBase(Cluster) { this->SetPosition(Position); }
	IndexIterator(_Cluster const* Cluster, SIZE_T, _Id const& Id): _IndexIteratorBase(Cluster) { this->template FindInternal<_Id const&>(Id); }

	// Access
	inline BOOL Find(_Id const& Id) { return this->template FindInternal<_Id const&>(Id); }
	inline _Id const& GetCurrentId()const { return this->pCurrent->GetId(); }
	inline _Item^ GetCurrentItem()const { return this->pCurrent->GetItem(); }

	// Assignment
	inline _It& operator=(_ItR const& It) { this->template Assign<_ItR>(It); return *this; }
	inline _It& operator=(_ItW const& It) { this->template Assign<_ItW>(It); return *this; }
};
#endif

#ifdef __cplusplus_winrt
// Iterator Index with Handles Read-Write
template <class _Id, class _Item, UINT _GroupSize>
class IndexIterator<_Id, _Item^, _GroupSize, false>: public IndexIteratorBase<_Id, _Item^, _GroupSize, false>
{
private:
	// Using
	using _It=IndexIterator<_Id, _Item^, _GroupSize, false>;
	using _ItR=IndexIterator<_Id, _Item^, _GroupSize, true>;
	using _ItW=IndexIterator<_Id, _Item^, _GroupSize, false>;
	using _IndexIteratorBase=IndexIteratorBase<_Id, _Item^, _GroupSize, false>;
	using _IndexItem=IndexItem<_Id, _Item^>;
	using _IndexGroup=IndexGroup<_Id, _Item^>;
	using _IndexItemGroup=IndexItemGroup<_Id, _Item^, _GroupSize>;
	using _IndexParentGroup=IndexParentGroup<_Id, _Item^, _GroupSize>;
	using _Cluster=::Clusters::Templates::Details::Cluster::Cluster<_IndexItem, _IndexGroup, _IndexItemGroup, _IndexParentGroup, _ItR, _ItW>;

public:
	// Con-/Destructors
	IndexIterator(_ItW const& It): _IndexIteratorBase(It) {}
	IndexIterator(_Cluster* Cluster, SIZE_T Position): _IndexIteratorBase(Cluster) { this->SetPosition(Position); }
	IndexIterator(_Cluster* Cluster, SIZE_T, _Id const& Id): _IndexIteratorBase(Cluster) { this->template FindInternal<_Id const&>(Id); }

	// Access
	inline BOOL Find(_Id const& Id) { return this->template FindInternal<_Id const&>(Id); }
	inline _Id const& GetCurrentId()const { return this->pCurrent->GetId(); }
	inline _Item^ GetCurrentItem()const { return this->pCurrent->GetItem(); }

	// Assignment
	inline _It& operator=(_ItW const& It) { this->template Assign<_ItW>(It); return *this; }

	// Modification
	inline VOID SetCurrentItem(_Item^ Item) { this->pCurrent->SetItem(Item); }
};
#endif

// Iterator Index with Strings Read-Only
template <class _Id, class _Char, BOOL _AllocItem, UINT _GroupSize>
class IndexIterator<_Id, StringItem<_Char, _AllocItem>, _GroupSize, true>: public IndexIteratorBase<_Id, StringItem<_Char, _AllocItem>, _GroupSize, true>
{
private:
	// Using
	using _It=IndexIterator<_Id, StringItem<_Char, _AllocItem>, _GroupSize, true>;
	using _ItR=IndexIterator<_Id, StringItem<_Char, _AllocItem>, _GroupSize, true>;
	using _ItW=IndexIterator<_Id, StringItem<_Char, _AllocItem>, _GroupSize, false>;
	using _IndexIteratorBase=IndexIteratorBase<_Id, StringItem<_Char, _AllocItem>, _GroupSize, true>;
	using _IndexItem=IndexItem<_Id, StringItem<_Char, _AllocItem>>;
	using _IndexGroup=IndexGroup<_Id, StringItem<_Char, _AllocItem>>;
	using _IndexItemGroup=IndexItemGroup<_Id, StringItem<_Char, _AllocItem>, _GroupSize>;
	using _IndexParentGroup=IndexParentGroup<_Id, StringItem<_Char, _AllocItem>, _GroupSize>;
	using _Cluster=::Clusters::Templates::Details::Cluster::Cluster<_IndexItem, _IndexGroup, _IndexItemGroup, _IndexParentGroup, _ItR, _ItW>;

public:
	// Con-/Destructors
	IndexIterator(_ItR const& It): _IndexIteratorBase(It) {}
	IndexIterator(_ItW const& It): _IndexIteratorBase(It) {}
	IndexIterator(_Cluster const* Cluster, SIZE_T Position): _IndexIteratorBase(Cluster) { this->SetPosition(Position); }
	IndexIterator(_Cluster const* Cluster, SIZE_T, _Id const& Id): _IndexIteratorBase(Cluster) { this->template FindInternal<_Id const&>(Id); }

	// Access
	inline BOOL Find(_Id const& Id) { return this->template FindInternal<_Id const&>(Id); }
	inline _Id const& GetCurrentId()const { return this->pCurrent->GetId(); }
	inline _Char const* GetCurrentItem()const { return this->pCurrent->GetItem(); }

	// Assignment
	inline _It& operator=(_ItR const& It) { this->template Assign<_ItR>(It); return *this; }
	inline _It& operator=(_ItW const& It) { this->template Assign<_ItW>(It); return *this; }
};

// Iterator Index with Strings Read-Write
template <class _Id, class _Char, UINT _GroupSize>
class IndexIterator<_Id, StringItem<_Char, true>, _GroupSize, false>: public IndexIteratorBase<_Id, StringItem<_Char, true>, _GroupSize, false>
{
private:
	// Using
	using _It=IndexIterator<_Id, StringItem<_Char, true>, _GroupSize, false>;
	using _ItR=IndexIterator<_Id, StringItem<_Char, true>, _GroupSize, true>;
	using _ItW=IndexIterator<_Id, StringItem<_Char, true>, _GroupSize, false>;
	using _IndexIteratorBase=IndexIteratorBase<_Id, StringItem<_Char, true>, _GroupSize, false>;
	using _IndexItem=IndexItem<_Id, StringItem<_Char, true>>;
	using _IndexGroup=IndexGroup<_Id, StringItem<_Char, true>>;
	using _IndexItemGroup=IndexItemGroup<_Id, StringItem<_Char, true>, _GroupSize>;
	using _IndexParentGroup=IndexParentGroup<_Id, StringItem<_Char, true>, _GroupSize>;
	using _Cluster=::Clusters::Templates::Details::Cluster::Cluster<_IndexItem, _IndexGroup, _IndexItemGroup, _IndexParentGroup, _ItR, _ItW>;

public:
	// Con-/Destructors
	IndexIterator(_ItW const& It): _IndexIteratorBase(It) {}
	IndexIterator(_Cluster* Cluster, SIZE_T Position): _IndexIteratorBase(Cluster) { this->SetPosition(Position); }
	IndexIterator(_Cluster* Cluster, SIZE_T, _Id const& Id): _IndexIteratorBase(Cluster) { this->template FindInternal<_Id const&>(Id); }

	// Access
	inline BOOL Find(_Id const& Id) { return this->template FindInternal<_Id const&>(Id); }
	inline _Id const& GetCurrentId()const { return this->pCurrent->GetId(); }
	inline _Char const* GetCurrentItem()const { return this->pCurrent->GetItem(); }

	// Assignment
	inline _It& operator=(_ItW const& It) { this->template Assign<_ItW>(It); return *this; }

	// Modification
	inline UINT SetCurrentItem(_Char const* Item, UINT Length=0) { return this->pCurrent->SetItem(Item, Length); }
};

// Iterator Index with Shared Strings Read-Only
template <class _Id, class _Char, UINT _GroupSize>
class IndexIterator<_Id, StringItem<_Char, false>, _GroupSize, true>: public IndexIteratorBase<_Id, StringItem<_Char, false>, _GroupSize, true>
{
private:
	// Using
	using _It=IndexIterator<_Id, StringItem<_Char, false>, _GroupSize, true>;
	using _ItR=IndexIterator<_Id, StringItem<_Char, false>, _GroupSize, true>;
	using _ItW=IndexIterator<_Id, StringItem<_Char, false>, _GroupSize, false>;
	using _IndexIteratorBase=IndexIteratorBase<_Id, StringItem<_Char, false>, _GroupSize, true>;
	using _IndexItem=IndexItem<_Id, StringItem<_Char, false>>;
	using _IndexGroup=IndexGroup<_Id, StringItem<_Char, false>>;
	using _IndexItemGroup=IndexItemGroup<_Id, StringItem<_Char, false>, _GroupSize>;
	using _IndexParentGroup=IndexParentGroup<_Id, StringItem<_Char, false>, _GroupSize>;
	using _Cluster=::Clusters::Templates::Details::Cluster::Cluster<_IndexItem, _IndexGroup, _IndexItemGroup, _IndexParentGroup, _ItR, _ItW>;

public:
	// Con-/Destructors
	IndexIterator(_ItR const& It): _IndexIteratorBase(It) {}
	IndexIterator(_ItW const& It): _IndexIteratorBase(It) {}
	IndexIterator(_Cluster const* Cluster, SIZE_T Position): _IndexIteratorBase(Cluster) { this->SetPosition(Position); }
	IndexIterator(_Cluster const* Cluster, SIZE_T, _Id const& Id): _IndexIteratorBase(Cluster) { this->template FindInternal<_Id const&>(Id); }

	// Access
	inline BOOL Find(_Id const& Id) { return this->template FindInternal<_Id const&>(Id); }
	inline _Id const& GetCurrentId()const { return this->pCurrent->GetId(); }
	inline _Char const* GetCurrentItem()const { return this->pCurrent->GetItem(); }

	// Assignment
	inline _It& operator=(_ItR const& It) { this->template Assign<_ItR>(It); return *this; }
	inline _It& operator=(_ItW const& It) { this->template Assign<_ItW>(It); return *this; }
};

// Iterator Index with Shared Strings Read-Write
template <class _Id, class _Char, UINT _GroupSize>
class IndexIterator<_Id, StringItem<_Char, false>, _GroupSize, false>: public IndexIteratorBase<_Id, StringItem<_Char, false>, _GroupSize, false>
{
private:
	// Using
	using _It=IndexIterator<_Id, StringItem<_Char, false>, _GroupSize, false>;
	using _ItR=IndexIterator<_Id, StringItem<_Char, false>, _GroupSize, true>;
	using _ItW=IndexIterator<_Id, StringItem<_Char, false>, _GroupSize, false>;
	using _IndexIteratorBase=IndexIteratorBase<_Id, StringItem<_Char, false>, _GroupSize, false>;
	using _IndexItem=IndexItem<_Id, StringItem<_Char, false>>;
	using _IndexGroup=IndexGroup<_Id, StringItem<_Char, false>>;
	using _IndexItemGroup=IndexItemGroup<_Id, StringItem<_Char, false>, _GroupSize>;
	using _IndexParentGroup=IndexParentGroup<_Id, StringItem<_Char, false>, _GroupSize>;
	using _Cluster=::Clusters::Templates::Details::Cluster::Cluster<_IndexItem, _IndexGroup, _IndexItemGroup, _IndexParentGroup, _ItR, _ItW>;

public:
	// Con-/Destructors
	IndexIterator(_ItW const& It): _IndexIteratorBase(It) {}
	IndexIterator(_Cluster* Cluster, SIZE_T Position): _IndexIteratorBase(Cluster) { this->SetPosition(Position); }
	IndexIterator(_Cluster* Cluster, SIZE_T Position, _Id const& Id): _IndexIteratorBase(Cluster) { this->template FindInternal<_Id const&>(Id); }

	// Access
	inline BOOL Find(_Id const& Id) { return this->template FindInternal<_Id const&>(Id); }
	inline _Id const& GetCurrentId()const { return this->pCurrent->GetId(); }
	inline _Char const* GetCurrentItem()const { return this->pCurrent->GetItem(); }

	// Assignment
	inline _It& operator=(_ItW const& It) { this->template Assign<_ItW>(It); return *this; }

	// Modification
	inline VOID SetCurrentItem(_Char const* Item) { this->pCurrent->SetItem(Item); }
};


//========================
// Iterator Pointer-Index
//========================

// Iterator Pointer-Index Read-Only
template <class _Id, class _Item, UINT _GroupSize>
class IndexIterator<_Id*, _Item, _GroupSize, true>: public IndexIteratorBase<_Id*, _Item, _GroupSize, true>
{
private:
	// Using
	using _It=IndexIterator<_Id*, _Item, _GroupSize, true>;
	using _ItR=IndexIterator<_Id*, _Item, _GroupSize, true>;
	using _ItW=IndexIterator<_Id*, _Item, _GroupSize, false>;
	using _IndexIteratorBase=IndexIteratorBase<_Id*, _Item, _GroupSize, true>;
	using _IndexItem=IndexItem<_Id*, _Item>;
	using _IndexGroup=IndexGroup<_Id*, _Item>;
	using _IndexItemGroup=IndexItemGroup<_Id*, _Item, _GroupSize>;
	using _IndexParentGroup=IndexParentGroup<_Id*, _Item, _GroupSize>;
	using _Cluster=::Clusters::Templates::Details::Cluster::Cluster<_IndexItem, _IndexGroup, _IndexItemGroup, _IndexParentGroup, _ItR, _ItW>;

public:
	// Con-/Destructors
	IndexIterator(_ItR const& It): _IndexIteratorBase(It) {}
	IndexIterator(_ItW const& It): _IndexIteratorBase(It) {}
	IndexIterator(_Cluster const* Cluster, SIZE_T Position): _IndexIteratorBase(Cluster) { this->SetPosition(Position); }
	IndexIterator(_Cluster const* Cluster, SIZE_T, _Id* Id): _IndexIteratorBase(Cluster) { _IndexIteratorBase::FindInternal(Id); }

	// Access
	inline BOOL Find(_Id* Id) { return _IndexIteratorBase::FindInternal(Id); }
	inline _Id* GetCurrentId()const { return this->pCurrent->GetId(); }
	inline _Item const& GetCurrentItem()const { return this->pCurrent->GetItem(); }

	// Assignment
	inline _It& operator=(_ItR const& It) { this->template Assign<_ItR>(It); return *this; }
	inline _It& operator=(_ItW const& It) { this->template Assign<_ItW>(It); return *this; }
};

// Iterator Pointer-Index Read-Write
template <class _Id, class _Item, UINT _GroupSize>
class IndexIterator<_Id*, _Item, _GroupSize, false>: public IndexIteratorBase<_Id*, _Item, _GroupSize, false>
{
private:
	// Using
	using _It=IndexIterator<_Id*, _Item, _GroupSize, false>;
	using _ItR=IndexIterator<_Id*, _Item, _GroupSize, true>;
	using _ItW=IndexIterator<_Id*, _Item, _GroupSize, false>;
	using _IndexIteratorBase=IndexIteratorBase<_Id*, _Item, _GroupSize, false>;
	using _IndexItem=IndexItem<_Id*, _Item>;
	using _IndexGroup=IndexGroup<_Id*, _Item>;
	using _IndexItemGroup=IndexItemGroup<_Id*, _Item, _GroupSize>;
	using _IndexParentGroup=IndexParentGroup<_Id*, _Item, _GroupSize>;
	using _Cluster=::Clusters::Templates::Details::Cluster::Cluster<_IndexItem, _IndexGroup, _IndexItemGroup, _IndexParentGroup, _ItR, _ItW>;

public:
	// Con-/Destructors
	IndexIterator(_ItW const& It): _IndexIteratorBase(It) {}
	IndexIterator(_Cluster* Cluster, SIZE_T Position): _IndexIteratorBase(Cluster) { this->SetPosition(Position); }
	IndexIterator(_Cluster* Cluster, SIZE_T, _Id* Id): _IndexIteratorBase(Cluster) { _IndexIteratorBase::FindInternal(Id); }

	// Access
	inline BOOL Find(_Id* Id) { return _IndexIteratorBase::FindInternal(Id); }
	inline _Id* GetCurrentId()const { return this->pCurrent->GetId(); }
	inline _Item& GetCurrentItem()const { return this->pCurrent->GetItem(); }

	// Assignment
	inline _It& operator=(_ItW const& It) { this->template Assign<_ItW>(It); return *this; }

	// Modification
	inline VOID SetCurrentItem(_Item const& Item) { this->pCurrent->SetItem(Item); }
};

// Iterator Pointer-Index without Items Read-Only
template <class _Id, UINT _GroupSize>
class IndexIterator<_Id*, VOID, _GroupSize, true>: public IndexIteratorBase<_Id*, VOID, _GroupSize, true>
{
private:
	// Using
	using _It=IndexIterator<_Id*, VOID, _GroupSize, true>;
	using _ItR=IndexIterator<_Id*, VOID, _GroupSize, true>;
	using _ItW=IndexIterator<_Id*, VOID, _GroupSize, false>;
	using _IndexIteratorBase=IndexIteratorBase<_Id*, VOID, _GroupSize, true>;
	using _IndexItem=IndexItem<_Id*, VOID>;
	using _IndexGroup=IndexGroup<_Id*, VOID>;
	using _IndexItemGroup=IndexItemGroup<_Id*, VOID, _GroupSize>;
	using _IndexParentGroup=IndexParentGroup<_Id*, VOID, _GroupSize>;
	using _Cluster=::Clusters::Templates::Details::Cluster::Cluster<_IndexItem, _IndexGroup, _IndexItemGroup, _IndexParentGroup, _ItR, _ItW>;

public:
	// Con-/Destructors
	IndexIterator(_ItR const& It): _IndexIteratorBase(It) {}
	IndexIterator(_ItW const& It): _IndexIteratorBase(It) {}
	IndexIterator(_Cluster const* Cluster, SIZE_T Position): _IndexIteratorBase(Cluster) { this->SetPosition(Position); }
	IndexIterator(_Cluster const* Cluster, SIZE_T, _Id* Id): _IndexIteratorBase(Cluster) { _IndexIteratorBase::FindInternal(Id); }

	// Access
	inline BOOL Find(_Id* Id) { return _IndexIteratorBase::FindInternal(Id); }
	inline _Id* GetCurrent()const { return this->pCurrent->GetId(); }

	// Assignment
	inline _It& operator=(_ItR const& It) { this->template Assign<_ItR>(It); return *this; }
	inline _It& operator=(_ItW const& It) { this->template Assign<_ItW>(It); return *this; }
};

// Iterator Pointer-Index without Items Read-Write
template <class _Id, UINT _GroupSize>
class IndexIterator<_Id*, VOID, _GroupSize, false>: public IndexIteratorBase<_Id*, VOID, _GroupSize, false>
{
private:
	// Using
	using _It=IndexIterator<_Id*, VOID, _GroupSize, false>;
	using _ItR=IndexIterator<_Id*, VOID, _GroupSize, true>;
	using _ItW=IndexIterator<_Id*, VOID, _GroupSize, false>;
	using _IndexIteratorBase=IndexIteratorBase<_Id*, VOID, _GroupSize, false>;
	using _IndexItem=IndexItem<_Id*, VOID>;
	using _IndexGroup=IndexGroup<_Id*, VOID>;
	using _IndexItemGroup=IndexItemGroup<_Id*, VOID, _GroupSize>;
	using _IndexParentGroup=IndexParentGroup<_Id*, VOID, _GroupSize>;
	using _Cluster=::Clusters::Templates::Details::Cluster::Cluster<_IndexItem, _IndexGroup, _IndexItemGroup, _IndexParentGroup, _ItR, _ItW>;

public:
	// Con-/Destructors
	IndexIterator(_ItW const& It): _IndexIteratorBase(It) {}
	IndexIterator(_Cluster* Cluster, SIZE_T Position): _IndexIteratorBase(Cluster) { this->SetPosition(Position); }
	IndexIterator(_Cluster* Cluster, SIZE_T, _Id* Id): _IndexIteratorBase(Cluster) { _IndexIteratorBase::FindInternal(Id); }

	// Assignment
	inline _It& operator=(_ItW const& It) { this->template Assign<_ItW>(It); return *this; }

	// Access
	inline BOOL Find(_Id* Id) { return _IndexIteratorBase::FindInternal(Id); }
	inline _Id* GetCurrent()const { return this->pCurrent->GetId(); }
};

// Iterator Pointer-Index with Pointers Read-Only
template <class _Id, class _Item, UINT _GroupSize>
class IndexIterator<_Id*, _Item*, _GroupSize, true>: public IndexIteratorBase<_Id*, _Item*, _GroupSize, true>
{
private:
	// Using
	using _It=IndexIterator<_Id*, _Item*, _GroupSize, true>;
	using _ItR=IndexIterator<_Id*, _Item*, _GroupSize, true>;
	using _ItW=IndexIterator<_Id*, _Item*, _GroupSize, false>;
	using _IndexIteratorBase=IndexIteratorBase<_Id*, _Item*, _GroupSize, true>;
	using _IndexItem=IndexItem<_Id*, _Item*>;
	using _IndexGroup=IndexGroup<_Id*, _Item*>;
	using _IndexItemGroup=IndexItemGroup<_Id*, _Item*, _GroupSize>;
	using _IndexParentGroup=IndexParentGroup<_Id*, _Item*, _GroupSize>;
	using _Cluster=::Clusters::Templates::Details::Cluster::Cluster<_IndexItem, _IndexGroup, _IndexItemGroup, _IndexParentGroup, _ItR, _ItW>;

public:
	// Con-/Destructors
	IndexIterator(_ItR const& It): _IndexIteratorBase(It) {}
	IndexIterator(_ItW const& It): _IndexIteratorBase(It) {}
	IndexIterator(_Cluster const* Cluster, SIZE_T Position): _IndexIteratorBase(Cluster) { this->SetPosition(Position); }
	IndexIterator(_Cluster const* Cluster, SIZE_T, _Id* Id): _IndexIteratorBase(Cluster) { _IndexIteratorBase::FindInternal(Id); }

	// Access
	inline BOOL Find(_Id* Id) { return _IndexIteratorBase::FindInternal(Id); }
	inline _Id* GetCurrentId()const { return this->pCurrent->GetId(); }
	inline _Item* GetCurrentItem()const { return this->pCurrent->GetItem(); }

	// Assignment
	inline _It& operator=(_ItR const& It) { this->template Assign<_ItR>(It); return *this; }
	inline _It& operator=(_ItW const& It) { this->template Assign<_ItW>(It); return *this; }
};

// Iterator Pointer-Index with Pointers Read-Write
template <class _Id, class _Item, UINT _GroupSize>
class IndexIterator<_Id*, _Item*, _GroupSize, false>: public IndexIteratorBase<_Id*, _Item*, _GroupSize, false>
{
private:
	// Using
	using _It=IndexIterator<_Id*, _Item*, _GroupSize, false>;
	using _ItR=IndexIterator<_Id*, _Item*, _GroupSize, true>;
	using _ItW=IndexIterator<_Id*, _Item*, _GroupSize, false>;
	using _IndexIteratorBase=IndexIteratorBase<_Id*, _Item*, _GroupSize, false>;
	using _IndexItem=IndexItem<_Id*, _Item*>;
	using _IndexGroup=IndexGroup<_Id*, _Item*>;
	using _IndexItemGroup=IndexItemGroup<_Id*, _Item*, _GroupSize>;
	using _IndexParentGroup=IndexParentGroup<_Id*, _Item*, _GroupSize>;
	using _Cluster=::Clusters::Templates::Details::Cluster::Cluster<_IndexItem, _IndexGroup, _IndexItemGroup, _IndexParentGroup, _ItR, _ItW>;

public:
	// Con-/Destructors
	IndexIterator(_ItW const& It): _IndexIteratorBase(It) {}
	IndexIterator(_Cluster* Cluster, SIZE_T Position): _IndexIteratorBase(Cluster) { this->SetPosition(Position); }
	IndexIterator(_Cluster* Cluster, SIZE_T, _Id* Id): _IndexIteratorBase(Cluster) { _IndexIteratorBase::FindInternal(Id); }

	// Access
	inline BOOL Find(_Id* Id) { return _IndexIteratorBase::FindInternal(Id); }
	inline _Id* GetCurrentId()const { return this->pCurrent->GetId(); }
	inline _Item* GetCurrentItem()const { return this->pCurrent->GetItem(); }

	// Assignment
	inline _It& operator=(_ItW const& It) { this->template Assign<_ItW>(It); return *this; }

	// Modification
	inline VOID SetCurrentItem(_Item* Item) { this->pCurrent->SetItem(Item); }
};

#ifdef __cplusplus_winrt
// Iterator Pointer-Index with Handles Read-Only
template <class _Id, class _Item, UINT _GroupSize>
class IndexIterator<_Id*, _Item^, _GroupSize, true>: public IndexIteratorBase<_Id*, _Item^, _GroupSize, true>
{
private:
	// Using
	using _It=IndexIterator<_Id*, _Item^, _GroupSize, true>;
	using _ItR=IndexIterator<_Id*, _Item^, _GroupSize, true>;
	using _ItW=IndexIterator<_Id*, _Item^, _GroupSize, false>;
	using _IndexIteratorBase=IndexIteratorBase<_Id*, _Item^, _GroupSize, true>;
	using _IndexItem=IndexItem<_Id*, _Item^>;
	using _IndexGroup=IndexGroup<_Id*, _Item^>;
	using _IndexItemGroup=IndexItemGroup<_Id*, _Item^, _GroupSize>;
	using _IndexParentGroup=IndexParentGroup<_Id*, _Item^, _GroupSize>;
	using _Cluster=::Clusters::Templates::Details::Cluster::Cluster<_IndexItem, _IndexGroup, _IndexItemGroup, _IndexParentGroup, _ItR, _ItW>;

public:
	// Con-/Destructors
	IndexIterator(_ItR const& It): _IndexIteratorBase(It) {}
	IndexIterator(_ItW const& It): _IndexIteratorBase(It) {}
	IndexIterator(_Cluster const* Cluster, SIZE_T Position): _IndexIteratorBase(Cluster) { this->SetPosition(Position); }
	IndexIterator(_Cluster const* Cluster, SIZE_T, _Id* Id): _IndexIteratorBase(Cluster) { _IndexIteratorBase::FindInternal(Id); }

	// Access
	inline BOOL Find(_Id* Id) { return _IndexIteratorBase::FindInternal(Id); }
	inline _Id* GetCurrentId()const { return this->pCurrent->GetId(); }
	inline _Item^ GetCurrentItem()const { return this->pCurrent->GetItem(); }

	// Assignment
	inline _It& operator=(_ItR const& It) { this->template Assign<_ItR>(It); return *this; }
	inline _It& operator=(_ItW const& It) { this->template Assign<_ItW>(It); return *this; }
};
#endif

#ifdef __cplusplus_winrt
// Iterator Pointer-Index with Handles Read-Write
template <class _Id, class _Item, UINT _GroupSize>
class IndexIterator<_Id*, _Item^, _GroupSize, false>: public IndexIteratorBase<_Id*, _Item^, _GroupSize, false>
{
private:
	// Using
	using _It=IndexIterator<_Id*, _Item^, _GroupSize, false>;
	using _ItR=IndexIterator<_Id*, _Item^, _GroupSize, true>;
	using _ItW=IndexIterator<_Id*, _Item^, _GroupSize, false>;
	using _IndexIteratorBase=IndexIteratorBase<_Id*, _Item^, _GroupSize, false>;
	using _IndexItem=IndexItem<_Id*, _Item^>;
	using _IndexGroup=IndexGroup<_Id*, _Item^>;
	using _IndexItemGroup=IndexItemGroup<_Id*, _Item^, _GroupSize>;
	using _IndexParentGroup=IndexParentGroup<_Id*, _Item^, _GroupSize>;
	using _Cluster=::Clusters::Templates::Details::Cluster::Cluster<_IndexItem, _IndexGroup, _IndexItemGroup, _IndexParentGroup, _ItR, _ItW>;

public:
	// Con-/Destructors
	IndexIterator(_ItW const& It): _IndexIteratorBase(It) {}
	IndexIterator(_Cluster* Cluster, SIZE_T Position): _IndexIteratorBase(Cluster) { this->SetPosition(Position); }
	IndexIterator(_Cluster* Cluster, SIZE_T, _Id* Id): _IndexIteratorBase(Cluster) { _IndexIteratorBase::FindInternal(Id); }

	// Access
	inline BOOL Find(_Id* Id) { return _IndexIteratorBase::FindInternal(Id); }
	inline _Id* GetCurrentId()const { return this->pCurrent->GetId(); }
	inline _Item^ GetCurrentItem()const { return this->pCurrent->GetItem(); }

	// Assignment
	inline _It& operator=(_ItW const& It) { this->template Assign<_ItW>(It); return *this; }

	// Modification
	inline VOID SetCurrentItem(_Item^ Item) { this->pCurrent->SetItem(Item); }
};
#endif

// Iterator Pointer-Index with Strings Read-Only
template <class _Id, class _Char, BOOL _AllocItem, UINT _GroupSize>
class IndexIterator<_Id*, StringItem<_Char, _AllocItem>, _GroupSize, true>: public IndexIteratorBase<_Id*, StringItem<_Char, _AllocItem>, _GroupSize, true>
{
private:
	// Using
	using _It=IndexIterator<_Id*, StringItem<_Char, _AllocItem>, _GroupSize, true>;
	using _ItR=IndexIterator<_Id*, StringItem<_Char, _AllocItem>, _GroupSize, true>;
	using _ItW=IndexIterator<_Id*, StringItem<_Char, _AllocItem>, _GroupSize, false>;
	using _IndexIteratorBase=IndexIteratorBase<_Id*, StringItem<_Char, _AllocItem>, _GroupSize, true>;
	using _IndexItem=IndexItem<_Id*, StringItem<_Char, _AllocItem>>;
	using _IndexGroup=IndexGroup<_Id*, StringItem<_Char, _AllocItem>>;
	using _IndexItemGroup=IndexItemGroup<_Id*, StringItem<_Char, _AllocItem>, _GroupSize>;
	using _IndexParentGroup=IndexParentGroup<_Id*, StringItem<_Char, _AllocItem>, _GroupSize>;
	using _Cluster=::Clusters::Templates::Details::Cluster::Cluster<_IndexItem, _IndexGroup, _IndexItemGroup, _IndexParentGroup, _ItR, _ItW>;

public:
	// Con-/Destructors
	IndexIterator(_ItR const& It): _IndexIteratorBase(It) {}
	IndexIterator(_ItW const& It): _IndexIteratorBase(It) {}
	IndexIterator(_Cluster const* Cluster, SIZE_T Position): _IndexIteratorBase(Cluster) { this->SetPosition(Position); }
	IndexIterator(_Cluster const* Cluster, SIZE_T, _Id* Id): _IndexIteratorBase(Cluster) { _IndexIteratorBase::FindInternal(Id); }

	// Access
	inline BOOL Find(_Id* Id) { return _IndexIteratorBase::FindInternal(Id); }
	inline _Id* GetCurrentId()const { return this->pCurrent->GetId(); }
	inline _Char const* GetCurrentItem()const { return this->pCurrent->GetItem(); }

	// Assignment
	inline _It& operator=(_ItR const& It) { this->template Assign<_ItR>(It); return *this; }
	inline _It& operator=(_ItW const& It) { this->template Assign<_ItW>(It); return *this; }
};

// Iterator Pointer-Index with Strings Read-Write
template <class _Id, class _Char, UINT _GroupSize>
class IndexIterator<_Id*, StringItem<_Char, true>, _GroupSize, false>: public IndexIteratorBase<_Id*, StringItem<_Char, true>, _GroupSize, false>
{
private:
	// Using
	using _It=IndexIterator<_Id*, StringItem<_Char, true>, _GroupSize, false>;
	using _ItR=IndexIterator<_Id*, StringItem<_Char, true>, _GroupSize, true>;
	using _ItW=IndexIterator<_Id*, StringItem<_Char, true>, _GroupSize, false>;
	using _IndexIteratorBase=IndexIteratorBase<_Id*, StringItem<_Char, true>, _GroupSize, false>;
	using _IndexItem=IndexItem<_Id*, StringItem<_Char, true>>;
	using _IndexGroup=IndexGroup<_Id*, StringItem<_Char, true>>;
	using _IndexItemGroup=IndexItemGroup<_Id*, StringItem<_Char, true>, _GroupSize>;
	using _IndexParentGroup=IndexParentGroup<_Id*, StringItem<_Char, true>, _GroupSize>;
	using _Cluster=::Clusters::Templates::Details::Cluster::Cluster<_IndexItem, _IndexGroup, _IndexItemGroup, _IndexParentGroup, _ItR, _ItW>;

public:
	// Con-/Destructors
	IndexIterator(_ItW const& It): _IndexIteratorBase(It) {}
	IndexIterator(_Cluster* Cluster, SIZE_T Position): _IndexIteratorBase(Cluster) { this->SetPosition(Position); }
	IndexIterator(_Cluster* Cluster, SIZE_T, _Id* Id): _IndexIteratorBase(Cluster) { _IndexIteratorBase::FindInternal(Id); }

	// Access
	inline BOOL Find(_Id* Id) { return _IndexIteratorBase::FindInternal(Id); }
	inline _Id* GetCurrentId()const { return this->pCurrent->GetId(); }
	inline _Char const* GetCurrentItem()const { return this->pCurrent->GetItem(); }

	// Assignment
	inline _It& operator=(_ItW const& It) { this->template Assign<_ItW>(It); return *this; }

	// Modification
	inline UINT SetCurrentItem(_Char const* Item, UINT Length=0) { return this->pCurrent->SetItem(Item, Length); }
};

// Iterator Pointer-Index with Shared Strings Read-Only
template <class _Id, class _Char, UINT _GroupSize>
class IndexIterator<_Id*, StringItem<_Char, false>, _GroupSize, true>: public IndexIteratorBase<_Id*, StringItem<_Char, false>, _GroupSize, true>
{
private:
	// Using
	using _It=IndexIterator<_Id*, StringItem<_Char, false>, _GroupSize, true>;
	using _ItR=IndexIterator<_Id*, StringItem<_Char, false>, _GroupSize, true>;
	using _ItW=IndexIterator<_Id*, StringItem<_Char, false>, _GroupSize, false>;
	using _IndexIteratorBase=IndexIteratorBase<_Id*, StringItem<_Char, false>, _GroupSize, true>;
	using _IndexItem=IndexItem<_Id*, StringItem<_Char, false>>;
	using _IndexGroup=IndexGroup<_Id*, StringItem<_Char, false>>;
	using _IndexItemGroup=IndexItemGroup<_Id*, StringItem<_Char, false>, _GroupSize>;
	using _IndexParentGroup=IndexParentGroup<_Id*, StringItem<_Char, false>, _GroupSize>;
	using _Cluster=::Clusters::Templates::Details::Cluster::Cluster<_IndexItem, _IndexGroup, _IndexItemGroup, _IndexParentGroup, _ItR, _ItW>;

public:
	// Con-/Destructors
	IndexIterator(_ItR const& It): _IndexIteratorBase(It) {}
	IndexIterator(_ItW const& It): _IndexIteratorBase(It) {}
	IndexIterator(_Cluster const* Cluster, SIZE_T Position): _IndexIteratorBase(Cluster) { this->SetPosition(Position); }
	IndexIterator(_Cluster const* Cluster, SIZE_T, _Id* Id): _IndexIteratorBase(Cluster) { _IndexIteratorBase::FindInternal(Id); }

	// Access
	inline BOOL Find(_Id* Id) { return _IndexIteratorBase::FindInternal(Id); }
	inline _Id* GetCurrentId()const { return this->pCurrent->GetId(); }
	inline _Char const* GetCurrentItem()const { return this->pCurrent->GetItem(); }

	// Assignment
	inline _It& operator=(_ItR const& It) { this->template Assign<_ItR>(It); return *this; }
	inline _It& operator=(_ItW const& It) { this->template Assign<_ItW>(It); return *this; }
};

// Iterator Pointer-Index with Shared Strings Read-Write
template <class _Id, class _Char, UINT _GroupSize>
class IndexIterator<_Id*, StringItem<_Char, false>, _GroupSize, false>: public IndexIteratorBase<_Id*, StringItem<_Char, false>, _GroupSize, false>
{
private:
	// Using
	using _It=IndexIterator<_Id*, StringItem<_Char, false>, _GroupSize, false>;
	using _ItR=IndexIterator<_Id*, StringItem<_Char, false>, _GroupSize, true>;
	using _ItW=IndexIterator<_Id*, StringItem<_Char, false>, _GroupSize, false>;
	using _IndexIteratorBase=IndexIteratorBase<_Id*, StringItem<_Char, false>, _GroupSize, false>;
	using _IndexItem=IndexItem<_Id*, StringItem<_Char, false>>;
	using _IndexGroup=IndexGroup<_Id*, StringItem<_Char, false>>;
	using _IndexItemGroup=IndexItemGroup<_Id*, StringItem<_Char, false>, _GroupSize>;
	using _IndexParentGroup=IndexParentGroup<_Id*, StringItem<_Char, false>, _GroupSize>;
	using _Cluster=::Clusters::Templates::Details::Cluster::Cluster<_IndexItem, _IndexGroup, _IndexItemGroup, _IndexParentGroup, _ItR, _ItW>;

public:
	// Con-/Destructors
	IndexIterator(_ItW const& It): _IndexIteratorBase(It) {}
	IndexIterator(_Cluster* Cluster, SIZE_T Position): _IndexIteratorBase(Cluster) { this->SetPosition(Position); }
	IndexIterator(_Cluster* Cluster, SIZE_T, _Id* Id): _IndexIteratorBase(Cluster) { _IndexIteratorBase::FindInternal(Id); }

	// Access
	inline BOOL Find(_Id* Id) { return _IndexIteratorBase::FindInternal(Id); }
	inline _Id* GetCurrentId()const { return this->pCurrent->GetId(); }
	inline _Char const* GetCurrentItem()const { return this->pCurrent->GetItem(); }

	// Assignment
	inline _It& operator=(_ItW const& It) { this->template Assign<_ItW>(It); return *this; }

	// Modification
	inline VOID SetCurrentItem(_Char const* Item) { this->pCurrent->SetItem(Item); }
};


//=======================
// Iterator Handle-Index
//=======================

#ifdef __cplusplus_winrt

// Iterator Handle-Index Read-Only
template <class _Id, class _Item, UINT _GroupSize>
class IndexIterator<_Id^, _Item, _GroupSize, true>: public IndexIteratorBase<_Id^, _Item, _GroupSize, true>
{
private:
	// Using
	using _It=IndexIterator<_Id^, _Item, _GroupSize, true>;
	using _ItR=IndexIterator<_Id^, _Item, _GroupSize, true>;
	using _ItW=IndexIterator<_Id^, _Item, _GroupSize, false>;
	using _IndexIteratorBase=IndexIteratorBase<_Id^, _Item, _GroupSize, true>;
	using _IndexItem=IndexItem<_Id^, _Item>;
	using _IndexGroup=IndexGroup<_Id^, _Item>;
	using _IndexItemGroup=IndexItemGroup<_Id^, _Item, _GroupSize>;
	using _IndexParentGroup=IndexParentGroup<_Id^, _Item, _GroupSize>;
	using _Cluster=::Clusters::Templates::Details::Cluster::Cluster<_IndexItem, _IndexGroup, _IndexItemGroup, _IndexParentGroup, _ItR, _ItW>;

public:
	// Con-/Destructors
	IndexIterator(_ItR const& It): _IndexIteratorBase(It) {}
	IndexIterator(_ItW const& It): _IndexIteratorBase(It) {}
	IndexIterator(_Cluster const* Cluster, SIZE_T Position): _IndexIteratorBase(Cluster) { this->SetPosition(Position); }
	IndexIterator(_Cluster const* Cluster, _Id^ Id): _IndexIteratorBase(Cluster) { _IndexIteratorBase::FindInternal(Id); }

	// Access
	inline BOOL Find(_Id^ Id) { return _IndexIteratorBase::FindInternal(Id); }
	inline _Id^ GetCurrentId()const { return this->pCurrent->GetId(); }
	inline _Item const& GetCurrentItem()const { return this->pCurrent->GetItem(); }

	// Assignment
	inline _It& operator=(_ItR const& It) { this->template Assign<_ItR>(It); return *this; }
	inline _It& operator=(_ItW const& It) { this->template Assign<_ItW>(It); return *this; }
};

// Iterator Handle-Index Read-Write
template <class _Id, class _Item, UINT _GroupSize>
class IndexIterator<_Id^, _Item, _GroupSize, false>: public IndexIteratorBase<_Id^, _Item, _GroupSize, false>
{
private:
	// Using
	using _It=IndexIterator<_Id^, _Item, _GroupSize, false>;
	using _ItR=IndexIterator<_Id^, _Item, _GroupSize, true>;
	using _ItW=IndexIterator<_Id^, _Item, _GroupSize, false>;
	using _IndexIteratorBase=IndexIteratorBase<_Id^, _Item, _GroupSize, false>;
	using _IndexItem=IndexItem<_Id^, _Item>;
	using _IndexGroup=IndexGroup<_Id^, _Item>;
	using _IndexItemGroup=IndexItemGroup<_Id^, _Item, _GroupSize>;
	using _IndexParentGroup=IndexParentGroup<_Id^, _Item, _GroupSize>;
	using _Cluster=::Clusters::Templates::Details::Cluster::Cluster<_IndexItem, _IndexGroup, _IndexItemGroup, _IndexParentGroup, _ItR, _ItW>;

public:
	// Con-/Destructors
	IndexIterator(_ItW const& It): _IndexIteratorBase(It) {}
	IndexIterator(_Cluster* Cluster, SIZE_T Position): _IndexIteratorBase(Cluster) { this->SetPosition(Position); }
	IndexIterator(_Cluster* Cluster, _Id^ Id): _IndexIteratorBase(Cluster) { _IndexIteratorBase::FindInternal(Id); }

	// Access
	inline BOOL Find(_Id^ Id) { return _IndexIteratorBase::FindInternal(Id); }
	inline _Id^ GetCurrentId()const { return this->pCurrent->GetId(); }
	inline _Item& GetCurrentItem()const { return this->pCurrent->GetItem(); }

	// Assignment
	inline _It& operator=(_ItW const& It) { this->template Assign<_ItW>(It); return *this; }

	// Modification
	inline VOID SetCurrentItem(_Item const& Item) { this->pCurrent->SetItem(Item); }
};

// Iterator Handle-Index without Items
template <class _Id, UINT _GroupSize>
class IndexIterator<_Id^, VOID, _GroupSize, true>: public IndexIteratorBase<_Id^, VOID, _GroupSize, true>
{
private:
	// Using
	using _It=IndexIterator<_Id^, VOID, _GroupSize, true>;
	using _ItR=IndexIterator<_Id^, VOID, _GroupSize, true>;
	using _ItW=IndexIterator<_Id^, VOID, _GroupSize, false>;
	using _IndexIteratorBase=IndexIteratorBase<_Id^, VOID, _GroupSize, true>;
	using _IndexItem=IndexItem<_Id^, VOID>;
	using _IndexGroup=IndexGroup<_Id^, VOID>;
	using _IndexItemGroup=IndexItemGroup<_Id^, VOID, _GroupSize>;
	using _IndexParentGroup=IndexParentGroup<_Id^, VOID, _GroupSize>;
	using _Cluster=::Clusters::Templates::Details::Cluster::Cluster<_IndexItem, _IndexGroup, _IndexItemGroup, _IndexParentGroup, _ItR, _ItW>;

public:
	// Con-/Destructors
	IndexIterator(_ItR const& It): _IndexIteratorBase(It) {}
	IndexIterator(_ItW const& It): _IndexIteratorBase(It) {}
	IndexIterator(_Cluster const* Cluster, SIZE_T Position): _IndexIteratorBase(Cluster) { this->SetPosition(Position); }
	IndexIterator(_Cluster const* Cluster, _Id^ Id): _IndexIteratorBase(Cluster) { _IndexIteratorBase::FindInternal(Id); }

	// Access
	inline BOOL Find(_Id^ Id) { return _IndexIteratorBase::FindInternal(Id); }
	inline _Id^ GetCurrent()const { return this->pCurrent->GetId(); }

	// Assignment
	inline _It& operator=(_ItR const& It) { this->template Assign<_ItR>(It); return *this; }
	inline _It& operator=(_ItW const& It) { this->template Assign<_ItW>(It); return *this; }
};

// Iterator Handle-Index without Items Read-Write
template <class _Id, UINT _GroupSize>
class IndexIterator<_Id^, VOID, _GroupSize, false>: public IndexIteratorBase<_Id^, VOID, _GroupSize, false>
{
private:
	// Using
	using _It=IndexIterator<_Id^, VOID, _GroupSize, false>;
	using _ItR=IndexIterator<_Id^, VOID, _GroupSize, true>;
	using _ItW=IndexIterator<_Id^, VOID, _GroupSize, false>;
	using _IndexIteratorBase=IndexIteratorBase<_Id^, VOID, _GroupSize, false>;
	using _IndexItem=IndexItem<_Id^, VOID>;
	using _IndexGroup=IndexGroup<_Id^, VOID>;
	using _IndexItemGroup=IndexItemGroup<_Id^, VOID, _GroupSize>;
	using _IndexParentGroup=IndexParentGroup<_Id^, VOID, _GroupSize>;
	using _Cluster=::Clusters::Templates::Details::Cluster::Cluster<_IndexItem, _IndexGroup, _IndexItemGroup, _IndexParentGroup, _ItR, _ItW>;

public:
	// Con-/Destructors
	IndexIterator(_ItW const& It): _IndexIteratorBase(It) {}
	IndexIterator(_Cluster* Cluster, SIZE_T Position): _IndexIteratorBase(Cluster) { this->SetPosition(Position); }
	IndexIterator(_Cluster* Cluster, _Id^ Id): _IndexIteratorBase(Cluster) { _IndexIteratorBase::FindInternal(Id); }

	// Access
	inline BOOL Find(_Id^ Id) { return _IndexIteratorBase::FindInternal(Id); }
	inline _Id^ GetCurrent()const { return this->pCurrent->GetId(); }

	// Assignment
	inline _It& operator=(_ItW const& It) { this->template Assign<_ItW>(It); return *this; }
};

// Iterator Handle-Index with Pointers Read-Only
template <class _Id, class _Item, UINT _GroupSize>
class IndexIterator<_Id^, _Item*, _GroupSize, true>: public IndexIteratorBase<_Id^, _Item*, _GroupSize, true>
{
private:
	// Using
	using _It=IndexIterator<_Id^, _Item*, _GroupSize, true>;
	using _ItR=IndexIterator<_Id^, _Item*, _GroupSize, true>;
	using _ItW=IndexIterator<_Id^, _Item*, _GroupSize, false>;
	using _IndexIteratorBase=IndexIteratorBase<_Id^, _Item*, _GroupSize, true>;
	using _IndexItem=IndexItem<_Id^, _Item*>;
	using _IndexGroup=IndexGroup<_Id^, _Item*>;
	using _IndexItemGroup=IndexItemGroup<_Id^, _Item*, _GroupSize>;
	using _IndexParentGroup=IndexParentGroup<_Id^, _Item*, _GroupSize>;
	using _Cluster=::Clusters::Templates::Details::Cluster::Cluster<_IndexItem, _IndexGroup, _IndexItemGroup, _IndexParentGroup, _ItR, _ItW>;

public:
	// Con-/Destructors
	IndexIterator(_ItR const& It): _IndexIteratorBase(It) {}
	IndexIterator(_ItW const& It): _IndexIteratorBase(It) {}
	IndexIterator(_Cluster const* Cluster, SIZE_T Position): _IndexIteratorBase(Cluster) { this->SetPosition(Position); }
	IndexIterator(_Cluster const* Cluster, _Id^ Id): _IndexIteratorBase(Cluster) { _IndexIteratorBase::FindInternal(Id); }

	// Access
	inline BOOL Find(_Id^ Id) { return _IndexIteratorBase::FindInternal(Id); }
	inline _Id^ GetCurrentId()const { return this->pCurrent->GetId(); }
	inline _Item* GetCurrentItem()const { return this->pCurrent->GetItem(); }

	// Assignment
	inline _It& operator=(_ItR const& It) { this->template Assign<_ItR>(It); return *this; }
	inline _It& operator=(_ItW const& It) { this->template Assign<_ItW>(It); return *this; }
};

// Iterator Handle-Index with Pointers Read-Write
template <class _Id, class _Item, UINT _GroupSize>
class IndexIterator<_Id^, _Item*, _GroupSize, false>: public IndexIteratorBase<_Id^, _Item*, _GroupSize, false>
{
private:
	// Using
	using _It=IndexIterator<_Id^, _Item*, _GroupSize, false>;
	using _ItR=IndexIterator<_Id^, _Item*, _GroupSize, true>;
	using _ItW=IndexIterator<_Id^, _Item*, _GroupSize, false>;
	using _IndexIteratorBase=IndexIteratorBase<_Id^, _Item*, _GroupSize, false>;
	using _IndexItem=IndexItem<_Id^, _Item*>;
	using _IndexGroup=IndexGroup<_Id^, _Item*>;
	using _IndexItemGroup=IndexItemGroup<_Id^, _Item*, _GroupSize>;
	using _IndexParentGroup=IndexParentGroup<_Id^, _Item*, _GroupSize>;
	using _Cluster=::Clusters::Templates::Details::Cluster::Cluster<_IndexItem, _IndexGroup, _IndexItemGroup, _IndexParentGroup, _ItR, _ItW>;

public:
	// Con-/Destructors
	IndexIterator(_ItW const& It): _IndexIteratorBase(It) {}
	IndexIterator(_Cluster* Cluster, SIZE_T Position): _IndexIteratorBase(Cluster) { this->SetPosition(Position); }
	IndexIterator(_Cluster* Cluster, _Id^ Id): _IndexIteratorBase(Cluster) { _IndexIteratorBase::FindInternal(Id); }

	// Access
	inline BOOL Find(_Id^ Id) { return _IndexIteratorBase::FindInternal(Id); }
	inline _Id^ GetCurrentId()const { return this->pCurrent->GetId(); }
	inline _Item* GetCurrentItem()const { return this->pCurrent->GetItem(); }

	// Assignment
	inline _It& operator=(_ItW const& It) { this->template Assign<_ItW>(It); return *this; }

	// Modification
	inline VOID SetCurrentItem(_Item* Item) { this->pCurrent->SetItem(Item); }
};

// Iterator Handle-Index with Handles Read-Only
template <class _Id, class _Item, UINT _GroupSize>
class IndexIterator<_Id^, _Item^, _GroupSize, true>: public IndexIteratorBase<_Id^, _Item^, _GroupSize, true>
{
private:
	// Using
	using _It=IndexIterator<_Id^, _Item^, _GroupSize, true>;
	using _ItR=IndexIterator<_Id^, _Item^, _GroupSize, true>;
	using _ItW=IndexIterator<_Id^, _Item^, _GroupSize, false>;
	using _IndexIteratorBase=IndexIteratorBase<_Id^, _Item^, _GroupSize, true>;
	using _IndexItem=IndexItem<_Id^, _Item^>;
	using _IndexGroup=IndexGroup<_Id^, _Item^>;
	using _IndexItemGroup=IndexItemGroup<_Id^, _Item^, _GroupSize>;
	using _IndexParentGroup=IndexParentGroup<_Id^, _Item^, _GroupSize>;
	using _Cluster=::Clusters::Templates::Details::Cluster::Cluster<_IndexItem, _IndexGroup, _IndexItemGroup, _IndexParentGroup, _ItR, _ItW>;

public:
	// Con-/Destructors
	IndexIterator(_ItR const& It): _IndexIteratorBase(It) {}
	IndexIterator(_ItW const& It): _IndexIteratorBase(It) {}
	IndexIterator(_Cluster const* Cluster, SIZE_T Position): _IndexIteratorBase(Cluster) { this->SetPosition(Position); }
	IndexIterator(_Cluster const* Cluster, _Id^ Id): _IndexIteratorBase(Cluster) { _IndexIteratorBase::FindInternal(Id); }

	// Access
	inline BOOL Find(_Id^ Id) { return _IndexIteratorBase::FindInternal(Id); }
	inline _Id^ GetCurrentId()const { return this->pCurrent->GetId(); }
	inline _Item^ GetCurrentItem()const { return this->pCurrent->GetItem(); }

	// Assignment
	inline _It& operator=(_ItR const& It) { this->template Assign<_ItR>(It); return *this; }
	inline _It& operator=(_ItW const& It) { this->template Assign<_ItW>(It); return *this; }
};

// Iterator Handle-Index with Handles Read-Write
template <class _Id, class _Item, UINT _GroupSize>
class IndexIterator<_Id^, _Item^, _GroupSize, false>: public IndexIteratorBase<_Id^, _Item^, _GroupSize, false>
{
private:
	// Using
	using _It=IndexIterator<_Id^, _Item^, _GroupSize, false>;
	using _ItR=IndexIterator<_Id^, _Item^, _GroupSize, true>;
	using _ItW=IndexIterator<_Id^, _Item^, _GroupSize, false>;
	using _IndexIteratorBase=IndexIteratorBase<_Id^, _Item^, _GroupSize, false>;
	using _IndexItem=IndexItem<_Id^, _Item^>;
	using _IndexGroup=IndexGroup<_Id^, _Item^>;
	using _IndexItemGroup=IndexItemGroup<_Id^, _Item^, _GroupSize>;
	using _IndexParentGroup=IndexParentGroup<_Id^, _Item^, _GroupSize>;
	using _Cluster=::Clusters::Templates::Details::Cluster::Cluster<_IndexItem, _IndexGroup, _IndexItemGroup, _IndexParentGroup, _ItR, _ItW>;

public:
	// Con-/Destructors
	IndexIterator(_ItW const& It): _IndexIteratorBase(It) {}
	IndexIterator(_Cluster* Cluster, SIZE_T Position): _IndexIteratorBase(Cluster) { this->SetPosition(Position); }
	IndexIterator(_Cluster* Cluster, _Id^ Id): _IndexIteratorBase(Cluster) { _IndexIteratorBase::FindInternal(Id); }

	// Access
	inline BOOL Find(_Id^ Id) { return _IndexIteratorBase::FindInternal(Id); }
	inline _Id^ GetCurrentId()const { return this->pCurrent->GetId(); }
	inline _Item^ GetCurrentItem()const { return this->pCurrent->GetItem(); }

	// Assignment
	inline _It& operator=(_ItW const& It) { this->template Assign<_ItW>(It); return *this; }

	// Modification
	inline VOID SetCurrentItem(_Item^ Item) { this->pCurrent->SetItem(Item); }
};

// Iterator Handle-Index with Strings Read-Only
template <class _Id, class _Char, BOOL _AllocItem, UINT _GroupSize>
class IndexIterator<_Id^, StringItem<_Char, _AllocItem>, _GroupSize, true>: public IndexIteratorBase<_Id^, StringItem<_Char, _AllocItem>, _GroupSize, true>
{
private:
	// Using
	using _It=IndexIterator<_Id^, StringItem<_Char, _AllocItem>, _GroupSize, true>;
	using _ItR=IndexIterator<_Id^, StringItem<_Char, _AllocItem>, _GroupSize, true>;
	using _ItW=IndexIterator<_Id^, StringItem<_Char, _AllocItem>, _GroupSize, false>;
	using _IndexIteratorBase=IndexIteratorBase<_Id^, StringItem<_Char, _AllocItem>, _GroupSize, true>;
	using _IndexItem=IndexItem<_Id^, StringItem<_Char, _AllocItem>>;
	using _IndexGroup=IndexGroup<_Id^, StringItem<_Char, _AllocItem>>;
	using _IndexItemGroup=IndexItemGroup<_Id^, StringItem<_Char, _AllocItem>, _GroupSize>;
	using _IndexParentGroup=IndexParentGroup<_Id^, StringItem<_Char, _AllocItem>, _GroupSize>;
	using _Cluster=::Clusters::Templates::Details::Cluster::Cluster<_IndexItem, _IndexGroup, _IndexItemGroup, _IndexParentGroup, _ItR, _ItW>;

public:
	// Con-/Destructors
	IndexIterator(_ItR const& It): _IndexIteratorBase(It) {}
	IndexIterator(_ItW const& It): _IndexIteratorBase(It) {}
	IndexIterator(_Cluster const* Cluster, SIZE_T Position): _IndexIteratorBase(Cluster) { this->SetPosition(Position); }
	IndexIterator(_Cluster const* Cluster, _Char const* Id, UINT Length=0, BOOL CaseSensitive=false): _IndexIteratorBase(Cluster) { _IndexIteratorBase::FindInternal(Id, Length, CaseSensitive); }

	// Access
	inline BOOL Find(_Char const* Id, UINT Length=0, BOOL CaseSensitive=false) { return _IndexIteratorBase::FindInternal(Id, Length, CaseSensitive); }
	inline _Id^ GetCurrentId()const { return this->pCurrent->GetId(); }
	inline _Char const* GetCurrentItem()const { return this->pCurrent->GetItem(); }

	// Assignment
	inline _It& operator=(_ItR const& It) { this->template Assign<_ItR>(It); return *this; }
	inline _It& operator=(_ItW const& It) { this->template Assign<_ItW>(It); return *this; }
};

// Iterator Handle-Index with Strings Read-Write
template <class _Id, class _Char, UINT _GroupSize>
class IndexIterator<_Id^, StringItem<_Char, true>, _GroupSize, false>: public IndexIteratorBase<_Id^, StringItem<_Char, true>, _GroupSize, false>
{
private:
	// Using
	using _It=IndexIterator<_Id^, StringItem<_Char, true>, _GroupSize, false>;
	using _ItR=IndexIterator<_Id^, StringItem<_Char, true>, _GroupSize, true>;
	using _ItW=IndexIterator<_Id^, StringItem<_Char, true>, _GroupSize, false>;
	using _IndexIteratorBase=IndexIteratorBase<_Id^, StringItem<_Char, true>, _GroupSize, false>;
	using _IndexItem=IndexItem<_Id^, StringItem<_Char, true>>;
	using _IndexGroup=IndexGroup<_Id^, StringItem<_Char, true>>;
	using _IndexItemGroup=IndexItemGroup<_Id^, StringItem<_Char, true>, _GroupSize>;
	using _IndexParentGroup=IndexParentGroup<_Id^, StringItem<_Char, true>, _GroupSize>;
	using _Cluster=::Clusters::Templates::Details::Cluster::Cluster<_IndexItem, _IndexGroup, _IndexItemGroup, _IndexParentGroup, _ItR, _ItW>;

public:
	// Con-/Destructors
	IndexIterator(_ItW const& It): _IndexIteratorBase(It) {}
	IndexIterator(_Cluster* Cluster, SIZE_T Position): _IndexIteratorBase(Cluster) { this->SetPosition(Position); }
	IndexIterator(_Cluster* Cluster, _Id^ Id): _IndexIteratorBase(Cluster) { _IndexIteratorBase::FindInternal(Id); }

	// Access
	inline BOOL Find(_Id^ Id) { return _IndexIteratorBase::FindInternal(Id); }
	inline _Id^ GetCurrentId()const { return this->pCurrent->GetId(); }
	inline _Char const* GetCurrentItem()const { return this->pCurrent->GetItem(); }

	// Assignment
	inline _It& operator=(_ItW const& It) { this->template Assign<_ItW>(It); return *this; }

	// Modification
	inline UINT SetCurrentItem(_Char const* Item, UINT Length=0) { return this->pCurrent->SetItem(Item, Length); }
};

// Iterator Handle-Index with Shared Strings Read-Only
template <class _Id, class _Char, UINT _GroupSize>
class IndexIterator<_Id^, StringItem<_Char, false>, _GroupSize, true>: public IndexIteratorBase<_Id^, StringItem<_Char, false>, _GroupSize, true>
{
private:
	// Using
	using _It=IndexIterator<_Id^, StringItem<_Char, false>, _GroupSize, true>;
	using _ItR=IndexIterator<_Id^, StringItem<_Char, false>, _GroupSize, true>;
	using _ItW=IndexIterator<_Id^, StringItem<_Char, false>, _GroupSize, false>;
	using _IndexIteratorBase=IndexIteratorBase<_Id^, StringItem<_Char, false>, _GroupSize, true>;
	using _IndexItem=IndexItem<_Id^, StringItem<_Char, false>>;
	using _IndexGroup=IndexGroup<_Id^, StringItem<_Char, false>>;
	using _IndexItemGroup=IndexItemGroup<_Id^, StringItem<_Char, false>, _GroupSize>;
	using _IndexParentGroup=IndexParentGroup<_Id^, StringItem<_Char, false>, _GroupSize>;
	using _Cluster=::Clusters::Templates::Details::Cluster::Cluster<_IndexItem, _IndexGroup, _IndexItemGroup, _IndexParentGroup, _ItR, _ItW>;

public:
	// Con-/Destructors
	IndexIterator(_ItR const& It): _IndexIteratorBase(It) {}
	IndexIterator(_ItW const& It): _IndexIteratorBase(It) {}
	IndexIterator(_Cluster const* Cluster, SIZE_T Position): _IndexIteratorBase(Cluster) { this->SetPosition(Position); }
	IndexIterator(_Cluster const* Cluster, _Char const* Id, UINT Length=0, BOOL CaseSensitive=false): _IndexIteratorBase(Cluster) { _IndexIteratorBase::FindInternal(Id, Length, CaseSensitive); }

	// Access
	inline BOOL Find(_Char const* Id, UINT Length=0, BOOL CaseSensitive=false) { return _IndexIteratorBase::FindInternal(Id, Length, CaseSensitive); }
	inline _Id^ GetCurrentId()const { return this->pCurrent->GetId(); }
	inline _Char const* GetCurrentItem()const { return this->pCurrent->GetItem(); }

	// Assignment
	inline _It& operator=(_ItR const& It) { this->template Assign<_ItR>(It); return *this; }
	inline _It& operator=(_ItW const& It) { this->template Assign<_ItW>(It); return *this; }
};

// Iterator Handle-Index with Shared Strings Read-Write
template <class _Id, class _Char, UINT _GroupSize>
class IndexIterator<_Id^, StringItem<_Char, false>, _GroupSize, false>: public IndexIteratorBase<_Id^, StringItem<_Char, false>, _GroupSize, false>
{
private:
	// Using
	using _It=IndexIterator<_Id^, StringItem<_Char, false>, _GroupSize, false>;
	using _ItR=IndexIterator<_Id^, StringItem<_Char, false>, _GroupSize, true>;
	using _ItW=IndexIterator<_Id^, StringItem<_Char, false>, _GroupSize, false>;
	using _IndexIteratorBase=IndexIteratorBase<_Id^, StringItem<_Char, false>, _GroupSize, false>;
	using _IndexItem=IndexItem<_Id^, StringItem<_Char, false>>;
	using _IndexGroup=IndexGroup<_Id^, StringItem<_Char, false>>;
	using _IndexItemGroup=IndexItemGroup<_Id^, StringItem<_Char, false>, _GroupSize>;
	using _IndexParentGroup=IndexParentGroup<_Id^, StringItem<_Char, false>, _GroupSize>;
	using _Cluster=::Clusters::Templates::Details::Cluster::Cluster<_IndexItem, _IndexGroup, _IndexItemGroup, _IndexParentGroup, _ItR, _ItW>;

public:
	// Con-/Destructors
	IndexIterator(_ItW const& It): _IndexIteratorBase(It) {}
	IndexIterator(_Cluster* Cluster, SIZE_T Position): _IndexIteratorBase(Cluster) { this->SetPosition(Position); }
	IndexIterator(_Cluster* Cluster, _Id^ Id): _IndexIteratorBase(Cluster) { _IndexIteratorBase::FindInternal(Id); }

	// Access
	inline BOOL Find(_Id^ Id) { return _IndexIteratorBase::FindInternal(Id); }
	inline _Id^ GetCurrentId()const { return this->pCurrent->GetId(); }
	inline _Char const* GetCurrentItem()const { return this->pCurrent->GetItem(); }

	// Assignment
	inline _It& operator=(_ItW const& It) { this->template Assign<_ItW>(It); return *this; }

	// Modification
	inline VOID SetCurrentItem(_Char const* Item) { this->pCurrent->SetItem(Item); }
};

#endif


//=======================
// Iterator String-Index
//=======================

// Iterator String-Index Read-Only
template <class _Char, BOOL _AllocId, class _Item, UINT _GroupSize>
class IndexIterator<StringItem<_Char, _AllocId>, _Item, _GroupSize, true>: public IndexIteratorBase<StringItem<_Char, true>, _Item, _GroupSize, true>
{
private:
	// Using
	using _It=IndexIterator<StringItem<_Char, _AllocId>, _Item, _GroupSize, true>;
	using _ItR=IndexIterator<StringItem<_Char, _AllocId>, _Item, _GroupSize, true>;
	using _ItW=IndexIterator<StringItem<_Char, _AllocId>, _Item, _GroupSize, false>;
	using _IndexIteratorBase=IndexIteratorBase<StringItem<_Char, _AllocId>, _Item, _GroupSize, true>;
	using _IndexItem=IndexItem<StringItem<_Char, _AllocId>, _Item>;
	using _IndexGroup=IndexGroup<StringItem<_Char, _AllocId>, _Item>;
	using _IndexItemGroup=IndexItemGroup<StringItem<_Char, _AllocId>, _Item, _GroupSize>;
	using _IndexParentGroup=IndexParentGroup<StringItem<_Char, _AllocId>, _Item, _GroupSize>;
	using _Cluster=::Clusters::Templates::Details::Cluster::Cluster<_IndexItem, _IndexGroup, _IndexItemGroup, _IndexParentGroup, _ItR, _ItW>;

public:
	// Con-/Destructors
	IndexIterator(_ItR const& It): _IndexIteratorBase(It) {}
	IndexIterator(_ItW const& It): _IndexIteratorBase(It) {}
	IndexIterator(_Cluster const* Cluster, SIZE_T Position): _IndexIteratorBase(Cluster) { this->SetPosition(Position); }
	IndexIterator(_Cluster const* Cluster, _Char const* Id, UINT Length, BOOL CaseSensitive): _IndexIteratorBase(Cluster) { _IndexIteratorBase::FindInternal(Id, Length, CaseSensitive); }

	// Access
	inline BOOL Find(_Char const* Id, UINT Length=0, BOOL CaseSensitive=false) { return _IndexIteratorBase::FindInternal(Id, Length, CaseSensitive); }
	inline _Char const* GetCurrentId()const { return this->pCurrent->GetId(); }
	inline _Item const& GetCurrentItem()const { return this->pCurrent->GetItem(); }

	// Assignment
	inline _It& operator=(_ItR const& It) { this->template Assign<_ItR>(It); return *this; }
	inline _It& operator=(_ItW const& It) { this->template Assign<_ItW>(It); return *this; }
};

// Iterator String-Index Read-Write
template <class _Char, BOOL _AllocId, class _Item, UINT _GroupSize>
class IndexIterator<StringItem<_Char, _AllocId>, _Item, _GroupSize, false>: public IndexIteratorBase<StringItem<_Char, _AllocId>, _Item, _GroupSize, false>
{
private:
	// Using
	using _It=IndexIterator<StringItem<_Char, _AllocId>, _Item, _GroupSize, false>;
	using _ItR=IndexIterator<StringItem<_Char, _AllocId>, _Item, _GroupSize, true>;
	using _ItW=IndexIterator<StringItem<_Char, _AllocId>, _Item, _GroupSize, false>;
	using _IndexIteratorBase=IndexIteratorBase<StringItem<_Char, _AllocId>, _Item, _GroupSize, false>;
	using _IndexItem=IndexItem<StringItem<_Char, _AllocId>, _Item>;
	using _IndexGroup=IndexGroup<StringItem<_Char, _AllocId>, _Item>;
	using _IndexItemGroup=IndexItemGroup<StringItem<_Char, _AllocId>, _Item, _GroupSize>;
	using _IndexParentGroup=IndexParentGroup<StringItem<_Char, _AllocId>, _Item, _GroupSize>;
	using _Cluster=::Clusters::Templates::Details::Cluster::Cluster<_IndexItem, _IndexGroup, _IndexItemGroup, _IndexParentGroup, _ItR, _ItW>;

public:
	// Con-/Destructors
	IndexIterator(_ItW const& It): _IndexIteratorBase(It) {}
	IndexIterator(_Cluster* Cluster, SIZE_T Position): _IndexIteratorBase(Cluster) { this->SetPosition(Position); }
	IndexIterator(_Cluster* Cluster, _Char const* Id, UINT Length, BOOL CaseSensitive): _IndexIteratorBase(Cluster) { _IndexIteratorBase::FindInternal(Id, Length, CaseSensitive); }

	// Access
	inline BOOL Find(_Char const* Id, UINT Length=0, BOOL CaseSensitive=false) { return _IndexIteratorBase::FindInternal(Id, Length, CaseSensitive); }
	inline _Char const* GetCurrentId()const { return this->pCurrent->GetId(); }
	inline _Item& GetCurrentItem()const { return this->pCurrent->GetItem(); }

	// Assignment
	inline _It& operator=(_ItW const& It) { this->template Assign<_ItW>(It); return *this; }

	// Modification
	inline VOID SetCurrentItem(_Item const& Item) { return this->pCurrent->SetItem(Item); }
};

// Iterator String-Index without Items Read-Only
template <class _Char, BOOL _AllocId, UINT _GroupSize>
class IndexIterator<StringItem<_Char, _AllocId>, VOID, _GroupSize, true>: public IndexIteratorBase<StringItem<_Char, _AllocId>, VOID, _GroupSize, true>
{
private:
	// Using
	using _It=IndexIterator<StringItem<_Char, _AllocId>, VOID, _GroupSize, true>;
	using _ItR=IndexIterator<StringItem<_Char, _AllocId>, VOID, _GroupSize, true>;
	using _ItW=IndexIterator<StringItem<_Char, _AllocId>, VOID, _GroupSize, false>;
	using _IndexIteratorBase=IndexIteratorBase<StringItem<_Char, _AllocId>, VOID, _GroupSize, true>;
	using _IndexItem=IndexItem<StringItem<_Char, _AllocId>, VOID>;
	using _IndexGroup=IndexGroup<StringItem<_Char, _AllocId>, VOID>;
	using _IndexItemGroup=IndexItemGroup<StringItem<_Char, _AllocId>, VOID, _GroupSize>;
	using _IndexParentGroup=IndexParentGroup<StringItem<_Char, _AllocId>, VOID, _GroupSize>;
	using _Cluster=::Clusters::Templates::Details::Cluster::Cluster<_IndexItem, _IndexGroup, _IndexItemGroup, _IndexParentGroup, _ItR, _ItW>;

public:
	// Con-/Destructors
	IndexIterator(_ItR const& It): _IndexIteratorBase(It) {}
	IndexIterator(_ItW const& It): _IndexIteratorBase(It) {}
	IndexIterator(_Cluster const* Cluster, SIZE_T Position): _IndexIteratorBase(Cluster) { this->SetPosition(Position); }
	IndexIterator(_Cluster const* Cluster, _Char const* Id, UINT Length, BOOL CaseSensitive): _IndexIteratorBase(Cluster) { _IndexIteratorBase::FindInternal(Id, Length, CaseSensitive); }

	// Access
	inline BOOL Find(_Char const* Id, UINT Length=0, BOOL CaseSensitive=false) { return _IndexIteratorBase::FindInternal(Id, Length, CaseSensitive); }
	inline _Char const* GetCurrent()const { return this->pCurrent->GetId(); }

	// Assignment
	inline _It& operator=(_ItR const& It) { this->template Assign<_ItR>(It); return *this; }
	inline _It& operator=(_ItW const& It) { this->template Assign<_ItW>(It); return *this; }
};

// Iterator String-Index without Items Read-Write
template <class _Char, BOOL _AllocId, UINT _GroupSize>
class IndexIterator<StringItem<_Char, _AllocId>, VOID, _GroupSize, false>: public IndexIteratorBase<StringItem<_Char, _AllocId>, VOID, _GroupSize, false>
{
private:
	// Using
	using _It=IndexIterator<StringItem<_Char, _AllocId>, VOID, _GroupSize, false>;
	using _ItR=IndexIterator<StringItem<_Char, _AllocId>, VOID, _GroupSize, true>;
	using _ItW=IndexIterator<StringItem<_Char, _AllocId>, VOID, _GroupSize, false>;
	using _IndexIteratorBase=IndexIteratorBase<StringItem<_Char, _AllocId>, VOID, _GroupSize, false>;
	using _IndexItem=IndexItem<StringItem<_Char, _AllocId>, VOID>;
	using _IndexGroup=IndexGroup<StringItem<_Char, _AllocId>, VOID>;
	using _IndexItemGroup=IndexItemGroup<StringItem<_Char, _AllocId>, VOID, _GroupSize>;
	using _IndexParentGroup=IndexParentGroup<StringItem<_Char, _AllocId>, VOID, _GroupSize>;
	using _Cluster=::Clusters::Templates::Details::Cluster::Cluster<_IndexItem, _IndexGroup, _IndexItemGroup, _IndexParentGroup, _ItR, _ItW>;

public:
	// Con-/Destructors
	IndexIterator(_ItW const& It): _IndexIteratorBase(It) {}
	IndexIterator(_Cluster* Cluster, SIZE_T Position): _IndexIteratorBase(Cluster) { this->SetPosition(Position); }
	IndexIterator(_Cluster* Cluster, _Char const* Id, UINT Length, BOOL CaseSensitive): _IndexIteratorBase(Cluster) { _IndexIteratorBase::FindInternal(Id, Length, CaseSensitive); }

	// Access
	inline BOOL Find(_Char const* Id, UINT Length=0, BOOL CaseSensitive=false) { return _IndexIteratorBase::FindInternal(Id, Length, CaseSensitive); }
	inline _Char const* GetCurrent()const { return this->pCurrent->GetId(); }

	// Assignment
	inline _It& operator=(_ItW const& It) { this->template Assign<_ItW>(It); return *this; }
};

// Iterator String-Index with Pointers Read-Only
template <class _Char, BOOL _AllocId, class _Item, UINT _GroupSize>
class IndexIterator<StringItem<_Char, _AllocId>, _Item*, _GroupSize, true>: public IndexIteratorBase<StringItem<_Char, _AllocId>, _Item*, _GroupSize, true>
{
private:
	// Using
	using _It=IndexIterator<StringItem<_Char, _AllocId>, _Item*, _GroupSize, true>;
	using _ItR=IndexIterator<StringItem<_Char, _AllocId>, _Item*, _GroupSize, true>;
	using _ItW=IndexIterator<StringItem<_Char, _AllocId>, _Item*, _GroupSize, false>;
	using _IndexIteratorBase=IndexIteratorBase<StringItem<_Char, _AllocId>, _Item*, _GroupSize, true>;
	using _IndexItem=IndexItem<StringItem<_Char, _AllocId>, _Item*>;
	using _IndexGroup=IndexGroup<StringItem<_Char, _AllocId>, _Item*>;
	using _IndexItemGroup=IndexItemGroup<StringItem<_Char, _AllocId>, _Item*, _GroupSize>;
	using _IndexParentGroup=IndexParentGroup<StringItem<_Char, _AllocId>, _Item*, _GroupSize>;
	using _Cluster=::Clusters::Templates::Details::Cluster::Cluster<_IndexItem, _IndexGroup, _IndexItemGroup, _IndexParentGroup, _ItR, _ItW>;

public:
	// Con-/Destructors
	IndexIterator(_ItR const& It): _IndexIteratorBase(It) {}
	IndexIterator(_ItW const& It): _IndexIteratorBase(It) {}
	IndexIterator(_Cluster const* Cluster, SIZE_T Position): _IndexIteratorBase(Cluster) { this->SetPosition(Position); }
	IndexIterator(_Cluster const* Cluster, _Char const* Id, UINT Length, BOOL CaseSensitive): _IndexIteratorBase(Cluster) { _IndexIteratorBase::FindInternal(Id, Length, CaseSensitive); }

	// Access
	inline BOOL Find(_Char const* Id, UINT Length=0, BOOL CaseSensitive=false) { return _IndexIteratorBase::FindInternal(Id, Length, CaseSensitive); }
	inline _Char const* GetCurrentId()const { return this->pCurrent->GetId(); }
	inline _Item* GetCurrentItem()const { return this->pCurrent->GetItem(); }

	// Assignment
	inline _It& operator=(_ItR const& It) { this->template Assign<_ItR>(It); return *this; }
	inline _It& operator=(_ItW const& It) { this->template Assign<_ItW>(It); return *this; }
};

// Iterator String-Index with Pointers Read-Write
template <class _Char, BOOL _AllocId, class _Item, UINT _GroupSize>
class IndexIterator<StringItem<_Char, _AllocId>, _Item*, _GroupSize, false>: public IndexIteratorBase<StringItem<_Char, _AllocId>, _Item*, _GroupSize, false>
{
private:
	// Using
	using _It=IndexIterator<StringItem<_Char, _AllocId>, _Item*, _GroupSize, false>;
	using _ItR=IndexIterator<StringItem<_Char, _AllocId>, _Item*, _GroupSize, true>;
	using _ItW=IndexIterator<StringItem<_Char, _AllocId>, _Item*, _GroupSize, false>;
	using _IndexIteratorBase=IndexIteratorBase<StringItem<_Char, _AllocId>, _Item*, _GroupSize, false>;
	using _IndexItem=IndexItem<StringItem<_Char, _AllocId>, _Item*>;
	using _IndexGroup=IndexGroup<StringItem<_Char, _AllocId>, _Item*>;
	using _IndexItemGroup=IndexItemGroup<StringItem<_Char, _AllocId>, _Item*, _GroupSize>;
	using _IndexParentGroup=IndexParentGroup<StringItem<_Char, _AllocId>, _Item*, _GroupSize>;
	using _Cluster=::Clusters::Templates::Details::Cluster::Cluster<_IndexItem, _IndexGroup, _IndexItemGroup, _IndexParentGroup, _ItR, _ItW>;

public:
	// Con-/Destructors
	IndexIterator(_ItW const& It): _IndexIteratorBase(It) {}
	IndexIterator(_Cluster* Cluster, SIZE_T Position): _IndexIteratorBase(Cluster) { this->SetPosition(Position); }
	IndexIterator(_Cluster* Cluster, _Char const* Id, UINT Length, BOOL CaseSensitive): _IndexIteratorBase(Cluster) { _IndexIteratorBase::FindInternal(Id, Length, CaseSensitive); }

	// Access
	inline BOOL Find(_Char const* Id, UINT Length=0, BOOL CaseSensitive=false) { return _IndexIteratorBase::FindInternal(Id, Length, CaseSensitive); }
	inline _Char const* GetCurrentId()const { return this->pCurrent->GetId(); }
	inline _Item* GetCurrentItem()const { return this->pCurrent->GetItem(); }

	// Assignment
	inline _It& operator=(_ItW const& It) { this->template Assign<_ItW>(It); return *this; }

	// Modification
	inline VOID SetCurrentItem(_Item* Item) { return this->pCurrent->SetItem(Item); }
};

#ifdef __cplusplus_winrt
// Iterator String-Index with Handles Read-Only
template <class _Char, BOOL _AllocId, class _Item, UINT _GroupSize>
class IndexIterator<StringItem<_Char, _AllocId>, _Item^, _GroupSize, true>: public IndexIteratorBase<StringItem<_Char, _AllocId>, _Item^, _GroupSize, true>
{
private:
	// Using
	using _It=IndexIterator<StringItem<_Char, _AllocId>, _Item^, _GroupSize, true>;
	using _ItR=IndexIterator<StringItem<_Char, _AllocId>, _Item^, _GroupSize, true>;
	using _ItW=IndexIterator<StringItem<_Char, _AllocId>, _Item^, _GroupSize, false>;
	using _IndexIteratorBase=IndexIteratorBase<StringItem<_Char, _AllocId>, _Item^, _GroupSize, true>;
	using _IndexItem=IndexItem<StringItem<_Char, _AllocId>, _Item^>;
	using _IndexGroup=IndexGroup<StringItem<_Char, _AllocId>, _Item^>;
	using _IndexItemGroup=IndexItemGroup<StringItem<_Char, _AllocId>, _Item^, _GroupSize>;
	using _IndexParentGroup=IndexParentGroup<StringItem<_Char, _AllocId>, _Item^, _GroupSize>;
	using _Cluster=::Clusters::Templates::Details::Cluster::Cluster<_IndexItem, _IndexGroup, _IndexItemGroup, _IndexParentGroup, _ItR, _ItW>;

public:
	// Con-/Destructors
	IndexIterator(_ItR const& It): _IndexIteratorBase(It) {}
	IndexIterator(_ItW const& It): _IndexIteratorBase(It) {}
	IndexIterator(_Cluster const* Cluster, SIZE_T Position): _IndexIteratorBase(Cluster) { this->SetPosition(Position); }
	IndexIterator(_Cluster const* Cluster, _Char const* Id, UINT Length, BOOL CaseSensitive): _IndexIteratorBase(Cluster) { _IndexIteratorBase::FindInternal(Id, Length, CaseSensitive); }

	// Access
	inline BOOL Find(_Char const* Id, UINT Length=0, BOOL CaseSensitive=false) { return _IndexIteratorBase::FindInternal(Id, Length, CaseSensitive); }
	inline _Char const* GetCurrentId()const { return this->pCurrent->GetId(); }
	inline _Item^ GetCurrentItem()const { return this->pCurrent->GetItem(); }

	// Assignment
	inline _It& operator=(_ItR const& It) { this->template Assign<_ItR>(It); return *this; }
	inline _It& operator=(_ItW const& It) { this->template Assign<_ItW>(It); return *this; }
};
#endif

#ifdef __cplusplus_winrt
// Iterator String-Index with Handles Read-Write
template <class _Char, BOOL _AllocId, class _Item, UINT _GroupSize>
class IndexIterator<StringItem<_Char, _AllocId>, _Item^, _GroupSize, false>: public IndexIteratorBase<StringItem<_Char, _AllocId>, _Item^, _GroupSize, false>
{
private:
	// Using
	using _It=IndexIterator<StringItem<_Char, _AllocId>, _Item^, _GroupSize, false>;
	using _ItR=IndexIterator<StringItem<_Char, _AllocId>, _Item^, _GroupSize, true>;
	using _ItW=IndexIterator<StringItem<_Char, _AllocId>, _Item^, _GroupSize, false>;
	using _IndexIteratorBase=IndexIteratorBase<StringItem<_Char, _AllocId>, _Item^, _GroupSize, false>;
	using _IndexItem=IndexItem<StringItem<_Char, _AllocId>, _Item^>;
	using _IndexGroup=IndexGroup<StringItem<_Char, _AllocId>, _Item^>;
	using _IndexItemGroup=IndexItemGroup<StringItem<_Char, _AllocId>, _Item^, _GroupSize>;
	using _IndexParentGroup=IndexParentGroup<StringItem<_Char, _AllocId>, _Item^, _GroupSize>;
	using _Cluster=::Clusters::Templates::Details::Cluster::Cluster<_IndexItem, _IndexGroup, _IndexItemGroup, _IndexParentGroup, _ItR, _ItW>;

public:
	// Con-/Destructors
	IndexIterator(_ItW const& It): _IndexIteratorBase(It) {}
	IndexIterator(_Cluster* Cluster, SIZE_T Position): _IndexIteratorBase(Cluster) { this->SetPosition(Position); }
	IndexIterator(_Cluster* Cluster, _Char const* Id, UINT Length, BOOL CaseSensitive): _IndexIteratorBase(Cluster) { _IndexIteratorBase::FindInternal(Id, Length, CaseSensitive); }

	// Access
	inline BOOL Find(_Char const* Id, UINT Length=0, BOOL CaseSensitive=false) { return _IndexIteratorBase::FindInternal(Id, Length, CaseSensitive); }
	inline _Char const* GetCurrentId()const { return this->pCurrent->GetId(); }
	inline _Item^ GetCurrentItem()const { return this->pCurrent->GetItem(); }

	// Assignment
	inline _It& operator=(_ItW const& It) { this->template Assign<_ItW>(It); return *this; }

	// Modification
	inline VOID SetCurrentItem(_Item^ Item) { return this->pCurrent->SetItem(Item); }
};
#endif

// Iterator String-Index with String-Items Read-Only
template <class _CharId, BOOL _AllocId, class _CharItem, UINT _GroupSize>
class IndexIterator<StringItem<_CharId, _AllocId>, StringItem<_CharItem, true>, _GroupSize, true>: public IndexIteratorBase<StringItem<_CharId, _AllocId>, StringItem<_CharItem, true>, _GroupSize, true>
{
private:
	// Using
	using _It=IndexIterator<StringItem<_CharId, _AllocId>, StringItem<_CharItem, true>, _GroupSize, true>;
	using _ItR=IndexIterator<StringItem<_CharId, _AllocId>, StringItem<_CharItem, true>, _GroupSize, true>;
	using _ItW=IndexIterator<StringItem<_CharId, _AllocId>, StringItem<_CharItem, true>, _GroupSize, false>;
	using _IndexIteratorBase=IndexIteratorBase<StringItem<_CharId, _AllocId>, StringItem<_CharItem, true>, _GroupSize, true>;
	using _IndexItem=IndexItem<StringItem<_CharId, _AllocId>, StringItem<_CharItem, true>>;
	using _IndexGroup=IndexGroup<StringItem<_CharId, _AllocId>, StringItem<_CharItem, true>>;
	using _IndexItemGroup=IndexItemGroup<StringItem<_CharId, _AllocId>, StringItem<_CharItem, true>, _GroupSize>;
	using _IndexParentGroup=IndexParentGroup<StringItem<_CharId, _AllocId>, StringItem<_CharItem, true>, _GroupSize>;
	using _Cluster=::Clusters::Templates::Details::Cluster::Cluster<_IndexItem, _IndexGroup, _IndexItemGroup, _IndexParentGroup, _ItR, _ItW>;

public:
	// Con-/Destructors
	IndexIterator(_ItR const& It): _IndexIteratorBase(It) {}
	IndexIterator(_ItW const& It): _IndexIteratorBase(It) {}
	IndexIterator(_Cluster const* Cluster, SIZE_T Position): _IndexIteratorBase(Cluster) { this->SetPosition(Position); }
	IndexIterator(_Cluster const* Cluster, _CharId const* Id, UINT Length, BOOL CaseSensitive): _IndexIteratorBase(Cluster) { _IndexIteratorBase::FindInternal(Id, Length, CaseSensitive); }

	// Access
	inline BOOL Find(_CharId const* Id, UINT Length=0, BOOL CaseSensitive=false) { return _IndexIteratorBase::FindInternal(Id, Length, CaseSensitive); }
	inline _CharId const* GetCurrentId()const { return this->pCurrent->GetId(); }
	inline _CharItem const* GetCurrentItem()const { return this->pCurrent->GetItem(); }

	// Assignment
	inline _It& operator=(_ItR const& It) { this->template Assign<_ItR>(It); return *this; }
	inline _It& operator=(_ItW const& It) { this->template Assign<_ItW>(It); return *this; }
};

// Iterator String-Index with String-Items Read-Write
template <class _CharId, BOOL _AllocId, class _CharItem, UINT _GroupSize>
class IndexIterator<StringItem<_CharId, _AllocId>, StringItem<_CharItem, true>, _GroupSize, false>: public IndexIteratorBase<StringItem<_CharId, _AllocId>, StringItem<_CharItem, true>, _GroupSize, false>
{
private:
	// Using
	using _It=IndexIterator<StringItem<_CharId, _AllocId>, StringItem<_CharItem, true>, _GroupSize, false>;
	using _ItR=IndexIterator<StringItem<_CharId, _AllocId>, StringItem<_CharItem, true>, _GroupSize, true>;
	using _ItW=IndexIterator<StringItem<_CharId, _AllocId>, StringItem<_CharItem, true>, _GroupSize, false>;
	using _IndexIteratorBase=IndexIteratorBase<StringItem<_CharId, _AllocId>, StringItem<_CharItem, true>, _GroupSize, false>;
	using _IndexItem=IndexItem<StringItem<_CharId, _AllocId>, StringItem<_CharItem, true>>;
	using _IndexGroup=IndexGroup<StringItem<_CharId, _AllocId>, StringItem<_CharItem, true>>;
	using _IndexItemGroup=IndexItemGroup<StringItem<_CharId, _AllocId>, StringItem<_CharItem, true>, _GroupSize>;
	using _IndexParentGroup=IndexParentGroup<StringItem<_CharId, _AllocId>, StringItem<_CharItem, true>, _GroupSize>;
	using _Cluster=::Clusters::Templates::Details::Cluster::Cluster<_IndexItem, _IndexGroup, _IndexItemGroup, _IndexParentGroup, _ItR, _ItW>;

public:
	// Con-/Destructors
	IndexIterator(_ItW const& It): _IndexIteratorBase(It) {}
	IndexIterator(_Cluster* Cluster, SIZE_T Position): _IndexIteratorBase(Cluster) { this->SetPosition(Position); }
	IndexIterator(_Cluster* Cluster, _CharId const* Id, UINT Length, BOOL CaseSensitive): _IndexIteratorBase(Cluster) { _IndexIteratorBase::FindInternal(Id, Length, CaseSensitive); }

	// Access
	inline BOOL Find(_CharId const* Id, UINT Length=0, BOOL CaseSensitive=false) { return _IndexIteratorBase::FindInternal(Id, Length, CaseSensitive); }
	inline _CharId const* GetCurrentId()const { return this->pCurrent->GetId(); }
	inline _CharItem const* GetCurrentItem()const { return this->pCurrent->GetItem(); }

	// Assignment
	inline _It& operator=(_ItW const& It) { this->template Assign<_ItW>(It); return *this; }

	// Modification
	inline UINT SetCurrentItem(_CharItem const* Value, UINT Length=0) { return this->pCurrent->SetItem(Value, Length); }
};

// Iterator String-Index with Shared String-Items Read-Only
template <class _CharId, BOOL _AllocId, class _CharItem, UINT _GroupSize>
class IndexIterator<StringItem<_CharId, _AllocId>, StringItem<_CharItem, false>, _GroupSize, true>: public IndexIteratorBase<StringItem<_CharId, _AllocId>, StringItem<_CharItem, false>, _GroupSize, true>
{
private:
	// Using
	using _It=IndexIterator<StringItem<_CharId, _AllocId>, StringItem<_CharItem, false>, _GroupSize, true>;
	using _ItR=IndexIterator<StringItem<_CharId, _AllocId>, StringItem<_CharItem, false>, _GroupSize, true>;
	using _ItW=IndexIterator<StringItem<_CharId, _AllocId>, StringItem<_CharItem, false>, _GroupSize, false>;
	using _IndexIteratorBase=IndexIteratorBase<StringItem<_CharId, _AllocId>, StringItem<_CharItem, false>, _GroupSize, true>;
	using _IndexItem=IndexItem<StringItem<_CharId, _AllocId>, StringItem<_CharItem, false>>;
	using _IndexGroup=IndexGroup<StringItem<_CharId, _AllocId>, StringItem<_CharItem, false>>;
	using _IndexItemGroup=IndexItemGroup<StringItem<_CharId, _AllocId>, StringItem<_CharItem, false>, _GroupSize>;
	using _IndexParentGroup=IndexParentGroup<StringItem<_CharId, _AllocId>, StringItem<_CharItem, false>, _GroupSize>;
	using _Cluster=::Clusters::Templates::Details::Cluster::Cluster<_IndexItem, _IndexGroup, _IndexItemGroup, _IndexParentGroup, _ItR, _ItW>;

public:
	// Con-/Destructors
	IndexIterator(_ItR const& It): _IndexIteratorBase(It) {}
	IndexIterator(_ItW const& It): _IndexIteratorBase(It) {}
	IndexIterator(_Cluster const* Cluster, SIZE_T Position): _IndexIteratorBase(Cluster) { this->SetPosition(Position); }
	IndexIterator(_Cluster const* Cluster, _CharId const* Id, UINT Length, BOOL CaseSensitive): _IndexIteratorBase(Cluster) { _IndexIteratorBase::FindInternal(Id, Length, CaseSensitive); }

	// Access
	inline BOOL Find(_CharId const* Id, UINT Length=0, BOOL CaseSensitive=false) { return _IndexIteratorBase::FindInternal(Id, Length, CaseSensitive); }
	inline _CharId const* GetCurrentId()const { return this->pCurrent->GetId(); }
	inline _CharItem const* GetCurrentItem()const { return this->pCurrent->GetItem(); }

	// Assignment
	inline _It& operator=(_ItR const& It) { this->template Assign<_ItR>(It); return *this; }
	inline _It& operator=(_ItW const& It) { this->template Assign<_ItW>(It); return *this; }
};

// Iterator String-Index with Shared String-Items Read-Write
template <class _CharId, BOOL _AllocId, class _CharItem, UINT _GroupSize>
class IndexIterator<StringItem<_CharId, _AllocId>, StringItem<_CharItem, false>, _GroupSize, false>: public IndexIteratorBase<StringItem<_CharId, _AllocId>, StringItem<_CharItem, false>, _GroupSize, false>
{
private:
	// Using
	using _It=IndexIterator<StringItem<_CharId, _AllocId>, StringItem<_CharItem, false>, _GroupSize, false>;
	using _ItR=IndexIterator<StringItem<_CharId, _AllocId>, StringItem<_CharItem, false>, _GroupSize, true>;
	using _ItW=IndexIterator<StringItem<_CharId, _AllocId>, StringItem<_CharItem, false>, _GroupSize, false>;
	using _IndexIteratorBase=IndexIteratorBase<StringItem<_CharId, _AllocId>, StringItem<_CharItem, false>, _GroupSize, false>;
	using _IndexItem=IndexItem<StringItem<_CharId, _AllocId>, StringItem<_CharItem, false>>;
	using _IndexGroup=IndexGroup<StringItem<_CharId, _AllocId>, StringItem<_CharItem, false>>;
	using _IndexItemGroup=IndexItemGroup<StringItem<_CharId, _AllocId>, StringItem<_CharItem, false>, _GroupSize>;
	using _IndexParentGroup=IndexParentGroup<StringItem<_CharId, _AllocId>, StringItem<_CharItem, false>, _GroupSize>;
	using _Cluster=::Clusters::Templates::Details::Cluster::Cluster<_IndexItem, _IndexGroup, _IndexItemGroup, _IndexParentGroup, _ItR, _ItW>;

public:
	// Con-/Destructors
	IndexIterator(_ItW const& It): _IndexIteratorBase(It) {}
	IndexIterator(_Cluster* Cluster, SIZE_T Position): _IndexIteratorBase(Cluster) { this->SetPosition(Position); }
	IndexIterator(_Cluster* Cluster, _CharId const* Id, UINT Length, BOOL CaseSensitive): _IndexIteratorBase(Cluster) { _IndexIteratorBase::FindInternal(Id, Length, CaseSensitive); }

	// Access
	inline BOOL Find(_CharId const* Id, UINT Length=0, BOOL CaseSensitive=false) { return _IndexIteratorBase::FindInternal(Id, Length, CaseSensitive); }
	inline _CharId const* GetCurrentId()const { return this->pCurrent->GetId(); }
	inline _CharItem const* GetCurrentItem()const { return this->pCurrent->GetItem(); }

	// Assignment
	inline _It& operator=(_ItW const& It) { this->template Assign<_ItW>(It); return *this; }

	// Modification
	inline VOID SetCurrentItem(_CharItem const* Value) { this->pCurrent->SetItem(Value); }
};

}}}}
