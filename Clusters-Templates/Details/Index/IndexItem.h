//=============
// IndexItem.h
//=============

#pragma once


//=======
// Using
//=======

#include "..\StringClass.h"


//===========
// Namespace
//===========

namespace Clusters {
	namespace Templates {
		namespace Details {
			namespace Index {


//============
// Item Index
//============

// Index-Item
template <class ID, class ITEM>
class IndexItem
{
public:
	// Con-/Destructors
	IndexItem(ID const& Id, ITEM const& Item): tId(Id), tItem(Item) {}

	// Access
	inline ID const& GetId()const { return tId; }
	inline ITEM& GetItem() { return tItem; }
	inline ITEM const& GetItem()const { return tItem; }

	// Modification
	inline VOID SetItem(ITEM const& Item) { tItem=Item; }
	
private:
	// Common
	ID tId;
	ITEM tItem;
};

// Empty Index-Item
template <class ID>
class IndexItem<ID, VOID>
{
public:
	// Con-/Destructors
	IndexItem(ID const& Id): tId(Id) {}

	// Access
	inline ID const& GetId()const { return tId; }
	
private:
	// Common
	ID tId;
};

// Index-Item with Pointer
template <class ID, class ITEM>
class IndexItem<ID, ITEM*>
{
public:
	// Con-/Destructors
	IndexItem(ID const& Id, ITEM* Item): pItem(Item), tId(Id) {}
	~IndexItem() { delete pItem; }

	// Access
	inline ID const& GetId()const { return tId; }
	inline ITEM* GetItem()const { return pItem; }

	// Modification
	inline ITEM* ReleaseItem() { return PointerRelease(&pItem); }
	inline VOID SetItem(ITEM* Item) { PointerAssign(&pItem, Item); }
	
private:
	// Common
	ITEM* pItem;
	ID tId;
};

#ifdef __cplusplus_winrt
// Index-Item with Handle
template <class ID, class ITEM>
class IndexItem<ID, ITEM^>
{
public:
	// Con-/Destructors
	IndexItem(ID const& Id, ITEM^ Item): hItem(Item), tId(Id) {}
	~IndexItem() { hItem=nullptr; }

	// Access
	inline ID const& GetId()const { return tId; }
	inline ITEM^ GetItem()const { return hItem; }

	// Modification
	inline VOID SetItem(ITEM^ Item) { hItem=Item; }
	
private:
	// Common
	ITEM^ hItem;
	ID tId;
};
#endif

// Index-Item with String
template <class ID, class CHAR>
class IndexItem<ID, String<CHAR, true>>
{
public:
	// Con-/Destructors
	IndexItem(ID const& Id, CHAR const* Item, UINT Length): cItem(Item, Length), tId(Id) {}

	// Access
	inline ID const& GetId()const { return tId; }
	inline CHAR const* GetItem()const { return cItem.Get(); }

	// Modification
	inline CHAR const* ReleaseItem() { return cItem.Release(); }
	inline VOID SetItem(LPCSTR Value) { cItem.Set(Value, Length); }
	
private:
	// Common
	String<CHAR, true> cItem;
	ID tId;
};

// Index-Item with Shared String
template <class ID, class CHAR>
class IndexItem<ID, String<CHAR, false>>
{
public:
	// Con-/Destructors
	IndexItem(ID const& Id, CHAR const* Item): cItem(Item), tId(Id) {}

	// Access
	inline ID const& GetId()const { return tId; }
	inline CHAR const* GetItem()const { return cItem.Get(); }

	// Modification
	inline VOID SetItem(LPCSTR Value) { cItem.Set(Value); }
	
private:
	// Common
	String<CHAR, false> cItem;
	ID tId;
};


//====================
// Item Pointer-Index
//====================

// Item Pointer-Index
template <class ID, class ITEM>
class IndexItem<ID*, ITEM>
{
public:
	// Con-/Destructors
	IndexItem(ID* Id, ITEM const& Item): pId(Id), tItem(Item) { ASSERT(Id); }
	~IndexItem() { delete pId; }

	// Access
	inline ID* GetId()const { return pId; }
	inline ITEM& GetItem() { return tItem; }
	inline ITEM const& GetItem()const { return tItem; }

	// Modification
	inline VOID SetItem(ITEM const& Item) { tItem=Item; }
	
private:
	// Common
	ID* pId;
	ITEM tItem;
};

// Empty Item Pointer-Index
template <class ID>
class IndexItem<ID*, VOID>
{
public:
	// Con-/Destructors
	IndexItem(ID* Id): pId(Id) { ASSERT(Id); }
	~IndexItem() { delete pId; }

	// Access
	inline ID* GetId()const { return pId; }

private:
	// Common
	ID* pId;
};

// Item Pointer-Index with Pointer
template <class ID, class ITEM>
class IndexItem<ID*, ITEM*>
{
public:
	// Con-/Destructors
	IndexItem(ID* Id, ITEM* Item): pId(Id), pItem(Item) { ASSERT(Id); }
	~IndexItem()
		{
		delete pId;
		delete pItem;
		}

	// Access
	inline ID* GetId()const { return pId; }
	inline ITEM* GetItem()const { return pItem; }

	// Modification
	inline ITEM* ReleaseItem() { return PointerRelease(&pItem); }
	inline VOID SetItem(ITEM* Item) { PointerAssign(&pItem, Item); }
	
private:
	// Common
	ID* pId;
	ITEM* pItem;
};

#ifdef __cplusplus_winrt
// Item Pointer-Index with Handle
template <class ID, class ITEM>
class IndexItem<ID*, ITEM^>
{
public:
	// Con-/Destructors
	IndexItem(ID* Id, ITEM^ Item): pId(Id), hItem(Item) { ASSERT(pId); }
	~IndexItem()
		{
		delete pId;
		hItem=nullptr;
		}

	// Access
	inline ID* GetId()const { return pId; }
	inline ITEM^ GetItem()const { return hItem; }

	// Modification
	inline VOID SetItem(ITEM^ Item) { hItem=Item; }
	
private:
	// Common
	ID* pId;
	ITEM^ hItem;
};
#endif

// Item Pointer-Index with String
template <class ID, class CHAR>
class IndexItem<ID*, String<CHAR, true>>
{
public:
	// Con-/Destructors
	IndexItem(ID* Id, CHAR const* Item, UINT Length): cItem(Item, Length), pId(Id) { ASSERT(pId); }
	~IndexItem() { delete pId; }

	// Access
	inline ID* GetId()const { return pId; }
	inline CHAR const* GetItem()const { return cItem.Get(); }

	// Modification
	inline CHAR const* ReleaseItem() { return cItem.Release(); }
	inline VOID SetItem(CHAR const* Value, UINT Length) { cItem.Set(Value, Length); }
	
private:
	// Common
	String<CHAR, true> cItem;
	ID* pId;
};

// Item Pointer-Index with Shared String
template <class ID, class CHAR>
class IndexItem<ID*, String<CHAR, false>>
{
public:
	// Con-/Destructors
	IndexItem(ID* Id, CHAR const* Item): cItem(Item), pId(Id) { ASSERT(pId); }
	~IndexItem() { delete pId; }

	// Access
	inline ID* GetId()const { return pId; }
	inline CHAR const* GetItem()const { return cItem.Get(); }

	// Modification
	inline VOID SetItem(CHAR const* Value) { cItem.Set(Value); }
	
private:
	// Common
	String<CHAR, true> cItem;
	ID* pId;
};


//===================
// Item Handle-Index
//===================

#ifdef __cplusplus_winrt

// Item Handle-Index
template <class ID, class ITEM>
class IndexItem<ID^, ITEM>
{
public:
	// Con-/Destructors
	IndexItem(ID^ Id, ITEM const& Item): hId(Id), tItem(Item) { ASSERT(Id); }
	~IndexItem() { hId=nullptr; }

	// Access
	inline ID^ GetId()const { return hId; }
	inline ITEM& GetItem() { return tItem; }
	inline ITEM const& GetItem()const { return tItem; }

	// Modification
	inline VOID SetItem(ITEM const& Item) { tItem=Item; }
	
private:
	// Common
	ID^ hId;
	ITEM tItem;
};

// Empty Item Handle-Index
template <class ID>
class IndexItem<ID^, VOID>
{
public:
	// Con-/Destructors
	IndexItem(ID^ Id): hId(Id) { ASSERT(hId); }
	~IndexItem() { hId=nullptr; }

	// Access
	inline ID^ GetId()const { return hId; }
	
private:
	// Common
	ID^ hId;
};

// Item Handle-Index with Pointer
template <class ID, class ITEM>
class IndexItem<ID^, ITEM*>
{
public:
	// Con-/Destructors
	IndexItem(ID^ Id, ITEM* Item): hId(Id), pItem(Item) { ASSERT(Id); }
	~IndexItem()
		{
		hId=nullptr;
		delete pItem;
		}

	// Access
	inline ID^ GetId()const { return hId; }
	inline ITEM* GetItem()const { return pItem; }

	// Modification
	inline ITEM* ReleaseItem() { return PointerRelease(&pItem); }
	inline VOID SetItem(ITEM* Item) { PointerAssign(&pItem, Item); }
	
private:
	// Common
	ID^ hId;
	ITEM* pItem;
};

// Item Handle-Index with Handle
template <class ID, class ITEM>
class IndexItem<ID^, ITEM^>
{
public:
	// Con-/Destructors
	IndexItem(ID^ Id, ITEM^ Item): hId(Id), hItem(Item) { ASSERT(hId); }
	~IndexItem()
		{
		hId=nullptr;
		hItem=nullptr;
		}

	// Access
	inline ID^ GetId()const { return hId; }
	inline ITEM^ GetItem()const { return hItem; }

	// Modification
	inline VOID SetItem(ITEM^ Item) { hItem=Item; }
	
private:
	// Common
	ID^ hId;
	ITEM^ hItem;
};

// Item Handle-Index with String
template <class ID, class CHAR>
class IndexItem<ID^, String<CHAR, true>>
{
public:
	// Con-/Destructors
	IndexItem(ID^ Id, CHAR const* Item, UINT Length): cItem(Item, Length), hId(Id) { ASSERT(hId); }
	~IndexItem() { hId=nullptr; }

	// Access
	inline ID^ GetId()const { return hId; }
	inline CHAR const* GetItem()const { return cItem.Get(); }

	// Modification
	inline CHAR const* ReleaseItem() { return cItem.Release(); }
	inline VOID SetItem(CHAR const* Value, UINT Length) { cItem.Set(Value, Length); }
	
private:
	// Common
	String<CHAR, true> cItem;
	ID^ hId;
};

// Item Handle-Index with Shared String
template <class ID, class CHAR>
class IndexItem<ID^, String<CHAR, false>>
{
public:
	// Con-/Destructors
	IndexItem(ID^ Id, CHAR const* Item): cItem(Item), hId(Id) { ASSERT(hId); }
	~IndexItem() { hId=nullptr; }

	// Access
	inline ID^ GetId()const { return hId; }
	inline CHAR const* GetItem()const { return cItem.Get(); }

	// Modification
	inline VOID SetItem(CHAR const* Value) { cItem.Set(Value); }
	
private:
	// Common
	String<CHAR, true> cItem;
	ID^ hId;
};

#endif


//===================
// Item String-Index
//===================

// Item String-Index
template <class CHAR, BOOL _AllocId, class ITEM>
class IndexItem<String<CHAR, _AllocId>, ITEM>
{
public:
	// Con-/Destructors
	IndexItem(CHAR const* Id, UINT Length, BOOL, ITEM const& Item): cId(Id, Length), tItem(Item) {}
	~IndexItem() {}

	// Access
	inline CHAR const* GetId()const { return cId.Get(); }
	inline ITEM& GetItem() { return tItem; }
	inline ITEM const& GetItem()const { return tItem; }

	// Modification
	inline VOID SetItem(ITEM const& Item) { tItem=Item; }
	
private:
	// Common
	String<CHAR, _AllocId> cId;
	ITEM tItem;
};

// Empty Item String-Index
template <class CHAR, BOOL _AllocId>
class IndexItem<String<CHAR, _AllocId>, VOID>
{
public:
	// Con-/Destructors
	IndexItem(CHAR const* Id, UINT Length, BOOL): cId(Id, Length) {}

	// Access
	inline CHAR const* GetId()const { return cId.Get(); }

private:
	// Common
	String<CHAR, true> cId;
};

// Item String-Index with Pointer
template <class CHAR, BOOL _AllocId, class ITEM>
class IndexItem<String<CHAR, _AllocId>, ITEM*>
{
public:
	// Con-/Destructors
	IndexItem(CHAR const* Id, UINT Length, BOOL, ITEM* Item): cId(Id, Length), pItem(Item) {}
	~IndexItem()
		{
		delete pItem;
		}

	// Access
	inline CHAR const* GetId()const { return cId.Get(); }
	inline ITEM* GetItem()const { return pItem; }

	// Modification
	inline ITEM* ReleaseItem() { return PointerRelease(&pItem); }
	inline VOID SetItem(ITEM* Item) { PointerAssign(&pItem, Item); }
	
private:
	// Common
	String<CHAR, _AllocId> cId;
	ITEM* pItem;
};

#ifdef __cplusplus_winrt
// Item String-Index with Handle
template <class CHAR, BOOL _AllocId, class ITEM>
class IndexItem<String<CHAR, _AllocId>, ITEM^>
{
public:
	// Con-/Destructors
	IndexItem(CHAR const* Id, UINT Length, BOOL, ITEM^ Item): cId(Id, Length), hItem(Item) {}
	~IndexItem()
		{
		hItem=nullptr;
		}

	// Access
	inline CHAR const* GetId()const { return cId.Get(); }
	inline ITEM^ GetItem()const { return hItem; }

	// Modification
	inline VOID SetItem(ITEM^ Item) { hItem=Item; }
	
private:
	// Common
	String<CHAR, _AllocId> cId;
	ITEM^ hItem;
};
#endif

// Item String-Index with String
template <class CHAR_ID, BOOL _AllocId, class CHAR_ITEM>
class IndexItem<String<CHAR_ID, _AllocId>, String<CHAR_ITEM, true>>
{
public:
	// Con-/Destructors
	IndexItem(CHAR_ID const* Id, UINT IdLength, BOOL, CHAR_ITEM const* Item, UINT ItemLength): cId(Id, IdLength), cItem(Item, ItemLength) {}

	// Access
	inline CHAR_ID const* GetId()const { return cId.Get(); }
	inline CHAR_ITEM const* GetItem()const { return cItem.Get(); }

	// Modification
	inline CHAR_ITEM const* ReleaseItem() { return cItem.Release(); }
	inline VOID SetItem(CHAR_ITEM const* Value, UINT Length) { cItem.Set(Value, Length); }
	
private:
	// Common
	String<CHAR_ID, _AllocId> cId;
	String<CHAR_ITEM, true> cItem;
};

// Item String-Index with Shared String
template <class CHAR_ID, BOOL _AllocId, class CHAR_ITEM>
class IndexItem<String<CHAR_ID, _AllocId>, String<CHAR_ITEM, false>>
{
public:
	// Con-/Destructors
	IndexItem(CHAR_ID const* Id, UINT IdLength, BOOL, CHAR_ITEM const* Item): cId(Id, IdLength), cItem(Item) {}

	// Access
	inline CHAR_ID const* GetId()const { return cId.Get(); }
	inline CHAR_ITEM const* GetItem()const { return cItem.Get(); }

	// Modification
	inline VOID SetItem(CHAR_ITEM const* Value) { cItem.Set(Value); }
	
private:
	// Common
	String<CHAR_ID, _AllocId> cId;
	String<CHAR_ITEM, false> cItem;
};

}}}}
