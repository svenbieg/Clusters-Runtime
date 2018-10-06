//============
// Iterator.h
//============

#pragma once


//=======
// Using
//=======

#include "Default\Conditional.h"
#include "Cluster.h"


//===========
// Namespace
//===========

namespace Clusters {
	namespace Details {
		namespace Cluster {


//=============================
// Base-Class Iterator Cluster
//=============================

template <class ITEM, class GROUP, class ITEMGROUP, class PARENTGROUP, BOOL _ReadOnly>
class IteratorBase
{
protected:
	// Friends
	template <class ITEM, class GROUP, class ITEMGROUP, class PARENTGROUP, BOOL _ReadOnly> friend class IteratorBase;
	template <class ITEM, class GROUP, class ITEMGROUP, class PARENTGROUP, BOOL _ReadOnly> friend class IteratorReadWrite;

	// Using
	using ITEM_PTR=Conditional<_ReadOnly, ITEM const*, ITEM*>;
	using GROUP_PTR=Conditional<_ReadOnly, GROUP const*, GROUP*>;
	using ITEMGROUP_PTR=Conditional<_ReadOnly, ITEMGROUP const*, ITEMGROUP*>;
	using PARENTGROUP_PTR=Conditional<_ReadOnly, PARENTGROUP const*, PARENTGROUP*>;
	using CLUSTER=ClusterBase<GROUP, ITEMGROUP, PARENTGROUP>;
	using CLUSTER_PTR=Conditional<_ReadOnly, CLUSTER const*, CLUSTER*>;
	using IT=IteratorBase<ITEM, GROUP, ITEMGROUP, PARENTGROUP, _ReadOnly>;
	using IT_R=IteratorBase<ITEM, GROUP, ITEMGROUP, PARENTGROUP, true>;
	using IT_W=IteratorBase<ITEM, GROUP, ITEMGROUP, PARENTGROUP, false>;

public:
	// Access
	virtual UINT64 GetPosition()
		{
		UINT ulevelcount=pCluster->pRoot->GetLevel()+1;
		UINT64 upos=0;
		for(UINT u=0; u<ulevelcount-1; u++)
			{
			PARENTGROUP_PTR pgroup=(PARENTGROUP_PTR)pIts[u].Group;
			UINT ugrouppos=pIts[u].Position;
			for(UINT v=0; v<ugrouppos; v++)
				upos+=pgroup->GetChild(v)->GetItemCount();
			}
		upos+=pIts[ulevelcount-1].Position;
		return upos;
		}
	inline BOOL HasCurrent() { return PointerValid(pCurrent); }
	
	// Common
	template <class IT> VOID Initialize(IT const& It)
		{
		UINT ulevelcount=pCluster->pRoot->GetLevel()+1;
		pIts=new ITERATOR[ulevelcount];
		CopyMemory(pIts, It.pIts, ulevelcount*sizeof(ITERATOR));
		pCurrent=It.pCurrent;
		}
	BOOL MoveNext()
		{
		UINT ulevelcount=pCluster->pRoot->GetLevel()+1;
		ITERATOR* pit=&pIts[ulevelcount-1];
		ITEMGROUP_PTR pitems=(ITEMGROUP_PTR)pit->Group;
		UINT ucount=pitems->GetChildCount();
		if(pit->Position+1<ucount)
			{
			pit->Position++;
			pCurrent=pitems->AddressOfItemAt(pit->Position);
			return true;
			}
		for(UINT u=ulevelcount-1; u>0; u--)
			{
			pit=&pIts[u-1];
			PARENTGROUP_PTR pparent=(PARENTGROUP_PTR)pit->Group;
			ucount=pparent->GetChildCount();
			if(pit->Position+1>=ucount)
				continue;
			pit->Position++;
			GROUP_PTR pgroup=pit->Group;
			for(; u<ulevelcount; u++)
				{
				pparent=(PARENTGROUP_PTR)pgroup;
				pgroup=pparent->GetChild(pit->Position);
				pit=&pIts[u];
				pit->Group=pgroup;
				pit->Position=0;
				}
			pitems=(ITEMGROUP_PTR)pgroup;
			pCurrent=pitems->AddressOfItemAt(0);
			return true;
			}
		pCurrent=nullptr;
		return false;
		}
	BOOL MovePrevious()
		{
		UINT ulevelcount=pCluster->pRoot->GetLevel()+1;
		ITERATOR* pit=&pIts[ulevelcount-1];
		ITEMGROUP_PTR pitems=(ITEMGROUP_PTR)pit->Group;
		if(pit->Position>0)
			{
			pit->Position--;
			pCurrent=pitems->AddressOfItemAt(pit->Position);
			return true;
			}
		for(UINT u=ulevelcount-1; u>0; u--)
			{
			pit=&pIts[u-1];
			PARENTGROUP_PTR pparent=(PARENTGROUP_PTR)pit->Group;
			if(pit->Position==0)
				continue;
			pit->Position--;
			GROUP_PTR pgroup=pit->Group;
			UINT upos=0;
			for(; u<ulevelcount; u++)
				{
				pparent=(PARENTGROUP_PTR)pgroup;
				pgroup=pparent->GetChild(pit->Position);
				upos=pgroup->GetChildCount()-1;
				pit=&pIts[u];
				pit->Group=pgroup;
				pit->Position=upos;
				}
			pitems=(ITEMGROUP_PTR)pgroup;
			pCurrent=pitems->AddressOfItemAt(upos);
			return true;
			}
		pCurrent=nullptr;
		return false;
		}
	VOID SetPosition(UINT64 Position)
		{
		pCurrent=nullptr;
		GROUP_PTR pgroup=pCluster->pRoot;
		UINT ulevelcount=pgroup->GetLevel()+1;
		if(pIts)
			delete pIts;
		pIts=new ITERATOR[ulevelcount];
		UINT upos=GetPosition(pgroup, &Position);
		pIts[0].Group=pgroup;
		pIts[0].Position=upos;
		for(UINT u=0; u<ulevelcount-1; u++)
			{
			PARENTGROUP_PTR pparent=(PARENTGROUP_PTR)pIts[u].Group;
			pgroup=pparent->GetChild(upos);
			upos=GetPosition(pgroup, &Position);
			pIts[u+1].Group=pgroup;
			pIts[u+1].Position=upos;
			}
		UINT uchildcount=pgroup->GetChildCount();
		if(upos<uchildcount)
			{
			ITEMGROUP_PTR pitems=(ITEMGROUP_PTR)pgroup;
			pCurrent=pitems->AddressOfItemAt(upos);
			}
		}

protected:
	// Helper-Struct
	typedef struct
		{
		GROUP_PTR Group;
		UINT Position;
		}ITERATOR;

	// Con-/Destructors
	IteratorBase(CLUSTER_PTR Cluster): pCluster(Cluster), pCurrent(nullptr), pIts(nullptr) { pCluster->cAccessControl.AccessRead(); }
	~IteratorBase() { PointerFree(pIts); pCluster->cAccessControl.ReleaseRead(); }

	// Common
	CLUSTER_PTR pCluster;
	ITEM_PTR pCurrent;
	ITERATOR* pIts;

private:
	// Common
	UINT GetPosition(GROUP_PTR Group, UINT64* Position)
		{
		UINT ucount=Group->GetChildCount();
		UINT ulevel=Group->GetLevel();
		if(ulevel==0)
			{
			UINT upos=(UINT)*Position;
			*Position=0;
			return upos;
			}
		PARENTGROUP_PTR pparent=(PARENTGROUP_PTR)Group;
		for(UINT u=0; u<ucount; u++)
			{
			GROUP_PTR pchild=pparent->GetChild(u);
			UINT64 uitemcount=pchild->GetItemCount();
			if(*Position<uitemcount)
				return u;
			*Position-=uitemcount;
			}
		return ucount;
		}
};


//=========================
// Iterator Cluster Shared
//=========================

// Iterator Cluster Read-Only
template <class ITEM, class GROUP, class ITEMGROUP, class PARENTGROUP, BOOL _ReadOnly>
class IteratorShared: public IteratorBase<ITEM, GROUP, ITEMGROUP, PARENTGROUP, true>
{
protected:
	// Con-/Destructors
	IteratorShared(IT_R const& It): IteratorBase(It.pCluster) { Initialize(It); }
	IteratorShared(CLUSTER const* Cluster): IteratorBase(Cluster) {}
	IteratorShared(CLUSTER const* Cluster, UINT64 Position): IteratorBase(Cluster) { SetPosition(Position); }
};

// Iterator Cluster Read-Write
template <class ITEM, class GROUP, class ITEMGROUP, class PARENTGROUP>
class IteratorShared<ITEM, GROUP, ITEMGROUP, PARENTGROUP, false>: public IteratorBase<ITEM, GROUP, ITEMGROUP, PARENTGROUP, false>
{
public:
	// Common
	VOID RemoveCurrent()
		{
		UINT64 upos=GetPosition();
			{
			ScopedWrite lock(pCluster->cAccessControl);
			pCluster->pRoot->RemoveAt(upos);
			}
		UINT64 ucount=pCluster->GetCount();
		if(upos>=ucount)
			{
			pCurrent=nullptr;
			return;
			}
		SetPosition(upos);
		}

protected:
	// Con-/Destructors
	IteratorShared(IT_W const& It): IteratorBase(It.pCluster) { Initialize(It); }
	IteratorShared(CLUSTER* Cluster): IteratorBase(Cluster) {}
	IteratorShared(CLUSTER* Cluster, UINT64 Position): IteratorBase(Cluster) { SetPosition(Position); }
};

}}}
