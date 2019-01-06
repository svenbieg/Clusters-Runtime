//=============
// IndexItemGroup.h
//=============

#pragma once


//=======
// Using
//=======

#include "../Cluster/ItemGroup.h"
#include "IndexGroup.h"


//===========
// Namespace
//===========

namespace Clusters {
	namespace Templates {
		namespace Details {
			namespace Index {


//=============================
// Base-Class Item-Group Index
//=============================

template <class _Id, class _Item, UINT _GroupSize>
class IndexItemGroupBase: public ::Clusters::Templates::Details::Cluster::ItemGroup<IndexItem<_Id, _Item>, IndexGroup<_Id, _Item>, _GroupSize>
{
private:
	// Using
	using _IndexItem=IndexItem<_Id, _Item>;
	using _ArrayHelper=ArrayHelper<_IndexItem, UINT>;

public:
	// Access
	inline _IndexItem* GetFirst()override { return _ArrayHelper::GetFirst(this->cItems.Get(), this->uItemCount); }
	inline _IndexItem const* GetFirst()const override { return _ArrayHelper::GetFirst(this->cItems.Get(), this->uItemCount); }
	inline _IndexItem* GetLast()override { return _ArrayHelper::GetLast(this->cItems.Get(), this->uItemCount); }
	inline _IndexItem const* GetLast()const override { return _ArrayHelper::GetLast(this->cItems.Get(), this->uItemCount); }

	// Modification
	inline VOID Append(_IndexItem const* Items, UINT Count, BOOL Moving) { _ArrayHelper::Append(this->cItems.Get(), _GroupSize, &this->uItemCount, Items, Count, Moving); }
	inline VOID InsertAt(UINT Position, _IndexItem const* Items, UINT Count, BOOL Moving) { _ArrayHelper::InsertAt(this->cItems.Get(), _GroupSize, &this->uItemCount, Position, Items, Count, Moving); }
	inline VOID RemoveAt(SIZE_T Position)override { _ArrayHelper::RemoveAt(this->cItems.Get(), &this->uItemCount, (UINT)Position); }
	inline VOID RemoveAt(UINT Position, UINT Count, BOOL Moving) { _ArrayHelper::RemoveAt(this->cItems.Get(), &this->uItemCount, Position, Count, Moving); }

protected:
	// Access
	template <class... _Params> _IndexItem* GetInternal(_Params... Id)
		{
		INT ipos=GetItemPos<_Params...>(Id...);
		if(ipos<0)
			return nullptr;
		return &this->cItems.Get()[ipos];
		}
	template <class... _Params> _IndexItem const* GetInternal(_Params... Id)const
		{
		INT ipos=GetItemPos<_Params...>(Id...);
		if(ipos<0)
			return nullptr;
		return &this->cItems.Get()[ipos];
		}
	template <class... _Params> INT GetItemPos(_Params... Id)const
		{
		if(!this->uItemCount)
			return -1;
		_IndexItem const* pitem=nullptr;
		UINT ustart=0;
		UINT uend=this->uItemCount;
		UINT uitem=0;
		while(ustart<uend)
			{
			uitem=ustart+(uend-ustart)/2;
			pitem=this->GetAt(uitem);
			if(this->IsAbove(pitem->GetId(), Id...))
				{
				uend=uitem;
				continue;
				}
			if(this->IsBelow(pitem->GetId(), Id...))
				{
				ustart=uitem+1;
				continue;
				}
			return uitem;
			}
		if(uitem>0&&this->IsAbove(pitem->GetId(), Id...))
			uitem--;
		return -(INT)uitem-1;
		}

	// Modification
	template <class... _Params> BOOL AddInternal(_IndexItem** Item, _Params... Id)
		{
		UINT u=GetInsertPos<_Params...>(Id...);
		if(u==_GroupSize+1)
			return true;
		if(this->uItemCount==_GroupSize)
			return false;
		*Item=_ArrayHelper::InsertAt(this->cItems.Get(), _GroupSize, &this->uItemCount, u);
		return true;
		}
	template <class... _Params> UINT GetInsertPos(_Params... Id)const
		{
		_IndexItem const* pitem=nullptr;
		UINT ustart=0;
		UINT uend=this->uItemCount;
		while(ustart<uend)
			{
			UINT u=ustart+(uend-ustart)/2;
			pitem=this->GetAt(u);
			if(this->IsAbove(pitem->GetId(), Id...))
				{
				uend=u;
				continue;
				}
			if(this->IsBelow(pitem->GetId(), Id...))
				{
				ustart=u+1;
				continue;
				}
			return _GroupSize+1;
			}
		return ustart;
		}
	template <class... _Params> BOOL RemoveInternal(_Params... Id)
		{
		INT ipos=GetItemPos<_Params...>(Id...);
		if(ipos<0)
			return false;
		_ArrayHelper::RemoveAt(this->cItems.Get(), &this->uItemCount, ipos);
		return true;
		}
};


//==================
// Item-Group Index
//==================

// Item-Group Index
template <class _Id, class _Item, UINT _GroupSize>
class IndexItemGroup: public IndexItemGroupBase<_Id, _Item, _GroupSize>
{
private:
	// Using
	using _IndexItem=IndexItem<_Id, _Item>;

public:
	// Access
	inline BOOL Contains(_Id const& Id)const override { return this->template GetItemPos<_Id const&>(Id)>=0; }
	inline INT Find(_Id const& Id)const override { return this->template GetItemPos<_Id const&>(Id); }
	inline _IndexItem* Get(_Id const& Id)override { return this->template GetInternal<_Id const&>(Id); }
	inline _IndexItem const* Get(_Id const& Id)const override { return this->template GetInternal<_Id const&>(Id); }

	// Modification
	inline BOOL Add(_IndexItem** Item, _Id const& Id, BOOL Again)override { return this->template AddInternal<_Id const&>(Item, Id); }
	inline BOOL Remove(_Id const& Id)override { return this->template RemoveInternal<_Id const&>(Id); }
};

// Item-Group Pointer-Index
template <class _Id, class _Item, UINT _GroupSize>
class IndexItemGroup<_Id*, _Item, _GroupSize>: public IndexItemGroupBase<_Id*, _Item, _GroupSize>
{
private:
	// Using
	using _IndexItem=IndexItem<_Id*, _Item>;

public:
	// Access
	inline BOOL Contains(_Id* Id)const override { return this->GetItemPos(Id)>=0; }
	inline INT Find(_Id* Id)const override { return this->GetItemPos(Id); }
	inline _IndexItem* Get(_Id* Id)override { return this->GetInternal(Id); }
	inline _IndexItem const* Get(_Id* Id)const override { return this->GetInternal(Id); }

	// Modification
	inline BOOL Add(_IndexItem** Item, _Id* Id, BOOL Again)override { return this->AddInternal(Item, Id); }
	inline BOOL Remove(_Id* Id)override { return this->RemoveInternal(Id); }
};

#ifdef __cplusplus_winrt
// Item-Group Handle-Index
template <class _Id, class _Item, UINT _GroupSize>
class IndexItemGroup<_Id^, _Item, _GroupSize>: public IndexItemGroupBase<_Id^, _Item, _GroupSize>
{
private:
	// Using
	using _IndexItem=IndexItem<_Id^, _Item>;

public:
	// Access
	inline BOOL Contains(_Id^ Id)const override { return this->GetItemPos(Id)>=0; }
	inline INT Find(_Id^ Id)const override { return this->GetItemPos(Id); }
	inline _IndexItem* Get(_Id^ Id)override { return this->GetInternal(Id); }
	inline _IndexItem const* Get(_Id^ Id)const override { return this->GetInternal(Id); }

	// Modification
	inline BOOL Add(_IndexItem** Item, _Id^ Id, BOOL Again)override { return this->AddInternal(Item, Id); }
	inline BOOL Remove(_Id^ Id)override { return this->RemoveInternal(Id); }
};
#endif

// Item-Group String-Index
template <class _Char, BOOL _AllocId, class _Item, UINT _GroupSize>
class IndexItemGroup<String<_Char, _AllocId>, _Item, _GroupSize>: public IndexItemGroupBase<String<_Char, _AllocId>, _Item, _GroupSize>
{
private:
	// Using
	using _IndexItem=IndexItem<String<_Char, _AllocId>, _Item>;

public:
	// Access
	inline BOOL Contains(_Char const* Id, UINT Length, BOOL CaseSensitive)const override { return this->GetItemPos(Id, Length, CaseSensitive)>=0; }
	inline INT Find(_Char const* Id, UINT Length, BOOL CaseSensitive)const override { return this->GetItemPos(Id, Length, CaseSensitive); }
	inline _IndexItem* Get(_Char const* Id, UINT Length, BOOL CaseSensitive)override { return this->GetInternal(Id, Length, CaseSensitive); }
	inline _IndexItem const* Get(_Char const* Id, UINT Length, BOOL CaseSensitive)const override { return this->GetInternal(Id, Length, CaseSensitive); }

	// Modification
	inline BOOL Add(_IndexItem** Item, _Char const* Id, UINT Length, BOOL CaseSensitive, BOOL Again)override { return this->AddInternal(Item, Id, Length, CaseSensitive); }
	inline BOOL Remove(_Char const* Id, UINT Length, BOOL CaseSensitive)override { return this->RemoveInternal(Id, Length, CaseSensitive); }
};

}}}}
