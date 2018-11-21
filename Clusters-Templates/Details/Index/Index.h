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
	namespace Templates {
		namespace Details {
			namespace Index {


//=======
// Index
//=======

// Index
template<class _Id, class _Item, unsigned int _GroupSize>
class Index: public IndexBase<_Id, _Item, _GroupSize>
{
public:
	// Access
	inline _Item operator[](_Id const& Id)const { return GetInternal<_Item const&, _Id const&>(Id); }
	inline bool Contains(_Id const& Id)const { return ContainsInternal<_Id const&>(Id); }
	inline IteratorReadWrite Find(_Id const& Id) { return IteratorReadWrite(this, 0, Id); }
	inline IteratorReadOnly Find(_Id const& Id)const { return IteratorReadOnly(this, 0, Id); }
	inline _Item Get(_Id const& Id)const { return GetInternal<_Item const&, _Id const&>(Id); }
	inline bool TryGet(_Id const& Id, _Item* Item)const { return TryGetInternal(Id, Item); }

	// Modification
	inline bool Add(_Id const& Id, _Item const& Item) { return AddInternal<_Id const&, _Item const&>(Id, Item); }
	inline bool Remove(_Id const& Id) { return RemoveInternal<_Id const&>(Id); }
	inline void Set(_Id const& Id, _Item const& Item) { SetInternal<_Id const&, _Item const&>(Id, Item); }
};

// Index without Items
template<class _Id, unsigned int _GroupSize>
class Index<_Id, void, _GroupSize>: public IndexBase<_Id, void, _GroupSize>
{
public:
	// Access
	inline bool Contains(_Id const& Id)const { return ContainsInternal<_Id const&>(Id); }
	inline IteratorReadWrite Find(_Id const& Id) { return IteratorReadWrite(this, 0, Id); }
	inline IteratorReadOnly Find(_Id const& Id)const { return IteratorReadOnly(this, 0, Id); }

	// Modification
	inline bool Add(_Id const& Id) { return AddInternal<_Id const&>(Id); }
	inline bool Remove(_Id const& Id) { return RemoveInternal<_Id const&>(Id); }
};

// Index with Pointer-Items
template<class _Id, class _Item, unsigned int _GroupSize>
class Index<_Id, _Item*, _GroupSize>: public IndexBase<_Id, _Item*, _GroupSize>
{
public:
	// Access
	inline _Item* operator[](_Id const& Id)const { return GetInternalPointer<_Item*, _Id const& Id>(Id); }
	inline bool Contains(_Id const& Id)const { return ContainsInternal<_Id const&>(Id); }
	inline IteratorReadWrite Find(_Id const& Id) { return IteratorReadWrite(this, 0, Id); }
	inline IteratorReadOnly Find(_Id const& Id)const { return IteratorReadOnly(this, 0, Id); }
	_Item* Get(_Id const& Id)const { return GetInternalPointer<_Item*, _Id const&>(Id); }

	// Modification
	inline bool Add(_Id const& Id, _Item* Item) { return AddInternal<_Id const&, _Item*>(Id, Item); }
	inline bool Remove(_Id const& Id) { return RemoveInternal<_Id const&>(Id); }
	inline void Set(_Id const& Id, _Item* Item) { SetInternal<_Id const&, _Item*>(Id, Item); }
};

#ifdef __cplusplus_winrt
// Index with Handle-Items
template<class _Id, class _Item, unsigned int _GroupSize>
class Index<_Id, _Item^, _GroupSize>: public IndexBase<_Id, _Item^, _GroupSize>
{
public:
	// Access
	inline _Item^ operator[](_Id const& Id)const { return GetInternalPointer<_Item^, _Id const&>(Id); }
	inline bool Contains(_Id const& Id)const { return ContainsInternal<_Id const&>(Id); }
	inline IteratorReadWrite Find(_Id const& Id) { return IteratorReadWrite(this, 0, Id); }
	inline IteratorReadOnly Find(_Id const& Id)const { return IteratorReadOnly(this, 0, Id); }
	_Item^ Get(_Id const& Id)const { return GetInternalPointer<_Item^, _Id const&>(Id); }

	// Modification
	inline bool Add(_Id const& Id, _Item^ Item) { return AddInternal<_Id const&, _Item^>(Id, Item); }
	inline bool Remove(_Id const& Id) { return RemoveInternal<_Id const&>(Id); }
	inline void Set(_Id const& Id, _Item^ Item) { SetInternal<_Id const&, _Item^>(Id, Item); }
};
#endif

// Index with String-Items
template<class _Id, class _Char, unsigned int _GroupSize>
class Index<_Id, String<_Char, true>, _GroupSize>: public IndexBase<_Id, String<_Char, true>, _GroupSize>
{
public:
	// Access
	inline _Char const* operator[](_Id const& Id)const { return GetInternalPointer<_Char const*, _Id const&>(Id); }
	inline bool Contains(_Id const& Id)const { return ContainsInternal<_Id const&>(Id); }
	inline IteratorReadWrite Find(_Id const& Id) { return IteratorReadWrite(this, 0, Id); }
	inline IteratorReadOnly Find(_Id const& Id)const { return IteratorReadOnly(this, 0, Id); }
	inline _Char const* Get(_Id const& Id)const { return GetInternalPointer<_Char const*, _Id const&>(Id); }

	// Modification
	inline bool Add(_Id const& Id, _Char const* Item, unsigned int Length=0) { return AddInternal<_Id const&, _Char const*, unsigned int>(Id, Item, Length); }
	inline bool Remove(_Id const& Id) { return RemoveInternal<_Id const&>(Id); }
	inline void Set(_Id const& Id, _Char const* Item, unsigned int Length=0) { SetInternal<_Id const&, _Char const*, unsigned int>(Id, Item, Length); }
};

// Index with Shared String-Items
template<class _Id, class _Char, unsigned int _GroupSize>
class Index<_Id, String<_Char, false>, _GroupSize>: public IndexBase<_Id, String<_Char, false>, _GroupSize>
{
public:
	// Access
	inline _Char const* operator[](_Id const& Id)const { return GetInternalPointer<_Char const*, _Id const&>(Id); }
	inline bool Contains(_Id const& Id)const { return ContainsInternal<_Id const&>(Id); }
	inline IteratorReadWrite Find(_Id const& Id) { return IteratorReadWrite(this, 0, Id); }
	inline IteratorReadOnly Find(_Id const& Id)const { return IteratorReadOnly(this, 0, Id); }
	inline _Char const* Get(_Id const& Id)const { return GetInternalPointer<_Char const*, _Id const&>(Id); }

	// Modification
	inline bool Add(_Id const& Id, _Char const* Item) { return AddInternal<_Id const&, _Char const*>(Id, Item); }
	inline bool Remove(_Id const& Id) { return RemoveInternal<_Id const&>(Id); }
	inline void Set(_Id const& Id, _Char const* Item) { SetInternal<_Id const&, _Char const*>(Id, Item); }
};


//===============
// Pointer-Index
//===============

// Pointer-Index
template<class _Id, class _Item, unsigned int _GroupSize>
class Index<_Id*, _Item, _GroupSize>: public IndexBase<_Id*, _Item, _GroupSize>
{
public:
	// Access
	inline _Item operator[](_Id* Id)const { return GetInternal<_Item const&, _Id*>(Id); }
	inline bool Contains(_Id* Id)const { return ContainsInternal(Id); }
	inline IteratorReadWrite Find(_Id* Id) { return IteratorReadWrite(this, Id); }
	inline IteratorReadOnly Find(_Id* Id)const { return IteratorReadOnly(this, Id); }
	inline _Item Get(_Id* Id)const { return GetInternal<_Item const&, _Id*>(Id); }
	inline bool TryGet(_Id* Id, _Item* Item)const { return TryGetInternal<_Item, _Id*>(Id, Item); }

	// Modification
	inline bool Add(_Id* Id, _Item const& Item) { return AddInternal<_Id*, _Item const&>(Id, Item); }
	inline bool Remove(_Id* Id) { return RemoveInternal(Id); }
	inline void Set(_Id* Id, _Item const& Item) { SetInternal<_Id*, _Item const&>(Id, Item); }
};

// Pointer-Index without Items
template<class _Id, unsigned int _GroupSize>
class Index<_Id*, void, _GroupSize>: public IndexBase<_Id*, void, _GroupSize>
{
public:
	// Access
	inline bool Contains(_Id* Id)const { return ContainsInternal(Id); }
	inline IteratorReadWrite Find(_Id* Id) { return IteratorReadWrite(this, Id); }
	inline IteratorReadOnly Find(_Id* Id)const { return IteratorReadOnly(this, Id); }

	// Modification
	inline bool Add(_Id* Id) { return AddInternal(Id); }
	inline bool Remove(_Id* Id) { return RemoveInternal(Id); }
};

// Pointer-Index with Pointer-Items
template<class _Id, class _Item, unsigned int _GroupSize>
class Index<_Id*, _Item*, _GroupSize>: public IndexBase<_Id*, _Item*, _GroupSize>
{
public:
	// Access
	inline _Item* operator[](_Id* Id)const { return GetInternalPointer<_Item*, _Id*>(Id); }
	inline bool Contains(_Id* Id)const { return ContainsInternal(Id); }
	inline IteratorReadWrite Find(_Id* Id) { return IteratorReadWrite(this, Id); }
	inline IteratorReadOnly Find(_Id* Id)const { return IteratorReadOnly(this, Id); }
	inline _Item* Get(_Id* Id)const { return GetInternalPointer<_Item*, _Id*>(Id); }

	// Modification
	inline bool Add(_Id* Id, _Item* Item) { return AddInternal(Id, Item); }
	inline bool Remove(_Id* Id) { return RemoveInternal(Id); }
	inline void Set(_Id* Id, _Item* Item) { SetInternal(Id, Item); }
};

#ifdef __cplusplus_winrt
// Pointer-Index with Handle-Items
template<class _Id, class _Item, unsigned int _GroupSize>
class Index<_Id*, _Item^, _GroupSize>: public IndexBase<_Id*, _Item^, _GroupSize>
{
public:
	// Access
	inline _Item^ operator[](_Id* Id)const { return GetInternalPointer<_Item^, _Id*>(Id); }
	inline bool Contains(_Id* Id)const { return ContainsInternal(Id); }
	inline IteratorReadWrite Find(_Id* Id) { return IteratorReadWrite(this, Id); }
	inline IteratorReadOnly Find(_Id* Id)const { return IteratorReadOnly(this, Id); }
	inline _Item^ Get(_Id* Id)const { return GetInternalPointer<_Item^, _Id*>(Id); }

	// Modification
	inline bool Add(_Id* Id, _Item^ Item) { return AddInternal(Id, Item); }
	inline bool Remove(_Id* Id) { return RemoveInternal(Id); }
	inline void Set(_Id* Id, _Item^ Item) { SetInternal(Id, Item); }
};
#endif

// Pointer-Index with String-Items
template<class _Id, class _Char, unsigned int _GroupSize>
class Index<_Id*, String<_Char, true>, _GroupSize>: public IndexBase<_Id*, String<_Char, true>, _GroupSize>
{
public:
	// Access
	inline _Char const* operator[](_Id* Id)const { return GetInternalPointer<_Char const*, _Id*>(Id); }
	inline bool Contains(_Id* Id)const { return ContainsInternal(Id); }
	inline IteratorReadWrite Find(_Id* Id) { return IteratorReadWrite(this, Id); }
	inline IteratorReadOnly Find(_Id* Id)const { return IteratorReadOnly(this, Id); }
	inline _Char const* Get(_Id* Id)const { return GetInternalPointer<_Char const*, _Id*>(Id); }

	// Modification
	inline bool Add(_Id* Id, _Char const* Item, unsigned int Length=0) { return AddInternal(Id, Item, Length); }
	inline bool Remove(_Id* Id) { return RemoveInternal(Id); }
	inline void Set(_Id* Id, _Char const* Item, unsigned int Length=0) { SetInternal(Id, Item, Length); }
};

// Pointer-Index with Shared String-Items
template<class _Id, class _Char, unsigned int _GroupSize>
class Index<_Id*, String<_Char, false>, _GroupSize>: public IndexBase<_Id*, String<_Char, false>, _GroupSize>
{
public:
	// Access
	inline _Char const* operator[](_Id* Id)const { return GetInternalPointer<_Char const*, _Id*>(Id); }
	inline bool Contains(_Id* Id)const { return ContainsInternal(Id); }
	inline IteratorReadWrite Find(_Id* Id) { return IteratorReadWrite(this, Id); }
	inline IteratorReadOnly Find(_Id* Id)const { return IteratorReadOnly(this, Id); }
	inline _Char const* Get(_Id* Id)const { return GetInternalPointer<_Char const*, _Id*>(Id); }

	// Modification
	inline bool Add(_Id* Id, _Char const* Item) { return AddInternal(Id, Item); }
	inline bool Remove(_Id* Id) { return RemoveInternal(Id); }
	inline void Set(_Id* Id, _Char const* Item) { SetInternal(Id, Item); }
};


//==============
// Handle-Index
//==============

#ifdef __cplusplus_winrt

// Handle-Index
template<class _Id, class _Item, unsigned int _GroupSize>
class Index<_Id^, _Item, _GroupSize>: public IndexBase<_Id^, _Item, _GroupSize>
{
public:
	// Access
	inline _Item operator[](_Id^ Id)const { return GetInternal<_Item const&, _Id^>(Id); }
	inline bool Contains(_Id^ Id)const { return ContainsInternal(Id); }
	inline IteratorReadWrite Find(_Id^ Id) { return IteratorReadWrite(this, Id); }
	inline IteratorReadOnly Find(_Id^ Id)const { return IteratorReadOnly(this, Id); }
	inline _Item Get(_Id^ Id)const { return GetInternal<_Item const&, _Id^>(Id); }
	inline bool TryGet(_Id^ Id, _Item* Item)const { return TryGetInternal(Id, Item); }

	// Modification
	inline bool Add(_Id^ Id, _Item const& Item) { return AddInternal<_Id^, _Item const&>(Id, Item); }
	inline bool Remove(_Id^ Id) { return RemoveInternal(Id); }
	inline void Set(_Id^ Id, _Item const& Item) { SetInternal<_Id^, _Item const&>(Id, Item); }
};

// Handle-Index without Items
template<class _Id, unsigned int _GroupSize>
class Index<_Id^, void, _GroupSize>: public IndexBase<_Id^, void, _GroupSize>
{
public:
	// Access
	inline bool Contains(_Id^ Id)const { return ContainsInternal(Id); }
	inline IteratorReadWrite Find(_Id^ Id) { return IteratorReadWrite(this, Id); }
	inline IteratorReadOnly Find(_Id^ Id)const { return IteratorReadOnly(this, Id); }

	// Modification
	inline bool Add(_Id^ Id) { return AddInternal(Id); }
	inline bool Remove(_Id^ Id) { return RemoveInternal(Id); }
};

// Handle-Index with Pointer-Items
template<class _Id, class _Item, unsigned int _GroupSize>
class Index<_Id^, _Item*, _GroupSize>: public IndexBase<_Id^, _Item*, _GroupSize>
{
public:
	// Access
	inline _Item* operator[](_Id^ Id)const { return GetInternalPointer<_Item*, _Id^>(Id); }
	inline bool Contains(_Id^ Id)const { return ContainsInternal(Id); }
	inline IteratorReadWrite Find(_Id^ Id) { return IteratorReadWrite(this, Id); }
	inline IteratorReadOnly Find(_Id^ Id)const { return IteratorReadOnly(this, Id); }
	inline _Item* Get(_Id^ Id)const { return GetInternalPointer<_Item*, _Id^>(Id); }

	// Modification
	inline bool Add(_Id^ Id, _Item* Item) { return AddInternal(Id, Item); }
	inline bool Remove(_Id^ Id) { return RemoveInternal(Id); }
	inline void Set(_Id^ Id, _Item* Item) { SetInternal(Id, Item); }
};

// Handle-Index with Handle-Items
template<class _Id, class _Item, unsigned int _GroupSize>
class Index<_Id^, _Item^, _GroupSize>: public IndexBase<_Id^, _Item^, _GroupSize>
{
public:
	// Access
	inline _Item^ operator[](_Id^ Id)const { return GetInternalPointer<_Item^, _Id^>(Id); }
	inline bool Contains(_Id^ Id)const { return ContainsInternal(Id); }
	inline IteratorReadWrite Find(_Id^ Id) { return IteratorReadWrite(this, Id); }
	inline IteratorReadOnly Find(_Id^ Id)const { return IteratorReadOnly(this, Id); }
	inline _Item^ Get(_Id^ Id)const { return GetInternalPointer<_Item^, _Id^>(Id); }

	// Modification
	inline bool Add(_Id^ Id, _Item^ Item) { return AddInternal(Id, Item); }
	inline bool Remove(_Id^ Id) { return RemoveInternal(Id); }
	inline void Set(_Id^ Id, _Item^ Item) { SetInternal(Id, Item); }
};

// Handle-Index with String-Items
template<class _Id, class _Char, unsigned int _GroupSize>
class Index<_Id^, String<_Char, true>, _GroupSize>: public IndexBase<_Id^, String<_Char, true>, _GroupSize>
{
public:
	// Access
	inline _Char const* operator[](_Id^ Id)const { return GetInternalPointer<_Char const*, _Id^>(Id); }
	inline bool Contains(_Id^ Id)const { return ContainsInternal(Id); }
	inline IteratorReadWrite Find(_Id^ Id) { return IteratorReadWrite(this, Id); }
	inline IteratorReadOnly Find(_Id^ Id)const { return IteratorReadOnly(this, Id); }
	inline _Char const* Get(_Id^ Id)const { return GetInternalPointer<_Char const*, _Id^>(Id); }

	// Modification
	inline bool Add(_Id^ Id, _Char const* Item, unsigned int Length=0) { return AddInternal(Id, Item, Length); }
	inline bool Remove(_Id^ Id) { return RemoveInternal(Id); }
	inline void Set(_Id^ Id, _Char const* Item, unsigned int Length=0) { SetInternal(Id, Item, Length); }
};

// Handle-Index with Shared String-Items
template<class _Id, class _Char, unsigned int _GroupSize>
class Index<_Id^, String<_Char, false>, _GroupSize>: public IndexBase<_Id^, String<_Char, false>, _GroupSize>
{
public:
	// Access
	inline _Char const* operator[](_Id^ Id)const { return GetInternalPointer<_Char const*, _Id^>(Id); }
	inline bool Contains(_Id^ Id)const { return ContainsInternal(Id); }
	inline IteratorReadWrite Find(_Id^ Id) { return IteratorReadWrite(this, Id); }
	inline IteratorReadOnly Find(_Id^ Id)const { return IteratorReadOnly(this, Id); }
	inline _Char const* Get(_Id^ Id)const { return GetInternalPointer<_Char const*, _Id^>(Id); }

	// Modification
	inline bool Add(_Id^ Id, _Char const* Item) { return AddInternal(Id, Item); }
	inline bool Remove(_Id^ Id) { return RemoveInternal(Id); }
	inline void Set(_Id^ Id, _Char const* Item) { SetInternal(Id, Item); }
};

#endif


//============
// Ansi-Index
//============

template <class _Item, unsigned int _GroupSize> class Index<char*, _Item, _GroupSize>: public StringIndex<char, true, _Item, _GroupSize> {}; // Ansi-Index
template <unsigned int _GroupSize> class Index<char*, void, _GroupSize>: public StringIndex<char, true, void, _GroupSize> {}; // Ansi-Index without Items
template <class _Item, unsigned int _GroupSize> class Index<char*, _Item*, _GroupSize>: public StringIndex<char, true, _Item*, _GroupSize> {}; // Ansi-Index with Pointer-Items
#ifdef __cplusplus_winrt
template <class _Item, unsigned int _GroupSize> class Index<char*, _Item^, _GroupSize>: public StringIndex<char, true, _Item^, _GroupSize> {}; // Ansi-Index with Handle-Items
#endif
template <unsigned int _GroupSize> class Index<char*, char*, _GroupSize>: public StringIndex<char, true, String<char, true>, _GroupSize> {}; // Ansi-Index with Ansi-Items
template <unsigned int _GroupSize> class Index<char*, char const*, _GroupSize>: public StringIndex<char, true, String<char, false>, _GroupSize> {}; // Ansi-Index with Shared Ansi-Items
template <unsigned int _GroupSize> class Index<char*, wchar_t*, _GroupSize>: public StringIndex<char, true, String<wchar_t, true>, _GroupSize> {}; // Ansi-Index with Unicode-Items
template <unsigned int _GroupSize> class Index<char*, wchar_t const*, _GroupSize>: public StringIndex<char, true, String<wchar_t, false>, _GroupSize> {}; // Ansi-Index with Shared Unicode-Items


//===================
// Shared Ansi-Index
//===================

template <class _Item, unsigned int _GroupSize> class Index<char const*, _Item, _GroupSize>: public StringIndex<char, false, _Item, _GroupSize> {}; // Shared Ansi-Index
template <unsigned int _GroupSize> class Index<char const*, void, _GroupSize>: public StringIndex<char, false, void, _GroupSize> {}; // Shared Ansi-Index without Items
template <class _Item, unsigned int _GroupSize> class Index<char const*, _Item*, _GroupSize>: public StringIndex<char, false, _Item*, _GroupSize> {}; // Shared Ansi-Index with Pointer-Items
#ifdef __cplusplus_winrt
template <class _Item, unsigned int _GroupSize> class Index<char const*, _Item^, _GroupSize>: public StringIndex<char, false, _Item^, _GroupSize> {}; // Shared Ansi-Index with Handle-Items
#endif
template <unsigned int _GroupSize> class Index<char const*, char*, _GroupSize>: public StringIndex<char, false, String<char, true>, _GroupSize> {}; // Shared Ansi-Index with Ansi-Items
template <unsigned int _GroupSize> class Index<char const*, char const*, _GroupSize>: public StringIndex<char, false, String<char, false>, _GroupSize> {}; // Shared Ansi-Index with Shared Ansi-Items
template <unsigned int _GroupSize> class Index<char const*, wchar_t*, _GroupSize>: public StringIndex<char, false, String<wchar_t, true>, _GroupSize> {}; // Shared Ansi-Index with Unicode-Items
template <unsigned int _GroupSize> class Index<char const*, wchar_t const*, _GroupSize>: public StringIndex<char, false, String<wchar_t, false>, _GroupSize> {}; // Shared Ansi-Index with Shared Unicode-Items


//===============
// Unicode-Index
//===============

template <class _Item, unsigned int _GroupSize> class Index<wchar_t*, _Item, _GroupSize>: public StringIndex<wchar_t, true, _Item, _GroupSize> {}; // Unicode-Index
template <unsigned int _GroupSize> class Index<wchar_t*, void, _GroupSize>: public StringIndex<wchar_t, true, void, _GroupSize> {}; // Unicode-Index without Items
template <class _Item, unsigned int _GroupSize> class Index<wchar_t*, _Item*, _GroupSize>: public StringIndex<wchar_t, true, _Item*, _GroupSize> {}; // Unicode-Index with Pointer-Items
#ifdef __cplusplus_winrt
template <class _Item, unsigned int _GroupSize> class Index<wchar_t*, _Item^, _GroupSize>: public StringIndex<wchar_t, true, _Item^, _GroupSize> {}; // Unicode-Index with Handle-Items
#endif
template <unsigned int _GroupSize> class Index<wchar_t*, char*, _GroupSize>: public StringIndex<wchar_t, true, String<char, true>, _GroupSize> {}; // Unicode-Index with Ansi-Items
template <unsigned int _GroupSize> class Index<wchar_t*, char const*, _GroupSize>: public StringIndex<wchar_t, true, String<char, false>, _GroupSize> {}; // Unicode-Index with Shared Ansi-Items
template <unsigned int _GroupSize> class Index<wchar_t*, wchar_t*, _GroupSize>: public StringIndex<wchar_t, true, String<wchar_t, true>, _GroupSize> {}; // Unicode-Index with Unicode-Items
template <unsigned int _GroupSize> class Index<wchar_t*, wchar_t const*, _GroupSize>: public StringIndex<wchar_t, true, String<wchar_t, false>, _GroupSize> {}; // Unicode-Index with Shared Unicode-Items


//======================
// Shared Unicode-Index
//======================

template <class _Item, unsigned int _GroupSize> class Index<wchar_t const*, _Item, _GroupSize>: public StringIndex<wchar_t, false, _Item, _GroupSize> {}; // Shared Unicode-Index
template <unsigned int _GroupSize> class Index<wchar_t const*, void, _GroupSize>: public StringIndex<wchar_t, false, void, _GroupSize> {}; // Shared Unicode-Index without Items
template <class _Item, unsigned int _GroupSize> class Index<wchar_t const*, _Item*, _GroupSize>: public StringIndex<wchar_t, false, _Item*, _GroupSize> {}; // Shared Unicode-Index with Pointer-Items
#ifdef __cplusplus_winrt
template <class _Item, unsigned int _GroupSize> class Index<wchar_t const*, _Item^, _GroupSize>: public StringIndex<wchar_t, false, _Item^, _GroupSize> {}; // Shared Unicode-Index with Handle-Items
#endif
template <unsigned int _GroupSize> class Index<wchar_t const*, char*, _GroupSize>: public StringIndex<wchar_t, false, String<char, true>, _GroupSize> {}; // Shared Unicode-Index with Ansi-Items
template <unsigned int _GroupSize> class Index<wchar_t const*, char const*, _GroupSize>: public StringIndex<wchar_t, false, String<char, false>, _GroupSize> {}; // Shared Unicode-Index with Shared Ansi-Items
template <unsigned int _GroupSize> class Index<wchar_t const*, wchar_t*, _GroupSize>: public StringIndex<wchar_t, false, String<wchar_t, true>, _GroupSize> {}; // Shared Unicode-Index with Unicode-Items
template <unsigned int _GroupSize> class Index<wchar_t const*, wchar_t const*, _GroupSize>: public StringIndex<wchar_t, false, String<wchar_t, false>, _GroupSize> {}; // Shared Unicode-Index with Shared Unicode-Items

}}}}
