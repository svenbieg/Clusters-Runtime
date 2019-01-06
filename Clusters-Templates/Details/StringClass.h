//===============
// StringClass.h
//===============

#pragma once


//=======
// Using
//=======

#include "Default/StringHelper.h"


//===========
// Namespace
//===========

namespace Clusters {
	namespace Templates {
		namespace Details {


//========
// String
//========

template <class _Char, BOOL _Alloc>
class String
{
public:
	// Con-/Destructors
	String(): pValue(nullptr) {}
	String(_Char const* Value, UINT Length=0): pValue(nullptr) { StringAssign(&pValue, nullptr, Value, Length); }
	~String() { delete pValue; }

	// Access
	inline _Char const* Get()const { return pValue; }

	// Modification
	_Char* Release() { return PointerRelease(&pValue); }
	inline UINT Set(_Char const* Value, UINT Length) { return StringAssign(&pValue, nullptr, Value, Length); }

private:
	// Common
	_Char* pValue;
};


//===============
// Shared String
//===============

template <class _Char>
class String<_Char, false>
{
public:
	// Con-/Destructors
	String(): pValue(nullptr) {}
	String(_Char const* Value, UINT Length=0): pValue(Value) { ASSERT(Length==0); }

	// Access
	inline _Char const* Get()const { return pValue; }

	// Modification
	_Char const* Release() { _Char const* pstr=pValue; pValue=nullptr; return pstr; }
	inline VOID Set(_Char const* Value) { pValue=Value; }

private:
	// Common
	_Char const* pValue;
};

}}}