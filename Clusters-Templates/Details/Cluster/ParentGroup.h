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

template <class _Item, class _Group, class _ItemGroup, class _ParentGroup, unsigned int _GroupSize>
class ParentGroupBase: public _Group
{
public:
	// Access
	inline _Group* GetChild(unsigned int Position)
		{
		ASSERT(Position<uChildCount);
		return ppChildren[Position];
		}
	inline _Group const* GetChild(unsigned int Position)const
		{
		ASSERT(Position<uChildCount);
		return ppChildren[Position];
		}
	inline unsigned int GetChildCount()const override { return uChildCount; }
	inline _Group* const* GetChildren()const { return ppChildren; }
	inline size_t GetItemCount()const override { return uItemCount; }
	inline unsigned int GetLevel()const override { return uLevel; }

	// Modification
	virtual void AppendGroups(_Group *const* Groups, unsigned int Count)
		{
		ASSERT(uChildCount+Count<=_GroupSize);
		size_t uaddcount=0;
		for(unsigned int u=0; u<Count; u++)
			uaddcount+=Groups[u]->GetItemCount();
		CopyMemory(&ppChildren[uChildCount], Groups, Count*sizeof(_Group*));
		uChildCount+=Count;
		uItemCount+=uaddcount;
		}
	bool CombineChildren(unsigned int Position)
		{
		unsigned int ucountat=ppChildren[Position]->GetChildCount();
		if(Position>0)
			{
			if(ucountat+ppChildren[Position-1]->GetChildCount()<=_GroupSize)
				{
				MoveChildren(Position, Position-1, ucountat);
				MoveMemory(&ppChildren[Position], &ppChildren[Position+1], (uChildCount-Position-1)*sizeof(void*));
				uChildCount--;
				return true;
				}
			}
		if(Position+1<uChildCount)
			{
			if(ucountat+ppChildren[Position+1]->GetChildCount()<=_GroupSize)
				{
				MoveChildren(Position, Position+1, ucountat);
				MoveMemory(&ppChildren[Position], &ppChildren[Position+1], (uChildCount-Position-1)*sizeof(void*));
				uChildCount--;
				return true;
				}
			}
		return false;
		}
	virtual void InsertGroups(unsigned int Position, _Group *const* Groups, unsigned int Count)
		{
		ASSERT(Position<=uChildCount);
		if(Position==uChildCount)
			return AppendGroups(Groups, Count);
		ASSERT(uChildCount+Count<=_GroupSize);
		size_t uaddcount=0;
		for(unsigned int u=0; u<Count; u++)
			uaddcount+=Groups[u]->GetItemCount();
		MoveMemory(&ppChildren[Position+Count], &ppChildren[Position], (uChildCount-Position)*sizeof(_Group*));
		CopyMemory(&ppChildren[Position], Groups, Count*sizeof(_Group*));
		uItemCount+=uaddcount;
		uChildCount+=Count;
		}
	void MoveChildren(unsigned int Source, unsigned int Destination, unsigned int Count)
		{
		ASSERT(Source!=Destination);
		if(Count==0)
			return;
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
				unsigned int usrccount=psrcp->GetChildCount();
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
				unsigned int usrccount=psrci->GetChildCount();
				pdsti->InsertAt(0, &pitems[usrccount-Count], Count, true);
				psrci->RemoveAt(usrccount-Count, Count, true);
				}
			}
		}
	virtual void RemoveAt(size_t Position)override
		{
		unsigned int ugroup=GetGroup(Position);
		ASSERT(ugroup<uChildCount);
		ppChildren[ugroup]->RemoveAt(Position);
		uItemCount--;
		CombineChildren(ugroup);
		}
	virtual void RemoveGroups(unsigned int Position, unsigned int Count)
		{
		ASSERT(Position+Count<=uChildCount);
		size_t uremcount=0;
		for(unsigned int u=0; u<Count; u++)
			uremcount+=ppChildren[Position+u]->GetItemCount();
		MoveMemory(&ppChildren[Position], &ppChildren[Position+Count], (uChildCount-Count-Position)*sizeof(_Group*));
		uItemCount-=uremcount;
		uChildCount-=Count;
		}
	inline void SetChildCount(unsigned int Count) { uChildCount=Count; }
	inline void SetLevel(unsigned int Level) { uLevel=Level; }
	bool SplitChild(unsigned int Position)
		{
		if(uChildCount==_GroupSize)
			return false;
		ASSERT(Position<uChildCount);
		unsigned int uspace=Position+1;
		MoveMemory(&ppChildren[uspace+1], &ppChildren[uspace], (uChildCount-uspace)*sizeof(void*));
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
	ParentGroupBase(unsigned int Level):
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
			for(unsigned int u=0; u<uChildCount; u++)
				ppChildren[u]=new _ParentGroup((_ParentGroup const&)*Group.ppChildren[u]);
			}
		else
			{
			for(unsigned int u=0; u<uChildCount; u++)
				ppChildren[u]=new _ItemGroup((_ItemGroup const&)*Group.ppChildren[u]);
			}
		}
	~ParentGroupBase()override
		{
		for(unsigned int u=0; u<uChildCount; u++)
				delete ppChildren[u];
		}

	// Common
	unsigned int GetGroup(size_t& Position)const
		{
		for(unsigned int u=0; u<uChildCount; u++)
			{
			size_t ucount=ppChildren[u]->GetItemCount();
			if(Position<ucount)
				return u;
			Position-=ucount;
			}
		return _GroupSize;
		}
	template <class _Ret> _Ret GetAtInternal(size_t Position)const
		{
		unsigned int ugroup=GetGroup(Position);
		ASSERT(ugroup<uChildCount);
		return ppChildren[ugroup]->GetAt(Position);
		}
	unsigned int GetNearestGroup(unsigned int Position)
		{
		int ibefore=Position-1;
		unsigned int uafter=Position+1;
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
	template <class _Ret> _Ret ReleaseAtInternal(size_t Position)
		{
		unsigned int ugroup=GetGroup(Position);
		ASSERT(ugroup<uChildCount);
		_Ret pitem=ppChildren[ugroup]->ReleaseAt(Position);
		uItemCount--;
		CombineChildren(ugroup);
		return pitem;
		}
	_Group* ppChildren[_GroupSize];
	unsigned int uChildCount;
	size_t uItemCount;
	unsigned int uLevel;
};


//======================
// Parent-Group Cluster
//======================

// Parent-Group Cluster
template <class _Item, class _Group, class _ItemGroup, class _ParentGroup, unsigned int _GroupSize>
class ParentGroup: public ParentGroupBase<_Item, _Group, _ItemGroup, _ParentGroup, _GroupSize>
{
public:
	// Access
	inline _Item* GetAt(size_t Position)override { return GetAtInternal<_Item*>(Position); }
	inline _Item const* GetAt(size_t Position)const override { return GetAtInternal<_Item const*>(Position); }

protected:
	// Con-/Destructors
	using _ParentGroupBase=ParentGroupBase<_Item, _Group, _ItemGroup, _ParentGroup, _GroupSize>;
	using _ParentGroupBase::_ParentGroupBase;
};

// Parent-Group Pointer-Cluster
template <class _Item, class _Group, class _ItemGroup, class _ParentGroup, unsigned int _GroupSize>
class ParentGroup<_Item*, _Group, _ItemGroup, _ParentGroup, _GroupSize>: public ParentGroupBase<_Item*, _Group, _ItemGroup, _ParentGroup, _GroupSize>
{
public:
	// Access
	inline _Item* GetAt(size_t Position)const override { return GetAtInternal<_Item*>(Position); }

	// Modification
	inline _Item* ReleaseAt(size_t Position)override { return ReleaseAtInternal<_Item*>(Position); }

protected:
	// Con-/Destructors
	using _ParentGroupBase=ParentGroupBase<_Item*, _Group, _ItemGroup, _ParentGroup, _GroupSize>;
	using _ParentGroupBase::_ParentGroupBase;
};

#ifdef __cplusplus_winrt
// Parent-Group Handle-Cluster
template <class _Item, class _Group, class _ItemGroup, class _ParentGroup, unsigned int _GroupSize>
class ParentGroup<_Item^, _Group, _ItemGroup, _ParentGroup, _GroupSize>: public ParentGroupBase<_Item^, _Group, _ItemGroup, _ParentGroup, _GroupSize>
{
public:
	// Access
	inline _Item^ GetAt(size_t Position)const override { return GetAtInternal<_Item^>(Position); }

protected:
	// Con-/Destructors
	using _ParentGroupBase=ParentGroupBase<_Item^, _Group, _ItemGroup, _ParentGroup, _GroupSize>;
	using _ParentGroupBase::_ParentGroupBase;
};
#endif

// Parent-Group String-Cluster
template <class _Char, bool _Alloc, class _Group, class _ItemGroup, class _ParentGroup, unsigned int _GroupSize>
class ParentGroup<String<_Char, _Alloc>, _Group, _ItemGroup, _ParentGroup, _GroupSize>: public ParentGroupBase<String<_Char, _Alloc>, _Group, _ItemGroup, _ParentGroup, _GroupSize>
{
public:
	// Access
	inline _Char const* GetAt(size_t Position)const override { return GetAtInternal<_Char const*>(Position); }

	// Modification
	inline _Char const* ReleaseAt(size_t Position)override { return ReleaseAtInternal<_Char const*>(Position); }

protected:
	// Con-/Destructors
	using _ParentGroupBase=ParentGroupBase<String<_Char, _Alloc>, _Group, _ItemGroup, _ParentGroup, _GroupSize>;
	using _ParentGroupBase::_ParentGroupBase;
};

}}}}
