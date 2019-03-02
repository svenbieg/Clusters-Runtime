//===============
// StringIndex.h
//===============

#pragma once


//=======
// Using
//=======

#include "../StringItem.h"
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
template<class _Char, BOOL _Alloc, class _Item, UINT _GroupSize>
class StringIndex: public IndexBase<StringItem<_Char, _Alloc>, _Item, _GroupSize>
{
private:
	// Using
	using _IndexBase=IndexBase<StringItem<_Char, _Alloc>, _Item, _GroupSize>;
	using _ItR=IndexIterator<StringItem<_Char, _Alloc>, _Item, _GroupSize, true>;
	using _ItW=IndexIterator<StringItem<_Char, _Alloc>, _Item, _GroupSize, false>;

public:
	// Access
	inline _Item operator[](_Char const* Id)const { return this->template GetInternal<_Item const&, _Char const*, UINT, BOOL>(Id, 0, false); }
	inline BOOL Contains(_Char const* Id, UINT Length=0, BOOL CaseSensitive=false)const { return this->ContainsInternal(Id, Length, CaseSensitive); }
	inline _ItW Find(_Char const* Id, UINT Length=0, BOOL CaseSensitive=false) { return _ItW(this, Id, Length, CaseSensitive); }
	inline _ItR Find(_Char const* Id, UINT Length=0, BOOL CaseSensitive=false)const { return _ItR(this, Id, Length, CaseSensitive); }
	inline _Item Get(_Char const* Id, UINT Length=0, BOOL CaseSensitive=false)const { return this->template GetInternal<_Item const&, _Char const*, UINT, BOOL>(Id, Length, CaseSensitive); }
	inline BOOL TryGet(_Char const* Id, _Item* Item, UINT Length=0, BOOL CaseSensitive=false)const { return this->template TryGetInternal<_Item, _Char const*, UINT, BOOL>(Id, Length, CaseSensitive, Item); }

	// Modification
	inline BOOL Add(_Char const* Id, _Item const& Item, UINT Length=0, BOOL CaseSensitive=false) { return this->template AddStringInternal<_Char, _Item const&>(Id, Length, CaseSensitive, Item); }
	inline BOOL Remove(_Char const* Id, UINT Length=0, BOOL CaseSensitive=false) { return this->RemoveInternal(Id, Length, CaseSensitive); }
	inline VOID Set(_Char const* Id, _Item const& Item, UINT Length=0, BOOL CaseSensitive=false) { this->template SetStringInternal<_Char, _Item const&>(Id, Length, CaseSensitive, Item); }
};

// String-Index without Items
template <class _Char, UINT _GroupSize>
class StringIndex<_Char, true, VOID, _GroupSize>: public IndexBase<StringItem<_Char, true>, VOID, _GroupSize>
{
private:
	// Using
	using _IndexBase=IndexBase<StringItem<_Char, true>, VOID, _GroupSize>;
	using _ItR=IndexIterator<StringItem<_Char, true>, VOID, _GroupSize, true>;
	using _ItW=IndexIterator<StringItem<_Char, true>, VOID, _GroupSize, false>;

public:
	// Access
	inline BOOL Contains(_Char const* Id, UINT Length=0, BOOL CaseSensitive=false)const { return this->ContainsInternal(Id, Length, CaseSensitive); }
	inline _ItW Find(_Char const* Id, UINT Length=0, BOOL CaseSensitive=false) { return _ItW(this, Id, Length, CaseSensitive); }
	inline _ItR Find(_Char const* Id, UINT Length=0, BOOL CaseSensitive=false)const { return _ItR(this, Id, Length, CaseSensitive); }

	// Modification
	inline BOOL Add(_Char const* Id, UINT Length=0, BOOL CaseSensitive=false) { return this->AddStringInternal(Id, Length, CaseSensitive); }
	inline BOOL Remove(_Char const* Id, UINT Length=0, BOOL CaseSensitive=false) { return this->RemoveInternal(Id, Length, CaseSensitive); }
};

// String-Index with Pointers
template<class _Char, class _Item, UINT _GroupSize>
class StringIndex<_Char, true, _Item*, _GroupSize>: public IndexBase<StringItem<_Char, true>, _Item*, _GroupSize>
{
private:
	// Using
	using _IndexBase=IndexBase<StringItem<_Char, true>, _Item*, _GroupSize>;
	using _ItR=IndexIterator<StringItem<_Char, true>, _Item*, _GroupSize, true>;
	using _ItW=IndexIterator<StringItem<_Char, true>, _Item*, _GroupSize, false>;

public:
	// Access
	inline _Item* operator[](_Char const* Id)const { return this->template GetInternalPointer<_Item*, _Char const*>(Id); }
	inline BOOL Contains(_Char const* Id, UINT Length=0, BOOL CaseSensitive=false)const { return this->ContainsInternal(Id, Length, CaseSensitive); }
	inline _ItW Find(_Char const* Id, UINT Length=0, BOOL CaseSensitive=false) { return _ItW(this, Id, Length, CaseSensitive); }
	inline _ItR Find(_Char const* Id, UINT Length=0, BOOL CaseSensitive=false)const { return _ItR(this, Id, Length, CaseSensitive); }
	inline _Item* Get(_Char const* Id, UINT Length=0, BOOL CaseSensitive=false)const { return this->template GetInternalPointer<_Item*, _Char const*, UINT, BOOL>(Id, Length, CaseSensitive); }

	// Modification
	inline BOOL Add(_Char const* Id, _Item* Item, UINT Length=0, BOOL CaseSensitive=false) { return this->AddStringInternal(Id, Length, CaseSensitive, Item); }
	inline BOOL Remove(_Char const* Id, UINT Length=0, BOOL CaseSensitive=false) { return this->RemoveInternal(Id, Length, CaseSensitive); }
	inline VOID Set(_Char const* Id, _Item* Item, UINT Length=0, BOOL CaseSensitive=false) { this->SetStringInternal(Id, Length, CaseSensitive, Item); }
};

#ifdef __cplusplus_winrt
// String-Index with Handles
template<class _Char, class _Item, UINT _GroupSize>
class StringIndex<_Char, true, _Item^, _GroupSize>: public IndexBase<StringItem<_Char, true>, _Item^, _GroupSize>
{
private:
	// Using
	using _IndexBase=IndexBase<StringItem<_Char, true>, _Item^, _GroupSize>;
	using _ItR=IndexIterator<StringItem<_Char, true>, _Item^, _GroupSize, true>;
	using _ItW=IndexIterator<StringItem<_Char, true>, _Item^, _GroupSize, false>;

public:
	// Access
	inline _Item^ operator[](_Char const* Id)const { return this->template GetInternalPointer<_Item^, _Char const*>(Id); }
	inline BOOL Contains(_Char const* Id, UINT Length=0, BOOL CaseSensitive=false)const { return this->ContainsInternal(Id, Length, CaseSensitive); }
	inline _ItW Find(_Char const* Id, UINT Length=0, BOOL CaseSensitive=false) { return _ItW(this, Id, Length, CaseSensitive); }
	inline _ItR Find(_Char const* Id, UINT Length=0, BOOL CaseSensitive=false)const { return _ItR(this, Id, Length, CaseSensitive); }
	inline _Item^ Get(_Char const* Id, UINT Length=0, BOOL CaseSensitive=false)const { return this->template GetInternalPointer<_Item^, _Char const*, UINT, BOOL>(Id, Length, CaseSensitive); }

	// Modification
	inline BOOL Add(_Char const* Id, _Item^ Item, UINT Length=0, BOOL CaseSensitive=false) { return this->AddStringInternal(Id, Length, CaseSensitve, Item); }
	inline BOOL Remove(_Char const* Id, UINT Length=0, BOOL CaseSensitive=false) { return this->RemoveInternal(Id, Length, CaseSensitive); }
	inline VOID Set(_Char const* Id, _Item^ Item, UINT Length=0, BOOL CaseSensitive=false) { this->SetStringInternal(Id, Length, CaseSensitive, Item); }
};
#endif

// String-Index with String-Items
template<class _CharId, class _CharItem, UINT _GroupSize>
class StringIndex<_CharId, true, StringItem<_CharItem, true>, _GroupSize>: public IndexBase<StringItem<_CharId, true>, StringItem<_CharItem, true>, _GroupSize>
{
private:
	// Using
	using _IndexBase=IndexBase<StringItem<_CharId, true>, StringItem<_CharItem, true>, _GroupSize>;
	using _ItR=IndexIterator<StringItem<_CharId, true>, StringItem<_CharItem, true>, _GroupSize, true>;
	using _ItW=IndexIterator<StringItem<_CharId, true>, StringItem<_CharItem, true>, _GroupSize, false>;

public:
	// Access
	inline _CharItem const* operator[](_CharId const* Id)const { return this->template GetInternalPointer<_CharItem const*, _CharId const*>(Id); }
	inline BOOL Contains(_CharId const* Id, UINT Length=0, BOOL CaseSensitive=false)const { return this->ContainsInternal(Id, Length, CaseSensitive); }
	inline _ItW Find(_CharId const* Id, UINT Length=0, BOOL CaseSensitive=false) { return _ItW(this, Id, Length, CaseSensitive); }
	inline _ItR Find(_CharId const* Id, UINT Length=0, BOOL CaseSensitive=false)const { return _ItR(this, Id, Length, CaseSensitive); }
	inline _CharItem const* Get(_CharId const* Id, UINT Length=0, BOOL CaseSensitive=false)const { return this->template GetInternalPointer<_CharItem const*, _CharId const*, UINT, BOOL>(Id, Length, CaseSensitive); }

	// Modification
	inline BOOL Add(_CharId const* Id, _CharItem const* Item, UINT IdLength=0, UINT ItemLength=0, BOOL CaseSensitive=false) { return this->AddStringInternal(Id, IdLength, CaseSensitive, Item, ItemLength); }
	inline BOOL Remove(_CharId const* Id, UINT Length=0, BOOL CaseSensitive=false) { return this->RemoveInternal(Id, Length, CaseSensitive); }
	inline VOID Set(_CharId const* Id, _CharItem const* Item, UINT IdLength=0, UINT ItemLength=0, BOOL CaseSensitive=false) { this->SetStringInternal(Id, IdLength, CaseSensitive, Item, ItemLength); }
};

// String-Index with Shared String-Items
template<class _CharId, class _CharItem, UINT _GroupSize>
class StringIndex<_CharId, true, StringItem<_CharItem, false>, _GroupSize>: public IndexBase<StringItem<_CharId, true>, StringItem<_CharItem, false>, _GroupSize>
{
private:
	// Using
	using _IndexBase=IndexBase<StringItem<_CharId, true>, StringItem<_CharItem, false>, _GroupSize>;
	using _ItR=IndexIterator<StringItem<_CharId, true>, StringItem<_CharItem, false>, _GroupSize, true>;
	using _ItW=IndexIterator<StringItem<_CharId, true>, StringItem<_CharItem, false>, _GroupSize, false>;

public:
	// Access
	inline _CharItem const* operator[](_CharId const* Id)const { return this->template GetInternalPointer<_CharItem const*, _CharId const*>(Id); }
	inline BOOL Contains(_CharId const* Id, UINT Length=0, BOOL CaseSensitive=false)const { return this->ContainsInternal(Id, Length, CaseSensitive); }
	inline _ItW Find(_CharId const* Id, UINT Length=0, BOOL CaseSensitive=false) { return _ItW(this, Id, Length, CaseSensitive); }
	inline _ItR Find(_CharId const* Id, UINT Length=0, BOOL CaseSensitive=false)const { return _ItR(this, Id, Length, CaseSensitive); }
	inline _CharItem const* Get(_CharId const* Id, UINT Length=0, BOOL CaseSensitive=false)const { return this->template GetInternalPointer<_CharItem const*, _CharId const*, UINT, BOOL>(Id, Length, CaseSensitive); }

	// Modification
	inline BOOL Add(_CharId const* Id, _CharItem const* Item, UINT IdLength=0, BOOL CaseSensitive=false) { return this->AddStringInternal(Id, IdLength, CaseSensitive, Item); }
	inline VOID Set(_CharId const* Id, _CharItem const* Item, UINT IdLength=0, BOOL CaseSensitive=false) { this->SetStringInternal(Id, IdLength, CaseSensitive, Item); }
};


//=====================
// Shared String-Index
//=====================

// Shared String-Index
template<class _Char, class _Item, UINT _GroupSize>
class StringIndex<_Char, false, _Item, _GroupSize>: public IndexBase<StringItem<_Char, false>, _Item, _GroupSize>
{
private:
	// Using
	using _IndexBase=IndexBase<StringItem<_Char, false>, _Item, _GroupSize>;
	using _ItR=IndexIterator<StringItem<_Char, false>, _Item, _GroupSize, true>;
	using _ItW=IndexIterator<StringItem<_Char, false>, _Item, _GroupSize, false>;

public:
	// Access
	inline _Item operator[](_Char const* Id)const { return this->template GetInternal<_Item const&, _Char const*>(Id); }
	inline BOOL Contains(_Char const* Id, UINT Length=0, BOOL CaseSensitive=false)const { return this->ContainsInternal(Id, Length, CaseSensitive); }
	inline _ItW Find(_Char const* Id, UINT Length=0, BOOL CaseSensitive=false) { return _ItW(this, Id, Length, CaseSensitive); }
	inline _ItR Find(_Char const* Id, UINT Length=0, BOOL CaseSensitive=false)const { return _ItR(this, Id, Length, CaseSensitive); }
	inline _Item Get(_Char const* Id, UINT Length=0, BOOL CaseSensitive=false)const { return this->template GetInternal<_Item const&, _Char const*, UINT, BOOL>(Id, Length, CaseSensitive); }
	inline BOOL TryGet(_Char const* Id, _Item* Item, UINT Length=0, BOOL CaseSensitive=false)const { return this->template TryGetInternal<_Item, _Char const*, UINT, BOOL>(Id, Length, CaseSensitive); }

	// Modification
	inline BOOL Add(_Char const* Id, _Item const& Item, BOOL CaseSensitive=false) { return this->template AddStringInternal<_Char, _Item const&>(Id, 0, CaseSensitive, Item); }
	inline BOOL Remove(_Char const* Id, UINT Length=0, BOOL CaseSensitive=false) { return this->RemoveInternal(Id, Length, CaseSensitive); }
	inline VOID Set(_Char const* Id, _Item const& Item, BOOL CaseSensitive=false) { this->template SetStringInternal<_Char, _Item const&>(Id, 0, CaseSensitive, Item); }
};

// Shared String-Index without Items
template <class _Char, UINT _GroupSize>
class StringIndex<_Char, false, VOID, _GroupSize>: public IndexBase<StringItem<_Char, false>, VOID, _GroupSize>
{
private:
	// Using
	using _IndexBase=IndexBase<StringItem<_Char, false>, VOID, _GroupSize>;
	using _ItR=IndexIterator<StringItem<_Char, false>, VOID, _GroupSize, true>;
	using _ItW=IndexIterator<StringItem<_Char, false>, VOID, _GroupSize, false>;

public:
	// Access
	inline BOOL Contains(_Char const* Id, UINT Length=0, BOOL CaseSensitive=false)const { return this->ContainsInternal(Id, Length, CaseSensitive); }
	inline _ItW Find(_Char const* Id, UINT Length=0, BOOL CaseSensitive=false) { return _ItW(this, Id, Length, CaseSensitive); }
	inline _ItR Find(_Char const* Id, UINT Length=0, BOOL CaseSensitive=false)const { return _ItR(this, Id, Length, CaseSensitive); }

	// Modification
	inline BOOL Add(_Char const* Id, BOOL CaseSensitive=false) { return this->AddStringInternal(Id, 0, CaseSensitive); }
	inline BOOL Remove(_Char const* Id, UINT Length=0, BOOL CaseSensitive=false) { return this->RemoveInternal(Id, Length, CaseSensitive); }
};

// Shared String-Index with Pointers
template<class _Char, class _Item, UINT _GroupSize>
class StringIndex<_Char, false, _Item*, _GroupSize>: public IndexBase<StringItem<_Char, false>, _Item*, _GroupSize>
{
private:
	// Using
	using _IndexBase=IndexBase<StringItem<_Char, false>, _Item*, _GroupSize>;
	using _ItR=IndexIterator<StringItem<_Char, false>, _Item*, _GroupSize, true>;
	using _ItW=IndexIterator<StringItem<_Char, false>, _Item*, _GroupSize, false>;

public:
	// Access
	inline _Item* operator[](_Char const* Id)const { return this->template GetInternalPointer<_Item*, _Char const*>(Id); }
	inline BOOL Contains(_Char const* Id, UINT Length=0, BOOL CaseSensitive=false)const { return this->ContainsInternal(Id, Length, CaseSensitive); }
	inline _ItW Find(_Char const* Id, UINT Length=0, BOOL CaseSensitive=false) { return _ItW(this, Id, Length, CaseSensitive); }
	inline _ItR Find(_Char const* Id, UINT Length=0, BOOL CaseSensitive=false)const { return _ItR(this, Id, Length, CaseSensitive); }
	inline _Item* Get(_Char const* Id, UINT Length=0, BOOL CaseSensitive=false)const { return this->template GetInternalPointer<_Item*, _Char const*, UINT, BOOL>(Id, Length, CaseSensitive); }

	// Modification
	inline BOOL Add(_Char const* Id, _Item* Item, BOOL CaseSensitive=false) { return this->AddStringInternal(Id, 0, CaseSensitive, Item); }
	inline BOOL Remove(_Char const* Id, UINT Length=0, BOOL CaseSensitive=false) { return this->RemoveInternal(Id, Length, CaseSensitive); }
	inline VOID Set(_Char const* Id, _Item* Item, BOOL CaseSensitive=false) { this->SetStringInternal(Id, 0, CaseSensitive, Item); }
};

#ifdef __cplusplus_winrt
// Shared String-Index with Handles
template<class _Char, class _Item, UINT _GroupSize>
class StringIndex<_Char, false, _Item^, _GroupSize>: public IndexBase<StringItem<_Char, false>, _Item^, _GroupSize>
{
private:
	// Using
	using _IndexBase=IndexBase<StringItem<_Char, false>, _Item^, _GroupSize>;
	using _ItR=IndexIterator<StringItem<_Char, false>, _Item^, _GroupSize, true>;
	using _ItW=IndexIterator<StringItem<_Char, false>, _Item^, _GroupSize, false>;

public:
	// Access
	inline _Item^ operator[](_Char const* Id)const { return this->template GetInternalPointer<_Item^, _Char const*>(Id); }
	inline BOOL Contains(_Char const* Id, UINT Length=0, BOOL CaseSensitive=false)const { return this->ContainsInternal(Id, Length, CaseSensitive); }
	inline _ItW Find(_Char const* Id, UINT Length=0, BOOL CaseSensitive=false) { return _ItW(this, Id, Length, CaseSensitive); }
	inline _ItR Find(_Char const* Id, UINT Length=0, BOOL CaseSensitive=false)const { return _ItR(this, Id, Length, CaseSensitive); }
	inline _Item^ Get(_Char const* Id, UINT Length=0, BOOL CaseSensitive=false)const { return this->template GetInternalPointer<_Item^, _Char const*, UINT, BOOL>(Id, Length, CaseSensitive); }

	// Modification
	inline BOOL Add(_Char const* Id, _Item^ Item, BOOL CaseSensitive=false) { return this->AddStringInternal(Id, 0, CaseSensitive, Item); }
	inline BOOL Remove(_Char const* Id, UINT Length=0, BOOL CaseSensitive=false) { return this->RemoveInternal(Id, Length, CaseSensitive); }
	inline VOID Set(_Char const* Id, _Item^ Item, BOOL CaseSensitive=false) { this->AddStringInternal(Id, 0, CaseSensitive, Item); }
};
#endif

// Shared String-Index with String-Items
template<class _CharId, class _CharItem, UINT _GroupSize>
class StringIndex<_CharId, false, StringItem<_CharItem, true>, _GroupSize>: public IndexBase<StringItem<_CharId, false>, StringItem<_CharItem, true>, _GroupSize>
{
private:
	// Using
	using _IndexBase=IndexBase<StringItem<_CharId, false>, StringItem<_CharItem, true>, _GroupSize>;
	using _ItR=IndexIterator<StringItem<_CharId, false>, StringItem<_CharItem, true>, _GroupSize, true>;
	using _ItW=IndexIterator<StringItem<_CharId, false>, StringItem<_CharItem, true>, _GroupSize, false>;

public:
	// Access
	inline _CharItem const* operator[](_CharId const* Id)const { return this->template GetInternalPointer<_CharItem const*, _CharId const*>(Id); }
	inline BOOL Contains(_CharId const* Id, UINT Length=0, BOOL CaseSensitive=false)const { return this->ContainsInternal(Id, Length, CaseSensitive); }
	inline _ItW Find(_CharId const* Id, UINT Length=0, BOOL CaseSensitive=false) { return _ItW(this, Id, Length, CaseSensitive); }
	inline _ItR Find(_CharId const* Id, UINT Length=0, BOOL CaseSensitive=false)const { return _ItR(this, Id, Length, CaseSensitive); }
	inline _CharItem const* Get(_CharId const* Id, UINT Length=0, BOOL CaseSensitive=false)const { return this->template GetInternalPointer<_CharItem const*, _CharId const*, UINT, BOOL>(Id, Length, CaseSensitive); }

	// Modification
	inline BOOL Add(_CharId const* Id, _CharItem const* Item, UINT ItemLength=0, BOOL CaseSensitive=false) { return this->AddStringInternal(Id, 0, CaseSensitive, Item, ItemLength); }
	inline BOOL Remove(_CharId const* Id, UINT Length=0, BOOL CaseSensitive=false) { return this->RemoveInternal(Id, Length, CaseSensitive); }
	inline VOID Set(_CharId const* Id, _CharItem const* Item, UINT ItemLength=0, BOOL CaseSensitive=false) { this->SetStringInternal(Id, 0, CaseSensitive, Item, ItemLength); }
};

// Shared String-Index with Shared String-Items
template<class _CharId, class _CharItem, UINT _GroupSize>
class StringIndex<_CharId, false, StringItem<_CharItem, false>, _GroupSize>: public IndexBase<StringItem<_CharId, false>, StringItem<_CharItem, false>, _GroupSize>
{
private:
	// Using
	using _IndexBase=IndexBase<StringItem<_CharId, false>, StringItem<_CharItem, false>, _GroupSize>;
	using _ItR=IndexIterator<StringItem<_CharId, false>, StringItem<_CharItem, false>, _GroupSize, true>;
	using _ItW=IndexIterator<StringItem<_CharId, false>, StringItem<_CharItem, false>, _GroupSize, false>;

public:
	// Access
	inline _CharItem const* operator[](_CharId const* Id)const { return this->template GetInternalPointer<_CharItem const*, _CharId const*>(Id, 0); }
	inline BOOL Contains(_CharId const* Id, UINT Length=0, BOOL CaseSensitive=false)const { return this->ContainsInternal(Id, Length, CaseSensitive); }
	inline _ItW Find(_CharId const* Id, UINT Length=0, BOOL CaseSensitive=false) { return _ItW(this, Id, Length, CaseSensitive); }
	inline _ItR Find(_CharId const* Id, UINT Length=0, BOOL CaseSensitive=false)const { return _ItR(this, Id, Length, CaseSensitive); }
	inline _CharItem const* Get(_CharId const* Id, UINT Length=0, BOOL CaseSensitive=false)const { return this->template GetInternalPointer<_CharItem const*, _CharId const*, UINT, BOOL>(Id, Length, CaseSensitive); }

	// Modification
	inline BOOL Add(_CharId const* Id, _CharItem const* Item, BOOL CaseSensitive=false) { return this->AddStringInternal(Id, 0, CaseSensitive, Item); }
	inline BOOL Remove(_CharId const* Id, UINT Length=0, BOOL CaseSensitive=false) { return this->RemoveInternal(Id, Length, CaseSensitive); }
	inline VOID Set(_CharId const* Id, _CharItem const* Item, BOOL CaseSensitive=false) { this->SetStringInternal(Id, 0, CaseSensitive, Item); }
};

}}}}
