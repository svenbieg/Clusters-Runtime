//==============
// StringList.h
//==============

#pragma once


//=======
// Using
//=======

#include "..\StringClass.h"
#include "ListBase.h"


//===========
// Namespace
//===========

namespace Clusters {
	namespace Templates {
		namespace Details {
			namespace List {


//=============
// String-List
//=============

// String-List
template<class _Char, bool _Alloc, unsigned int _GroupSize>
class StringList: public ListBase<String<_Char, _Alloc>, _GroupSize>
{
public:
	// Access
	inline _Char const* operator[](size_t Position)const { return GetAt(Position); }
	inline _Char const* GetAt(size_t Position)const { return pRoot->GetAt(Position); }

	// Modification
	unsigned int Append(_Char const* Value, unsigned int Length=0)
		{
		String<_Char, _Alloc>* pitem=AppendInternal();
		new (pitem) String<_Char, _Alloc>();
		return pitem->Set(Value, Length);
		}
	unsigned int InsertAt(size_t Position, _Char const* Value, unsigned int Length=0)
		{
		String<_Char, _Alloc>* pitem=InsertInternal(Position);
		new (pitem) String<_Char, _Alloc>();
		return pitem->Set(Value, Length);
		}
	_Char const* ReleaseAt(size_t Position)
		{
		_Char const* pitem=pRoot->ReleaseAt(Position);
		UpdateRoot();
		return pitem;
		}
};

// Shared String-List
template<class _Char, unsigned int _GroupSize>
class StringList<_Char, false, _GroupSize>: public ListBase<String<_Char, false>, _GroupSize>
{
public:
	// Access
	inline _Char const* operator[](size_t Position)const { return GetAt(Position); }
	_Char const* GetAt(size_t Position)const { return pRoot->GetAt(Position); }

	// Modification
	void Append(_Char const* Value)
		{
		String<_Char, false>* pitem=AppendInternal();
		new (pitem) String<_Char, false>();
		pitem->Set(Value);
		}
	void InsertAt(size_t Position, _Char const* Value)
		{
		String<_Char, false>* pitem=InsertInternal(Position);
		new (pitem) String<_Char, false>();
		pitem->Set(Value);
		}
	_Char const* ReleaseAt(size_t Position)
		{
		_Char const* pitem=pRoot->ReleaseAt(Position);
		UpdateRoot();
		return pitem;
		}
};

}}}}
