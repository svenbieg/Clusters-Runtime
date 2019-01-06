//===========
// Pointer.h
//===========

#pragma once


//=========
// Pointer
//=========

template <class _Item>
class Pointer
{
public:
	// Con-/Destructors
	Pointer(): pItem(nullptr) {}
	Pointer(_Item* Item): pItem(Item) {}
	~Pointer()
		{
		if(pItem)
			delete pItem;
		}

	// Access
	inline operator BOOL()const { return pItem!=nullptr; }
	inline operator _Item*()const { return pItem; }
	inline BOOL operator==(_Item* Item) { return pItem==Item; }
	inline BOOL operator==(const Pointer<_Item>& Pointer) { return pItem==Pointer.pItem; }
	inline _Item* operator->()const { return pItem; }
	inline _Item* Get()const { return pItem; }

	// Assignment
	Pointer& operator=(_Item* Item)
		{
		if(pItem)
			delete pItem;
		pItem=Item;
		return *this;
		}

private:
	_Item* pItem;
};
