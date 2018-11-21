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

template<class _Item, unsigned int _GroupSize>
class ListBase: public ::Clusters::Templates::Details::Cluster::Cluster<_Item, ListGroup<_Item>, ListItemGroup<_Item, _GroupSize>, ListParentGroup<_Item, _GroupSize>, ListIterator<_Item, _GroupSize, true>, ListIterator<_Item, _GroupSize, false>>
{
public:
	// Types
	typedef ListIterator<_Item, _GroupSize, true> IteratorReadOnly;
	typedef ListIterator<_Item, _GroupSize, false> IteratorReadWrite;

	// Modification
	void RemoveAt(size_t Position)
		{
		pRoot->RemoveAt(Position);
		UpdateRoot();
		}

protected:
	// Using
	using _ParentGroup=ListParentGroup<_Item, _GroupSize>;

	// Modification
	_Item* AppendInternal()
		{
		_Item* pitem=pRoot->Append(false);
		if(pitem)
			return pitem;
		pRoot=new _ParentGroup(pRoot);
		pitem=pRoot->Append(true);
		ASSERT(pitem);
		return pitem;
		}
	_Item* InsertInternal(size_t Position)
		{
		_Item* pitem=pRoot->InsertAt(Position, false);
		if(pitem)
			return pitem;
		pRoot=new _ParentGroup(pRoot);
		pitem=pRoot->InsertAt(Position, true);
		ASSERT(pitem);
		return pitem;
		}
};

}}}}
