//===============
// ParentGroup.h
//===============

#pragma once


//===========
// Namespace
//===========

namespace Clusters {
	namespace Details {
		namespace Cluster {


//=================================
// Base-Class Parent-Group Cluster
//=================================

template <class ITEM, class GROUP, class ITEMGROUP, class PARENTGROUP, UINT _GroupSize>
class ParentGroupBase: public GROUP
{
public:
	// Access
	inline GROUP* GetChild(UINT Position)
		{
		ASSERT(Position<uChildCount);
		return ppChildren[Position];
		}
	inline GROUP const* GetChild(UINT Position)const
		{
		ASSERT(Position<uChildCount);
		return ppChildren[Position];
		}
	inline UINT GetChildCount()const override { return uChildCount; }
	inline GROUP* const* GetChildren()const { return ppChildren; }
	inline UINT64 GetItemCount()const override { return uItemCount; }
	inline UINT GetLevel()const override { return uLevel; }

	// Modification
	virtual VOID AppendGroups(GROUP *const* Groups, UINT Count)
		{
		ASSERT(uChildCount+Count<=_GroupSize);
		UINT64 uaddcount=0;
		for(UINT u=0; u<Count; u++)
			uaddcount+=Groups[u]->GetItemCount();
		CopyMemory(&ppChildren[uChildCount], Groups, Count*sizeof(GROUP*));
		uChildCount+=Count;
		uItemCount+=uaddcount;
		}
	BOOL CombineChildren(UINT Position)
		{
		UINT udst=_GroupSize;
		UINT ucount=GetCombination(Position, &udst);
		if(udst==_GroupSize)
			return false;
		if(ucount>0)
			MoveChildren(Position, udst, ucount);
		delete ppChildren[Position];
		MoveMemory(&ppChildren[Position], &ppChildren[Position+1], (uChildCount-Position-1)*sizeof(VOID*));
		uChildCount--;
		return true;
		}
	virtual VOID InsertGroups(UINT Position, GROUP *const* Groups, UINT Count)
		{
		ASSERT(Position<=uChildCount);
		if(Position==uChildCount)
			return AppendGroups(Groups, Count);
		ASSERT(uChildCount+Count<=_GroupSize);
		UINT64 uaddcount=0;
		for(UINT u=0; u<Count; u++)
			uaddcount+=Groups[u]->GetItemCount();
		MoveMemory(&ppChildren[Position+Count], &ppChildren[Position], (uChildCount-Position)*sizeof(GROUP*));
		CopyMemory(&ppChildren[Position], Groups, Count*sizeof(GROUP*));
		uItemCount+=uaddcount;
		uChildCount+=Count;
		}
	VOID MoveChildren(UINT Source, UINT Destination, UINT Count)
		{
		ASSERT(Source!=Destination);
		ASSERT(Count>0);
		while(Source>Destination+1)
			{
			MoveChildren(Destination+1, Destination, Count);
			Destination++;
			}
		while(Source+1<Destination)
			{
			MoveChildren(Destination-1, Destination, Count);
			Destination--;
			}
		GROUP* psrc=ppChildren[Source];
		ASSERT(Count<=psrc->GetChildCount());
		GROUP* pdst=ppChildren[Destination];
		ASSERT(pdst->GetChildCount()+Count<=_GroupSize);
		if(uLevel>1)
			{
			PARENTGROUP* psrcp=(PARENTGROUP*)psrc;
			PARENTGROUP* pdstp=(PARENTGROUP*)pdst;
			if(Source>Destination)
				{
				pdstp->AppendGroups(psrcp->GetChildren(), Count);
				psrcp->RemoveGroups(0, Count);
				}
			else
				{
				GROUP* const* ppgroups=psrcp->GetChildren();
				UINT usrccount=psrcp->GetChildCount();
				pdstp->InsertGroups(0, &ppgroups[usrccount-Count], Count);
				psrcp->RemoveGroups(usrccount-Count, Count);
				}
			}
		else
			{
			ITEMGROUP* psrci=(ITEMGROUP*)psrc;
			ITEMGROUP* pdsti=(ITEMGROUP*)pdst;
			if(Source>Destination)
				{
				ITEM const* pitems=psrci->GetItems();
				pdsti->Append(pitems, Count, true);
				psrci->RemoveAt(0, Count, true);
				}
			else
				{
				ITEM const* pitems=psrci->GetItems();
				UINT usrccount=psrci->GetChildCount();
				pdsti->InsertAt(0, &pitems[usrccount-Count], Count, true);
				psrci->RemoveAt(usrccount-Count, Count, true);
				}
			}
		}
	virtual VOID RemoveAt(UINT64 Position)override
		{
		UINT ugroup=GetGroup(Position);
		ASSERT(ugroup<uChildCount);
		ppChildren[ugroup]->RemoveAt(Position);
		uItemCount--;
		CombineChildren(ugroup);
		}
	virtual VOID RemoveGroups(UINT Position, UINT Count)
		{
		ASSERT(Position+Count<=uChildCount);
		UINT64 uremcount=0;
		for(UINT u=0; u<Count; u++)
			uremcount+=ppChildren[Position+u]->GetItemCount();
		MoveMemory(&ppChildren[Position], &ppChildren[Position+Count], (uChildCount-Count-Position)*sizeof(GROUP*));
		uItemCount-=uremcount;
		uChildCount-=Count;
		}
	inline VOID SetChildCount(UINT Count) { uChildCount=Count; }
	inline VOID SetLevel(UINT Level) { uLevel=Level; }
	BOOL SplitChild(UINT Position)
		{
		if(uChildCount==_GroupSize)
			return false;
		ASSERT(Position<uChildCount);
		UINT uspace=Position+1;
		MoveMemory(&ppChildren[uspace+1], &ppChildren[uspace], (uChildCount-uspace)*sizeof(VOID*));
		if(uLevel==1)
			{
			ppChildren[uspace]=new ITEMGROUP();
			}
		else
			{
			ppChildren[uspace]=new PARENTGROUP(uLevel-1);
			}
		uChildCount++;
		return true;
		}

protected:
	// Con-/Destructors
	ParentGroupBase():
		uChildCount(0),
		uItemCount(0),
		uLevel(0)
		{}
	ParentGroupBase(UINT Level):
		uChildCount(0),
		uItemCount(0),
		uLevel(Level)
		{}
	ParentGroupBase(GROUP* Child):
		uChildCount(1),
		uItemCount(Child->GetItemCount()),
		uLevel(Child->GetLevel()+1)
		{
		ppChildren[0]=Child;
		}
	ParentGroupBase(ParentGroupBase const& Group):
		uChildCount(Group.uChildCount),
		uItemCount(Group.GetItemCount()),
		uLevel(Group.GetLevel())
		{
		if(uLevel>1)
			{
			for(UINT u=0; u<uChildCount; u++)
				ppChildren[u]=new PARENTGROUP((PARENTGROUP const&)*Group.ppChildren[u]);
			}
		else
			{
			for(UINT u=0; u<uChildCount; u++)
				ppChildren[u]=new ITEMGROUP((ITEMGROUP const&)*Group.ppChildren[u]);
			}
		}
	~ParentGroupBase()override
		{
		for(UINT u=0; u<uChildCount; u++)
				delete ppChildren[u];
		}

	// Common
	UINT GetCombination(UINT Position, UINT* Destination)
		{
		UINT ucur=ppChildren[Position]->GetChildCount();
		if(ucur==0)
			{
			*Destination=ucur;
			return 0;
			}
		UINT ubefore=_GroupSize+1;
		if(Position>0)
			ubefore=ppChildren[Position-1]->GetChildCount();
		if(ubefore+ucur<=_GroupSize)
			{
			*Destination=Position-1;
			return ucur;
			}
		UINT uafter=_GroupSize+1;
		if(Position<uChildCount-1)
			uafter=ppChildren[Position+1]->GetChildCount();
		if(ucur+uafter<=_GroupSize)
			{
			*Destination=Position+1;
			return ucur;
			}
		return 0;
		}
	UINT GetGroup(UINT64& Position)const
		{
		for(UINT u=0; u<uChildCount; u++)
			{
			UINT64 ucount=ppChildren[u]->GetItemCount();
			if(Position<ucount)
				return u;
			Position-=ucount;
			}
		return _GroupSize;
		}
	template <class RET> RET GetAtInternal(UINT64 Position)const
		{
		UINT ugroup=GetGroup(Position);
		ASSERT(ugroup<uChildCount);
		return ppChildren[ugroup]->GetAt(Position);
		}
	UINT GetNearestGroup(UINT Position, UINT* Nearest)
		{
		INT ibefore=Position-1;
		UINT uafter=Position+1;
		while(ibefore>=0||uafter<uChildCount)
			{
			if(ibefore>=0)
				{
				if(ppChildren[ibefore]->GetChildCount()<_GroupSize)
					{
					*Nearest=ibefore;
					return Position-ibefore;
					}
				ibefore--;
				}
			if(uafter<uChildCount)
				{
				if(ppChildren[uafter]->GetChildCount()<_GroupSize)
					{
					*Nearest=uafter;
					return uafter-Position;
					}
				uafter++;
				}
			}
		return 0;
		}
	template <class RET> RET ReleaseAtInternal(UINT64 Position)
		{
		UINT ugroup=GetGroup(Position);
		ASSERT(ugroup<uChildCount);
		RET pitem=ppChildren[ugroup]->ReleaseAt(Position);
		uItemCount--;
		CombineChildren(ugroup);
		return pitem;
		}
	GROUP* ppChildren[_GroupSize];
	UINT uChildCount;
	UINT64 uItemCount;
	UINT uLevel;
};


//======================
// Parent-Group Cluster
//======================

// Parent-Group Cluster
template <class ITEM, class GROUP, class ITEMGROUP, class PARENTGROUP, UINT _GroupSize>
class ParentGroup: public ParentGroupBase<ITEM, GROUP, ITEMGROUP, PARENTGROUP, _GroupSize>
{
public:
	// Access
	inline ITEM* GetAt(UINT64 Position)override { return GetAtInternal<ITEM*>(Position); }
	inline ITEM const* GetAt(UINT64 Position)const override { return GetAtInternal<ITEM const*>(Position); }

protected:
	// Con-/Destructors
	using PARENTGROUPBASE=ParentGroupBase<ITEM, GROUP, ITEMGROUP, PARENTGROUP, _GroupSize>;
	using PARENTGROUPBASE::PARENTGROUPBASE;
};

// Parent-Group Pointer-Cluster
template <class ITEM, class GROUP, class ITEMGROUP, class PARENTGROUP, UINT _GroupSize>
class ParentGroup<ITEM*, GROUP, ITEMGROUP, PARENTGROUP, _GroupSize>: public ParentGroupBase<ITEM*, GROUP, ITEMGROUP, PARENTGROUP, _GroupSize>
{
public:
	// Access
	inline ITEM* GetAt(UINT64 Position)const override { return GetAtInternal<ITEM*>(Position); }

	// Modification
	inline ITEM* ReleaseAt(UINT64 Position)override { return ReleaseAtInternal<ITEM*>(Position); }

protected:
	// Con-/Destructors
	using PARENTGROUPBASE=ParentGroupBase<ITEM*, GROUP, ITEMGROUP, PARENTGROUP, _GroupSize>;
	using PARENTGROUPBASE::PARENTGROUPBASE;
};

#ifdef __cplusplus_winrt
// Parent-Group Handle-Cluster
template <class ITEM, class GROUP, class ITEMGROUP, class PARENTGROUP, UINT _GroupSize>
class ParentGroup<ITEM^, GROUP, ITEMGROUP, PARENTGROUP, _GroupSize>: public ParentGroupBase<ITEM^, GROUP, ITEMGROUP, PARENTGROUP, _GroupSize>
{
public:
	// Access
	inline ITEM^ GetAt(UINT64 Position)const override { return GetAtInternal<ITEM^>(Position); }

protected:
	// Con-/Destructors
	using PARENTGROUPBASE=ParentGroupBase<ITEM^, GROUP, ITEMGROUP, PARENTGROUP, _GroupSize>;
	using PARENTGROUPBASE::PARENTGROUPBASE;
};
#endif

// Parent-Group String-Cluster
template <class CHAR, BOOL _Alloc, class GROUP, class ITEMGROUP, class PARENTGROUP, UINT _GroupSize>
class ParentGroup<String<CHAR, _Alloc>, GROUP, ITEMGROUP, PARENTGROUP, _GroupSize>: public ParentGroupBase<String<CHAR, _Alloc>, GROUP, ITEMGROUP, PARENTGROUP, _GroupSize>
{
public:
	// Access
	inline CHAR const* GetAt(UINT64 Position)const override { return GetAtInternal<CHAR const*>(Position); }

	// Modification
	inline CHAR const* ReleaseAt(UINT64 Position)override { return ReleaseAtInternal<CHAR const*>(Position); }

protected:
	// Con-/Destructors
	using PARENTGROUPBASE=ParentGroupBase<String<CHAR, _Alloc>, GROUP, ITEMGROUP, PARENTGROUP, _GroupSize>;
	using PARENTGROUPBASE::PARENTGROUPBASE;
};

}}}
