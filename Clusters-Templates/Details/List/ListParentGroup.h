//===================
// ListParentGroup.h
//===================

#pragma once


//=======
// Using
//=======

#include "../Cluster/ParentGroup.h"
#include "ListItemGroup.h"


//===========
// Namespace
//===========

namespace Clusters {
	namespace Templates {
		namespace Details {
			namespace List {


//===================
// Parent-Group List
//===================

// Parent-Group List
template<class _Item, UINT _GroupSize>
class ListParentGroup: public ::Clusters::Templates::Details::Cluster::ParentGroup<_Item, ListGroup<_Item>, ListItemGroup<_Item, _GroupSize>, ListParentGroup<_Item, _GroupSize>, _GroupSize>
{
private:
	// Using
	using _ListGroup=ListGroup<_Item>;
	using _ListItemGroup=ListItemGroup<_Item, _GroupSize>;
	using _ListParentGroup=ListParentGroup<_Item, _GroupSize>;
	using _ParentGroup=::Clusters::Templates::Details::Cluster::ParentGroup<_Item, _ListGroup, _ListItemGroup, _ListParentGroup, _GroupSize>;

public:
	// Con-/Destructors
	ListParentGroup(): _ParentGroup() {}
	ListParentGroup(UINT Level): _ParentGroup(Level) {}
	ListParentGroup(_ListGroup* Group): _ParentGroup(Group) {}
	ListParentGroup(ListParentGroup const& Group): _ParentGroup(Group) {}

	// Modification
	_Item* Append(BOOL Again)override
		{
		_Item* pitem=DoAppend(Again);
		if(!pitem)
			return nullptr;
		this->uItemCount++;
		return pitem;
		}
	SIZE_T Append(_Item const* Items, SIZE_T Count)override
		{
		if(Count==0)
			return 0;
		UINT udst=DoMinimize();
		SIZE_T upos=0;
		for(; udst<this->uChildCount; udst++)
			{
			SIZE_T uwritten=this->ppChildren[udst]->Append(&Items[upos], Count);
			if(uwritten>0)
				{
				uItemCount+=uwritten;
				upos+=uwritten;
				Count-=uwritten;
				if(Count==0)
					break;
				}
			}
		if(Count==0)
			{
			FreeChildren(udst);
			return upos;
			}
		while(Count>0)
			{
			if(uChildCount==_GroupSize)
				break;
			if(uLevel==1)
				{
				ppChildren[uChildCount]=new _ListItemGroup();
				}
			else
				{
				ppChildren[uChildCount]=new _ListParentGroup(uLevel-1);
				}
			uChildCount++;
			UINT uwritten=ppChildren[uChildCount-1]->Append(&Items[upos], Count);
			uItemCount+=uwritten;
			upos+=uwritten;
			Count-=uwritten;
			}
		return upos;
		}
	_Item* InsertAt(SIZE_T Position, BOOL Again)override
		{
		_Item* pitem=DoInsert(Position, Again);
		if(!pitem)
			return nullptr;
		this->uItemCount++;
		return pitem;
		}
	VOID Minimize()
		{
		UINT ucount=DoMinimize();
		FreeChildren(ucount);
		}

protected:
	// Access
	UINT GetInsertPos(SIZE_T* Position, UINT* Group)
		{
		SIZE_T upos=*Position;
		for(UINT u=0; u<this->uChildCount; u++)
			{
			SIZE_T ucount=this->ppChildren[u]->GetItemCount();
			if(upos<=ucount)
				{
				*Group=u;
				*Position=upos;
				if(upos==ucount&&u+1<this->uChildCount)
					return 2;
				return 1;
				}
			upos-=ucount;
			}
		return 0;
		}

	// Modification
	_Item* DoAppend(BOOL Again)
		{
		UINT ugroup=this->uChildCount-1;
		if(!Again)
			{
			_Item* pitem=this->ppChildren[ugroup]->Append(false);
			if(pitem)
				return pitem;
			UINT udst=this->GetNearestGroup(ugroup);
			if(udst<this->uChildCount)
				{
				this->MoveChildren(ugroup, udst, 1);
				pitem=this->ppChildren[ugroup]->Append(false);
				ASSERT(pitem);
				return pitem;
				}
			}
		if(!this->SplitChild(ugroup))
			return nullptr;
		this->MoveChildren(ugroup, ugroup+1, 1);
		_Item* pitem=this->ppChildren[ugroup+1]->Append(Again);
		ASSERT(pitem);
		return pitem;
		}
	_Item* DoInsert(SIZE_T Position, BOOL Again)
		{
		SIZE_T upos=Position;
		UINT ugroup=0;
		UINT uinscount=GetInsertPos(&upos, &ugroup);
		if(!uinscount)
			return nullptr;
		if(!Again)
			{
			SIZE_T uat=upos;
			for(UINT u=0; u<uinscount; u++)
				{
				_Item* pitem=this->ppChildren[ugroup+u]->InsertAt(uat, false);
				if(pitem)
					return pitem;
				uat=0;
				}
			UINT uempty=this->GetNearestGroup(ugroup);
			if(uempty<this->uChildCount)
				{
				if(uinscount>1&&uempty>ugroup)
					ugroup++;
				this->MoveEmptySlot(uempty, ugroup);
				upos=Position;
				uinscount=GetInsertPos(&upos, &ugroup);
				SIZE_T uat=upos;
				for(UINT u=0; u<uinscount; u++)
					{
					_Item* pitem=this->ppChildren[ugroup+u]->InsertAt(uat, false);
					if(pitem)
						return pitem;
					uat=0;
					}
				}
			}
		if(!this->SplitChild(ugroup))
			return nullptr;
		this->MoveChildren(ugroup, ugroup+1, 1);
		SIZE_T ucount=this->ppChildren[ugroup]->GetItemCount();
		if(upos>=ucount)
			{
			ugroup++;
			upos-=ucount;
			}
		_Item* pitem=this->ppChildren[ugroup]->InsertAt(upos, Again);
		ASSERT(pitem);
		return pitem;
		}
	UINT DoMinimize()
		{
		UINT udst=0;
		UINT usrc=0;
		for(; udst<this->uChildCount; udst++)
			{
			UINT ufree=_GroupSize-this->ppChildren[udst]->GetChildCount();
			if(ufree==0)
				continue;
			if(usrc<=udst)
				usrc=udst+1;
			for(; usrc<this->uChildCount; usrc++)
				{
				UINT ucount=this->ppChildren[usrc]->GetChildCount();
				if(ucount==0)
					continue;
				UINT umove=ucount<ufree? ucount: ufree;
				this->MoveChildren(usrc, udst, umove);
				ufree-=umove;
				if(ufree==0)
					break;
				}
			if(usrc>=this->uChildCount)
				break;
			}
		return udst;
		}
	VOID FreeChildren(UINT Count)
		{
		if(Count>=uChildCount)
			return;
		if(this->ppChildren[Count]->GetChildCount()>0)
			Count++;
		for(UINT u=Count; u<this->uChildCount; u++)
			delete this->ppChildren[u];
		uChildCount=Count;
		}
};

}}}}
