//===============
// ArrayHelper.h
//===============

#pragma once


//=======
// Using
//=======

#include <utility>


//=========================
// Array-Helper Base-Class
//=========================

namespace Details {

template <class T, class S>
class ArrayHelperBase
{
public:
	// Access
	template <class _Param> static BOOL Contains(T const* Items, S Count, _Param Item)
		{
		for (S u=0; u<Count; u++)
			{
			if(Items[u]==Item)
				return true;
			}
		return false;
		}
	static T* GetAt(T* Items, S Count, S Position)
		{
		if(Position>=Count)
			return nullptr;
		return &Items[Position];
		}
	static T const* GetAt(T const* Items, S Count, S Position)
		{
		if(Position>=Count)
			return nullptr;
		return &Items[Position];
		}
	static T* GetFirst(T* Items, S Count)
		{
		if(Count==0)
			return nullptr;
		return &Items[0];
		}
	static T const* GetFirst(T const* Items, S Count)
		{
		if(Count==0)
			return nullptr;
		return &Items[0];
		}
	static T* GetLast(T* Items, S Count)
		{
		if(Count==0)
			return nullptr;
		return &Items[Count-1];
		}
	static T const* GetLast(T const* Items, S Count)
		{
		if(Count==0)
			return nullptr;
		return &Items[Count-1];
		}
	template <class _Param> static BOOL GetPosition(T const* Items, S Count, _Param Item, S* Position)
		{
		for (S u=0; u<Count; u++)
			{
			if(Items[u]==Item)
				{
				*Position=u;
				return true;
				}
			}
		return false;
		}
};


//====================
// Array-Helper Typed
//====================

template <class T, class S, bool _NeedsConstruction>
class ArrayHelperTyped: public ArrayHelperBase<T, S>
{
public:
	// Assignment
	static inline VOID InitItem(T* Item) { new (Item) T(); }
	static inline VOID InitItems(T* Items, S Count) { for(S u=0; u<Count; u++)new (&Items[u]) T(); }

	// Modification
	static inline VOID DestroyItem(T* Item) { *Item.~T(); }
	static inline VOID DestroyItems(T* Items, S Count) { for(S u=0; u<Count; u++)Items[u].~T(); }
	static inline VOID MoveItems(T* Destination, T const* Source, S Count)
		{
		if(Destination>Source)
			{
			for(S u=0; u<Count; u++)
				new (&Destination[Count-u-1]) T(std::move(Source[Count-u-1]));
			}
		else if(Destination<Source)
			{
			for(S u=0; u<Count; u++)
				new (&Destination[u]) T(std::move(Source[u]));
			}
		}
	static inline VOID OverWriteItems(T* Destination, T const* Source, S Count)
		{
		for(S u=0; u<Count; u++)
			new (&Destination[u]) T(Source[u]);
		}
	static inline VOID SetAt(T* Items, S Count, S Position, T const& Set) { Items[Position]=Set; }
	static inline VOID SetItem(T* Item, T const& NewItem) { *Item=NewItem; }
};

template <class T, class S>
class ArrayHelperTyped<T, S, false>: public ArrayHelperBase<T, S>
{
public:
	// Assignment
	static inline VOID InitItem(T* Item) {}
	static inline VOID InitItems(T* Items, S Count) {}

	// Modification
	static inline VOID DestroyItem(T* Item) {}
	static inline VOID DestroyItems(T* Items, S Count) {}
	static inline VOID MoveItems(T* Destination, T const* Source, S Count) { MoveMemory(Destination, Source, Count*sizeof(T)); }
	static inline VOID OverWriteItems(T* Destination, T const* Source, S Count) { CopyMemory(Destination, Source, Count*sizeof(T)); }
	static inline VOID SetAt(T* Items, S Count, S Position, T const& Set) { Items[Position]=Set; }
	static inline VOID SetItem(T* Item, T const& NewItem) { *Item=NewItem; }
};

template <class T, class S>
class ArrayHelperTyped<T*, S, true>: public ArrayHelperBase<T*, S>
{
public:
	// Assignment
	static inline VOID InitItem(T** Item) { *Item=nullptr; }
	static inline VOID InitItems(T** Items, S Count) { ZeroMemory(Items, Count*sizeof(T*)); }

	// Modification
	static inline VOID DestroyItem(T** Item) { delete *Item; }
	static inline VOID DestroyItems(T** Items, S Count) { for(S u=0; u<Count; u++)delete Items[u]; }
	static inline VOID MoveItems(T** Destination, T* const* Source, S Count) { MoveMemory(Destination, Source, Count*sizeof(T*)); }
	static inline VOID OverWriteItems(T** Destination, T* const* Source, S Count) { CopyMemory(Destination, Source, Count*sizeof(T*)); }
	static inline VOID SetAt(T** Items, S Count, S Position, T* Set) { if(Items[Position]!=nullptr)delete Items[Position]; Items[Position]=Set; }
	static inline VOID SetItem(T** Item, T* NewItem) { if(*Item!=nullptr)delete *Item; *Item=NewItem; }
};

#ifdef __cplusplus_winrt
template <class T, class S>
class ArrayHelperTyped<T^, S, true>: public ArrayHelperBase<T^, S>
{
public:
	// Assignment
	static inline VOID InitItem(T^* Item) { ZeroMemory(&Item, sizeof(T^)); }
	static inline VOID InitItems(T^* Items, S Count) { ZeroMemory(Items, Count*sizeof(T^)); }

	// Modification
	static inline VOID DestroyItem(T^* Item) { *Item=nullptr; }
	static inline VOID DestroyItems(T^* Items, S Count) { for(S u=0; u<Count; u++)Items[u]=nullptr; }
	static inline VOID MoveItems(T^* Destination, T^ const* Source, S Count) { MoveMemory(Destination, Source, Count*sizeof(T^)); }
	static inline VOID OverWriteItems(T^* Destination, T^ const* Source, S Count)
		{
		ZeroMemory(Destination, Count*sizeof(T^));
		for(S u=0; u<Count; u++)
			Destination[u]=Source[u];
		}
	static inline VOID SetAt(T^* Items, S Count, S Position, T^ Set) { Items[Position]=Set; }
	static inline VOID SetItem(T^* Item, T^ NewItem) { *Item=NewItem; }
};
#endif

}


//==============
// Array-Helper
//==============

template <class T, class S>
class ArrayHelper: public ::Details::ArrayHelperTyped<T, S, TypeNeedsConstruction<T>::value>
{
private:
	// Using
	using _ArrayHelperBase=::Details::ArrayHelperBase<T, S>;
	using _ArrayHelperTyped=::Details::ArrayHelperTyped<T, S, TypeNeedsConstruction<T>::value>;

public:
	// Modification
	static VOID Clear(T** Items, S* Size, S* Count)
		{
		_ArrayHelperTyped::DestroyItems(*Items, *Count);
		operator delete(*Items);
		*Items=nullptr;
		*Size=0;
		*Count=0;
		}
	static T* Append(T* Items, S Size, S* Count)
		{
		UINT ucount=*Count;
		if(ucount==Size)
			return nullptr;
		T* pitem=&Items[ucount];
		*Count=ucount+1;
		return pitem;
		}
	static T* Append(T** Items, S* Size, S* Count, UINT BlockSize)
		{
		S ucount=*Count;
		S unewcount=ucount+1;
		if(unewcount>*Size)
			SetSize(Items, Size, Count, BlockSize, unewcount);
		T* pitems=*Items;
		*Count=unewcount;
		return &pitems[ucount];
		}
	static S Append(T* Items, S Size, S* Count, T const* Insert, S InsertCount, BOOL Moving=false)
		{
		S ucount=*Count;
		S unewcount=ucount+InsertCount;
		if(unewcount>Size)
			{
			ASSERT(true);
			return ucount;
			}
		if(Moving)
			{
			_ArrayHelperTyped::MoveItems(&Items[ucount], Insert, InsertCount);
			}
		else
			{
			_ArrayHelperTyped::OverWriteItems(&Items[ucount], Insert, InsertCount);
			}
		*Count=unewcount;
		return unewcount;
		}
	static S Append(T** Items, S* Size, S* Count, UINT BlockSize, T const* NewItems, S InsertCount, BOOL Moving=false)
		{
		S ucount=*Count;
		S unewcount=ucount+InsertCount;
		if(unewcount>*Size)
			SetSize(Items, Size, Count, unewcount, BlockSize);
		T* pitems=*Items;
		if(Moving)
			{
			_ArrayHelperTyped::MoveItems(&pitems[ucount], NewItems, InsertCount);
			}
		else
			{
			_ArrayHelperTyped::OverWriteItems(&pitems[ucount], NewItems, InsertCount);
			}
		*Count=unewcount;
		return unewcount;
		}
	static S Assign(T** Items, S* Size, S* Count, UINT BlockSize, T const* NewItems, S NewCount, BOOL Moving=false)
		{
		T* pitems=*Items;
		_ArrayHelperTyped::DestroyItems(pitems, *Count);
		*Count=0;
		SetSize(Items, Size, Count, BlockSize, NewCount);
		if(Moving)
			{
			_ArrayHelperTyped::MoveItems(pitems, NewItems, NewCount);
			}
		else
			{
			_ArrayHelperTyped::OverWriteItems(pitems, NewItems, NewCount);
			}
		*Count=NewCount;
		return NewCount;
		}
	static T* InsertAt(T* Items, S Size, S* Count, S Position)
		{
		S ucount=*Count;
		if(ucount==Size||Position>ucount)
			return nullptr;
		if(Position<ucount)
			_ArrayHelperTyped::MoveItems(&Items[Position+1], &Items[Position], ucount-Position);
		*Count=ucount+1;
		return &Items[Position];
		}
	static T* InsertAt(T** Items, S* Size, S* Count, UINT BlockSize, S Position)
		{
		S ucount=*Count;
		if(Position>ucount)
			return nullptr;
		S unewcount=ucount+1;
		if(unewcount>*Size)
			SetSize(Items, Size, Count, BlockSize, unewcount);
		T* pitems=*Items;
		if(Position<ucount)
			_ArrayHelperTyped::MoveItems(&pitems[Position+1], &pitems[Position], ucount-Position);
		*Count=unewcount;
		return &pitems[Position];
		}
	static S InsertAt(T* Items, S Size, S* Count, S Position, T const* Insert, S InsertCount, BOOL Moving=false)
		{
		S ucount=*Count;
		S unewcount=ucount+InsertCount;
		if(Position>ucount||unewcount>Size)
			{
			ASSERT(true);
			return ucount;
			}
		if(Position<ucount)
			_ArrayHelperTyped::MoveItems(&Items[Position+InsertCount], &Items[Position], ucount-Position);
		if(Moving)
			{
			_ArrayHelperTyped::MoveItems(&Items[Position], Insert, InsertCount);
			}
		else
			{
			_ArrayHelperTyped::OverWriteItems(&Items[Position], Insert, InsertCount);
			}
		*Count=unewcount;
		return unewcount;
		}
	static S InsertAt(T** Items, S* Size, S* Count, UINT BlockSize, S Position, T const* NewItems, S NewCount, BOOL Moving=false)
		{
		S ucount=*Count;
		if(Position>ucount)
			{
			ASSERT(true);
			return ucount;
			}
		S unewcount=ucount+NewCount;
		if(unewcount>*Size)
			SetSize(Items, Size, Count, unewcount, BlockSize);
		T* pitems=*Items;
		_ArrayHelperTyped::MoveItems(&pitems[Position+NewCount], &pitems[Position], ucount-Position);
		if(Moving)
			{
			_ArrayHelperTyped::MoveItems(&pitems[Position], NewItems, NewCount);
			}
		else
			{
			_ArrayHelperTyped::OverWriteItems(&pitems[Position], NewItems, NewCount);
			}
		*Count=unewcount;
		return unewcount;
		}
	static S RemoveAt(T* Items, S* Count, S Position, S RemoveCount=1, BOOL Moving=false)
		{
		S ucount=*Count;
		if(Position+RemoveCount>ucount)
			{
			ASSERT(true);
			return ucount;
			}
		if(!Moving)
			_ArrayHelperTyped::DestroyItems(&Items[Position], RemoveCount);
		_ArrayHelperTyped::MoveItems(&Items[Position], &Items[Position+RemoveCount], ucount-Position-RemoveCount);
		ucount-=RemoveCount;
		*Count=ucount;
		return ucount;
		}
	static S RemoveAt(T** Items, S* Size, S* Count, UINT BlockSize, S Position, S RemoveCount=1, BOOL Moving=false, BOOL DoShrink=true)
		{
		S ucount=*Count;
		if(Position+RemoveCount>ucount)
			{
			ASSERT(true);
			return ucount;
			}
		T* pitems=*Items;
		if(!Moving)
			_ArrayHelperTyped::DestroyItems(&pitems[Position], RemoveCount);
		_ArrayHelperTyped::MoveItems(&pitems[Position], &pitems[Position+RemoveCount], ucount-Position-RemoveCount);
		ucount-=RemoveCount;
		*Count=ucount;
		if(DoShrink)
			Shrink(Items, Size, *Count, BlockSize);
		return ucount;
		}
	static BOOL RemoveItem(T** Items, S* Size, S* Count, UINT BlockSize, T const* Remove)
		{
		T* pitems=*Items;
		S ucount=*Count;
		for (S u=0; u<ucount; u++)
			{
			if(pitems[u]==*Remove)
				{
				RemoveAt(Items, Size, Count, u, 1, BlockSize, false);
				return true;
				}
			}
		return false;
		}
	static S RemoveItem(T** Items, S* Size, S* Count, UINT BlockSize, T const* Remove, S RemoveCount)
		{
		T* pitems=*Items;
		S ucount=*Count;
		for (S uitem=0; uitem<RemoveCount; uitem++)
			{
			for (S u=0; u<ucount; )
				{
				if(pitems[u]==Remove[uitem])
					{
					RemoveAt(Items, Size, Count, u, 1, BlockSize, false);
					ucount--;
					}
				else
					{
					u++;
					}
				}
			}
		Shrink(Items, Size, *Count, BlockSize);
		return ucount;
		}
	static VOID SetCount(T** Items, S* Size, S* Count, UINT BlockSize, S NewCount)
		{
		S ucount=*Count;
		if(NewCount==ucount)
			return;
		if(!NewCount)
			{
			Clear(Items, Size, Count);
			return;
			}
		T* pitems=*Items;
		if(ucount>NewCount)
			_ArrayHelperTyped::DestroyItems(&pitems[NewCount], ucount-NewCount);
		S unewsize=BlockAlign<S>(NewCount, BlockSize);
		if(unewsize != *Size)
			{
			T* pold=pitems;
			pitems=(T*)operator new(unewsize*sizeof(T));
			S ucopy=MIN(ucount, NewCount);
			_ArrayHelperTyped::MoveItems(pitems, pold, ucopy);
			operator delete(pold);
			*Items=pitems;
			*Size=unewsize;
			}
		if(NewCount>ucount)
			_ArrayHelperTyped::InitItems(&pitems[ucount], NewCount-ucount);
		*Count=NewCount;
		}
	static VOID SetSize(T** Items, S* Size, S* Count, UINT BlockSize, S NewSize)
		{
		if(!NewSize)
			{
			Clear(Items, Size, Count);
			return;
			}
		S usize=*Size;
		S unewsize=BlockAlign<S>(NewSize, BlockSize);
		if(unewsize==usize)
			return;
		T* pitems=*Items;
		S ucount=*Count;
		S unewcount=MIN(unewsize, ucount);
		if(unewcount<ucount)
			_ArrayHelperTyped::DestroyItems(&pitems[unewcount], ucount-unewcount);
		*Items=(T*)operator new(unewsize*sizeof(T));
		S ucopy=MIN(ucount, unewcount);
		_ArrayHelperTyped::MoveItems(*Items, pitems, ucopy);
		operator delete(pitems);
		*Size=unewsize;
		*Count=unewcount;
		}
	static VOID Shrink(T** Items, S* Size, S Count, UINT BlockSize)
		{
		S usize=*Size;
		S unewsize=BlockAlign(Count, BlockSize);
		if(unewsize==usize)
			return;
		T* pitems=(T*)operator new(unewsize*sizeof(T));
		_ArrayHelperTyped::MoveItems(pitems, *Items, Count);
		operator delete(*Items);
		*Items=pitems;
		*Size=unewsize;
		}

	// Pointers
	static T ReleaseAt(T* Items, S* Count, S Position)
		{
		S ucount=*Count;
		if(Position>=ucount)
			{
			ASSERT(true);
			return nullptr;
			}
		T pitem=Items[Position];
		_ArrayHelperTyped::MoveItems(&Items[Position], &Items[Position+1], ucount-Position-1);
		ucount--;
		*Count=ucount;
		return pitem;
		}
	static T ReleaseAt(T** Items, S* Size, S* Count, UINT BlockSize, S Position)
		{
		S ucount=*Count;
		if(Position>=ucount)
			{
			ASSERT(true);
			return nullptr;
			}
		T* pitems=*Items;
		T pitem=pitems[Position];
		_ArrayHelperTyped::MoveItems(&pitems[Position], &pitems[Position+1], ucount-Position-1);
		*Count--;
		Shrink(Items, Size, *Count, BlockSize);
		return pitem;
		}
	static T ReleaseItem(T* Items, S* Count, T* Item)
		{
		S upos=0;
		if(_ArrayHelperBase::GetPosition(Items, *Count, Item, &upos))
			return ReleaseAt(Items, Count, upos);
		return nullptr;
		}
	static T ReleaseItem(T** Items, S* Size, S* Count, UINT BlockSize, T Item)
		{
		S upos=0;
		if(_ArrayHelperBase::GetPosition(*Items, *Count, Item, &upos))
			return ReleaseAt(Items, Size, Count, BlockSize, upos);
		return nullptr;
		}
};
