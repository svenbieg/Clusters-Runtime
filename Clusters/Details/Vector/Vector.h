//==========
// Vector.h
//==========

#pragma once


//=======
// Using
//=======

#include "StringVector.h"


//===========
// Namespace
//===========

namespace Clusters {
	namespace Details {
		namespace Vector {


//========
// Vector
//========

// Vector
template<class ITEM, UINT _GroupSize>
class Vector: public VectorBase<ITEM, _GroupSize>
{
public:
	// Access
	virtual inline ITEM& operator[](UINT64 Position) { return *pRoot->GetAt(Position); }
	virtual inline ITEM const& operator[](UINT64 Position)const { return *pRoot->GetAt(Position); }
	virtual inline ITEM& GetAt(UINT64 Position) { return *pRoot->GetAt(Position); }
	virtual inline ITEM const& GetAt(UINT64 Position)const { return *pRoot->GetAt(Position); }

	// Modification
	inline ITEM* Append(ITEM const& Item)
		{
		ITEM* pitem=AppendInternal();
		new (pitem) ITEM(Item);
		return pitem;
		}
	inline ITEM* InsertAt(UINT64 Position, ITEM const& Item)
		{
		ITEM* pitem=InsertInternal(Position);
		new (pitem) ITEM(Item);
		return pitem;
		}
};

// Pointer-Vector
template<class ITEM, UINT _GroupSize>
class Vector<ITEM*, _GroupSize>: public VectorBase<ITEM*, _GroupSize>
{
public:
	// Access
	inline ITEM* operator[](UINT64 Position) { return pRoot->GetAt(Position); }

	// Modification
	inline VOID Append(ITEM* Item)
		{
		ITEM** pitem=AppendInternal();
		*pitem=Item;
		}
	inline VOID InsertAt(UINT64 Position, ITEM* Item)
		{
		ITEM** pitem=InsertInternal(Position);
		*pitem=Item;
		}
	ITEM* ReleaseAt(UINT64 Position)
		{
		ITEM* pitem=pRoot->ReleaseAt(Position);
		UpdateRoot();
		return pitem;
		}
};

#ifdef __cplusplus_winrt
// Handle-Vector
template<class ITEM, UINT _GroupSize>
class Vector<ITEM^, _GroupSize>: public VectorBase<ITEM^, _GroupSize>
{
public:
	// Access
	inline ITEM^ operator[](UINT64 Position) { return pRoot->GetAt(Position); }

	// Modification
	inline VOID Append(ITEM^ Item)
		{
		ITEM^* pitem=AppendInternal();
		ZeroMemory(pitem, sizeof(ITEM^));
		*pitem=Item;
		}
	inline VOID InsertAt(UINT64 Position, ITEM^ Item)
		{
		ITEM^* pitem=InsertInternal(Position);
		ZeroMemory(pitem, sizeof(ITEM^));
		*pitem=Item;
		}
};
#endif

template<UINT _GroupSize> class Vector<LPSTR, _GroupSize>: public StringVector<CHAR, true, _GroupSize> {}; // Ansi-Vector
template<UINT _GroupSize> class Vector<LPCSTR, _GroupSize>: public StringVector<CHAR, false, _GroupSize> {}; // Shared Ansi-Vector
template<UINT _GroupSize> class Vector<LPWSTR, _GroupSize>: public StringVector<WCHAR, true, _GroupSize> {}; // Unicode-Vector
template<UINT _GroupSize> class Vector<LPCWSTR, _GroupSize>: public StringVector<WCHAR, false, _GroupSize> {}; // Shared Unicode-Vector

}}}
