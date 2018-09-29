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
	namespace Details {
		namespace List {


//=============
// String-List
//=============

// String-List
template<class CHAR, BOOL _Alloc, UINT _GroupSize>
class StringList: public ListBase<String<CHAR, _Alloc>, _GroupSize>
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

// Shared String-List
template<class CHAR, UINT _GroupSize>
class StringList<CHAR, false, _GroupSize>: public ListBase<String<CHAR, false>, _GroupSize>
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
