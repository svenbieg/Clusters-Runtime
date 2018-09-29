//=============
// Allocator.h
//=============

#pragma once


//=====================
// Uninitialized Array
//=====================

template <class T, SIZE_T SIZE>
class Allocator
{
public:
	// Access
	inline T* Get() { return (T*)pItems; }
	inline T const* Get()const { return (T const*)pItems; }
	inline T& GetAt(SIZE_T Position) { T* pitems=(T*)pItems; return pitems[Position]; }
	inline T const& GetAt(SIZE_T Position)const { T const* pitems=(T const*)pItems; return pitems[Position]; }

private:
	// Common
	alignas(alignof(T[SIZE])) BYTE pItems[sizeof(T[SIZE])];
};
