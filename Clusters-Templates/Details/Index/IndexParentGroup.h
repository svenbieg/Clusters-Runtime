//====================
// IndexParentGroup.h
//====================

#pragma once


//=======
// Using
//=======

#include "..\Cluster\ParentGroup.h"
#include "IndexItemGroup.h"


//===========
// Namespace
//===========

namespace Clusters {
	namespace Templates {
		namespace Details {
			namespace Index {


//===============================
// Base-Class Parent-Group Index
//===============================

template <class _Id, class _Item, class _ParentGroup, unsigned int _GroupSize>
class IndexParentGroupBase: public ::Clusters::Templates::Details::Cluster::ParentGroup<IndexItem<_Id, _Item>, IndexGroup<_Id, _Item>, IndexItemGroup<_Id, _Item, _GroupSize>, _ParentGroup, _GroupSize>
{
protected:
	// Using
	using _IndexItem=IndexItem<_Id, _Item>;
	using _Group=IndexGroup<_Id, _Item>;
	using _ItemGroup=IndexItemGroup<_Id, _Item, _GroupSize>;

private:
	// Using
	using _Base=::Clusters::Templates::Details::Cluster::ParentGroup<_IndexItem, _Group, _ItemGroup, _ParentGroup, _GroupSize>;

public:
	// Modification
	inline void AppendGroups(_Group *const* Groups, unsigned int Count)override
		{
		_Base::AppendGroups(Groups, Count);
		UpdateBounds();
		}
	inline void InsertGroups(unsigned int Position, _Group *const* Groups, unsigned int Count)override
		{
		_Base::InsertGroups(Position, Groups, Count);
		UpdateBounds();
		}
	inline void RemoveAt(size_t Position)override
		{
		_Base::RemoveAt(Position);
		UpdateBounds();
		}
	inline void RemoveGroups(unsigned int Position, unsigned int Count)override
		{
		_Base::RemoveGroups(Position, Count);
		UpdateBounds();
		}

	// Access
	inline _IndexItem* GetFirst()override { return pFirst; }
	inline _IndexItem const* GetFirst()const override { return pFirst; }
	inline _IndexItem* GetLast()override { return pLast; }
	inline _IndexItem const* GetLast()const override { return pLast; }

protected:
	// Con-/Destructors
	IndexParentGroupBase(): _Base(), pFirst(nullptr), pLast(nullptr) {}
	IndexParentGroupBase(unsigned int Level): _Base(Level), pFirst(nullptr), pLast(nullptr) {}
	IndexParentGroupBase(_Group* Child): _Base(Child), pFirst(Child->GetFirst()), pLast(Child->GetLast()) {}
	IndexParentGroupBase(IndexParentGroupBase const& Group): _Base(Group), pFirst(ppChildren[0]->GetFirst()), pLast(ppChildren[uChildCount-1]->GetLast()) {}

	// Access
	template <class... _Params> _IndexItem* GetInternal(_Params... Id)
		{
		int ipos=GetItemPos<_Params...>(Id...);
		if(ipos<0)
			return nullptr;
		return ppChildren[ipos]->Get(Id...);
		}
	template <class... _Params> _IndexItem const* GetInternal(_Params... Id)const
		{
		int ipos=GetItemPos<_Params...>(Id...);
		if(ipos<0)
			return nullptr;
		return ppChildren[ipos]->Get(Id...);
		}
	template <class... _Params> int GetItemPos(_Params... Id)const
		{
		if(!uChildCount)
			return -1;
		unsigned int ustart=0;
		unsigned int uend=uChildCount;
		unsigned int ugroup=0;
		_IndexItem* pfirst=nullptr;
		_IndexItem* plast=nullptr;
		while(ustart<uend)
			{
			ugroup=ustart+(uend-ustart)/2;
			pfirst=ppChildren[ugroup]->GetFirst();
			if(IsAbove(pfirst->GetId(), Id...))
				{
				uend=ugroup;
				continue;
				}
			plast=ppChildren[ugroup]->GetLast();
			if(IsBelow(plast->GetId(), Id...))
				{
				ustart=ugroup+1;
				continue;
				}
			return ugroup;
			}
		if(ugroup>0&&IsAbove(pfirst->GetId(), Id...))
			ugroup--;
		return -(int)ugroup-1;
		}

	// Modification
	template <class... _Params> bool AddInternal(_IndexItem** Item, bool Again, _Params... Id)
		{
		if(!DoAdd<_Params...>(Item, Again, Id...))
			return false;
		if(!*Item)
			return true;
		uItemCount++;
		UpdateBounds();
		return true;
		}
	template <class... _Params> bool DoAdd(_IndexItem** Item, bool Again, _Params... Id)
		{
		unsigned int ugroup=0;
		unsigned int uinscount=GetInsertPos<_Params...>(&ugroup, Id...);
		if(uinscount==_GroupSize)
			return true;
		if(!Again)
			{
			for(unsigned int u=0; u<uinscount; u++)
				{
				if(ppChildren[ugroup+u]->Add(Item, Id..., false))
					return true;
				}
			unsigned int udst=GetNearestGroup(ugroup);
			if(udst<uChildCount)
				{
				if(uinscount>1&&udst>ugroup)
					ugroup++;
				MoveChildren(ugroup, udst, 1);
				if(ppChildren[ugroup]->Add(Item, Id..., false))
					return true;
				}
			}
		if(!SplitChild(ugroup))
			return false;
		MoveChildren(ugroup, ugroup+1, 1);
		_IndexItem* plast=ppChildren[ugroup]->GetLast();
		if(IsBelow(plast->GetId(), Id...))
			ugroup++;
		bool badded=ppChildren[ugroup]->Add(Item, Id..., Again);
		ASSERT(badded);
		return true;
		}
	template <class... _Params> unsigned int GetInsertPos(unsigned int* Group, _Params... Id)const
		{
		ASSERT(uChildCount>0);
		unsigned int ustart=0;
		unsigned int uend=uChildCount;
		_IndexItem* pfirst=nullptr;
		_IndexItem* plast=nullptr;
		while(ustart<uend)
			{
			unsigned int u=ustart+(uend-ustart)/2;
			pfirst=ppChildren[u]->GetFirst();
			plast=ppChildren[u]->GetLast();
			if(IsEqual(pfirst->GetId(), Id...))
				return _GroupSize;
			if(IsEqual(plast->GetId(), Id...))
				return _GroupSize;
			if(IsAbove(pfirst->GetId(), Id...))
				{
				uend=u;
				continue;
				}
			if(IsBelow(plast->GetId(), Id...))
				{
				ustart=u+1;
				continue;
				}
			ustart=u;
			break;
			}
		if(ustart>uChildCount-1)
			ustart=uChildCount-1;
		*Group=ustart;
		if(ustart>0)
			{
			pfirst=ppChildren[ustart]->GetFirst();
			if(IsAbove(pfirst->GetId(), Id...))
				{
				*Group=ustart-1;
				return 2;
				}
			}
		if(ustart+1<uChildCount)
			{
			plast=ppChildren[ustart]->GetLast();
			if(IsBelow(plast->GetId(), Id...))
				return 2;
			}
		return 1;
		}
	template <class... _Params> bool RemoveInternal(_Params... Id)
		{
		int ipos=GetItemPos<_Params...>(Id...);
		if(ipos<0)
			return false;
		if(!ppChildren[ipos]->Remove(Id...))
			return false;
		uItemCount--;
		CombineChildren(ipos);
		UpdateBounds();
		return true;
		}
	void UpdateBounds()
		{
		if(!uChildCount)
			return;
		pFirst=ppChildren[0]->GetFirst();
		pLast=ppChildren[uChildCount-1]->GetLast();
		}
	_IndexItem* pFirst;
	_IndexItem* pLast;
};


//====================
// Parent-Group Index
//====================

// Parent-Group Index
template <class _Id, class _Item, unsigned int _GroupSize>
class IndexParentGroup: public IndexParentGroupBase<_Id, _Item, IndexParentGroup<_Id, _Item, _GroupSize>, _GroupSize>
{
private:
	// Using
	using _Base=IndexParentGroupBase<_Id, _Item, IndexParentGroup<_Id, _Item, _GroupSize>, _GroupSize>;

public:
	// Con-/Destructors
	IndexParentGroup(): _Base() {}
	IndexParentGroup(unsigned int Level): _Base(Level) {}
	IndexParentGroup(_Group* Child): _Base(Child) {}
	IndexParentGroup(IndexParentGroup const& Group): _Base(Group) {}

	// Access
	inline bool Contains(_Id const& Id)const override { return GetItemPos<_Id const&>(Id)>=0; }
	inline int Find(_Id const& Id)const override { return GetItemPos<_Id const&>(Id); }
	inline _IndexItem* Get(_Id const& Id)override { return GetInternal<_Id const&>(Id); }
	inline _IndexItem const* Get(_Id const& Id)const override { return GetInternal<_Id const&>(Id); }

	// Modification
	inline bool Add(_IndexItem** Item, _Id const& Id, bool Again)override { return AddInternal<_Id const&>(Item, Again, Id); }
	inline bool Remove(_Id const& Id)override { return RemoveInternal<_Id const&>(Id); }
};

// Parent-Group Pointer-Index
template <class _Id, class _Item, unsigned int _GroupSize>
class IndexParentGroup<_Id*, _Item, _GroupSize>: public IndexParentGroupBase<_Id*, _Item, IndexParentGroup<_Id*, _Item, _GroupSize>, _GroupSize>
{
private:
	// Using
	using _Base=IndexParentGroupBase<_Id*, _Item, IndexParentGroup<_Id*, _Item, _GroupSize>, _GroupSize>;

public:
	// Con-/Destructors
	IndexParentGroup(): _Base() {}
	IndexParentGroup(unsigned int Level): _Base(Level) {}
	IndexParentGroup(_Group* Child): _Base(Child) {}
	IndexParentGroup(IndexParentGroup const& Group): _Base(Group) {}

	// Access
	inline bool Contains(_Id* Id)const override { return GetItemPos(Id)>=0; }
	inline int Find(_Id* Id)const override { return GetItemPos(Id); }
	inline _IndexItem* Get(_Id* Id)override { return GetInternal(Id); }
	inline _IndexItem const* Get(_Id* Id)const override { return GetInternal(Id); }

	// Modification
	inline bool Add(_IndexItem** Item, _Id* Id, bool Again)override { return AddInternal(Item, Again, Id); }
	inline bool Remove(_Id* Id)override { return RemoveInternal(Id); }
};

#ifdef __cplusplus_winrt
// Parent-Group Handle-Index
template <class _Id, class _Item, unsigned int _GroupSize>
class IndexParentGroup<_Id^, _Item, _GroupSize>: public IndexParentGroupBase<_Id^, _Item, IndexParentGroup<_Id^, _Item, _GroupSize>, _GroupSize>
{
private:
	// Using
	using _Base=IndexParentGroupBase<_Id^, _Item, IndexParentGroup<_Id^, _Item, _GroupSize>, _GroupSize>;

public:
	// Con-/Destructors
	IndexParentGroup(): _Base() {}
	IndexParentGroup(unsigned int Level): _Base(Level) {}
	IndexParentGroup(_Group* Child): _Base(Child) {}
	IndexParentGroup(IndexParentGroup const& Group): _Base(Group) {}

	// Access
	inline bool Contains(_Id^ Id)const override { return GetItemPos(Id)>=0; }
	inline int Find(_Id^ Id)const override { return GetItemPos(Id); }
	inline _IndexItem* Get(_Id^ Id)override { return GetInternal(Id); }
	inline _IndexItem const* Get(_Id^ Id)const override { return GetInternal(Id); }

	// Modification
	inline bool Add(_IndexItem** Item, _Id^ Id, bool Again)override { return AddInternal(Item, Again, Id); }
	inline bool Remove(_Id^ Id)override { return RemoveInternal(Id); }
};
#endif

// Parent-Group String-Index
template <class _Char, bool _AllocId, class _Item, unsigned int _GroupSize>
class IndexParentGroup<String<_Char, _AllocId>, _Item, _GroupSize>: public IndexParentGroupBase<String<_Char, _AllocId>, _Item, IndexParentGroup<String<_Char, _AllocId>, _Item, _GroupSize>, _GroupSize>
{
private:
	// Using
	using _Base=IndexParentGroupBase<String<_Char, _AllocId>, _Item, IndexParentGroup<String<_Char, _AllocId>, _Item, _GroupSize>, _GroupSize>;

public:
	// Con-/Destructors
	IndexParentGroup(): _Base() {}
	IndexParentGroup(unsigned int Level): _Base(Level) {}
	IndexParentGroup(_Group* Child): _Base(Child) {}
	IndexParentGroup(IndexParentGroup const& Group): _Base(Group) {}

	// Access
	inline bool Contains(_Char const* Id, unsigned int Length, bool CaseSensitive)const override { return GetItemPos(Id, Length, CaseSensitive)>=0; }
	inline int Find(_Char const* Id, unsigned int Length, bool CaseSensitive)const override { return GetItemPos(Id, Length, CaseSensitive); }
	inline _IndexItem* Get(_Char const* Id, unsigned int Length, bool CaseSensitive)override { return GetInternal(Id, Length, CaseSensitive); }
	inline _IndexItem const* Get(_Char const* Id, unsigned int Length, bool CaseSensitive)const override { return GetInternal(Id, Length, CaseSensitive); }

	// Modification
	inline bool Add(_IndexItem** Item, _Char const* Id, unsigned int Length, bool CaseSensitive, bool Again)override { return AddInternal(Item, Again, Id, Length, CaseSensitive); }
	inline bool Remove(_Char const* Id, unsigned int Length, bool CaseSensitive)override { return RemoveInternal(Id, Length, CaseSensitive); }
};

}}}}
