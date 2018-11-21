//=================
// ListItemGroup.h
//=================

#pragma once


//=======
// Using
//=======

#include "..\Cluster\ItemGroup.h"
#include "ListGroup.h"


//===========
// Namespace
//===========

namespace Clusters {
	namespace Templates {
		namespace Details {
			namespace List {


//=================
// Item-Group List
//=================

template <class _Item, unsigned int _GroupSize>
class ListItemGroup: public ::Clusters::Templates::Details::Cluster::ItemGroup<_Item, ListGroup<_Item>, _GroupSize>
{
private:
	// Using
	using _ArrayHelper=ArrayHelper<_Item, unsigned int>;
	using _Base=::Clusters::Templates::Details::Cluster::ItemGroup<_Item, ListGroup<_Item>, _GroupSize>;
	using _ItemGroup=ItemGroup<_Item, ListGroup<_Item>, _GroupSize>;

public:
	// Con-/Destructors
	using _Base::_Base;

	// Modification
	inline _Item* Append(bool Again)override { return _ItemGroup::Append(); }
	inline void Append(_Item const* Items, unsigned int Count, bool CopyOnly) { _ArrayHelper::Append(cItems.Get(), _GroupSize, &uItemCount, Items, Count, CopyOnly); }
	inline _Item* InsertAt(size_t Position, bool Again)override { return _ItemGroup::InsertAt((unsigned int)Position); }
	inline void InsertAt(unsigned int Position, _Item const* Items, unsigned int Count, bool CopyOnly) { _ArrayHelper::InsertAt(cItems.Get(), _GroupSize, &uItemCount, Position, Items, Count, CopyOnly); }
	inline void RemoveAt(unsigned int Position, unsigned int Count, bool RemoveOnly) { _ArrayHelper::RemoveAt(cItems.Get(), &uItemCount, Position, Count, RemoveOnly); }
};

}}}}
