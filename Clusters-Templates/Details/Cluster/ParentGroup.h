//===============
// ParentGroup.h
//===============

#pragma once


//===========
// Namespace
//===========

namespace Clusters {
	namespace Templates {
		namespace Details {
			namespace Cluster {


//=================================
// Base-Class Parent-Group Cluster
//=================================

template <class _Item, class _Group, class _ItemGroup, class _ParentGroup, UINT _GroupSize>
class ParentGroupBase: public _Group
{
public:
	// Access
	inline _Group* GetChild(UINT Position)
		{
		ASSERT(Position<uChildCount);
		return ppChildren[Position];
		}
	inline _Group const* GetChild(UINT Position)const
		{
		ASSERT(Position<uChildCount);
		return ppChildren[Position];
		}
	inline UINT GetChildCount()const override { return uChildCount; }
	inline _Group* const* GetChildren()const { return ppChildren; }
	inline SIZE_T GetItemCount()const override { return uItemCount; }
	inline UINT GetLevel()const override { return uLevel; }

	// Modification
	virtual VOID AppendGroups(_Group *const* Groups, UINT Count)
		{
		ASSERT(uChildCount+Count<=_GroupSize);
		SIZE_T uaddcount=0;
		for(UINT u=0; u<Count; u++)
			uaddcount+=Groups[u]->GetItemCount();
		CopyMemory(&ppChildren[uChildCount], Groups, Count*sizeof(_Group*));
		uChildCount+=Count;
		uItemCount+=uaddcount;
		}
	BOOL CombineChildren(UINT Position)
		{
		UINT ucountat=ppChildren[Position]->GetChildCount();
		if(Position>0)
			{
			if(ucountat+ppChildren[Position-1]->GetChildCount()<=_GroupSize)
				{
				MoveChildren(Position, Position-1, ucountat);
				RemoveInternal(Position);
				return true;
				}
			}
		if(Position+1<uChildCount)
			{
			if(ucountat+ppChildren[Position+1]->GetChildCount()<=_GroupSize)
				{
				MoveChildren(Position, Position+1, ucountat);
				RemoveInternal(Position);
				return true;
				}
			}
		return false;
		}
	virtual VOID InsertGroups(UINT Position, _Group *const* Groups, UINT Count)
		{
		ASSERT(Position<=uChildCount);
		if(Position==uChildCount)
			return AppendGroups(Groups, Count);
		ASSERT(uChildCount+Count<=_GroupSize);
		SIZE_T uaddcount=0;
		for(UINT u=0; u<Count; u++)
			uaddcount+=Groups[u]->GetItemCount();
		MoveMemory(&ppChildren[Position+Count], &ppChildren[Position], (uChildCount-Position)*sizeof(_Group*));
		CopyMemory(&ppChildren[Position], Groups, Count*sizeof(_Group*));
		uItemCount+=uaddcount;
		uChildCount+=Count;
		}
	VOID MoveChildren(UINT Source, UINT Destination, UINT Count)
		{
		ASSERT(Source!=Destination);
		if(Count==0)
			return;
		_Group* psrc=ppChildren[Source];
		ASSERT(Count<=psrc->GetChildCount());
		_Group* pdst=ppChildren[Destination];
		ASSERT(pdst->GetChildCount()+Count<=_GroupSize);
		if(uLevel>1)
			{
			_ParentGroup* psrcp=(_ParentGroup*)psrc;
			_ParentGroup* pdstp=(_ParentGroup*)pdst;
			if(Source>Destination)
				{
				pdstp->AppendGroups(psrcp->GetChildren(), Count);
				psrcp->RemoveGroups(0, Count);
				}
			else
				{
				_Group* const* ppgroups=psrcp->GetChildren();
				UINT usrccount=psrcp->GetChildCount();
				pdstp->InsertGroups(0, &ppgroups[usrccount-Count], Count);
				psrcp->RemoveGroups(usrccount-Count, Count);
				}
			}
		else
			{
			_ItemGroup* psrci=(_ItemGroup*)psrc;
			_ItemGroup* pdsti=(_ItemGroup*)pdst;
			if(Source>Destination)
				{
				_Item const* pitems=psrci->GetItems();
				pdsti->Append(pitems, Count, true);
				psrci->RemoveAt(0, Count, true);
				}
			else
				{
				_Item const* pitems=psrci->GetItems();
				UINT usrccount=psrci->GetChildCount();
				pdsti->InsertAt(0, &pitems[usrccount-Count], Count, true);
				psrci->RemoveAt(usrccount-Count, Count, true);
				}
			}
		}
	VOID MoveEmptySlot(UINT Source, UINT Destination)
		{
		if(Source<Destination)
			{
			for(UINT u=Source; u<Destination; u++)
				MoveChildren(u+1, u, 1);
			}
		else
			{
			for(UINT u=Source; u>Destination; u--)
				MoveChildren(u-1, u, 1);
			}
		}
	virtual VOID RemoveAt(SIZE_T Position)override
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
		SIZE_T uremcount=0;
		for(UINT u=0; u<Count; u++)
			uremcount+=ppChildren[Position+u]->GetItemCount();
		MoveMemory(&ppChildren[Position], &ppChildren[Position+Count], (uChildCount-Count-Position)*sizeof(_Group*));
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
			ppChildren[uspace]=new _ItemGroup();
			}
		else
			{
			ppChildren[uspace]=new _ParentGroup(uLevel-1);
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
	ParentGroupBase(_Group* Child):
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
				ppChildren[u]=new _ParentGroup((_ParentGroup const&)*Group.ppChildren[u]);
			}
		else
			{
			for(UINT u=0; u<uChildCount; u++)
				ppChildren[u]=new _ItemGroup((_ItemGroup const&)*Group.ppChildren[u]);
			}
		}
	~ParentGroupBase()override
		{
		for(UINT u=0; u<uChildCount; u++)
				delete ppChildren[u];
		}

	// Common
	UINT GetGroup(SIZE_T& Position)const
		{
		for(UINT u=0; u<uChildCount; u++)
			{
			SIZE_T ucount=ppChildren[u]->GetItemCount();
			if(Position<ucount)
				return u;
			Position-=ucount;
			}
		return _GroupSize;
		}
	template <class _Ret> _Ret GetAtInternal(SIZE_T Position)const
		{
		UINT ugroup=GetGroup(Position);
		ASSERT(ugroup<uChildCount);
		return ppChildren[ugroup]->GetAt(Position);
		}
	UINT GetNearestGroup(UINT Position)
		{
		INT ibefore=Position-1;
		UINT uafter=Position+1;
		while(ibefore>=0||uafter<uChildCount)
			{
			if(ibefore>=0)
				{
				if(ppChildren[ibefore]->GetChildCount()<_GroupSize)
					return ibefore;
				ibefore--;
				}
			if(uafter<uChildCount)
				{
				if(ppChildren[uafter]->GetChildCount()<_GroupSize)
					return uafter;
				uafter++;
				}
			}
		return _GroupSize;
		}
	template <class _Ret> _Ret ReleaseAtInternal(SIZE_T Position)
		{
		UINT ugroup=GetGroup(Position);
		ASSERT(ugroup<uChildCount);
		_Ret pitem=ppChildren[ugroup]->ReleaseAt(Position);
		uItemCount--;
		CombineChildren(ugroup);
		return pitem;
		}
	VOID RemoveInternal(UINT Position)
		{
		delete ppChildren[Position];
		if(Position+1<uChildCount)
			MoveMemory(&ppChildren[Position], &ppChildren[Position+1], (uChildCount-Position-1)*sizeof(VOID*));
		uChildCount--;
		}
	_Group* ppChildren[_GroupSize];
	UINT uChildCount;
	SIZE_T uItemCount;
	UINT uLevel;
};


//======================
// Parent-Group Cluster
//======================

// Parent-Group Cluster
template <class _Item, class _Group, class _ItemGroup, class _ParentGroup, UINT _GroupSize>
class ParentGroup: public ParentGroupBase<_Item, _Group, _ItemGroup, _ParentGroup, _GroupSize>
{
private:
	// Using
	using _ParentGroupBase=ParentGroupBase<_Item, _Group, _ItemGroup, _ParentGroup, _GroupSize>;

public:
	// Access
	inline _Item* GetAt(SIZE_T Position)override { return this->template GetAtInternal<_Item*>(Position); }
	inline _Item const* GetAt(SIZE_T Position)const override { return this->template GetAtInternal<_Item const*>(Position); }

protected:
	// Con-/Destructors
	using _ParentGroupBase::_ParentGroupBase;
};

// Parent-Group Pointer-Cluster
template <class _Item, class _Group, class _ItemGroup, class _ParentGroup, UINT _GroupSize>
class ParentGroup<_Item*, _Group, _ItemGroup, _ParentGroup, _GroupSize>: public ParentGroupBase<_Item*, _Group, _ItemGroup, _ParentGroup, _GroupSize>
{
private:
	// Using
	using _ParentGroupBase=ParentGroupBase<_Item*, _Group, _ItemGroup, _ParentGroup, _GroupSize>;

public:
	// Access
	inline _Item* GetAt(SIZE_T Position)const override { return this->template GetAtInternal<_Item*>(Position); }

	// Modification
	inline _Item* ReleaseAt(SIZE_T Position)override { return this->template ReleaseAtInternal<_Item*>(Position); }

protected:
	// Con-/Destructors
	using _ParentGroupBase::_ParentGroupBase;
};

#ifdef __cplusplus_winrt
// Parent-Group Handle-Cluster
template <class _Item, class _Group, class _ItemGroup, class _ParentGroup, UINT _GroupSize>
class ParentGroup<_Item^, _Group, _ItemGroup, _ParentGroup, _GroupSize>: public ParentGroupBase<_Item^, _Group, _ItemGroup, _ParentGroup, _GroupSize>
{
private:
	// Using
	using _ParentGroupBase=ParentGroupBase<_Item^, _Group, _ItemGroup, _ParentGroup, _GroupSize>;

public:
	// Access
	inline _Item^ GetAt(SIZE_T Position)const override { return this->template GetAtInternal<_Item^>(Position); }

protected:
	// Con-/Destructors
	using _ParentGroupBase::_ParentGroupBase;
};
#endif

// Parent-Group String-Cluster
template <class _Char, BOOL _Alloc, class _Group, class _ItemGroup, class _ParentGroup, UINT _GroupSize>
class ParentGroup<StringItem<_Char, _Alloc>, _Group, _ItemGroup, _ParentGroup, _GroupSize>: public ParentGroupBase<StringItem<_Char, _Alloc>, _Group, _ItemGroup, _ParentGroup, _GroupSize>
{
private:
	// Using
	using _ParentGroupBase=ParentGroupBase<StringItem<_Char, _Alloc>, _Group, _ItemGroup, _ParentGroup, _GroupSize>;

public:
	// Access
	inline _Char const* GetAt(SIZE_T Position)const override { return this->template GetAtInternal<_Char const*>(Position); }

	// Modification
	inline _Char const* ReleaseAt(SIZE_T Position)override { return this->template ReleaseAtInternal<_Char const*>(Position); }

protected:
	// Con-/Destructors
	using _ParentGroupBase::_ParentGroupBase;
};

}}}}
