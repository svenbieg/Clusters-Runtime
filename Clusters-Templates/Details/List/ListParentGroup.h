//===================
// ListParentGroup.h
//===================

#pragma once


//=======
// Using
//=======

#include "..\Cluster\ParentGroup.h"
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
template<class _Item, unsigned int _GroupSize>
class ListParentGroup: public Clusters::Templates::Details::Cluster::ParentGroup<_Item, ListGroup<_Item>, ListItemGroup<_Item, _GroupSize>, ListParentGroup<_Item, _GroupSize>, _GroupSize>
{
private:
	// Using
	using _Group=ListGroup<_Item>;
	using _ItemGroup=ListItemGroup<_Item, _GroupSize>;
	using _ParentGroup=ListParentGroup<_Item, _GroupSize>;
	using _Base=Clusters::Templates::Details::Cluster::ParentGroup<_Item, _Group, _ItemGroup, _ParentGroup, _GroupSize>;

public:
	// Con-/Destructors
	ListParentGroup(): _Base() {}
	ListParentGroup(unsigned int Level): _Base(Level) {}
	ListParentGroup(_Group* Group): _Base(Group) {}
	ListParentGroup(ListParentGroup const& Group): _Base(Group) {}

	// Modification
	_Item* Append(bool Again)override
		{
		_Item* pitem=DoAppend(Again);
		if(!pitem)
			return nullptr;
		uItemCount++;
		return pitem;
		}
	_Item* InsertAt(size_t Position, bool Again)override
		{
		_Item* pitem=DoInsert(Position, Again);
		if(!pitem)
			return nullptr;
		uItemCount++;
		return pitem;
		}

protected:
	_Item* DoAppend(bool Again)
		{
		unsigned int ugroup=uChildCount-1;
		if(!Again)
			{
			_Item* pitem=ppChildren[ugroup]->Append(false);
			if(pitem)
				return pitem;
			unsigned int udst=GetNearestGroup(ugroup);
			if(udst<uChildCount)
				{
				MoveChildren(ugroup, udst, 1);
				pitem=ppChildren[ugroup]->Append(false);
				ASSERT(pitem);
				return pitem;
				}
			}
		if(!SplitChild(ugroup))
			return nullptr;
		MoveChildren(ugroup, ugroup+1, 1);
		_Item* pitem=ppChildren[ugroup+1]->Append(Again);
		ASSERT(pitem);
		return pitem;
		}
	_Item* DoInsert(size_t Position, bool Again)
		{
		size_t upos=Position;
		unsigned int ugroup=0;
		unsigned int uinscount=GetInsertPos(&upos, &ugroup);
		if(!uinscount)
			return nullptr;
		if(!Again)
			{
			size_t uat=upos;
			for(unsigned int u=0; u<uinscount; u++)
				{
				_Item* pitem=ppChildren[ugroup+u]->InsertAt(uat, false);
				if(pitem)
					return pitem;
				uat=0;
				}
			unsigned int udst=GetNearestGroup(ugroup);
			if(udst<uChildCount)
				{
				if(uinscount>1&&udst>ugroup)
					ugroup++;
				MoveChildren(ugroup, udst, 1);
				upos=Position;
				uinscount=GetInsertPos(&upos, &ugroup);
				size_t uat=upos;
				for(unsigned int u=0; u<uinscount; u++)
					{
					_Item* pitem=ppChildren[ugroup+u]->InsertAt(uat, false);
					if(pitem)
						return pitem;
					uat=0;
					}
				}
			}
		if(!SplitChild(ugroup))
			return nullptr;
		MoveChildren(ugroup, ugroup+1, 1);
		size_t ucount=ppChildren[ugroup]->GetItemCount();
		if(upos>=ucount)
			{
			ugroup++;
			upos-=ucount;
			}
		_Item* pitem=ppChildren[ugroup]->InsertAt(upos, Again);
		ASSERT(pitem);
		return pitem;
		}
	unsigned int GetInsertPos(size_t* Position, unsigned int* Group)
		{
		size_t upos=*Position;
		for(unsigned int u=0; u<uChildCount; u++)
			{
			size_t ucount=ppChildren[u]->GetItemCount();
			if(upos<=ucount)
				{
				*Group=u;
				*Position=upos;
				if(upos==ucount&&u+1<uChildCount)
					return 2;
				return 1;
				}
			upos-=ucount;
			}
		return 0;
		}
};

}}}}
