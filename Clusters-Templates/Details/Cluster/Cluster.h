//===========
// Cluster.h
//===========

#pragma once


//=======
// Using
//=======

#include "..\StringClass.h"


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
			template <class _Id, class _Item, unsigned int _GroupSize, bool _ReadOnly> class IndexIteratorBase;
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
	template <class _Item, class _Group, class _ItemGroup, class _ParentGroup, bool _ReadOnly> friend class IteratorBase;
	template <class _Item, class _Group, class _ItemGroup, class _ParentGroup, bool _ReadOnly> friend class IteratorShared;
	template <class _Id, class _Item, unsigned int _GroupSize, bool _ReadOnly> friend class ::Clusters::Templates::Details::Index::IndexIteratorBase;

	// Access
	operator bool()const
		{
		if(!PointerValid(this))
			return false;
		return pRoot->GetCount()>0;
		}
	virtual inline size_t GetCount()const
		{
		return pRoot->GetItemCount();
		}

	// Modification
	void Clear()
		{
		auto proot=pRoot;
		pRoot=new _ItemGroup();
		delete proot;
		}

protected:
	// Con-/Destructors
	ClusterBase(): pRoot(new _ItemGroup()) {}
	ClusterBase(ClusterBase const& Cluster)
		{
		if(Cluster.pRoot->GetLevel()>1)
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
	void UpdateRoot()
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
	// Access
	inline _ItW At(size_t Position) { return _ItW(this, Position); }
	inline _ItR At(size_t Position)const { return _ItR(this, Position); }
	inline _ItR At(_ItR const& It)const { return _ItR(It); }
	inline _ItR At(_ItW const& It)const { return _ItR(It); }
	inline _ItW First() { return _ItW(this, 0); }
	inline _ItR First()const { return _ItR(this, 0); }
	inline _ItW Last() { return _ItW(this, pRoot->GetItemCount()-1); }
	inline _ItR Last()const { return _ItR(this, pRoot->GetItemCount()-1); }
};

// Pointer-Cluster
template <class _Item, class _Group, class _ItemGroup, class _ParentGroup, class _ItR, class _ItW>
class Cluster<_Item*, _Group, _ItemGroup, _ParentGroup, _ItR, _ItW>: public ClusterBase<_Group, _ItemGroup, _ParentGroup>
{
public:
	// Access
	inline _ItW At(size_t Position) { return _ItW(this, Position); }
	inline _ItR At(size_t Position)const { return _ItR(this, Position); }
	inline _ItR At(_ItR const& It)const { return _ItR(It); }
	inline _ItR At(_ItW const& It)const { return _ItR(It); }
	inline _ItW First() { return _ItW(this, 0); }
	inline _ItR First()const { return _ItR(this, 0); }
	inline _ItW Last() { return _ItW(this, pRoot->GetItemCount()-1); }
	inline _ItR Last()const { return _ItR(this, pRoot->GetItemCount()-1); }
};


#ifdef __cplusplus_winrt
// Handle-Cluster
template <class _Item, class _Group, class _ItemGroup, class _ParentGroup, class _ItR, class _ItW>
class Cluster<_Item^, _Group, _ItemGroup, _ParentGroup, _ItR, _ItW>: public ClusterBase<_Group, _ItemGroup, _ParentGroup>
{
public:
	// Access
	inline _ItW At(size_t Position) { return _ItW(this, Position); }
	inline _ItR At(size_t Position)const { return _ItR(this, Position); }
	inline _ItR At(_ItR const& It)const { return _ItR(It); }
	inline _ItR At(_ItW const& It)const { return _ItR(It); }
	inline _ItW First() { return _ItW(this, 0); }
	inline _ItR First()const { return _ItR(this, 0); }
	inline _ItW Last() { return _ItW(this, pRoot->GetItemCount()-1); }
	inline _ItR Last()const { return _ItR(this, pRoot->GetItemCount()-1); }
};
#endif

// String-Cluster
template <class _Char, bool _Alloc, class _Group, class _ItemGroup, class _ParentGroup, class _ItR, class _ItW>
class Cluster<String<_Char, _Alloc>, _Group, _ItemGroup, _ParentGroup, _ItR, _ItW>: public ClusterBase<_Group, _ItemGroup, _ParentGroup>
{
public:
	// Access
	inline _ItW At(size_t Position) { return _ItW(this, Position); }
	inline _ItR At(size_t Position)const { return _ItR(this, Position); }
	inline _ItR At(_ItR const& It)const { return _ItR(It); }
	inline _ItR At(_ItW const& It)const { return _ItR(It); }
	inline _ItW First() { return _ItW(this, 0); }
	inline _ItR First()const { return _ItR(this, 0); }
	inline _ItW Last() { return _ItW(this, pRoot->GetItemCount()-1); }
	inline _ItR Last()const { return _ItR(this, pRoot->GetItemCount()-1); }
};

}}}}
