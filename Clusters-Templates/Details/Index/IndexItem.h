//=============
// IndexItem.h
//=============

#pragma once


//=======
// Using
//=======

#include "../StringClass.h"


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
template <class _Id, class _Item>
class IndexItem
{
public:
	// Con-/Destructors
	IndexItem(_Id const& Id, _Item const& Item): tId(Id), tItem(Item) {}

	// Access
	inline _Id const& GetId()const { return tId; }
	inline _Item& GetItem() { return tItem; }
	inline _Item const& GetItem()const { return tItem; }

	// Modification
	inline VOID SetItem(_Item const& Item) { tItem=Item; }
	
private:
	// Common
	_Id tId;
	_Item tItem;
};

// Empty Index-Item
template <class _Id>
class IndexItem<_Id, VOID>
{
public:
	// Con-/Destructors
	IndexItem(_Id const& Id): tId(Id) {}

	// Access
	inline _Id const& GetId()const { return tId; }
	
private:
	// Common
	_Id tId;
};

// Index-Item with Pointer
template <class _Id, class _Item>
class IndexItem<_Id, _Item*>
{
public:
	// Con-/Destructors
	IndexItem(_Id const& Id, _Item* Item): pItem(Item), tId(Id) {}
	~IndexItem() { delete pItem; }

	// Access
	inline _Id const& GetId()const { return tId; }
	inline _Item* GetItem()const { return pItem; }

	// Modification
	inline _Item* ReleaseItem() { return PointerRelease(&pItem); }
	inline VOID SetItem(_Item* Item) { PointerAssign(&pItem, Item); }
	
private:
	// Common
	_Item* pItem;
	_Id tId;
};

#ifdef __cplusplus_winrt
// Index-Item with Handle
template <class _Id, class _Item>
class IndexItem<_Id, _Item^>
{
public:
	// Con-/Destructors
	IndexItem(_Id const& Id, _Item^ Item): hItem(Item), tId(Id) {}
	~IndexItem() { hItem=nullptr; }

	// Access
	inline _Id const& GetId()const { return tId; }
	inline _Item^ GetItem()const { return hItem; }

	// Modification
	inline VOID SetItem(_Item^ Item) { hItem=Item; }
	
private:
	// Common
	_Item^ hItem;
	_Id tId;
};
#endif

// Index-Item with String-Item
template <class _Id, class _Char>
class IndexItem<_Id, StringItem<_Char, true>>
{
public:
	// Con-/Destructors
	IndexItem(_Id const& Id, _Char const* Item, UINT Length): cItem(Item, Length), tId(Id) {}

	// Access
	inline _Id const& GetId()const { return tId; }
	inline _Char const* GetItem()const { return cItem.Get(); }

	// Modification
	inline _Char const* ReleaseItem() { return cItem.Release(); }
	inline VOID SetItem(char const* Value, UINT Length) { cItem.Set(Value, Length); }
	
private:
	// Common
	StringItem<_Char, true> cItem;
	_Id tId;
};

// Index-Item with Shared String-Item
template <class _Id, class _Char>
class IndexItem<_Id, StringItem<_Char, false>>
{
public:
	// Con-/Destructors
	IndexItem(_Id const& Id, _Char const* Item): cItem(Item), tId(Id) {}

	// Access
	inline _Id const& GetId()const { return tId; }
	inline _Char const* GetItem()const { return cItem.Get(); }

	// Modification
	inline VOID SetItem(char const* Value) { cItem.Set(Value); }
	
private:
	// Common
	StringItem<_Char, false> cItem;
	_Id tId;
};


//====================
// Item Pointer-Index
//====================

// Item Pointer-Index
template <class _Id, class _Item>
class IndexItem<_Id*, _Item>
{
public:
	// Con-/Destructors
	IndexItem(_Id* Id, _Item const& Item): pId(Id), tItem(Item) { ASSERT(Id); }
	~IndexItem() { delete pId; }

	// Access
	inline _Id* GetId()const { return pId; }
	inline _Item& GetItem() { return tItem; }
	inline _Item const& GetItem()const { return tItem; }

	// Modification
	inline VOID SetItem(_Item const& Item) { tItem=Item; }
	
private:
	// Common
	_Id* pId;
	_Item tItem;
};

// Empty Item Pointer-Index
template <class _Id>
class IndexItem<_Id*, VOID>
{
public:
	// Con-/Destructors
	IndexItem(_Id* Id): pId(Id) { ASSERT(Id); }
	~IndexItem() { delete pId; }

	// Access
	inline _Id* GetId()const { return pId; }

private:
	// Common
	_Id* pId;
};

// Item Pointer-Index with Pointer
template <class _Id, class _Item>
class IndexItem<_Id*, _Item*>
{
public:
	// Con-/Destructors
	IndexItem(_Id* Id, _Item* Item): pId(Id), pItem(Item) { ASSERT(Id); }
	~IndexItem()
		{
		delete pId;
		delete pItem;
		}

	// Access
	inline _Id* GetId()const { return pId; }
	inline _Item* GetItem()const { return pItem; }

	// Modification
	inline _Item* ReleaseItem() { return PointerRelease(&pItem); }
	inline VOID SetItem(_Item* Item) { PointerAssign(&pItem, Item); }
	
private:
	// Common
	_Id* pId;
	_Item* pItem;
};

#ifdef __cplusplus_winrt
// Item Pointer-Index with Handle
template <class _Id, class _Item>
class IndexItem<_Id*, _Item^>
{
public:
	// Con-/Destructors
	IndexItem(_Id* Id, _Item^ Item): pId(Id), hItem(Item) { ASSERT(pId); }
	~IndexItem()
		{
		delete pId;
		hItem=nullptr;
		}

	// Access
	inline _Id* GetId()const { return pId; }
	inline _Item^ GetItem()const { return hItem; }

	// Modification
	inline VOID SetItem(_Item^ Item) { hItem=Item; }
	
private:
	// Common
	_Id* pId;
	_Item^ hItem;
};
#endif

// Item Pointer-Index with String-Item
template <class _Id, class _Char>
class IndexItem<_Id*, StringItem<_Char, true>>
{
public:
	// Con-/Destructors
	IndexItem(_Id* Id, _Char const* Item, UINT Length): cItem(Item, Length), pId(Id) { ASSERT(pId); }
	~IndexItem() { delete pId; }

	// Access
	inline _Id* GetId()const { return pId; }
	inline _Char const* GetItem()const { return cItem.Get(); }

	// Modification
	inline _Char const* ReleaseItem() { return cItem.Release(); }
	inline VOID SetItem(_Char const* Value, UINT Length) { cItem.Set(Value, Length); }
	
private:
	// Common
	StringItem<_Char, true> cItem;
	_Id* pId;
};

// Item Pointer-Index with Shared String-Item
template <class _Id, class _Char>
class IndexItem<_Id*, StringItem<_Char, false>>
{
public:
	// Con-/Destructors
	IndexItem(_Id* Id, _Char const* Item): cItem(Item), pId(Id) { ASSERT(pId); }
	~IndexItem() { delete pId; }

	// Access
	inline _Id* GetId()const { return pId; }
	inline _Char const* GetItem()const { return cItem.Get(); }

	// Modification
	inline VOID SetItem(_Char const* Value) { cItem.Set(Value); }
	
private:
	// Common
	StringItem<_Char, true> cItem;
	_Id* pId;
};


//===================
// Item Handle-Index
//===================

#ifdef __cplusplus_winrt

// Item Handle-Index
template <class _Id, class _Item>
class IndexItem<_Id^, _Item>
{
public:
	// Con-/Destructors
	IndexItem(_Id^ Id, _Item const& Item): hId(Id), tItem(Item) { ASSERT(Id); }
	~IndexItem() { hId=nullptr; }

	// Access
	inline _Id^ GetId()const { return hId; }
	inline _Item& GetItem() { return tItem; }
	inline _Item const& GetItem()const { return tItem; }

	// Modification
	inline VOID SetItem(_Item const& Item) { tItem=Item; }
	
private:
	// Common
	_Id^ hId;
	_Item tItem;
};

// Empty Item Handle-Index
template <class _Id>
class IndexItem<_Id^, VOID>
{
public:
	// Con-/Destructors
	IndexItem(_Id^ Id): hId(Id) { ASSERT(hId); }
	~IndexItem() { hId=nullptr; }

	// Access
	inline _Id^ GetId()const { return hId; }
	
private:
	// Common
	_Id^ hId;
};

// Item Handle-Index with Pointer
template <class _Id, class _Item>
class IndexItem<_Id^, _Item*>
{
public:
	// Con-/Destructors
	IndexItem(_Id^ Id, _Item* Item): hId(Id), pItem(Item) { ASSERT(Id); }
	~IndexItem()
		{
		hId=nullptr;
		delete pItem;
		}

	// Access
	inline _Id^ GetId()const { return hId; }
	inline _Item* GetItem()const { return pItem; }

	// Modification
	inline _Item* ReleaseItem() { return PointerRelease(&pItem); }
	inline VOID SetItem(_Item* Item) { PointerAssign(&pItem, Item); }
	
private:
	// Common
	_Id^ hId;
	_Item* pItem;
};

// Item Handle-Index with Handle
template <class _Id, class _Item>
class IndexItem<_Id^, _Item^>
{
public:
	// Con-/Destructors
	IndexItem(_Id^ Id, _Item^ Item): hId(Id), hItem(Item) { ASSERT(hId); }
	~IndexItem()
		{
		hId=nullptr;
		hItem=nullptr;
		}

	// Access
	inline _Id^ GetId()const { return hId; }
	inline _Item^ GetItem()const { return hItem; }

	// Modification
	inline VOID SetItem(_Item^ Item) { hItem=Item; }
	
private:
	// Common
	_Id^ hId;
	_Item^ hItem;
};

// Item Handle-Index with String-Item
template <class _Id, class _Char>
class IndexItem<_Id^, StringItem<_Char, true>>
{
public:
	// Con-/Destructors
	IndexItem(_Id^ Id, _Char const* Item, UINT Length): cItem(Item, Length), hId(Id) { ASSERT(hId); }
	~IndexItem() { hId=nullptr; }

	// Access
	inline _Id^ GetId()const { return hId; }
	inline _Char const* GetItem()const { return cItem.Get(); }

	// Modification
	inline _Char const* ReleaseItem() { return cItem.Release(); }
	inline VOID SetItem(_Char const* Value, UINT Length) { cItem.Set(Value, Length); }
	
private:
	// Common
	StringItem<_Char, true> cItem;
	_Id^ hId;
};

// Item Handle-Index with Shared String-Item
template <class _Id, class _Char>
class IndexItem<_Id^, StringItem<_Char, false>>
{
public:
	// Con-/Destructors
	IndexItem(_Id^ Id, _Char const* Item): cItem(Item), hId(Id) { ASSERT(hId); }
	~IndexItem() { hId=nullptr; }

	// Access
	inline _Id^ GetId()const { return hId; }
	inline _Char const* GetItem()const { return cItem.Get(); }

	// Modification
	inline VOID SetItem(_Char const* Value) { cItem.Set(Value); }
	
private:
	// Common
	StringItem<_Char, true> cItem;
	_Id^ hId;
};

#endif


//===================
// Item String-Index
//===================

// Item String-Index
template <class _Char, BOOL _AllocId, class _Item>
class IndexItem<StringItem<_Char, _AllocId>, _Item>
{
public:
	// Con-/Destructors
	IndexItem(_Char const* Id, UINT Length, BOOL, _Item const& Item): cId(Id, Length), tItem(Item) {}
	~IndexItem() {}

	// Access
	inline _Char const* GetId()const { return cId.Get(); }
	inline _Item& GetItem() { return tItem; }
	inline _Item const& GetItem()const { return tItem; }

	// Modification
	inline VOID SetItem(_Item const& Item) { tItem=Item; }
	
private:
	// Common
	StringItem<_Char, _AllocId> cId;
	_Item tItem;
};

// Empty Item String-Index
template <class _Char, BOOL _AllocId>
class IndexItem<StringItem<_Char, _AllocId>, VOID>
{
public:
	// Con-/Destructors
	IndexItem(_Char const* Id, UINT Length, BOOL): cId(Id, Length) {}

	// Access
	inline _Char const* GetId()const { return cId.Get(); }

private:
	// Common
	StringItem<_Char, true> cId;
};

// Item String-Index with Pointer
template <class _Char, BOOL _AllocId, class _Item>
class IndexItem<StringItem<_Char, _AllocId>, _Item*>
{
public:
	// Con-/Destructors
	IndexItem(_Char const* Id, UINT Length, BOOL, _Item* Item): cId(Id, Length), pItem(Item) {}
	~IndexItem()
		{
		delete pItem;
		}

	// Access
	inline _Char const* GetId()const { return cId.Get(); }
	inline _Item* GetItem()const { return pItem; }

	// Modification
	inline _Item* ReleaseItem() { return PointerRelease(&pItem); }
	inline VOID SetItem(_Item* Item) { PointerAssign(&pItem, Item); }
	
private:
	// Common
	StringItem<_Char, _AllocId> cId;
	_Item* pItem;
};

#ifdef __cplusplus_winrt
// Item String-Index with Handle
template <class _Char, BOOL _AllocId, class _Item>
class IndexItem<StringItem<_Char, _AllocId>, _Item^>
{
public:
	// Con-/Destructors
	IndexItem(_Char const* Id, UINT Length, BOOL, _Item^ Item): cId(Id, Length), hItem(Item) {}
	~IndexItem()
		{
		hItem=nullptr;
		}

	// Access
	inline _Char const* GetId()const { return cId.Get(); }
	inline _Item^ GetItem()const { return hItem; }

	// Modification
	inline VOID SetItem(_Item^ Item) { hItem=Item; }
	
private:
	// Common
	StringItem<_Char, _AllocId> cId;
	_Item^ hItem;
};
#endif

// Item String-Index with String-Item
template <class _CharId, BOOL _AllocId, class _CharItem>
class IndexItem<StringItem<_CharId, _AllocId>, StringItem<_CharItem, true>>
{
public:
	// Con-/Destructors
	IndexItem(_CharId const* Id, UINT IdLength, BOOL, _CharItem const* Item, UINT ItemLength): cId(Id, IdLength), cItem(Item, ItemLength) {}

	// Access
	inline _CharId const* GetId()const { return cId.Get(); }
	inline _CharItem const* GetItem()const { return cItem.Get(); }

	// Modification
	inline _CharItem const* ReleaseItem() { return cItem.Release(); }
	inline VOID SetItem(_CharItem const* Value, UINT Length) { cItem.Set(Value, Length); }
	
private:
	// Common
	StringItem<_CharId, _AllocId> cId;
	StringItem<_CharItem, true> cItem;
};

// Item String-Index with Shared String-Item
template <class _CharId, BOOL _AllocId, class _CharItem>
class IndexItem<StringItem<_CharId, _AllocId>, StringItem<_CharItem, false>>
{
public:
	// Con-/Destructors
	IndexItem(_CharId const* Id, UINT IdLength, BOOL, _CharItem const* Item): cId(Id, IdLength), cItem(Item) {}

	// Access
	inline _CharId const* GetId()const { return cId.Get(); }
	inline _CharItem const* GetItem()const { return cItem.Get(); }

	// Modification
	inline VOID SetItem(_CharItem const* Value) { cItem.Set(Value); }
	
private:
	// Common
	StringItem<_CharId, _AllocId> cId;
	StringItem<_CharItem, false> cItem;
};

}}}}
