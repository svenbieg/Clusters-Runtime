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
	namespace Templates {
		namespace Details {
			namespace List {


//=================
// List Base-Class
//=================

template<class ITEM, UINT _GroupSize>
class ListBase: public ::Clusters::Templates::Details::Cluster::Cluster<ITEM, ListGroup<ITEM>, ListItemGroup<ITEM, _GroupSize>, ListParentGroup<ITEM, _GroupSize>, ListIterator<ITEM, _GroupSize, true>, ListIterator<ITEM, _GroupSize, false>>
{
public:
	// Types
	typedef ListIterator<ITEM, _GroupSize, true> IteratorReadOnly;
	typedef ListIterator<ITEM, _GroupSize, false> IteratorReadWrite;

	// Modification
	VOID RemoveAt(UINT64 Position)
		{
		pRoot->RemoveAt(Position);
		UpdateRoot();
		}

protected:
	// Using
	using PARENTGROUP=ListParentGroup<ITEM, _GroupSize>;

	// Modification
	ITEM* AppendInternal()
		{
		ITEM* pitem=pRoot->Append(false);
		if(pitem)
			return pitem;
		pRoot=new PARENTGROUP(pRoot);
		pitem=pRoot->Append(true);
		ASSERT(pitem);
		return pitem;
		}
	ITEM* InsertInternal(UINT64 Position)
		{
		ITEM* pitem=pRoot->InsertAt(Position, false);
		if(pitem)
			return pitem;
		pRoot=new PARENTGROUP(pRoot);
		pitem=pRoot->InsertAt(Position, true);
		ASSERT(pitem);
		return pitem;
		}
};

}}}}