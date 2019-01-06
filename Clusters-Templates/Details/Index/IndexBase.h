//=============
// IndexBase.h
//=============

#pragma once


//=======
// Using
//=======

#include "../Cluster/Cluster.h"
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

template<class _Id, class _Item, UINT _GroupSize>
class IndexBase: public ::Clusters::Templates::Details::Cluster::Cluster<IndexItem<_Id, _Item>, IndexGroup<_Id, _Item>, IndexItemGroup<_Id, _Item, _GroupSize>, IndexParentGroup<_Id, _Item, _GroupSize>, IndexIterator<_Id, _Item, _GroupSize, true>, IndexIterator<_Id, _Item, _GroupSize, false>>
{
private:
	// Using
	using _IndexItem=IndexItem<_Id, _Item>;
	using _IndexGroup=IndexGroup<_Id, _Item>;
	using _IndexItemGroup=IndexItemGroup<_Id, _Item, _GroupSize>;
	using _IndexParentGroup=IndexParentGroup<_Id, _Item, _GroupSize>;
	using _ClusterBase=::Clusters::Templates::Details::Cluster::ClusterBase<_IndexGroup, _IndexItemGroup, _IndexParentGroup>;

protected:
	// Access
	template <class... _Params> inline BOOL ContainsInternal(_Params... Id)const { return this->pRoot->Contains(Id...); }
	template <class _Ret, class... _Params> inline _Ret GetInternal(_Params... Id)const { return this->pRoot->Get(Id...)->GetItem(); }
	template <class _Ret, class... _Params> _Ret GetInternalPointer(_Params... Id)const
		{
		auto pitem=this->pRoot->Get(Id...);
		if(pitem==nullptr)
			return nullptr;
		return pitem->GetItem();
		}
	template <class _Ret, class... _Params> BOOL TryGetInternal(_Params... Id, _Ret* Item)const
		{
		_IndexItem* pitem=this->pRoot->Get(Id...);
		if(!pitem)
			return false;
		*Item=pitem->GetItem();
		return true;
		}

	// Modification
	template <class __Id, class... __Item> BOOL AddInternal(__Id Id, __Item... Item)
		{
		_IndexItem* pitem=nullptr;
		if(!this->pRoot->Add(&pitem, Id, false))
			{
			this->pRoot=new _IndexParentGroup(this->pRoot);
			this->pRoot->Add(&pitem, Id, true);
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
	template <class _Char, class... __Item> BOOL AddStringInternal(_Char const* Id, UINT Length, BOOL CaseSensitive, __Item... Item)
		{
		_IndexItem* pitem=nullptr;
		if(!this->pRoot->Add(&pitem, Id, Length, CaseSensitive, false))
			{
			this->pRoot=new _IndexParentGroup(this->pRoot);
			this->pRoot->Add(&pitem, Id, Length, CaseSensitive, true);
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
	template <class... _Params> BOOL RemoveInternal(_Params... Id)
		{
		if(!this->pRoot->Remove(Id...))
			return false;
		this->UpdateRoot();
		return true;
		}
	template <class __Id, class... __Item> VOID SetInternal(__Id Id, __Item... Item)
		{
		_IndexItem* pitem=this->pRoot->Get(Id);
		if(pitem!=nullptr)
			{
			pitem->SetItem(Item...);
			return;
			}
		AddInternal<__Id, __Item...>(Id, Item...);
		}
	template <class _Char, class... __Item> VOID SetStringInternal(_Char const* Id, UINT Length, BOOL CaseSensitive, __Item... Item)
		{
		_IndexItem* pitem=this->pRoot->Get(Id, Length, CaseSensitive);
		if(pitem!=nullptr)
			{
			pitem->SetItem(Item...);
			return;
			}
		AddStringInternal<_Char, __Item...>(Id, Length, CaseSensitive, Item...);
		}
};

}}}}
