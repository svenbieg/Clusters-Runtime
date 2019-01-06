//==============
// StringList.h
//==============

#pragma once


//=======
// Using
//=======

#include "../StringClass.h"
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
template<class _Char, BOOL _Alloc, UINT _GroupSize>
class StringList: public ListBase<String<_Char, _Alloc>, _GroupSize>
{
public:
	// Access
	inline _Char const* operator[](SIZE_T Position)const { return this->GetAt(Position); }
	inline _Char const* GetAt(SIZE_T Position)const { return this->GetAt(Position); }

	// Modification
	UINT Append(_Char const* Value, UINT Length=0)
		{
		String<_Char, _Alloc>* pitem=this->AppendInternal();
		new (pitem) String<_Char, _Alloc>();
		return pitem->Set(Value, Length);
		}
	UINT InsertAt(SIZE_T Position, _Char const* Value, UINT Length=0)
		{
		String<_Char, _Alloc>* pitem=this->InsertInternal(Position);
		new (pitem) String<_Char, _Alloc>();
		return pitem->Set(Value, Length);
		}
	_Char const* ReleaseAt(SIZE_T Position)
		{
		_Char const* pitem=this->pRoot->ReleaseAt(Position);
		this->UpdateRoot();
		return pitem;
		}
};

// Shared String-List
template<class _Char, UINT _GroupSize>
class StringList<_Char, false, _GroupSize>: public ListBase<String<_Char, false>, _GroupSize>
{
public:
	// Access
	inline _Char const* operator[](SIZE_T Position)const { return this->GetAt(Position); }
	inline _Char const* GetAt(SIZE_T Position)const { return this->GetAt(Position); }

	// Modification
	VOID Append(_Char const* Value)
		{
		String<_Char, false>* pitem=this->AppendInternal();
		new (pitem) String<_Char, false>();
		pitem->Set(Value);
		}
	VOID InsertAt(SIZE_T Position, _Char const* Value)
		{
		String<_Char, false>* pitem=this->InsertInternal(Position);
		new (pitem) String<_Char, false>();
		pitem->Set(Value);
		}
	_Char const* ReleaseAt(SIZE_T Position)
		{
		_Char const* pitem=this->pRoot->ReleaseAt(Position);
		this->UpdateRoot();
		return pitem;
		}
};

}}}}
