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

template<class _Id, class _Item, unsigned int _GroupSize>
class IndexBase: public ::Clusters::Templates::Details::Cluster::Cluster<IndexItem<_Id, _Item>, IndexGroup<_Id, _Item>, IndexItemGroup<_Id, _Item, _GroupSize>, IndexParentGroup<_Id, _Item, _GroupSize>, IndexIterator<_Id, _Item, _GroupSize, true>, IndexIterator<_Id, _Item, _GroupSize, false>>
{
public:
	// Types
	typedef IndexIterator<_Id, _Item, _GroupSize, true> IteratorReadOnly;
	typedef IndexIterator<_Id, _Item, _GroupSize, false> IteratorReadWrite;

protected:
	// Using
	using _IndexItem=IndexItem<_Id, _Item>;
	using _Group=IndexGroup<_Id, _Item>;
	using _ItemGroup=IndexItemGroup<_Id, _Item, _GroupSize>;
	using _ParentGroup=IndexParentGroup<_Id, _Item, _GroupSize>;

	// Access
	template <class... _Params> inline bool ContainsInternal(_Params... Id)const { return pRoot->Contains(Id...); }
	template <class _Ret, class... _Params> inline _Ret GetInternal(_Params... Id)const { return pRoot->Get(Id...)->GetItem(); }
	template <class _Ret, class... _Params> _Ret GetInternalPointer(_Params... Id)const
		{
		auto pitem=pRoot->Get(Id...);
		if(pitem==nullptr)
			return nullptr;
		return pitem->GetItem();
		}
	template <class _Ret, class... _Params> bool TryGetInternal(_Params... Id, _Ret* Item)const
		{
		_IndexItem* pitem=pRoot->Get(Id...);
		if(!pitem)
			return false;
		*Item=pitem->GetItem();
		return true;
		}

	// Modification
	template <class _Id, class... _Item> bool AddInternal(_Id Id, _Item... Item)
		{
		_IndexItem* pitem=nullptr;
		if(!pRoot->Add(&pitem, Id, false))
			{
			pRoot=new _ParentGroup(pRoot);
			pRoot->Add(&pitem, Id, true);
			ASSERT(pitem!=nullptr);
			}
		else
			{
			if(pitem==nullptr)
				return false;
			}
		new (pitem) _IndexItem(Id, Item...);
		return true;
		}
	template <class _Char, class... _Item> bool AddStringInternal(_Char const* Id, unsigned int Length, bool CaseSensitive, _Item... Item)
		{
		_IndexItem* pitem=nullptr;
		if(!pRoot->Add(&pitem, Id, Length, CaseSensitive, false))
			{
			pRoot=new _ParentGroup(pRoot);
			pRoot->Add(&pitem, Id, Length, CaseSensitive, true);
			ASSERT(pitem!=nullptr);
			}
		else
			{
			if(pitem==nullptr)
				return false;
			}
		new (pitem) _IndexItem(Id, Length, false,  Item...);
		return true;
		}
	template <class... _Params> bool RemoveInternal(_Params... Id)
		{
		if(!pRoot->Remove(Id...))
			return false;
		UpdateRoot();
		return true;
		}
	template <class _Id, class... _Item> void SetInternal(_Id Id, _Item... Item)
		{
		_IndexItem* pitem=pRoot->Get(Id);
		if(pitem!=nullptr)
			{
			pitem->SetItem(Item...);
			return;
			}
		AddInternal<_Id, _Item...>(Id, Item...);
		}
	template <class _Char, class... _Item> void SetStringInternal(_Char const* Id, unsigned int Length, bool CaseSensitive, _Item... Item)
		{
		_IndexItem* pitem=pRoot->Get(Id, Length, CaseSensitive);
		if(pitem!=nullptr)
			{
			pitem->SetItem(Item...);
			return;
			}
		AddStringInternal<_Char, _Item...>(Id, Length, CaseSensitive, Item...);
		}
};

}}}}
