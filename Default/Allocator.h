//=============
// Allocator.h
//=============

#pragma once


//=====================
// Uninitialized Array
//=====================

template <class _Item, SIZE_T _Size>
class Allocator
{
public:
	// Access
	inline _Item* Get() { return (_Item*)pItems; }
	inline _Item const* Get()const { return (_Item const*)pItems; }
	inline _Item& GetAt(SIZE_T Position) { _Item* pitems=(_Item*)pItems; return pitems[Position]; }
	inline _Item const& GetAt(SIZE_T Position)const { _Item const* pitems=(_Item const*)pItems; return pitems[Position]; }

private:
	// Common
	alignas(alignof(_Item[_Size])) BYTE pItems[sizeof(_Item[_Size])];
};
