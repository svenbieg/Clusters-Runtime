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
	namespace Details {
		namespace List {


//=================
// Item-Group List
//=================

template <class ITEM, UINT _GroupSize>
class ListItemGroup: public ::Clusters::Details::Cluster::ItemGroup<ITEM, ListGroup<ITEM>, _GroupSize>
{
private:
	// Using
	using ARRAYHELPER=ArrayHelper<ITEM, UINT>;
	using BASE=::Clusters::Details::Cluster::ItemGroup<ITEM, ListGroup<ITEM>, _GroupSize>;
	using ITEMGROUP=ItemGroup<ITEM, ListGroup<ITEM>, _GroupSize>;

public:
	// Con-/Destructors
	using BASE::BASE;

	// Modification
	inline ITEM* Append(BOOL Again)override { return ITEMGROUP::Append(); }
	inline VOID Append(ITEM const* Items, UINT Count, BOOL CopyOnly) { ARRAYHELPER::Append(cItems.Get(), _GroupSize, &uItemCount, Items, Count, CopyOnly); }
	inline ITEM* InsertAt(UINT64 Position, BOOL Again)override { return ITEMGROUP::InsertAt(ToUINT(Position)); }
	inline VOID InsertAt(UINT Position, ITEM const* Items, UINT Count, BOOL CopyOnly) { ARRAYHELPER::InsertAt(cItems.Get(), _GroupSize, &uItemCount, Position, Items, Count, CopyOnly); }
	inline VOID RemoveAt(UINT Position, UINT Count, BOOL RemoveOnly) { ARRAYHELPER::RemoveAt(cItems.Get(), &uItemCount, Position, Count, RemoveOnly); }
};

}}}
