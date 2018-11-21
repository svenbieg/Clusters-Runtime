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
template<class _Char, bool _Alloc, class _Item, unsigned int _GroupSize>
class StringIndex: public IndexBase<String<_Char, _Alloc>, _Item, _GroupSize>
{
public:
	// Access
	inline _Item operator[](_Char const* Id)const { return GetStringInternal<_Item const&, _Char const*, unsigned int, bool>(Id, 0, false); }
	inline bool Contains(_Char const* Id, unsigned int Length=0, bool CaseSensitive=false)const { return ContainsInternal(Id, Length, CaseSensitive); }
	inline IteratorReadWrite Find(_Char const* Id, unsigned int Length=0, bool CaseSensitive=false) { return IteratorReadWrite(this, Id, Length, CaseSensitive); }
	inline IteratorReadOnly Find(_Char const* Id, unsigned int Length=0, bool CaseSensitive=false)const { return IteratorReadOnly(this, Id, Length, CaseSensitive); }
	inline _Item Get(_Char const* Id, unsigned int Length=0, bool CaseSensitive=false)const { return GetStringInternal<_Item const&, _Char const*, unsigned int, bool>(Id, Length, CaseSensitive); }
	inline bool TryGet(_Char const* Id, _Item*, unsigned int Length=0, bool CaseSensitive=false)const { return TryGetInternal<_Item, _Char const*, unsigned int, bool>(Id, Length, CaseSensitive, Item); }

	// Modification
	inline bool Add(_Char const* Id, _Item const& Item, unsigned int Length=0, bool CaseSensitive=false) { return AddStringInternal<_Char, _Item const&>(Id, Length, CaseSensitive, Item); }
	inline bool Remove(_Char const* Id, unsigned int Length=0, bool CaseSensitive=false) { return RemoveInternal(Id, Length, CaseSensitive); }
	inline void Set(_Char const* Id, _Item const& Item, unsigned int Length=0, bool CaseSensitive=false) { SetStringInternal<_Char, _Item const&>(Id, Length, CaseSensitive, Item); }
};

// String-Index without Items
template <class _Char, unsigned int _GroupSize>
class StringIndex<_Char, true, void, _GroupSize>: public IndexBase<String<_Char, true>, void, _GroupSize>
{
public:
	// Access
	inline bool Contains(_Char const* Id, unsigned int Length=0, bool CaseSensitive=false)const { return ContainsInternal(Id, Length, CaseSensitive); }
	inline IteratorReadWrite Find(_Char const* Id, unsigned int Length=0, bool CaseSensitive=false) { return IteratorReadWrite(this, Id, Length, CaseSensitive); }
	inline IteratorReadOnly Find(_Char const* Id, unsigned int Length=0, bool CaseSensitive=false)const { return IteratorReadOnly(this, Id, Length, CaseSensitive); }

	// Modification
	inline bool Add(_Char const* Id, unsigned int Length=0, bool CaseSensitive=false) { return AddStringInternal(Id, Length, CaseSensitive); }
	inline bool Remove(_Char const* Id, unsigned int Length=0, bool CaseSensitive=false) { return RemoveInternal(Id, Length, CaseSensitive); }
};

// String-Index with Pointers
template<class _Char, class _Item, unsigned int _GroupSize>
class StringIndex<_Char, true, _Item*, _GroupSize>: public IndexBase<String<_Char, true>, _Item*, _GroupSize>
{
public:
	// Access
	inline _Item* operator[](_Char const* Id)const { return GetInternalPointer<_Item*, _Char const*>(Id); }
	inline bool Contains(_Char const* Id, unsigned int Length=0, bool CaseSensitive=false)const { return ContainsInternal(Id, Length, CaseSensitive); }
	inline IteratorReadWrite Find(_Char const* Id, unsigned int Length=0, bool CaseSensitive=false) { return IteratorReadWrite(this, Id, Length, CaseSensitive); }
	inline IteratorReadOnly Find(_Char const* Id, unsigned int Length=0, bool CaseSensitive=false)const { return IteratorReadOnly(this, Id, Length, CaseSensitive); }
	inline _Item* Get(_Char const* Id, unsigned int Length=0, bool CaseSensitive=false)const { return GetInternalPointer<_Item*, _Char const*, unsigned int, bool>(Id, Length, CaseSensitive); }

	// Modification
	inline bool Add(_Char const* Id, _Item* Item, unsigned int Length=0, bool CaseSensitive=false) { return AddStringInternal(Id, Length, CaseSensitive, Item); }
	inline bool Remove(_Char const* Id, unsigned int Length=0, bool CaseSensitive=false) { return RemoveInternal(Id, Length, CaseSensitive); }
	inline void Set(_Char const* Id, _Item* Item, unsigned int Length=0, bool CaseSensitive=false) { SetStringInternal(Id, Length, CaseSensitive, Item); }
};

#ifdef __cplusplus_winrt
// String-Index with Handles
template<class _Char, class _Item, unsigned int _GroupSize>
class StringIndex<_Char, true, _Item^, _GroupSize>: public IndexBase<String<_Char, true>, _Item^, _GroupSize>
{
public:
	// Access
	inline _Item^ operator[](_Char const* Id)const { return GetInternalPointer<_Item^, _Char const*>(Id); }
	inline bool Contains(_Char const* Id, unsigned int Length=0, bool CaseSensitive=false)const { return ContainsInternal(Id, Length, CaseSensitive); }
	inline IteratorReadWrite Find(_Char const* Id, unsigned int Length=0, bool CaseSensitive=false) { return IteratorReadWrite(this, Id, Length, CaseSensitive); }
	inline IteratorReadOnly Find(_Char const* Id, unsigned int Length=0, bool CaseSensitive=false)const { return IteratorReadOnly(this, Id, Length, CaseSensitive); }
	inline _Item^ Get(_Char const* Id, unsigned int Length=0, bool CaseSensitive=false)const { return GetInternalPointer<_Item^, _Char const*, unsigned int, bool>(Id, Length, CaseSensitive); }

	// Modification
	inline bool Add(_Char const* Id, _Item^ Item, unsigned int Length=0, bool CaseSensitive=false) { return AddStringInternal(Id, Length, CaseSensitve, Item); }
	inline bool Remove(_Char const* Id, unsigned int Length=0, bool CaseSensitive=false) { return RemoveInternal(Id, Length, CaseSensitive); }
	inline void Set(_Char const* Id, _Item^ Item, unsigned int Length=0, bool CaseSensitive=false) { SetStringInternal(Id, Length, CaseSensitive, Item); }
};
#endif

// String-Index with Strings
template<class _CharId, class _CharItem, unsigned int _GroupSize>
class StringIndex<_CharId, true, String<_CharItem, true>, _GroupSize>: public IndexBase<String<_CharId, true>, String<_CharItem, true>, _GroupSize>
{
public:
	// Access
	inline _CharItem const* operator[](_CharId const* Id)const { return GetInternalPointer<_CharItem const*, _CharId const*>(Id); }
	inline bool Contains(_CharId const* Id, unsigned int Length=0, bool CaseSensitive=false)const { return ContainsInternal(Id, Length, CaseSensitive); }
	inline IteratorReadWrite Find(_CharId const* Id, unsigned int Length=0, bool CaseSensitive=false) { return IteratorReadWrite(this, Id, Length, CaseSensitive); }
	inline IteratorReadOnly Find(_CharId const* Id, unsigned int Length=0, bool CaseSensitive=false)const { return IteratorReadOnly(this, Id, Length, CaseSensitive); }
	inline _CharItem const* Get(_CharId const* Id, unsigned int Length=0, bool CaseSensitive=false)const { return GetInternalPointer<_CharItem const*, _CharId const*, unsigned int, bool>(Id, Length, CaseSensitive); }

	// Modification
	inline bool Add(_CharId const* Id, _CharItem const* Item, unsigned int IdLength=0, unsigned int ItemLength=0, bool CaseSensitive=false) { return AddStringInternal(Id, IdLength, CaseSensitive, Item, ItemLength); }
	inline bool Remove(_CharId const* Id, unsigned int Length=0, bool CaseSensitive=false) { return RemoveInternal(Id, Length, CaseSensitive); }
	inline void Set(_CharId const* Id, _CharItem const* Item, unsigned int IdLength=0, unsigned int ItemLength=0, bool CaseSensitive=false) { SetStringInternal(Id, IdLength, CaseSensitive, Item, ItemLength); }
};

// String-Index with Shared Strings
template<class _CharId, class _CharItem, unsigned int _GroupSize>
class StringIndex<_CharId, true, String<_CharItem, false>, _GroupSize>: public IndexBase<String<_CharId, true>, String<_CharItem, false>, _GroupSize>
{
public:
	// Access
	inline _CharItem const* operator[](_CharId const* Id)const { return GetInternalPointer<_CharItem const*, _CharId const*>(Id); }
	inline bool Contains(_CharId const* Id, unsigned int Length=0, bool CaseSensitive=false)const { return ContainsInternal(Id, Length, CaseSensitive); }
	inline IteratorReadWrite Find(_CharId const* Id, unsigned int Length=0, bool CaseSensitive=false) { return IteratorReadWrite(this, Id, Length, CaseSensitive); }
	inline IteratorReadOnly Find(_CharId const* Id, unsigned int Length=0, bool CaseSensitive=false)const { return IteratorReadOnly(this, Id, Length, CaseSensitive); }
	inline _CharItem const* Get(_CharId const* Id, unsigned int Length=0, bool CaseSensitive=false)const { return GetInternalPointer<_CharItem const*, _CharId const*, unsigned int, bool>(Id, Length, CaseSensitive); }

	// Modification
	inline bool Add(_CharId const* Id, _CharItem const* Item, unsigned int IdLength=0, bool CaseSensitive=false) { return AddStringInternal(Id, IdLength, CaseSensitive, Item); }
	inline void Set(_CharId const* Id, _CharItem const* Item, unsigned int IdLength=0, bool CaseSensitive=false) { SetStringInternal(Id, IdLength, CaseSensitive, Item); }
};


//=====================
// Shared String-Index
//=====================

// Shared String-Index
template<class _Char, class _Item, unsigned int _GroupSize>
class StringIndex<_Char, false, _Item, _GroupSize>: public IndexBase<String<_Char, false>, _Item, _GroupSize>
{
public:
	// Access
	inline _Item operator[](_Char const* Id)const { return GetInternal<_Item const&, _Char const*>(Id); }
	inline bool Contains(_Char const* Id, unsigned int Length=0, bool CaseSensitive=false)const { return ContainsInternal(Id, Length, CaseSensitive); }
	inline IteratorReadWrite Find(_Char const* Id, unsigned int Length=0, bool CaseSensitive=false) { return IteratorReadWrite(this, Id, Length, CaseSensitive); }
	inline IteratorReadOnly Find(_Char const* Id, unsigned int Length=0, bool CaseSensitive=false)const { return IteratorReadOnly(this, Id, Length, CaseSensitive); }
	inline _Item Get(_Char const* Id, unsigned int Length=0, bool CaseSensitive=false)const { return GetInternal<_Item const&, _Char const*, unsigned int, bool>(Id, Length, CaseSensitive); }
	inline bool TryGet(_Char const* Id, _Item* Item, unsigned int Length=0, bool CaseSensitive=false)const { return TryGetInternal<_Item, _Char const*, unsigned int, bool>(Id, Length, CaseSensitive); }

	// Modification
	inline bool Add(_Char const* Id, _Item const& Item, bool CaseSensitive=false) { return AddStringInternal<_Char, _Item const&>(Id, 0, CaseSensitive, Item); }
	inline bool Remove(_Char const* Id, unsigned int Length=0, bool CaseSensitive=false) { return RemoveInternal(Id, Length, CaseSensitive); }
	inline void Set(_Char const* Id, _Item const& Item, bool CaseSensitive=false) { SetStringInternal<_Char, _Item const&>(Id, 0, CaseSensitive, Item); }
};

// Shared String-Index without Items
template <class _Char, unsigned int _GroupSize>
class StringIndex<_Char, false, void, _GroupSize>: public IndexBase<String<_Char, false>, void, _GroupSize>
{
public:
	// Access
	inline bool Contains(_Char const* Id, unsigned int Length=0, bool CaseSensitive=false)const { return ContainsInternal(Id, Length, CaseSensitive); }
	inline IteratorReadWrite Find(_Char const* Id, unsigned int Length=0, bool CaseSensitive=false) { return IteratorReadWrite(this, Id, Length, CaseSensitive); }
	inline IteratorReadOnly Find(_Char const* Id, unsigned int Length=0, bool CaseSensitive=false)const { return IteratorReadOnly(this, Id, Length, CaseSensitive); }

	// Modification
	inline bool Add(_Char const* Id, bool CaseSensitive=false) { return AddStringInternal(Id, 0, CaseSensitive); }
	inline bool Remove(_Char const* Id, unsigned int Length=0, bool CaseSensitive=false) { return RemoveInternal(Id, Length, CaseSensitive); }
};

// Shared String-Index with Pointers
template<class _Char, class _Item, unsigned int _GroupSize>
class StringIndex<_Char, false, _Item*, _GroupSize>: public IndexBase<String<_Char, false>, _Item*, _GroupSize>
{
public:
	// Access
	inline _Item* operator[](_Char const* Id)const { return GetInternalPointer<_Item*, _Char const*>(Id); }
	inline bool Contains(_Char const* Id, unsigned int Length=0, bool CaseSensitive=false)const { return ContainsInternal(Id, Length, CaseSensitive); }
	inline IteratorReadWrite Find(_Char const* Id, unsigned int Length=0, bool CaseSensitive=false) { return IteratorReadWrite(this, Id, Length, CaseSensitive); }
	inline IteratorReadOnly Find(_Char const* Id, unsigned int Length=0, bool CaseSensitive=false)const { return IteratorReadOnly(this, Id, Length, CaseSensitive); }
	inline _Item* Get(_Char const* Id, unsigned int Length=0, bool CaseSensitive=false)const { return GetInternalPointer<_Item*, _Char const*, unsigned int, bool>(Id, Length, CaseSensitive); }

	// Modification
	inline bool Add(_Char const* Id, _Item* Item, bool CaseSensitive=false) { return AddStringInternal(Id, 0, CaseSensitive, Item); }
	inline bool Remove(_Char const* Id, unsigned int Length=0, bool CaseSensitive=false) { return RemoveInternal(Id, Length, CaseSensitive); }
	inline void Set(_Char const* Id, _Item* Item, bool CaseSensitive=false) { SetStringInternal(Id, 0, CaseSensitive, Item); }
};

#ifdef __cplusplus_winrt
// Shared String-Index with Handles
template<class _Char, class _Item, unsigned int _GroupSize>
class StringIndex<_Char, false, _Item^, _GroupSize>: public IndexBase<String<_Char, false>, _Item^, _GroupSize>
{
public:
	// Access
	inline _Item^ operator[](_Char const* Id)const { return GetInternalPointer<_Item^, _Char const*>(Id); }
	inline bool Contains(_Char const* Id, unsigned int Length=0, bool CaseSensitive=false)const { return ContainsInternal(Id, Length, CaseSensitive); }
	inline IteratorReadWrite Find(_Char const* Id, unsigned int Length=0, bool CaseSensitive=false) { return IteratorReadWrite(this, Id, Length, CaseSensitive); }
	inline IteratorReadOnly Find(_Char const* Id, unsigned int Length=0, bool CaseSensitive=false)const { return IteratorReadOnly(this, Id, Length, CaseSensitive); }
	inline _Item^ Get(_Char const* Id, unsigned int Length=0, bool CaseSensitive=false)const { return GetInternalPointer<_Item^, _Char const*, unsigned int, bool>(Id, Length, CaseSensitive); }

	// Modification
	inline bool Add(_Char const* Id, _Item^ Item, bool CaseSensitive=false) { return AddStringInternal(Id, 0, CaseSensitive, Item); }
	inline bool Remove(_Char const* Id, unsigned int Length=0, bool CaseSensitive=false) { return RemoveInternal(Id, Length, CaseSensitive); }
	inline void Set(_Char const* Id, _Item^ Item, bool CaseSensitive=false) { AddStringInternal(Id, 0, CaseSensitive, Item); }
};
#endif

// Shared String-Index with Strings
template<class _CharId, class _CharItem, unsigned int _GroupSize>
class StringIndex<_CharId, false, String<_CharItem, true>, _GroupSize>: public IndexBase<String<_CharId, false>, String<_CharItem, true>, _GroupSize>
{
public:
	// Access
	inline _CharItem const* operator[](_CharId const* Id)const { return GetInternalPointer<_CharItem const*, _CharId const*>(Id); }
	inline bool Contains(_CharId const* Id, unsigned int Length=0, bool CaseSensitive=false)const { return ContainsInternal(Id, Length, CaseSensitive); }
	inline IteratorReadWrite Find(_CharId const* Id, unsigned int Length=0, bool CaseSensitive=false) { return IteratorReadWrite(this, Id, Length, CaseSensitive); }
	inline IteratorReadOnly Find(_CharId const* Id, unsigned int Length=0, bool CaseSensitive=false)const { return IteratorReadOnly(this, Id, Length, CaseSensitive); }
	inline _CharItem const* Get(_CharId const* Id, unsigned int Length=0, bool CaseSensitive=false)const { return GetInternalPointer<_CharItem const*, _CharId const*, unsigned int, bool>(Id, Length, CaseSensitive); }

	// Modification
	inline bool Add(_CharId const* Id, _CharItem const* Item, unsigned int ItemLength=0, bool CaseSensitive=false) { return AddStringInternal(Id, 0, CaseSensitive, Item, ItemLength); }
	inline bool Remove(_CharId const* Id, unsigned int Length=0, bool CaseSensitive=false) { return RemoveInternal(Id, Length, CaseSensitive); }
	inline void Set(_CharId const* Id, _CharItem const* Item, unsigned int ItemLength=0, bool CaseSensitive=false) { SetStringInternal(Id, 0, CaseSensitive, Item, ItemLength); }
};

// Shared String-Index with Shared Strings
template<class _CharId, class _CharItem, unsigned int _GroupSize>
class StringIndex<_CharId, false, String<_CharItem, false>, _GroupSize>: public IndexBase<String<_CharId, false>, String<_CharItem, false>, _GroupSize>
{
public:
	// Access
	inline _CharItem const* operator[](_CharId const* Id)const { return GetInternalPointer<_CharItem const*, _CharId const*>(Id, 0); }
	inline bool Contains(_CharId const* Id, unsigned int Length=0, bool CaseSensitive=false)const { return ContainsInternal(Id, Length, CaseSensitive); }
	inline IteratorReadWrite Find(_CharId const* Id, unsigned int Length=0, bool CaseSensitive=false) { return IteratorReadWrite(this, Id, Length, CaseSensitive); }
	inline IteratorReadOnly Find(_CharId const* Id, unsigned int Length=0, bool CaseSensitive=false)const { return IteratorReadOnly(this, Id, Length, CaseSensitive); }
	inline _CharItem const* Get(_CharId const* Id, unsigned int Length=0, bool CaseSensitive=false)const { return GetInternalPointer<_CharItem const*, _CharId const*, unsigned int, bool>(Id, Length, CaseSensitive); }

	// Modification
	inline bool Add(_CharId const* Id, _CharItem const* Item, bool CaseSensitive=false) { return AddStringInternal(Id, 0, CaseSensitive, Item); }
	inline bool Remove(_CharId const* Id, unsigned int Length=0, bool CaseSensitive=false) { return RemoveInternal(Id, Length, CaseSensitive); }
	inline void Set(_CharId const* Id, _CharItem const* Item, bool CaseSensitive=false) { SetStringInternal(Id, 0, CaseSensitive, Item); }
};

}}}}
