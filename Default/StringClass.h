//===============
// StringClass.h
//===============

#pragma once


//=======
// Using
//=======

#include "StringHelper.h"


//========
// String
//========

template <class T, BOOL _Alloc>
class String
{
public:
	// Con-/Destructors
	String(): pValue(nullptr) {}
	String(T const* Value, UINT Length=0) { StringAssign(&pValue, nullptr, Value, Length); }
	String(String<T, _Alloc> const& String) { StringAssign(&pValue, nullptr, String.pValue); }
	~String() { StringFree(pValue); }

	// Access
	inline T const* Get()const { return pValue; }

	// Modification
	T* Release() { return PointerRelease(&pValue); }
	inline UINT Set(T const* Value, UINT Length) { return StringAssign(&pValue, nullptr, Value, Length); }

private:
	// Common
	T* pValue;
};


//===============
// Shared String
//===============

template <class T>
class String<T, false>
{
public:
	// Con-/Destructors
	String(): pValue(nullptr) {}
	String(T const* Value): pValue(Value) {}
	String(T const* Value, UINT): pValue(Value) {}

	// Access
	inline T const* Get()const { return pValue; }

	// Modification
	T const* Release() { T const* pstr=pValue; pValue=nullptr; return pstr; }
	inline VOID Set(T const* Value) { pValue=Value; }
	//inline VOID Set(T const* Value, UINT) { pValue=Value; }

private:
	// Common
	T const* pValue;
};
