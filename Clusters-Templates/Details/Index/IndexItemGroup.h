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
	namespace Templates {
		namespace Details {
			namespace Index {


//=============================
// Base-Class Item-Group Index
//=============================

template <class _Id, class _Item, unsigned int _GroupSize>
class IndexItemGroupBase: public ::Clusters::Templates::Details::Cluster::ItemGroup<IndexItem<_Id, _Item>, IndexGroup<_Id, _Item>, _GroupSize>
{
protected:
	// Using
	using _IndexItem=IndexItem<_Id, _Item>;

private:
	// Using
	using _ArrayHelper=ArrayHelper<IndexItem<_Id, _Item>, unsigned int>;
	using _Base=::Clusters::Templates::Details::Cluster::ItemGroup<_IndexItem, IndexGroup<_Id, _Item>, _GroupSize>;

public:
	// Access
	inline _IndexItem* GetFirst()override { return _ArrayHelper::GetFirst(cItems.Get(), uItemCount); }
	inline _IndexItem const* GetFirst()const override { return _ArrayHelper::GetFirst(cItems.Get(), uItemCount); }
	inline _IndexItem* GetLast()override { return _ArrayHelper::GetLast(cItems.Get(), uItemCount); }
	inline _IndexItem const* GetLast()const override { return _ArrayHelper::GetLast(cItems.Get(), uItemCount); }

	// Modification
	inline void Append(_IndexItem const* Items, unsigned int Count, bool CopyOnly) { _ArrayHelper::Append(cItems.Get(), _GroupSize, &uItemCount, Items, Count, CopyOnly); }
	inline void InsertAt(unsigned int Position, _IndexItem const* Items, unsigned int Count, bool CopyOnly) { _ArrayHelper::InsertAt(cItems.Get(), _GroupSize, &uItemCount, Position, Items, Count, CopyOnly); }
	inline void RemoveAt(unsigned int Position, unsigned int Count, bool RemoveOnly) { _ArrayHelper::RemoveAt(cItems.Get(), &uItemCount, Position, Count, RemoveOnly); }

protected:
	// Con-/Destructors
	using _Base::_Base;

	// Access
	template <class... _Params> _IndexItem* GetInternal(_Params... Id)
		{
		int ipos=GetItemPos<_Params...>(Id...);
		if(ipos<0)
			return nullptr;
		return &cItems.Get()[ipos];
		}
	template <class... _Params> _IndexItem const* GetInternal(_Params... Id)const
		{
		int ipos=GetItemPos<_Params...>(Id...);
		if(ipos<0)
			return nullptr;
		return &cItems.Get()[ipos];
		}
	template <class... _Params> int GetItemPos(_Params... Id)const
		{
		if(!uItemCount)
			return -1;
		_IndexItem const* pitem=nullptr;
		unsigned int ustart=0;
		unsigned int uend=uItemCount;
		unsigned int uitem=0;
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
		return -(int)uitem-1;
		}

	// Modification
	template <class... _Params> bool AddInternal(_IndexItem** Item, _Params... Id)
		{
		unsigned int u=GetInsertPos<_Params...>(Id...);
		if(u==_GroupSize+1)
			return true;
		if(uItemCount==_GroupSize)
			return false;
		*Item=_ArrayHelper::InsertAt(cItems.Get(), _GroupSize, &uItemCount, u);
		return true;
		}
	template <class... _Params> unsigned int GetInsertPos(_Params... Id)const
		{
		_IndexItem const* pitem=nullptr;
		unsigned int ustart=0;
		unsigned int uend=uItemCount;
		while(ustart<uend)
			{
			unsigned int u=ustart+(uend-ustart)/2;
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
			return _GroupSize+1;
			}
		return ustart;
		}
	template <class... _Params> bool RemoveInternal(_Params... Id)
		{
		int ipos=GetItemPos<_Params...>(Id...);
		if(ipos<0)
			return false;
		_ArrayHelper::RemoveAt(cItems.Get(), &uItemCount, ipos);
		return true;
		}
};


//==================
// Item-Group Index
//==================

// Item-Group Index
template <class _Id, class _Item, unsigned int _GroupSize>
class IndexItemGroup: public IndexItemGroupBase<_Id, _Item, _GroupSize>
{
public:
	// Access
	inline bool Contains(_Id const& Id)const override { return GetItemPos<_Id const&>(Id)>=0; }
	inline int Find(_Id const& Id)const override { return GetItemPos<_Id const&>(Id); }
	inline _IndexItem* Get(_Id const& Id)override { return GetInternal<_Id const&>(Id); }
	inline _IndexItem const* Get(_Id const& Id)const override { return GetInternal<_Id const&>(Id); }

	// Modification
	inline bool Add(_IndexItem** Item, _Id const& Id, bool Again)override { return AddInternal<_Id const&>(Item, Id); }
	inline bool Remove(_Id const& Id)override { return RemoveInternal<_Id const&>(Id); }
};

// Item-Group Pointer-Index
template <class _Id, class _Item, unsigned int _GroupSize>
class IndexItemGroup<_Id*, _Item, _GroupSize>: public IndexItemGroupBase<_Id*, _Item, _GroupSize>
{
public:
	// Access
	inline bool Contains(_Id* Id)const override { return GetItemPos(Id)>=0; }
	inline int Find(_Id* Id)const override { return GetItemPos(Id); }
	inline _IndexItem* Get(_Id* Id)override { return GetInternal(Id); }
	inline _IndexItem const* Get(_Id* Id)const override { return GetInternal(Id); }

	// Modification
	inline bool Add(_IndexItem** Item, _Id* Id, bool Again)override { return AddInternal(Item, Id); }
	inline bool Remove(_Id* Id)override { return RemoveInternal(Id); }
};

#ifdef __cplusplus_winrt
// Item-Group Handle-Index
template <class _Id, class _Item, unsigned int _GroupSize>
class IndexItemGroup<_Id^, _Item, _GroupSize>: public IndexItemGroupBase<_Id^, _Item, _GroupSize>
{
public:
	// Access
	inline bool Contains(_Id^ Id)const override { return GetItemPos(Id)>=0; }
	inline int Find(_Id^ Id)const override { return GetItemPos(Id); }
	inline _IndexItem* Get(_Id^ Id)override { return GetInternal(Id); }
	inline _IndexItem const* Get(_Id^ Id)const override { return GetInternal(Id); }

	// Modification
	inline bool Add(_IndexItem** Item, _Id^ Id, bool Again)override { return AddInternal(Item, Id); }
	inline bool Remove(_Id^ Id)override { return RemoveInternal(Id); }
};
#endif

// Item-Group String-Index
template <class _Char, bool _AllocId, class _Item, unsigned int _GroupSize>
class IndexItemGroup<String<_Char, _AllocId>, _Item, _GroupSize>: public IndexItemGroupBase<String<_Char, _AllocId>, _Item, _GroupSize>
{
public:
	// Access
	inline bool Contains(_Char const* Id, unsigned int Length, bool CaseSensitive)const override { return GetItemPos(Id, Length, CaseSensitive)>=0; }
	inline int Find(_Char const* Id, unsigned int Length, bool CaseSensitive)const override { return GetItemPos(Id, Length, CaseSensitive); }
	inline _IndexItem* Get(_Char const* Id, unsigned int Length, bool CaseSensitive)override { return GetInternal(Id, Length, CaseSensitive); }
	inline _IndexItem const* Get(_Char const* Id, unsigned int Length, bool CaseSensitive)const override { return GetInternal(Id, Length, CaseSensitive); }

	// Modification
	inline bool Add(_IndexItem** Item, _Char const* Id, unsigned int Length, bool CaseSensitive, bool Again)override { return AddInternal(Item, Id, Length, CaseSensitive); }
	inline bool Remove(_Char const* Id, unsigned int Length, bool CaseSensitive)override { return RemoveInternal(Id, Length, CaseSensitive); }
};

}}}}
