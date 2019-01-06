//=================
// ListItemGroup.h
//=================

#pragma once


//=======
// Using
//=======

#include "../Cluster/ItemGroup.h"
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

template <class _Item, UINT _GroupSize>
class ListItemGroup: public ::Clusters::Templates::Details::Cluster::ItemGroup<_Item, ListGroup<_Item>, _GroupSize>
{
private:
	// Using
	using _ListGroup=ListGroup<_Item>;
	using _ItemGroup=::Clusters::Templates::Details::Cluster::ItemGroup<_Item, _ListGroup, _GroupSize>;
	using _ArrayHelper=ArrayHelper<_Item, UINT>;

public:
	// Con-/Destructors
	ListItemGroup() {}
	ListItemGroup(ListItemGroup const& Group): _ItemGroup(Group) {}

	// Modification
	inline _Item* Append(BOOL Again)override { return _ArrayHelper::Append(this->cItems.Get(), _GroupSize, &this->uItemCount); }
	inline VOID Append(_Item const* Items, UINT Count, BOOL Moving) { _ArrayHelper::Append(this->cItems.Get(), _GroupSize, &this->uItemCount, Items, Count, Moving); }
	inline _Item* InsertAt(SIZE_T Position, BOOL Again)override { return _ArrayHelper::InsertAt(this->cItems.Get(), _GroupSize, &this->uItemCount, (UINT)Position); }
	inline VOID InsertAt(UINT Position, _Item const* Items, UINT Count, BOOL Moving) { _ArrayHelper::InsertAt(this->cItems.Get(), _GroupSize, &this->uItemCount, Position, Items, Count, Moving); }
	inline VOID RemoveAt(UINT Position, UINT Count, BOOL Moving) { _ArrayHelper::RemoveAt(this->cItems.Get(), &this->uItemCount, Position, Count, Moving); }
};

}}}}
