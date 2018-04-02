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
	typedef IndexItem<ID, ITEM> Item;
	typedef IndexItemGroup<ID, ITEM, _GroupSize> ItemGroup;
	typedef IndexIterator<ID, ITEM, _GroupSize, true> IteratorReadOnly;
	typedef IndexIterator<ID, ITEM, _GroupSize, false> IteratorReadWrite;
	typedef IndexParentGroup<ID, ITEM, _GroupSize> ParentGroup;

protected:
	// Access
	template <class RET, class... PARAMS> RET GetInternal(PARAMS... Id)const
		{
		Item* pitem=pRoot->Get(Id...);
		if(!pitem)
			return nullptr;
		return pitem->GetItem();
		}
	template <class... PARAMS> ITEM* GetInternalAddress(PARAMS... Id)
		{
		Item* pitem=pRoot->Get(Id...);
		if(!pitem)
			return nullptr;
		return &pitem->GetItem();
		}
	template <class... PARAMS> ITEM const* GetInternalAddress(PARAMS... Id)const
		{
		Item* pitem=pRoot->Get(Id...);
		if(!pitem)
			return nullptr;
		return &pitem->GetItem();
		}

	// Modification
	template <class... PARAMS> Item* AddInternal(PARAMS... Id)
		{
		Item* pitem=nullptr;
		if(pRoot->Add(&pitem, Id..., false))
			return pitem;
		pRoot=new ParentGroup(pRoot);
		BOOL badded=pRoot->Add(&pitem, Id..., true);
		ASSERT(badded);
		return pitem;
		}
	template <class PTR> PTR ReleaseInternal(UINT64 Position)
		{
		PTR pitem=pRoot->ReleaseAt(Position);
		UpdateRoot();
		return pitem;
		}
	template <class... PARAMS> BOOL RemoveInternal(PARAMS... Id)
		{
		if(!pRoot->Remove(Id...))
			return false;
		UpdateRoot();
		return true;
		}
};

}}}
