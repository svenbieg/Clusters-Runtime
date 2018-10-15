//=============
// ItemGroup.h
//=============

#pragma once


//=======
// Using
//=======

#include "Default\Allocator.h"
#include "Default\ArrayHelper.h"


//===========
// Namespace
//===========

namespace Clusters {
	namespace Details {
		namespace Cluster {


//===============================
// Base-Class Item-Group Cluster
//===============================

template <class ITEM, class GROUP, UINT _GroupSize>
class ItemGroupBase: public GROUP
{
protected:
	// Using
	using ARRAYHELPER=ArrayHelper<ITEM, UINT>;

public:
	// Access
	inline ITEM* AddressOfItemAt(UINT Position) { return &cItems.GetAt(Position); }
	inline ITEM const* AddressOfItemAt(UINT Position)const { return &cItems.GetAt(Position); }
	inline UINT GetChildCount()const override { return uItemCount; }
	inline UINT64 GetItemCount()const override { return uItemCount; }
	inline ITEM* GetItems() { return cItems.Get(); }
	inline ITEM const* GetItems()const { return cItems.Get(); }
	inline UINT GetLevel()const { return 0; }

	// Modification
	ITEM* Append()
		{
		if(uItemCount==_GroupSize)
			return nullptr;
		return ARRAYHELPER::Append(cItems.Get(), _GroupSize, &uItemCount);
		}
	ITEM* InsertAt(UINT Position)
		{
		if(uItemCount==_GroupSize)
			return nullptr;
		return ARRAYHELPER::InsertAt(cItems.Get(), _GroupSize, &uItemCount, Position);
		}
	inline VOID RemoveAt(UINT64 Position)override { ARRAYHELPER::RemoveAt(cItems.Get(), &uItemCount, ToUINT(Position)); }

protected:
	// Con-/Destructors
	ItemGroupBase(): uItemCount(0) {}
	ItemGroupBase(ItemGroupBase const& Group): uItemCount(Group.uItemCount) { ARRAYHELPER::OverWriteItems(cItems.Get(), Group.cItems.Get(), uItemCount); }
	~ItemGroupBase()override { ARRAYHELPER::DestroyItems(cItems.Get(), uItemCount); }

	// Common
	Allocator<ITEM, _GroupSize> cItems;
	UINT uItemCount;
};


//====================
// Item-Group Cluster
//====================

// Item-Group Cluster
template <class ITEM, class GROUP, UINT _GroupSize>
class ItemGroup: public ItemGroupBase<ITEM, GROUP, _GroupSize>
{
public:
	// Access
	inline ITEM* GetAt(UINT64 Position)override { return ARRAYHELPER::GetAt(cItems.Get(), uItemCount, ToUINT(Position)); }
	inline ITEM const* GetAt(UINT64 Position)const override { return ARRAYHELPER::GetAt(cItems.Get(), uItemCount, ToUINT(Position)); }

protected:
	// Con-/Destructors
	using ITEMGROUPBASE=ItemGroupBase<ITEM, GROUP, _GroupSize>;
	using ITEMGROUPBASE::ITEMGROUPBASE;
};

// Item-Group Pointer-Cluster
template <class ITEM, class GROUP, UINT _GroupSize>
class ItemGroup<ITEM*, GROUP, _GroupSize>: public ItemGroupBase<ITEM*, GROUP, _GroupSize>
{
public:
	// Access
	inline ITEM* GetAt(UINT64 Position)const override { return *ARRAYHELPER::GetAt(cItems.Get(), uItemCount, ToUINT(Position)); }

	// Modification
	inline ITEM* ReleaseAt(UINT64 Position) { return ARRAYHELPER::ReleaseAt(cItems.Get(), &uItemCount, ToUINT(Position)); }

protected:
	// Con-/Destructors
	using ITEMGROUPBASE=ItemGroupBase<ITEM*, GROUP, _GroupSize>;
	using ITEMGROUPBASE::ITEMGROUPBASE;
};

#ifdef __cplusplus_winrt
// Item-Group Handle-Cluster
template <class ITEM, class GROUP, UINT _GroupSize>
class ItemGroup<ITEM^, GROUP, _GroupSize>: public ItemGroupBase<ITEM^, GROUP, _GroupSize>
{
public:
	// Access
	inline ITEM^ GetAt(UINT64 Position)const override { return *ARRAYHELPER::GetAt(cItems.Get(), uItemCount, ToUINT(Position)); }

protected:
	// Con-/Destructors
	using ITEMGROUPBASE=ItemGroupBase<ITEM^, GROUP, _GroupSize>;
	using ITEMGROUPBASE::ITEMGROUPBASE;
};
#endif

// Item-Group String-Cluster
template <class CHAR, BOOL _Alloc, class GROUP, UINT _GroupSize>
class ItemGroup<String<CHAR, _Alloc>, GROUP, _GroupSize>: public ItemGroupBase<String<CHAR, _Alloc>, GROUP, _GroupSize>
{
public:
	// Access
	inline CHAR const* GetAt(UINT64 Position)const override { return ARRAYHELPER::GetAt(cItems.Get(), uItemCount, ToUINT(Position))->Get(); }

	// Modification
	inline CHAR const* ReleaseAt(UINT64 Position)
		{
		CHAR const* pstr=ARRAYHELPER::GetAt(cItems.Get(), uItemCount, ToUINT(Position))->Release();
		ARRAYHELPER::RemoveAt(cItems.Get(), &uItemCount, ToUINT(Position));
		return pstr;
		}

protected:
	// Con-/Destructors
	using ITEMGROUPBASE=ItemGroupBase<String<CHAR, _Alloc>, GROUP, _GroupSize>;
	using ITEMGROUPBASE::ITEMGROUPBASE;
};

}}}
