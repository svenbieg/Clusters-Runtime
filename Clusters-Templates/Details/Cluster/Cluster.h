//===========
// Cluster.h
//===========

#pragma once


//=======
// Using
//=======

#include "../StringClass.h"


//======================
// Forward-Declarations
//======================

namespace Clusters
{
namespace Templates
	{
	namespace Details
		{
		namespace Index
			{
			template <class _Id, class _Item, UINT _GroupSize, BOOL _ReadOnly> class IndexIteratorBase;
			}
		}
	}
}


//===========
// Namespace
//===========

namespace Clusters {
	namespace Templates {
		namespace Details {
			namespace Cluster {


//====================
// Base-Class Cluster
//====================

template <class _Group, class _ItemGroup, class _ParentGroup>
class ClusterBase
{
public:
	// Friends
	template <class __Item, class __Group, class __ItemGroup, class __ParentGroup, BOOL __ReadOnly> friend class IteratorBase;
	template <class __Item, class __Group, class __ItemGroup, class __ParentGroup, BOOL __ReadOnly> friend class IteratorShared;
	template <class __Id, class __Item, UINT __GroupSize, BOOL __ReadOnly> friend class Clusters::Templates::Details::Index::IndexIteratorBase;

	// Access
	operator BOOL()const
		{
		if(!PointerValid(this))
			return false;
		return pRoot->GetCount()>0;
		}
	virtual inline SIZE_T GetCount()const
		{
		return pRoot->GetItemCount();
		}

	// Modification
	VOID Clear()
		{
		auto proot=pRoot;
		pRoot=new _ItemGroup();
		delete proot;
		}
	VOID RemoveAt(SIZE_T Position)
		{
		pRoot->RemoveAt(Position);
		UpdateRoot();
		}

protected:
	// Con-/Destructors
	ClusterBase(): pRoot(new _ItemGroup()) {}
	ClusterBase(ClusterBase const& Cluster)
		{
		if(Cluster.pRoot->GetLevel()>0)
			{
			pRoot=new _ParentGroup((_ParentGroup const&)*Cluster.pRoot);
			}
		else
			{
			pRoot=new _ItemGroup((_ItemGroup const&)*Cluster.pRoot);
			}
		}
	~ClusterBase() { delete pRoot; }

	// Common
	VOID UpdateRoot()
		{
		if(pRoot->GetChildCount()==1&&pRoot->GetLevel()>0)
			{
			auto proot=(_ParentGroup*)pRoot;
			pRoot=proot->GetChild(0);
			proot->SetChildCount(0);
			delete proot;
			}
		}
	_Group* pRoot;
};


//=========
// Cluster
//=========

// Cluster
template <class _Item, class _Group, class _ItemGroup, class _ParentGroup, class _ItR, class _ItW>
class Cluster: public ClusterBase<_Group, _ItemGroup, _ParentGroup>
{
public:
	// Types
	typedef _ItR ConstIterator;
	typedef _ItW Iterator;

	// Access
	inline _ItW At(SIZE_T Position) { return _ItW(this, Position); }
	inline _ItR At(SIZE_T Position)const { return _ItR(this, Position); }
	inline _ItR At(_ItR const& It)const { return _ItR(It); }
	inline _ItR At(_ItW const& It)const { return _ItR(It); }
	inline _ItW First() { return _ItW(this, 0); }
	inline _ItR First()const { return _ItR(this, 0); }
	inline _ItW Last() { return _ItW(this, this->pRoot->GetItemCount()-1); }
	inline _ItR Last()const { return _ItR(this, this->pRoot->GetItemCount()-1); }
};

}}}}
