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
namespace Details
	{
	namespace Index
		{
		template <class ID, class ITEM, UINT _GroupSize, BOOL _ReadOnly> class IndexIteratorBase;
		}
	}
}


//===========
// Namespace
//===========

namespace Clusters {
	namespace Details {
		namespace Cluster {


//====================
// Base-Class Cluster
//====================

template <class GROUP, class ITEMGROUP, class PARENTGROUP>
class ClusterBase
{
public:
	// Friends
	template <class ITEM, class GROUP, class ITEMGROUP, class PARENTGROUP, BOOL _ReadOnly> friend class IteratorBase;
	template <class ITEM, class GROUP, class ITEMGROUP, class PARENTGROUP, BOOL _ReadOnly> friend class IteratorShared;
	template <class ID, class ITEM, UINT _GroupSize, BOOL _ReadOnly> friend class ::Clusters::Details::Index::IndexIteratorBase;

	// Access
	operator bool()const
		{
		if(!PointerValid(this))
			return false;
		return pRoot->GetCount()>0;
		}
	virtual inline UINT64 GetCount()const
		{
		return pRoot->GetItemCount();
		}

	// Modification
	VOID Clear()
		{
		auto proot=pRoot;
		pRoot=new ITEMGROUP();
		delete proot;
		}

protected:
	// Con-/Destructors
	ClusterBase(): pRoot(new ITEMGROUP()) {}
	ClusterBase(ClusterBase const& Cluster)
		{
		if(Cluster.pRoot->GetLevel()>1)
			{
			pRoot=new PARENTGROUP((PARENTGROUP const&)*Cluster.pRoot);
			}
		else
			{
			pRoot=new ITEMGROUP((ITEMGROUP const&)*Cluster.pRoot);
			}
		}
	~ClusterBase() { delete pRoot; }

	// Common
	VOID UpdateRoot()
		{
		if(pRoot->GetChildCount()==1&&pRoot->GetLevel()>0)
			{
			auto proot=(PARENTGROUP*)pRoot;
			pRoot=proot->GetChild(0);
			proot->SetChildCount(0);
			delete proot;
			}
		}
	GROUP* pRoot;
};


//=========
// Cluster
//=========

// Cluster
template <class ITEM, class GROUP, class ITEMGROUP, class PARENTGROUP, class IT_R, class IT_W>
class Cluster: public ClusterBase<GROUP, ITEMGROUP, PARENTGROUP>
{
public:
	// Access
	inline IT_W At(UINT64 Position) { return IT_W(this, Position); }
	inline IT_R At(UINT64 Position)const { return IT_R(this, Position); }
	inline IT_R At(IT_R const& It)const { return IT_R(It); }
	inline IT_R At(IT_W const& It)const { return IT_R(It); }
	inline IT_W First() { return IT_W(this, 0); }
	inline IT_R First()const { return IT_R(this, 0); }
	inline IT_W Last() { return IT_W(this, pRoot->GetItemCount()-1); }
	inline IT_R Last()const { return IT_R(this, pRoot->GetItemCount()-1); }
};

// Pointer-Cluster
template <class ITEM, class GROUP, class ITEMGROUP, class PARENTGROUP, class IT_R, class IT_W>
class Cluster<ITEM*, GROUP, ITEMGROUP, PARENTGROUP, IT_R, IT_W>: public ClusterBase<GROUP, ITEMGROUP, PARENTGROUP>
{
public:
	// Access
	inline IT_W At(UINT64 Position) { return IT_W(this, Position); }
	inline IT_R At(UINT64 Position)const { return IT_R(this, Position); }
	inline IT_R At(IT_R const& It)const { return IT_R(It); }
	inline IT_R At(IT_W const& It)const { return IT_R(It); }
	inline IT_W First() { return IT_W(this, 0); }
	inline IT_R First()const { return IT_R(this, 0); }
	inline IT_W Last() { return IT_W(this, pRoot->GetItemCount()-1); }
	inline IT_R Last()const { return IT_R(this, pRoot->GetItemCount()-1); }
};


#ifdef __cplusplus_winrt
// Handle-Cluster
template <class ITEM, class GROUP, class ITEMGROUP, class PARENTGROUP, class IT_R, class IT_W>
class Cluster<ITEM^, GROUP, ITEMGROUP, PARENTGROUP, IT_R, IT_W>: public ClusterBase<GROUP, ITEMGROUP, PARENTGROUP>
{
public:
	// Access
	inline IT_W At(UINT64 Position) { return IT_W(this, Position); }
	inline IT_R At(UINT64 Position)const { return IT_R(this, Position); }
	inline IT_R At(IT_R const& It)const { return IT_R(It); }
	inline IT_R At(IT_W const& It)const { return IT_R(It); }
	inline IT_W First() { return IT_W(this, 0); }
	inline IT_R First()const { return IT_R(this, 0); }
	inline IT_W Last() { return IT_W(this, pRoot->GetItemCount()-1); }
	inline IT_R Last()const { return IT_R(this, pRoot->GetItemCount()-1); }
};
#endif

// String-Cluster
template <class CHAR, BOOL _Alloc, class GROUP, class ITEMGROUP, class PARENTGROUP, class IT_R, class IT_W>
class Cluster<String<CHAR, _Alloc>, GROUP, ITEMGROUP, PARENTGROUP, IT_R, IT_W>: public ClusterBase<GROUP, ITEMGROUP, PARENTGROUP>
{
public:
	// Access
	inline IT_W At(UINT64 Position) { return IT_W(this, Position); }
	inline IT_R At(UINT64 Position)const { return IT_R(this, Position); }
	inline IT_R At(IT_R const& It)const { return IT_R(It); }
	inline IT_R At(IT_W const& It)const { return IT_R(It); }
	inline IT_W First() { return IT_W(this, 0); }
	inline IT_R First()const { return IT_R(this, 0); }
	inline IT_W Last() { return IT_W(this, pRoot->GetItemCount()-1); }
	inline IT_R Last()const { return IT_R(this, pRoot->GetItemCount()-1); }
};

}}}
