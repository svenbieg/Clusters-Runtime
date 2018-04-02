//==============
// VectorBase.h
//==============

#pragma once


//=======
// Using
//=======

#include "..\Cluster\Cluster.h"
#include "VectorIterator.h"


//===========
// Namespace
//===========

namespace Clusters {
	namespace Details {
		namespace Vector {


//===================
// Vector Base-Class
//===================

template<class ITEM, UINT _GroupSize>
class VectorBase: public ::Clusters::Details::Cluster::Cluster<ITEM, VectorGroup<ITEM>, VectorItemGroup<ITEM, _GroupSize>, VectorParentGroup<ITEM, _GroupSize>, VectorIterator<ITEM, _GroupSize, true>, VectorIterator<ITEM, _GroupSize, false>>
{
public:
	// Types
	typedef VectorItemGroup<ITEM, _GroupSize> ItemGroup;
	typedef VectorIterator<ITEM, _GroupSize, true> IteratorReadOnly;
	typedef VectorIterator<ITEM, _GroupSize, false> IteratorReadWrite;
	typedef VectorParentGroup<ITEM, _GroupSize> ParentGroup;

	// Access
	inline IteratorReadWrite At(UINT64 Position) { return IteratorReadWrite(this, Position); }
	inline IteratorReadOnly At(UINT64 Position)const { return IteratorReadOnly(this, Position); }

protected:
	// Modification
	ITEM* AppendInternal()
		{
		ITEM* pitem=pRoot->Append(false);
		if(pitem)
			return pitem;
		pRoot=new ParentGroup(pRoot);
		pitem=pRoot->Append(true);
		ASSERT(pitem);
		return pitem;
		}
	ITEM* InsertInternal(UINT64 Position)
		{
		ITEM* pitem=pRoot->InsertAt(Position, false);
		if(pitem)
			return pitem;
		pRoot=new ParentGroup(pRoot);
		pitem=pRoot->InsertAt(Position, true);
		ASSERT(pitem);
		return pitem;
		}
};

}}}
