//=====================
// VectorParentGroup.h
//=====================

#pragma once


//=======
// Using
//=======

#include "..\Cluster\ParentGroup.h"
#include "VectorItemGroup.h"


//===========
// Namespace
//===========

namespace Clusters {
	namespace Details {
		namespace Vector {


//=====================
// Parent-Group Vector
//=====================

// Parent-Group Vector
template<class ITEM, UINT _GroupSize>
class VectorParentGroup: public ::Clusters::Details::Cluster::ParentGroup<ITEM, VectorGroup<ITEM>, VectorItemGroup<ITEM, _GroupSize>, VectorParentGroup<ITEM, _GroupSize>, _GroupSize>
{
private:
	// Using
	using GROUP=VectorGroup<ITEM>;
	using ITEMGROUP=VectorItemGroup<ITEM, _GroupSize>;
	using PARENTGROUP=VectorParentGroup<ITEM, _GroupSize>;
	using BASE=::Clusters::Details::Cluster::ParentGroup<ITEM, GROUP, ITEMGROUP, PARENTGROUP, _GroupSize>;

public:
	// Con-/Destructors
	VectorParentGroup() {}
	VectorParentGroup(UINT Level): BASE(Level) {}
	VectorParentGroup(GROUP* Child): BASE(Child) {}

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
			UINT udst=_GroupSize;
			UINT udif=GetNearestGroup(ugroup, &udst);
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
		UINT pinsgroups[2];
		UINT64 pinsids[2];
		UINT uinscount=GetInsertPos(Position, pinsgroups, pinsids);
		if(!uinscount)
			return nullptr;
		if(!Again)
			{
			for(UINT u=0; u<uinscount; u++)
				{
				UINT ugroup=pinsgroups[u];
				ITEM* pitem=ppChildren[ugroup]->InsertAt(pinsids[u], false);
				if(pitem)
					return pitem;
				}
			UINT64 uid=pinsids[0];
			UINT usrc=pinsgroups[0];
			UINT udst=_GroupSize;
			UINT udif=GetNearestGroup(pinsgroups[0], &udst);
			if(uinscount>1)
				{
				UINT udst1=0;
				UINT udif1=GetNearestGroup(pinsgroups[1], &udst1);
				if(udif1<udif)
					{
					uid=pinsids[1];
					usrc=pinsgroups[1];
					udst=udst1;
					}
				}
			if(udst<uChildCount)
				{
				MoveChildren(usrc, udst, 1);
				uinscount=GetInsertPos(Position, pinsgroups, pinsids);
				if(!uinscount)
					return nullptr;
				for(UINT u=0; u<uinscount; u++)
					{
					UINT ugroup=pinsgroups[u];
					ITEM* pitem=ppChildren[ugroup]->InsertAt(pinsids[u], false);
					if(pitem)
						return pitem;
					}
				}
			}
		UINT ugroup=pinsgroups[0];
		UINT64 uid=pinsids[0];
		if(!SplitChild(ugroup))
			return nullptr;
		MoveChildren(ugroup, ugroup+1, 1);
		UINT64 ucount=ppChildren[ugroup]->GetItemCount();
		if(uid>=ucount)
			{
			ugroup++;
			uid-=ucount;
			}
		ITEM* pitem=ppChildren[ugroup]->InsertAt(uid, Again);
		ASSERT(pitem);
		return pitem;
		}
	UINT GetInsertPos(UINT64 Position, UINT* Groups, UINT64* Ids)
		{
		UINT64 uid=Position;
		for(UINT u=0; u<uChildCount; u++)
			{
			UINT64 ucount=ppChildren[u]->GetItemCount();
			if(uid<ucount)
				{
				Groups[0]=u;
				Ids[0]=uid;
				return 1;
				}
			if(uid==ucount)
				{
				Groups[0]=u;
				Ids[0]=ucount;
				if(u+1<uChildCount)
					{
					UINT64 ufree=ppChildren[u]->GetFreeCount();
					UINT64 ufree1=ppChildren[u+1]->GetFreeCount();
					if(ufree>=ufree1)
						{
						Groups[1]=u+1;
						Ids[1]=0;
						return 2;
						}
					Groups[0]=u+1;
					Ids[0]=0;
					Groups[1]=u;
					Ids[1]=ucount;
					return 2;
					}
				return 1;
				}
			uid-=ucount;
			}
		return 0;
		}
};

}}}
