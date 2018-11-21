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
	namespace Templates {
		namespace Details {
			namespace Cluster {


//=============================
// Base-Class Iterator Cluster
//=============================

template <class _Item, class _Group, class _ItemGroup, class _ParentGroup, bool _ReadOnly>
class IteratorBase
{
protected:
	// Friends
	template <class _Item, class _Group, class _ItemGroup, class _ParentGroup, bool _ReadOnly> friend class IteratorBase;
	template <class _Item, class _Group, class _ItemGroup, class _ParentGroup, bool _ReadOnly> friend class IteratorReadWrite;

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
	virtual size_t GetPosition()
		{
		unsigned int ulevelcount=pCluster->pRoot->GetLevel()+1;
		size_t upos=0;
		for(unsigned int u=0; u<ulevelcount-1; u++)
			{
			_ParentGroupPtr pgroup=(_ParentGroupPtr)pIts[u].Group;
			unsigned int ugrouppos=pIts[u].Position;
			for(unsigned int v=0; v<ugrouppos; v++)
				upos+=pgroup->GetChild(v)->GetItemCount();
			}
		upos+=pIts[ulevelcount-1].Position;
		return upos;
		}
	inline bool HasCurrent() { return pCurrent!=nullptr; }
	
	// Common
	template <class _It> void Initialize(_It const& It)
		{
		unsigned int ulevelcount=pCluster->pRoot->GetLevel()+1;
		pIts=new ITERATOR[ulevelcount];
		CopyMemory(pIts, It.pIts, ulevelcount*sizeof(ITERATOR));
		pCurrent=It.pCurrent;
		}
	bool MoveNext()
		{
		if(pCurrent==nullptr)
			return false;
		unsigned int ulevelcount=pCluster->pRoot->GetLevel()+1;
		ITERATOR* pit=&pIts[ulevelcount-1];
		_ItemGroupPtr pitems=(_ItemGroupPtr)pit->Group;
		unsigned int ucount=pitems->GetChildCount();
		if(pit->Position+1<ucount)
			{
			pit->Position++;
			pCurrent=pitems->AddressOfItemAt(pit->Position);
			return true;
			}
		for(unsigned int u=ulevelcount-1; u>0; u--)
			{
			pit=&pIts[u-1];
			_ParentGroupPtr pparent=(_ParentGroupPtr)pit->Group;
			ucount=pparent->GetChildCount();
			if(pit->Position+1>=ucount)
				continue;
			pit->Position++;
			_GroupPtr pgroup=pit->Group;
			for(; u<ulevelcount; u++)
				{
				pparent=(_ParentGroupPtr)pgroup;
				pgroup=pparent->GetChild(pit->Position);
				pit=&pIts[u];
				pit->Group=pgroup;
				pit->Position=0;
				}
			pitems=(_ItemGroupPtr)pgroup;
			pCurrent=pitems->AddressOfItemAt(0);
			return true;
			}
		pCurrent=nullptr;
		return false;
		}
	bool MovePrevious()
		{
		if(pCurrent==nullptr)
			return false;
		unsigned int ulevelcount=pCluster->pRoot->GetLevel()+1;
		ITERATOR* pit=&pIts[ulevelcount-1];
		_ItemGroupPtr pitems=(_ItemGroupPtr)pit->Group;
		if(pit->Position>0)
			{
			pit->Position--;
			pCurrent=pitems->AddressOfItemAt(pit->Position);
			return true;
			}
		for(unsigned int u=ulevelcount-1; u>0; u--)
			{
			pit=&pIts[u-1];
			_ParentGroupPtr pparent=(_ParentGroupPtr)pit->Group;
			if(pit->Position==0)
				continue;
			pit->Position--;
			_GroupPtr pgroup=pit->Group;
			unsigned int upos=0;
			for(; u<ulevelcount; u++)
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
	void SetPosition(size_t Position)
		{
		pCurrent=nullptr;
		_GroupPtr pgroup=pCluster->pRoot;
		unsigned int ulevelcount=pgroup->GetLevel()+1;
		if(pIts)
			delete pIts;
		pIts=new ITERATOR[ulevelcount];
		int ipos=GetPosition(pgroup, &Position);
		if(ipos==-1)
			return;
		pIts[0].Group=pgroup;
		pIts[0].Position=ipos;
		for(unsigned int u=0; u<ulevelcount-1; u++)
			{
			_ParentGroupPtr pparent=(_ParentGroupPtr)pIts[u].Group;
			pgroup=pparent->GetChild(ipos);
			ipos=GetPosition(pgroup, &Position);
			if(ipos==-1)
				return;
			pIts[u+1].Group=pgroup;
			pIts[u+1].Position=ipos;
			}
		int ichildcount=pgroup->GetChildCount();
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
		unsigned int Position;
		}ITERATOR;

	// Con-/Destructors
	IteratorBase(_ClusterPtr Cluster): pCluster(Cluster), pCurrent(nullptr), pIts(nullptr) {}
	~IteratorBase() { if(pIts)delete pIts; }

	// Common
	_ClusterPtr pCluster;
	_ItemPtr pCurrent;
	ITERATOR* pIts;

private:
	// Common
	int GetPosition(_GroupPtr Group, size_t* Position)
		{
		unsigned int ucount=Group->GetChildCount();
		unsigned int ulevel=Group->GetLevel();
		if(ulevel==0)
			{
			unsigned int upos=(unsigned int)*Position;
			*Position=0;
			return upos;
			}
		_ParentGroupPtr pparent=(_ParentGroupPtr)Group;
		size_t uitemcount=0;
		for(unsigned int u=0; u<ucount; u++)
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
template <class _Item, class _Group, class _ItemGroup, class _ParentGroup, bool _ReadOnly>
class IteratorShared: public IteratorBase<_Item, _Group, _ItemGroup, _ParentGroup, true>
{
protected:
	// Con-/Destructors
	IteratorShared(_ItR const& It): IteratorBase(It.pCluster) { Initialize(It); }
	IteratorShared(_ItW const& It): IteratorBase(It.pCluster) { Initialize(It); }
	IteratorShared(_Cluster const* Cluster): IteratorBase(Cluster) {}
	IteratorShared(_Cluster const* Cluster, size_t Position): IteratorBase(Cluster) { SetPosition(Position); }
};

// Iterator Cluster Read-Write
template <class _Item, class _Group, class _ItemGroup, class _ParentGroup>
class IteratorShared<_Item, _Group, _ItemGroup, _ParentGroup, false>: public IteratorBase<_Item, _Group, _ItemGroup, _ParentGroup, false>
{
public:
	// Common
	void RemoveCurrent()
		{
		size_t upos=GetPosition();
		pCluster->pRoot->RemoveAt(upos);
		pCurrent=nullptr;
		SetPosition(upos);
		}

protected:
	// Con-/Destructors
	IteratorShared(_ItW const& It): IteratorBase(It.pCluster) { Initialize(It); }
	IteratorShared(_Cluster* Cluster): IteratorBase(Cluster) {}
	IteratorShared(_Cluster* Cluster, size_t Position): IteratorBase(Cluster) { SetPosition(Position); }
};

}}}}
