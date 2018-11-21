//=================
// IndexIterator.h
//=================

#pragma once


//=======
// Using
//=======

#include "..\Cluster\Cluster.h"
#include "..\Cluster\Iterator.h"
#include "..\StringClass.h"
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

template <class _Id, class _Item, unsigned int _GroupSize, bool _ReadOnly>
class IndexIteratorBase: public ::Clusters::Templates::Details::Cluster::IteratorShared<IndexItem<_Id, _Item>, IndexGroup<_Id, _Item>, IndexItemGroup<_Id, _Item, _GroupSize>, IndexParentGroup<_Id, _Item, _GroupSize>, _ReadOnly>
{
protected:
	// Using
	using _IndexItem=IndexItem<_Id, _Item>;
	using _Group=IndexGroup<_Id, _Item>;
	using _GroupPtr=Conditional<_ReadOnly, _Group const*, _Group*>;
	using _ItemGroup=IndexItemGroup<_Id, _Item, _GroupSize>;
	using _ItemGroupPtr=Conditional<_ReadOnly, _ItemGroup const*, _ItemGroup*>;
	using _ParentGroup=IndexParentGroup<_Id, _Item, _GroupSize>;
	using _ParentGroupPtr=Conditional<_ReadOnly, _ParentGroup const*, _ParentGroup*>;
	using _Base=::Clusters::Templates::Details::Cluster::IteratorShared<_IndexItem, _Group, _ItemGroup, _ParentGroup, _ReadOnly>;

	// Con-/Destructors
	using _Base::_Base;
	
	// Common
	template <class... _Params> bool FindInternal(_Params... Id)
		{
		pCurrent=nullptr;
		bool bfound=true;
		_GroupPtr pgroup=pCluster->pRoot;
		unsigned int ulevelcount=pgroup->GetLevel()+1;
		if(pIts)
			delete pIts;
		pIts=new ITERATOR[ulevelcount];
		for(unsigned int u=0; u<ulevelcount-1; u++)
			{
			_ParentGroupPtr pparent=(_ParentGroupPtr)pgroup;
			int ipos=pparent->Find(Id...);
			if(ipos<0)
				{
				bfound=false;
				ipos++;
				ipos*=-1;
				}
			pIts[u].Group=pgroup;
			pIts[u].Position=ipos;
			pgroup=pparent->GetChild(ipos);
			}
		_ItemGroupPtr pitemgroup=(_ItemGroupPtr)pgroup;
		int ipos=pitemgroup->Find(Id...);
		if(ipos<0)
			{
			bfound=false;
			ipos++;
			ipos*=-1;
			}
		pIts[ulevelcount-1].Group=pgroup;
		pIts[ulevelcount-1].Position=ipos;
		pCurrent=pitemgroup->GetAt(ipos);
		return bfound;
		}
};


//================
// Iterator Index
//================

// Iterator Index Read-Only
template <class _Id, class _Item, unsigned int _GroupSize, bool _ReadOnly>
class IndexIterator: public IndexIteratorBase<_Id, _Item, _GroupSize, true>
{
private:
	// Using
	using _ItR=IndexIterator<_Id, _Item, _GroupSize, true>;
	using _ItW=IndexIterator<_Id, _Item, _GroupSize, false>;
	using _Cluster=::Clusters::Templates::Details::Cluster::Cluster<_IndexItem, _Group, _ItemGroup, _ParentGroup, _ItR, _ItW>;

public:
	// Con-/Destructors
	IndexIterator(_ItR const& It): IndexIteratorBase(It) {}
	IndexIterator(_ItW const& It): IndexIteratorBase(It) {}
	IndexIterator(_Cluster const* Cluster, size_t Position): IndexIteratorBase(Cluster) { SetPosition(Position); }
	IndexIterator(_Cluster const* Cluster, size_t, _Id const& Id): IndexIteratorBase(Cluster) { FindInternal<_Id const&>(Id); }

	// Access
	inline bool Find(_Id const& Id) { return FindInternal<_Id const&>(Id); }
	inline _Id const& GetCurrentId()const { return pCurrent->GetId(); }
	inline _Item const& GetCurrentItem()const { return pCurrent->GetItem(); }
};

// Iterator Index Read-Write
template <class _Id, class _Item, unsigned int _GroupSize>
class IndexIterator<_Id, _Item, _GroupSize, false>: public IndexIteratorBase<_Id, _Item, _GroupSize, false>
{
private:
	// Using
	using _ItR=IndexIterator<_Id, _Item, _GroupSize, true>;
	using _ItW=IndexIterator<_Id, _Item, _GroupSize, false>;
	using _Cluster=::Clusters::Templates::Details::Cluster::Cluster<_IndexItem, _Group, _ItemGroup, _ParentGroup, _ItR, _ItW>;

public:
	// Con-/Destructors
	IndexIterator(_ItW const& It): IndexIteratorBase(It) {}
	IndexIterator(_Cluster* Cluster, size_t Position): IndexIteratorBase(Cluster) { SetPosition(Position); }
	IndexIterator(_Cluster* Cluster, size_t, _Id const& Id): IndexIteratorBase(Cluster) { FindInternal<_Id const&>(Id); }

	// Access
	inline bool Find(_Id const& Id) { return FindInternal<_Id const&>(Id); }
	inline _Id const& GetCurrentId()const { return pCurrent->GetId(); }
	inline _Item& GetCurrentItem()const { return pCurrent->GetItem(); }

	// Modification
	inline void SetCurrentItem(_Item const& Item) { pCurrent->SetItem(Item); }
};

// Iterator Index without Items Read-Only
template <class _Id, unsigned int _GroupSize>
class IndexIterator<_Id, void, _GroupSize, true>: public IndexIteratorBase<_Id, void, _GroupSize, true>
{
private:
	// Using
	using _ItR=IndexIterator<_Id, void, _GroupSize, true>;
	using _ItW=IndexIterator<_Id, void, _GroupSize, false>;
	using _Cluster=::Clusters::Templates::Details::Cluster::Cluster<_IndexItem, _Group, _ItemGroup, _ParentGroup, _ItR, _ItW>;

public:
	// Con-/Destructors
	IndexIterator(_ItR const& It): IndexIteratorBase(It) {}
	IndexIterator(_ItW const& It): IndexIteratorBase(It) {}
	IndexIterator(_Cluster const* Cluster, size_t Position): IndexIteratorBase(Cluster) { SetPosition(Position); }
	IndexIterator(_Cluster const* Cluster, size_t, _Id const& Id): IndexIteratorBase(Cluster) { FindInternal<_Id const&>(Id); }

	// Access
	inline bool Find(_Id const& Id) { return FindInternal<_Id const&>(Id); }
	inline _Id const& GetCurrent()const { return pCurrent->GetId(); }
};

// Iterator Index without Items Read-Write
template <class _Id, unsigned int _GroupSize>
class IndexIterator<_Id, void, _GroupSize, false>: public IndexIteratorBase<_Id, void, _GroupSize, false>
{
private:
	// Using
	using _ItR=IndexIterator<_Id, void, _GroupSize, true>;
	using _ItW=IndexIterator<_Id, void, _GroupSize, false>;
	using _Cluster=::Clusters::Templates::Details::Cluster::Cluster<_IndexItem, _Group, _ItemGroup, _ParentGroup, _ItR, _ItW>;

public:
	// Con-/Destructors
	IndexIterator(_ItW const& It): IndexIteratorBase(It) {}
	IndexIterator(_Cluster* Cluster, size_t Position): IndexIteratorBase(Cluster) { SetPosition(Position); }
	IndexIterator(_Cluster* Cluster, size_t, _Id const& Id): IndexIteratorBase(Cluster) { FindInternal<_Id const&>(Id); }

	// Access
	inline bool Find(_Id const& Id) { return FindInternal<_Id const&>(Id); }
	inline _Id const& GetCurrent()const { return pCurrent->GetId(); }
};

// Iterator Index with Pointers Read-Only
template <class _Id, class _Item, unsigned int _GroupSize>
class IndexIterator<_Id, _Item*, _GroupSize, true>: public IndexIteratorBase<_Id, _Item*, _GroupSize, true>
{
private:
	// Using
	using _ItR=IndexIterator<_Id, _Item*, _GroupSize, true>;
	using _ItW=IndexIterator<_Id, _Item*, _GroupSize, false>;
	using _Cluster=::Clusters::Templates::Details::Cluster::Cluster<_IndexItem, _Group, _ItemGroup, _ParentGroup, _ItR, _ItW>;

public:
	// Con-/Destructors
	IndexIterator(_ItR const& It): IndexIteratorBase(It) {}
	IndexIterator(_ItW const& It): IndexIteratorBase(It) {}
	IndexIterator(_Cluster const* Cluster, size_t Position): IndexIteratorBase(Cluster) { SetPosition(Position); }
	IndexIterator(_Cluster const* Cluster, size_t, _Id const& Id): IndexIteratorBase(Cluster) { FindInternal<_Id const&>(Id); }

	// Access
	inline bool Find(_Id const& Id) { return FindInternal<_Id const&>(Id); }
	inline _Id const& GetCurrentId()const { return pCurrent->GetId(); }
	inline _Item* GetCurrentItem()const { return pCurrent->GetItem(); }
};

// Iterator Index with Pointers Read-Write
template <class _Id, class _Item, unsigned int _GroupSize>
class IndexIterator<_Id, _Item*, _GroupSize, false>: public IndexIteratorBase<_Id, _Item*, _GroupSize, false>
{
private:
	// Using
	using _ItR=IndexIterator<_Id, _Item*, _GroupSize, true>;
	using _ItW=IndexIterator<_Id, _Item*, _GroupSize, false>;
	using _Cluster=::Clusters::Templates::Details::Cluster::Cluster<_IndexItem, _Group, _ItemGroup, _ParentGroup, _ItR, _ItW>;

public:
	// Con-/Destructors
	IndexIterator(_ItW const& It): IndexIteratorBase(It) {}
	IndexIterator(_Cluster* Cluster, size_t Position): IndexIteratorBase(Cluster) { SetPosition(Position); }
	IndexIterator(_Cluster* Cluster, size_t, _Id const& Id): IndexIteratorBase(Cluster) { FindInternal<_Id const&>(Id); }

	// Access
	inline bool Find(_Id const& Id) { return FindInternal<_Id const&>(Id); }
	inline _Id const& GetCurrentId()const { return pCurrent->GetId(); }
	inline _Item* GetCurrentItem()const { return pCurrent->GetItem(); }

	// Modification
	inline void SetCurrentItem(_Item* Item) { pCurrent->SetItem(Item); }
};

#ifdef __cplusplus_winrt
// Iterator Index with Handles Read-Only
template <class _Id, class _Item, unsigned int _GroupSize>
class IndexIterator<_Id, _Item^, _GroupSize, true>: public IndexIteratorBase<_Id, _Item^, _GroupSize, true>
{
private:
	// Using
	using _ItR=IndexIterator<_Id, _Item^, _GroupSize, true>;
	using _ItW=IndexIterator<_Id, _Item^, _GroupSize, false>;
	using _Cluster=::Clusters::Templates::Details::Cluster::Cluster<_IndexItem, _Group, _ItemGroup, _ParentGroup, _ItR, _ItW>;

public:
	// Con-/Destructors
	IndexIterator(_ItR const& It): IndexIteratorBase(It) {}
	IndexIterator(_ItW const& It): IndexIteratorBase(It) {}
	IndexIterator(_Cluster const* Cluster, size_t Position): IndexIteratorBase(Cluster) { SetPosition(Position); }
	IndexIterator(_Cluster const* Cluster, size_t, _Id const& Id): IndexIteratorBase(Cluster) { FindInternal<_Id const&>(Id); }

	// Access
	inline bool Find(_Id const& Id) { return FindInternal<_Id const&>(Id); }
	inline _Id const& GetCurrentId()const { return pCurrent->GetId(); }
	inline _Item^ GetCurrentItem()const { return pCurrent->GetItem(); }
};
#endif

#ifdef __cplusplus_winrt
// Iterator Index with Handles Read-Write
template <class _Id, class _Item, unsigned int _GroupSize>
class IndexIterator<_Id, _Item^, _GroupSize, false>: public IndexIteratorBase<_Id, _Item^, _GroupSize, false>
{
private:
	// Using
	using _ItR=IndexIterator<_Id, _Item^, _GroupSize, true>;
	using _ItW=IndexIterator<_Id, _Item^, _GroupSize, false>;
	using _Cluster=::Clusters::Templates::Details::Cluster::Cluster<_IndexItem, _Group, _ItemGroup, _ParentGroup, _ItR, _ItW>;

public:
	// Con-/Destructors
	IndexIterator(_ItW const& It): IndexIteratorBase(It) {}
	IndexIterator(_Cluster* Cluster, size_t Position): IndexIteratorBase(Cluster) { SetPosition(Position); }
	IndexIterator(_Cluster* Cluster, size_t, _Id const& Id): IndexIteratorBase(Cluster) { FindInternal<_Id const&>(Id); }

	// Access
	inline bool Find(_Id const& Id) { return FindInternal<_Id const&>(Id); }
	inline _Id const& GetCurrentId()const { return pCurrent->GetId(); }
	inline _Item^ GetCurrentItem()const { return pCurrent->GetItem(); }

	// Modification
	inline void SetCurrentItem(_Item^ Item) { pCurrent->SetItem(Item); }
};
#endif

// Iterator Index with Strings Read-Only
template <class _Id, class _Char, bool _AllocItem, unsigned int _GroupSize>
class IndexIterator<_Id, String<_Char, _AllocItem>, _GroupSize, true>: public IndexIteratorBase<_Id, String<_Char, _AllocItem>, _GroupSize, true>
{
private:
	// Using
	using _ItR=IndexIterator<_Id, String<_Char, _AllocItem>, _GroupSize, true>;
	using _ItW=IndexIterator<_Id, String<_Char, _AllocItem>, _GroupSize, false>;
	using _Cluster=::Clusters::Templates::Details::Cluster::Cluster<_IndexItem, _Group, _ItemGroup, _ParentGroup, _ItR, _ItW>;

public:
	// Con-/Destructors
	IndexIterator(_ItR const& It): IndexIteratorBase(It) {}
	IndexIterator(_ItW const& It): IndexIteratorBase(It) {}
	IndexIterator(_Cluster const* Cluster, size_t Position): IndexIteratorBase(Cluster) { SetPosition(Position); }
	IndexIterator(_Cluster const* Cluster, size_t, _Id const& Id): IndexIteratorBase(Cluster) { FindInternal<_Id const&>(Id); }

	// Access
	inline bool Find(_Id const& Id) { return FindInternal<_Id const&>(Id); }
	inline _Id const& GetCurrentId()const { return pCurrent->GetId(); }
	inline _Char const* GetCurrentItem()const { return pCurrent->GetItem(); }
};

// Iterator Index with Strings Read-Write
template <class _Id, class _Char, unsigned int _GroupSize>
class IndexIterator<_Id, String<_Char, true>, _GroupSize, false>: public IndexIteratorBase<_Id, String<_Char, true>, _GroupSize, false>
{
private:
	// Using
	using _ItR=IndexIterator<_Id, String<_Char, true>, _GroupSize, true>;
	using _ItW=IndexIterator<_Id, String<_Char, true>, _GroupSize, false>;
	using _Cluster=::Clusters::Templates::Details::Cluster::Cluster<_IndexItem, _Group, _ItemGroup, _ParentGroup, _ItR, _ItW>;

public:
	// Con-/Destructors
	IndexIterator(_ItW const& It): IndexIteratorBase(It) {}
	IndexIterator(_Cluster* Cluster, size_t Position): IndexIteratorBase(Cluster) { SetPosition(Position); }
	IndexIterator(_Cluster* Cluster, size_t, _Id const& Id): IndexIteratorBase(Cluster) { FindInternal<_Id const&>(Id); }

	// Access
	inline bool Find(_Id const& Id) { return FindInternal<_Id const&>(Id); }
	inline _Id const& GetCurrentId()const { return pCurrent->GetId(); }
	inline _Char const* GetCurrentItem()const { return pCurrent->GetItem(); }

	// Modification
	inline unsigned int SetCurrentItem(_Char const* Item, unsigned int Length=0) { return pCurrent->SetItem(Item, Length); }
};

// Iterator Index with Shared Strings Read-Only
template <class _Id, class _Char, unsigned int _GroupSize>
class IndexIterator<_Id, String<_Char, false>, _GroupSize, true>: public IndexIteratorBase<_Id, String<_Char, false>, _GroupSize, true>
{
private:
	// Using
	using _ItR=IndexIterator<_Id, String<_Char, false>, _GroupSize, true>;
	using _ItW=IndexIterator<_Id, String<_Char, false>, _GroupSize, false>;
	using _Cluster=::Clusters::Templates::Details::Cluster::Cluster<_IndexItem, _Group, _ItemGroup, _ParentGroup, _ItR, _ItW>;

public:
	// Con-/Destructors
	IndexIterator(_ItR const& It): IndexIteratorBase(It) {}
	IndexIterator(_ItW const& It): IndexIteratorBase(It) {}
	IndexIterator(_Cluster const* Cluster, size_t Position): IndexIteratorBase(Cluster) { SetPosition(Position); }
	IndexIterator(_Cluster const* Cluster, size_t, _Id const& Id): IndexIteratorBase(Cluster) { FindInternal<_Id const&>(Id); }

	// Access
	inline bool Find(_Id const& Id) { return FindInternal<_Id const&>(Id); }
	inline _Id const& GetCurrentId()const { return pCurrent->GetId(); }
	inline _Char const* GetCurrentItem()const { return pCurrent->GetItem(); }
};

// Iterator Index with Shared Strings Read-Write
template <class _Id, class _Char, unsigned int _GroupSize>
class IndexIterator<_Id, String<_Char, false>, _GroupSize, false>: public IndexIteratorBase<_Id, String<_Char, false>, _GroupSize, false>
{
private:
	// Using
	using _ItR=IndexIterator<_Id, String<_Char, false>, _GroupSize, true>;
	using _ItW=IndexIterator<_Id, String<_Char, false>, _GroupSize, false>;
	using _Cluster=::Clusters::Templates::Details::Cluster::Cluster<_IndexItem, _Group, _ItemGroup, _ParentGroup, _ItR, _ItW>;

public:
	// Con-/Destructors
	IndexIterator(_ItW const& It): IndexIteratorBase(It) {}
	IndexIterator(_Cluster* Cluster, size_t Position): IndexIteratorBase(Cluster) { SetPosition(Position); }
	IndexIterator(_Cluster* Cluster, size_t Position, _Id const& Id): IndexIteratorBase(Cluster) { FindInternal<_Id const&>(Id); }

	// Access
	inline bool Find(_Id const& Id) { return FindInternal<_Id const&>(Id); }
	inline _Id const& GetCurrentId()const { return pCurrent->GetId(); }
	inline _Char const* GetCurrentItem()const { return pCurrent->GetItem(); }

	// Modification
	inline void SetCurrentItem(_Char const* Item) { pCurrent->SetItem(Item); }
};


//========================
// Iterator Pointer-Index
//========================

// Iterator Pointer-Index Read-Only
template <class _Id, class _Item, unsigned int _GroupSize>
class IndexIterator<_Id*, _Item, _GroupSize, true>: public IndexIteratorBase<_Id*, _Item, _GroupSize, true>
{
private:
	// Using
	using _ItR=IndexIterator<_Id*, _Item, _GroupSize, true>;
	using _ItW=IndexIterator<_Id*, _Item, _GroupSize, false>;
	using _Cluster=::Clusters::Templates::Details::Cluster::Cluster<_IndexItem, _Group, _ItemGroup, _ParentGroup, _ItR, _ItW>;

public:
	// Con-/Destructors
	IndexIterator(_ItR const& It): IndexIteratorBase(It) {}
	IndexIterator(_ItW const& It): IndexIteratorBase(It) {}
	IndexIterator(_Cluster const* Cluster, size_t Position): IndexIteratorBase(Cluster) { SetPosition(Position); }
	IndexIterator(_Cluster const* Cluster, size_t, _Id* Id): IndexIteratorBase(Cluster) { FindInternal(Id); }

	// Access
	inline bool Find(_Id* Id) { return FindInternal(Id); }
	inline _Id* GetCurrentId()const { return pCurrent->GetId(); }
	inline _Item const& GetCurrentItem()const { return pCurrent->GetItem(); }
};

// Iterator Pointer-Index Read-Write
template <class _Id, class _Item, unsigned int _GroupSize>
class IndexIterator<_Id*, _Item, _GroupSize, false>: public IndexIteratorBase<_Id*, _Item, _GroupSize, false>
{
private:
	// Using
	using _ItR=IndexIterator<_Id*, _Item, _GroupSize, true>;
	using _ItW=IndexIterator<_Id*, _Item, _GroupSize, false>;
	using _Cluster=::Clusters::Templates::Details::Cluster::Cluster<_IndexItem, _Group, _ItemGroup, _ParentGroup, _ItR, _ItW>;

public:
	// Con-/Destructors
	IndexIterator(_ItW const& It): IndexIteratorBase(It) {}
	IndexIterator(_Cluster* Cluster, size_t Position): IndexIteratorBase(Cluster) { SetPosition(Position); }
	IndexIterator(_Cluster* Cluster, size_t, _Id* Id): IndexIteratorBase(Cluster) { FindInternal(Id); }

	// Access
	inline bool Find(_Id* Id) { return FindInternal(Id); }
	inline _Id* GetCurrentId()const { return pCurrent->GetId(); }
	inline _Item& GetCurrentItem()const { return pCurrent->GetItem(); }

	// Modification
	inline void SetCurrentItem(_Item const& Item) { pCurrent->SetItem(Item); }
};

// Iterator Pointer-Index without Items Read-Only
template <class _Id, unsigned int _GroupSize>
class IndexIterator<_Id*, void, _GroupSize, true>: public IndexIteratorBase<_Id*, void, _GroupSize, true>
{
private:
	// Using
	using _ItR=IndexIterator<_Id*, void, _GroupSize, true>;
	using _ItW=IndexIterator<_Id*, void, _GroupSize, false>;
	using _Cluster=::Clusters::Templates::Details::Cluster::Cluster<_IndexItem, _Group, _ItemGroup, _ParentGroup, _ItR, _ItW>;

public:
	// Con-/Destructors
	IndexIterator(_ItR const& It): IndexIteratorBase(It) {}
	IndexIterator(_ItW const& It): IndexIteratorBase(It) {}
	IndexIterator(_Cluster const* Cluster, size_t Position): IndexIteratorBase(Cluster) { SetPosition(Position); }
	IndexIterator(_Cluster const* Cluster, size_t, _Id* Id): IndexIteratorBase(Cluster) { FindInternal(Id); }

	// Access
	inline bool Find(_Id* Id) { return FindInternal(Id); }
	inline _Id* GetCurrent()const { return pCurrent->GetId(); }
};

// Iterator Pointer-Index without Items Read-Write
template <class _Id, unsigned int _GroupSize>
class IndexIterator<_Id*, void, _GroupSize, false>: public IndexIteratorBase<_Id*, void, _GroupSize, false>
{
private:
	// Using
	using _ItR=IndexIterator<_Id*, void, _GroupSize, true>;
	using _ItW=IndexIterator<_Id*, void, _GroupSize, false>;
	using _Cluster=::Clusters::Templates::Details::Cluster::Cluster<_IndexItem, _Group, _ItemGroup, _ParentGroup, _ItR, _ItW>;

public:
	// Con-/Destructors
	IndexIterator(_ItW const& It): IndexIteratorBase(It) {}
	IndexIterator(_Cluster* Cluster, size_t Position): IndexIteratorBase(Cluster) { SetPosition(Position); }
	IndexIterator(_Cluster* Cluster, size_t, _Id* Id): IndexIteratorBase(Cluster) { FindInternal(Id); }

	// Access
	inline bool Find(_Id* Id) { return FindInternal(Id); }
	inline _Id* GetCurrent()const { return pCurrent->GetId(); }
};

// Iterator Pointer-Index with Pointers Read-Only
template <class _Id, class _Item, unsigned int _GroupSize>
class IndexIterator<_Id*, _Item*, _GroupSize, true>: public IndexIteratorBase<_Id*, _Item*, _GroupSize, true>
{
private:
	// Using
	using _ItR=IndexIterator<_Id*, _Item*, _GroupSize, true>;
	using _ItW=IndexIterator<_Id*, _Item*, _GroupSize, false>;
	using _Cluster=::Clusters::Templates::Details::Cluster::Cluster<_IndexItem, _Group, _ItemGroup, _ParentGroup, _ItR, _ItW>;

public:
	// Con-/Destructors
	IndexIterator(_ItR const& It): IndexIteratorBase(It) {}
	IndexIterator(_ItW const& It): IndexIteratorBase(It) {}
	IndexIterator(_Cluster const* Cluster, size_t Position): IndexIteratorBase(Cluster) { SetPosition(Position); }
	IndexIterator(_Cluster const* Cluster, size_t, _Id* Id): IndexIteratorBase(Cluster) { FindInternal(Id); }

	// Access
	inline bool Find(_Id* Id) { return FindInternal(Id); }
	inline _Id* GetCurrentId()const { return pCurrent->GetId(); }
	inline _Item* GetCurrentItem()const { return pCurrent->GetItem(); }
};

// Iterator Pointer-Index with Pointers Read-Write
template <class _Id, class _Item, unsigned int _GroupSize>
class IndexIterator<_Id*, _Item*, _GroupSize, false>: public IndexIteratorBase<_Id*, _Item*, _GroupSize, false>
{
private:
	// Using
	using _ItR=IndexIterator<_Id*, _Item*, _GroupSize, true>;
	using _ItW=IndexIterator<_Id*, _Item*, _GroupSize, false>;
	using _Cluster=::Clusters::Templates::Details::Cluster::Cluster<_IndexItem, _Group, _ItemGroup, _ParentGroup, _ItR, _ItW>;

public:
	// Con-/Destructors
	IndexIterator(_ItW const& It): IndexIteratorBase(It) {}
	IndexIterator(_Cluster* Cluster, size_t Position): IndexIteratorBase(Cluster) { SetPosition(Position); }
	IndexIterator(_Cluster* Cluster, size_t, _Id* Id): IndexIteratorBase(Cluster) { FindInternal(Id); }

	// Access
	inline bool Find(_Id* Id) { return FindInternal(Id); }
	inline _Id* GetCurrentId()const { return pCurrent->GetId(); }
	inline _Item* GetCurrentItem()const { return pCurrent->GetItem(); }

	// Modification
	inline void SetCurrentItem(_Item* Item) { pCurrent->SetItem(Item); }
};

#ifdef __cplusplus_winrt
// Iterator Pointer-Index with Handles Read-Only
template <class _Id, class _Item, unsigned int _GroupSize>
class IndexIterator<_Id*, _Item^, _GroupSize, true>: public IndexIteratorBase<_Id*, _Item^, _GroupSize, true>
{
private:
	// Using
	using _ItR=IndexIterator<_Id*, _Item^, _GroupSize, true>;
	using _ItW=IndexIterator<_Id*, _Item^, _GroupSize, false>;
	using _Cluster=::Clusters::Templates::Details::Cluster::Cluster<_IndexItem, _Group, _ItemGroup, _ParentGroup, _ItR, _ItW>;

public:
	// Con-/Destructors
	IndexIterator(_ItR const& It): IndexIteratorBase(It) {}
	IndexIterator(_ItW const& It): IndexIteratorBase(It) {}
	IndexIterator(_Cluster const* Cluster, size_t Position): IndexIteratorBase(Cluster) { SetPosition(Position); }
	IndexIterator(_Cluster const* Cluster, size_t, _Id* Id): IndexIteratorBase(Cluster) { FindInternal(Id); }

	// Access
	inline bool Find(_Id* Id) { return FindInternal(Id); }
	inline _Id* GetCurrentId()const { return pCurrent->GetId(); }
	inline _Item^ GetCurrentItem()const { return pCurrent->GetItem(); }
};
#endif

#ifdef __cplusplus_winrt
// Iterator Pointer-Index with Handles Read-Write
template <class _Id, class _Item, unsigned int _GroupSize>
class IndexIterator<_Id*, _Item^, _GroupSize, false>: public IndexIteratorBase<_Id*, _Item^, _GroupSize, false>
{
private:
	// Using
	using _ItR=IndexIterator<_Id*, _Item^, _GroupSize, true>;
	using _ItW=IndexIterator<_Id*, _Item^, _GroupSize, false>;
	using _Cluster=::Clusters::Templates::Details::Cluster::Cluster<_IndexItem, _Group, _ItemGroup, _ParentGroup, _ItR, _ItW>;

public:
	// Con-/Destructors
	IndexIterator(_ItW const& It): IndexIteratorBase(It) {}
	IndexIterator(_Cluster* Cluster, size_t Position): IndexIteratorBase(Cluster) { SetPosition(Position); }
	IndexIterator(_Cluster* Cluster, size_t, _Id* Id): IndexIteratorBase(Cluster) { FindInternal(Id); }

	// Access
	inline bool Find(_Id* Id) { return FindInternal(Id); }
	inline _Id* GetCurrentId()const { return pCurrent->GetId(); }
	inline _Item^ GetCurrentItem()const { return pCurrent->GetItem(); }

	// Modification
	inline void SetCurrentItem(_Item^ Item) { pCurrent->SetItem(Item); }
};
#endif

// Iterator Pointer-Index with Strings Read-Only
template <class _Id, class _Char, bool _AllocItem, unsigned int _GroupSize>
class IndexIterator<_Id*, String<_Char, _AllocItem>, _GroupSize, true>: public IndexIteratorBase<_Id*, String<_Char, _AllocItem>, _GroupSize, true>
{
private:
	// Using
	using _ItR=IndexIterator<_Id*, String<_Char, _AllocItem>, _GroupSize, true>;
	using _ItW=IndexIterator<_Id*, String<_Char, _AllocItem>, _GroupSize, false>;
	using _Cluster=::Clusters::Templates::Details::Cluster::Cluster<_IndexItem, _Group, _ItemGroup, _ParentGroup, _ItR, _ItW>;

public:
	// Con-/Destructors
	IndexIterator(_ItR const& It): IndexIteratorBase(It) {}
	IndexIterator(_ItW const& It): IndexIteratorBase(It) {}
	IndexIterator(_Cluster const* Cluster, size_t Position): IndexIteratorBase(Cluster) { SetPosition(Position); }
	IndexIterator(_Cluster const* Cluster, size_t, _Id* Id): IndexIteratorBase(Cluster) { FindInternal(Id); }

	// Access
	inline bool Find(_Id* Id) { return FindInternal(Id); }
	inline _Id* GetCurrentId()const { return pCurrent->GetId(); }
	inline _Char const* GetCurrentItem()const { return pCurrent->GetItem(); }
};

// Iterator Pointer-Index with Strings Read-Write
template <class _Id, class _Char, unsigned int _GroupSize>
class IndexIterator<_Id*, String<_Char, true>, _GroupSize, false>: public IndexIteratorBase<_Id*, String<_Char, true>, _GroupSize, false>
{
private:
	// Using
	using _ItR=IndexIterator<_Id*, String<_Char, true>, _GroupSize, true>;
	using _ItW=IndexIterator<_Id*, String<_Char, true>, _GroupSize, false>;
	using _Cluster=::Clusters::Templates::Details::Cluster::Cluster<_IndexItem, _Group, _ItemGroup, _ParentGroup, _ItR, _ItW>;

public:
	// Con-/Destructors
	IndexIterator(_ItW const& It): IndexIteratorBase(It) {}
	IndexIterator(_Cluster* Cluster, size_t Position): IndexIteratorBase(Cluster) { SetPosition(Position); }
	IndexIterator(_Cluster* Cluster, size_t, _Id* Id): IndexIteratorBase(Cluster) { FindInternal(Id); }

	// Access
	inline bool Find(_Id* Id) { return FindInternal(Id); }
	inline _Id* GetCurrentId()const { return pCurrent->GetId(); }
	inline _Char const* GetCurrentItem()const { return pCurrent->GetItem(); }

	// Modification
	inline unsigned int SetCurrentItem(_Char const* Item, unsigned int Length=0) { return pCurrent->SetItem(Item, Length); }
};

// Iterator Pointer-Index with Shared Strings Read-Only
template <class _Id, class _Char, unsigned int _GroupSize>
class IndexIterator<_Id*, String<_Char, false>, _GroupSize, true>: public IndexIteratorBase<_Id*, String<_Char, false>, _GroupSize, true>
{
private:
	// Using
	using _ItR=IndexIterator<_Id*, String<_Char, false>, _GroupSize, true>;
	using _ItW=IndexIterator<_Id*, String<_Char, false>, _GroupSize, false>;
	using _Cluster=::Clusters::Templates::Details::Cluster::Cluster<_IndexItem, _Group, _ItemGroup, _ParentGroup, _ItR, _ItW>;

public:
	// Con-/Destructors
	IndexIterator(_ItR const& It): IndexIteratorBase(It) {}
	IndexIterator(_ItW const& It): IndexIteratorBase(It) {}
	IndexIterator(_Cluster const* Cluster, size_t Position): IndexIteratorBase(Cluster) { SetPosition(Position); }
	IndexIterator(_Cluster const* Cluster, size_t, _Id* Id): IndexIteratorBase(Cluster) { FindInternal(Id); }

	// Access
	inline bool Find(_Id* Id) { return FindInternal(Id); }
	inline _Id* GetCurrentId()const { return pCurrent->GetId(); }
	inline _Char const* GetCurrentItem()const { return pCurrent->GetItem(); }
};

// Iterator Pointer-Index with Shared Strings Read-Write
template <class _Id, class _Char, unsigned int _GroupSize>
class IndexIterator<_Id*, String<_Char, false>, _GroupSize, false>: public IndexIteratorBase<_Id*, String<_Char, false>, _GroupSize, false>
{
private:
	// Using
	using _ItR=IndexIterator<_Id*, String<_Char, false>, _GroupSize, true>;
	using _ItW=IndexIterator<_Id*, String<_Char, false>, _GroupSize, false>;
	using _Cluster=::Clusters::Templates::Details::Cluster::Cluster<_IndexItem, _Group, _ItemGroup, _ParentGroup, _ItR, _ItW>;

public:
	// Con-/Destructors
	IndexIterator(_ItW const& It): IndexIteratorBase(It) {}
	IndexIterator(_Cluster* Cluster, size_t Position): IndexIteratorBase(Cluster) { SetPosition(Position); }
	IndexIterator(_Cluster* Cluster, size_t, _Id* Id): IndexIteratorBase(Cluster) { FindInternal(Id); }

	// Access
	inline bool Find(_Id* Id) { return FindInternal(Id); }
	inline _Id* GetCurrentId()const { return pCurrent->GetId(); }
	inline _Char const* GetCurrentItem()const { return pCurrent->GetItem(); }

	// Modification
	inline void SetCurrentItem(_Char const* Item) { pCurrent->SetItem(Item); }
};


//=======================
// Iterator Handle-Index
//=======================

#ifdef __cplusplus_winrt

// Iterator Handle-Index Read-Only
template <class _Id, class _Item, unsigned int _GroupSize>
class IndexIterator<_Id^, _Item, _GroupSize, true>: public IndexIteratorBase<_Id^, _Item, _GroupSize, true>
{
private:
	// Using
	using _ItR=IndexIterator<_Id^, _Item, _GroupSize, true>;
	using _ItW=IndexIterator<_Id^, _Item, _GroupSize, false>;
	using _Cluster=::Clusters::Templates::Details::Cluster::Cluster<_IndexItem, _Group, _ItemGroup, _ParentGroup, _ItR, _ItW>;

public:
	// Con-/Destructors
	IndexIterator(_ItR const& It): IndexIteratorBase(It) {}
	IndexIterator(_ItW const& It): IndexIteratorBase(It) {}
	IndexIterator(_Cluster const* Cluster, size_t Position): IndexIteratorBase(Cluster) { SetPosition(Position); }
	IndexIterator(_Cluster const* Cluster, _Id^ Id): IndexIteratorBase(Cluster) { FindInternal(Id); }

	// Access
	inline bool Find(_Id^ Id) { return FindInternal(Id); }
	inline _Id^ GetCurrentId()const { return pCurrent->GetId(); }
	inline _Item const& GetCurrentItem()const { return pCurrent->GetItem(); }
};

// Iterator Handle-Index Read-Write
template <class _Id, class _Item, unsigned int _GroupSize>
class IndexIterator<_Id^, _Item, _GroupSize, false>: public IndexIteratorBase<_Id^, _Item, _GroupSize, false>
{
private:
	// Using
	using _ItR=IndexIterator<_Id^, _Item, _GroupSize, true>;
	using _ItW=IndexIterator<_Id^, _Item, _GroupSize, false>;
	using _Cluster=::Clusters::Templates::Details::Cluster::Cluster<_IndexItem, _Group, _ItemGroup, _ParentGroup, _ItR, _ItW>;

public:
	// Con-/Destructors
	IndexIterator(_ItW const& It): IndexIteratorBase(It) {}
	IndexIterator(_Cluster* Cluster, size_t Position): IndexIteratorBase(Cluster) { SetPosition(Position); }
	IndexIterator(_Cluster* Cluster, _Id^ Id): IndexIteratorBase(Cluster) { FindInternal(Id); }

	// Access
	inline bool Find(_Id^ Id) { return FindInternal(Id); }
	inline _Id^ GetCurrentId()const { return pCurrent->GetId(); }
	inline _Item& GetCurrentItem()const { return pCurrent->GetItem(); }

	// Modification
	inline void SetCurrentItem(_Item const& Item) { pCurrent->SetItem(Item); }
};

// Iterator Handle-Index without Items
template <class _Id, unsigned int _GroupSize>
class IndexIterator<_Id^, void, _GroupSize, true>: public IndexIteratorBase<_Id^, void, _GroupSize, true>
{
private:
	// Using
	using _ItR=IndexIterator<_Id^, void, _GroupSize, true>;
	using _ItW=IndexIterator<_Id^, void, _GroupSize, false>;
	using _Cluster=::Clusters::Templates::Details::Cluster::Cluster<_IndexItem, _Group, _ItemGroup, _ParentGroup, _ItR, _ItW>;

public:
	// Con-/Destructors
	IndexIterator(_ItR const& It): IndexIteratorBase(It) {}
	IndexIterator(_ItW const& It): IndexIteratorBase(It) {}
	IndexIterator(_Cluster const* Cluster, size_t Position): IndexIteratorBase(Cluster) { SetPosition(Position); }
	IndexIterator(_Cluster const* Cluster, _Id^ Id): IndexIteratorBase(Cluster) { FindInternal(Id); }

	// Access
	inline bool Find(_Id^ Id) { return FindInternal(Id); }
	inline _Id^ GetCurrent()const { return pCurrent->GetId(); }
};

// Iterator Handle-Index without Items Read-Write
template <class _Id, unsigned int _GroupSize>
class IndexIterator<_Id^, void, _GroupSize, false>: public IndexIteratorBase<_Id^, void, _GroupSize, false>
{
private:
	// Using
	using _ItR=IndexIterator<_Id^, void, _GroupSize, true>;
	using _ItW=IndexIterator<_Id^, void, _GroupSize, false>;
	using _Cluster=::Clusters::Templates::Details::Cluster::Cluster<_IndexItem, _Group, _ItemGroup, _ParentGroup, _ItR, _ItW>;

public:
	// Con-/Destructors
	IndexIterator(_ItW const& It): IndexIteratorBase(It) {}
	IndexIterator(_Cluster* Cluster, size_t Position): IndexIteratorBase(Cluster) { SetPosition(Position); }
	IndexIterator(_Cluster* Cluster, _Id^ Id): IndexIteratorBase(Cluster) { FindInternal(Id); }

	// Access
	inline bool Find(_Id^ Id) { return FindInternal(Id); }
	inline _Id^ GetCurrent()const { return pCurrent->GetId(); }
};

// Iterator Handle-Index with Pointers Read-Only
template <class _Id, class _Item, unsigned int _GroupSize>
class IndexIterator<_Id^, _Item*, _GroupSize, true>: public IndexIteratorBase<_Id^, _Item*, _GroupSize, true>
{
private:
	// Using
	using _ItR=IndexIterator<_Id^, _Item*, _GroupSize, true>;
	using _ItW=IndexIterator<_Id^, _Item*, _GroupSize, false>;
	using _Cluster=::Clusters::Templates::Details::Cluster::Cluster<_IndexItem, _Group, _ItemGroup, _ParentGroup, _ItR, _ItW>;

public:
	// Con-/Destructors
	IndexIterator(_ItR const& It): IndexIteratorBase(It) {}
	IndexIterator(_ItW const& It): IndexIteratorBase(It) {}
	IndexIterator(_Cluster const* Cluster, size_t Position): IndexIteratorBase(Cluster) { SetPosition(Position); }
	IndexIterator(_Cluster const* Cluster, _Id^ Id): IndexIteratorBase(Cluster) { FindInternal(Id); }

	// Access
	inline bool Find(_Id^ Id) { return FindInternal(Id); }
	inline _Id^ GetCurrentId()const { return pCurrent->GetId(); }
	inline _Item* GetCurrentItem()const { return pCurrent->GetItem(); }
};

// Iterator Handle-Index with Pointers Read-Write
template <class _Id, class _Item, unsigned int _GroupSize>
class IndexIterator<_Id^, _Item*, _GroupSize, false>: public IndexIteratorBase<_Id^, _Item*, _GroupSize, false>
{
private:
	// Using
	using _ItR=IndexIterator<_Id^, _Item*, _GroupSize, true>;
	using _ItW=IndexIterator<_Id^, _Item*, _GroupSize, false>;
	using _Cluster=::Clusters::Templates::Details::Cluster::Cluster<_IndexItem, _Group, _ItemGroup, _ParentGroup, _ItR, _ItW>;

public:
	// Con-/Destructors
	IndexIterator(_ItW const& It): IndexIteratorBase(It) {}
	IndexIterator(_Cluster* Cluster, size_t Position): IndexIteratorBase(Cluster) { SetPosition(Position); }
	IndexIterator(_Cluster* Cluster, _Id^ Id): IndexIteratorBase(Cluster) { FindInternal(Id); }

	// Access
	inline bool Find(_Id^ Id) { return FindInternal(Id); }
	inline _Id^ GetCurrentId()const { return pCurrent->GetId(); }
	inline _Item* GetCurrentItem()const { return pCurrent->GetItem(); }

	// Modification
	inline void SetCurrentItem(_Item* Item) { pCurrent->SetItem(Item); }
};

// Iterator Handle-Index with Handles Read-Only
template <class _Id, class _Item, unsigned int _GroupSize>
class IndexIterator<_Id^, _Item^, _GroupSize, true>: public IndexIteratorBase<_Id^, _Item^, _GroupSize, true>
{
private:
	// Using
	using _ItR=IndexIterator<_Id^, _Item^, _GroupSize, true>;
	using _ItW=IndexIterator<_Id^, _Item^, _GroupSize, false>;
	using _Cluster=::Clusters::Templates::Details::Cluster::Cluster<_IndexItem, _Group, _ItemGroup, _ParentGroup, _ItR, _ItW>;

public:
	// Con-/Destructors
	IndexIterator(_ItR const& It): IndexIteratorBase(It) {}
	IndexIterator(_ItW const& It): IndexIteratorBase(It) {}
	IndexIterator(_Cluster const* Cluster, size_t Position): IndexIteratorBase(Cluster) { SetPosition(Position); }
	IndexIterator(_Cluster const* Cluster, _Id^ Id): IndexIteratorBase(Cluster) { FindInternal(Id); }

	// Access
	inline bool Find(_Id^ Id) { return FindInternal(Id); }
	inline _Id^ GetCurrentId()const { return pCurrent->GetId(); }
	inline _Item^ GetCurrentItem()const { return pCurrent->GetItem(); }
};

// Iterator Handle-Index with Handles Read-Write
template <class _Id, class _Item, unsigned int _GroupSize>
class IndexIterator<_Id^, _Item^, _GroupSize, false>: public IndexIteratorBase<_Id^, _Item^, _GroupSize, false>
{
private:
	// Using
	using _ItR=IndexIterator<_Id^, _Item^, _GroupSize, true>;
	using _ItW=IndexIterator<_Id^, _Item^, _GroupSize, false>;
	using _Cluster=::Clusters::Templates::Details::Cluster::Cluster<_IndexItem, _Group, _ItemGroup, _ParentGroup, _ItR, _ItW>;

public:
	// Con-/Destructors
	IndexIterator(_ItW const& It): IndexIteratorBase(It) {}
	IndexIterator(_Cluster* Cluster, size_t Position): IndexIteratorBase(Cluster) { SetPosition(Position); }
	IndexIterator(_Cluster* Cluster, _Id^ Id): IndexIteratorBase(Cluster) { FindInternal(Id); }

	// Access
	inline bool Find(_Id^ Id) { return FindInternal(Id); }
	inline _Id^ GetCurrentId()const { return pCurrent->GetId(); }
	inline _Item^ GetCurrentItem()const { return pCurrent->GetItem(); }

	// Modification
	inline void SetCurrentItem(_Item^ Item) { pCurrent->SetItem(Item); }
};

// Iterator Handle-Index with Strings Read-Only
template <class _Id, class _Char, bool _AllocItem, unsigned int _GroupSize>
class IndexIterator<_Id^, String<_Char, _AllocItem>, _GroupSize, true>: public IndexIteratorBase<_Id^, String<_Char, _AllocItem>, _GroupSize, true>
{
private:
	// Using
	using _ItR=IndexIterator<_Id^, String<_Char, _AllocItem>, _GroupSize, true>;
	using _ItW=IndexIterator<_Id^, String<_Char, _AllocItem>, _GroupSize, false>;
	using _Cluster=::Clusters::Templates::Details::Cluster::Cluster<_IndexItem, _Group, _ItemGroup, _ParentGroup, _ItR, _ItW>;

public:
	// Con-/Destructors
	IndexIterator(_ItR const& It): IndexIteratorBase(It) {}
	IndexIterator(_ItW const& It): IndexIteratorBase(It) {}
	IndexIterator(_Cluster const* Cluster, size_t Position): IndexIteratorBase(Cluster) { SetPosition(Position); }
	IndexIterator(_Cluster const* Cluster, _Char const* Id, unsigned int Length=0, bool CaseSensitive=false): IndexIteratorBase(Cluster) { FindInternal(Id, Length, CaseSensitive); }

	// Access
	inline bool Find(_Char const* Id, unsigned int Length=0, bool CaseSensitive=false) { return FindInternal(Id, Length, CaseSensitive); }
	inline _Id^ GetCurrentId()const { return pCurrent->GetId(); }
	inline _Char const* GetCurrentItem()const { return pCurrent->GetItem(); }
};

// Iterator Handle-Index with Strings Read-Write
template <class _Id, class _Char, unsigned int _GroupSize>
class IndexIterator<_Id^, String<_Char, true>, _GroupSize, false>: public IndexIteratorBase<_Id^, String<_Char, true>, _GroupSize, false>
{
private:
	// Using
	using _ItR=IndexIterator<_Id^, String<_Char, true>, _GroupSize, true>;
	using _ItW=IndexIterator<_Id^, String<_Char, true>, _GroupSize, false>;
	using _Cluster=::Clusters::Templates::Details::Cluster::Cluster<_IndexItem, _Group, _ItemGroup, _ParentGroup, _ItR, _ItW>;

public:
	// Con-/Destructors
	IndexIterator(_ItW const& It): IndexIteratorBase(It) {}
	IndexIterator(_Cluster* Cluster, size_t Position): IndexIteratorBase(Cluster) { SetPosition(Position); }
	IndexIterator(_Cluster* Cluster, _Id^ Id): IndexIteratorBase(Cluster) { FindInternal(Id); }

	// Access
	inline bool Find(_Id^ Id) { return FindInternal(Id); }
	inline _Id^ GetCurrentId()const { return pCurrent->GetId(); }
	inline _Char const* GetCurrentItem()const { return pCurrent->GetItem(); }

	// Modification
	inline unsigned int SetCurrentItem(_Char const* Item, unsigned int Length=0) { return pCurrent->SetItem(Item, Length); }
};

// Iterator Handle-Index with Shared Strings Read-Only
template <class _Id, class _Char, unsigned int _GroupSize>
class IndexIterator<_Id^, String<_Char, false>, _GroupSize, true>: public IndexIteratorBase<_Id^, String<_Char, false>, _GroupSize, true>
{
private:
	// Using
	using _ItR=IndexIterator<_Id^, String<_Char, false>, _GroupSize, true>;
	using _ItW=IndexIterator<_Id^, String<_Char, false>, _GroupSize, false>;
	using _Cluster=::Clusters::Templates::Details::Cluster::Cluster<_IndexItem, _Group, _ItemGroup, _ParentGroup, _ItR, _ItW>;

public:
	// Con-/Destructors
	IndexIterator(_ItR const& It): IndexIteratorBase(It) {}
	IndexIterator(_ItW const& It): IndexIteratorBase(It) {}
	IndexIterator(_Cluster const* Cluster, size_t Position): IndexIteratorBase(Cluster) { SetPosition(Position); }
	IndexIterator(_Cluster const* Cluster, _Char const* Id, unsigned int Length=0, bool CaseSensitive=false): IndexIteratorBase(Cluster) { FindInternal(Id, Length, CaseSensitive); }

	// Access
	inline bool Find(_Char const* Id, unsigned int Length=0, bool CaseSensitive=false) { return FindInternal(Id, Length, CaseSensitive); }
	inline _Id^ GetCurrentId()const { return pCurrent->GetId(); }
	inline _Char const* GetCurrentItem()const { return pCurrent->GetItem(); }
};

// Iterator Handle-Index with Shared Strings Read-Write
template <class _Id, class _Char, unsigned int _GroupSize>
class IndexIterator<_Id^, String<_Char, false>, _GroupSize, false>: public IndexIteratorBase<_Id^, String<_Char, false>, _GroupSize, false>
{
private:
	// Using
	using _ItR=IndexIterator<_Id^, String<_Char, false>, _GroupSize, true>;
	using _ItW=IndexIterator<_Id^, String<_Char, false>, _GroupSize, false>;
	using _Cluster=::Clusters::Templates::Details::Cluster::Cluster<_IndexItem, _Group, _ItemGroup, _ParentGroup, _ItR, _ItW>;

public:
	// Con-/Destructors
	IndexIterator(_ItW const& It): IndexIteratorBase(It) {}
	IndexIterator(_Cluster* Cluster, size_t Position): IndexIteratorBase(Cluster) { SetPosition(Position); }
	IndexIterator(_Cluster* Cluster, _Id^ Id): IndexIteratorBase(Cluster) { FindInternal(Id); }

	// Access
	inline bool Find(_Id^ Id) { return FindInternal(Id); }
	inline _Id^ GetCurrentId()const { return pCurrent->GetId(); }
	inline _Char const* GetCurrentItem()const { return pCurrent->GetItem(); }

	// Modification
	inline void SetCurrentItem(_Char const* Item) { pCurrent->SetItem(Item); }
};

#endif


//=======================
// Iterator String-Index
//=======================

// Iterator String-Index Read-Only
template <class _Char, bool _AllocId, class _Item, unsigned int _GroupSize>
class IndexIterator<String<_Char, _AllocId>, _Item, _GroupSize, true>: public IndexIteratorBase<String<_Char, true>, _Item, _GroupSize, true>
{
private:
	// Using
	using _ItR=IndexIterator<String<_Char, _AllocId>, _Item, _GroupSize, true>;
	using _ItW=IndexIterator<String<_Char, _AllocId>, _Item, _GroupSize, false>;
	using _Cluster=::Clusters::Templates::Details::Cluster::Cluster<_IndexItem, _Group, _ItemGroup, _ParentGroup, _ItR, _ItW>;

public:
	// Con-/Destructors
	IndexIterator(_ItR const& It): IndexIteratorBase(It) {}
	IndexIterator(_ItW const& It): IndexIteratorBase(It) {}
	IndexIterator(_Cluster const* Cluster, size_t Position): IndexIteratorBase(Cluster) { SetPosition(Position); }
	IndexIterator(_Cluster const* Cluster, _Char const* Id, unsigned int Length, bool CaseSensitive): IndexIteratorBase(Cluster) { FindInternal(Id, Length, CaseSensitive); }

	// Access
	inline bool Find(_Char const* Id, unsigned int Length=0, bool CaseSensitive=false) { return FindInternal(Id, Length, CaseSensitive); }
	inline _Char const* GetCurrentId()const { return pCurrent->GetId(); }
	inline _Item const& GetCurrentItem()const { return pCurrent->GetItem(); }
};

// Iterator String-Index Read-Write
template <class _Char, bool _AllocId, class _Item, unsigned int _GroupSize>
class IndexIterator<String<_Char, _AllocId>, _Item, _GroupSize, false>: public IndexIteratorBase<String<_Char, _AllocId>, _Item, _GroupSize, false>
{
private:
	// Using
	using _ItR=IndexIterator<String<_Char, _AllocId>, _Item, _GroupSize, true>;
	using _ItW=IndexIterator<String<_Char, _AllocId>, _Item, _GroupSize, false>;
	using _Cluster=::Clusters::Templates::Details::Cluster::Cluster<_IndexItem, _Group, _ItemGroup, _ParentGroup, _ItR, _ItW>;

public:
	// Con-/Destructors
	IndexIterator(_ItW const& It): IndexIteratorBase(It) {}
	IndexIterator(_Cluster* Cluster, size_t Position): IndexIteratorBase(Cluster) { SetPosition(Position); }
	IndexIterator(_Cluster* Cluster, _Char const* Id, unsigned int Length, bool CaseSensitive): IndexIteratorBase(Cluster) { FindInternal(Id, Length, CaseSensitive); }

	// Access
	inline bool Find(_Char const* Id, unsigned int Length=0, bool CaseSensitive=false) { return FindInternal(Id, Length, CaseSensitive); }
	inline _Char const* GetCurrentId()const { return pCurrent->GetId(); }
	inline _Item& GetCurrentItem()const { return pCurrent->GetItem(); }

	// Modification
	inline void SetCurrentItem(_Item const& Item) { return pCurrent->SetItem(Item); }
};

// Iterator String-Index without Items Read-Only
template <class _Char, bool _AllocId, unsigned int _GroupSize>
class IndexIterator<String<_Char, _AllocId>, void, _GroupSize, true>: public IndexIteratorBase<String<_Char, _AllocId>, void, _GroupSize, true>
{
private:
	// Using
	using _ItR=IndexIterator<String<_Char, _AllocId>, void, _GroupSize, true>;
	using _ItW=IndexIterator<String<_Char, _AllocId>, void, _GroupSize, false>;
	using _Cluster=::Clusters::Templates::Details::Cluster::Cluster<_IndexItem, _Group, _ItemGroup, _ParentGroup, _ItR, _ItW>;

public:
	// Con-/Destructors
	IndexIterator(_ItR const& It): IndexIteratorBase(It) {}
	IndexIterator(_ItW const& It): IndexIteratorBase(It) {}
	IndexIterator(_Cluster const* Cluster, size_t Position): IndexIteratorBase(Cluster) { SetPosition(Position); }
	IndexIterator(_Cluster const* Cluster, _Char const* Id, unsigned int Length, bool CaseSensitive): IndexIteratorBase(Cluster) { FindInternal(Id, Length, CaseSensitive); }

	// Access
	inline bool Find(_Char const* Id, unsigned int Length=0, bool CaseSensitive=false) { return FindInternal(Id, Length, CaseSensitive); }
	inline _Char const* GetCurrent()const { return pCurrent->GetId(); }
};

// Iterator String-Index without Items Read-Write
template <class _Char, bool _AllocId, unsigned int _GroupSize>
class IndexIterator<String<_Char, _AllocId>, void, _GroupSize, false>: public IndexIteratorBase<String<_Char, _AllocId>, void, _GroupSize, false>
{
private:
	// Using
	using _ItR=IndexIterator<String<_Char, _AllocId>, void, _GroupSize, true>;
	using _ItW=IndexIterator<String<_Char, _AllocId>, void, _GroupSize, false>;
	using _Cluster=::Clusters::Templates::Details::Cluster::Cluster<_IndexItem, _Group, _ItemGroup, _ParentGroup, _ItR, _ItW>;

public:
	// Con-/Destructors
	IndexIterator(_ItW const& It): IndexIteratorBase(It) {}
	IndexIterator(_Cluster* Cluster, size_t Position): IndexIteratorBase(Cluster) { SetPosition(Position); }
	IndexIterator(_Cluster* Cluster, _Char const* Id, unsigned int Length, bool CaseSensitive): IndexIteratorBase(Cluster) { FindInternal(Id, Length, CaseSensitive); }

	// Access
	inline bool Find(_Char const* Id, unsigned int Length=0, bool CaseSensitive=false) { return FindInternal(Id, Length, CaseSensitive); }
	inline _Char const* GetCurrent()const { return pCurrent->GetId(); }
};

// Iterator String-Index with Pointers Read-Only
template <class _Char, bool _AllocId, class _Item, unsigned int _GroupSize>
class IndexIterator<String<_Char, _AllocId>, _Item*, _GroupSize, true>: public IndexIteratorBase<String<_Char, _AllocId>, _Item*, _GroupSize, true>
{
private:
	// Using
	using _ItR=IndexIterator<String<_Char, _AllocId>, _Item*, _GroupSize, true>;
	using _ItW=IndexIterator<String<_Char, _AllocId>, _Item*, _GroupSize, false>;
	using _Cluster=::Clusters::Templates::Details::Cluster::Cluster<_IndexItem, _Group, _ItemGroup, _ParentGroup, _ItR, _ItW>;

public:
	// Con-/Destructors
	IndexIterator(_ItR const& It): IndexIteratorBase(It) {}
	IndexIterator(_ItW const& It): IndexIteratorBase(It) {}
	IndexIterator(_Cluster const* Cluster, size_t Position): IndexIteratorBase(Cluster) { SetPosition(Position); }
	IndexIterator(_Cluster const* Cluster, _Char const* Id, unsigned int Length, bool CaseSensitive): IndexIteratorBase(Cluster) { FindInternal(Id, Length, CaseSensitive); }

	// Access
	inline bool Find(_Char const* Id, unsigned int Length=0, bool CaseSensitive=false) { return FindInternal(Id, Length, CaseSensitive); }
	inline _Char const* GetCurrentId()const { return pCurrent->GetId(); }
	inline _Item* GetCurrentItem()const { return pCurrent->GetItem(); }
};

// Iterator String-Index with Pointers Read-Write
template <class _Char, bool _AllocId, class _Item, unsigned int _GroupSize>
class IndexIterator<String<_Char, _AllocId>, _Item*, _GroupSize, false>: public IndexIteratorBase<String<_Char, _AllocId>, _Item*, _GroupSize, false>
{
private:
	// Using
	using _ItR=IndexIterator<String<_Char, _AllocId>, _Item*, _GroupSize, true>;
	using _ItW=IndexIterator<String<_Char, _AllocId>, _Item*, _GroupSize, false>;
	using _Cluster=::Clusters::Templates::Details::Cluster::Cluster<_IndexItem, _Group, _ItemGroup, _ParentGroup, _ItR, _ItW>;

public:
	// Con-/Destructors
	IndexIterator(_ItW const& It): IndexIteratorBase(It) {}
	IndexIterator(_Cluster* Cluster, size_t Position): IndexIteratorBase(Cluster) { SetPosition(Position); }
	IndexIterator(_Cluster* Cluster, _Char const* Id, unsigned int Length, bool CaseSensitive): IndexIteratorBase(Cluster) { FindInternal(Id, Length, CaseSensitive); }

	// Access
	inline bool Find(_Char const* Id, unsigned int Length=0, bool CaseSensitive=false) { return FindInternal(Id, Length, CaseSensitive); }
	inline _Char const* GetCurrentId()const { return pCurrent->GetId(); }
	inline _Item* GetCurrentItem()const { return pCurrent->GetItem(); }

	// Modification
	inline void SetCurrentItem(_Item* Item) { return pCurrent->SetItem(Item); }
};

#ifdef __cplusplus_winrt
// Iterator String-Index with Handles Read-Only
template <class _Char, bool _AllocId, class _Item, unsigned int _GroupSize>
class IndexIterator<String<_Char, _AllocId>, _Item^, _GroupSize, true>: public IndexIteratorBase<String<_Char, _AllocId>, _Item^, _GroupSize, true>
{
private:
	// Using
	using _ItR=IndexIterator<String<_Char, _AllocId>, _Item^, _GroupSize, true>;
	using _ItW=IndexIterator<String<_Char, _AllocId>, _Item^, _GroupSize, false>;
	using _Cluster=::Clusters::Templates::Details::Cluster::Cluster<_IndexItem, _Group, _ItemGroup, _ParentGroup, _ItR, _ItW>;

public:
	// Con-/Destructors
	IndexIterator(_ItR const& It): IndexIteratorBase(It) {}
	IndexIterator(_ItW const& It): IndexIteratorBase(It) {}
	IndexIterator(_Cluster const* Cluster, size_t Position): IndexIteratorBase(Cluster) { SetPosition(Position); }
	IndexIterator(_Cluster const* Cluster, _Char const* Id, unsigned int Length, bool CaseSensitive): IndexIteratorBase(Cluster) { FindInternal(Id, Length, CaseSensitive); }

	// Access
	inline bool Find(_Char const* Id, unsigned int Length=0, bool CaseSensitive=false) { return FindInternal(Id, Length, CaseSensitive); }
	inline _Char const* GetCurrentId()const { return pCurrent->GetId(); }
	inline _Item^ GetCurrentItem()const { return pCurrent->GetItem(); }
};
#endif

#ifdef __cplusplus_winrt
// Iterator String-Index with Handles Read-Write
template <class _Char, bool _AllocId, class _Item, unsigned int _GroupSize>
class IndexIterator<String<_Char, _AllocId>, _Item^, _GroupSize, false>: public IndexIteratorBase<String<_Char, _AllocId>, _Item^, _GroupSize, false>
{
private:
	// Using
	using _ItR=IndexIterator<String<_Char, _AllocId>, _Item^, _GroupSize, true>;
	using _ItW=IndexIterator<String<_Char, _AllocId>, _Item^, _GroupSize, false>;
	using _Cluster=::Clusters::Templates::Details::Cluster::Cluster<_IndexItem, _Group, _ItemGroup, _ParentGroup, _ItR, _ItW>;

public:
	// Con-/Destructors
	IndexIterator(_ItW const& It): IndexIteratorBase(It) {}
	IndexIterator(_Cluster* Cluster, size_t Position): IndexIteratorBase(Cluster) { SetPosition(Position); }
	IndexIterator(_Cluster* Cluster, _Char const* Id, unsigned int Length, bool CaseSensitive): IndexIteratorBase(Cluster) { FindInternal(Id, Length, CaseSensitive); }

	// Access
	inline bool Find(_Char const* Id, unsigned int Length=0, bool CaseSensitive=false) { return FindInternal(Id, Length, CaseSensitive); }
	inline _Char const* GetCurrentId()const { return pCurrent->GetId(); }
	inline _Item^ GetCurrentItem()const { return pCurrent->GetItem(); }

	// Modification
	inline void SetCurrentItem(_Item^ Item) { return pCurrent->SetItem(Item); }
};
#endif

// Iterator String-Index with Strings Read-Only
template <class _CharId, bool _AllocId, class _CharItem, unsigned int _GroupSize>
class IndexIterator<String<_CharId, _AllocId>, String<_CharItem, true>, _GroupSize, true>: public IndexIteratorBase<String<_CharId, _AllocId>, String<_CharItem, true>, _GroupSize, true>
{
private:
	// Using
	using _ItR=IndexIterator<String<_CharId, _AllocId>, String<_CharItem, true>, _GroupSize, true>;
	using _ItW=IndexIterator<String<_CharId, _AllocId>, String<_CharItem, true>, _GroupSize, false>;
	using _Cluster=::Clusters::Templates::Details::Cluster::Cluster<_IndexItem, _Group, _ItemGroup, _ParentGroup, _ItR, _ItW>;

public:
	// Con-/Destructors
	IndexIterator(_ItR const& It): IndexIteratorBase(It) {}
	IndexIterator(_ItW const& It): IndexIteratorBase(It) {}
	IndexIterator(_Cluster const* Cluster, size_t Position): IndexIteratorBase(Cluster) { SetPosition(Position); }
	IndexIterator(_Cluster const* Cluster, _CharId const* Id, unsigned int Length, bool CaseSensitive): IndexIteratorBase(Cluster) { FindInternal(Id, Length, CaseSensitive); }

	// Access
	inline bool Find(_CharId const* Id, unsigned int Length=0, bool CaseSensitive=false) { return FindInternal(Id, Length, CaseSensitive); }
	inline _CharId const* GetCurrentId()const { return pCurrent->GetId(); }
	inline _CharItem const* GetCurrentItem()const { return pCurrent->GetItem(); }
};

// Iterator String-Index with Strings Read-Write
template <class _CharId, bool _AllocId, class _CharItem, unsigned int _GroupSize>
class IndexIterator<String<_CharId, _AllocId>, String<_CharItem, true>, _GroupSize, false>: public IndexIteratorBase<String<_CharId, _AllocId>, String<_CharItem, true>, _GroupSize, false>
{
private:
	// Using
	using _ItR=IndexIterator<String<_CharId, _AllocId>, String<_CharItem, true>, _GroupSize, true>;
	using _ItW=IndexIterator<String<_CharId, _AllocId>, String<_CharItem, true>, _GroupSize, false>;
	using _Cluster=::Clusters::Templates::Details::Cluster::Cluster<_IndexItem, _Group, _ItemGroup, _ParentGroup, _ItR, _ItW>;

public:
	// Con-/Destructors
	IndexIterator(_ItW const& It): IndexIteratorBase(It) {}
	IndexIterator(_Cluster* Cluster, size_t Position): IndexIteratorBase(Cluster) { SetPosition(Position); }
	IndexIterator(_Cluster* Cluster, _CharId const* Id, unsigned int Length, bool CaseSensitive): IndexIteratorBase(Cluster) { FindInternal(Id, Length, CaseSensitive); }

	// Access
	inline bool Find(_CharId const* Id, unsigned int Length=0, bool CaseSensitive=false) { return FindInternal(Id, Length, CaseSensitive); }
	inline _CharId const* GetCurrentId()const { return pCurrent->GetId(); }
	inline _CharItem const* GetCurrentItem()const { return pCurrent->GetItem(); }

	// Modification
	inline unsigned int SetCurrentItem(_CharItem const* Value, unsigned int Length=0) { return pCurrent->SetItem(Value, Length); }
};

// Iterator String-Index with Shared Strings Read-Only
template <class _CharId, bool _AllocId, class _CharItem, unsigned int _GroupSize>
class IndexIterator<String<_CharId, _AllocId>, String<_CharItem, false>, _GroupSize, true>: public IndexIteratorBase<String<_CharId, _AllocId>, String<_CharItem, false>, _GroupSize, true>
{
private:
	// Using
	using _ItR=IndexIterator<String<_CharId, _AllocId>, String<_CharItem, false>, _GroupSize, true>;
	using _ItW=IndexIterator<String<_CharId, _AllocId>, String<_CharItem, false>, _GroupSize, false>;
	using _Cluster=::Clusters::Templates::Details::Cluster::Cluster<_IndexItem, _Group, _ItemGroup, _ParentGroup, _ItR, _ItW>;

public:
	// Con-/Destructors
	IndexIterator(_ItR const& It): IndexIteratorBase(It) {}
	IndexIterator(_ItW const& It): IndexIteratorBase(It) {}
	IndexIterator(_Cluster const* Cluster, size_t Position): IndexIteratorBase(Cluster) { SetPosition(Position); }
	IndexIterator(_Cluster const* Cluster, _CharId const* Id, unsigned int Length, bool CaseSensitive): IndexIteratorBase(Cluster) { FindInternal(Id, Length, CaseSensitive); }

	// Access
	inline bool Find(_CharId const* Id, unsigned int Length=0, bool CaseSensitive=false) { return FindInternal(Id, Length, CaseSensitive); }
	inline _CharId const* GetCurrentId()const { return pCurrent->GetId(); }
	inline _CharItem const* GetCurrentItem()const { return pCurrent->GetItem(); }
};

// Iterator String-Index with Shared Strings Read-Write
template <class _CharId, bool _AllocId, class _CharItem, unsigned int _GroupSize>
class IndexIterator<String<_CharId, _AllocId>, String<_CharItem, false>, _GroupSize, false>: public IndexIteratorBase<String<_CharId, _AllocId>, String<_CharItem, false>, _GroupSize, false>
{
private:
	// Using
	using _ItR=IndexIterator<String<_CharId, _AllocId>, String<_CharItem, false>, _GroupSize, true>;
	using _ItW=IndexIterator<String<_CharId, _AllocId>, String<_CharItem, false>, _GroupSize, false>;
	using _Cluster=::Clusters::Templates::Details::Cluster::Cluster<_IndexItem, _Group, _ItemGroup, _ParentGroup, _ItR, _ItW>;

public:
	// Con-/Destructors
	IndexIterator(_ItW const& It): IndexIteratorBase(It) {}
	IndexIterator(_Cluster* Cluster, size_t Position): IndexIteratorBase(Cluster) { SetPosition(Position); }
	IndexIterator(_Cluster* Cluster, _CharId const* Id, unsigned int Length, bool CaseSensitive): IndexIteratorBase(Cluster) { FindInternal(Id, Length, CaseSensitive); }

	// Access
	inline bool Find(_CharId const* Id, unsigned int Length=0, bool CaseSensitive=false) { return FindInternal(Id, Length, CaseSensitive); }
	inline _CharId const* GetCurrentId()const { return pCurrent->GetId(); }
	inline _CharItem const* GetCurrentItem()const { return pCurrent->GetItem(); }

	// Modification
	inline void SetCurrentItem(_CharItem const* Value) { pCurrent->SetItem(Value); }
};

}}}}
