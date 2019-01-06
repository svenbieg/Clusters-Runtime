//=============
// ItemGroup.h
//=============

#pragma once


//=======
// Using
//=======

#include "Default/Allocator.h"
#include "Default/ArrayHelper.h"


//===========
// Namespace
//===========

namespace Clusters {
	namespace Templates {
		namespace Details {
			namespace Cluster {


//===============================
// Base-Class Item-Group Cluster
//===============================

template <class _Item, class _Group, UINT _GroupSize>
class ItemGroupBase: public _Group
{
private:
	// Using
	using _ArrayHelper=ArrayHelper<_Item, UINT>;

public:
	// Access
	inline _Item* AddressOfItemAt(UINT Position) { return &cItems.GetAt(Position); }
	inline _Item const* AddressOfItemAt(UINT Position)const { return &cItems.GetAt(Position); }
	inline UINT GetChildCount()const override { return uItemCount; }
	inline SIZE_T GetItemCount()const override { return uItemCount; }
	inline _Item* GetItems() { return cItems.Get(); }
	inline _Item const* GetItems()const { return cItems.Get(); }
	inline UINT GetLevel()const { return 0; }

	// Modification
	inline VOID RemoveAt(SIZE_T Position)override { _ArrayHelper::RemoveAt(cItems.Get(), &uItemCount, (UINT)Position); }

protected:
	// Con-/Destructors
	ItemGroupBase(): uItemCount(0) {}
	ItemGroupBase(ItemGroupBase const& Group): uItemCount(Group.uItemCount) { _ArrayHelper::OverWriteItems(cItems.Get(), Group.cItems.Get(), uItemCount); }
	~ItemGroupBase()override { _ArrayHelper::DestroyItems(cItems.Get(), uItemCount); }

	// Common
	Allocator<_Item, _GroupSize> cItems;
	UINT uItemCount;
};


//====================
// Item-Group Cluster
//====================

// Item-Group Cluster
template <class _Item, class _Group, UINT _GroupSize>
class ItemGroup: public ItemGroupBase<_Item, _Group, _GroupSize>
{
private:
	// Using
	using _ArrayHelper=ArrayHelper<_Item, UINT>;

public:
	// Access
	inline _Item* GetAt(SIZE_T Position)override { return _ArrayHelper::GetAt(this->cItems.Get(), this->uItemCount, (UINT)Position); }
	inline _Item const* GetAt(SIZE_T Position)const override { return _ArrayHelper::GetAt(this->cItems.Get(), this->uItemCount, (UINT)Position); }
};

// Item-Group Pointer-Cluster
template <class _Item, class _Group, UINT _GroupSize>
class ItemGroup<_Item*, _Group, _GroupSize>: public ItemGroupBase<_Item*, _Group, _GroupSize>
{
private:
	// Using
	using _ArrayHelper=ArrayHelper<_Item*, UINT>;

public:
	// Access
	inline _Item* GetAt(SIZE_T Position)const override { return *_ArrayHelper::GetAt(this->cItems.Get(), this->uItemCount, (UINT)Position); }

	// Modification
	inline _Item* ReleaseAt(SIZE_T Position) { return _ArrayHelper::ReleaseAt(this->cItems.Get(), &this->uItemCount, (UINT)Position); }
};

#ifdef __cplusplus_winrt
// Item-Group Handle-Cluster
template <class _Item, class _Group, UINT _GroupSize>
class ItemGroup<_Item^, _Group, _GroupSize>: public ItemGroupBase<_Item^, _Group, _GroupSize>
{
private:
	// Using
	using _ArrayHelper=ArrayHelper<_Item^, UINT>;

public:
	// Access
	inline _Item^ GetAt(SIZE_T Position)const override { return *_ArrayHelper::GetAt(this->cItems.Get(), this->uItemCount, (UINT)Position); }
};
#endif

// Item-Group String-Cluster
template <class _Char, BOOL _Alloc, class _Group, UINT _GroupSize>
class ItemGroup<String<_Char, _Alloc>, _Group, _GroupSize>: public ItemGroupBase<String<_Char, _Alloc>, _Group, _GroupSize>
{
private:
	// Using
	using _ArrayHelper=ArrayHelper<String<_Char, _Alloc>, UINT>;

public:
	// Access
	inline _Char const* GetAt(SIZE_T Position)const override { return _ArrayHelper::GetAt(this->cItems.Get(), this->uItemCount, (UINT)Position)->Get(); }

	// Modification
	inline _Char const* ReleaseAt(SIZE_T Position)
		{
		_Char const* pstr=_ArrayHelper::GetAt(this->cItems.Get(), this->uItemCount, (UINT)Position)->Release();
		_ArrayHelper::RemoveAt(this->cItems.Get(), &this->uItemCount, (UINT)Position);
		return pstr;
		}
};

}}}}
