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
template<class _Id, class _Item, UINT _GroupSize>
class Index: public IndexBase<_Id, _Item, _GroupSize>
{
private:
	// Using
	using _IndexBase=IndexBase<_Id, _Item, _GroupSize>;
	using _ItR=IndexIterator<_Id, _Item, _GroupSize, true>;
	using _ItW=IndexIterator<_Id, _Item, _GroupSize, false>;

public:
	// Access
	inline _Item operator[](_Id const& Id)const { return this->template GetInternal<_Item const&, _Id const&>(Id); }
	inline BOOL Contains(_Id const& Id)const { return this->template ContainsInternal<_Id const&>(Id); }
	inline _ItW Find(_Id const& Id) { return _ItW(this, 0, Id); }
	inline _ItR Find(_Id const& Id)const { return _ItR(this, 0, Id); }
	inline _Item Get(_Id const& Id)const { return this->template GetInternal<_Item const&, _Id const&>(Id); }
	inline BOOL TryGet(_Id const& Id, _Item* Item)const { return this->template TryGetInternal<_Item, _Id const&>(Id, Item); }

	// Modification
	inline BOOL Add(_Id const& Id, _Item const& Item) { return this->template AddInternal<_Id const&, _Item const&>(Id, Item); }
	inline BOOL Remove(_Id const& Id) { return this->template RemoveInternal<_Id const&>(Id); }
	inline VOID Set(_Id const& Id, _Item const& Item) { this->template SetInternal<_Id const&, _Item const&>(Id, Item); }
};

// Index without Items
template<class _Id, UINT _GroupSize>
class Index<_Id, VOID, _GroupSize>: public IndexBase<_Id, VOID, _GroupSize>
{
private:
	// Using
	using _IndexBase=IndexBase<_Id, VOID, _GroupSize>;
	using _ItR=IndexIterator<_Id, VOID, _GroupSize, true>;
	using _ItW=IndexIterator<_Id, VOID, _GroupSize, false>;

public:
	// Access
	inline BOOL Contains(_Id const& Id)const { return this->template ContainsInternal<_Id const&>(Id); }
	inline _ItW Find(_Id const& Id) { return _ItW(this, 0, Id); }
	inline _ItR Find(_Id const& Id)const { return _ItR(this, 0, Id); }

	// Modification
	inline BOOL Add(_Id const& Id) { return this->template AddInternal<_Id const&>(Id); }
	inline BOOL Remove(_Id const& Id) { return this->template RemoveInternal<_Id const&>(Id); }
};

// Index with Pointer-Items
template<class _Id, class _Item, UINT _GroupSize>
class Index<_Id, _Item*, _GroupSize>: public IndexBase<_Id, _Item*, _GroupSize>
{
private:
	// Using
	using _IndexBase=IndexBase<_Id, _Item*, _GroupSize>;
	using _ItR=IndexIterator<_Id, _Item*, _GroupSize, true>;
	using _ItW=IndexIterator<_Id, _Item*, _GroupSize, false>;

public:
	// Access
	inline _Item* operator[](_Id const& Id)const { return this->template GetInternalPointer<_Item*, _Id const&>(Id); }
	inline BOOL Contains(_Id const& Id)const { return this->template ContainsInternal<_Id const&>(Id); }
	inline _ItW Find(_Id const& Id) { return _ItW(this, 0, Id); }
	inline _ItR Find(_Id const& Id)const { return _ItR(this, 0, Id); }
	_Item* Get(_Id const& Id)const { return this->template GetInternalPointer<_Item*, _Id const&>(Id); }

	// Modification
	inline BOOL Add(_Id const& Id, _Item* Item) { return this->template AddInternal<_Id const&, _Item*>(Id, Item); }
	inline BOOL Remove(_Id const& Id) { return this->template RemoveInternal<_Id const&>(Id); }
	inline VOID Set(_Id const& Id, _Item* Item) { this->template SetInternal<_Id const&, _Item*>(Id, Item); }
};

#ifdef __cplusplus_winrt
// Index with Handle-Items
template<class _Id, class _Item, UINT _GroupSize>
class Index<_Id, _Item^, _GroupSize>: public IndexBase<_Id, _Item^, _GroupSize>
{
private:
	// Using
	using _IndexBase=IndexBase<_Id, _Item^, _GroupSize>;
	using _ItR=IndexIterator<_Id, _Item^, _GroupSize, true>;
	using _ItW=IndexIterator<_Id, _Item^, _GroupSize, false>;

public:
	// Access
	inline _Item^ operator[](_Id const& Id)const { return this->template GetInternalPointer<_Item^, _Id const&>(Id); }
	inline BOOL Contains(_Id const& Id)const { return this->template ContainsInternal<_Id const&>(Id); }
	inline _ItW Find(_Id const& Id) { return _ItW(this, 0, Id); }
	inline _ItR Find(_Id const& Id)const { return _ItR(this, 0, Id); }
	_Item^ Get(_Id const& Id)const { return this->template GetInternalPointer<_Item^, _Id const&>(Id); }

	// Modification
	inline BOOL Add(_Id const& Id, _Item^ Item) { return this->template AddInternal<_Id const&, _Item^>(Id, Item); }
	inline BOOL Remove(_Id const& Id) { return this->template RemoveInternal<_Id const&>(Id); }
	inline VOID Set(_Id const& Id, _Item^ Item) { this->template SetInternal<_Id const&, _Item^>(Id, Item); }
};
#endif

// Index with String-Items
template<class _Id, class _Char, UINT _GroupSize>
class Index<_Id, StringItem<_Char, true>, _GroupSize>: public IndexBase<_Id, StringItem<_Char, true>, _GroupSize>
{
private:
	// Using
	using _IndexBase=IndexBase<_Id, StringItem<_Char, true>, _GroupSize>;
	using _ItR=IndexIterator<_Id, StringItem<_Char, true>, _GroupSize, true>;
	using _ItW=IndexIterator<_Id, StringItem<_Char, true>, _GroupSize, false>;

public:
	// Access
	inline _Char const* operator[](_Id const& Id)const { return this->template GetInternalPointer<_Char const*, _Id const&>(Id); }
	inline BOOL Contains(_Id const& Id)const { return this->template ContainsInternal<_Id const&>(Id); }
	inline _ItW Find(_Id const& Id) { return _ItW(this, 0, Id); }
	inline _ItR Find(_Id const& Id)const { return _ItR(this, 0, Id); }
	inline _Char const* Get(_Id const& Id)const { return this->template GetInternalPointer<_Char const*, _Id const&>(Id); }

	// Modification
	inline BOOL Add(_Id const& Id, _Char const* Item, UINT Length=0) { return this->template AddInternal<_Id const&, _Char const*, UINT>(Id, Item, Length); }
	inline BOOL Remove(_Id const& Id) { return this->template RemoveInternal<_Id const&>(Id); }
	inline VOID Set(_Id const& Id, _Char const* Item, UINT Length=0) { this->template SetInternal<_Id const&, _Char const*, UINT>(Id, Item, Length); }
};

// Index with Shared String-Items
template<class _Id, class _Char, UINT _GroupSize>
class Index<_Id, StringItem<_Char, false>, _GroupSize>: public IndexBase<_Id, StringItem<_Char, false>, _GroupSize>
{
private:
	// Using
	using _IndexBase=IndexBase<_Id, StringItem<_Char, false>, _GroupSize>;
	using _ItR=IndexIterator<_Id, StringItem<_Char, false>, _GroupSize, true>;
	using _ItW=IndexIterator<_Id, StringItem<_Char, false>, _GroupSize, false>;

public:
	// Access
	inline _Char const* operator[](_Id const& Id)const { return this->template GetInternalPointer<_Char const*, _Id const&>(Id); }
	inline BOOL Contains(_Id const& Id)const { return this->template ContainsInternal<_Id const&>(Id); }
	inline _ItW Find(_Id const& Id) { return _ItW(this, 0, Id); }
	inline _ItR Find(_Id const& Id)const { return _ItR(this, 0, Id); }
	inline _Char const* Get(_Id const& Id)const { return this->template GetInternalPointer<_Char const*, _Id const&>(Id); }

	// Modification
	inline BOOL Add(_Id const& Id, _Char const* Item) { return this->template AddInternal<_Id const&, _Char const*>(Id, Item); }
	inline BOOL Remove(_Id const& Id) { return this->template RemoveInternal<_Id const&>(Id); }
	inline VOID Set(_Id const& Id, _Char const* Item) { this->template SetInternal<_Id const&, _Char const*>(Id, Item); }
};


//===============
// Pointer-Index
//===============

// Pointer-Index
template<class _Id, class _Item, UINT _GroupSize>
class Index<_Id*, _Item, _GroupSize>: public IndexBase<_Id*, _Item, _GroupSize>
{
private:
	// Using
	using _IndexBase=IndexBase<_Id*, _Item, _GroupSize>;
	using _ItR=IndexIterator<_Id*, _Item, _GroupSize, true>;
	using _ItW=IndexIterator<_Id*, _Item, _GroupSize, false>;

public:
	// Access
	inline _Item operator[](_Id* Id)const { return this->template GetInternal<_Item const&, _Id*>(Id); }
	inline BOOL Contains(_Id* Id)const { return this->ContainsInternal(Id); }
	inline _ItW Find(_Id* Id) { return _ItW(this, Id); }
	inline _ItR Find(_Id* Id)const { return _ItR(this, Id); }
	inline _Item Get(_Id* Id)const { return this->template GetInternal<_Item const&, _Id*>(Id); }
	inline BOOL TryGet(_Id* Id, _Item* Item)const { return this->template TryGetInternal<_Item, _Id*>(Id, Item); }

	// Modification
	inline BOOL Add(_Id* Id, _Item const& Item) { return this->template AddInternal<_Id*, _Item const&>(Id, Item); }
	inline BOOL Remove(_Id* Id) { return this->RemoveInternal(Id); }
	inline VOID Set(_Id* Id, _Item const& Item) { this->template SetInternal<_Id*, _Item const&>(Id, Item); }
};

// Pointer-Index without Items
template<class _Id, UINT _GroupSize>
class Index<_Id*, VOID, _GroupSize>: public IndexBase<_Id*, VOID, _GroupSize>
{
private:
	// Using
	using _IndexBase=IndexBase<_Id*, VOID, _GroupSize>;
	using _ItR=IndexIterator<_Id*, VOID, _GroupSize, true>;
	using _ItW=IndexIterator<_Id*, VOID, _GroupSize, false>;

public:
	// Access
	inline BOOL Contains(_Id* Id)const { return this->ContainsInternal(Id); }
	inline _ItW Find(_Id* Id) { return _ItW(this, Id); }
	inline _ItR Find(_Id* Id)const { return _ItR(this, Id); }

	// Modification
	inline BOOL Add(_Id* Id) { return this->AddInternal(Id); }
	inline BOOL Remove(_Id* Id) { return this->RemoveInternal(Id); }
};

// Pointer-Index with Pointer-Items
template<class _Id, class _Item, UINT _GroupSize>
class Index<_Id*, _Item*, _GroupSize>: public IndexBase<_Id*, _Item*, _GroupSize>
{
private:
	// Using
	using _IndexBase=IndexBase<_Id*, _Item*, _GroupSize>;
	using _ItR=IndexIterator<_Id*, _Item*, _GroupSize, true>;
	using _ItW=IndexIterator<_Id*, _Item*, _GroupSize, false>;

public:
	// Access
	inline _Item* operator[](_Id* Id)const { return this->template GetInternalPointer<_Item*, _Id*>(Id); }
	inline BOOL Contains(_Id* Id)const { return this->ContainsInternal(Id); }
	inline _ItW Find(_Id* Id) { return _ItW(this, Id); }
	inline _ItR Find(_Id* Id)const { return _ItR(this, Id); }
	inline _Item* Get(_Id* Id)const { return this->template GetInternalPointer<_Item*, _Id*>(Id); }

	// Modification
	inline BOOL Add(_Id* Id, _Item* Item) { return this->AddInternal(Id, Item); }
	inline BOOL Remove(_Id* Id) { return this->RemoveInternal(Id); }
	inline VOID Set(_Id* Id, _Item* Item) { this->SetInternal(Id, Item); }
};

#ifdef __cplusplus_winrt
// Pointer-Index with Handle-Items
template<class _Id, class _Item, UINT _GroupSize>
class Index<_Id*, _Item^, _GroupSize>: public IndexBase<_Id*, _Item^, _GroupSize>
{
private:
	// Using
	using _IndexBase=IndexBase<_Id*, _Item^, _GroupSize>;
	using _ItR=IndexIterator<_Id*, _Item^, _GroupSize, true>;
	using _ItW=IndexIterator<_Id*, _Item^, _GroupSize, false>;

public:
	// Access
	inline _Item^ operator[](_Id* Id)const { return this->template GetInternalPointer<_Item^, _Id*>(Id); }
	inline BOOL Contains(_Id* Id)const { return this->ContainsInternal(Id); }
	inline _ItW Find(_Id* Id) { return _ItW(this, Id); }
	inline _ItR Find(_Id* Id)const { return _ItR(this, Id); }
	inline _Item^ Get(_Id* Id)const { return this->template GetInternalPointer<_Item^, _Id*>(Id); }

	// Modification
	inline BOOL Add(_Id* Id, _Item^ Item) { return this->AddInternal(Id, Item); }
	inline BOOL Remove(_Id* Id) { return this->RemoveInternal(Id); }
	inline VOID Set(_Id* Id, _Item^ Item) { this->SetInternal(Id, Item); }
};
#endif

// Pointer-Index with String-Items
template<class _Id, class _Char, UINT _GroupSize>
class Index<_Id*, StringItem<_Char, true>, _GroupSize>: public IndexBase<_Id*, StringItem<_Char, true>, _GroupSize>
{
private:
	// Using
	using _IndexBase=IndexBase<_Id*, StringItem<_Char, true>, _GroupSize>;
	using _ItR=IndexIterator<_Id*, StringItem<_Char, true>, _GroupSize, true>;
	using _ItW=IndexIterator<_Id*, StringItem<_Char, true>, _GroupSize, false>;

public:
	// Access
	inline _Char const* operator[](_Id* Id)const { return this->template GetInternalPointer<_Char const*, _Id*>(Id); }
	inline BOOL Contains(_Id* Id)const { return this->ContainsInternal(Id); }
	inline _ItW Find(_Id* Id) { return _ItW(this, Id); }
	inline _ItR Find(_Id* Id)const { return _ItR(this, Id); }
	inline _Char const* Get(_Id* Id)const { return this->template GetInternalPointer<_Char const*, _Id*>(Id); }

	// Modification
	inline BOOL Add(_Id* Id, _Char const* Item, UINT Length=0) { return this->AddInternal(Id, Item, Length); }
	inline BOOL Remove(_Id* Id) { return this->RemoveInternal(Id); }
	inline VOID Set(_Id* Id, _Char const* Item, UINT Length=0) { this->SetInternal(Id, Item, Length); }
};

// Pointer-Index with Shared String-Items
template<class _Id, class _Char, UINT _GroupSize>
class Index<_Id*, StringItem<_Char, false>, _GroupSize>: public IndexBase<_Id*, StringItem<_Char, false>, _GroupSize>
{
private:
	// Using
	using _IndexBase=IndexBase<_Id*, StringItem<_Char, false>, _GroupSize>;
	using _ItR=IndexIterator<_Id*, StringItem<_Char, false>, _GroupSize, true>;
	using _ItW=IndexIterator<_Id*, StringItem<_Char, false>, _GroupSize, false>;

public:
	// Access
	inline _Char const* operator[](_Id* Id)const { return this->template GetInternalPointer<_Char const*, _Id*>(Id); }
	inline BOOL Contains(_Id* Id)const { return this->ContainsInternal(Id); }
	inline _ItW Find(_Id* Id) { return _ItW(this, Id); }
	inline _ItR Find(_Id* Id)const { return _ItR(this, Id); }
	inline _Char const* Get(_Id* Id)const { return this->template GetInternalPointer<_Char const*, _Id*>(Id); }

	// Modification
	inline BOOL Add(_Id* Id, _Char const* Item) { return this->AddInternal(Id, Item); }
	inline BOOL Remove(_Id* Id) { return this->RemoveInternal(Id); }
	inline VOID Set(_Id* Id, _Char const* Item) { this->SetInternal(Id, Item); }
};


//==============
// Handle-Index
//==============

#ifdef __cplusplus_winrt

// Handle-Index
template<class _Id, class _Item, UINT _GroupSize>
class Index<_Id^, _Item, _GroupSize>: public IndexBase<_Id^, _Item, _GroupSize>
{
private:
	// Using
	using _IndexBase=IndexBase<_Id^, _Item, _GroupSize>;
	using _ItR=IndexIterator<_Id^, _Item, _GroupSize, true>;
	using _ItW=IndexIterator<_Id^, _Item, _GroupSize, false>;

public:
	// Access
	inline _Item operator[](_Id^ Id)const { return this->template GetInternal<_Item const&, _Id^>(Id); }
	inline BOOL Contains(_Id^ Id)const { return this->ContainsInternal(Id); }
	inline _ItW Find(_Id^ Id) { return _ItW(this, Id); }
	inline _ItR Find(_Id^ Id)const { return _ItR(this, Id); }
	inline _Item Get(_Id^ Id)const { return this->template GetInternal<_Item const&, _Id^>(Id); }
	inline BOOL TryGet(_Id^ Id, _Item* Item)const { return this->template TryGetInternal<_Item, _Id^>(Id, Item); }

	// Modification
	inline BOOL Add(_Id^ Id, _Item const& Item) { return this->template AddInternal<_Id^, _Item const&>(Id, Item); }
	inline BOOL Remove(_Id^ Id) { return this->RemoveInternal(Id); }
	inline VOID Set(_Id^ Id, _Item const& Item) { this->template SetInternal<_Id^, _Item const&>(Id, Item); }
};

// Handle-Index without Items
template<class _Id, UINT _GroupSize>
class Index<_Id^, VOID, _GroupSize>: public IndexBase<_Id^, VOID, _GroupSize>
{
private:
	// Using
	using _IndexBase=IndexBase<_Id^, VOID, _GroupSize>;
	using _ItR=IndexIterator<_Id^, VOID, _GroupSize, true>;
	using _ItW=IndexIterator<_Id^, VOID, _GroupSize, false>;

public:
	// Access
	inline BOOL Contains(_Id^ Id)const { return this->ContainsInternal(Id); }
	inline _ItW Find(_Id^ Id) { return _ItW(this, Id); }
	inline _ItR Find(_Id^ Id)const { return _ItR(this, Id); }

	// Modification
	inline BOOL Add(_Id^ Id) { return this->AddInternal(Id); }
	inline BOOL Remove(_Id^ Id) { return this->RemoveInternal(Id); }
};

// Handle-Index with Pointer-Items
template<class _Id, class _Item, UINT _GroupSize>
class Index<_Id^, _Item*, _GroupSize>: public IndexBase<_Id^, _Item*, _GroupSize>
{
private:
	// Using
	using _IndexBase=IndexBase<_Id^, _Item*, _GroupSize>;
	using _ItR=IndexIterator<_Id^, _Item*, _GroupSize, true>;
	using _ItW=IndexIterator<_Id^, _Item*, _GroupSize, false>;

public:
	// Access
	inline _Item* operator[](_Id^ Id)const { return this->template GetInternalPointer<_Item*, _Id^>(Id); }
	inline BOOL Contains(_Id^ Id)const { return this->ContainsInternal(Id); }
	inline _ItW Find(_Id^ Id) { return _ItW(this, Id); }
	inline _ItR Find(_Id^ Id)const { return _ItR(this, Id); }
	inline _Item* Get(_Id^ Id)const { return this->template GetInternalPointer<_Item*, _Id^>(Id); }

	// Modification
	inline BOOL Add(_Id^ Id, _Item* Item) { return this->AddInternal(Id, Item); }
	inline BOOL Remove(_Id^ Id) { return this->RemoveInternal(Id); }
	inline VOID Set(_Id^ Id, _Item* Item) { this->SetInternal(Id, Item); }
};

// Handle-Index with Handle-Items
template<class _Id, class _Item, UINT _GroupSize>
class Index<_Id^, _Item^, _GroupSize>: public IndexBase<_Id^, _Item^, _GroupSize>
{
private:
	// Using
	using _IndexBase=IndexBase<_Id^, _Item^, _GroupSize>;
	using _ItR=IndexIterator<_Id^, _Item^, _GroupSize, true>;
	using _ItW=IndexIterator<_Id^, _Item^, _GroupSize, false>;

public:
	// Access
	inline _Item^ operator[](_Id^ Id)const { return this->template GetInternalPointer<_Item^, _Id^>(Id); }
	inline BOOL Contains(_Id^ Id)const { return this->ContainsInternal(Id); }
	inline _ItW Find(_Id^ Id) { return _ItW(this, Id); }
	inline _ItR Find(_Id^ Id)const { return _ItR(this, Id); }
	inline _Item^ Get(_Id^ Id)const { return this->template GetInternalPointer<_Item^, _Id^>(Id); }

	// Modification
	inline BOOL Add(_Id^ Id, _Item^ Item) { return this->AddInternal(Id, Item); }
	inline BOOL Remove(_Id^ Id) { return this->RemoveInternal(Id); }
	inline VOID Set(_Id^ Id, _Item^ Item) { this->SetInternal(Id, Item); }
};

// Handle-Index with String-Items
template<class _Id, class _Char, UINT _GroupSize>
class Index<_Id^, StringItem<_Char, true>, _GroupSize>: public IndexBase<_Id^, StringItem<_Char, true>, _GroupSize>
{
private:
	// Using
	using _IndexBase=IndexBase<_Id^, StringItem<_Char, true>, _GroupSize>;
	using _ItR=IndexIterator<_Id^, StringItem<_Char, true>, _GroupSize, true>;
	using _ItW=IndexIterator<_Id^, StringItem<_Char, true>, _GroupSize, false>;

public:
	// Access
	inline _Char const* operator[](_Id^ Id)const { return this->template GetInternalPointer<_Char const*, _Id^>(Id); }
	inline BOOL Contains(_Id^ Id)const { return this->ContainsInternal(Id); }
	inline _ItW Find(_Id^ Id) { return _ItW(this, Id); }
	inline _ItR Find(_Id^ Id)const { return _ItR(this, Id); }
	inline _Char const* Get(_Id^ Id)const { return this->template GetInternalPointer<_Char const*, _Id^>(Id); }

	// Modification
	inline BOOL Add(_Id^ Id, _Char const* Item, UINT Length=0) { return this->AddInternal(Id, Item, Length); }
	inline BOOL Remove(_Id^ Id) { return this->RemoveInternal(Id); }
	inline VOID Set(_Id^ Id, _Char const* Item, UINT Length=0) { this->SetInternal(Id, Item, Length); }
};

// Handle-Index with Shared String-Items
template<class _Id, class _Char, UINT _GroupSize>
class Index<_Id^, StringItem<_Char, false>, _GroupSize>: public IndexBase<_Id^, StringItem<_Char, false>, _GroupSize>
{
private:
	// Using
	using _IndexBase=IndexBase<_Id^, StringItem<_Char, false>, _GroupSize>;
	using _ItR=IndexIterator<_Id^, StringItem<_Char, false>, _GroupSize, true>;
	using _ItW=IndexIterator<_Id^, StringItem<_Char, false>, _GroupSize, false>;

public:
	// Access
	inline _Char const* operator[](_Id^ Id)const { return this->template GetInternalPointer<_Char const*, _Id^>(Id); }
	inline BOOL Contains(_Id^ Id)const { return this->ContainsInternal(Id); }
	inline _ItW Find(_Id^ Id) { return _ItW(this, Id); }
	inline _ItR Find(_Id^ Id)const { return _ItR(this, Id); }
	inline _Char const* Get(_Id^ Id)const { return this->template GetInternalPointer<_Char const*, _Id^>(Id); }

	// Modification
	inline BOOL Add(_Id^ Id, _Char const* Item) { return this->AddInternal(Id, Item); }
	inline BOOL Remove(_Id^ Id) { return this->RemoveInternal(Id); }
	inline VOID Set(_Id^ Id, _Char const* Item) { this->SetInternal(Id, Item); }
};

#endif


//============
// Ansi-Index
//============

template <class _Item, UINT _GroupSize> class Index<char*, _Item, _GroupSize>: public StringIndex<char, true, _Item, _GroupSize> {}; // Ansi-Index
template <UINT _GroupSize> class Index<char*, VOID, _GroupSize>: public StringIndex<char, true, VOID, _GroupSize> {}; // Ansi-Index without Items
template <class _Item, UINT _GroupSize> class Index<char*, _Item*, _GroupSize>: public StringIndex<char, true, _Item*, _GroupSize> {}; // Ansi-Index with Pointer-Items
#ifdef __cplusplus_winrt
template <class _Item, UINT _GroupSize> class Index<char*, _Item^, _GroupSize>: public StringIndex<char, true, _Item^, _GroupSize> {}; // Ansi-Index with Handle-Items
#endif
template <UINT _GroupSize> class Index<char*, char*, _GroupSize>: public StringIndex<char, true, StringItem<char, true>, _GroupSize> {}; // Ansi-Index with Ansi-Items
template <UINT _GroupSize> class Index<char*, char const*, _GroupSize>: public StringIndex<char, true, StringItem<char, false>, _GroupSize> {}; // Ansi-Index with Shared Ansi-Items
template <UINT _GroupSize> class Index<char*, wchar_t*, _GroupSize>: public StringIndex<char, true, StringItem<wchar_t, true>, _GroupSize> {}; // Ansi-Index with Unicode-Items
template <UINT _GroupSize> class Index<char*, wchar_t const*, _GroupSize>: public StringIndex<char, true, StringItem<wchar_t, false>, _GroupSize> {}; // Ansi-Index with Shared Unicode-Items


//===================
// Shared Ansi-Index
//===================

template <class _Item, UINT _GroupSize> class Index<char const*, _Item, _GroupSize>: public StringIndex<char, false, _Item, _GroupSize> {}; // Shared Ansi-Index
template <UINT _GroupSize> class Index<char const*, VOID, _GroupSize>: public StringIndex<char, false, VOID, _GroupSize> {}; // Shared Ansi-Index without Items
template <class _Item, UINT _GroupSize> class Index<char const*, _Item*, _GroupSize>: public StringIndex<char, false, _Item*, _GroupSize> {}; // Shared Ansi-Index with Pointer-Items
#ifdef __cplusplus_winrt
template <class _Item, UINT _GroupSize> class Index<char const*, _Item^, _GroupSize>: public StringIndex<char, false, _Item^, _GroupSize> {}; // Shared Ansi-Index with Handle-Items
#endif
template <UINT _GroupSize> class Index<char const*, char*, _GroupSize>: public StringIndex<char, false, StringItem<char, true>, _GroupSize> {}; // Shared Ansi-Index with Ansi-Items
template <UINT _GroupSize> class Index<char const*, char const*, _GroupSize>: public StringIndex<char, false, StringItem<char, false>, _GroupSize> {}; // Shared Ansi-Index with Shared Ansi-Items
template <UINT _GroupSize> class Index<char const*, wchar_t*, _GroupSize>: public StringIndex<char, false, StringItem<wchar_t, true>, _GroupSize> {}; // Shared Ansi-Index with Unicode-Items
template <UINT _GroupSize> class Index<char const*, wchar_t const*, _GroupSize>: public StringIndex<char, false, StringItem<wchar_t, false>, _GroupSize> {}; // Shared Ansi-Index with Shared Unicode-Items


//===============
// Unicode-Index
//===============

template <class _Item, UINT _GroupSize> class Index<wchar_t*, _Item, _GroupSize>: public StringIndex<wchar_t, true, _Item, _GroupSize> {}; // Unicode-Index
template <UINT _GroupSize> class Index<wchar_t*, VOID, _GroupSize>: public StringIndex<wchar_t, true, VOID, _GroupSize> {}; // Unicode-Index without Items
template <class _Item, UINT _GroupSize> class Index<wchar_t*, _Item*, _GroupSize>: public StringIndex<wchar_t, true, _Item*, _GroupSize> {}; // Unicode-Index with Pointer-Items
#ifdef __cplusplus_winrt
template <class _Item, UINT _GroupSize> class Index<wchar_t*, _Item^, _GroupSize>: public StringIndex<wchar_t, true, _Item^, _GroupSize> {}; // Unicode-Index with Handle-Items
#endif
template <UINT _GroupSize> class Index<wchar_t*, char*, _GroupSize>: public StringIndex<wchar_t, true, StringItem<char, true>, _GroupSize> {}; // Unicode-Index with Ansi-Items
template <UINT _GroupSize> class Index<wchar_t*, char const*, _GroupSize>: public StringIndex<wchar_t, true, StringItem<char, false>, _GroupSize> {}; // Unicode-Index with Shared Ansi-Items
template <UINT _GroupSize> class Index<wchar_t*, wchar_t*, _GroupSize>: public StringIndex<wchar_t, true, StringItem<wchar_t, true>, _GroupSize> {}; // Unicode-Index with Unicode-Items
template <UINT _GroupSize> class Index<wchar_t*, wchar_t const*, _GroupSize>: public StringIndex<wchar_t, true, StringItem<wchar_t, false>, _GroupSize> {}; // Unicode-Index with Shared Unicode-Items


//======================
// Shared Unicode-Index
//======================

template <class _Item, UINT _GroupSize> class Index<wchar_t const*, _Item, _GroupSize>: public StringIndex<wchar_t, false, _Item, _GroupSize> {}; // Shared Unicode-Index
template <UINT _GroupSize> class Index<wchar_t const*, VOID, _GroupSize>: public StringIndex<wchar_t, false, VOID, _GroupSize> {}; // Shared Unicode-Index without Items
template <class _Item, UINT _GroupSize> class Index<wchar_t const*, _Item*, _GroupSize>: public StringIndex<wchar_t, false, _Item*, _GroupSize> {}; // Shared Unicode-Index with Pointer-Items
#ifdef __cplusplus_winrt
template <class _Item, UINT _GroupSize> class Index<wchar_t const*, _Item^, _GroupSize>: public StringIndex<wchar_t, false, _Item^, _GroupSize> {}; // Shared Unicode-Index with Handle-Items
#endif
template <UINT _GroupSize> class Index<wchar_t const*, char*, _GroupSize>: public StringIndex<wchar_t, false, StringItem<char, true>, _GroupSize> {}; // Shared Unicode-Index with Ansi-Items
template <UINT _GroupSize> class Index<wchar_t const*, char const*, _GroupSize>: public StringIndex<wchar_t, false, StringItem<char, false>, _GroupSize> {}; // Shared Unicode-Index with Shared Ansi-Items
template <UINT _GroupSize> class Index<wchar_t const*, wchar_t*, _GroupSize>: public StringIndex<wchar_t, false, StringItem<wchar_t, true>, _GroupSize> {}; // Shared Unicode-Index with Unicode-Items
template <UINT _GroupSize> class Index<wchar_t const*, wchar_t const*, _GroupSize>: public StringIndex<wchar_t, false, StringItem<wchar_t, false>, _GroupSize> {}; // Shared Unicode-Index with Shared Unicode-Items

}}}}
