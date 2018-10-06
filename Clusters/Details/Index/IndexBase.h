//=============
// IndexBase.h
//=============

#pragma once


//=======
// Using
//=======

#include "..\Cluster\Cluster.h"
#include "IndexIterator.h"


//===========
// Namespace
//===========

namespace Clusters {
	namespace Details {
		namespace Index {


//==================
// Base-Class Index
//==================

template<class ID, class ITEM, UINT _GroupSize>
class IndexBase: public ::Clusters::Details::Cluster::Cluster<IndexItem<ID, ITEM>, IndexGroup<ID, ITEM>, IndexItemGroup<ID, ITEM, _GroupSize>, IndexParentGroup<ID, ITEM, _GroupSize>, IndexIterator<ID, ITEM, _GroupSize, true>, IndexIterator<ID, ITEM, _GroupSize, false>>
{
public:
	// Types
	typedef IndexIterator<ID, ITEM, _GroupSize, true> IteratorReadOnly;
	typedef IndexIterator<ID, ITEM, _GroupSize, false> IteratorReadWrite;

protected:
	// Using
	using INDEXITEM=IndexItem<ID, ITEM>;
	using GROUP=IndexGroup<ID, ITEM>;
	using ITEMGROUP=IndexItemGroup<ID, ITEM, _GroupSize>;
	using PARENTGROUP=IndexParentGroup<ID, ITEM, _GroupSize>;

	// Access
	template <class... PARAMS> BOOL ContainsInternal(PARAMS... Id)const
		{
		ScopedRead lock(cAccessControl);
		return pRoot->Contains(Id...);
		}
	template <class RET, class... PARAMS> RET GetInternal(PARAMS... Id)const
		{
		ScopedRead lock(cAccessControl);
		return pRoot->Get(Id...)->GetItem();
		}
	template <class RET, class... PARAMS> BOOL TryGetInternal(PARAMS... Id, RET* Item)const
		{
		ScopedRead lock(cAccessControl);
		INDEXITEM* pitem=pRoot->Get(Id...);
		if(!pitem)
			return false;
		*Item=pitem->GetItem();
		return true;
		}

	// Modification
	template <class ID, class... ITEM> inline BOOL AddInternal(ID Id, ITEM... Item)
		{
		ScopedWrite lock(cAccessControl);
		return DoAddInternal<ID, ITEM...>(Id, Item...);
		}
	template <class CHAR, class... ITEM> inline BOOL AddStringInternal(CHAR const* Id, UINT Length, BOOL CaseSensitive, ITEM... Item)
		{
		ScopedWrite lock(cAccessControl);
		return DoAddStringInternal<CHAR, ITEM...>(Id, Length, CaseSensitive, Item...);
		}
	template <class ID, class... ITEM> BOOL DoAddInternal(ID Id, ITEM... Item)
		{
		INDEXITEM* pitem=nullptr;
		if(!pRoot->Add(&pitem, Id, false))
			{
			pRoot=new PARENTGROUP(pRoot);
			pRoot->Add(&pitem, Id, true);
			ASSERT(pitem!=nullptr);
			}
		else
			{
			if(pitem==nullptr)
				return false;
			}
		new (pitem) INDEXITEM(Id, Item...);
		return true;
		}
	template <class CHAR, class... ITEM> BOOL DoAddStringInternal(CHAR const* Id, UINT Length, BOOL CaseSensitive, ITEM... Item)
		{
		INDEXITEM* pitem=nullptr;
		if(!pRoot->Add(&pitem, Id, Length, CaseSensitive, false))
			{
			pRoot=new PARENTGROUP(pRoot);
			pRoot->Add(&pitem, Id, Length, CaseSensitive, true);
			ASSERT(pitem!=nullptr);
			}
		else
			{
			if(pitem==nullptr)
				return false;
			}
		new (pitem) INDEXITEM(Id, Length, false,  Item...);
		return true;
		}
	template <class PTR> PTR ReleaseInternal(UINT64 Position)
		{
		ScopedWrite lock(cAccessControl);
		PTR pitem=pRoot->ReleaseAt(Position);
		UpdateRoot();
		return pitem;
		}
	template <class... PARAMS> BOOL RemoveInternal(PARAMS... Id)
		{
		ScopedWrite lock(cAccessControl);
		if(!pRoot->Remove(Id...))
			return false;
		UpdateRoot();
		return true;
		}
	template <class ID, class... ITEM> VOID SetInternal(ID Id, ITEM... Item)
		{
		ScopedWrite lock(cAccessControl);
		INDEXITEM* pitem=pRoot->Get(Id);
		if(pitem!=nullptr)
			{
			pitem->SetItem(Item...);
			return;
			}
		DoAddInternal<ID, ITEM...>(Id, Item...);
		}
	template <class CHAR, class... ITEM> VOID SetStringInternal(CHAR const* Id, UINT Length, BOOL CaseSensitive, ITEM... Item)
		{
		ScopedWrite lock(cAccessControl);
		INDEXITEM* pitem=pRoot->Get(Id, Length, CaseSensitive);
		if(pitem!=nullptr)
			{
			pitem->SetItem(Item...);
			return;
			}
		DoAddStringInternal<CHAR, ITEM...>(Id, Length, CaseSensitive, Item...);
		}
};

}}}
