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

#pragma region Index

// Index
template<class ID, class ITEM, UINT _GroupSize>
class Index: public IndexBase<ID, ITEM, _GroupSize>
{
public:
	// Access
	inline ITEM& operator[](ID const& Id) { return *GetInternalAddress<ID const&>(Id); }
	inline ITEM const& operator[](ID const& Id)const { return *GetInternalAddress<ID const&>(Id); }
	inline BOOL Contains(ID const& Id)const { return pRoot->Contains(Id); }
	inline IteratorReadWrite Find(ID const& Id) { return IteratorReadWrite(this, 0, Id); }
	inline IteratorReadOnly Find(ID const& Id)const { return IteratorReadOnly(this, 0, Id); }
	inline ITEM& Get(ID const& Id) { return *GetInternalAddress<ID const&>(Id); }
	inline ITEM const& Get(ID const& Id)const { return *GetInternalAddress<ID const&>(Id); }
	inline INDEXITEM& GetAt(UINT64 Position) { return *pRoot->GetAt(Position); }
	inline INDEXITEM const& GetAt(UINT64 Position)const { return *pRoot->GetAt(Position); }
	inline ITEM* TryGet(ID const& Id)const { return GetInternalAddress<ID const&>(Id); }

	// Modification
	INDEXITEM* Add(ID const& Id)
		{
		ScopedWrite lock(cAccessControl);
		INDEXITEM* pitem=AddInternal<ID const&>(Id);
		if(!pitem)
			return nullptr;
		new (pitem) INDEXITEM(Id);
		return pitem;
		}
	INDEXITEM* Add(ID const& Id, ITEM const& Value)
		{
		ScopedWrite lock(cAccessControl);
		INDEXITEM* pitem=AddInternal<ID const&>(Id);
		if(!pitem)
			return nullptr;
		new (pitem) INDEXITEM(Id, Value);
		return pitem;
		}
	inline BOOL Remove(ID const& Id) { return RemoveInternal<ID const&>(Id); }
	INDEXITEM* Set(ID const& Id, ITEM const& Value)
		{
		ScopedWrite lock(cAccessControl);
		INDEXITEM* pitem=pRoot->Get(Id);
		if(pitem)
			{
			pitem->SetItem(Value);
			return pitem;
			}
		pitem=AddInternal<ID const&>(Id);
		if(!pitem)
			return nullptr;
		new (pitem) INDEXITEM(Id, Value);
		return pitem;
		}
};

// Index without Items
template<class ID, UINT _GroupSize>
class Index<ID, VOID, _GroupSize>: public IndexBase<ID, VOID, _GroupSize>
{
public:
	// Access
	inline ID const& operator[](UINT64 Position)const { return pRoot->GetAt(Position)->GetId(); }
	inline BOOL Contains(ID const& Id)const { return pRoot->Contains(Id); }
	inline IteratorReadWrite Find(ID const& Id) { return IteratorReadWrite(this, 0, Id); }
	inline IteratorReadOnly Find(ID const& Id)const { return IteratorReadOnly(this, 0, Id); }
	inline ID const& GetAt(UINT64 Position)const { return pRoot->GetAt(Position)->GetId(); }

	// Modification
	INDEXITEM* Add(ID const& Id)
		{
		ScopedWrite lock(cAccessControl);
		INDEXITEM* pitem=AddInternal<ID const&>(Id);
		if(!pitem)
			return nullptr;
		new (pitem) INDEXITEM(Id);
		return pitem;
		}
	inline BOOL Remove(ID const& Id) { return RemoveInternal<ID const&>(Id); }
};

// Index with Pointer-Items
template<class ID, class ITEM, UINT _GroupSize>
class Index<ID, ITEM*, _GroupSize>: public IndexBase<ID, ITEM*, _GroupSize>
{
public:
	// Access
	inline ITEM* operator[](ID const& Id)const { return GetIternal<ITEM*, ID const& Id>(Id); }
	inline BOOL Contains(ID const& Id)const { return pRoot->Contains(Id); }
	inline IteratorReadWrite Find(ID const& Id) { return IteratorReadWrite(this, 0, Id); }
	inline IteratorReadOnly Find(ID const& Id)const { return IteratorReadOnly(this, 0, Id); }
	inline ITEM* Get(ID const& Id)const { return GetInternal<ITEM*, ID const& Id>(Id); }
	inline INDEXITEM& GetAt(UINT64 Position) { return *pRoot->GetAt(Position); }
	inline INDEXITEM const& GetAt(UINT64 Position)const { return *pRoot->GetAt(Position); }

	// Modification
	INDEXITEM* Add(ID const& Id, ITEM* Value)
		{
		ScopedWrite lock(cAccessControl);
		INDEXITEM* pitem=AddInternal<ID const&>(Id);
		if(!pitem)
			return nullptr;
		new (pitem) INDEXITEM(Id, Value);
		return pitem;
		}
	inline ITEM* ReleaseAt(UINT64 Position) { return ReleaseInternal<ITEM*>(Position); }
	inline BOOL Remove(ID const& Id) { return RemoveInternal<ID const&>(Id); }
	INDEXITEM* Set(ID const& Id, ITEM* Value)
		{
		ScopedWrite lock(cAccessControl);
		INDEXITEM* pitem=pRoot->Get(Id);
		if(pitem)
			{
			pitem->SetItem(Value);
			return pitem;
			}
		pitem=AddInternal<ID const&>(Id);
		if(!pitem)
			return nullptr;
		new (pitem) INDEXITEM(Id, Value);
		return pitem;
		}
};

#ifdef __cplusplus_winrt
// Index with Handle-Items
template<class ID, class ITEM, UINT _GroupSize>
class Index<ID, ITEM^, _GroupSize>: public IndexBase<ID, ITEM^, _GroupSize>
{
public:
	// Access
	inline ITEM^ operator[](ID const& Id)const { return GetInternal<ITEM^, ID const&>(Id); }
	inline BOOL Contains(ID const& Id)const { return pRoot->Contains(Id); }
	inline IteratorReadWrite Find(ID const& Id) { return IteratorReadWrite(this, 0, Id); }
	inline IteratorReadOnly Find(ID const& Id)const { return IteratorReadOnly(this, 0, Id); }
	inline ITEM^ Get(ID const& Id)const { return GetInternal<ITEM^, ID const&>(Id); }
	inline INDEXITEM& GetAt(UINT64 Position) { return *pRoot->GetAt(Position); }
	inline INDEXITEM const& GetAt(UINT64 Position)const { return *pRoot->GetAt(Position); }

	// Modification
	INDEXITEM* Add(ID const& Id, ITEM^ Value)
		{
		ScopedWrite lock(cAccessControl);
		INDEXITEM* pitem=AddInternal<ID const&>(Id);
		if(!pitem)
			return nullptr;
		new (pitem) INDEXITEM(Id, Value);
		return pitem;
		}
	inline BOOL Remove(ID const& Id) { return RemoveInternal<ID const&>(Id); }
	INDEXITEM* Set(ID const& Id, ITEM^ Value)
		{
		ScopedWrite lock(cAccessControl);
		INDEXITEM* pitem=pRoot->Get(Id);
		if(pitem)
			{
			pitem->SetItem(Value);
			return pitem;
			}
		pitem=AddInternal<ID const&>(Id);
		if(!pitem)
			return nullptr;
		new (pitem) INDEXITEM(Id, Value);
		return pitem;
		}
};
#endif

// Index with String-Items
template<class ID, class CHAR, UINT _GroupSize>
class Index<ID, String<CHAR, true>, _GroupSize>: public IndexBase<ID, String<CHAR, true>, _GroupSize>
{
public:
	// Access
	inline CHAR const* operator[](ID const& Id)const { return GetInternal<CHAR const*, ID const&>(Id); }
	inline BOOL Contains(ID const& Id)const { return pRoot->Contains(Id); }
	inline IteratorReadWrite Find(ID const& Id) { return IteratorReadWrite(this, 0, Id); }
	inline IteratorReadOnly Find(ID const& Id)const { return IteratorReadOnly(this, 0, Id); }
	inline CHAR const* Get(ID const& Id)const { return GetInternal<CHAR const*, ID const&>(Id); }
	inline INDEXITEM& GetAt(UINT64 Position) { return *pRoot->GetAt(Position); }
	inline INDEXITEM const& GetAt(UINT64 Position)const { return *pRoot->GetAt(Position); }

	// Modification
	INDEXITEM* Add(ID const& Id, CHAR const* Value, UINT Length=0)
		{
		ScopedWrite lock(cAccessControl);
		INDEXITEM* pitem=AddInternal<ID const&>(Id);
		if(!pitem)
			return nullptr;
		new (pitem) INDEXITEM(Id, Value, Length);
		return pitem;
		}
	inline CHAR const* ReleaseAt(UINT64 Position) { return ReleaseInternal<CHAR const*>(Position); }
	inline BOOL Remove(ID const& Id) { return RemoveInternal<ID const&>(Id); }
	INDEXITEM* Set(ID const& Id, CHAR const* Value, UINT Length=0)
		{
		ScopedWrite lock(cAccessControl);
		INDEXITEM* pitem=pRoot->Get(Id);
		if(pitem)
			{
			pitem->SetItem(Value, Length);
			return pitem;
			}
		pitem=AddInternal<ID const&>(Id);
		if(!pitem)
			return nullptr;
		new (pitem) INDEXITEM(Id, Value, Length);
		return pitem;
		}
};

// Index with Shared String-Items
template<class ID, class CHAR, UINT _GroupSize>
class Index<ID, String<CHAR, false>, _GroupSize>: public IndexBase<ID, String<CHAR, false>, _GroupSize>
{
public:
	// Access
	inline CHAR const* operator[](ID const& Id)const { return GetInternal<CHAR const*, ID const&>(Id); }
	inline BOOL Contains(ID const& Id)const { return pRoot->Contains(Id); }
	inline IteratorReadWrite Find(ID const& Id) { return IteratorReadWrite(this, 0, Id); }
	inline IteratorReadOnly Find(ID const& Id)const { return IteratorReadOnly(this, 0, Id); }
	inline CHAR const* Get(ID const& Id)const { return GetInternal<CHAR const*, ID const&>(Id); }
	inline INDEXITEM& GetAt(UINT64 Position) { return *pRoot->GetAt(Position); }
	inline INDEXITEM const& GetAt(UINT64 Position)const { return *pRoot->GetAt(Position); }

	// Modification
	INDEXITEM* Add(ID const& Id, CHAR const* Value)
		{
		ScopedWrite lock(cAccessControl);
		INDEXITEM* pitem=AddInternal<ID const&>(Id);
		if(!pitem)
			return nullptr;
		new (pitem) INDEXITEM(Id, Value);
		return pitem;
		}
	inline CHAR const* ReleaseAt(UINT64 Position) { return ReleaseInternal<CHAR const*>(Position); }
	inline BOOL Remove(ID const& Id) { return RemoveInternal<ID const&>(Id); }
	INDEXITEM* Set(ID const& Id, CHAR const* Value)
		{
		ScopedWrite lock(cAccessControl);
		INDEXITEM* pitem=pRoot->Get(Id);
		if(pitem)
			{
			pitem->SetItem(Value);
			return pitem;
			}
		pitem=AddInternal<ID const&>(Id);
		if(!pitem)
			return nullptr;
		new (pitem) INDEXITEM(Id, Value);
		return pitem;
		}
};

#pragma endregion


//===============
// Pointer-Index
//===============

#pragma region Pointer-Index

// Pointer-Index
template<class ID, class ITEM, UINT _GroupSize>
class Index<ID*, ITEM, _GroupSize>: public IndexBase<ID*, ITEM, _GroupSize>
{
public:
	// Access
	inline ITEM& operator[](ID* Id) { return *GetInternalAddress(Id); }
	inline ITEM const& operator[](ID* Id)const { return *GetInternalAddress(Id); }
	inline BOOL Contains(ID* Id)const { return pRoot->Contains(Id); }
	inline IteratorReadWrite Find(ID* Id) { return IteratorReadWrite(this, Id); }
	inline IteratorReadOnly Find(ID* Id)const { return IteratorReadOnly(this, Id); }
	inline ITEM& Get(ID* Id) { return *GetInternalAddress(Id); }
	inline ITEM const& Get(ID* Id)const { return *GetInternalAddress(Id); }
	inline INDEXITEM& GetAt(UINT64 Position) { return *pRoot->GetAt(Position); }
	inline INDEXITEM const& GetAt(UINT64 Position)const { return *pRoot->GetAt(Position); }
	inline ITEM* TryGet(ID* Id)const { return GetInternalAdd(Id); }

	// Modification
	INDEXITEM* Add(ID* Id)
		{
		ScopedWrite lock(cAccessControl);
		INDEXITEM* pitem=AddInternal(Id);
		if(!pitem)
			return nullptr;
		new (pitem) INDEXITEM(Id);
		return pitem;
		}
	INDEXITEM* Add(ID* Id, ITEM const& Value)
		{
		ScopedWrite lock(cAccessControl);
		INDEXITEM* pitem=AddInternal(Id);
		if(!pitem)
			return nullptr;
		new (pitem) INDEXITEM(Id, Value);
		return pitem;
		}
	inline BOOL Remove(ID* Id) { return RemoveInternal(Id); }
	INDEXITEM* Set(ID* Id, ITEM const& Value)
		{
		ScopedWrite lock(cAccessControl);
		INDEXITEM* pitem=pRoot->Get(Id);
		if(pitem)
			{
			pitem->SetItem(Value);
			return pitem;
			}
		pitem=AddInternal(Id);
		if(!pitem)
			return nullptr;
		new (pitem) INDEXITEM(Id, Value);
		return pitem;
		}
};

// Pointer-Index without Items
template<class ID, UINT _GroupSize>
class Index<ID*, VOID, _GroupSize>: public IndexBase<ID*, VOID, _GroupSize>
{
public:
	// Access
	inline ID* operator[](UINT64 Position)const { return pRoot->GetAt(Position)->GetItem(); }
	inline BOOL Contains(ID* Id)const { return pRoot->Contains(Id); }
	inline IteratorReadWrite Find(ID* Id) { return IteratorReadWrite(this, Id); }
	inline IteratorReadOnly Find(ID* Id)const { return IteratorReadOnly(this, Id); }
	inline ID* GetAt(UINT64 Position)const { return pRoot->GetAt(Position)->GetItem(); }

	// Modification
	INDEXITEM* Add(ID* Id)
		{
		ScopedWrite lock(cAccessControl);
		INDEXITEM* pitem=AddInternal(Id);
		if(!pitem)
			return nullptr;
		new (pitem) INDEXITEM(Id);
		return pitem;
		}
	inline BOOL Remove(ID* Id) { return RemoveInternal(Id); }
};

// Pointer-Index with Pointer-Items
template<class ID, class ITEM, UINT _GroupSize>
class Index<ID*, ITEM*, _GroupSize>: public IndexBase<ID*, ITEM*, _GroupSize>
{
public:
	// Access
	inline ITEM* operator[](ID* Id)const { return GetInternal<ITEM*, ID*>(Id); }
	inline BOOL Contains(ID* Id)const { return pRoot->Contains(Id); }
	inline IteratorReadWrite Find(ID* Id) { return IteratorReadWrite(this, Id); }
	inline IteratorReadOnly Find(ID* Id)const { return IteratorReadOnly(this, Id); }
	inline ITEM* Get(ID* Id)const { return GetInternal<ITEM*, ID*>(Id); }
	inline INDEXITEM& GetAt(UINT64 Position) { return *pRoot->GetAt(Position); }
	inline INDEXITEM const& GetAt(UINT64 Position)const { return *pRoot->GetAt(Position); }

	// Modification
	INDEXITEM* Add(ID* Id, ITEM* Value)
		{
		ScopedWrite lock(cAccessControl);
		INDEXITEM* pitem=AddInternal(Id);
		if(!pitem)
			return nullptr;
		new (pitem) INDEXITEM(Id, Value);
		return pitem;
		}
	inline ITEM* ReleaseAt(UINT64 Position) { return ReleaseInternal<ITEM*>(Position); }
	inline BOOL Remove(ID* Id) { return RemoveInternal(Id); }
	INDEXITEM* Set(ID* Id, ITEM* Value)
		{
		ScopedWrite lock(cAccessControl);
		INDEXITEM* pitem=pRoot->Get(Id);
		if(pitem)
			{
			pitem->SetItem(Value);
			return pitem;
			}
		pitem=AddInternal(Id);
		if(!pitem)
			return nullptr;
		new (pitem) INDEXITEM(Id, Value);
		return pitem;
		}
};

#ifdef __cplusplus_winrt
// Pointer-Index with Handle-Items
template<class ID, class ITEM, UINT _GroupSize>
class Index<ID*, ITEM^, _GroupSize>: public IndexBase<ID*, ITEM^, _GroupSize>
{
public:
	// Access
	inline ITEM^ operator[](ID* Id)const { return GetInternal<ITEM^, ID*>(Id); }
	inline BOOL Contains(ID* Id)const { return pRoot->Contains(Id); }
	inline IteratorReadWrite Find(ID* Id) { return IteratorReadWrite(this, Id); }
	inline IteratorReadOnly Find(ID* Id)const { return IteratorReadOnly(this, Id); }
	inline ITEM^ Get(ID* Id)const { return GetInternal<ITEM^, ID*>(Id); }
	inline INDEXITEM& GetAt(UINT64 Position) { return *pRoot->GetAt(Position); }
	inline INDEXITEM const& GetAt(UINT64 Position)const { return *pRoot->GetAt(Position); }

	// Modification
	INDEXITEM* Add(ID* Id, ITEM^ Value)
		{
		ScopedWrite lock(cAccessControl);
		INDEXITEM* pitem=AddInternal(Id);
		if(!pitem)
			return nullptr;
		new (pitem) INDEXITEM(Id, Value);
		return pitem;
		}
	inline BOOL Remove(ID* Id) { return RemoveInternal(Id); }
	INDEXITEM* Set(ID* Id, ITEM^ Value)
		{
		ScopedWrite lock(cAccessControl);
		INDEXITEM* pitem=pRoot->Get(Id);
		if(pitem)
			{
			pitem->SetItem(Value);
			return pitem;
			}
		pitem=AddInternal(Id);
		if(!pitem)
			return nullptr;
		new (pitem) INDEXITEM(Id, Value);
		return pitem;
		}
};
#endif

// Pointer-Index with String-Items
template<class ID, class CHAR, UINT _GroupSize>
class Index<ID*, String<CHAR, true>, _GroupSize>: public IndexBase<ID*, String<CHAR, true>, _GroupSize>
{
public:
	// Access
	inline CHAR const* operator[](ID* Id)const { return GetInternal<CHAR const*, ID*>(Id); }
	inline BOOL Contains(ID* Id)const { return pRoot->Contains(Id); }
	inline IteratorReadWrite Find(ID* Id) { return IteratorReadWrite(this, Id); }
	inline IteratorReadOnly Find(ID* Id)const { return IteratorReadOnly(this, Id); }
	inline CHAR const* Get(ID* Id)const { return GetInternal<CHAR const*, ID*>(Id); }
	inline INDEXITEM& GetAt(UINT64 Position) { return *pRoot->GetAt(Position); }
	inline INDEXITEM const& GetAt(UINT64 Position)const { return *pRoot->GetAt(Position); }

	// Modification
	INDEXITEM* Add(ID* Id, CHAR const* Value, UINT Length=0)
		{
		ScopedWrite lock(cAccessControl);
		INDEXITEM* pitem=AddInternal(Id);
		if(!pitem)
			return nullptr;
		new (pitem) INDEXITEM(Id, Value, Length);
		return pitem;
		}
	inline CHAR const* ReleaseAt(UINT64 Position) { return ReleaseInternal<CHAR const*>(Position); }
	inline BOOL Remove(ID* Id) { return RemoveInternal(Id); }
	INDEXITEM* Set(ID* Id, CHAR const* Value, UINT Length=0)
		{
		ScopedWrite lock(cAccessControl);
		INDEXITEM* pitem=pRoot->Get(Id);
		if(pitem)
			{
			pitem->SetItem(Value, Length);
			return pitem;
			}
		pitem=AddInternal(Id);
		if(!pitem)
			return nullptr;
		new (pitem) INDEXITEM(Id, Value, Length);
		return pitem;
		}
};

// Pointer-Index with Shared String-Items
template<class ID, class CHAR, UINT _GroupSize>
class Index<ID*, String<CHAR, false>, _GroupSize>: public IndexBase<ID*, String<CHAR, false>, _GroupSize>
{
public:
	// Access
	inline CHAR const* operator[](ID* Id)const { return GetInternal<CHAR const*, ID*>(Id); }
	inline BOOL Contains(ID* Id)const { return pRoot->Contains(Id); }
	inline IteratorReadWrite Find(ID* Id) { return IteratorReadWrite(this, Id); }
	inline IteratorReadOnly Find(ID* Id)const { return IteratorReadOnly(this, Id); }
	inline CHAR const* Get(ID* Id)const { return GetInternal<CHAR const*, ID*>(Id); }
	inline INDEXITEM& GetAt(UINT64 Position) { return *pRoot->GetAt(Position); }
	inline INDEXITEM const& GetAt(UINT64 Position)const { return *pRoot->GetAt(Position); }

	// Modification
	INDEXITEM* Add(ID* Id, CHAR const* Value)
		{
		ScopedWrite lock(cAccessControl);
		INDEXITEM* pitem=AddInternal(Id);
		if(!pitem)
			return nullptr;
		new (pitem) INDEXITEM(Id, Value);
		return pitem;
		}
	inline CHAR const* ReleaseAt(UINT64 Position) { return ReleaseInternal<CHAR const*>(Position); }
	inline BOOL Remove(ID* Id) { return RemoveInternal(Id); }
	INDEXITEM* Set(ID Id, CHAR const* Value)
		{
		ScopedWrite lock(cAccessControl);
		INDEXITEM* pitem=pRoot->Get(Id);
		if(pitem)
			{
			pitem->SetItem(Value);
			return pitem;
			}
		pitem=AddInternal(Id);
		if(!pitem)
			return nullptr;
		new (pitem) INDEXITEM(Id, Value);
		return pitem;
		}
};

#pragma endregion


//==============
// Handle-Index
//==============

#pragma region Handle-Index

#ifdef __cplusplus_winrt

// Handle-Index
template<class ID, class ITEM, UINT _GroupSize>
class Index<ID^, ITEM, _GroupSize>: public IndexBase<ID^, ITEM, _GroupSize>
{
public:
	// Access
	inline ITEM& operator[](ID^ Id) { return *GetInternalAddress(Id); }
	inline ITEM const& operator[](ID^ Id)const { return *GetInternalAddress(Id); }
	inline BOOL Contains(ID^ Id)const { return pRoot->Contains(Id); }
	inline IteratorReadWrite Find(ID^ Id) { return IteratorReadWrite(this, Id); }
	inline IteratorReadOnly Find(ID^ Id)const { return IteratorReadOnly(this, Id); }
	inline ITEM& Get(ID^ Id) { return *GetInternalAddress(Id); }
	inline ITEM const& Get(ID^ Id)const { return *GetInternalAddress(Id); }
	inline INDEXITEM& GetAt(UINT64 Position) { return *pRoot->GetAt(Position); }
	inline INDEXITEM const& GetAt(UINT64 Position)const { return *pRoot->GetAt(Position); }
	inline ITEM* TryGet(ID^ Id)const { return GetInternalAdd(Id); }

	// Modification
	INDEXITEM* Add(ID^ Id)
		{
		ScopedWrite lock(cAccessControl);
		INDEXITEM* pitem=AddInternal(Id);
		if(!pitem)
			return nullptr;
		new (pitem) INDEXITEM(Id);
		return pitem;
		}
	INDEXITEM* Add(ID^ Id, ITEM const& Value)
		{
		ScopedWrite lock(cAccessControl);
		INDEXITEM* pitem=AddInternal(Id);
		if(!pitem)
			return nullptr;
		new (pitem) INDEXITEM(Id, Value);
		return pitem;
		}
	inline BOOL Remove(ID^ Id) { return RemoveInternal(Id); }
	INDEXITEM* Set(ID^ Id, ITEM const& Value)
		{
		ScopedWrite lock(cAccessControl);
		INDEXITEM* pitem=pRoot->Get(Id);
		if(pitem)
			{
			pitem->SetItem(Value);
			return pitem;
			}
		pitem=AddInternal(Id);
		if(!pitem)
			return nullptr;
		new (pitem) INDEXITEM(Id, Value);
		return pitem;
		}
};

// Handle-Index without Items
template<class ID, UINT _GroupSize>
class Index<ID^, VOID, _GroupSize>: public IndexBase<ID^, VOID, _GroupSize>
{
public:
	// Access
	inline ID^ operator[](UINT64 Position)const { return pRoot->GetAt(Position)->GetItem(); }
	inline BOOL Contains(ID^ Id)const { return pRoot->Contains(Id); }
	inline IteratorReadWrite Find(ID^ Id) { return IteratorReadWrite(this, Id); }
	inline IteratorReadOnly Find(ID^ Id)const { return IteratorReadOnly(this, Id); }
	inline ID^ GetAt(UINT64 Position)const { return pRoot->GetAt(Position)->GetItem(); }

	// Modification
	INDEXITEM* Add(ID^ Id)
		{
		ScopedWrite lock(cAccessControl);
		INDEXITEM* pitem=AddInternal(Id);
		if(!pitem)
			return nullptr;
		new (pitem) INDEXITEM(Id);
		return pitem;
		}
	inline BOOL Remove(ID^ Id) { return RemoveInternal(Id); }
};

// Handle-Index with Pointer-Items
template<class ID, class ITEM, UINT _GroupSize>
class Index<ID^, ITEM*, _GroupSize>: public IndexBase<ID^, ITEM*, _GroupSize>
{
public:
	// Access
	inline ITEM* operator[](ID^ Id)const { return GetInternal<ITEM*, ID^>(Id); }
	inline BOOL Contains(ID^ Id)const { return pRoot->Contains(Id); }
	inline IteratorReadWrite Find(ID^ Id) { return IteratorReadWrite(this, Id); }
	inline IteratorReadOnly Find(ID^ Id)const { return IteratorReadOnly(this, Id); }
	inline ITEM* Get(ID^ Id)const { return GetInternal<ITEM*, ID^>(Id); }
	inline INDEXITEM& GetAt(UINT64 Position) { return *pRoot->GetAt(Position); }
	inline INDEXITEM const& GetAt(UINT64 Position)const { return *pRoot->GetAt(Position); }

	// Modification
	INDEXITEM* Add(ID^ Id, ITEM* Value)
		{
		ScopedWrite lock(cAccessControl);
		INDEXITEM* pitem=AddInternal(Id);
		if(!pitem)
			return nullptr;
		new (pitem) INDEXITEM(Id, Value);
		return pitem;
		}
	inline ITEM* ReleaseAt(UINT64 Position) { return ReleaseInternal<ITEM*>(Position); }
	inline BOOL Remove(ID^ Id) { return RemoveInternal(Id); }
	INDEXITEM* Set(ID^ Id, ITEM* Value)
		{
		ScopedWrite lock(cAccessControl);
		INDEXITEM* pitem=pRoot->Get(Id);
		if(pitem)
			{
			pitem->SetItem(Value);
			return pitem;
			}
		pitem=AddInternal(Id);
		if(!pitem)
			return nullptr;
		new (pitem) INDEXITEM(Id, Value);
		return pitem;
		}
};

// Handle-Index with Handle-Items
template<class ID, class ITEM, UINT _GroupSize>
class Index<ID^, ITEM^, _GroupSize>: public IndexBase<ID^, ITEM^, _GroupSize>
{
public:
	// Access
	inline ITEM^ operator[](ID^ Id)const { return GetInternal<ITEM^, ID^>(Id); }
	inline BOOL Contains(ID^ Id)const { return pRoot->Contains(Id); }
	inline IteratorReadWrite Find(ID^ Id) { return IteratorReadWrite(this, Id); }
	inline IteratorReadOnly Find(ID^ Id)const { return IteratorReadOnly(this, Id); }
	inline ITEM^ Get(ID^ Id)const { return GetInternal<ITEM^, ID^>(Id); }
	inline INDEXITEM& GetAt(UINT64 Position) { return *pRoot->GetAt(Position); }
	inline INDEXITEM const& GetAt(UINT64 Position)const { return *pRoot->GetAt(Position); }

	// Modification
	INDEXITEM* Add(ID^ Id, ITEM^ Value)
		{
		ScopedWrite lock(cAccessControl);
		INDEXITEM* pitem=AddInternal(Id);
		if(!pitem)
			return nullptr;
		new (pitem) INDEXITEM(Id, Value);
		return pitem;
		}
	inline BOOL Remove(ID^ Id) { return RemoveInternal(Id); }
	INDEXITEM* Set(ID^ Id, ITEM^ Value)
		{
		ScopedWrite lock(cAccessControl);
		INDEXITEM* pitem=pRoot->Get(Id);
		if(pitem)
			{
			pitem->SetItem(Value);
			return pitem;
			}
		pitem=AddInternal(Id);
		if(!pitem)
			return nullptr;
		new (pitem) INDEXITEM(Id, Value);
		return pitem;
		}
};

// Handle-Index with String-Items
template<class ID, class CHAR, UINT _GroupSize>
class Index<ID^, String<CHAR, true>, _GroupSize>: public IndexBase<ID^, String<CHAR, true>, _GroupSize>
{
public:
	// Access
	inline CHAR const* operator[](ID^ Id)const { return GetInternal<CHAR const*, ID^>(Id); }
	inline BOOL Contains(ID^ Id)const { return pRoot->Contains(Id); }
	inline IteratorReadWrite Find(ID^ Id) { return IteratorReadWrite(this, Id); }
	inline IteratorReadOnly Find(ID^ Id)const { return IteratorReadOnly(this, Id); }
	inline CHAR const* Get(ID^ Id)const { return GetInternal<CHAR const*, ID^>(Id); }
	inline INDEXITEM& GetAt(UINT64 Position) { return *pRoot->GetAt(Position); }
	inline INDEXITEM const& GetAt(UINT64 Position)const { return *pRoot->GetAt(Position); }

	// Modification
	INDEXITEM* Add(ID^ Id, CHAR const* Value, UINT Length=0)
		{
		ScopedWrite lock(cAccessControl);
		INDEXITEM* pitem=AddInternal(Id);
		if(!pitem)
			return nullptr;
		new (pitem) INDEXITEM(Id, Value, Length);
		return pitem;
		}
	inline CHAR const* ReleaseAt(UINT64 Position) { return ReleaseInternal<CHAR const*>(Position); }
	inline BOOL Remove(ID^ Id) { return RemoveInternal(Id); }
	INDEXITEM* Set(ID^ Id, CHAR const* Value, UINT Length=0)
		{
		ScopedWrite lock(cAccessControl);
		INDEXITEM* pitem=pRoot->Get(Id);
		if(pitem)
			{
			pitem->SetItem(Value, Length);
			return pitem;
			}
		pitem=AddInternal(Id);
		if(!pitem)
			return nullptr;
		new (pitem) INDEXITEM(Id, Value, Length);
		return pitem;
		}
};

// Handle-Index with Shared String-Items
template<class ID, class CHAR, UINT _GroupSize>
class Index<ID^, String<CHAR, false>, _GroupSize>: public IndexBase<ID^, String<CHAR, false>, _GroupSize>
{
public:
	// Access
	inline CHAR const* operator[](ID^ Id)const { return GetInternal<CHAR const*, ID^>(Id); }
	inline BOOL Contains(ID^ Id)const { return pRoot->Contains(Id); }
	inline IteratorReadWrite Find(ID^ Id) { return IteratorReadWrite(this, Id); }
	inline IteratorReadOnly Find(ID^ Id)const { return IteratorReadOnly(this, Id); }
	inline CHAR const* Get(ID^ Id)const { return GetInternal<CHAR const*, ID^>(Id); }
	inline INDEXITEM& GetAt(UINT64 Position) { return *pRoot->GetAt(Position); }
	inline INDEXITEM const& GetAt(UINT64 Position)const { return *pRoot->GetAt(Position); }

	// Modification
	INDEXITEM* Add(ID^ Id, CHAR const* Value)
		{
		ScopedWrite lock(cAccessControl);
		INDEXITEM* pitem=AddInternal(Id);
		if(!pitem)
			return nullptr;
		new (pitem) INDEXITEM(Id, Value);
		return pitem;
		}
	inline CHAR const* ReleaseAt(UINT64 Position) { return ReleaseInternal<CHAR const*>(Position); }
	inline BOOL Remove(ID^ Id) { return RemoveInternal(Id); }
	INDEXITEM* Set(ID^ Id, CHAR const* Value)
		{
		ScopedWrite lock(cAccessControl);
		INDEXITEM* pitem=pRoot->Get(Id);
		if(pitem)
			{
			pitem->SetItem(Value);
			return pitem;
			}
		pitem=AddInternal(Id);
		if(!pitem)
			return nullptr;
		new (pitem) INDEXITEM(Id, Value);
		return pitem;
		}
};

#endif

#pragma endregion


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
