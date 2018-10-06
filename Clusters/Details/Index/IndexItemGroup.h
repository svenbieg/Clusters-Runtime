//=============
// IndexItemGroup.h
//=============

#pragma once


//=======
// Using
//=======

#include "..\Cluster\ItemGroup.h"
#include "IndexGroup.h"


//===========
// Namespace
//===========

namespace Clusters {
	namespace Details {
		namespace Index {


//=============================
// Base-Class Item-Group Index
//=============================

template <class ID, class ITEM, UINT _GroupSize>
class IndexItemGroupBase: public ::Clusters::Details::Cluster::ItemGroup<IndexItem<ID, ITEM>, IndexGroup<ID, ITEM>, _GroupSize>
{
protected:
	// Using
	using INDEXITEM=IndexItem<ID, ITEM>;

private:
	// Using
	using ARRAYHELPER=ArrayHelper<IndexItem<ID, ITEM>, UINT>;
	using BASE=::Clusters::Details::Cluster::ItemGroup<INDEXITEM, IndexGroup<ID, ITEM>, _GroupSize>;

public:
	// Con-/Destructors
	using BASE::BASE;

	// Access
	inline INDEXITEM* GetFirst()override { return ARRAYHELPER::GetFirst(cItems.Get(), uItemCount); }
	inline INDEXITEM const* GetFirst()const override { return ARRAYHELPER::GetFirst(cItems.Get(), uItemCount); }
	inline INDEXITEM* GetLast()override { return ARRAYHELPER::GetLast(cItems.Get(), uItemCount); }
	inline INDEXITEM const* GetLast()const override { return ARRAYHELPER::GetLast(cItems.Get(), uItemCount); }

	// Modification
	inline VOID Append(INDEXITEM const* Items, UINT Count, BOOL CopyOnly) { ARRAYHELPER::Append(cItems.Get(), _GroupSize, &uItemCount, Items, Count, CopyOnly); }
	inline VOID InsertAt(UINT Position, INDEXITEM const* Items, UINT Count, BOOL CopyOnly) { ARRAYHELPER::InsertAt(cItems.Get(), _GroupSize, &uItemCount, Position, Items, Count, CopyOnly); }
	inline VOID RemoveAt(UINT Position, UINT Count, BOOL RemoveOnly) { ARRAYHELPER::RemoveAt(cItems.Get(), &uItemCount, Position, Count, RemoveOnly); }

protected:
	// Access
	template <class... PARAMS> INDEXITEM* GetInternal(PARAMS... Id)
		{
		INT ipos=GetItemPos<PARAMS...>(Id...);
		if(ipos<0)
			return nullptr;
		return &cItems.Get()[ipos];
		}
	template <class... PARAMS> INDEXITEM const* GetInternal(PARAMS... Id)const
		{
		INT ipos=GetItemPos<PARAMS...>(Id...);
		if(ipos<0)
			return nullptr;
		return &cItems.Get()[ipos];
		}
	template <class... PARAMS> INT GetItemPos(PARAMS... Id)const
		{
		if(!uItemCount)
			return -1;
		INDEXITEM const* pitem=nullptr;
		UINT ustart=0;
		UINT uend=uItemCount;
		UINT uitem=0;
		while(ustart<uend)
			{
			uitem=ustart+(uend-ustart)/2;
			pitem=GetAt(uitem);
			if(IsAbove(pitem->GetId(), Id...))
				{
				uend=uitem;
				continue;
				}
			if(IsBelow(pitem->GetId(), Id...))
				{
				ustart=uitem+1;
				continue;
				}
			return uitem;
			}
		if(uitem>0&&IsAbove(pitem->GetId(), Id...))
			uitem--;
		return -(INT)uitem-1;
		}

	// Modification
	template <class... PARAMS> BOOL AddInternal(INDEXITEM** Item, PARAMS... Id)
		{
		UINT u=GetInsertPos<PARAMS...>(Id...);
		if(u>uItemCount)
			return true;
		if(uItemCount+1>_GroupSize)
			return false;
		*Item=ARRAYHELPER::InsertAt(cItems.Get(), _GroupSize, &uItemCount, u);
		return true;
		}
	template <class... PARAMS> UINT GetInsertPos(PARAMS... Id)const
		{
		INDEXITEM const* pitem=nullptr;
		UINT ustart=0;
		UINT uend=uItemCount;
		while(ustart<uend)
			{
			UINT u=ustart+(uend-ustart)/2;
			pitem=GetAt(u);
			if(IsAbove(pitem->GetId(), Id...))
				{
				uend=u;
				continue;
				}
			if(IsBelow(pitem->GetId(), Id...))
				{
				ustart=u+1;
				continue;
				}
			return _GroupSize;
			}
		return ustart;
		}
	template <class... PARAMS> BOOL RemoveInternal(PARAMS... Id)
		{
		INT ipos=GetItemPos<PARAMS...>(Id...);
		if(ipos<0)
			return false;
		ARRAYHELPER::RemoveAt(cItems.Get(), &uItemCount, ipos);
		return true;
		}
};


//==================
// Item-Group Index
//==================

// Item-Group Index
template <class ID, class ITEM, UINT _GroupSize>
class IndexItemGroup: public IndexItemGroupBase<ID, ITEM, _GroupSize>
{
public:
	// Access
	inline BOOL Contains(ID const& Id)const override { return GetItemPos<ID const&>(Id)>=0; }
	inline INT Find(ID const& Id)const override { return GetItemPos<ID const&>(Id); }
	inline INDEXITEM* Get(ID const& Id)override { return GetInternal<ID const&>(Id); }
	inline INDEXITEM const* Get(ID const& Id)const override { return GetInternal<ID const&>(Id); }

	// Modification
	inline BOOL Add(INDEXITEM** Item, ID const& Id, BOOL Again)override { return AddInternal<ID const&>(Item, Id); }
	inline BOOL Remove(ID const& Id)override { return RemoveInternal<ID const&>(Id); }
};

// Item-Group Pointer-Index
template <class ID, class ITEM, UINT _GroupSize>
class IndexItemGroup<ID*, ITEM, _GroupSize>: public IndexItemGroupBase<ID*, ITEM, _GroupSize>
{
public:
	// Access
	inline BOOL Contains(ID* Id)const override { return GetItemPos(Id)>=0; }
	inline INT Find(ID* Id)const override { return GetItemPos(Id); }
	inline INDEXITEM* Get(ID* Id)override { return GetInternal(Id); }
	inline INDEXITEM const* Get(ID* Id)const override { return GetInternal(Id); }

	// Modification
	inline BOOL Add(INDEXITEM** Item, ID* Id, BOOL Again)override { return AddInternal(Item, Id); }
	inline BOOL Remove(ID* Id)override { return RemoveInternal(Id); }
};

#ifdef __cplusplus_winrt
// Item-Group Handle-Index
template <class ID, class ITEM, UINT _GroupSize>
class IndexItemGroup<ID^, ITEM, _GroupSize>: public IndexItemGroupBase<ID^, ITEM, _GroupSize>
{
public:
	// Access
	inline BOOL Contains(ID^ Id)const override { return GetItemPos(Id)>=0; }
	inline INT Find(ID^ Id)const override { return GetItemPos(Id); }
	inline INDEXITEM* Get(ID^ Id)override { return GetInternal(Id); }
	inline INDEXITEM const* Get(ID^ Id)const override { return GetInternal(Id); }

	// Modification
	inline BOOL Add(INDEXITEM** Item, ID^ Id, BOOL Again)override { return AddInternal(Item, Id); }
	inline BOOL Remove(ID^ Id)override { return RemoveInternal(Id); }
};
#endif

// Item-Group String-Index
template <class CHAR, BOOL _AllocId, class ITEM, UINT _GroupSize>
class IndexItemGroup<String<CHAR, _AllocId>, ITEM, _GroupSize>: public IndexItemGroupBase<String<CHAR, _AllocId>, ITEM, _GroupSize>
{
public:
	// Access
	inline BOOL Contains(CHAR const* Id, UINT Length, BOOL CaseSensitive)const override { return GetItemPos(Id, Length, CaseSensitive)>=0; }
	inline INT Find(CHAR const* Id, UINT Length, BOOL CaseSensitive)const override { return GetItemPos(Id, Length, CaseSensitive); }
	inline INDEXITEM* Get(CHAR const* Id, UINT Length, BOOL CaseSensitive)override { return GetInternal(Id, Length, CaseSensitive); }
	inline INDEXITEM const* Get(CHAR const* Id, UINT Length, BOOL CaseSensitive)const override { return GetInternal(Id, Length, CaseSensitive); }

	// Modification
	inline BOOL Add(INDEXITEM** Item, CHAR const* Id, UINT Length, BOOL CaseSensitive, BOOL Again)override { return AddInternal(Item, Id, Length, CaseSensitive); }
	inline BOOL Remove(CHAR const* Id, UINT Length, BOOL CaseSensitive)override { return RemoveInternal(Id, Length, CaseSensitive); }
};

}}}
