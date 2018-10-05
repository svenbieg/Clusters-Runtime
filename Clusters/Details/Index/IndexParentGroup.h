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
	namespace Details {
		namespace Index {


//===============================
// Base-Class Parent-Group Index
//===============================

template <class ID, class ITEM, class PARENTGROUP, UINT _GroupSize>
class IndexParentGroupBase: public ::Clusters::Details::Cluster::ParentGroup<IndexItem<ID, ITEM>, IndexGroup<ID, ITEM>, IndexItemGroup<ID, ITEM, _GroupSize>, PARENTGROUP, _GroupSize>
{
protected:
	// Using
	using INDEXITEM=IndexItem<ID, ITEM>;
	using GROUP=IndexGroup<ID, ITEM>;
	using ITEMGROUP=IndexItemGroup<ID, ITEM, _GroupSize>;

private:
	// Using
	using BASE=::Clusters::Details::Cluster::ParentGroup<INDEXITEM, GROUP, ITEMGROUP, PARENTGROUP, _GroupSize>;

public:
	// Modification
	inline VOID AppendGroups(GROUP *const* Groups, UINT Count)override
		{
		BASE::AppendGroups(Groups, Count);
		UpdateBounds();
		}
	inline VOID InsertGroups(UINT Position, GROUP *const* Groups, UINT Count)override
		{
		BASE::InsertGroups(Position, Groups, Count);
		UpdateBounds();
		}
	inline VOID RemoveAt(UINT64 Position)override
		{
		BASE::RemoveAt(Position);
		UpdateBounds();
		}
	inline VOID RemoveGroups(UINT Position, UINT Count)override
		{
		BASE::RemoveGroups(Position, Count);
		UpdateBounds();
		}

	// Access
	inline INDEXITEM* GetFirst()override { return pFirst; }
	inline INDEXITEM const* GetFirst()const override { return pFirst; }
	inline INDEXITEM* GetLast()override { return pLast; }
	inline INDEXITEM const* GetLast()const override { return pLast; }

protected:
	// Con-/Destructors
	IndexParentGroupBase(): BASE(), pFirst(nullptr), pLast(nullptr) {}
	IndexParentGroupBase(UINT Level): BASE(Level), pFirst(nullptr), pLast(nullptr) {}
	IndexParentGroupBase(GROUP* Child): BASE(Child), pFirst(Child->GetFirst()), pLast(Child->GetLast()) {}
	IndexParentGroupBase(IndexParentGroupBase const& Group): BASE(Group), pFirst(ppChildren[0]->GetFirst()), pLast(ppChildren[uChildCount-1]->GetLast()) {}

	// Access
	template <class... PARAMS> INDEXITEM* GetInternal(PARAMS... Id)
		{
		INT ipos=GetItemPos<PARAMS...>(Id...);
		if(ipos<0)
			return nullptr;
		return ppChildren[ipos]->Get(Id...);
		}
	template <class... PARAMS> INDEXITEM const* GetInternal(PARAMS... Id)const
		{
		INT ipos=GetItemPos<PARAMS...>(Id...);
		if(ipos<0)
			return nullptr;
		return ppChildren[ipos]->Get(Id...);
		}
	template <class... PARAMS> INT GetItemPos(PARAMS... Id)const
		{
		if(!uChildCount)
			return -1;
		UINT ustart=0;
		UINT uend=uChildCount;
		UINT ugroup=0;
		INDEXITEM* pfirst=nullptr;
		INDEXITEM* plast=nullptr;
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
		return -(INT)ugroup-1;
		}

	// Modification
	template <class... PARAMS> BOOL AddInternal(INDEXITEM** Item, BOOL Again, PARAMS... Id)
		{
		if(!DoAdd<PARAMS...>(Item, Again, Id...))
			return false;
		if(!*Item)
			return true;
		uItemCount++;
		UpdateBounds();
		return true;
		}
	template <class... PARAMS> BOOL DoAdd(INDEXITEM** Item, BOOL Again, PARAMS... Id)
		{
		UINT ppos[2];
		UINT uinscount=GetInsertPos<PARAMS...>(ppos, Id...);
		if(!uinscount)
			return false;
		if(uinscount==_GroupSize)
			return true;
		if(!Again)
			{
			for(UINT u=0; u<uinscount; u++)
				{
				UINT upos=ppos[u];
				if(ppChildren[upos]->Add(Item, Id..., false))
					return true;
				}
			UINT usrc=ppos[0];
			UINT udst=_GroupSize;
			UINT udif=GetNearestGroup(ppos[0], &udst);
			if(uinscount>1)
				{
				UINT udst1=_GroupSize;
				UINT udif1=GetNearestGroup(ppos[1], &udst1);
				if(udif1<udif)
					{
					usrc=ppos[1];
					udst=udst1;
					}
				}
			if(udst<uChildCount)
				{
				MoveChildren(usrc, udst, 1);
				uinscount=GetInsertPos<PARAMS...>(ppos, Id...);
				if(!uinscount)
					return false;
				if(uinscount==_GroupSize)
					return true;
				for(UINT u=0; u<uinscount; u++)
					{
					UINT ugroup=ppos[u];
					if(ppChildren[ugroup]->Add(Item, Id..., false))
						return true;
					}
				}
			}
		UINT ugroup=ppos[0];
		if(!SplitChild(ugroup))
			return false;
		MoveChildren(ugroup, ugroup+1, 1);
		INDEXITEM* plast=ppChildren[ugroup]->GetLast();
		if(IsBelow(plast->GetId(), Id...))
			ugroup++;
		BOOL badded=ppChildren[ugroup]->Add(Item, Id..., Again);
		ASSERT(badded);
		return true;
		}
	template <class... PARAMS> UINT GetInsertPos(UINT* Groups, PARAMS... Id)const
		{
		if(!uChildCount)
			return 0;
		UINT ustart=0;
		UINT uend=uChildCount;
		INDEXITEM* pfirst=nullptr;
		INDEXITEM* plast=nullptr;
		while(ustart<uend)
			{
			UINT u=ustart+(uend-ustart)/2;
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
		ustart=min(ustart, uChildCount-1);
		Groups[0]=ustart;
		if(ustart>0)
			{
			pfirst=ppChildren[ustart]->GetFirst();
			if(IsAbove(pfirst->GetId(), Id...))
				{
				UINT64 ufree=ppChildren[ustart]->GetFreeCount();
				UINT64 ufree1=ppChildren[ustart-1]->GetFreeCount();
				if(ufree>=ufree1)
					{
					Groups[1]=ustart-1;
					return 2;
					}
				Groups[0]=ustart-1;
				Groups[1]=ustart;
				return 2;
				}
			}
		if(ustart+1<uChildCount)
			{
			plast=ppChildren[ustart]->GetLast();
			if(IsBelow(plast->GetId(), Id...))
				{
				UINT64 ufree=ppChildren[ustart]->GetFreeCount();
				UINT64 ufree1=ppChildren[ustart+1]->GetFreeCount();
				if(ufree>=ufree1)
					{
					Groups[1]=ustart+1;
					return 2;
					}
				Groups[0]=ustart+1;
				Groups[1]=ustart;
				return 2;
				}
			}
		return 1;
		}
	template <class... PARAMS> BOOL RemoveInternal(PARAMS... Id)
		{
		INT ipos=GetItemPos<PARAMS...>(Id...);
		if(ipos<0)
			return false;
		if(!ppChildren[ipos]->Remove(Id...))
			return false;
		uItemCount--;
		CombineChildren(ipos);
		UpdateBounds();
		return true;
		}
	VOID UpdateBounds()
		{
		if(!uChildCount)
			return;
		pFirst=ppChildren[0]->GetFirst();
		pLast=ppChildren[uChildCount-1]->GetLast();
		}
	INDEXITEM* pFirst;
	INDEXITEM* pLast;
};


//====================
// Parent-Group Index
//====================

// Parent-Group Index
template <class ID, class ITEM, UINT _GroupSize>
class IndexParentGroup: public IndexParentGroupBase<ID, ITEM, IndexParentGroup<ID, ITEM, _GroupSize>, _GroupSize>
{
private:
	// Using
	using BASE=IndexParentGroupBase<ID, ITEM, IndexParentGroup<ID, ITEM, _GroupSize>, _GroupSize>;

public:
	// Con-/Destructors
	IndexParentGroup(): BASE() {}
	IndexParentGroup(UINT Level): BASE(Level) {}
	IndexParentGroup(GROUP* Child): BASE(Child) {}
	IndexParentGroup(IndexParentGroup const& Group): BASE(Group) {}

	// Access
	inline BOOL Contains(ID const& Id)const override { return GetItemPos<ID const&>(Id)>=0; }
	inline INT Find(ID const& Id)const override { return GetItemPos<ID const&>(Id); }
	inline INDEXITEM* Get(ID const& Id)override { return GetInternal<ID const&>(Id); }
	inline INDEXITEM const* Get(ID const& Id)const override { return GetInternal<ID const&>(Id); }

	// Modification
	inline BOOL Add(INDEXITEM** Item, ID const& Id, BOOL Again)override { return AddInternal<ID const&>(Item, Again, Id); }
	inline BOOL Remove(ID const& Id)override { return RemoveInternal<ID const&>(Id); }
};

// Parent-Group Pointer-Index
template <class ID, class ITEM, UINT _GroupSize>
class IndexParentGroup<ID*, ITEM, _GroupSize>: public IndexParentGroupBase<ID*, ITEM, IndexParentGroup<ID*, ITEM, _GroupSize>, _GroupSize>
{
private:
	// Using
	using BASE=IndexParentGroupBase<ID*, ITEM, IndexParentGroup<ID*, ITEM, _GroupSize>, _GroupSize>;

public:
	// Con-/Destructors
	IndexParentGroup(): BASE() {}
	IndexParentGroup(UINT Level): BASE(Level) {}
	IndexParentGroup(GROUP* Child): BASE(Child) {}
	IndexParentGroup(IndexParentGroup const& Group): BASE(Group) {}

	// Access
	inline BOOL Contains(ID* Id)const override { return GetItemPos(Id)>=0; }
	inline INT Find(ID* Id)const override { return GetItemPos(Id); }
	inline INDEXITEM* Get(ID* Id)override { return GetInternal(Id); }
	inline INDEXITEM const* Get(ID* Id)const override { return GetInternal(Id); }

	// Modification
	inline BOOL Add(INDEXITEM** Item, ID* Id, BOOL Again)override { return AddInternal(Item, Again, Id); }
	inline BOOL Remove(ID* Id)override { return RemoveInternal(Id); }
};

#ifdef __cplusplus_winrt
// Parent-Group Handle-Index
template <class ID, class ITEM, UINT _GroupSize>
class IndexParentGroup<ID^, ITEM, _GroupSize>: public IndexParentGroupBase<ID^, ITEM, IndexParentGroup<ID^, ITEM, _GroupSize>, _GroupSize>
{
private:
	// Using
	using BASE=IndexParentGroupBase<ID^, ITEM, IndexParentGroup<ID^, ITEM, _GroupSize>, _GroupSize>;

public:
	// Con-/Destructors
	IndexParentGroup(): BASE() {}
	IndexParentGroup(UINT Level): BASE(Level) {}
	IndexParentGroup(GROUP* Child): BASE(Child) {}
	IndexParentGroup(IndexParentGroup const& Group): BASE(Group) {}

	// Access
	inline BOOL Contains(ID^ Id)const override { return GetItemPos(Id)>=0; }
	inline INT Find(ID^ Id)const override { return GetItemPos(Id); }
	inline INDEXITEM* Get(ID^ Id)override { return GetInternal(Id); }
	inline INDEXITEM const* Get(ID^ Id)const override { return GetInternal(Id); }

	// Modification
	inline BOOL Add(INDEXITEM** Item, ID^ Id, BOOL Again)override { return AddInternal(Item, Again, Id); }
	inline BOOL Remove(ID^ Id)override { return RemoveInternal(Id); }
};
#endif

// Parent-Group String-Index
template <class CHAR, BOOL _AllocId, class ITEM, UINT _GroupSize>
class IndexParentGroup<String<CHAR, _AllocId>, ITEM, _GroupSize>: public IndexParentGroupBase<String<CHAR, _AllocId>, ITEM, IndexParentGroup<String<CHAR, _AllocId>, ITEM, _GroupSize>, _GroupSize>
{
private:
	// Using
	using BASE=IndexParentGroupBase<String<CHAR, _AllocId>, ITEM, IndexParentGroup<String<CHAR, _AllocId>, ITEM, _GroupSize>, _GroupSize>;

public:
	// Con-/Destructors
	IndexParentGroup(): BASE() {}
	IndexParentGroup(UINT Level): BASE(Level) {}
	IndexParentGroup(GROUP* Child): BASE(Child) {}
	IndexParentGroup(IndexParentGroup const& Group): BASE(Group) {}

	// Access
	inline BOOL Contains(CHAR const* Id, UINT Length, BOOL CaseSensitive)const override { return GetItemPos(Id, Length, CaseSensitive)>=0; }
	inline INT Find(CHAR const* Id, UINT Length, BOOL CaseSensitive)const override { return GetItemPos(Id, Length, CaseSensitive); }
	inline INDEXITEM* Get(CHAR const* Id, UINT Length, BOOL CaseSensitive)override { return GetInternal(Id, Length, CaseSensitive); }
	inline INDEXITEM const* Get(CHAR const* Id, UINT Length, BOOL CaseSensitive)const override { return GetInternal(Id, Length, CaseSensitive); }

	// Modification
	inline BOOL Add(INDEXITEM** Item, CHAR const* Id, UINT Length, BOOL CaseSensitive, BOOL Again)override { return AddInternal(Item, Again, Id, Length, CaseSensitive); }
	inline BOOL Remove(CHAR const* Id, UINT Length, BOOL CaseSensitive)override { return RemoveInternal(Id, Length, CaseSensitive); }
};

}}}
