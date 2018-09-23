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
	namespace Details {
		namespace Index {


//==============
// String-Index
//==============

#pragma region String-Index

// String-Index
template<class CHAR, BOOL _Alloc, class ITEM, UINT _GroupSize>
class StringIndex: public IndexBase<String<CHAR, _Alloc>, ITEM, _GroupSize>
{
public:
	// Access
	inline ITEM& operator[](CHAR const* Id) { return *GetInternalAddress(Id, 0, false); }
	inline ITEM const& operator[](CHAR const* Id)const { return *GetInternalAddress(Id, 0, false); }
	inline BOOL Contains(CHAR const* Id, UINT Length=0, BOOL CaseSensitive=false)const { return pRoot->Contains(Id, Length, CaseSensitive); }
	inline IteratorReadWrite Find(CHAR const* Id, UINT Length=0, BOOL CaseSensitive=false) { return IteratorReadWrite(this, Id, Length, CaseSensitive); }
	inline IteratorReadOnly Find(CHAR const* Id, UINT Length=0, BOOL CaseSensitive=false)const { return IteratorReadOnly(this, Id, Length, CaseSensitive); }
	inline ITEM& Get(CHAR const* Id, UINT Length=0, BOOL CaseSensitive=false) { return *GetInternalAddress(Id, Length, CaseSensitive); }
	inline ITEM const& Get(CHAR const* Id, UINT Length=0, BOOL CaseSensitive=false)const { return *GetInternalAddress(Id, Length, CaseSensitive); }
	inline Item& GetAt(UINT64 Position) { return *pRoot->GetAt(Position); }
	inline Item const& GetAt(UINT64 Position)const { return *pRoot->GetAt(Position); }
	inline ITEM* TryGet(CHAR const* Id, UINT Length=0, BOOL CaseSensitive=false)const { return GetInternalAddress(Id, Length, CaseSensitive); }

	// Modification
	inline Item* Add(CHAR const* Id, UINT Length=0, BOOL CaseSensitive=false)
		{
		Item* pitem=AddInternal(Id, Length, CaseSensitive);
		if(!pitem)
			return nullptr;
		new (pitem) Item(Id, Length);
		return pitem;
		}
	inline Item* Add(CHAR const* Id, ITEM const& Value, UINT Length=0, BOOL CaseSensitive=false)
		{
		Item* pitem=AddInternal(Id, Length, CaseSensitive);
		if(!pitem)
			return nullptr;
		new (pitem) Item(Id, Length, Value);
		return pitem;
		}
	inline BOOL Remove(CHAR const* Id, UINT Length=0, BOOL CaseSensitive=false) { return RemoveInternal(Id, Length, CaseSensitive); }
	inline Item* Set(CHAR const* Id, ITEM const& Value, UINT Length=0, BOOL CaseSensitive=false)
		{
		Item* pitem=pRoot->Get(Id, Length, CaseSensitive);
		if(pitem)
			{
			pitem->SetItem(Value);
			return pitem;
			}
		return Add(Id, Value, Length, CaseSensitive);
		}
};

// String-Index with Empty Items
template <class CHAR, UINT _GroupSize>
class StringIndex<CHAR, true, VOID, _GroupSize>: public IndexBase<String<CHAR, true>, VOID, _GroupSize>
{
public:
	// Access
	inline CHAR const* operator[](UINT64 Position)const { return pRoot->GetAt(Position)->GetItem(); }
	inline BOOL Contains(CHAR const* Id, UINT Length=0, BOOL CaseSensitive=false)const { return pRoot->Contains(Id, Length, CaseSensitive); }
	inline IteratorReadWrite Find(CHAR const* Id, UINT Length=0, BOOL CaseSensitive=false) { return IteratorReadWrite(this, Id, Length, CaseSensitive); }
	inline IteratorReadOnly Find(CHAR const* Id, UINT Length=0, BOOL CaseSensitive=false)const { return IteratorReadOnly(this, Id, Length, CaseSensitive); }
	inline CHAR const* GetAt(UINT64 Position)const { return pRoot->GetAt(Position)->GetItem(); }

	// Modification
	inline Item* Add(CHAR const* Id, UINT Length=0, BOOL CaseSensitive=false)
		{
		Item* pitem=AddInternal(Id, Length, CaseSensitive);
		if(!pitem)
			return nullptr;
		new (pitem) Item(Id, Length);
		return pitem;
		}
	inline CHAR const* ReleaseAt(UINT64 Position) { return ReleaseInternal<CHAR const*>(Position); }
	inline BOOL Remove(CHAR const* Id, UINT Length=0, BOOL CaseSensitive=false) { return RemoveInternal(Id, Length, CaseSensitive); }
};

// String-Index with Pointers
template<class CHAR, class ITEM, UINT _GroupSize>
class StringIndex<CHAR, true, ITEM*, _GroupSize>: public IndexBase<String<CHAR, true>, ITEM*, _GroupSize>
{
public:
	// Access
	inline ITEM* operator[](CHAR const* Id)const { return GetInternal<ITEM*, CHAR const*, UINT, BOOL>(Id, 0, false); }
	inline BOOL Contains(CHAR const* Id, UINT Length=0, BOOL CaseSensitive=false)const { return pRoot->Contains(Id, Length, CaseSensitive); }
	inline IteratorReadWrite Find(CHAR const* Id, UINT Length=0, BOOL CaseSensitive=false) { return IteratorReadWrite(this, Id, Length, CaseSensitive); }
	inline IteratorReadOnly Find(CHAR const* Id, UINT Length=0, BOOL CaseSensitive=false)const { return IteratorReadOnly(this, Id, Length, CaseSensitive); }
	inline ITEM* Get(CHAR const* Id, UINT Length=0, BOOL CaseSensitive=false)const { return GetInternal<ITEM*, CHAR const*, UINT, BOOL>(Id, Length, CaseSensitive); }
	inline Item& GetAt(UINT64 Position) { return *pRoot->GetAt(Position); }
	inline Item const& GetAt(UINT64 Position)const { return *pRoot->GetAt(Position); }

	// Modification
	inline Item* Add(CHAR const* Id, ITEM* Value, UINT Length=0, BOOL CaseSensitive=false)
		{
		Item* pitem=AddInternal(Id, Length, CaseSensitive);
		if(!pitem)
			return nullptr;
		new (pitem) Item(Id, Length, Value);
		return pitem;
		}
	inline ITEM* ReleaseAt(UINT64 Position) { return ReleaseInternal<ITEM*>(Position); }
	inline BOOL Remove(CHAR const* Id, UINT Length=0, BOOL CaseSensitive=false) { return RemoveInternal(Id, Length, CaseSensitive); }
	inline Item* Set(CHAR const* Id, ITEM* Value, UINT Length=0, BOOL CaseSensitive=false)
		{
		Item* pitem=pRoot->Get(Id, Length, CaseSensitive);
		if(pitem)
			{
			pitem->SetItem(Value);
			return pitem;
			}
		return Add(Id, Value, Length, CaseSensitive);
		}
};

#ifdef __cplusplus_winrt
// String-Index with Handles
template<class CHAR, class ITEM, UINT _GroupSize>
class StringIndex<CHAR, true, ITEM^, _GroupSize>: public IndexBase<String<CHAR, true>, ITEM^, _GroupSize>
{
public:
	// Access
	inline ITEM^ operator[](CHAR const* Id)const { return GetInternal<ITEM^, CHAR const*, UINT, BOOL>(Id, 0, false); }
	inline BOOL Contains(CHAR const* Id, UINT Length=0, BOOL CaseSensitive=false)const { return pRoot->Contains(Id, Length, CaseSensitive); }
	inline IteratorReadWrite Find(CHAR const* Id, UINT Length=0, BOOL CaseSensitive=false) { return IteratorReadWrite(this, Id, Length, CaseSensitive); }
	inline IteratorReadOnly Find(CHAR const* Id, UINT Length=0, BOOL CaseSensitive=false)const { return IteratorReadOnly(this, Id, Length, CaseSensitive); }
	inline ITEM^ Get(CHAR const* Id, UINT Length=0, BOOL CaseSensitive=false)const { return GetInternal<ITEM^, CHAR const*, UINT, BOOL>(Id, Length, CaseSensitive); }
	inline Item& GetAt(UINT64 Position) { return *pRoot->GetAt(Position); }
	inline Item const& GetAt(UINT64 Position)const { return *pRoot->GetAt(Position); }

	// Modification
	inline Item* Add(CHAR const* Id, ITEM^ Value, UINT Length=0, BOOL CaseSensitive=false)
		{
		Item* pitem=AddInternal(Id, Length, CaseSensitive);
		if(!pitem)
			return nullptr;
		new (pitem) Item(Id, Length, Value);
		return pitem;
		}
	inline BOOL Remove(CHAR const* Id, UINT Length=0, BOOL CaseSensitive=false) { return RemoveInternal(Id, Length, CaseSensitive); }
	inline Item* Set(CHAR const* Id, ITEM^ Value, UINT Length=0, BOOL CaseSensitive=false)
		{
		Item* pitem=pRoot->Get(Id, Length, CaseSensitive);
		if(pitem)
			{
			pitem->SetItem(Value);
			return;
			}
		return Add(Id, Value, Length, CaseSensitive);
		}
};
#endif

// String-Index with Strings
template<class CHAR_ID, class CHAR_ITEM, UINT _GroupSize>
class StringIndex<CHAR_ID, true, String<CHAR_ITEM, true>, _GroupSize>: public IndexBase<String<CHAR_ID, true>, String<CHAR_ITEM, true>, _GroupSize>
{
public:
	// Access
	inline CHAR const* operator[](CHAR const* Id)const { return GetInternal<CHAR const*, CHAR const*, UINT, BOOL>(Id, 0, false); }
	inline BOOL Contains(CHAR const* Id, UINT Length=0, BOOL CaseSensitive=false)const { return pRoot->Contains(Id, Length, CaseSensitive); }
	inline IteratorReadWrite Find(CHAR const* Id, UINT Length=0, BOOL CaseSensitive=false) { return IteratorReadWrite(this, Id, Length, CaseSensitive); }
	inline IteratorReadOnly Find(CHAR const* Id, UINT Length=0, BOOL CaseSensitive=false)const { return IteratorReadOnly(this, Id, Length, CaseSensitive); }
	inline CHAR const* Get(CHAR const* Id, UINT Length=0, BOOL CaseSensitive=false)const { return GetInternal<CHAR const*, CHAR const*, UINT, BOOL>(Id, Length, CaseSensitive); }
	inline Item& GetAt(UINT64 Position) { return *pRoot->GetAt(Position); }
	inline Item const& GetAt(UINT64 Position)const { return *pRoot->GetAt(Position); }

	// Modification
	inline Item* Add(CHAR const* Id, CHAR const* Value, UINT IdLength=0, UINT ValueLength=0, BOOL CaseSensitive=false)
		{
		Item* pitem=AddInternal(Id, IdLength, CaseSensitive);
		if(!pitem)
			return nullptr;
		new (pitem) Item(Id, IdLength, Value, ValueLength);
		return pitem;
		}
	inline CHAR const* ReleaseAt(UINT64 Position) { return ReleaseInternal<CHAR const*>(Position); }
	inline BOOL Remove(CHAR const* Id, UINT Length=0, BOOL CaseSensitive=false) { return RemoveInternal(Id, Length, CaseSensitive); }
	inline Item* Set(CHAR const* Id, CHAR const* Value, UINT IdLength=0, UINT ValueLength=0, BOOL CaseSensitive=false)
		{
		Item* pitem=pRoot->Get(Id, IdLength, CaseSensitive);
		if(pitem)
			{
			pitem->SetItem(Value, ValueLength);
			return pitem;
			}
		return Add(Id, Value, IdLength, ValueLength, CaseSensitive);
		}
};

// String-Index with Shared Strings
template<class CHAR_ID, class CHAR_ITEM, UINT _GroupSize>
class StringIndex<CHAR_ID, true, String<CHAR_ITEM, false>, _GroupSize>: public IndexBase<String<CHAR_ID, true>, String<CHAR_ITEM, false>, _GroupSize>
{
public:
	// Access
	inline CHAR const* operator[](CHAR const* Id)const { return GetInternal<CHAR const*, CHAR const*, UINT, BOOL>(Id, 0, false); }
	inline BOOL Contains(CHAR const* Id, UINT Length=0, BOOL CaseSensitive=false)const { return pRoot->Contains(Id, Length, CaseSensitive); }
	inline IteratorReadWrite Find(CHAR const* Id, UINT Length=0, BOOL CaseSensitive=false) { return IteratorReadWrite(this, Id, Length, CaseSensitive); }
	inline IteratorReadOnly Find(CHAR const* Id, UINT Length=0, BOOL CaseSensitive=false)const { return IteratorReadOnly(this, Id, Length, CaseSensitive); }
	inline CHAR const* Get(CHAR const* Id, UINT Length=0, BOOL CaseSensitive=false)const { return GetInternal<CHAR const*, CHAR const*, UINT, BOOL>(Id, Length, CaseSensitive); }
	inline Item& GetAt(UINT64 Position) { return *pRoot->GetAt(Position); }
	inline Item const& GetAt(UINT64 Position)const { return *pRoot->GetAt(Position); }

	// Modification
	inline Item* Add(CHAR const* Id, CHAR const* Value, UINT IdLength=0, UINT ValueLength=0, BOOL CaseSensitive=false)
		{
		Item* pitem=AddInternal(Id, IdLength, CaseSensitive);
		if(!pitem)
			return nullptr;
		new (pitem) Item(Id, IdLength, Value, ValueLength);
		return pitem;
		}
	inline CHAR const* ReleaseAt(UINT64 Position) { return ReleaseInternal<CHAR const*>(Position); }
	inline BOOL Remove(CHAR const* Id, UINT Length=0, BOOL CaseSensitive=false) { return RemoveInternal(Id, Length, CaseSensitive); }
	inline Item* Set(CHAR const* Id, CHAR const* Value, UINT IdLength=0, UINT ValueLength=0, BOOL CaseSensitive=false)
		{
		Item* pitem=pRoot->Get(Id, IdLength, CaseSensitive);
		if(pitem)
			{
			pitem->SetItem(Value, ValueLength);
			return pitem;
			}
		return Add(Id, Value, IdLength, ValueLength, CaseSensitive);
		}
};

#pragma endregion


//=====================
// Shared String-Index
//=====================

#pragma region Shared String-Index

// Shared String-Index
template<class CHAR, class ITEM, UINT _GroupSize>
class StringIndex<CHAR, false, ITEM, _GroupSize>: public IndexBase<String<CHAR, false>, ITEM, _GroupSize>
{
public:
	// Access
	inline ITEM& operator[](CHAR const* Id) { return *GetInternalAddress(Id, 0, false); }
	inline ITEM const& operator[](CHAR const* Id)const { return *GetInternalAddress(Id, 0, false); }
	inline BOOL Contains(CHAR const* Id, UINT Length=0, BOOL CaseSensitive=false)const { return pRoot->Contains(Id, Length, CaseSensitive); }
	inline IteratorReadWrite Find(CHAR const* Id, UINT Length=0, BOOL CaseSensitive=false) { return IteratorReadWrite(this, Id, Length, CaseSensitive); }
	inline IteratorReadOnly Find(CHAR const* Id, UINT Length=0, BOOL CaseSensitive=false)const { return IteratorReadOnly(this, Id, Length, CaseSensitive); }
	inline ITEM& Get(CHAR const* Id, UINT Length=0, BOOL CaseSensitive=false) { return *GetInternalAddress(Id, Length, CaseSensitive); }
	inline ITEM const& Get(CHAR const* Id, UINT Length=0, BOOL CaseSensitive=false)const { return *GetInternalAddress(Id, Length, CaseSensitive); }
	inline Item& GetAt(UINT64 Position) { return *pRoot->GetAt(Position); }
	inline Item const& GetAt(UINT64 Position)const { return *pRoot->GetAt(Position); }
	inline ITEM* TryGet(CHAR const* Id, UINT Length=0, BOOL CaseSensitive=false)const { return GetInternalAddress(Id, Length, CaseSensitive); }

	// Modification
	inline Item* Add(CHAR const* Id, BOOL CaseSensitive=false)
		{
		Item* pitem=AddInternal(Id, 0, CaseSensitive);
		if(!pitem)
			return nullptr;
		new (pitem) Item(Id, 0);
		return pitem;
		}
	inline Item* Add(CHAR const* Id, ITEM const& Value, BOOL CaseSensitive=false)
		{
		Item* pitem=AddInternal(Id, 0, CaseSensitive);
		if(!pitem)
			return nullptr;
		new (pitem) Item(Id, 0, Value);
		return pitem;
		}
	inline BOOL Remove(CHAR const* Id, UINT Length=0, BOOL CaseSensitive=false) { return RemoveInternal(Id, Length, CaseSensitive); }
	inline Item* Set(CHAR const* Id, ITEM const& Value, BOOL CaseSensitive=false)
		{
		Item* pitem=pRoot->Get(Id, 0, CaseSensitive);
		if(pitem)
			{
			pitem->SetItem(Value);
			return pitem;
			}
		return Add(Id, Value, CaseSensitive);
		}
};

// Shared String-Index with Empty Items
template <class CHAR, UINT _GroupSize>
class StringIndex<CHAR, false, VOID, _GroupSize>: public IndexBase<String<CHAR, false>, VOID, _GroupSize>
{
public:
	// Access
	inline CHAR const* operator[](UINT64 Position)const { return pRoot->GetAt(Position)->GetItem(); }
	inline BOOL Contains(CHAR const* Id, UINT Length=0, BOOL CaseSensitive=false)const { return pRoot->Contains(Id, Length, CaseSensitive); }
	inline IteratorReadWrite Find(CHAR const* Id, UINT Length=0, BOOL CaseSensitive=false) { return IteratorReadWrite(this, Id, Length, CaseSensitive); }
	inline IteratorReadOnly Find(CHAR const* Id, UINT Length=0, BOOL CaseSensitive=false)const { return IteratorReadOnly(this, Id, Length, CaseSensitive); }
	inline CHAR const* GetAt(UINT64 Position)const { return pRoot->GetAt(Position)->GetItem(); }

	// Modification
	inline Item* Add(CHAR const* Id, BOOL CaseSensitive=false)
		{
		Item* pitem=AddInternal(Id, 0, CaseSensitive);
		if(!pitem)
			return nullptr;
		new (pitem) Item(Id, 0);
		return pitem;
		}
	inline CHAR const* ReleaseAt(UINT64 Position) { return ReleaseInternal<CHAR const*>(Position); }
	inline BOOL Remove(CHAR const* Id, UINT Length=0, BOOL CaseSensitive=false) { return RemoveInternal(Id, Length, CaseSensitive); }
};

// Shared String-Index with Pointers
template<class CHAR, class ITEM, UINT _GroupSize>
class StringIndex<CHAR, false, ITEM*, _GroupSize>: public IndexBase<String<CHAR, false>, ITEM*, _GroupSize>
{
public:
	// Access
	inline ITEM* operator[](CHAR const* Id)const { return GetInternal<ITEM*, CHAR const*, UINT, BOOL>(Id, 0, false); }
	inline BOOL Contains(CHAR const* Id, UINT Length=0, BOOL CaseSensitive=false)const { return pRoot->Contains(Id, Length, CaseSensitive); }
	inline IteratorReadWrite Find(CHAR const* Id, UINT Length=0, BOOL CaseSensitive=false) { return IteratorReadWrite(this, Id, Length, CaseSensitive); }
	inline IteratorReadOnly Find(CHAR const* Id, UINT Length=0, BOOL CaseSensitive=false)const { return IteratorReadOnly(this, Id, Length, CaseSensitive); }
	inline ITEM* Get(CHAR const* Id, UINT Length=0, BOOL CaseSensitive=false)const { return GetInternal<ITEM*, CHAR const*, UINT, BOOL>(Id, Length, CaseSensitive); }
	inline Item& GetAt(UINT64 Position) { return *pRoot->GetAt(Position); }
	inline Item const& GetAt(UINT64 Position)const { return *pRoot->GetAt(Position); }

	// Modification
	inline Item* Add(CHAR const* Id, ITEM* Value, BOOL CaseSensitive=false)
		{
		Item* pitem=AddInternal(Id, 0, CaseSensitive);
		if(!pitem)
			return nullptr;
		new (pitem) Item(Id, 0, Value);
		return pitem;
		}
	inline ITEM* ReleaseAt(UINT64 Position) { return ReleaseInternal<ITEM*>(Position); }
	inline BOOL Remove(CHAR const* Id, UINT Length=0, BOOL CaseSensitive=false) { return RemoveInternal(Id, Length, CaseSensitive); }
	inline Item* Set(CHAR const* Id, ITEM* Value, BOOL CaseSensitive=false)
		{
		Item* pitem=pRoot->Get(Id, 0, CaseSensitive);
		if(pitem)
			{
			pitem->SetItem(Value);
			return pitem;
			}
		return Add(Id, Value, CaseSensitive);
		}
};

#ifdef __cplusplus_winrt
// Shared String-Index with Handles
template<class CHAR, class ITEM, UINT _GroupSize>
class StringIndex<CHAR, false, ITEM^, _GroupSize>: public IndexBase<String<CHAR, false>, ITEM^, _GroupSize>
{
public:
	// Access
	inline ITEM^ operator[](CHAR const* Id)const { return GetInternal<ITEM^, CHAR const*, UINT, BOOL>(Id, 0, false); }
	inline BOOL Contains(CHAR const* Id, UINT Length=0, BOOL CaseSensitive=false)const { return pRoot->Contains(Id, Length, CaseSensitive); }
	inline IteratorReadWrite Find(CHAR const* Id, UINT Length=0, BOOL CaseSensitive=false) { return IteratorReadWrite(this, Id, Length, CaseSensitive); }
	inline IteratorReadOnly Find(CHAR const* Id, UINT Length=0, BOOL CaseSensitive=false)const { return IteratorReadOnly(this, Id, Length, CaseSensitive); }
	inline ITEM^ Get(CHAR const* Id, UINT Length=0, BOOL CaseSensitive=false)const { return GetInternal<ITEM^, CHAR const*, UINT, BOOL>(Id, Length, CaseSensitive); }
	inline Item& GetAt(UINT64 Position) { return *pRoot->GetAt(Position); }
	inline Item const& GetAt(UINT64 Position)const { return *pRoot->GetAt(Position); }

	// Modification
	inline Item* Add(CHAR const* Id, ITEM^ Value, BOOL CaseSensitive=false)
		{
		Item* pitem=AddInternal(Id, 0, CaseSensitive);
		if(!pitem)
			return nullptr;
		new (pitem) Item(Id, 0, Value);
		return pitem;
		}
	inline BOOL Remove(CHAR const* Id, UINT Length=0, BOOL CaseSensitive=false) { return RemoveInternal(Id, Length, CaseSensitive); }
	inline Item* Set(CHAR const* Id, ITEM^ Value, BOOL CaseSensitive=false)
		{
		Item* pitem=pRoot->Get(Id, 0, CaseSensitive);
		if(pitem)
			{
			pitem->SetItem(Value);
			return;
			}
		return Add(Id, Value, CaseSensitive);
		}
};
#endif

// Shared String-Index with Strings
template<class CHAR_ID, class CHAR_ITEM, UINT _GroupSize>
class StringIndex<CHAR_ID, false, String<CHAR_ITEM, true>, _GroupSize>: public IndexBase<String<CHAR_ID, false>, String<CHAR_ITEM, true>, _GroupSize>
{
public:
	// Access
	inline CHAR const* operator[](CHAR const* Id)const { return GetInternal<CHAR const*, CHAR const*, UINT, BOOL>(Id, 0, false); }
	inline BOOL Contains(CHAR const* Id, UINT Length=0, BOOL CaseSensitive=false)const { return pRoot->Contains(Id, Length, CaseSensitive); }
	inline IteratorReadWrite Find(CHAR const* Id, UINT Length=0, BOOL CaseSensitive=false) { return IteratorReadWrite(this, Id, Length, CaseSensitive); }
	inline IteratorReadOnly Find(CHAR const* Id, UINT Length=0, BOOL CaseSensitive=false)const { return IteratorReadOnly(this, Id, Length, CaseSensitive); }
	inline CHAR const* Get(CHAR const* Id, UINT Length=0, BOOL CaseSensitive=false)const { return GetInternal<CHAR const*, CHAR const*, UINT, BOOL>(Id, Length, CaseSensitive); }
	inline Item& GetAt(UINT64 Position) { return *pRoot->GetAt(Position); }
	inline Item const& GetAt(UINT64 Position)const { return *pRoot->GetAt(Position); }

	// Modification
	inline Item* Add(CHAR const* Id, CHAR const* Value, UINT ValueLength=0, BOOL CaseSensitive=false)
		{
		Item* pitem=AddInternal(Id, 0, CaseSensitive);
		if(!pitem)
			return nullptr;
		new (pitem) Item(Id, 0, Value, ValueLength);
		return pitem;
		}
	inline CHAR const* ReleaseAt(UINT64 Position) { return ReleaseInternal<CHAR const*>(Position); }
	inline BOOL Remove(CHAR const* Id, UINT Length=0, BOOL CaseSensitive=false) { return RemoveInternal(Id, Length, CaseSensitive); }
	inline Item* Set(CHAR const* Id, CHAR const* Value, UINT ValueLength=0, BOOL CaseSensitive=false)
		{
		Item* pitem=pRoot->Get(Id, 0, CaseSensitive);
		if(pitem)
			{
			pitem->SetItem(Value, ValueLength);
			return pitem;
			}
		return Add(Id, Value, ValueLength, CaseSensitive);
		}
};

// Shared String-Index with Shared Strings
template<class CHAR_ID, class CHAR_ITEM, UINT _GroupSize>
class StringIndex<CHAR_ID, false, String<CHAR_ITEM, false>, _GroupSize>: public IndexBase<String<CHAR_ID, false>, String<CHAR_ITEM, false>, _GroupSize>
{
public:
	// Access
	inline CHAR const* operator[](CHAR const* Id)const { return GetInternal<CHAR const*, CHAR const*, UINT, BOOL>(Id, 0, false); }
	inline BOOL Contains(CHAR const* Id, UINT Length=0, BOOL CaseSensitive=false)const { return pRoot->Contains(Id, Length, CaseSensitive); }
	inline IteratorReadWrite Find(CHAR const* Id, UINT Length=0, BOOL CaseSensitive=false) { return IteratorReadWrite(this, Id, Length, CaseSensitive); }
	inline IteratorReadOnly Find(CHAR const* Id, UINT Length=0, BOOL CaseSensitive=false)const { return IteratorReadOnly(this, Id, Length, CaseSensitive); }
	inline CHAR const* Get(CHAR const* Id, UINT Length=0, BOOL CaseSensitive=false)const { return GetInternal<CHAR const*, CHAR const*, UINT, BOOL>(Id, Length, CaseSensitive); }
	inline Item& GetAt(UINT64 Position) { return *pRoot->GetAt(Position); }
	inline Item const& GetAt(UINT64 Position)const { return *pRoot->GetAt(Position); }

	// Modification
	inline Item* Add(CHAR const* Id, CHAR const* Value, BOOL CaseSensitive=false)
		{
		Item* pitem=AddInternal(Id, 0, CaseSensitive);
		if(!pitem)
			return nullptr;
		new (pitem) Item(Id, 0, Value, 0);
		return pitem;
		}
	inline CHAR const* ReleaseAt(UINT64 Position) { return ReleaseInternal<CHAR const*>(Position); }
	inline BOOL Remove(CHAR const* Id, UINT Length=0, BOOL CaseSensitive=false) { return RemoveInternal(Id, Length, CaseSensitive); }
	inline Item* Set(CHAR const* Id, CHAR const* Value, BOOL CaseSensitive=false)
		{
		Item* pitem=pRoot->Get(Id, 0, CaseSensitive);
		if(pitem)
			{
			pitem->SetItem(Item, 0);
			return pitem;
			}
		return Add(Id, Value, CaseSensitive);
		}
};

#pragma endregion

}}}
