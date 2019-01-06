//=======
// Box.h
//=======

#pragma once


//===============
// Encapsulation
//===============

template <class T>
class Box
{
public:
	// Con-/Destructors
	Box() {}
	Box(T const& Item): tItem(Item) {}

	// Access
	operator T&() { return tItem; }
	operator T const&()const { return tItem; }

private:
	// Common
	T tItem;
};

// Encapsulation Pointer
template <class T>
class Box<T*>
{
public:
	// Con-/Destructors
	Box(): pItem(nullptr) {}
	Box(T* Item): pItem(Item) {}

	// Access
	operator T*() { return pItem; }
	operator T const*()const { return pItem; }
	T* operator->() { return pItem; }

private:
	// Common
	T* pItem;
};
