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
template<class ITEM, UINT _GroupSize>
class ListParentGroup: public Clusters::Templates::Details::Cluster::ParentGroup<ITEM, ListGroup<ITEM>, ListItemGroup<ITEM, _GroupSize>, ListParentGroup<ITEM, _GroupSize>, _GroupSize>
{
private:
	// Using
	using GROUP=ListGroup<ITEM>;
	using ITEMGROUP=ListItemGroup<ITEM, _GroupSize>;
	using PARENTGROUP=ListParentGroup<ITEM, _GroupSize>;
	using BASE=Clusters::Templates::Details::Cluster::ParentGroup<ITEM, GROUP, ITEMGROUP, PARENTGROUP, _GroupSize>;

public:
	// Con-/Destructors
	ListParentGroup(): BASE() {}
	ListParentGroup(UINT Level): BASE(Level) {}
	ListParentGroup(GROUP* Group): BASE(Group) {}
	ListParentGroup(ListParentGroup const& Group): BASE(Group) {}

	// Modification
	ITEM* Append(BOOL Again)override
		{
		ITEM* pitem=DoAppend(Again);
		if(!pitem)
			return nullptr;
		uItemCount++;
		return pitem;
		}
	ITEM* InsertAt(UINT64 Position, BOOL Again)override
		{
		ITEM* pitem=DoInsert(Position, Again);
		if(!pitem)
			return nullptr;
		uItemCount++;
		return pitem;
		}

protected:
	ITEM* DoAppend(BOOL Again)
		{
		UINT ugroup=uChildCount-1;
		if(!Again)
			{
			ITEM* pitem=ppChildren[ugroup]->Append(false);
			if(pitem)
				return pitem;
			UINT udst=GetNearestGroup(ugroup);
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
		ITEM* pitem=ppChildren[ugroup+1]->Append(Again);
		ASSERT(pitem);
		return pitem;
		}
	ITEM* DoInsert(UINT64 Position, BOOL Again)
		{
		UINT64 upos=Position;
		UINT ugroup=0;
		UINT uinscount=GetInsertPos(&upos, &ugroup);
		if(!uinscount)
			return nullptr;
		if(!Again)
			{
			UINT64 uat=upos;
			for(UINT u=0; u<uinscount; u++)
				{
				ITEM* pitem=ppChildren[ugroup+u]->InsertAt(uat, false);
				if(pitem)
					return pitem;
				uat=0;
				}
			UINT udst=GetNearestGroup(ugroup);
			if(udst<uChildCount)
				{
				if(uinscount>1&&udst>ugroup)
					ugroup++;
				MoveChildren(ugroup, udst, 1);
				upos=Position;
				uinscount=GetInsertPos(&upos, &ugroup);
				UINT64 uat=upos;
				for(UINT u=0; u<uinscount; u++)
					{
					ITEM* pitem=ppChildren[ugroup+u]->InsertAt(uat, false);
					if(pitem)
						return pitem;
					uat=0;
					}
				}
			}
		if(!SplitChild(ugroup))
			return nullptr;
		MoveChildren(ugroup, ugroup+1, 1);
		UINT64 ucount=ppChildren[ugroup]->GetItemCount();
		if(upos>=ucount)
			{
			ugroup++;
			upos-=ucount;
			}
		ITEM* pitem=ppChildren[ugroup]->InsertAt(upos, Again);
		ASSERT(pitem);
		return pitem;
		}
	UINT GetInsertPos(UINT64* Position, UINT* Group)
		{
		UINT64 upos=*Position;
		for(UINT u=0; u<uChildCount; u++)
			{
			UINT64 ucount=ppChildren[u]->GetItemCount();
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
