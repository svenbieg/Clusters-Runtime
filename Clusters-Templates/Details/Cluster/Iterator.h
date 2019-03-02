//============
// Iterator.h
//============

#pragma once


//=======
// Using
//=======

#include "Default/Conditional.h"
#include "Cluster.h"


//===========
// Namespace
//===========

namespace Clusters {
	namespace Templates {
		namespace Details {
			namespace Cluster {


//=============================
// Base-Class Iterator Cluster
//=============================

template <class _Item, class _Group, class _ItemGroup, class _ParentGroup, BOOL _ReadOnly>
class IteratorBase
{
private:
	// Friends
	template <class __Item, class __Group, class __ItemGroup, class __ParentGroup, BOOL __ReadOnly> friend class IteratorBase;
	template <class __Item, class __Group, class __ItemGroup, class __ParentGroup, BOOL __ReadOnly> friend class Iterator;

	// Using
	using _ItemPtr=Conditional<_ReadOnly, _Item const*, _Item*>;
	using _GroupPtr=Conditional<_ReadOnly, _Group const*, _Group*>;
	using _ItemGroupPtr=Conditional<_ReadOnly, _ItemGroup const*, _ItemGroup*>;
	using _ParentGroupPtr=Conditional<_ReadOnly, _ParentGroup const*, _ParentGroup*>;
	using _Cluster=ClusterBase<_Group, _ItemGroup, _ParentGroup>;
	using _ClusterPtr=Conditional<_ReadOnly, _Cluster const*, _Cluster*>;
	using _It=IteratorBase<_Item, _Group, _ItemGroup, _ParentGroup, _ReadOnly>;
	using _ItR=IteratorBase<_Item, _Group, _ItemGroup, _ParentGroup, true>;
	using _ItW=IteratorBase<_Item, _Group, _ItemGroup, _ParentGroup, false>;

public:
	// Access
	virtual SIZE_T GetPosition()
		{
		SIZE_T upos=0;
		for(UINT u=0; u<uLevelCount-1; u++)
			{
			_ParentGroupPtr pgroup=(_ParentGroupPtr)pIts[u].Group;
			UINT ugrouppos=pIts[u].Position;
			for(UINT v=0; v<ugrouppos; v++)
				upos+=pgroup->GetChild(v)->GetItemCount();
			}
		upos+=pIts[uLevelCount-1].Position;
		return upos;
		}
	inline BOOL HasCurrent() { return pCurrent!=nullptr; }
	
	// Modification
	BOOL MoveNext()
		{
		if(pCurrent==nullptr)
			return false;
		ITERATOR* pit=&pIts[uLevelCount-1];
		_ItemGroupPtr pitems=(_ItemGroupPtr)pit->Group;
		UINT ucount=pitems->GetChildCount();
		if(pit->Position+1<ucount)
			{
			pit->Position++;
			pCurrent=pitems->AddressOfItemAt(pit->Position);
			return true;
			}
		for(UINT u=uLevelCount-1; u>0; u--)
			{
			pit=&pIts[u-1];
			_ParentGroupPtr pparent=(_ParentGroupPtr)pit->Group;
			ucount=pparent->GetChildCount();
			if(pit->Position+1>=ucount)
				continue;
			pit->Position++;
			_GroupPtr pgroup=pit->Group;
			for(; u<uLevelCount; u++)
				{
				pparent=(_ParentGroupPtr)pgroup;
				pgroup=pparent->GetChild(pit->Position);
				pIts[u].Group=pgroup;
				pIts[u].Position=0;
				}
			pitems=(_ItemGroupPtr)pgroup;
			pCurrent=pitems->AddressOfItemAt(0);
			return true;
			}
		pCurrent=nullptr;
		return false;
		}
	BOOL MovePrevious()
		{
		if(pCurrent==nullptr)
			return false;
		ITERATOR* pit=&pIts[uLevelCount-1];
		_ItemGroupPtr pitems=(_ItemGroupPtr)pit->Group;
		if(pit->Position>0)
			{
			pit->Position--;
			pCurrent=pitems->AddressOfItemAt(pit->Position);
			return true;
			}
		for(UINT u=uLevelCount-1; u>0; u--)
			{
			pit=&pIts[u-1];
			_ParentGroupPtr pparent=(_ParentGroupPtr)pit->Group;
			if(pit->Position==0)
				continue;
			pit->Position--;
			_GroupPtr pgroup=pit->Group;
			UINT upos=0;
			for(; u<uLevelCount; u++)
				{
				pparent=(_ParentGroupPtr)pgroup;
				pgroup=pparent->GetChild(pit->Position);
				upos=pgroup->GetChildCount()-1;
				pit=&pIts[u];
				pit->Group=pgroup;
				pit->Position=upos;
				}
			pitems=(_ItemGroupPtr)pgroup;
			pCurrent=pitems->AddressOfItemAt(upos);
			return true;
			}
		pCurrent=nullptr;
		return false;
		}
	VOID SetPosition(SIZE_T Position)
		{
		pCurrent=nullptr;
		_GroupPtr pgroup=pCluster->pRoot;
		SetLevelCount(pgroup->GetLevel()+1);
		INT ipos=GetPosition(pgroup, &Position);
		if(ipos==-1)
			return;
		pIts[0].Group=pgroup;
		pIts[0].Position=ipos;
		for(UINT u=0; u<uLevelCount-1; u++)
			{
			_ParentGroupPtr pparent=(_ParentGroupPtr)pIts[u].Group;
			pgroup=pparent->GetChild(ipos);
			ipos=GetPosition(pgroup, &Position);
			if(ipos==-1)
				return;
			pIts[u+1].Group=pgroup;
			pIts[u+1].Position=ipos;
			}
		INT ichildcount=pgroup->GetChildCount();
		if(ipos<ichildcount)
			{
			_ItemGroupPtr pitems=(_ItemGroupPtr)pgroup;
			pCurrent=pitems->AddressOfItemAt(ipos);
			}
		}

protected:
	// Helper-Struct
	typedef struct
		{
		_GroupPtr Group;
		UINT Position;
		}ITERATOR;

	// Con-/Destructors
	IteratorBase(_ClusterPtr Cluster): pCluster(Cluster), pCurrent(nullptr), pIts(nullptr), uLevelCount(0) {}
	~IteratorBase() { if(pIts)delete pIts; }

	// Common
	template <class __It> VOID Assign(__It const& It)
		{
		pCluster=It.pCluster;
		pCurrent=It.pCurrent;
		SetLevelCount(It.uLevelCount);
		CopyMemory(pIts, It.pIts, uLevelCount*sizeof(ITERATOR));
		}
	VOID SetLevelCount(UINT LevelCount)
		{
		if(uLevelCount==LevelCount)
			return;
		operator delete(pIts);
		pIts=(ITERATOR*)operator new(LevelCount*sizeof(ITERATOR));
		uLevelCount=LevelCount;
		}
	_ClusterPtr pCluster;
	_ItemPtr pCurrent;
	ITERATOR* pIts;
	UINT uLevelCount;

protected:
	// Common
	INT GetPosition(_GroupPtr Group, SIZE_T* Position)
		{
		UINT ucount=Group->GetChildCount();
		UINT ulevel=Group->GetLevel();
		if(ulevel==0)
			{
			UINT upos=(UINT)*Position;
			*Position=0;
			return upos;
			}
		_ParentGroupPtr pparent=(_ParentGroupPtr)Group;
		SIZE_T uitemcount=0;
		for(UINT u=0; u<ucount; u++)
			{
			_GroupPtr pchild=pparent->GetChild(u);
			uitemcount=pchild->GetItemCount();
			if(*Position<uitemcount)
				return u;
			*Position-=uitemcount;
			}
		return -1;
		}
};


//=========================
// Iterator Cluster Shared
//=========================

// Iterator Cluster Read-Only
template <class _Item, class _Group, class _ItemGroup, class _ParentGroup, BOOL _ReadOnly>
class IteratorShared: public IteratorBase<_Item, _Group, _ItemGroup, _ParentGroup, true>
{
private:
	// Using
	using _ItR=IteratorBase<_Item, _Group, _ItemGroup, _ParentGroup, true>;
	using _ItW=IteratorBase<_Item, _Group, _ItemGroup, _ParentGroup, false>;
	using _IteratorBase=IteratorBase<_Item, _Group, _ItemGroup, _ParentGroup, true>;
	using _Cluster=ClusterBase<_Group, _ItemGroup, _ParentGroup>;

protected:
	// Con-/Destructors
	IteratorShared(_ItR const& It): _IteratorBase(It.pCluster) { this->Assign(It); }
	IteratorShared(_ItW const& It): _IteratorBase(It.pCluster) { this->Assign(It); }
	IteratorShared(_Cluster const* Cluster): _IteratorBase(Cluster) {}
};

// Iterator Cluster Read-Write
template <class _Item, class _Group, class _ItemGroup, class _ParentGroup>
class IteratorShared<_Item, _Group, _ItemGroup, _ParentGroup, false>: public IteratorBase<_Item, _Group, _ItemGroup, _ParentGroup, false>
{
private:
	// Using
	using _ItW=IteratorBase<_Item, _Group, _ItemGroup, _ParentGroup, false>;
	using _IteratorBase=IteratorBase<_Item, _Group, _ItemGroup, _ParentGroup, false>;
	using _Cluster=ClusterBase<_Group, _ItemGroup, _ParentGroup>;

public:
	// Common
	VOID RemoveCurrent()
		{
		SIZE_T upos=this->GetPosition();
		this->pCluster->RemoveAt(upos);
		this->pCurrent=nullptr;
		this->SetPosition(upos);
		}

protected:
	// Con-/Destructors
	IteratorShared(_ItW const& It): _IteratorBase(It.pCluster) { this->Assign(It); }
	IteratorShared(_Cluster* Cluster): _IteratorBase(Cluster) {}
};

}}}}
