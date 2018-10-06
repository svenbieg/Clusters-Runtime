//=========
// Index.h
//=========

#pragma once


//=======
// Using
//=======

#include "StringIndex.h"


//===========
// Namespace
//===========

namespace Clusters {
	namespace Details {
		namespace Index {


//=======
// Index
//=======

// Index
template<class ID, class ITEM, UINT _GroupSize>
class Index: public IndexBase<ID, ITEM, _GroupSize>
{
public:
	// Access
	inline ITEM operator[](ID const& Id)const { return GetInternal<ITEM const&, ID const&>(Id); }
	inline BOOL Contains(ID const& Id)const { return ContainsInternal<ID const&>(Id); }
	inline IteratorReadWrite Find(ID const& Id) { return IteratorReadWrite(this, 0, Id); }
	inline IteratorReadOnly Find(ID const& Id)const { return IteratorReadOnly(this, 0, Id); }
	inline ITEM Get(ID const& Id)const { return GetInternal<ITEM const&, ID const&>(Id); }
	inline BOOL TryGet(ID const& Id, ITEM* Item)const { return TryGetInternal(Id, Item); }

	// Modification
	inline BOOL Add(ID const& Id, ITEM const& Item) { return AddInternal<ID const&, ITEM const&>(Id, Item); }
	inline BOOL Remove(ID const& Id) { return RemoveInternal<ID const&>(Id); }
	inline VOID Set(ID const& Id, ITEM const& Item) { SetInternal<ID const&, ITEM const&>(Id, Item); }
};

// Index without Items
template<class ID, UINT _GroupSize>
class Index<ID, VOID, _GroupSize>: public IndexBase<ID, VOID, _GroupSize>
{
public:
	// Access
	inline BOOL Contains(ID const& Id)const { return ContainsInternal<ID const&>(Id); }
	inline IteratorReadWrite Find(ID const& Id) { return IteratorReadWrite(this, 0, Id); }
	inline IteratorReadOnly Find(ID const& Id)const { return IteratorReadOnly(this, 0, Id); }

	// Modification
	inline BOOL Add(ID const& Id) { return AddInternal<ID const&>(Id); }
	inline BOOL Remove(ID const& Id) { return RemoveInternal<ID const&>(Id); }
};

// Index with Pointer-Items
template<class ID, class ITEM, UINT _GroupSize>
class Index<ID, ITEM*, _GroupSize>: public IndexBase<ID, ITEM*, _GroupSize>
{
public:
	// Access
	inline ITEM* operator[](ID const& Id)const { return GetInternal<ITEM*, ID const& Id>(Id); }
	inline BOOL Contains(ID const& Id)const { return ContainsInternal<ID const&>(Id); }
	inline IteratorReadWrite Find(ID const& Id) { return IteratorReadWrite(this, 0, Id); }
	inline IteratorReadOnly Find(ID const& Id)const { return IteratorReadOnly(this, 0, Id); }
	inline ITEM* Get(ID const& Id)const { return GetInternal<ITEM*, ID const& Id>(Id); }

	// Modification
	inline BOOL Add(ID const& Id, ITEM* Item) { return AddInternal<ID const&, ITEM*>(Id, Item); }
	inline BOOL Remove(ID const& Id) { return RemoveInternal<ID const&>(Id); }
	inline VOID Set(ID const& Id, ITEM* Item) { SetInternal<ID const&, ITEM*>(Id, Item); }
};

#ifdef __cplusplus_winrt
// Index with Handle-Items
template<class ID, class ITEM, UINT _GroupSize>
class Index<ID, ITEM^, _GroupSize>: public IndexBase<ID, ITEM^, _GroupSize>
{
public:
	// Access
	inline ITEM^ operator[](ID const& Id)const { return GetInternal<ITEM^, ID const&>(Id); }
	inline BOOL Contains(ID const& Id)const { return ContainsInternal<ID const&>(Id); }
	inline IteratorReadWrite Find(ID const& Id) { return IteratorReadWrite(this, 0, Id); }
	inline IteratorReadOnly Find(ID const& Id)const { return IteratorReadOnly(this, 0, Id); }
	inline ITEM^ Get(ID const& Id)const { return GetInternal<ITEM^, ID const&>(Id); }

	// Modification
	inline BOOL Add(ID const& Id, ITEM^ Item) { return AddInternal<ID const&, ITEM^>(Id, Item); }
	inline BOOL Remove(ID const& Id) { return RemoveInternal<ID const&>(Id); }
	inline VOID Set(ID const& Id, ITEM^ Item) { SetInternal<ID const&, ITEM^>(Id, Item); }
};
#endif

// Index with String-Items
template<class ID, class CHAR, UINT _GroupSize>
class Index<ID, String<CHAR, true>, _GroupSize>: public IndexBase<ID, String<CHAR, true>, _GroupSize>
{
public:
	// Access
	inline CHAR const* operator[](ID const& Id)const { return GetInternal<CHAR const*, ID const&>(Id); }
	inline BOOL Contains(ID const& Id)const { return ContainsInternal<ID const&>(Id); }
	inline IteratorReadWrite Find(ID const& Id) { return IteratorReadWrite(this, 0, Id); }
	inline IteratorReadOnly Find(ID const& Id)const { return IteratorReadOnly(this, 0, Id); }
	inline CHAR const* Get(ID const& Id)const { return GetInternal<CHAR const*, ID const&>(Id); }

	// Modification
	inline BOOL Add(ID const& Id, CHAR const* Item, UINT Length=0) { return AddInternal<ID const&, CHAR const*, UINT>(Id, Item, Length); }
	inline BOOL Remove(ID const& Id) { return RemoveInternal<ID const&>(Id); }
	inline VOID Set(ID const& Id, CHAR const* Item, UINT Length=0) { SetInternal<ID const&, CHAR const*, UINT>(Id, Item, Length); }
};

// Index with Shared String-Items
template<class ID, class CHAR, UINT _GroupSize>
class Index<ID, String<CHAR, false>, _GroupSize>: public IndexBase<ID, String<CHAR, false>, _GroupSize>
{
public:
	// Access
	inline CHAR const* operator[](ID const& Id)const { return GetInternal<CHAR const*, ID const&>(Id); }
	inline BOOL Contains(ID const& Id)const { return ContainsInternal<ID const&>(Id); }
	inline IteratorReadWrite Find(ID const& Id) { return IteratorReadWrite(this, 0, Id); }
	inline IteratorReadOnly Find(ID const& Id)const { return IteratorReadOnly(this, 0, Id); }
	inline CHAR const* Get(ID const& Id)const { return GetInternal<CHAR const*, ID const&>(Id); }

	// Modification
	inline BOOL Add(ID const& Id, CHAR const* Item) { return AddInternal<ID const&, CHAR const*>(Id, Item); }
	inline BOOL Remove(ID const& Id) { return RemoveInternal<ID const&>(Id); }
	inline VOID Set(ID const& Id, CHAR const* Item) { SetInternal<ID const&, CHAR const*>(Id, Item); }
};


//===============
// Pointer-Index
//===============

// Pointer-Index
template<class ID, class ITEM, UINT _GroupSize>
class Index<ID*, ITEM, _GroupSize>: public IndexBase<ID*, ITEM, _GroupSize>
{
public:
	// Access
	inline ITEM operator[](ID* Id)const { return GetInternal<ITEM const&, ID*>(Id); }
	inline BOOL Contains(ID* Id)const { return ContainsInternal(Id); }
	inline IteratorReadWrite Find(ID* Id) { return IteratorReadWrite(this, Id); }
	inline IteratorReadOnly Find(ID* Id)const { return IteratorReadOnly(this, Id); }
	inline ITEM Get(ID* Id)const { return GetInternal<ITEM const&, ID*>(Id); }
	inline BOOL TryGet(ID* Id, ITEM* Item)const { return TryGetInternal<ITEM, ID*>(Id, Item); }

	// Modification
	inline BOOL Add(ID* Id, ITEM const& Item) { return AddInternal<ID*, ITEM const&>(Id, Item); }
	inline BOOL Remove(ID* Id) { return RemoveInternal(Id); }
	inline VOID Set(ID* Id, ITEM const& Item) { SetInternal<ID*, ITEM const&>(Id, Item); }
};

// Pointer-Index without Items
template<class ID, UINT _GroupSize>
class Index<ID*, VOID, _GroupSize>: public IndexBase<ID*, VOID, _GroupSize>
{
public:
	// Access
	inline BOOL Contains(ID* Id)const { return ContainsInternal(Id); }
	inline IteratorReadWrite Find(ID* Id) { return IteratorReadWrite(this, Id); }
	inline IteratorReadOnly Find(ID* Id)const { return IteratorReadOnly(this, Id); }

	// Modification
	inline BOOL Add(ID* Id) { return AddInternal(Id); }
	inline BOOL Remove(ID* Id) { return RemoveInternal(Id); }
};

// Pointer-Index with Pointer-Items
template<class ID, class ITEM, UINT _GroupSize>
class Index<ID*, ITEM*, _GroupSize>: public IndexBase<ID*, ITEM*, _GroupSize>
{
public:
	// Access
	inline ITEM* operator[](ID* Id)const { return GetInternal<ITEM*, ID*>(Id); }
	inline BOOL Contains(ID* Id)const { return ContainsInternal(Id); }
	inline IteratorReadWrite Find(ID* Id) { return IteratorReadWrite(this, Id); }
	inline IteratorReadOnly Find(ID* Id)const { return IteratorReadOnly(this, Id); }
	inline ITEM* Get(ID* Id)const { return GetInternal<ITEM*, ID*>(Id); }

	// Modification
	inline BOOL Add(ID* Id, ITEM* Item) { return AddInternal(Id, Item); }
	inline BOOL Remove(ID* Id) { return RemoveInternal(Id); }
	inline VOID Set(ID* Id, ITEM* Item) { SetInternal(Id, Item); }
};

#ifdef __cplusplus_winrt
// Pointer-Index with Handle-Items
template<class ID, class ITEM, UINT _GroupSize>
class Index<ID*, ITEM^, _GroupSize>: public IndexBase<ID*, ITEM^, _GroupSize>
{
public:
	// Access
	inline ITEM^ operator[](ID* Id)const { return GetInternal<ITEM^, ID*>(Id); }
	inline BOOL Contains(ID* Id)const { return ContainsInternal(Id); }
	inline IteratorReadWrite Find(ID* Id) { return IteratorReadWrite(this, Id); }
	inline IteratorReadOnly Find(ID* Id)const { return IteratorReadOnly(this, Id); }
	inline ITEM^ Get(ID* Id)const { return GetInternal<ITEM^, ID*>(Id); }

	// Modification
	inline BOOL Add(ID* Id, ITEM^ Item) { return AddInternal(Id, Item); }
	inline BOOL Remove(ID* Id) { return RemoveInternal(Id); }
	inline VOID Set(ID* Id, ITEM^ Item) { SetInternal(Id, Item); }
};
#endif

// Pointer-Index with String-Items
template<class ID, class CHAR, UINT _GroupSize>
class Index<ID*, String<CHAR, true>, _GroupSize>: public IndexBase<ID*, String<CHAR, true>, _GroupSize>
{
public:
	// Access
	inline CHAR const* operator[](ID* Id)const { return GetInternal<CHAR const*, ID*>(Id); }
	inline BOOL Contains(ID* Id)const { return ContainsInternal(Id); }
	inline IteratorReadWrite Find(ID* Id) { return IteratorReadWrite(this, Id); }
	inline IteratorReadOnly Find(ID* Id)const { return IteratorReadOnly(this, Id); }
	inline CHAR const* Get(ID* Id)const { return GetInternal<CHAR const*, ID*>(Id); }

	// Modification
	inline BOOL Add(ID* Id, CHAR const* Item, UINT Length=0) { return AddInternal(Id, Item, Length); }
	inline BOOL Remove(ID* Id) { return RemoveInternal(Id); }
	inline VOID Set(ID* Id, CHAR const* Item, UINT Length=0) { SetInternal(Id, Item, Length); }
};

// Pointer-Index with Shared String-Items
template<class ID, class CHAR, UINT _GroupSize>
class Index<ID*, String<CHAR, false>, _GroupSize>: public IndexBase<ID*, String<CHAR, false>, _GroupSize>
{
public:
	// Access
	inline CHAR const* operator[](ID* Id)const { return GetInternal<CHAR const*, ID*>(Id); }
	inline BOOL Contains(ID* Id)const { return ContainsInternal(Id); }
	inline IteratorReadWrite Find(ID* Id) { return IteratorReadWrite(this, Id); }
	inline IteratorReadOnly Find(ID* Id)const { return IteratorReadOnly(this, Id); }
	inline CHAR const* Get(ID* Id)const { return GetInternal<CHAR const*, ID*>(Id); }

	// Modification
	inline BOOL Add(ID* Id, CHAR const* Item) { return AddInternal(Id, Item); }
	inline BOOL Remove(ID* Id) { return RemoveInternal(Id); }
	inline VOID Set(ID* Id, CHAR const* Item) { SetInternal(Id, Item); }
};


//==============
// Handle-Index
//==============

#ifdef __cplusplus_winrt

// Handle-Index
template<class ID, class ITEM, UINT _GroupSize>
class Index<ID^, ITEM, _GroupSize>: public IndexBase<ID^, ITEM, _GroupSize>
{
public:
	// Access
	inline ITEM operator[](ID^ Id)const { return GetInternal<ITEM const&, ID^>(Id); }
	inline BOOL Contains(ID^ Id)const { return ContainsInternal(Id); }
	inline IteratorReadWrite Find(ID^ Id) { return IteratorReadWrite(this, Id); }
	inline IteratorReadOnly Find(ID^ Id)const { return IteratorReadOnly(this, Id); }
	inline ITEM Get(ID^ Id)const { return GetInternal<ITEM const&, ID^>(Id); }
	inline BOOL TryGet(ID^ Id, ITEM* Item)const { return TryGetInternal(Id, Item); }

	// Modification
	inline BOOL Add(ID^ Id, ITEM const& Item) { return AddInternal<ID^, ITEM const&>(Id, Item); }
	inline BOOL Remove(ID^ Id) { return RemoveInternal(Id); }
	inline VOID Set(ID^ Id, ITEM const& Item) { SetInternal<ID^, ITEM const&>(Id, Item); }
};

// Handle-Index without Items
template<class ID, UINT _GroupSize>
class Index<ID^, VOID, _GroupSize>: public IndexBase<ID^, VOID, _GroupSize>
{
public:
	// Access
	inline BOOL Contains(ID^ Id)const { return ContainsInternal(Id); }
	inline IteratorReadWrite Find(ID^ Id) { return IteratorReadWrite(this, Id); }
	inline IteratorReadOnly Find(ID^ Id)const { return IteratorReadOnly(this, Id); }

	// Modification
	inline BOOL Add(ID^ Id) { return AddInternal(Id); }
	inline BOOL Remove(ID^ Id) { return RemoveInternal(Id); }
};

// Handle-Index with Pointer-Items
template<class ID, class ITEM, UINT _GroupSize>
class Index<ID^, ITEM*, _GroupSize>: public IndexBase<ID^, ITEM*, _GroupSize>
{
public:
	// Access
	inline ITEM* operator[](ID^ Id)const { return GetInternal<ITEM*, ID^>(Id); }
	inline BOOL Contains(ID^ Id)const { return ContainsInternal(Id); }
	inline IteratorReadWrite Find(ID^ Id) { return IteratorReadWrite(this, Id); }
	inline IteratorReadOnly Find(ID^ Id)const { return IteratorReadOnly(this, Id); }
	inline ITEM* Get(ID^ Id)const { return GetInternal<ITEM*, ID^>(Id); }

	// Modification
	inline BOOL Add(ID^ Id, ITEM* Item) { return AddInternal(Id, Item); }
	inline BOOL Remove(ID^ Id) { return RemoveInternal(Id); }
	inline VOID Set(ID^ Id, ITEM* Item) { SetInternal(Id, Item); }
};

// Handle-Index with Handle-Items
template<class ID, class ITEM, UINT _GroupSize>
class Index<ID^, ITEM^, _GroupSize>: public IndexBase<ID^, ITEM^, _GroupSize>
{
public:
	// Access
	inline ITEM^ operator[](ID^ Id)const { return GetInternal<ITEM^, ID^>(Id); }
	inline BOOL Contains(ID^ Id)const { return ContainsInternal(Id); }
	inline IteratorReadWrite Find(ID^ Id) { return IteratorReadWrite(this, Id); }
	inline IteratorReadOnly Find(ID^ Id)const { return IteratorReadOnly(this, Id); }
	inline ITEM^ Get(ID^ Id)const { return GetInternal<ITEM^, ID^>(Id); }

	// Modification
	inline BOOL Add(ID^ Id, ITEM^ Item) { return AddInternal(Id, Item); }
	inline BOOL Remove(ID^ Id) { return RemoveInternal(Id); }
	inline VOID Set(ID^ Id, ITEM^ Item) { SetInternal(Id, Item); }
};

// Handle-Index with String-Items
template<class ID, class CHAR, UINT _GroupSize>
class Index<ID^, String<CHAR, true>, _GroupSize>: public IndexBase<ID^, String<CHAR, true>, _GroupSize>
{
public:
	// Access
	inline CHAR const* operator[](ID^ Id)const { return GetInternal<CHAR const*, ID^>(Id); }
	inline BOOL Contains(ID^ Id)const { return ContainsInternal(Id); }
	inline IteratorReadWrite Find(ID^ Id) { return IteratorReadWrite(this, Id); }
	inline IteratorReadOnly Find(ID^ Id)const { return IteratorReadOnly(this, Id); }
	inline CHAR const* Get(ID^ Id)const { return GetInternal<CHAR const*, ID^>(Id); }

	// Modification
	inline BOOL Add(ID^ Id, CHAR const* Item, UINT Length=0) { return AddInternal(Id, Item, Length); }
	inline BOOL Remove(ID^ Id) { return RemoveInternal(Id); }
	inline VOID Set(ID^ Id, CHAR const* Item, UINT Length=0) { SetInternal(Id, Item, Length); }
};

// Handle-Index with Shared String-Items
template<class ID, class CHAR, UINT _GroupSize>
class Index<ID^, String<CHAR, false>, _GroupSize>: public IndexBase<ID^, String<CHAR, false>, _GroupSize>
{
public:
	// Access
	inline CHAR const* operator[](ID^ Id)const { return GetInternal<CHAR const*, ID^>(Id); }
	inline BOOL Contains(ID^ Id)const { return ContainsInternal(Id); }
	inline IteratorReadWrite Find(ID^ Id) { return IteratorReadWrite(this, Id); }
	inline IteratorReadOnly Find(ID^ Id)const { return IteratorReadOnly(this, Id); }
	inline CHAR const* Get(ID^ Id)const { return GetInternal<CHAR const*, ID^>(Id); }

	// Modification
	inline BOOL Add(ID^ Id, CHAR const* Item) { return AddInternal(Id, Item); }
	inline BOOL Remove(ID^ Id) { return RemoveInternal(Id); }
	inline VOID Set(ID^ Id, CHAR const* Item) { SetInternal(Id, Item); }
};

#endif


//============
// Ansi-Index
//============

template <class ITEM, UINT _GroupSize> class Index<LPSTR, ITEM, _GroupSize>: public StringIndex<CHAR, true, ITEM, _GroupSize> {}; // Ansi-Index
template <UINT _GroupSize> class Index<LPSTR, VOID, _GroupSize>: public StringIndex<CHAR, true, VOID, _GroupSize> {}; // Ansi-Index without Items
template <class ITEM, UINT _GroupSize> class Index<LPSTR, ITEM*, _GroupSize>: public StringIndex<CHAR, true, ITEM*, _GroupSize> {}; // Ansi-Index with Pointer-Items
#ifdef __cplusplus_winrt
template <class ITEM, UINT _GroupSize> class Index<LPSTR, ITEM^, _GroupSize>: public StringIndex<CHAR, true, ITEM^, _GroupSize> {}; // Ansi-Index with Handle-Items
#endif
template <UINT _GroupSize> class Index<LPSTR, LPSTR, _GroupSize>: public StringIndex<CHAR, true, String<CHAR, true>, _GroupSize> {}; // Ansi-Index with Ansi-Items
template <UINT _GroupSize> class Index<LPSTR, LPCSTR, _GroupSize>: public StringIndex<CHAR, true, String<CHAR, false>, _GroupSize> {}; // Ansi-Index with Shared Ansi-Items
template <UINT _GroupSize> class Index<LPSTR, LPWSTR, _GroupSize>: public StringIndex<CHAR, true, String<WCHAR, true>, _GroupSize> {}; // Ansi-Index with Unicode-Items
template <UINT _GroupSize> class Index<LPSTR, LPCWSTR, _GroupSize>: public StringIndex<CHAR, true, String<WCHAR, false>, _GroupSize> {}; // Ansi-Index with Shared Unicode-Items


//===================
// Shared Ansi-Index
//===================

template <class ITEM, UINT _GroupSize> class Index<LPCSTR, ITEM, _GroupSize>: public StringIndex<CHAR, false, ITEM, _GroupSize> {}; // Shared Ansi-Index
template <UINT _GroupSize> class Index<LPCSTR, VOID, _GroupSize>: public StringIndex<CHAR, false, VOID, _GroupSize> {}; // Shared Ansi-Index without Items
template <class ITEM, UINT _GroupSize> class Index<LPCSTR, ITEM*, _GroupSize>: public StringIndex<CHAR, false, ITEM*, _GroupSize> {}; // Shared Ansi-Index with Pointer-Items
#ifdef __cplusplus_winrt
template <class ITEM, UINT _GroupSize> class Index<LPCSTR, ITEM^, _GroupSize>: public StringIndex<CHAR, false, ITEM^, _GroupSize> {}; // Shared Ansi-Index with Handle-Items
#endif
template <UINT _GroupSize> class Index<LPCSTR, LPSTR, _GroupSize>: public StringIndex<CHAR, false, String<CHAR, true>, _GroupSize> {}; // Shared Ansi-Index with Ansi-Items
template <UINT _GroupSize> class Index<LPCSTR, LPCSTR, _GroupSize>: public StringIndex<CHAR, false, String<CHAR, false>, _GroupSize> {}; // Shared Ansi-Index with Shared Ansi-Items
template <UINT _GroupSize> class Index<LPCSTR, LPWSTR, _GroupSize>: public StringIndex<CHAR, false, String<WCHAR, true>, _GroupSize> {}; // Shared Ansi-Index with Unicode-Items
template <UINT _GroupSize> class Index<LPCSTR, LPCWSTR, _GroupSize>: public StringIndex<CHAR, false, String<WCHAR, false>, _GroupSize> {}; // Shared Ansi-Index with Shared Unicode-Items


//===============
// Unicode-Index
//===============

template <class ITEM, UINT _GroupSize> class Index<LPWSTR, ITEM, _GroupSize>: public StringIndex<WCHAR, true, ITEM, _GroupSize> {}; // Unicode-Index
template <UINT _GroupSize> class Index<LPWSTR, VOID, _GroupSize>: public StringIndex<WCHAR, true, VOID, _GroupSize> {}; // Unicode-Index without Items
template <class ITEM, UINT _GroupSize> class Index<LPWSTR, ITEM*, _GroupSize>: public StringIndex<WCHAR, true, ITEM*, _GroupSize> {}; // Unicode-Index with Pointer-Items
#ifdef __cplusplus_winrt
template <class ITEM, UINT _GroupSize> class Index<LPWSTR, ITEM^, _GroupSize>: public StringIndex<WCHAR, true, ITEM^, _GroupSize> {}; // Unicode-Index with Handle-Items
#endif
template <UINT _GroupSize> class Index<LPWSTR, LPSTR, _GroupSize>: public StringIndex<WCHAR, true, String<CHAR, true>, _GroupSize> {}; // Unicode-Index with Ansi-Items
template <UINT _GroupSize> class Index<LPWSTR, LPCSTR, _GroupSize>: public StringIndex<WCHAR, true, String<CHAR, false>, _GroupSize> {}; // Unicode-Index with Shared Ansi-Items
template <UINT _GroupSize> class Index<LPWSTR, LPWSTR, _GroupSize>: public StringIndex<WCHAR, true, String<WCHAR, true>, _GroupSize> {}; // Unicode-Index with Unicode-Items
template <UINT _GroupSize> class Index<LPWSTR, LPCWSTR, _GroupSize>: public StringIndex<WCHAR, true, String<WCHAR, false>, _GroupSize> {}; // Unicode-Index with Shared Unicode-Items


//======================
// Shared Unicode-Index
//======================

template <class ITEM, UINT _GroupSize> class Index<LPCWSTR, ITEM, _GroupSize>: public StringIndex<WCHAR, false, ITEM, _GroupSize> {}; // Shared Unicode-Index
template <UINT _GroupSize> class Index<LPCWSTR, VOID, _GroupSize>: public StringIndex<WCHAR, false, VOID, _GroupSize> {}; // Shared Unicode-Index without Items
template <class ITEM, UINT _GroupSize> class Index<LPCWSTR, ITEM*, _GroupSize>: public StringIndex<WCHAR, false, ITEM*, _GroupSize> {}; // Shared Unicode-Index with Pointer-Items
#ifdef __cplusplus_winrt
template <class ITEM, UINT _GroupSize> class Index<LPCWSTR, ITEM^, _GroupSize>: public StringIndex<WCHAR, false, ITEM^, _GroupSize> {}; // Shared Unicode-Index with Handle-Items
#endif
template <UINT _GroupSize> class Index<LPCWSTR, LPSTR, _GroupSize>: public StringIndex<WCHAR, false, String<CHAR, true>, _GroupSize> {}; // Shared Unicode-Index with Ansi-Items
template <UINT _GroupSize> class Index<LPCWSTR, LPCSTR, _GroupSize>: public StringIndex<WCHAR, false, String<CHAR, false>, _GroupSize> {}; // Shared Unicode-Index with Shared Ansi-Items
template <UINT _GroupSize> class Index<LPCWSTR, LPWSTR, _GroupSize>: public StringIndex<WCHAR, false, String<WCHAR, true>, _GroupSize> {}; // Shared Unicode-Index with Unicode-Items
template <UINT _GroupSize> class Index<LPCWSTR, LPCWSTR, _GroupSize>: public StringIndex<WCHAR, false, String<WCHAR, false>, _GroupSize> {}; // Shared Unicode-Index with Shared Unicode-Items

}}}
