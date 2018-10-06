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
	template <class RET, class... PARAMS> RET GetInternal(PARAMS... Id)const
		{
		INDEXITEM* pitem=pRoot->Get(Id...);
		if(!pitem)
			return nullptr;
		return pitem->GetItem();
		}
	template <class... PARAMS> ITEM* GetInternalAddress(PARAMS... Id)
		{
		INDEXITEM* pitem=pRoot->Get(Id...);
		if(!pitem)
			return nullptr;
		return &pitem->GetItem();
		}
	template <class... PARAMS> ITEM const* GetInternalAddress(PARAMS... Id)const
		{
		INDEXITEM* pitem=pRoot->Get(Id...);
		if(!pitem)
			return nullptr;
		return &pitem->GetItem();
		}

	// Modification
	template <class... PARAMS> INDEXITEM* AddInternal(PARAMS... Id)
		{
		INDEXITEM* pitem=nullptr;
		if(pRoot->Add(&pitem, Id..., false))
			return pitem;
		pRoot=new PARENTGROUP(pRoot);
		BOOL badded=pRoot->Add(&pitem, Id..., true);
		ASSERT(badded);
		return pitem;
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
};

}}}
