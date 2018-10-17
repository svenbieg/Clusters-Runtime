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
	namespace Templates {
		namespace Details {
			namespace Index {


//==================
// Base-Class Index
//==================

template<class ID, class ITEM, UINT _GroupSize>
class IndexBase: public ::Clusters::Templates::Details::Cluster::Cluster<IndexItem<ID, ITEM>, IndexGroup<ID, ITEM>, IndexItemGroup<ID, ITEM, _GroupSize>, IndexParentGroup<ID, ITEM, _GroupSize>, IndexIterator<ID, ITEM, _GroupSize, true>, IndexIterator<ID, ITEM, _GroupSize, false>>
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
	template <class... PARAMS> inline BOOL ContainsInternal(PARAMS... Id)const { return pRoot->Contains(Id...); }
	template <class RET, class... PARAMS> inline RET GetInternal(PARAMS... Id)const { return pRoot->Get(Id...)->GetItem(); }
	template <class RET, class... PARAMS> BOOL TryGetInternal(PARAMS... Id, RET* Item)const
		{
		INDEXITEM* pitem=pRoot->Get(Id...);
		if(!pitem)
			return false;
		*Item=pitem->GetItem();
		return true;
		}

	// Modification
	template <class ID, class... ITEM> BOOL AddInternal(ID Id, ITEM... Item)
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
	template <class CHAR, class... ITEM> BOOL AddStringInternal(CHAR const* Id, UINT Length, BOOL CaseSensitive, ITEM... Item)
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
	template <class... PARAMS> BOOL RemoveInternal(PARAMS... Id)
		{
		if(!pRoot->Remove(Id...))
			return false;
		UpdateRoot();
		return true;
		}
	template <class ID, class... ITEM> VOID SetInternal(ID Id, ITEM... Item)
		{
		INDEXITEM* pitem=pRoot->Get(Id);
		if(pitem!=nullptr)
			{
			pitem->SetItem(Item...);
			return;
			}
		AddInternal<ID, ITEM...>(Id, Item...);
		}
	template <class CHAR, class... ITEM> VOID SetStringInternal(CHAR const* Id, UINT Length, BOOL CaseSensitive, ITEM... Item)
		{
		INDEXITEM* pitem=pRoot->Get(Id, Length, CaseSensitive);
		if(pitem!=nullptr)
			{
			pitem->SetItem(Item...);
			return;
			}
		AddStringInternal<CHAR, ITEM...>(Id, Length, CaseSensitive, Item...);
		}
};

}}}}
