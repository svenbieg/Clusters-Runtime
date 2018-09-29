//========
// List.h
//========

#pragma once


//=======
// Using
//=======

#include "StringList.h"


//===========
// Namespace
//===========

namespace Clusters {
	namespace Details {
		namespace List {


//======
// List
//======

// List
template<class ITEM, UINT _GroupSize>
class List: public ListBase<ITEM, _GroupSize>
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

// Pointer-List
template<class ITEM, UINT _GroupSize>
class List<ITEM*, _GroupSize>: public ListBase<ITEM*, _GroupSize>
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
// Handle-List
template<class ITEM, UINT _GroupSize>
class List<ITEM^, _GroupSize>: public ListBase<ITEM^, _GroupSize>
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

template<UINT _GroupSize> class List<LPSTR, _GroupSize>: public StringList<CHAR, true, _GroupSize> {}; // Ansi-List
template<UINT _GroupSize> class List<LPCSTR, _GroupSize>: public StringList<CHAR, false, _GroupSize> {}; // Shared Ansi-List
template<UINT _GroupSize> class List<LPWSTR, _GroupSize>: public StringList<WCHAR, true, _GroupSize> {}; // Unicode-List
template<UINT _GroupSize> class List<LPCWSTR, _GroupSize>: public StringList<WCHAR, false, _GroupSize> {}; // Shared Unicode-List

}}}
