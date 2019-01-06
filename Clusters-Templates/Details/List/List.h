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
	namespace Templates {
		namespace Details {
			namespace List {


//======
// List
//======

// List
template<class _Item, UINT _GroupSize>
class List: public ListBase<_Item, _GroupSize>
{
public:
	// Access
	virtual inline _Item operator[](SIZE_T Position)const { return *this->pRoot->GetAt(Position); }
	virtual inline _Item GetAt(SIZE_T Position)const { return *this->pRoot->GetAt(Position); }

	// Modification
	inline _Item* Append(_Item const& Item)
		{
		_Item* pitem=this->AppendInternal();
		new (pitem) _Item(Item);
		return pitem;
		}
	inline _Item* InsertAt(SIZE_T Position, _Item const& Item)
		{
		_Item* pitem=this->InsertInternal(Position);
		new (pitem) _Item(Item);
		return pitem;
		}
};

// Pointer-List
template<class _Item, UINT _GroupSize>
class List<_Item*, _GroupSize>: public ListBase<_Item*, _GroupSize>
{
public:
	// Access
	inline _Item* operator[](SIZE_T Position)const { return this->GetAt(Position); }
	inline _Item* GetAt(SIZE_T Position)const { return this->GetAt(Position); }

	// Modification
	inline VOID Append(_Item* Item) { *this->AppendInternal()=Item; }
	inline VOID InsertAt(SIZE_T Position, _Item* Item) { *this->InsertInternal(Position)=Item; }
	_Item* ReleaseAt(SIZE_T Position)
		{
		_Item* pitem=this->pRoot->ReleaseAt(Position);
		this->UpdateRoot();
		return pitem;
		}
};

#ifdef __cplusplus_winrt
// Handle-List
template<class _Item, UINT _GroupSize>
class List<_Item^, _GroupSize>: public ListBase<_Item^, _GroupSize>
{
public:
	// Access
	inline _Item^ operator[](SIZE_T Position)const { return this->pRoot->GetAt(Position); }
	inline _Item^ GetAt(SIZE_T Position)const { return this->pRoot->GetAt(Position); }

	// Modification
	VOID Append(_Item^ Item)
		{
		_Item^* pitem=this->AppendInternal();
		ZeroMemory(pitem, sizeof(_Item^));
		*pitem=Item;
		}
	VOID InsertAt(SIZE_T Position, _Item^ Item)
		{
		_Item^* pitem=this->InsertInternal(Position);
		ZeroMemory(pitem, sizeof(_Item^));
		*pitem=Item;
		}
};
#endif

template<UINT _GroupSize> class List<char*, _GroupSize>: public StringList<char, true, _GroupSize> {}; // Ansi-List
template<UINT _GroupSize> class List<char const*, _GroupSize>: public StringList<char, false, _GroupSize> {}; // Shared Ansi-List
template<UINT _GroupSize> class List<wchar_t*, _GroupSize>: public StringList<wchar_t, true, _GroupSize> {}; // Unicode-List
template<UINT _GroupSize> class List<wchar_t const*, _GroupSize>: public StringList<wchar_t, false, _GroupSize> {}; // Shared Unicode-List

}}}}
