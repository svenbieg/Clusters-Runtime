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
	_Item* InsertAt(SIZE_T Position, BOOL Again)override
		{
		_Item* pitem=DoInsert(Position, Again);
		if(!pitem)
			return nullptr;
		this->uItemCount++;
		return pitem;
		}

protected:
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
			UINT udst=this->GetNearestGroup(ugroup);
			if(udst<this->uChildCount)
				{
				if(uinscount>1&&udst>ugroup)
					ugroup++;
				this->MoveChildren(ugroup, udst, 1);
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
};

}}}}
