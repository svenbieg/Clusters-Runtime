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
	namespace Details {
		namespace Index {


//===========================
// Base-Class Iterator Index
//===========================

template <class ID, class ITEM, UINT _GroupSize, BOOL _ReadOnly>
class IndexIteratorBase: public ::Clusters::Details::Cluster::IteratorShared<IndexItem<ID, ITEM>, IndexGroup<ID, ITEM>, IndexItemGroup<ID, ITEM, _GroupSize>, IndexParentGroup<ID, ITEM, _GroupSize>, _ReadOnly>
{
protected:
	// Using
	using INDEXITEM=IndexItem<ID, ITEM>;
	using GROUP=IndexGroup<ID, ITEM>;
	using GROUP_PTR=Conditional<_ReadOnly, GROUP const*, GROUP*>;
	using ITEMGROUP=IndexItemGroup<ID, ITEM, _GroupSize>;
	using ITEMGROUP_PTR=Conditional<_ReadOnly, ITEMGROUP const*, ITEMGROUP*>;
	using PARENTGROUP=IndexParentGroup<ID, ITEM, _GroupSize>;
	using PARENTGROUP_PTR=Conditional<_ReadOnly, PARENTGROUP const*, PARENTGROUP*>;
	using BASE=::Clusters::Details::Cluster::IteratorShared<INDEXITEM, GROUP, ITEMGROUP, PARENTGROUP, _ReadOnly>;

	// Con-/Destructors
	using BASE::BASE;
	
	// Common
	template <class... PARAMS> BOOL FindInternal(PARAMS... Id)
		{
		BOOL bfound=true;
		GROUP_PTR pgroup=pCluster->pRoot;
		UINT ulevelcount=pgroup->GetLevel()+1;
		if(pIts)
			delete pIts;
		pIts=new ITERATOR[ulevelcount];
		for(UINT u=0; u<ulevelcount-1; u++)
			{
			PARENTGROUP_PTR pparent=(PARENTGROUP_PTR)pgroup;
			INT ipos=pparent->Find(Id...);
			if(ipos<0)
				{
				bfound;
				ipos++;
				ipos*=-1;
				}
			pIts[u].Group=pgroup;
			pIts[u].Position=ipos;
			pgroup=pparent->GetChild(ipos);
			}
		ITEMGROUP_PTR pitemgroup=(ITEMGROUP_PTR)pgroup;
		INT ipos=pitemgroup->Find(Id...);
		if(ipos<0)
			{
			bfound;
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

#pragma region Index

// Iterator Index Read-Only
template <class ID, class ITEM, UINT _GroupSize, BOOL _ReadOnly>
class IndexIterator: public IndexIteratorBase<ID, ITEM, _GroupSize, true>
{
private:
	// Using
	using IT_R=IndexIterator<ID, ITEM, _GroupSize, true>;
	using IT_W=IndexIterator<ID, ITEM, _GroupSize, false>;
	using CLUSTER=::Clusters::Details::Cluster::Cluster<INDEXITEM, GROUP, ITEMGROUP, PARENTGROUP, IT_R, IT_W>;

public:
	// Con-/Destructors
	IndexIterator(IT_R const& It): IndexIteratorBase(It) {}
	IndexIterator(CLUSTER const* Cluster, UINT64 Position): IndexIteratorBase(Cluster) { SetPosition(Position); }
	IndexIterator(CLUSTER const* Cluster, UINT64, ID const& Id): IndexIteratorBase(Cluster) { FindInternal<ID const&>(Id); }

	// Access
	inline BOOL Find(ID const& Id) { return FindInternal<ID const&>(Id); }
	inline ID const& GetCurrentId()const { return pCurrent->GetId(); }
	inline ITEM const& GetCurrentItem()const { return pCurrent->GetItem(); }
};

// Iterator Index Read-Write
template <class ID, class ITEM, UINT _GroupSize>
class IndexIterator<ID, ITEM, _GroupSize, false>: public IndexIteratorBase<ID, ITEM, _GroupSize, false>
{
private:
	// Using
	using IT_R=IndexIterator<ID, ITEM, _GroupSize, true>;
	using IT_W=IndexIterator<ID, ITEM, _GroupSize, false>;
	using CLUSTER=::Clusters::Details::Cluster::Cluster<INDEXITEM, GROUP, ITEMGROUP, PARENTGROUP, IT_R, IT_W>;

public:
	// Con-/Destructors
	IndexIterator(IT_W const& It): IndexIteratorBase(It) {}
	IndexIterator(CLUSTER* Cluster, UINT64 Position): IndexIteratorBase(Cluster) { SetPosition(Position); }
	IndexIterator(CLUSTER* Cluster, UINT64, ID const& Id): IndexIteratorBase(Cluster) { FindInternal<ID const&>(Id); }

	// Access
	inline BOOL Find(ID const& Id) { return FindInternal<ID const&>(Id); }
	inline ID const& GetCurrentId()const { return pCurrent->GetId(); }
	inline ITEM& GetCurrentItem()const { return pCurrent->GetItem(); }

	// Modification
	inline VOID SetCurrentItem(ITEM const& Item) { pCurrent->SetItem(Item); }
};

// Iterator Index without Items Read-Only
template <class ID, UINT _GroupSize>
class IndexIterator<ID, VOID, _GroupSize, true>: public IndexIteratorBase<ID, VOID, _GroupSize, true>
{
private:
	// Using
	using IT_R=IndexIterator<ID, VOID, _GroupSize, true>;
	using IT_W=IndexIterator<ID, VOID, _GroupSize, false>;
	using CLUSTER=::Clusters::Details::Cluster::Cluster<INDEXITEM, GROUP, ITEMGROUP, PARENTGROUP, IT_R, IT_W>;

public:
	// Con-/Destructors
	IndexIterator(IT_R const& It): IndexIteratorBase(It) {}
	IndexIterator(CLUSTER const* Cluster, UINT64 Position): IndexIteratorBase(Cluster) { SetPosition(Position); }
	IndexIterator(CLUSTER const* Cluster, UINT64, ID const& Id): IndexIteratorBase(Cluster) { FindInternal<ID const&>(Id); }

	// Access
	inline BOOL Find(ID const& Id) { return FindInternal<ID const&>(Id); }
	inline ID const& GetCurrent()const { return pCurrent->GetId(); }
};

// Iterator Index without Items Read-Write
template <class ID, UINT _GroupSize>
class IndexIterator<ID, VOID, _GroupSize, false>: public IndexIteratorBase<ID, VOID, _GroupSize, false>
{
private:
	// Using
	using IT_R=IndexIterator<ID, VOID, _GroupSize, true>;
	using IT_W=IndexIterator<ID, VOID, _GroupSize, false>;
	using CLUSTER=::Clusters::Details::Cluster::Cluster<INDEXITEM, GROUP, ITEMGROUP, PARENTGROUP, IT_R, IT_W>;

public:
	// Con-/Destructors
	IndexIterator(IT_W const& It): IndexIteratorBase(It) {}
	IndexIterator(CLUSTER* Cluster, UINT64 Position): IndexIteratorBase(Cluster) { SetPosition(Position); }
	IndexIterator(CLUSTER* Cluster, UINT64, ID const& Id): IndexIteratorBase(Cluster) { FindInternal<ID const&>(Id); }

	// Access
	inline BOOL Find(ID const& Id) { return FindInternal<ID const&>(Id); }
	inline ID const& GetCurrent()const { return pCurrent->GetId(); }
};

// Iterator Index with Pointers Read-Only
template <class ID, class ITEM, UINT _GroupSize>
class IndexIterator<ID, ITEM*, _GroupSize, true>: public IndexIteratorBase<ID, ITEM*, _GroupSize, true>
{
private:
	// Using
	using IT_R=IndexIterator<ID, ITEM*, _GroupSize, true>;
	using IT_W=IndexIterator<ID, ITEM*, _GroupSize, false>;
	using CLUSTER=::Clusters::Details::Cluster::Cluster<INDEXITEM, GROUP, ITEMGROUP, PARENTGROUP, IT_R, IT_W>;

public:
	// Con-/Destructors
	IndexIterator(IT_R const& It): IndexIteratorBase(It) {}
	IndexIterator(CLUSTER const* Cluster, UINT64 Position): IndexIteratorBase(Cluster) { SetPosition(Position); }
	IndexIterator(CLUSTER const* Cluster, UINT64, ID const& Id): IndexIteratorBase(Cluster) { FindInternal<ID const&>(Id); }

	// Access
	inline BOOL Find(ID const& Id) { return FindInternal<ID const&>(Id); }
	inline ID const& GetCurrentId()const { return pCurrent->GetId(); }
	inline ITEM* GetCurrentItem()const { return pCurrent->GetItem(); }
};

// Iterator Index with Pointers Read-Write
template <class ID, class ITEM, UINT _GroupSize>
class IndexIterator<ID, ITEM*, _GroupSize, false>: public IndexIteratorBase<ID, ITEM*, _GroupSize, false>
{
private:
	// Using
	using IT_R=IndexIterator<ID, ITEM*, _GroupSize, true>;
	using IT_W=IndexIterator<ID, ITEM*, _GroupSize, false>;
	using CLUSTER=::Clusters::Details::Cluster::Cluster<INDEXITEM, GROUP, ITEMGROUP, PARENTGROUP, IT_R, IT_W>;

public:
	// Con-/Destructors
	IndexIterator(IT_W const& It): IndexIteratorBase(It) {}
	IndexIterator(CLUSTER* Cluster, UINT64 Position): IndexIteratorBase(Cluster) { SetPosition(Position); }
	IndexIterator(CLUSTER* Cluster, UINT64, ID const& Id): IndexIteratorBase(Cluster) { FindInternal<ID const&>(Id); }

	// Access
	inline BOOL Find(ID const& Id) { return FindInternal<ID const&>(Id); }
	inline ID const& GetCurrentId()const { return pCurrent->GetId(); }
	inline ITEM* GetCurrentItem()const { return pCurrent->GetItem(); }

	// Modification
	inline VOID SetCurrentItem(ITEM* Item) { pCurrent->SetItem(Item); }
};

#ifdef __cplusplus_winrt
// Iterator Index with Handles Read-Only
template <class ID, class ITEM, UINT _GroupSize>
class IndexIterator<ID, ITEM^, _GroupSize, true>: public IndexIteratorBase<ID, ITEM^, _GroupSize, true>
{
private:
	// Using
	using IT_R=IndexIterator<ID, ITEM^, _GroupSize, true>;
	using IT_W=IndexIterator<ID, ITEM^, _GroupSize, false>;
	using CLUSTER=::Clusters::Details::Cluster::Cluster<INDEXITEM, GROUP, ITEMGROUP, PARENTGROUP, IT_R, IT_W>;

public:
	// Con-/Destructors
	IndexIterator(IT_R const& It): IndexIteratorBase(It) {}
	IndexIterator(CLUSTER const* Cluster, UINT64 Position): IndexIteratorBase(Cluster) { SetPosition(Position); }
	IndexIterator(CLUSTER const* Cluster, UINT64, ID const& Id): IndexIteratorBase(Cluster) { FindInternal<ID const&>(Id); }

	// Access
	inline BOOL Find(ID const& Id) { return FindInternal<ID const&>(Id); }
	inline ID const& GetCurrentId()const { return pCurrent->GetId(); }
	inline ITEM^ GetCurrentItem()const { return pCurrent->GetItem(); }
};
#endif

#ifdef __cplusplus_winrt
// Iterator Index with Handles Read-Write
template <class ID, class ITEM, UINT _GroupSize>
class IndexIterator<ID, ITEM^, _GroupSize, false>: public IndexIteratorBase<ID, ITEM^, _GroupSize, false>
{
private:
	// Using
	using IT_R=IndexIterator<ID, ITEM^, _GroupSize, true>;
	using IT_W=IndexIterator<ID, ITEM^, _GroupSize, false>;
	using CLUSTER=::Clusters::Details::Cluster::Cluster<INDEXITEM, GROUP, ITEMGROUP, PARENTGROUP, IT_R, IT_W>;

public:
	// Con-/Destructors
	IndexIterator(IT_W const& It): IndexIteratorBase(It) {}
	IndexIterator(CLUSTER* Cluster, UINT64 Position): IndexIteratorBase(Cluster) { SetPosition(Position); }
	IndexIterator(CLUSTER* Cluster, UINT64, ID const& Id): IndexIteratorBase(Cluster) { FindInternal<ID const&>(Id); }

	// Access
	inline BOOL Find(ID const& Id) { return FindInternal<ID const&>(Id); }
	inline ID const& GetCurrentId()const { return pCurrent->GetId(); }
	inline ITEM^ GetCurrentItem()const { return pCurrent->GetItem(); }

	// Modification
	inline VOID SetCurrentItem(ITEM^ Item) { pCurrent->SetItem(Item); }
};
#endif

// Iterator Index with Strings Read-Only
template <class ID, class CHAR, BOOL _AllocItem, UINT _GroupSize>
class IndexIterator<ID, String<CHAR, _AllocItem>, _GroupSize, true>: public IndexIteratorBase<ID, String<CHAR, _AllocItem>, _GroupSize, true>
{
private:
	// Using
	using IT_R=IndexIterator<ID, String<CHAR, _AllocItem>, _GroupSize, true>;
	using IT_W=IndexIterator<ID, String<CHAR, _AllocItem>, _GroupSize, false>;
	using CLUSTER=::Clusters::Details::Cluster::Cluster<INDEXITEM, GROUP, ITEMGROUP, PARENTGROUP, IT_R, IT_W>;

public:
	// Con-/Destructors
	IndexIterator(IT_R const& It): IndexIteratorBase(It) {}
	IndexIterator(CLUSTER const* Cluster, UINT64 Position): IndexIteratorBase(Cluster) { SetPosition(Position); }
	IndexIterator(CLUSTER const* Cluster, UINT64, ID const& Id): IndexIteratorBase(Cluster) { FindInternal<ID const&>(Id); }

	// Access
	inline BOOL Find(ID const& Id) { return FindInternal<ID const&>(Id); }
	inline ID const& GetCurrentId()const { return pCurrent->GetId(); }
	inline CHAR const* GetCurrentItem()const { return pCurrent->GetItem(); }
};

// Iterator Index with Strings Read-Write
template <class ID, class CHAR, UINT _GroupSize>
class IndexIterator<ID, String<CHAR, true>, _GroupSize, false>: public IndexIteratorBase<ID, String<CHAR, true>, _GroupSize, false>
{
private:
	// Using
	using IT_R=IndexIterator<ID, String<CHAR, true>, _GroupSize, true>;
	using IT_W=IndexIterator<ID, String<CHAR, true>, _GroupSize, false>;
	using CLUSTER=::Clusters::Details::Cluster::Cluster<INDEXITEM, GROUP, ITEMGROUP, PARENTGROUP, IT_R, IT_W>;

public:
	// Con-/Destructors
	IndexIterator(IT_W const& It): IndexIteratorBase(It) {}
	IndexIterator(CLUSTER* Cluster, UINT64 Position): IndexIteratorBase(Cluster) { SetPosition(Position); }
	IndexIterator(CLUSTER* Cluster, UINT64, ID const& Id): IndexIteratorBase(Cluster) { FindInternal<ID const&>(Id); }

	// Access
	inline BOOL Find(ID const& Id) { return FindInternal<ID const&>(Id); }
	inline ID const& GetCurrentId()const { return pCurrent->GetId(); }
	inline CHAR const* GetCurrentItem()const { return pCurrent->GetItem(); }

	// Modification
	inline UINT SetCurrentItem(CHAR const* Item, UINT Length=0) { return pCurrent->SetItem(Item, Length); }
};

// Iterator Index with Shared Strings Read-Only
template <class ID, class CHAR, UINT _GroupSize>
class IndexIterator<ID, String<CHAR, false>, _GroupSize, true>: public IndexIteratorBase<ID, String<CHAR, false>, _GroupSize, true>
{
private:
	// Using
	using IT_R=IndexIterator<ID, String<CHAR, false>, _GroupSize, true>;
	using IT_W=IndexIterator<ID, String<CHAR, false>, _GroupSize, false>;
	using CLUSTER=::Clusters::Details::Cluster::Cluster<INDEXITEM, GROUP, ITEMGROUP, PARENTGROUP, IT_R, IT_W>;

public:
	// Con-/Destructors
	IndexIterator(IT_R const& It): IndexIteratorBase(It) {}
	IndexIterator(CLUSTER const* Cluster, UINT64 Position): IndexIteratorBase(Cluster) { SetPosition(Position); }
	IndexIterator(CLUSTER const* Cluster, UINT64, ID const& Id): IndexIteratorBase(Cluster) { FindInternal<ID const&>(Id); }

	// Access
	inline BOOL Find(ID const& Id) { return FindInternal<ID const&>(Id); }
	inline ID const& GetCurrentId()const { return pCurrent->GetId(); }
	inline CHAR const* GetCurrentItem()const { return pCurrent->GetItem(); }
};

// Iterator Index with Shared Strings Read-Write
template <class ID, class CHAR, UINT _GroupSize>
class IndexIterator<ID, String<CHAR, false>, _GroupSize, false>: public IndexIteratorBase<ID, String<CHAR, false>, _GroupSize, false>
{
private:
	// Using
	using IT_R=IndexIterator<ID, String<CHAR, false>, _GroupSize, true>;
	using IT_W=IndexIterator<ID, String<CHAR, false>, _GroupSize, false>;
	using CLUSTER=::Clusters::Details::Cluster::Cluster<INDEXITEM, GROUP, ITEMGROUP, PARENTGROUP, IT_R, IT_W>;

public:
	// Con-/Destructors
	IndexIterator(IT_W const& It): IndexIteratorBase(It) {}
	IndexIterator(CLUSTER* Cluster, UINT64 Position): IndexIteratorBase(Cluster) { SetPosition(Position); }
	IndexIterator(CLUSTER* Cluster, UINT64 Position, ID const& Id): IndexIteratorBase(Cluster) { FindInternal<ID const&>(Id); }

	// Access
	inline BOOL Find(ID const& Id) { return FindInternal<ID const&>(Id); }
	inline ID const& GetCurrentId()const { return pCurrent->GetId(); }
	inline CHAR const* GetCurrentItem()const { return pCurrent->GetItem(); }

	// Modification
	inline VOID SetCurrentItem(CHAR const* Item) { pCurrent->SetItem(Item); }
};

#pragma endregion


//========================
// Iterator Pointer-Index
//========================

#pragma region Pointer-Index

// Iterator Pointer-Index Read-Only
template <class ID, class ITEM, UINT _GroupSize>
class IndexIterator<ID*, ITEM, _GroupSize, true>: public IndexIteratorBase<ID*, ITEM, _GroupSize, true>
{
private:
	// Using
	using IT_R=IndexIterator<ID*, ITEM, _GroupSize, true>;
	using IT_W=IndexIterator<ID*, ITEM, _GroupSize, false>;
	using CLUSTER=::Clusters::Details::Cluster::Cluster<INDEXITEM, GROUP, ITEMGROUP, PARENTGROUP, IT_R, IT_W>;

public:
	// Con-/Destructors
	IndexIterator(IT_R const& It): IndexIteratorBase(It) {}
	IndexIterator(CLUSTER const* Cluster, UINT64 Position): IndexIteratorBase(Cluster) { SetPosition(Position); }
	IndexIterator(CLUSTER const* Cluster, UINT64, ID* Id): IndexIteratorBase(Cluster) { FindInternal(Id); }

	// Access
	inline BOOL Find(ID* Id) { return FindInternal(Id); }
	inline ID* GetCurrentId()const { return pCurrent->GetId(); }
	inline ITEM const& GetCurrentItem()const { return pCurrent->GetItem(); }
};

// Iterator Pointer-Index Read-Write
template <class ID, class ITEM, UINT _GroupSize>
class IndexIterator<ID*, ITEM, _GroupSize, false>: public IndexIteratorBase<ID*, ITEM, _GroupSize, false>
{
private:
	// Using
	using IT_R=IndexIterator<ID*, ITEM, _GroupSize, true>;
	using IT_W=IndexIterator<ID*, ITEM, _GroupSize, false>;
	using CLUSTER=::Clusters::Details::Cluster::Cluster<INDEXITEM, GROUP, ITEMGROUP, PARENTGROUP, IT_R, IT_W>;

public:
	// Con-/Destructors
	IndexIterator(IT_W const& It): IndexIteratorBase(It) {}
	IndexIterator(CLUSTER* Cluster, UINT64 Position): IndexIteratorBase(Cluster) { SetPosition(Position); }
	IndexIterator(CLUSTER* Cluster, UINT64, ID* Id): IndexIteratorBase(Cluster) { FindInternal(Id); }

	// Access
	inline BOOL Find(ID* Id) { return FindInternal(Id); }
	inline ID* GetCurrentId()const { return pCurrent->GetId(); }
	inline ITEM& GetCurrentItem()const { return pCurrent->GetItem(); }

	// Modification
	inline VOID SetCurrentItem(ITEM const& Item) { pCurrent->SetItem(Item); }
};

// Iterator Pointer-Index without Items Read-Only
template <class ID, UINT _GroupSize>
class IndexIterator<ID*, VOID, _GroupSize, true>: public IndexIteratorBase<ID*, VOID, _GroupSize, true>
{
private:
	// Using
	using IT_R=IndexIterator<ID*, VOID, _GroupSize, true>;
	using IT_W=IndexIterator<ID*, VOID, _GroupSize, false>;
	using CLUSTER=::Clusters::Details::Cluster::Cluster<INDEXITEM, GROUP, ITEMGROUP, PARENTGROUP, IT_R, IT_W>;

public:
	// Con-/Destructors
	IndexIterator(IT_R const& It): IndexIteratorBase(It) {}
	IndexIterator(CLUSTER const* Cluster, UINT64 Position): IndexIteratorBase(Cluster) { SetPosition(Position); }
	IndexIterator(CLUSTER const* Cluster, UINT64, ID* Id): IndexIteratorBase(Cluster) { FindInternal(Id); }

	// Access
	inline BOOL Find(ID* Id) { return FindInternal(Id); }
	inline ID* GetCurrent()const { return pCurrent->GetId(); }
};

// Iterator Pointer-Index without Items Read-Write
template <class ID, UINT _GroupSize>
class IndexIterator<ID*, VOID, _GroupSize, false>: public IndexIteratorBase<ID*, VOID, _GroupSize, false>
{
private:
	// Using
	using IT_R=IndexIterator<ID*, VOID, _GroupSize, true>;
	using IT_W=IndexIterator<ID*, VOID, _GroupSize, false>;
	using CLUSTER=::Clusters::Details::Cluster::Cluster<INDEXITEM, GROUP, ITEMGROUP, PARENTGROUP, IT_R, IT_W>;

public:
	// Con-/Destructors
	IndexIterator(IT_W const& It): IndexIteratorBase(It) {}
	IndexIterator(CLUSTER* Cluster, UINT64 Position): IndexIteratorBase(Cluster) { SetPosition(Position); }
	IndexIterator(CLUSTER* Cluster, UINT64, ID* Id): IndexIteratorBase(Cluster) { FindInternal(Id); }

	// Access
	inline BOOL Find(ID* Id) { return FindInternal(Id); }
	inline ID* GetCurrent()const { return pCurrent->GetId(); }
};

// Iterator Pointer-Index with Pointers Read-Only
template <class ID, class ITEM, UINT _GroupSize>
class IndexIterator<ID*, ITEM*, _GroupSize, true>: public IndexIteratorBase<ID*, ITEM*, _GroupSize, true>
{
private:
	// Using
	using IT_R=IndexIterator<ID*, ITEM*, _GroupSize, true>;
	using IT_W=IndexIterator<ID*, ITEM*, _GroupSize, false>;
	using CLUSTER=::Clusters::Details::Cluster::Cluster<INDEXITEM, GROUP, ITEMGROUP, PARENTGROUP, IT_R, IT_W>;

public:
	// Con-/Destructors
	IndexIterator(IT_R const& It): IndexIteratorBase(It) {}
	IndexIterator(CLUSTER const* Cluster, UINT64 Position): IndexIteratorBase(Cluster) { SetPosition(Position); }
	IndexIterator(CLUSTER const* Cluster, UINT64, ID* Id): IndexIteratorBase(Cluster) { FindInternal(Id); }

	// Access
	inline BOOL Find(ID* Id) { return FindInternal(Id); }
	inline ID* GetCurrentId()const { return pCurrent->GetId(); }
	inline ITEM* GetCurrentItem()const { return pCurrent->GetItem(); }
};

// Iterator Pointer-Index with Pointers Read-Write
template <class ID, class ITEM, UINT _GroupSize>
class IndexIterator<ID*, ITEM*, _GroupSize, false>: public IndexIteratorBase<ID*, ITEM*, _GroupSize, false>
{
private:
	// Using
	using IT_R=IndexIterator<ID*, ITEM*, _GroupSize, true>;
	using IT_W=IndexIterator<ID*, ITEM*, _GroupSize, false>;
	using CLUSTER=::Clusters::Details::Cluster::Cluster<INDEXITEM, GROUP, ITEMGROUP, PARENTGROUP, IT_R, IT_W>;

public:
	// Con-/Destructors
	IndexIterator(IT_W const& It): IndexIteratorBase(It) {}
	IndexIterator(CLUSTER* Cluster, UINT64 Position): IndexIteratorBase(Cluster) { SetPosition(Position); }
	IndexIterator(CLUSTER* Cluster, UINT64, ID* Id): IndexIteratorBase(Cluster) { FindInternal(Id); }

	// Access
	inline BOOL Find(ID* Id) { return FindInternal(Id); }
	inline ID* GetCurrentId()const { return pCurrent->GetId(); }
	inline ITEM* GetCurrentItem()const { return pCurrent->GetItem(); }

	// Modification
	inline VOID SetCurrentItem(ITEM* Item) { pCurrent->SetItem(Item); }
};

#ifdef __cplusplus_winrt
// Iterator Pointer-Index with Handles Read-Only
template <class ID, class ITEM, UINT _GroupSize>
class IndexIterator<ID*, ITEM^, _GroupSize, true>: public IndexIteratorBase<ID*, ITEM^, _GroupSize, true>
{
private:
	// Using
	using IT_R=IndexIterator<ID*, ITEM^, _GroupSize, true>;
	using IT_W=IndexIterator<ID*, ITEM^, _GroupSize, false>;
	using CLUSTER=::Clusters::Details::Cluster::Cluster<INDEXITEM, GROUP, ITEMGROUP, PARENTGROUP, IT_R, IT_W>;

public:
	// Con-/Destructors
	IndexIterator(IT_R const& It): IndexIteratorBase(It) {}
	IndexIterator(CLUSTER const* Cluster, UINT64 Position): IndexIteratorBase(Cluster) { SetPosition(Position); }
	IndexIterator(CLUSTER const* Cluster, UINT64, ID* Id): IndexIteratorBase(Cluster) { FindInternal(Id); }

	// Access
	inline BOOL Find(ID* Id) { return FindInternal(Id); }
	inline ID* GetCurrentId()const { return pCurrent->GetId(); }
	inline ITEM^ GetCurrentItem()const { return pCurrent->GetItem(); }
};
#endif

#ifdef __cplusplus_winrt
// Iterator Pointer-Index with Handles Read-Write
template <class ID, class ITEM, UINT _GroupSize>
class IndexIterator<ID*, ITEM^, _GroupSize, false>: public IndexIteratorBase<ID*, ITEM^, _GroupSize, false>
{
private:
	// Using
	using IT_R=IndexIterator<ID*, ITEM^, _GroupSize, true>;
	using IT_W=IndexIterator<ID*, ITEM^, _GroupSize, false>;
	using CLUSTER=::Clusters::Details::Cluster::Cluster<INDEXITEM, GROUP, ITEMGROUP, PARENTGROUP, IT_R, IT_W>;

public:
	// Con-/Destructors
	IndexIterator(IT_W const& It): IndexIteratorBase(It) {}
	IndexIterator(CLUSTER* Cluster, UINT64 Position): IndexIteratorBase(Cluster) { SetPosition(Position); }
	IndexIterator(CLUSTER* Cluster, UINT64, ID* Id): IndexIteratorBase(Cluster) { FindInternal(Id); }

	// Access
	inline BOOL Find(ID* Id) { return FindInternal(Id); }
	inline ID* GetCurrentId()const { return pCurrent->GetId(); }
	inline ITEM^ GetCurrentItem()const { return pCurrent->GetItem(); }

	// Modification
	inline VOID SetCurrentItem(ITEM^ Item) { pCurrent->SetItem(Item); }
};
#endif

// Iterator Pointer-Index with Strings Read-Only
template <class ID, class CHAR, BOOL _AllocItem, UINT _GroupSize>
class IndexIterator<ID*, String<CHAR, _AllocItem>, _GroupSize, true>: public IndexIteratorBase<ID*, String<CHAR, _AllocItem>, _GroupSize, true>
{
private:
	// Using
	using IT_R=IndexIterator<ID*, String<CHAR, _AllocItem>, _GroupSize, true>;
	using IT_W=IndexIterator<ID*, String<CHAR, _AllocItem>, _GroupSize, false>;
	using CLUSTER=::Clusters::Details::Cluster::Cluster<INDEXITEM, GROUP, ITEMGROUP, PARENTGROUP, IT_R, IT_W>;

public:
	// Con-/Destructors
	IndexIterator(IT_R const& It): IndexIteratorBase(It) {}
	IndexIterator(CLUSTER const* Cluster, UINT64 Position): IndexIteratorBase(Cluster) { SetPosition(Position); }
	IndexIterator(CLUSTER const* Cluster, UINT64, ID* Id): IndexIteratorBase(Cluster) { FindInternal(Id); }

	// Access
	inline BOOL Find(ID* Id) { return FindInternal(Id); }
	inline ID* GetCurrentId()const { return pCurrent->GetId(); }
	inline CHAR const* GetCurrentItem()const { return pCurrent->GetItem(); }
};

// Iterator Pointer-Index with Strings Read-Write
template <class ID, class CHAR, UINT _GroupSize>
class IndexIterator<ID*, String<CHAR, true>, _GroupSize, false>: public IndexIteratorBase<ID*, String<CHAR, true>, _GroupSize, false>
{
private:
	// Using
	using IT_R=IndexIterator<ID*, String<CHAR, true>, _GroupSize, true>;
	using IT_W=IndexIterator<ID*, String<CHAR, true>, _GroupSize, false>;
	using CLUSTER=::Clusters::Details::Cluster::Cluster<INDEXITEM, GROUP, ITEMGROUP, PARENTGROUP, IT_R, IT_W>;

public:
	// Con-/Destructors
	IndexIterator(IT_W const& It): IndexIteratorBase(It) {}
	IndexIterator(CLUSTER* Cluster, UINT64 Position): IndexIteratorBase(Cluster) { SetPosition(Position); }
	IndexIterator(CLUSTER* Cluster, UINT64, ID* Id): IndexIteratorBase(Cluster) { FindInternal(Id); }

	// Access
	inline BOOL Find(ID* Id) { return FindInternal(Id); }
	inline ID* GetCurrentId()const { return pCurrent->GetId(); }
	inline CHAR const* GetCurrentItem()const { return pCurrent->GetItem(); }

	// Modification
	inline UINT SetCurrentItem(CHAR const* Item, UINT Length=0) { return pCurrent->SetItem(Item, Length); }
};

// Iterator Pointer-Index with Shared Strings Read-Only
template <class ID, class CHAR, UINT _GroupSize>
class IndexIterator<ID*, String<CHAR, false>, _GroupSize, true>: public IndexIteratorBase<ID*, String<CHAR, false>, _GroupSize, true>
{
private:
	// Using
	using IT_R=IndexIterator<ID*, String<CHAR, false>, _GroupSize, true>;
	using IT_W=IndexIterator<ID*, String<CHAR, false>, _GroupSize, false>;
	using CLUSTER=::Clusters::Details::Cluster::Cluster<INDEXITEM, GROUP, ITEMGROUP, PARENTGROUP, IT_R, IT_W>;

public:
	// Con-/Destructors
	IndexIterator(IT_R const& It): IndexIteratorBase(It) {}
	IndexIterator(CLUSTER const* Cluster, UINT64 Position): IndexIteratorBase(Cluster) { SetPosition(Position); }
	IndexIterator(CLUSTER const* Cluster, UINT64, ID* Id): IndexIteratorBase(Cluster) { FindInternal(Id); }

	// Access
	inline BOOL Find(ID* Id) { return FindInternal(Id); }
	inline ID* GetCurrentId()const { return pCurrent->GetId(); }
	inline CHAR const* GetCurrentItem()const { return pCurrent->GetItem(); }
};

// Iterator Pointer-Index with Shared Strings Read-Write
template <class ID, class CHAR, UINT _GroupSize>
class IndexIterator<ID*, String<CHAR, false>, _GroupSize, false>: public IndexIteratorBase<ID*, String<CHAR, false>, _GroupSize, false>
{
private:
	// Using
	using IT_R=IndexIterator<ID*, String<CHAR, false>, _GroupSize, true>;
	using IT_W=IndexIterator<ID*, String<CHAR, false>, _GroupSize, false>;
	using CLUSTER=::Clusters::Details::Cluster::Cluster<INDEXITEM, GROUP, ITEMGROUP, PARENTGROUP, IT_R, IT_W>;

public:
	// Con-/Destructors
	IndexIterator(IT_W const& It): IndexIteratorBase(It) {}
	IndexIterator(CLUSTER* Cluster, UINT64 Position): IndexIteratorBase(Cluster) { SetPosition(Position); }
	IndexIterator(CLUSTER* Cluster, UINT64, ID* Id): IndexIteratorBase(Cluster) { FindInternal(Id); }

	// Access
	inline BOOL Find(ID* Id) { return FindInternal(Id); }
	inline ID* GetCurrentId()const { return pCurrent->GetId(); }
	inline CHAR const* GetCurrentItem()const { return pCurrent->GetItem(); }

	// Modification
	inline VOID SetCurrentItem(CHAR const* Item) { pCurrent->SetItem(Item); }
};

#pragma endregion


//=======================
// Iterator Handle-Index
//=======================

#pragma region Handle-Index

#ifdef __cplusplus_winrt

// Iterator Handle-Index Read-Only
template <class ID, class ITEM, UINT _GroupSize>
class IndexIterator<ID^, ITEM, _GroupSize, true>: public IndexIteratorBase<ID^, ITEM, _GroupSize, true>
{
private:
	// Using
	using IT_R=IndexIterator<ID^, ITEM, _GroupSize, true>;
	using IT_W=IndexIterator<ID^, ITEM, _GroupSize, false>;
	using CLUSTER=::Clusters::Details::Cluster::Cluster<INDEXITEM, GROUP, ITEMGROUP, PARENTGROUP, IT_R, IT_W>;

public:
	// Con-/Destructors
	IndexIterator(IT_R const& It): IndexIteratorBase(It) {}
	IndexIterator(CLUSTER const* Cluster, UINT64 Position): IndexIteratorBase(Cluster) { SetPosition(Position); }
	IndexIterator(CLUSTER const* Cluster, ID^ Id): IndexIteratorBase(Cluster) { FindInternal(Id); }

	// Access
	inline BOOL Find(ID^ Id) { return FindInternal(Id); }
	inline ID^ GetCurrentId()const { return pCurrent->GetId(); }
	inline ITEM const& GetCurrentItem()const { return pCurrent->GetItem(); }
};

// Iterator Handle-Index Read-Write
template <class ID, class ITEM, UINT _GroupSize>
class IndexIterator<ID^, ITEM, _GroupSize, false>: public IndexIteratorBase<ID^, ITEM, _GroupSize, false>
{
private:
	// Using
	using IT_R=IndexIterator<ID^, ITEM, _GroupSize, true>;
	using IT_W=IndexIterator<ID^, ITEM, _GroupSize, false>;
	using CLUSTER=::Clusters::Details::Cluster::Cluster<INDEXITEM, GROUP, ITEMGROUP, PARENTGROUP, IT_R, IT_W>;

public:
	// Con-/Destructors
	IndexIterator(IT_W const& It): IndexIteratorBase(It) {}
	IndexIterator(CLUSTER* Cluster, UINT64 Position): IndexIteratorBase(Cluster) { SetPosition(Position); }
	IndexIterator(CLUSTER* Cluster, ID^ Id): IndexIteratorBase(Cluster) { FindInternal(Id); }

	// Access
	inline BOOL Find(ID^ Id) { return FindInternal(Id); }
	inline ID^ GetCurrentId()const { return pCurrent->GetId(); }
	inline ITEM& GetCurrentItem()const { return pCurrent->GetItem(); }

	// Modification
	inline VOID SetCurrentItem(ITEM const& Item) { pCurrent->SetItem(Item); }
};

// Iterator Handle-Index without Items
template <class ID, UINT _GroupSize>
class IndexIterator<ID^, VOID, _GroupSize, true>: public IndexIteratorBase<ID^, VOID, _GroupSize, true>
{
private:
	// Using
	using IT_R=IndexIterator<ID^, VOID, _GroupSize, true>;
	using IT_W=IndexIterator<ID^, VOID, _GroupSize, false>;
	using CLUSTER=::Clusters::Details::Cluster::Cluster<INDEXITEM, GROUP, ITEMGROUP, PARENTGROUP, IT_R, IT_W>;

public:
	// Con-/Destructors
	IndexIterator(IT_R const& It): IndexIteratorBase(It) {}
	IndexIterator(CLUSTER const* Cluster, UINT64 Position): IndexIteratorBase(Cluster) { SetPosition(Position); }
	IndexIterator(CLUSTER const* Cluster, ID^ Id): IndexIteratorBase(Cluster) { FindInternal(Id); }

	// Access
	inline BOOL Find(ID^ Id) { return FindInternal(Id); }
	inline ID^ GetCurrent()const { return pCurrent->GetId(); }
};

// Iterator Handle-Index without Items Read-Write
template <class ID, UINT _GroupSize>
class IndexIterator<ID^, VOID, _GroupSize, false>: public IndexIteratorBase<ID^, VOID, _GroupSize, false>
{
private:
	// Using
	using IT_R=IndexIterator<ID^, VOID, _GroupSize, true>;
	using IT_W=IndexIterator<ID^, VOID, _GroupSize, false>;
	using CLUSTER=::Clusters::Details::Cluster::Cluster<INDEXITEM, GROUP, ITEMGROUP, PARENTGROUP, IT_R, IT_W>;

public:
	// Con-/Destructors
	IndexIterator(IT_W const& It): IndexIteratorBase(It) {}
	IndexIterator(CLUSTER* Cluster, UINT64 Position): IndexIteratorBase(Cluster) { SetPosition(Position); }
	IndexIterator(CLUSTER* Cluster, ID^ Id): IndexIteratorBase(Cluster) { FindInternal(Id); }

	// Access
	inline BOOL Find(ID^ Id) { return FindInternal(Id); }
	inline ID^ GetCurrent()const { return pCurrent->GetId(); }
};

// Iterator Handle-Index with Pointers Read-Only
template <class ID, class ITEM, UINT _GroupSize>
class IndexIterator<ID^, ITEM*, _GroupSize, true>: public IndexIteratorBase<ID^, ITEM*, _GroupSize, true>
{
private:
	// Using
	using IT_R=IndexIterator<ID^, ITEM*, _GroupSize, true>;
	using IT_W=IndexIterator<ID^, ITEM*, _GroupSize, false>;
	using CLUSTER=::Clusters::Details::Cluster::Cluster<INDEXITEM, GROUP, ITEMGROUP, PARENTGROUP, IT_R, IT_W>;

public:
	// Con-/Destructors
	IndexIterator(IT_R const& It): IndexIteratorBase(It) {}
	IndexIterator(CLUSTER const* Cluster, UINT64 Position): IndexIteratorBase(Cluster) { SetPosition(Position); }
	IndexIterator(CLUSTER const* Cluster, ID^ Id): IndexIteratorBase(Cluster) { FindInternal(Id); }

	// Access
	inline BOOL Find(ID^ Id) { return FindInternal(Id); }
	inline ID^ GetCurrentId()const { return pCurrent->GetId(); }
	inline ITEM* GetCurrentItem()const { return pCurrent->GetItem(); }
};

// Iterator Handle-Index with Pointers Read-Write
template <class ID, class ITEM, UINT _GroupSize>
class IndexIterator<ID^, ITEM*, _GroupSize, false>: public IndexIteratorBase<ID^, ITEM*, _GroupSize, false>
{
private:
	// Using
	using IT_R=IndexIterator<ID^, ITEM*, _GroupSize, true>;
	using IT_W=IndexIterator<ID^, ITEM*, _GroupSize, false>;
	using CLUSTER=::Clusters::Details::Cluster::Cluster<INDEXITEM, GROUP, ITEMGROUP, PARENTGROUP, IT_R, IT_W>;

public:
	// Con-/Destructors
	IndexIterator(IT_W const& It): IndexIteratorBase(It) {}
	IndexIterator(CLUSTER* Cluster, UINT64 Position): IndexIteratorBase(Cluster) { SetPosition(Position); }
	IndexIterator(CLUSTER* Cluster, ID^ Id): IndexIteratorBase(Cluster) { FindInternal(Id); }

	// Access
	inline BOOL Find(ID^ Id) { return FindInternal(Id); }
	inline ID^ GetCurrentId()const { return pCurrent->GetId(); }
	inline ITEM* GetCurrentItem()const { return pCurrent->GetItem(); }

	// Modification
	inline VOID SetCurrentItem(ITEM* Item) { pCurrent->SetItem(Item); }
};

// Iterator Handle-Index with Handles Read-Only
template <class ID, class ITEM, UINT _GroupSize>
class IndexIterator<ID^, ITEM^, _GroupSize, true>: public IndexIteratorBase<ID^, ITEM^, _GroupSize, true>
{
private:
	// Using
	using IT_R=IndexIterator<ID^, ITEM^, _GroupSize, true>;
	using IT_W=IndexIterator<ID^, ITEM^, _GroupSize, false>;
	using CLUSTER=::Clusters::Details::Cluster::Cluster<INDEXITEM, GROUP, ITEMGROUP, PARENTGROUP, IT_R, IT_W>;

public:
	// Con-/Destructors
	IndexIterator(IT_R const& It): IndexIteratorBase(It) {}
	IndexIterator(CLUSTER const* Cluster, UINT64 Position): IndexIteratorBase(Cluster) { SetPosition(Position); }
	IndexIterator(CLUSTER const* Cluster, ID^ Id): IndexIteratorBase(Cluster) { FindInternal(Id); }

	// Access
	inline BOOL Find(ID^ Id) { return FindInternal(Id); }
	inline ID^ GetCurrentId()const { return pCurrent->GetId(); }
	inline ITEM^ GetCurrentItem()const { return pCurrent->GetItem(); }
};

// Iterator Handle-Index with Handles Read-Write
template <class ID, class ITEM, UINT _GroupSize>
class IndexIterator<ID^, ITEM^, _GroupSize, false>: public IndexIteratorBase<ID^, ITEM^, _GroupSize, false>
{
private:
	// Using
	using IT_R=IndexIterator<ID^, ITEM^, _GroupSize, true>;
	using IT_W=IndexIterator<ID^, ITEM^, _GroupSize, false>;
	using CLUSTER=::Clusters::Details::Cluster::Cluster<INDEXITEM, GROUP, ITEMGROUP, PARENTGROUP, IT_R, IT_W>;

public:
	// Con-/Destructors
	IndexIterator(IT_W const& It): IndexIteratorBase(It) {}
	IndexIterator(CLUSTER* Cluster, UINT64 Position): IndexIteratorBase(Cluster) { SetPosition(Position); }
	IndexIterator(CLUSTER* Cluster, ID^ Id): IndexIteratorBase(Cluster) { FindInternal(Id); }

	// Access
	inline BOOL Find(ID^ Id) { return FindInternal(Id); }
	inline ID^ GetCurrentId()const { return pCurrent->GetId(); }
	inline ITEM^ GetCurrentItem()const { return pCurrent->GetItem(); }

	// Modification
	inline VOID SetCurrentItem(ITEM^ Item) { pCurrent->SetItem(Item); }
};

// Iterator Handle-Index with Strings Read-Only
template <class ID, class CHAR, BOOL _AllocItem, UINT _GroupSize>
class IndexIterator<ID^, String<CHAR, _AllocItem>, _GroupSize, true>: public IndexIteratorBase<ID^, String<CHAR, _AllocItem>, _GroupSize, true>
{
private:
	// Using
	using IT_R=IndexIterator<ID^, String<CHAR, _AllocItem>, _GroupSize, true>;
	using IT_W=IndexIterator<ID^, String<CHAR, _AllocItem>, _GroupSize, false>;
	using CLUSTER=::Clusters::Details::Cluster::Cluster<INDEXITEM, GROUP, ITEMGROUP, PARENTGROUP, IT_R, IT_W>;

public:
	// Con-/Destructors
	IndexIterator(IT_R const& It): IndexIteratorBase(It) {}
	IndexIterator(CLUSTER const* Cluster, UINT64 Position): IndexIteratorBase(Cluster) { SetPosition(Position); }
	IndexIterator(CLUSTER const* Cluster, CHAR const* Id, UINT Length=0, BOOL CaseSensitive=false): IndexIteratorBase(Cluster) { FindInternal(Id, Length, CaseSensitive); }

	// Access
	inline BOOL Find(CHAR const* Id, UINT Length=0, BOOL CaseSensitive=false) { return FindInternal(Id, Length, CaseSensitive); }
	inline ID^ GetCurrentId()const { return pCurrent->GetId(); }
	inline CHAR const* GetCurrentItem()const { return pCurrent->GetItem(); }
};

// Iterator Handle-Index with Strings Read-Write
template <class ID, class CHAR, UINT _GroupSize>
class IndexIterator<ID^, String<CHAR, true>, _GroupSize, false>: public IndexIteratorBase<ID^, String<CHAR, true>, _GroupSize, false>
{
private:
	// Using
	using IT_R=IndexIterator<ID^, String<CHAR, true>, _GroupSize, true>;
	using IT_W=IndexIterator<ID^, String<CHAR, true>, _GroupSize, false>;
	using CLUSTER=::Clusters::Details::Cluster::Cluster<INDEXITEM, GROUP, ITEMGROUP, PARENTGROUP, IT_R, IT_W>;

public:
	// Con-/Destructors
	IndexIterator(IT_W const& It): IndexIteratorBase(It) {}
	IndexIterator(CLUSTER* Cluster, UINT64 Position): IndexIteratorBase(Cluster) { SetPosition(Position); }
	IndexIterator(CLUSTER* Cluster, ID^ Id): IndexIteratorBase(Cluster) { FindInternal(Id); }

	// Access
	inline BOOL Find(ID^ Id) { return FindInternal(Id); }
	inline ID^ GetCurrentId()const { return pCurrent->GetId(); }
	inline CHAR const* GetCurrentItem()const { return pCurrent->GetItem(); }

	// Modification
	inline UINT SetCurrentItem(CHAR const* Item, UINT Length=0) { return pCurrent->SetItem(Item, Length); }
};

// Iterator Handle-Index with Shared Strings Read-Only
template <class ID, class CHAR, UINT _GroupSize>
class IndexIterator<ID^, String<CHAR, false>, _GroupSize, true>: public IndexIteratorBase<ID^, String<CHAR, false>, _GroupSize, true>
{
private:
	// Using
	using IT_R=IndexIterator<ID^, String<CHAR, false>, _GroupSize, true>;
	using IT_W=IndexIterator<ID^, String<CHAR, false>, _GroupSize, false>;
	using CLUSTER=::Clusters::Details::Cluster::Cluster<INDEXITEM, GROUP, ITEMGROUP, PARENTGROUP, IT_R, IT_W>;

public:
	// Con-/Destructors
	IndexIterator(IT_R const& It): IndexIteratorBase(It) {}
	IndexIterator(CLUSTER const* Cluster, UINT64 Position): IndexIteratorBase(Cluster) { SetPosition(Position); }
	IndexIterator(CLUSTER const* Cluster, CHAR const* Id, UINT Length=0, BOOL CaseSensitive=false): IndexIteratorBase(Cluster) { FindInternal(Id, Length, CaseSensitive); }

	// Access
	inline BOOL Find(CHAR const* Id, UINT Length=0, BOOL CaseSensitive=false) { return FindInternal(Id, Length, CaseSensitive); }
	inline ID^ GetCurrentId()const { return pCurrent->GetId(); }
	inline CHAR const* GetCurrentItem()const { return pCurrent->GetItem(); }
};

// Iterator Handle-Index with Shared Strings Read-Write
template <class ID, class CHAR, UINT _GroupSize>
class IndexIterator<ID^, String<CHAR, false>, _GroupSize, false>: public IndexIteratorBase<ID^, String<CHAR, false>, _GroupSize, false>
{
private:
	// Using
	using IT_R=IndexIterator<ID^, String<CHAR, false>, _GroupSize, true>;
	using IT_W=IndexIterator<ID^, String<CHAR, false>, _GroupSize, false>;
	using CLUSTER=::Clusters::Details::Cluster::Cluster<INDEXITEM, GROUP, ITEMGROUP, PARENTGROUP, IT_R, IT_W>;

public:
	// Con-/Destructors
	IndexIterator(IT_W const& It): IndexIteratorBase(It) {}
	IndexIterator(CLUSTER* Cluster, UINT64 Position): IndexIteratorBase(Cluster) { SetPosition(Position); }
	IndexIterator(CLUSTER* Cluster, ID^ Id): IndexIteratorBase(Cluster) { FindInternal(Id); }

	// Access
	inline BOOL Find(ID^ Id) { return FindInternal(Id); }
	inline ID^ GetCurrentId()const { return pCurrent->GetId(); }
	inline CHAR const* GetCurrentItem()const { return pCurrent->GetItem(); }

	// Modification
	inline VOID SetCurrentItem(CHAR const* Item) { pCurrent->SetItem(Item); }
};

#endif

#pragma endregion


//=======================
// Iterator String-Index
//=======================

#pragma region String-Index

// Iterator String-Index Read-Only
template <class CHAR, BOOL _AllocId, class ITEM, UINT _GroupSize>
class IndexIterator<String<CHAR, _AllocId>, ITEM, _GroupSize, true>: public IndexIteratorBase<String<CHAR, true>, ITEM, _GroupSize, true>
{
private:
	// Using
	using IT_R=IndexIterator<String<CHAR, _AllocId>, ITEM, _GroupSize, true>;
	using IT_W=IndexIterator<String<CHAR, _AllocId>, ITEM, _GroupSize, false>;
	using CLUSTER=::Clusters::Details::Cluster::Cluster<INDEXITEM, GROUP, ITEMGROUP, PARENTGROUP, IT_R, IT_W>;

public:
	// Con-/Destructors
	IndexIterator(IT_R const& It): IndexIteratorBase(It) {}
	IndexIterator(CLUSTER const* Cluster, UINT64 Position): IndexIteratorBase(Cluster) { SetPosition(Position); }
	IndexIterator(CLUSTER const* Cluster, CHAR const* Id, UINT Length, BOOL CaseSensitive): IndexIteratorBase(Cluster) { FindInternal(Id, Length, CaseSensitive); }

	// Access
	inline BOOL Find(CHAR const* Id, UINT Length=0, BOOL CaseSensitive=false) { return FindInternal(Id, Length, CaseSensitive); }
	inline CHAR const* GetCurrentId()const { return pCurrent->GetId(); }
	inline ITEM const& GetCurrentItem()const { return pCurrent->GetItem(); }
};

// Iterator String-Index Read-Write
template <class CHAR, BOOL _AllocId, class ITEM, UINT _GroupSize>
class IndexIterator<String<CHAR, _AllocId>, ITEM, _GroupSize, false>: public IndexIteratorBase<String<CHAR, _AllocId>, ITEM, _GroupSize, false>
{
private:
	// Using
	using IT_R=IndexIterator<String<CHAR, _AllocId>, ITEM, _GroupSize, true>;
	using IT_W=IndexIterator<String<CHAR, _AllocId>, ITEM, _GroupSize, false>;
	using CLUSTER=::Clusters::Details::Cluster::Cluster<INDEXITEM, GROUP, ITEMGROUP, PARENTGROUP, IT_R, IT_W>;

public:
	// Con-/Destructors
	IndexIterator(IT_W const& It): IndexIteratorBase(It) {}
	IndexIterator(CLUSTER* Cluster, UINT64 Position): IndexIteratorBase(Cluster) { SetPosition(Position); }
	IndexIterator(CLUSTER* Cluster, CHAR const* Id, UINT Length, BOOL CaseSensitive): IndexIteratorBase(Cluster) { FindInternal(Id, Length, CaseSensitive); }

	// Access
	inline BOOL Find(CHAR const* Id, UINT Length=0, BOOL CaseSensitive=false) { return FindInternal(Id, Length, CaseSensitive); }
	inline CHAR const* GetCurrentId()const { return pCurrent->GetId(); }
	inline ITEM& GetCurrentItem()const { return pCurrent->GetItem(); }

	// Modification
	inline VOID SetCurrentItem(ITEM const& Item) { return pCurrent->SetItem(Item); }
};

// Iterator String-Index without Items Read-Only
template <class CHAR, BOOL _AllocId, UINT _GroupSize>
class IndexIterator<String<CHAR, _AllocId>, VOID, _GroupSize, true>: public IndexIteratorBase<String<CHAR, _AllocId>, VOID, _GroupSize, true>
{
private:
	// Using
	using IT_R=IndexIterator<String<CHAR, _AllocId>, VOID, _GroupSize, true>;
	using IT_W=IndexIterator<String<CHAR, _AllocId>, VOID, _GroupSize, false>;
	using CLUSTER=::Clusters::Details::Cluster::Cluster<INDEXITEM, GROUP, ITEMGROUP, PARENTGROUP, IT_R, IT_W>;

public:
	// Con-/Destructors
	IndexIterator(IT_R const& It): IndexIteratorBase(It) {}
	IndexIterator(CLUSTER const* Cluster, UINT64 Position): IndexIteratorBase(Cluster) { SetPosition(Position); }
	IndexIterator(CLUSTER const* Cluster, CHAR const* Id, UINT Length, BOOL CaseSensitive): IndexIteratorBase(Cluster) { FindInternal(Id, Length, CaseSensitive); }

	// Access
	inline BOOL Find(CHAR const* Id, UINT Length=0, BOOL CaseSensitive=false) { return FindInternal(Id, Length, CaseSensitive); }
	inline CHAR const* GetCurrent()const { return pCurrent->GetId(); }
};

// Iterator String-Index without Items Read-Write
template <class CHAR, BOOL _AllocId, UINT _GroupSize>
class IndexIterator<String<CHAR, _AllocId>, VOID, _GroupSize, false>: public IndexIteratorBase<String<CHAR, _AllocId>, VOID, _GroupSize, false>
{
private:
	// Using
	using IT_R=IndexIterator<String<CHAR, _AllocId>, VOID, _GroupSize, true>;
	using IT_W=IndexIterator<String<CHAR, _AllocId>, VOID, _GroupSize, false>;
	using CLUSTER=::Clusters::Details::Cluster::Cluster<INDEXITEM, GROUP, ITEMGROUP, PARENTGROUP, IT_R, IT_W>;

public:
	// Con-/Destructors
	IndexIterator(IT_W const& It): IndexIteratorBase(It) {}
	IndexIterator(CLUSTER* Cluster, UINT64 Position): IndexIteratorBase(Cluster) { SetPosition(Position); }
	IndexIterator(CLUSTER* Cluster, CHAR const* Id, UINT Length, BOOL CaseSensitive): IndexIteratorBase(Cluster) { FindInternal(Id, Length, CaseSensitive); }

	// Access
	inline BOOL Find(CHAR const* Id, UINT Length=0, BOOL CaseSensitive=false) { return FindInternal(Id, Length, CaseSensitive); }
	inline CHAR const* GetCurrent()const { return pCurrent->GetId(); }
};

// Iterator String-Index with Pointers Read-Only
template <class CHAR, BOOL _AllocId, class ITEM, UINT _GroupSize>
class IndexIterator<String<CHAR, _AllocId>, ITEM*, _GroupSize, true>: public IndexIteratorBase<String<CHAR, _AllocId>, ITEM*, _GroupSize, true>
{
private:
	// Using
	using IT_R=IndexIterator<String<CHAR, _AllocId>, ITEM*, _GroupSize, true>;
	using IT_W=IndexIterator<String<CHAR, _AllocId>, ITEM*, _GroupSize, false>;
	using CLUSTER=::Clusters::Details::Cluster::Cluster<INDEXITEM, GROUP, ITEMGROUP, PARENTGROUP, IT_R, IT_W>;

public:
	// Con-/Destructors
	IndexIterator(IT_R const& It): IndexIteratorBase(It) {}
	IndexIterator(CLUSTER const* Cluster, UINT64 Position): IndexIteratorBase(Cluster) { SetPosition(Position); }
	IndexIterator(CLUSTER const* Cluster, CHAR const* Id, UINT Length, BOOL CaseSensitive): IndexIteratorBase(Cluster) { FindInternal(Id, Length, CaseSensitive); }

	// Access
	inline BOOL Find(CHAR const* Id, UINT Length=0, BOOL CaseSensitive=false) { return FindInternal(Id, Length, CaseSensitive); }
	inline CHAR const* GetCurrentId()const { return pCurrent->GetId(); }
	inline ITEM* GetCurrentItem()const { return pCurrent->GetItem(); }
};

// Iterator String-Index with Pointers Read-Write
template <class CHAR, BOOL _AllocId, class ITEM, UINT _GroupSize>
class IndexIterator<String<CHAR, _AllocId>, ITEM*, _GroupSize, false>: public IndexIteratorBase<String<CHAR, _AllocId>, ITEM*, _GroupSize, false>
{
private:
	// Using
	using IT_R=IndexIterator<String<CHAR, _AllocId>, ITEM*, _GroupSize, true>;
	using IT_W=IndexIterator<String<CHAR, _AllocId>, ITEM*, _GroupSize, false>;
	using CLUSTER=::Clusters::Details::Cluster::Cluster<INDEXITEM, GROUP, ITEMGROUP, PARENTGROUP, IT_R, IT_W>;

public:
	// Con-/Destructors
	IndexIterator(IT_W const& It): IndexIteratorBase(It) {}
	IndexIterator(CLUSTER* Cluster, UINT64 Position): IndexIteratorBase(Cluster) { SetPosition(Position); }
	IndexIterator(CLUSTER* Cluster, CHAR const* Id, UINT Length, BOOL CaseSensitive): IndexIteratorBase(Cluster) { FindInternal(Id, Length, CaseSensitive); }

	// Access
	inline BOOL Find(CHAR const* Id, UINT Length=0, BOOL CaseSensitive=false) { return FindInternal(Id, Length, CaseSensitive); }
	inline CHAR const* GetCurrentId()const { return pCurrent->GetId(); }
	inline ITEM* GetCurrentItem()const { return pCurrent->GetItem(); }

	// Modification
	inline VOID SetCurrentItem(ITEM* Item) { return pCurrent->SetItem(Item); }
};

#ifdef __cplusplus_winrt
// Iterator String-Index with Handles Read-Only
template <class CHAR, BOOL _AllocId, class ITEM, UINT _GroupSize>
class IndexIterator<String<CHAR, _AllocId>, ITEM^, _GroupSize, true>: public IndexIteratorBase<String<CHAR, _AllocId>, ITEM^, _GroupSize, true>
{
private:
	// Using
	using IT_R=IndexIterator<String<CHAR, _AllocId>, ITEM^, _GroupSize, true>;
	using IT_W=IndexIterator<String<CHAR, _AllocId>, ITEM^, _GroupSize, false>;
	using CLUSTER=::Clusters::Details::Cluster::Cluster<INDEXITEM, GROUP, ITEMGROUP, PARENTGROUP, IT_R, IT_W>;

public:
	// Con-/Destructors
	IndexIterator(IT_R const& It): IndexIteratorBase(It) {}
	IndexIterator(CLUSTER const* Cluster, UINT64 Position): IndexIteratorBase(Cluster) { SetPosition(Position); }
	IndexIterator(CLUSTER const* Cluster, CHAR const* Id, UINT Length, BOOL CaseSensitive): IndexIteratorBase(Cluster) { FindInternal(Id, Length, CaseSensitive); }

	// Access
	inline BOOL Find(CHAR const* Id, UINT Length=0, BOOL CaseSensitive=false) { return FindInternal(Id, Length, CaseSensitive); }
	inline CHAR const* GetCurrentId()const { return pCurrent->GetId(); }
	inline ITEM^ GetCurrentItem()const { return pCurrent->GetItem(); }
};
#endif

#ifdef __cplusplus_winrt
// Iterator String-Index with Handles Read-Write
template <class CHAR, BOOL _AllocId, class ITEM, UINT _GroupSize>
class IndexIterator<String<CHAR, _AllocId>, ITEM^, _GroupSize, false>: public IndexIteratorBase<String<CHAR, _AllocId>, ITEM^, _GroupSize, false>
{
private:
	// Using
	using IT_R=IndexIterator<String<CHAR, _AllocId>, ITEM^, _GroupSize, true>;
	using IT_W=IndexIterator<String<CHAR, _AllocId>, ITEM^, _GroupSize, false>;
	using CLUSTER=::Clusters::Details::Cluster::Cluster<INDEXITEM, GROUP, ITEMGROUP, PARENTGROUP, IT_R, IT_W>;

public:
	// Con-/Destructors
	IndexIterator(IT_W const& It): IndexIteratorBase(It) {}
	IndexIterator(CLUSTER* Cluster, UINT64 Position): IndexIteratorBase(Cluster) { SetPosition(Position); }
	IndexIterator(CLUSTER* Cluster, CHAR const* Id, UINT Length, BOOL CaseSensitive): IndexIteratorBase(Cluster) { FindInternal(Id, Length, CaseSensitive); }

	// Access
	inline BOOL Find(CHAR const* Id, UINT Length=0, BOOL CaseSensitive=false) { return FindInternal(Id, Length, CaseSensitive); }
	inline CHAR const* GetCurrentId()const { return pCurrent->GetId(); }
	inline ITEM^ GetCurrentItem()const { return pCurrent->GetItem(); }

	// Modification
	inline VOID SetCurrentItem(ITEM^ Item) { return pCurrent->SetItem(Item); }
};
#endif

// Iterator String-Index with Strings Read-Only
template <class CHAR_ID, BOOL _AllocId, class CHAR_ITEM, UINT _GroupSize>
class IndexIterator<String<CHAR_ID, _AllocId>, String<CHAR_ITEM, true>, _GroupSize, true>: public IndexIteratorBase<String<CHAR_ID, _AllocId>, String<CHAR_ITEM, true>, _GroupSize, true>
{
private:
	// Using
	using IT_R=IndexIterator<String<CHAR_ID, _AllocId>, String<CHAR_ITEM, true>, _GroupSize, true>;
	using IT_W=IndexIterator<String<CHAR_ID, _AllocId>, String<CHAR_ITEM, true>, _GroupSize, false>;
	using CLUSTER=::Clusters::Details::Cluster::Cluster<INDEXITEM, GROUP, ITEMGROUP, PARENTGROUP, IT_R, IT_W>;

public:
	// Con-/Destructors
	IndexIterator(IT_R const& It): IndexIteratorBase(It) {}
	IndexIterator(CLUSTER const* Cluster, UINT64 Position): IndexIteratorBase(Cluster) { SetPosition(Position); }
	IndexIterator(CLUSTER const* Cluster, CHAR_ID const* Id, UINT Length, BOOL CaseSensitive): IndexIteratorBase(Cluster) { FindInternal(Id, Length, CaseSensitive); }

	// Access
	inline BOOL Find(CHAR_ID const* Id, UINT Length=0, BOOL CaseSensitive=false) { return FindInternal(Id, Length, CaseSensitive); }
	inline CHAR_ID const* GetCurrentId()const { return pCurrent->GetId(); }
	inline CHAR_ITEM const* GetCurrentItem()const { return pCurrent->GetItem(); }
};

// Iterator String-Index with Strings Read-Write
template <class CHAR_ID, BOOL _AllocId, class CHAR_ITEM, UINT _GroupSize>
class IndexIterator<String<CHAR_ID, _AllocId>, String<CHAR_ITEM, true>, _GroupSize, false>: public IndexIteratorBase<String<CHAR_ID, _AllocId>, String<CHAR_ITEM, true>, _GroupSize, false>
{
private:
	// Using
	using IT_R=IndexIterator<String<CHAR_ID, _AllocId>, String<CHAR_ITEM, true>, _GroupSize, true>;
	using IT_W=IndexIterator<String<CHAR_ID, _AllocId>, String<CHAR_ITEM, true>, _GroupSize, false>;
	using CLUSTER=::Clusters::Details::Cluster::Cluster<INDEXITEM, GROUP, ITEMGROUP, PARENTGROUP, IT_R, IT_W>;

public:
	// Con-/Destructors
	IndexIterator(IT_W const& It): IndexIteratorBase(It) {}
	IndexIterator(CLUSTER* Cluster, UINT64 Position): IndexIteratorBase(Cluster) { SetPosition(Position); }
	IndexIterator(CLUSTER* Cluster, CHAR_ID const* Id, UINT Length, BOOL CaseSensitive): IndexIteratorBase(Cluster) { FindInternal(Id, Length, CaseSensitive); }

	// Access
	inline BOOL Find(CHAR_ID const* Id, UINT Length=0, BOOL CaseSensitive=false) { return FindInternal(Id, Length, CaseSensitive); }
	inline CHAR_ID const* GetCurrentId()const { return pCurrent->GetId(); }
	inline CHAR_ITEM const* GetCurrentItem()const { return pCurrent->GetItem(); }

	// Modification
	inline UINT SetCurrentItem(CHAR_ITEM const* Value, UINT Length=0) { return pCurrent->SetItem(Value, Length); }
};

// Iterator String-Index with Shared Strings Read-Only
template <class CHAR_ID, BOOL _AllocId, class CHAR_ITEM, UINT _GroupSize>
class IndexIterator<String<CHAR_ID, _AllocId>, String<CHAR_ITEM, false>, _GroupSize, true>: public IndexIteratorBase<String<CHAR_ID, _AllocId>, String<CHAR_ITEM, false>, _GroupSize, true>
{
private:
	// Using
	using IT_R=IndexIterator<String<CHAR_ID, _AllocId>, String<CHAR_ITEM, false>, _GroupSize, true>;
	using IT_W=IndexIterator<String<CHAR_ID, _AllocId>, String<CHAR_ITEM, false>, _GroupSize, false>;
	using CLUSTER=::Clusters::Details::Cluster::Cluster<INDEXITEM, GROUP, ITEMGROUP, PARENTGROUP, IT_R, IT_W>;

public:
	// Con-/Destructors
	IndexIterator(IT_R const& It): IndexIteratorBase(It) {}
	IndexIterator(CLUSTER const* Cluster, UINT64 Position): IndexIteratorBase(Cluster) { SetPosition(Position); }
	IndexIterator(CLUSTER const* Cluster, CHAR_ID const* Id, UINT Length, BOOL CaseSensitive): IndexIteratorBase(Cluster) { FindInternal(Id, Length, CaseSensitive); }

	// Access
	inline BOOL Find(CHAR_ID const* Id, UINT Length=0, BOOL CaseSensitive=false) { return FindInternal(Id, Length, CaseSensitive); }
	inline CHAR_ID const* GetCurrentId()const { return pCurrent->GetId(); }
	inline CHAR_ITEM const* GetCurrentItem()const { return pCurrent->GetItem(); }
};

// Iterator String-Index with Shared Strings Read-Write
template <class CHAR_ID, BOOL _AllocId, class CHAR_ITEM, UINT _GroupSize>
class IndexIterator<String<CHAR_ID, _AllocId>, String<CHAR_ITEM, false>, _GroupSize, false>: public IndexIteratorBase<String<CHAR_ID, _AllocId>, String<CHAR_ITEM, false>, _GroupSize, false>
{
private:
	// Using
	using IT_R=IndexIterator<String<CHAR_ID, _AllocId>, String<CHAR_ITEM, false>, _GroupSize, true>;
	using IT_W=IndexIterator<String<CHAR_ID, _AllocId>, String<CHAR_ITEM, false>, _GroupSize, false>;
	using CLUSTER=::Clusters::Details::Cluster::Cluster<INDEXITEM, GROUP, ITEMGROUP, PARENTGROUP, IT_R, IT_W>;

public:
	// Con-/Destructors
	IndexIterator(IT_W const& It): IndexIteratorBase(It) {}
	IndexIterator(CLUSTER* Cluster, UINT64 Position): IndexIteratorBase(Cluster) { SetPosition(Position); }
	IndexIterator(CLUSTER* Cluster, CHAR_ID const* Id, UINT Length, BOOL CaseSensitive): IndexIteratorBase(Cluster) { FindInternal(Id, Length, CaseSensitive); }

	// Access
	inline BOOL Find(CHAR_ID const* Id, UINT Length=0, BOOL CaseSensitive=false) { return FindInternal(Id, Length, CaseSensitive); }
	inline CHAR_ID const* GetCurrentId()const { return pCurrent->GetId(); }
	inline CHAR_ITEM const* GetCurrentItem()const { return pCurrent->GetItem(); }

	// Modification
	inline VOID SetCurrentItem(CHAR_ITEM const* Value) { pCurrent->SetItem(Value); }
};

#pragma endregion

}}}
