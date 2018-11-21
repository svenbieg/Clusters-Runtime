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
template<class _Item, unsigned int _GroupSize>
class List: public ListBase<_Item, _GroupSize>
{
public:
	// Access
	virtual inline _Item operator[](size_t Position)const { return GetAt(Position); }
	virtual inline _Item GetAt(size_t Position)const { return *pRoot->GetAt(Position); }

	// Modification
	inline _Item* Append(_Item const& Item)
		{
		_Item* pitem=AppendInternal();
		new (pitem) _Item(Item);
		return pitem;
		}
	inline _Item* InsertAt(size_t Position, _Item const& Item)
		{
		_Item* pitem=InsertInternal(Position);
		new (pitem) _Item(Item);
		return pitem;
		}
};

// Pointer-List
template<class _Item, unsigned int _GroupSize>
class List<_Item*, _GroupSize>: public ListBase<_Item*, _GroupSize>
{
public:
	// Access
	inline _Item* operator[](size_t Position)const { return GetAt(Position); }
	inline _Item* GetAt(size_t Position)const { return pRoot->GetAt(Position); }

	// Modification
	inline void Append(_Item* Item) { *AppendInternal()=Item; }
	inline void InsertAt(size_t Position, _Item* Item) { *InsertInternal(Position)=Item; }
	_Item* ReleaseAt(size_t Position)
		{
		_Item* pitem=pRoot->ReleaseAt(Position);
		UpdateRoot();
		return pitem;
		}
};

#ifdef __cplusplus_winrt
// Handle-List
template<class _Item, unsigned int _GroupSize>
class List<_Item^, _GroupSize>: public ListBase<_Item^, _GroupSize>
{
public:
	// Access
	inline _Item^ operator[](size_t Position)const { return GetAt(Position); }
	inline _Item^ GetAt(size_t Position)const { return pRoot->GetAt(Position); }

	// Modification
	void Append(_Item^ Item)
		{
		_Item^* pitem=AppendInternal();
		ZeroMemory(pitem, sizeof(_Item^));
		*pitem=Item;
		}
	void InsertAt(size_t Position, _Item^ Item)
		{
		_Item^* pitem=InsertInternal(Position);
		ZeroMemory(pitem, sizeof(_Item^));
		*pitem=Item;
		}
};
#endif

template<unsigned int _GroupSize> class List<char*, _GroupSize>: public StringList<_Char, true, _GroupSize> {}; // Ansi-List
template<unsigned int _GroupSize> class List<char const*, _GroupSize>: public StringList<_Char, false, _GroupSize> {}; // Shared Ansi-List
template<unsigned int _GroupSize> class List<wchar_t*, _GroupSize>: public StringList<wchar_t, true, _GroupSize> {}; // Unicode-List
template<unsigned int _GroupSize> class List<wchar_t const*, _GroupSize>: public StringList<wchar_t, false, _GroupSize> {}; // Shared Unicode-List

}}}}
