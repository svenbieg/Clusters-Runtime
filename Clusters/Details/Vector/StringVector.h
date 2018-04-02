//================
// StringVector.h
//================

#pragma once


//=======
// Using
//=======

#include "Default\StringClass.h"
#include "VectorBase.h"


//===========
// Namespace
//===========

namespace Clusters {
	namespace Details {
		namespace Vector {


//===============
// String-Vector
//===============

// String-Vector
template<class CHAR, BOOL _Alloc, UINT _GroupSize>
class StringVector: public VectorBase<String<CHAR, _Alloc>, _GroupSize>
{
public:
	// Access
	inline CHAR const* operator[](UINT64 Position) { return pRoot->GetAt(Position); }

	// Modification
	inline UINT Append(CHAR const* Value, UINT Length=0)
		{
		String<CHAR, _Alloc>* pitem=AppendInternal();
		new (pitem) String<CHAR, _Alloc>();
		return pitem->Set(Value, Length);
		}
	inline UINT InsertAt(UINT64 Position, CHAR const* Value, UINT Length=0)
		{
		String<CHAR, _Alloc>* pitem=InsertInternal(Position);
		new (pitem) String<CHAR, _Alloc>();
		return pitem->Set(Value, Length);
		}
	CHAR const* ReleaseAt(UINT64 Position)
		{
		CHAR const* pitem=pRoot->ReleaseAt(Position);
		UpdateRoot();
		return pitem;
		}
};

// Shared String-Vector
template<class CHAR, UINT _GroupSize>
class StringVector<CHAR, false, _GroupSize>: public VectorBase<String<CHAR, false>, _GroupSize>
{
public:
	// Access
	inline CHAR const* operator[](UINT64 Position) { return pRoot->GetAt(Position); }

	// Modification
	inline VOID Append(CHAR const* Value)
		{
		String<CHAR, false>* pitem=AppendInternal();
		new (pitem) String<CHAR, false>();
		pitem->Set(Value);
		}
	inline VOID InsertAt(UINT64 Position, CHAR const* Value)
		{
		String<CHAR, false>* pitem=InsertInternal(Position);
		new (pitem) String<CHAR, false>();
		pitem->Set(Value);
		}
	CHAR const* ReleaseAt(UINT64 Position)
		{
		CHAR const* pitem=pRoot->ReleaseAt(Position);
		UpdateRoot();
		return pitem;
		}
};

}}}
