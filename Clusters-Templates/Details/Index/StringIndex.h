//===============
// StringIndex.h
//===============

#pragma once


//=======
// Using
//=======

#include "..\StringClass.h"
#include "IndexBase.h"


//===========
// Namespace
//===========

namespace Clusters {
	namespace Templates {
		namespace Details {
			namespace Index {


//==============
// String-Index
//==============

// String-Index
template<class CHAR, BOOL _Alloc, class ITEM, UINT _GroupSize>
class StringIndex: public IndexBase<String<CHAR, _Alloc>, ITEM, _GroupSize>
{
public:
	// Access
	inline ITEM operator[](CHAR const* Id)const { return GetStringInternal<ITEM const&, CHAR const*, UINT, BOOL>(Id, 0, false); }
	inline BOOL Contains(CHAR const* Id, UINT Length=0, BOOL CaseSensitive=false)const { return ContainsInternal(Id, Length, CaseSensitive); }
	inline IteratorReadWrite Find(CHAR const* Id, UINT Length=0, BOOL CaseSensitive=false) { return IteratorReadWrite(this, Id, Length, CaseSensitive); }
	inline IteratorReadOnly Find(CHAR const* Id, UINT Length=0, BOOL CaseSensitive=false)const { return IteratorReadOnly(this, Id, Length, CaseSensitive); }
	inline ITEM Get(CHAR const* Id, UINT Length=0, BOOL CaseSensitive=false)const { return GetStringInternal<ITEM const&, CHAR const*, UINT, BOOL>(Id, Length, CaseSensitive); }
	inline BOOL TryGet(CHAR const* Id, ITEM*, UINT Length=0, BOOL CaseSensitive=false)const { return TryGetInternal<ITEM, CHAR const*, UINT, BOOL>(Id, Length, CaseSensitive, Item); }

	// Modification
	inline BOOL Add(CHAR const* Id, ITEM const& Item, UINT Length=0, BOOL CaseSensitive=false) { return AddStringInternal<CHAR, ITEM const&>(Id, Length, CaseSensitive, Item); }
	inline BOOL Remove(CHAR const* Id, UINT Length=0, BOOL CaseSensitive=false) { return RemoveInternal(Id, Length, CaseSensitive); }
	inline VOID Set(CHAR const* Id, ITEM const& Item, UINT Length=0, BOOL CaseSensitive=false) { SetStringInternal<CHAR, ITEM const&>(Id, Length, CaseSensitive, Item); }
};

// String-Index without Items
template <class CHAR, UINT _GroupSize>
class StringIndex<CHAR, true, VOID, _GroupSize>: public IndexBase<String<CHAR, true>, VOID, _GroupSize>
{
public:
	// Access
	inline BOOL Contains(CHAR const* Id, UINT Length=0, BOOL CaseSensitive=false)const { return ContainsInternal(Id, Length, CaseSensitive); }
	inline IteratorReadWrite Find(CHAR const* Id, UINT Length=0, BOOL CaseSensitive=false) { return IteratorReadWrite(this, Id, Length, CaseSensitive); }
	inline IteratorReadOnly Find(CHAR const* Id, UINT Length=0, BOOL CaseSensitive=false)const { return IteratorReadOnly(this, Id, Length, CaseSensitive); }

	// Modification
	inline BOOL Add(CHAR const* Id, UINT Length=0, BOOL CaseSensitive=false) { return AddStringInternal(Id, Length, CaseSensitive); }
	inline BOOL Remove(CHAR const* Id, UINT Length=0, BOOL CaseSensitive=false) { return RemoveInternal(Id, Length, CaseSensitive); }
};

// String-Index with Pointers
template<class CHAR, class ITEM, UINT _GroupSize>
class StringIndex<CHAR, true, ITEM*, _GroupSize>: public IndexBase<String<CHAR, true>, ITEM*, _GroupSize>
{
public:
	// Access
	inline ITEM* operator[](CHAR const* Id)const { return GetInternal<ITEM*, CHAR const*>(Id); }
	inline BOOL Contains(CHAR const* Id, UINT Length=0, BOOL CaseSensitive=false)const { return ContainsInternal(Id, Length, CaseSensitive); }
	inline IteratorReadWrite Find(CHAR const* Id, UINT Length=0, BOOL CaseSensitive=false) { return IteratorReadWrite(this, Id, Length, CaseSensitive); }
	inline IteratorReadOnly Find(CHAR const* Id, UINT Length=0, BOOL CaseSensitive=false)const { return IteratorReadOnly(this, Id, Length, CaseSensitive); }
	inline ITEM* Get(CHAR const* Id, UINT Length=0, BOOL CaseSensitive=false)const { return GetInternal<ITEM*, CHAR const*, UINT, BOOL>(Id, Length, CaseSensitive); }

	// Modification
	inline BOOL Add(CHAR const* Id, ITEM* Item, UINT Length=0, BOOL CaseSensitive=false) { return AddStringInternal(Id, Length, CaseSensitive, Item); }
	inline BOOL Remove(CHAR const* Id, UINT Length=0, BOOL CaseSensitive=false) { return RemoveInternal(Id, Length, CaseSensitive); }
	inline VOID Set(CHAR const* Id, ITEM* Item, UINT Length=0, BOOL CaseSensitive=false) { SetStringInternal(Id, Length, CaseSensitive, Item); }
};

#ifdef __cplusplus_winrt
// String-Index with Handles
template<class CHAR, class ITEM, UINT _GroupSize>
class StringIndex<CHAR, true, ITEM^, _GroupSize>: public IndexBase<String<CHAR, true>, ITEM^, _GroupSize>
{
public:
	// Access
	inline ITEM^ operator[](CHAR const* Id)const { return GetInternal<ITEM^, CHAR const*>(Id); }
	inline BOOL Contains(CHAR const* Id, UINT Length=0, BOOL CaseSensitive=false)const { return ContainsInternal(Id, Length, CaseSensitive); }
	inline IteratorReadWrite Find(CHAR const* Id, UINT Length=0, BOOL CaseSensitive=false) { return IteratorReadWrite(this, Id, Length, CaseSensitive); }
	inline IteratorReadOnly Find(CHAR const* Id, UINT Length=0, BOOL CaseSensitive=false)const { return IteratorReadOnly(this, Id, Length, CaseSensitive); }
	inline ITEM^ Get(CHAR const* Id, UINT Length=0, BOOL CaseSensitive=false)const { return GetInternal<ITEM^, CHAR const*, UINT, BOOL>(Id, Length, CaseSensitive); }

	// Modification
	inline BOOL Add(CHAR const* Id, ITEM^ Item, UINT Length=0, BOOL CaseSensitive=false) { return AddStringInternal(Id, Length, CaseSensitve, Item); }
	inline BOOL Remove(CHAR const* Id, UINT Length=0, BOOL CaseSensitive=false) { return RemoveInternal(Id, Length, CaseSensitive); }
	inline VOID Set(CHAR const* Id, ITEM^ Item, UINT Length=0, BOOL CaseSensitive=false) { SetStringInternal(Id, Length, CaseSensitive, Item); }
};
#endif

// String-Index with Strings
template<class CHAR_ID, class CHAR_ITEM, UINT _GroupSize>
class StringIndex<CHAR_ID, true, String<CHAR_ITEM, true>, _GroupSize>: public IndexBase<String<CHAR_ID, true>, String<CHAR_ITEM, true>, _GroupSize>
{
public:
	// Access
	inline CHAR const* operator[](CHAR const* Id)const { return GetInternal<CHAR const*, CHAR const*>(Id); }
	inline BOOL Contains(CHAR const* Id, UINT Length=0, BOOL CaseSensitive=false)const { return ContainsInternal(Id, Length, CaseSensitive); }
	inline IteratorReadWrite Find(CHAR const* Id, UINT Length=0, BOOL CaseSensitive=false) { return IteratorReadWrite(this, Id, Length, CaseSensitive); }
	inline IteratorReadOnly Find(CHAR const* Id, UINT Length=0, BOOL CaseSensitive=false)const { return IteratorReadOnly(this, Id, Length, CaseSensitive); }
	inline CHAR const* Get(CHAR const* Id, UINT Length=0, BOOL CaseSensitive=false)const { return GetInternal<CHAR const*, CHAR const*, UINT, BOOL>(Id, Length, CaseSensitive); }

	// Modification
	inline BOOL Add(CHAR const* Id, CHAR const* Item, UINT IdLength=0, UINT ItemLength=0, BOOL CaseSensitive=false) { return AddStringInternal(Id, IdLength, CaseSensitive, Item, ItemLength); }
	inline BOOL Remove(CHAR const* Id, UINT Length=0, BOOL CaseSensitive=false) { return RemoveInternal(Id, Length, CaseSensitive); }
	inline VOID Set(CHAR const* Id, CHAR const* Item, UINT IdLength=0, UINT ItemLength=0, BOOL CaseSensitive=false) { SetStringInternal(Id, IdLength, CaseSensitive, Item, ItemLength); }
};

// String-Index with Shared Strings
template<class CHAR_ID, class CHAR_ITEM, UINT _GroupSize>
class StringIndex<CHAR_ID, true, String<CHAR_ITEM, false>, _GroupSize>: public IndexBase<String<CHAR_ID, true>, String<CHAR_ITEM, false>, _GroupSize>
{
public:
	// Access
	inline CHAR const* operator[](CHAR const* Id)const { return GetInternal<CHAR const*, CHAR const*>(Id); }
	inline BOOL Contains(CHAR const* Id, UINT Length=0, BOOL CaseSensitive=false)const { return ContainsInternal(Id, Length, CaseSensitive); }
	inline IteratorReadWrite Find(CHAR const* Id, UINT Length=0, BOOL CaseSensitive=false) { return IteratorReadWrite(this, Id, Length, CaseSensitive); }
	inline IteratorReadOnly Find(CHAR const* Id, UINT Length=0, BOOL CaseSensitive=false)const { return IteratorReadOnly(this, Id, Length, CaseSensitive); }
	inline CHAR const* Get(CHAR const* Id, UINT Length=0, BOOL CaseSensitive=false)const { return GetInternal<CHAR const*, CHAR const*, UINT, BOOL>(Id, Length, CaseSensitive); }

	// Modification
	inline BOOL Add(CHAR const* Id, CHAR const* Item, UINT IdLength=0, BOOL CaseSensitive=false) { return AddStringInternal(Id, IdLength, CaseSensitive, Item); }
	inline VOID Set(CHAR const* Id, CHAR const* Item, UINT IdLength=0, BOOL CaseSensitive=false) { SetStringInternal(Id, IdLength, CaseSensitive, Item); }
};


//=====================
// Shared String-Index
//=====================

// Shared String-Index
template<class CHAR, class ITEM, UINT _GroupSize>
class StringIndex<CHAR, false, ITEM, _GroupSize>: public IndexBase<String<CHAR, false>, ITEM, _GroupSize>
{
public:
	// Access
	inline ITEM operator[](CHAR const* Id)const { return GetInternal<ITEM const&, CHAR const*>(Id); }
	inline BOOL Contains(CHAR const* Id, UINT Length=0, BOOL CaseSensitive=false)const { return ContainsInternal(Id, Length, CaseSensitive); }
	inline IteratorReadWrite Find(CHAR const* Id, UINT Length=0, BOOL CaseSensitive=false) { return IteratorReadWrite(this, Id, Length, CaseSensitive); }
	inline IteratorReadOnly Find(CHAR const* Id, UINT Length=0, BOOL CaseSensitive=false)const { return IteratorReadOnly(this, Id, Length, CaseSensitive); }
	inline ITEM Get(CHAR const* Id, UINT Length=0, BOOL CaseSensitive=false)const { return GetInternal<ITEM const&, CHAR const*, UINT, BOOL>(Id, Length, CaseSensitive); }
	inline BOOL TryGet(CHAR const* Id, ITEM* Item, UINT Length=0, BOOL CaseSensitive=false)const { return TryGetInternal<ITEM, CHAR const*, UINT, BOOL>(Id, Length, CaseSensitive); }

	// Modification
	inline BOOL Add(CHAR const* Id, ITEM const& Item, BOOL CaseSensitive=false) { return AddStringInternal<CHAR, ITEM const&>(Id, 0, CaseSensitive, Item); }
	inline BOOL Remove(CHAR const* Id, UINT Length=0, BOOL CaseSensitive=false) { return RemoveInternal(Id, Length, CaseSensitive); }
	inline VOID Set(CHAR const* Id, ITEM const& Item, BOOL CaseSensitive=false) { SetStringInternal<CHAR, ITEM const&>(Id, 0, CaseSensitive, Item); }
};

// Shared String-Index without Items
template <class CHAR, UINT _GroupSize>
class StringIndex<CHAR, false, VOID, _GroupSize>: public IndexBase<String<CHAR, false>, VOID, _GroupSize>
{
public:
	// Access
	inline BOOL Contains(CHAR const* Id, UINT Length=0, BOOL CaseSensitive=false)const { return ContainsInternal(Id, Length, CaseSensitive); }
	inline IteratorReadWrite Find(CHAR const* Id, UINT Length=0, BOOL CaseSensitive=false) { return IteratorReadWrite(this, Id, Length, CaseSensitive); }
	inline IteratorReadOnly Find(CHAR const* Id, UINT Length=0, BOOL CaseSensitive=false)const { return IteratorReadOnly(this, Id, Length, CaseSensitive); }

	// Modification
	inline BOOL Add(CHAR const* Id, BOOL CaseSensitive=false) { return AddStringInternal(Id, 0, CaseSensitive); }
	inline BOOL Remove(CHAR const* Id, UINT Length=0, BOOL CaseSensitive=false) { return RemoveInternal(Id, Length, CaseSensitive); }
};

// Shared String-Index with Pointers
template<class CHAR, class ITEM, UINT _GroupSize>
class StringIndex<CHAR, false, ITEM*, _GroupSize>: public IndexBase<String<CHAR, false>, ITEM*, _GroupSize>
{
public:
	// Access
	inline ITEM* operator[](CHAR const* Id)const { return GetInternal<ITEM*, CHAR const*>(Id); }
	inline BOOL Contains(CHAR const* Id, UINT Length=0, BOOL CaseSensitive=false)const { return ContainsInternal(Id, Length, CaseSensitive); }
	inline IteratorReadWrite Find(CHAR const* Id, UINT Length=0, BOOL CaseSensitive=false) { return IteratorReadWrite(this, Id, Length, CaseSensitive); }
	inline IteratorReadOnly Find(CHAR const* Id, UINT Length=0, BOOL CaseSensitive=false)const { return IteratorReadOnly(this, Id, Length, CaseSensitive); }
	inline ITEM* Get(CHAR const* Id, UINT Length=0, BOOL CaseSensitive=false)const { return GetInternal<ITEM*, CHAR const*, UINT, BOOL>(Id, Length, CaseSensitive); }

	// Modification
	inline BOOL Add(CHAR const* Id, ITEM* Item, BOOL CaseSensitive=false) { return AddStringInternal(Id, 0, CaseSensitive, Item); }
	inline BOOL Remove(CHAR const* Id, UINT Length=0, BOOL CaseSensitive=false) { return RemoveInternal(Id, Length, CaseSensitive); }
	inline VOID Set(CHAR const* Id, ITEM* Item, BOOL CaseSensitive=false) { SetStringInternal(Id, 0, CaseSensitive, Item); }
};

#ifdef __cplusplus_winrt
// Shared String-Index with Handles
template<class CHAR, class ITEM, UINT _GroupSize>
class StringIndex<CHAR, false, ITEM^, _GroupSize>: public IndexBase<String<CHAR, false>, ITEM^, _GroupSize>
{
public:
	// Access
	inline ITEM^ operator[](CHAR const* Id)const { return GetInternal<ITEM^, CHAR const*>(Id); }
	inline BOOL Contains(CHAR const* Id, UINT Length=0, BOOL CaseSensitive=false)const { return ContainsInternal(Id, Length, CaseSensitive); }
	inline IteratorReadWrite Find(CHAR const* Id, UINT Length=0, BOOL CaseSensitive=false) { return IteratorReadWrite(this, Id, Length, CaseSensitive); }
	inline IteratorReadOnly Find(CHAR const* Id, UINT Length=0, BOOL CaseSensitive=false)const { return IteratorReadOnly(this, Id, Length, CaseSensitive); }
	inline ITEM^ Get(CHAR const* Id, UINT Length=0, BOOL CaseSensitive=false)const { return GetInternal<ITEM^, CHAR const*, UINT, BOOL>(Id, Length, CaseSensitive); }

	// Modification
	inline BOOL Add(CHAR const* Id, ITEM^ Item, BOOL CaseSensitive=false) { return AddStringInternal(Id, 0, CaseSensitive, Item); }
	inline BOOL Remove(CHAR const* Id, UINT Length=0, BOOL CaseSensitive=false) { return RemoveInternal(Id, Length, CaseSensitive); }
	inline VOID Set(CHAR const* Id, ITEM^ Item, BOOL CaseSensitive=false) { AddStringInternal(Id, 0, CaseSensitive, Item); }
};
#endif

// Shared String-Index with Strings
template<class CHAR_ID, class CHAR_ITEM, UINT _GroupSize>
class StringIndex<CHAR_ID, false, String<CHAR_ITEM, true>, _GroupSize>: public IndexBase<String<CHAR_ID, false>, String<CHAR_ITEM, true>, _GroupSize>
{
public:
	// Access
	inline CHAR const* operator[](CHAR const* Id)const { return GetInternal<CHAR const*, CHAR const*>(Id); }
	inline BOOL Contains(CHAR const* Id, UINT Length=0, BOOL CaseSensitive=false)const { return ContainsInternal(Id, Length, CaseSensitive); }
	inline IteratorReadWrite Find(CHAR const* Id, UINT Length=0, BOOL CaseSensitive=false) { return IteratorReadWrite(this, Id, Length, CaseSensitive); }
	inline IteratorReadOnly Find(CHAR const* Id, UINT Length=0, BOOL CaseSensitive=false)const { return IteratorReadOnly(this, Id, Length, CaseSensitive); }
	inline CHAR const* Get(CHAR const* Id, UINT Length=0, BOOL CaseSensitive=false)const { return GetInternal<CHAR const*, CHAR const*, UINT, BOOL>(Id, Length, CaseSensitive); }

	// Modification
	inline BOOL Add(CHAR const* Id, CHAR const* Item, UINT ItemLength=0, BOOL CaseSensitive=false) { return AddStringInternal(Id, 0, CaseSensitive, Item, ItemLength); }
	inline BOOL Remove(CHAR const* Id, UINT Length=0, BOOL CaseSensitive=false) { return RemoveInternal(Id, Length, CaseSensitive); }
	inline VOID Set(CHAR const* Id, CHAR const* Item, UINT ItemLength=0, BOOL CaseSensitive=false) { SetStringInternal(Id, 0, CaseSensitive, Item, ItemLength); }
};

// Shared String-Index with Shared Strings
template<class CHAR_ID, class CHAR_ITEM, UINT _GroupSize>
class StringIndex<CHAR_ID, false, String<CHAR_ITEM, false>, _GroupSize>: public IndexBase<String<CHAR_ID, false>, String<CHAR_ITEM, false>, _GroupSize>
{
public:
	// Access
	inline CHAR const* operator[](CHAR const* Id)const { return GetInternal<CHAR const*, CHAR const*>(Id, 0); }
	inline BOOL Contains(CHAR const* Id, UINT Length=0, BOOL CaseSensitive=false)const { return ContainsInternal(Id, Length, CaseSensitive); }
	inline IteratorReadWrite Find(CHAR const* Id, UINT Length=0, BOOL CaseSensitive=false) { return IteratorReadWrite(this, Id, Length, CaseSensitive); }
	inline IteratorReadOnly Find(CHAR const* Id, UINT Length=0, BOOL CaseSensitive=false)const { return IteratorReadOnly(this, Id, Length, CaseSensitive); }
	inline CHAR const* Get(CHAR const* Id, UINT Length=0, BOOL CaseSensitive=false)const { return GetInternal<CHAR const*, CHAR const*, UINT, BOOL>(Id, Length, CaseSensitive); }

	// Modification
	inline BOOL Add(CHAR const* Id, CHAR const* Item, BOOL CaseSensitive=false) { return AddStringInternal(Id, 0, CaseSensitive, Item); }
	inline BOOL Remove(CHAR const* Id, UINT Length=0, BOOL CaseSensitive=false) { return RemoveInternal(Id, Length, CaseSensitive); }
	inline VOID Set(CHAR const* Id, CHAR const* Item, BOOL CaseSensitive=false) { SetStringInternal(Id, 0, CaseSensitive, Item); }
};

}}}}
