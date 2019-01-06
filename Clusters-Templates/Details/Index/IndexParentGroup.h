//====================
// IndexParentGroup.h
//====================

#pragma once


//=======
// Using
//=======

#include "../Cluster/ParentGroup.h"
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

template <class _Id, class _Item, class _ParentGroup, UINT _GroupSize>
class IndexParentGroupBase: public ::Clusters::Templates::Details::Cluster::ParentGroup<IndexItem<_Id, _Item>, IndexGroup<_Id, _Item>, IndexItemGroup<_Id, _Item, _GroupSize>, _ParentGroup, _GroupSize>
{
private:
	// Using
	using _IndexItem=IndexItem<_Id, _Item>;
	using _IndexGroup=IndexGroup<_Id, _Item>;
	using _IndexItemGroup=IndexItemGroup<_Id, _Item, _GroupSize>;
	using _IndexParentGroupBase=IndexParentGroupBase<_Id, _Item, _ParentGroup, _GroupSize>;
	using _ClusterParentGroup=::Clusters::Templates::Details::Cluster::ParentGroup<_IndexItem, _IndexGroup, _IndexItemGroup, _ParentGroup, _GroupSize>;

public:
	// Modification
	inline VOID AppendGroups(_IndexGroup *const* Groups, UINT Count)override
		{
		_ClusterParentGroup::AppendGroups(Groups, Count);
		UpdateBounds();
		}
	inline VOID InsertGroups(UINT Position, _IndexGroup *const* Groups, UINT Count)override
		{
		_ClusterParentGroup::InsertGroups(Position, Groups, Count);
		UpdateBounds();
		}
	inline VOID RemoveAt(SIZE_T Position)override
		{
		_ClusterParentGroup::RemoveAt(Position);
		UpdateBounds();
		}
	inline VOID RemoveGroups(UINT Position, UINT Count)override
		{
		_ClusterParentGroup::RemoveGroups(Position, Count);
		UpdateBounds();
		}

	// Access
	inline _IndexItem* GetFirst()override { return pFirst; }
	inline _IndexItem const* GetFirst()const override { return pFirst; }
	inline _IndexItem* GetLast()override { return pLast; }
	inline _IndexItem const* GetLast()const override { return pLast; }

protected:
	// Con-/Destructors
	IndexParentGroupBase(): _ClusterParentGroup(), pFirst(nullptr), pLast(nullptr) {}
	IndexParentGroupBase(UINT Level): _ClusterParentGroup(Level), pFirst(nullptr), pLast(nullptr) {}
	IndexParentGroupBase(_IndexGroup* Child): _ClusterParentGroup(Child), pFirst(Child->GetFirst()), pLast(Child->GetLast()) {}
	IndexParentGroupBase(_IndexParentGroupBase const& Group):
		_ClusterParentGroup(Group),
		pFirst(this->ppChildren[0]->GetFirst()),
		pLast(this->ppChildren[this->uChildCount-1]->GetLast())
		{}

	// Access
	template <class... _Params> _IndexItem* GetInternal(_Params... Id)
		{
		INT ipos=GetItemPos<_Params...>(Id...);
		if(ipos<0)
			return nullptr;
		return this->ppChildren[ipos]->Get(Id...);
		}
	template <class... _Params> _IndexItem const* GetInternal(_Params... Id)const
		{
		INT ipos=GetItemPos<_Params...>(Id...);
		if(ipos<0)
			return nullptr;
		return this->ppChildren[ipos]->Get(Id...);
		}
	template <class... _Params> INT GetItemPos(_Params... Id)const
		{
		if(!this->uChildCount)
			return -1;
		UINT ustart=0;
		UINT uend=this->uChildCount;
		UINT ugroup=0;
		_IndexItem* pfirst=nullptr;
		_IndexItem* plast=nullptr;
		while(ustart<uend)
			{
			ugroup=ustart+(uend-ustart)/2;
			pfirst=this->ppChildren[ugroup]->GetFirst();
			if(this->IsAbove(pfirst->GetId(), Id...))
				{
				uend=ugroup;
				continue;
				}
			plast=this->ppChildren[ugroup]->GetLast();
			if(this->IsBelow(plast->GetId(), Id...))
				{
				ustart=ugroup+1;
				continue;
				}
			return ugroup;
			}
		if(ugroup>0&&this->IsAbove(pfirst->GetId(), Id...))
			ugroup--;
		return -(INT)ugroup-1;
		}

	// Modification
	template <class... _Params> BOOL AddInternal(_IndexItem** Item, BOOL Again, _Params... Id)
		{
		if(!DoAdd<_Params...>(Item, Again, Id...))
			return false;
		if(!*Item)
			return true;
		this->uItemCount++;
		UpdateBounds();
		return true;
		}
	template <class... _Params> BOOL DoAdd(_IndexItem** Item, BOOL Again, _Params... Id)
		{
		UINT ugroup=0;
		UINT uinscount=GetInsertPos<_Params...>(&ugroup, Id...);
		if(uinscount==_GroupSize)
			return true;
		if(!Again)
			{
			for(UINT u=0; u<uinscount; u++)
				{
				if(this->ppChildren[ugroup+u]->Add(Item, Id..., false))
					return true;
				}
			UINT udst=this->GetNearestGroup(ugroup);
			if(udst<this->uChildCount)
				{
				if(uinscount>1&&udst>ugroup)
					ugroup++;
				this->MoveChildren(ugroup, udst, 1);
				if(this->ppChildren[ugroup]->Add(Item, Id..., false))
					return true;
				}
			}
		if(!this->SplitChild(ugroup))
			return false;
		this->MoveChildren(ugroup, ugroup+1, 1);
		_IndexItem* plast=this->ppChildren[ugroup]->GetLast();
		if(this->IsBelow(plast->GetId(), Id...))
			ugroup++;
		BOOL badded=this->ppChildren[ugroup]->Add(Item, Id..., Again);
		ASSERT(badded);
		return true;
		}
	template <class... _Params> UINT GetInsertPos(UINT* Group, _Params... Id)const
		{
		ASSERT(this->uChildCount>0);
		UINT ustart=0;
		UINT uend=this->uChildCount;
		_IndexItem* pfirst=nullptr;
		_IndexItem* plast=nullptr;
		while(ustart<uend)
			{
			UINT u=ustart+(uend-ustart)/2;
			pfirst=this->ppChildren[u]->GetFirst();
			plast=this->ppChildren[u]->GetLast();
			if(this->IsEqual(pfirst->GetId(), Id...))
				return _GroupSize;
			if(this->IsEqual(plast->GetId(), Id...))
				return _GroupSize;
			if(this->IsAbove(pfirst->GetId(), Id...))
				{
				uend=u;
				continue;
				}
			if(this->IsBelow(plast->GetId(), Id...))
				{
				ustart=u+1;
				continue;
				}
			ustart=u;
			break;
			}
		if(ustart>this->uChildCount-1)
			ustart=this->uChildCount-1;
		*Group=ustart;
		if(ustart>0)
			{
			pfirst=this->ppChildren[ustart]->GetFirst();
			if(this->IsAbove(pfirst->GetId(), Id...))
				{
				*Group=ustart-1;
				return 2;
				}
			}
		if(ustart+1<this->uChildCount)
			{
			plast=this->ppChildren[ustart]->GetLast();
			if(this->IsBelow(plast->GetId(), Id...))
				return 2;
			}
		return 1;
		}
	template <class... _Params> BOOL RemoveInternal(_Params... Id)
		{
		INT ipos=GetItemPos<_Params...>(Id...);
		if(ipos<0)
			return false;
		if(!this->ppChildren[ipos]->Remove(Id...))
			return false;
		this->uItemCount--;
		this->CombineChildren(ipos);
		UpdateBounds();
		return true;
		}
	VOID UpdateBounds()
		{
		if(!this->uChildCount)
			return;
		pFirst=this->ppChildren[0]->GetFirst();
		pLast=this->ppChildren[this->uChildCount-1]->GetLast();
		}
	_IndexItem* pFirst;
	_IndexItem* pLast;
};


//====================
// Parent-Group Index
//====================

// Parent-Group Index
template <class _Id, class _Item, UINT _GroupSize>
class IndexParentGroup: public IndexParentGroupBase<_Id, _Item, IndexParentGroup<_Id, _Item, _GroupSize>, _GroupSize>
{
private:
	// Using
	using _IndexItem=IndexItem<_Id, _Item>;
	using _IndexGroup=IndexGroup<_Id, _Item>;
	using _IndexParentGroup=IndexParentGroup<_Id, _Item, _GroupSize>;
	using _IndexParentGroupBase=IndexParentGroupBase<_Id, _Item, _IndexParentGroup, _GroupSize>;

public:
	// Con-/Destructors
	IndexParentGroup(): _IndexParentGroupBase() {}
	IndexParentGroup(UINT Level): _IndexParentGroupBase(Level) {}
	IndexParentGroup(_IndexGroup* Child): _IndexParentGroupBase(Child) {}
	IndexParentGroup(IndexParentGroup const& Group): _IndexParentGroupBase(Group) {}

	// Access
	inline BOOL Contains(_Id const& Id)const override { return this->template GetItemPos<_Id const&>(Id)>=0; }
	inline INT Find(_Id const& Id)const override { return this->template GetItemPos<_Id const&>(Id); }
	inline _IndexItem* Get(_Id const& Id)override { return this->template GetInternal<_Id const&>(Id); }
	inline _IndexItem const* Get(_Id const& Id)const override { return this->template GetInternal<_Id const&>(Id); }

	// Modification
	inline BOOL Add(_IndexItem** Item, _Id const& Id, BOOL Again)override { return this->template AddInternal<_Id const&>(Item, Again, Id); }
	inline BOOL Remove(_Id const& Id)override { return this->template RemoveInternal<_Id const&>(Id); }
};

// Parent-Group Pointer-Index
template <class _Id, class _Item, UINT _GroupSize>
class IndexParentGroup<_Id*, _Item, _GroupSize>: public IndexParentGroupBase<_Id*, _Item, IndexParentGroup<_Id*, _Item, _GroupSize>, _GroupSize>
{
private:
	// Using
	using _IndexItem=IndexItem<_Id*, _Item>;
	using _IndexGroup=IndexGroup<_Id*, _Item>;
	using _IndexParentGroup=IndexParentGroup<_Id*, _Item, _GroupSize>;
	using _IndexParentGroupBase=IndexParentGroupBase<_Id*, _Item, _IndexParentGroup, _GroupSize>;

public:
	// Con-/Destructors
	IndexParentGroup(): _IndexParentGroupBase() {}
	IndexParentGroup(UINT Level): _IndexParentGroupBase(Level) {}
	IndexParentGroup(_IndexGroup* Child): _IndexParentGroupBase(Child) {}
	IndexParentGroup(IndexParentGroup const& Group): _IndexParentGroupBase(Group) {}

	// Access
	inline BOOL Contains(_Id* Id)const override { return this->GetItemPos(Id)>=0; }
	inline INT Find(_Id* Id)const override { return this->GetItemPos(Id); }
	inline _IndexItem* Get(_Id* Id)override { return this->GetInternal(Id); }
	inline _IndexItem const* Get(_Id* Id)const override { return this->GetInternal(Id); }

	// Modification
	inline BOOL Add(_IndexItem** Item, _Id* Id, BOOL Again)override { return this->AddInternal(Item, Again, Id); }
	inline BOOL Remove(_Id* Id)override { return this->RemoveInternal(Id); }
};

#ifdef __cplusplus_winrt
// Parent-Group Handle-Index
template <class _Id, class _Item, UINT _GroupSize>
class IndexParentGroup<_Id^, _Item, _GroupSize>: public IndexParentGroupBase<_Id^, _Item, IndexParentGroup<_Id^, _Item, _GroupSize>, _GroupSize>
{
private:
	// Using
	using _IndexItem=IndexItem<_Id^, _Item>;
	using _IndexGroup=IndexGroup<_Id^, _Item>;
	using _IndexParentGroup=IndexParentGroup<_Id^, _Item, _GroupSize>;
	using _IndexParentGroupBase=IndexParentGroupBase<_Id^, _Item, _IndexParentGroup, _GroupSize>;

public:
	// Con-/Destructors
	IndexParentGroup(): _IndexParentGroupBase() {}
	IndexParentGroup(UINT Level): _IndexParentGroupBase(Level) {}
	IndexParentGroup(_IndexGroup* Child): _IndexParentGroupBase(Child) {}
	IndexParentGroup(IndexParentGroup const& Group): _IndexParentGroupBase(Group) {}

	// Access
	inline BOOL Contains(_Id^ Id)const override { return this->GetItemPos(Id)>=0; }
	inline INT Find(_Id^ Id)const override { return this->GetItemPos(Id); }
	inline _IndexItem* Get(_Id^ Id)override { return this->GetInternal(Id); }
	inline _IndexItem const* Get(_Id^ Id)const override { return this->GetInternal(Id); }

	// Modification
	inline BOOL Add(_IndexItem** Item, _Id^ Id, BOOL Again)override { return this->AddInternal(Item, Again, Id); }
	inline BOOL Remove(_Id^ Id)override { return this->RemoveInternal(Id); }
};
#endif

// Parent-Group String-Index
template <class _Char, BOOL _AllocId, class _Item, UINT _GroupSize>
class IndexParentGroup<String<_Char, _AllocId>, _Item, _GroupSize>: public IndexParentGroupBase<String<_Char, _AllocId>, _Item, IndexParentGroup<String<_Char, _AllocId>, _Item, _GroupSize>, _GroupSize>
{
private:
	// Using
	using _IndexItem=IndexItem<String<_Char, _AllocId>, _Item>;
	using _IndexGroup=IndexGroup<String<_Char, _AllocId>, _Item>;
	using _IndexParentGroup=IndexParentGroup<String<_Char, _AllocId>, _Item, _GroupSize>;
	using _IndexParentGroupBase=IndexParentGroupBase<String<_Char, _AllocId>, _Item, _IndexParentGroup, _GroupSize>;

public:
	// Con-/Destructors
	IndexParentGroup(): _IndexParentGroupBase() {}
	IndexParentGroup(UINT Level): _IndexParentGroupBase(Level) {}
	IndexParentGroup(_IndexGroup* Child): _IndexParentGroupBase(Child) {}
	IndexParentGroup(IndexParentGroup const& Group): _IndexParentGroupBase(Group) {}

	// Access
	inline BOOL Contains(_Char const* Id, UINT Length, BOOL CaseSensitive)const override { return this->GetItemPos(Id, Length, CaseSensitive)>=0; }
	inline INT Find(_Char const* Id, UINT Length, BOOL CaseSensitive)const override { return this->GetItemPos(Id, Length, CaseSensitive); }
	inline _IndexItem* Get(_Char const* Id, UINT Length, BOOL CaseSensitive)override { return this->GetInternal(Id, Length, CaseSensitive); }
	inline _IndexItem const* Get(_Char const* Id, UINT Length, BOOL CaseSensitive)const override { return this->GetInternal(Id, Length, CaseSensitive); }

	// Modification
	inline BOOL Add(_IndexItem** Item, _Char const* Id, UINT Length, BOOL CaseSensitive, BOOL Again)override { return this->AddInternal(Item, Again, Id, Length, CaseSensitive); }
	inline BOOL Remove(_Char const* Id, UINT Length, BOOL CaseSensitive)override { return this->RemoveInternal(Id, Length, CaseSensitive); }
};

}}}}
