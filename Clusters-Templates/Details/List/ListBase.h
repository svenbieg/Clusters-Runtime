//============
// ListBase.h
//============

#pragma once


//=======
// Using
//=======

#include "../Cluster/Cluster.h"
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

template<class _Item, UINT _GroupSize>
class ListBase: public ::Clusters::Templates::Details::Cluster::Cluster<_Item, ListGroup<_Item>, ListItemGroup<_Item, _GroupSize>, ListParentGroup<_Item, _GroupSize>, ListIterator<_Item, _GroupSize, true>, ListIterator<_Item, _GroupSize, false>>
{
protected:
	// Using
	using _ListParentGroup=ListParentGroup<_Item, _GroupSize>;

	// Modification
	_Item* AppendInternal()
		{
		_Item* pitem=this->pRoot->Append(false);
		if(pitem)
			return pitem;
		this->pRoot=new _ListParentGroup(this->pRoot);
		pitem=this->pRoot->Append(true);
		ASSERT(pitem);
		return pitem;
		}
	_Item* InsertInternal(SIZE_T Position)
		{
		_Item* pitem=this->pRoot->InsertAt(Position, false);
		if(pitem)
			return pitem;
		this->pRoot=new _ListParentGroup(this->pRoot);
		pitem=this->pRoot->InsertAt(Position, true);
		ASSERT(pitem);
		return pitem;
		}
};

}}}}
