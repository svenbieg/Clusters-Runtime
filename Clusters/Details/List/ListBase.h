//============
// ListBase.h
//============

#pragma once


//=======
// Using
//=======

#include "..\Cluster\Cluster.h"
#include "ListIterator.h"


//===========
// Namespace
//===========

namespace Clusters {
	namespace Details {
		namespace List {


//=================
// List Base-Class
//=================

template<class ITEM, UINT _GroupSize>
class ListBase: public ::Clusters::Details::Cluster::Cluster<ITEM, ListGroup<ITEM>, ListItemGroup<ITEM, _GroupSize>, ListParentGroup<ITEM, _GroupSize>, ListIterator<ITEM, _GroupSize, true>, ListIterator<ITEM, _GroupSize, false>>
{
public:
	// Types
	typedef ListItemGroup<ITEM, _GroupSize> ItemGroup;
	typedef ListIterator<ITEM, _GroupSize, true> IteratorReadOnly;
	typedef ListIterator<ITEM, _GroupSize, false> IteratorReadWrite;
	typedef ListParentGroup<ITEM, _GroupSize> ParentGroup;

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
