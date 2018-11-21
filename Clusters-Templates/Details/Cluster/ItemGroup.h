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
	namespace Templates {
		namespace Details {
			namespace Cluster {


//===============================
// Base-Class Item-Group Cluster
//===============================

template <class _Item, class _Group, unsigned int _GroupSize>
class ItemGroupBase: public _Group
{
protected:
	// Using
	using _ArrayHelper=ArrayHelper<_Item, unsigned int>;

public:
	// Access
	inline _Item* AddressOfItemAt(unsigned int Position) { return &cItems.GetAt(Position); }
	inline _Item const* AddressOfItemAt(unsigned int Position)const { return &cItems.GetAt(Position); }
	inline unsigned int GetChildCount()const override { return uItemCount; }
	inline size_t GetItemCount()const override { return uItemCount; }
	inline _Item* GetItems() { return cItems.Get(); }
	inline _Item const* GetItems()const { return cItems.Get(); }
	inline unsigned int GetLevel()const { return 0; }

	// Modification
	_Item* Append()
		{
		if(uItemCount==_GroupSize)
			return nullptr;
		return _ArrayHelper::Append(cItems.Get(), _GroupSize, &uItemCount);
		}
	_Item* InsertAt(unsigned int Position)
		{
		if(uItemCount==_GroupSize)
			return nullptr;
		return _ArrayHelper::InsertAt(cItems.Get(), _GroupSize, &uItemCount, Position);
		}
	inline void RemoveAt(size_t Position)override { _ArrayHelper::RemoveAt(cItems.Get(), &uItemCount, (unsigned int)Position); }

protected:
	// Con-/Destructors
	ItemGroupBase(): uItemCount(0) {}
	ItemGroupBase(ItemGroupBase const& Group): uItemCount(Group.uItemCount) { _ArrayHelper::OverWriteItems(cItems.Get(), Group.cItems.Get(), uItemCount); }
	~ItemGroupBase()override { _ArrayHelper::DestroyItems(cItems.Get(), uItemCount); }

	// Common
	Allocator<_Item, _GroupSize> cItems;
	unsigned int uItemCount;
};


//====================
// Item-Group Cluster
//====================

// Item-Group Cluster
template <class _Item, class _Group, unsigned int _GroupSize>
class ItemGroup: public ItemGroupBase<_Item, _Group, _GroupSize>
{
public:
	// Access
	inline _Item* GetAt(size_t Position)override { return _ArrayHelper::GetAt(cItems.Get(), uItemCount, (unsigned int)Position); }
	inline _Item const* GetAt(size_t Position)const override { return _ArrayHelper::GetAt(cItems.Get(), uItemCount, (unsigned int)Position); }

protected:
	// Con-/Destructors
	using _ItemGroupBase=ItemGroupBase<_Item, _Group, _GroupSize>;
	using _ItemGroupBase::_ItemGroupBase;
};

// Item-Group Pointer-Cluster
template <class _Item, class _Group, unsigned int _GroupSize>
class ItemGroup<_Item*, _Group, _GroupSize>: public ItemGroupBase<_Item*, _Group, _GroupSize>
{
public:
	// Access
	inline _Item* GetAt(size_t Position)const override { return *_ArrayHelper::GetAt(cItems.Get(), uItemCount, (unsigned int)Position); }

	// Modification
	inline _Item* ReleaseAt(size_t Position) { return _ArrayHelper::ReleaseAt(cItems.Get(), &uItemCount, (unsigned int)Position); }

protected:
	// Con-/Destructors
	using _ItemGroupBase=ItemGroupBase<_Item*, _Group, _GroupSize>;
	using _ItemGroupBase::_ItemGroupBase;
};

#ifdef __cplusplus_winrt
// Item-Group Handle-Cluster
template <class _Item, class _Group, unsigned int _GroupSize>
class ItemGroup<_Item^, _Group, _GroupSize>: public ItemGroupBase<_Item^, _Group, _GroupSize>
{
public:
	// Access
	inline _Item^ GetAt(size_t Position)const override { return *_ArrayHelper::GetAt(cItems.Get(), uItemCount, (unsigned int)Position); }

protected:
	// Con-/Destructors
	using _ItemGroupBase=ItemGroupBase<_Item^, _Group, _GroupSize>;
	using _ItemGroupBase::_ItemGroupBase;
};
#endif

// Item-Group String-Cluster
template <class _Char, bool _Alloc, class _Group, unsigned int _GroupSize>
class ItemGroup<String<_Char, _Alloc>, _Group, _GroupSize>: public ItemGroupBase<String<_Char, _Alloc>, _Group, _GroupSize>
{
public:
	// Access
	inline _Char const* GetAt(size_t Position)const override { return _ArrayHelper::GetAt(cItems.Get(), uItemCount, (unsigned int)Position)->Get(); }

	// Modification
	inline _Char const* ReleaseAt(size_t Position)
		{
		_Char const* pstr=_ArrayHelper::GetAt(cItems.Get(), uItemCount, (unsigned int)Position)->Release();
		_ArrayHelper::RemoveAt(cItems.Get(), &uItemCount, (unsigned int)Position);
		return pstr;
		}

protected:
	// Con-/Destructors
	using _ItemGroupBase=ItemGroupBase<String<_Char, _Alloc>, _Group, _GroupSize>;
	using _ItemGroupBase::_ItemGroupBase;
};

}}}}
