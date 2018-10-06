//===============
// ArrayHelper.h
//===============

#pragma once


//=========================
// Array-Helper Base-Class
//=========================

namespace Details {

template <class T, class S>
class ArrayHelperBase
{
public:
	// Access
	static inline T* GetAt(T* Items, S Count, S Position)
		{
		if(Position>=Count)
			return nullptr;
		return &Items[Position];
		}
	static inline T const* GetAt(T const* Items, S Count, S Position)
		{
		if(Position>=Count)
			return nullptr;
		return &Items[Position];
		}
	static inline T* GetFirst(T* Items, S Count)
		{
		if(Count==0)
			return nullptr;
		return &Items[0];
		}
	static inline T const* GetFirst(T const* Items, S Count)
		{
		if(Count==0)
			return nullptr;
		return &Items[0];
		}
	static inline T* GetLast(T* Items, S Count)
		{
		if(Count==0)
			return nullptr;
		return &Items[Count-1];
		}
	static inline T const* GetLast(T const* Items, S Count)
		{
		if(Count==0)
			return nullptr;
		return &Items[Count-1];
		}

protected:
	// Search
	template <class PARAM> static BOOL ContainsInternal(T const* Items, S Count, PARAM Item)
		{
		for(S u=0; u<Count; u++)
			{
			if(Items[u]==Item)
				return true;
			}
		return false;
		}
	template <class PARAM> static BOOL GetPositionInternal(T const* Items, S Count, PARAM Item, S* Position)
		{
		for(S u=0; u<Count; u++)
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

}


//====================
// Typed Array-Helper
//====================

namespace Details {

template <class T, class S, bool _IntegratedType>
class ArrayHelperTyped
{};

// Array-Helper Integrated Types
template <class T, class S>
class ArrayHelperTyped<T, S, true>: public ArrayHelperBase<T, S>
{
public:
	// Search
	static inline BOOL Contains(T const* Items, S Count, T Item) { return ContainsInternal(Items, Count, Item); }
	static inline BOOL GetPosition(T const* Items, S Count, T Item, S* Position) { return GetPositionInternal(Items, Count, Item, Position); }

	// Assignment
	static inline VOID InitItem(T* Item) {}
	static inline VOID InitItems(T* Items, S Count) {}

	// Modification
	static inline VOID DestroyItem(T* Item) {}
	static inline VOID DestroyItems(T* Items, S Count) {}
	static inline VOID MoveItems(T* Destination, T const* Source, S Count) { MoveMemory(Destination, Source, Count*sizeof(T)); }
	static inline VOID OverWriteItem(T* Item, T const& NewItem) { *Item=NewItem; }
	static inline VOID OverWriteItems(T* Items, T const* NewItems, S Count) { CopyMemory(Items, NewItems, Count*sizeof(T)); }
	static inline VOID SetAt(T* Items, S Count, S Position, T Set)
		{
		ASSERT(Position<Count);
		Items[Position]=Set;
		}
	static inline VOID SetItem(T* Item, T NewItem) { *Item=NewItem; }

private:
	// Con-/Destructors
	ArrayHelperTyped() {}
};

// Array-Helper Non-Integrated Types
template <class T, class S>
class ArrayHelperTyped<T, S, false>: public ArrayHelperBase<T, S>
{
public:
	// Search
	static inline BOOL Contains(T const* Items, S Count, T const& Item) { return ContainsInternal<T const&>(Items, Count, Item); }
	static inline BOOL GetPosition(T const* Items, S Count, T const& Item, S* Position) { return GetPositionInternal<T const&>(Items, Count, Item, Position); }

	// Assignment
	static inline VOID InitItem(T* Item) { new (Item) T(); }
	static inline VOID InitItems(T* Items, S Count)
		{
		for(S u=0; u<Count; u++)
			new (&Items[u]) T();
		}

	// Modification
	static inline VOID DestroyItem(T* Item) { *Item.~T(); }
	static inline VOID DestroyItems(T* Items, S Count)
		{
		for(S u=0; u<Count; u++)
			Items[u].~T();
		}
	static inline VOID MoveItems(T* Destination, T const* Source, S Count) { MoveMemory(Destination, Source, Count*sizeof(T)); }
	static inline VOID OverWriteItem(T* Item, T const& NewItem) { new(Item) T(NewItem); }
	static inline VOID OverWriteItems(T* Items, T const* NewItems, S Count)
		{
		for(S u=0; u<Count; u++)
			new (&Items[u]) T(NewItems[u]);
		}
	static inline VOID SetAt(T* Items, S Count, S Position, T const& Set)
		{
		ASSERT(Position<Count);
		Items[Position]=Set;
		}
	static inline VOID SetItem(T* Item, T const& NewItem) { *Item=NewItem; }

private:
	// Con-/Destructors
	ArrayHelperTyped() {}
};

// Typed Array-Helper Pointers
template <class T, class S>
class ArrayHelperTyped<T*, S, false>: public ArrayHelperBase<T*, S>
{
public:
	// Search
	static inline BOOL Contains(T* const* Items, S Count, T* Item) { return ContainsInternal(Items, Count, Item); }
	static inline BOOL GetPosition(T* const* Items, S Count, T* Item, S* Position) { return GetPositionInternal(Items, Count, Item, Position); }

	// Assignment
	static inline VOID InitItem(T** Item) { *Item=nullptr; }
	static inline VOID InitItems(T** Items, S Count) { ZeroMemory(Items, Count*sizeof(T*)); }

	// Modification
	static inline VOID DestroyItem(T** Item) { PointerFree(*Item); }
	static inline VOID DestroyItems(T** Items, S Count)
		{
		for(S u=0; u<Count; u++)
			PointerFree(Items[u]);
		}
	static inline VOID MoveItems(T** Destination, T* const* Source, S Count) { MoveMemory(Destination, Source, Count*sizeof(T*)); }
	static inline VOID OverWriteItem(T** Item, T* NewItem) { *Item=NewItem; }
	static inline VOID OverWriteItems(T** Items, T* const* NewItems, S Count) { CopyMemory(Items, NewItems, Count*sizeof(T*)); }
	static inline VOID SetAt(T** Items, S Count, S Position, T* Set)
		{
		ASSERT(Position<Count);
		PointerAssign(&Items[Position], Set);
		}
	static inline VOID SetItem(T** Item, T* NewItem) { PointerAssign(Item, NewItem); }

private:
	// Con-/Destructors
	ArrayHelperTyped() {}
};

#ifdef __cplusplus_winrt
// Typed Array-Helper Handles
template <class T, class S>
class ArrayHelperTyped<T^, S, false>: public ArrayHelperBase<T^, S>
{
public:
	// Search
	static inline BOOL Contains(T^ const* Items, S Count, T^ Item) { return ContainsInternal(Items, Count, Item); }
	static inline BOOL GetPosition(T^ const* Items, S Count, T^ Item, S* Position) { return GetPositionInternal(Items, Count, Item, Position); }

	// Assignment
	static inline VOID InitItem(T^* Item) { ZeroMemory(Item, sizeof(T^)); }
	static inline VOID InitItems(T^* Items, S Count) { ZeroMemory(Items, Count*sizeof(T^)); }

	// Modification
	static inline VOID DestroyItem(T^* Item) { *Item=nullptr; }
	static inline VOID DestroyItems(T^* Items, S Count)
		{
		for(S u=0; u<Count; u++)
			Items[u]=nullptr;
		}
	static inline VOID MoveItems(T^* Destination, T^ const* Source, S Count) { MoveMemory(Destination, Source, Count*sizeof(T^)); }
	static inline VOID OverWriteItem(T^* Item, T^ NewItem) { ZeroMemory(Item, sizeof(T^)); *Item=NewItem; }
	static inline VOID OverWriteItems(T^* Items, T^ const* NewItems, S Count)
		{
		ZeroMemory(Items, Count*sizeof(T^));
		for(S u=0; u<Count; u++)
			Items[u]=NewItems[u];
		}
	static inline VOID SetAt(T^* Items, S Count, S Position, T^ Set)
		{
		ASSERT(Position<Count);
		Items[Position]=Set;
		}
	static inline VOID SetItem(T^* Item, T^ NewItem) { *Item=NewItem; }

private:
	// Con-/Destructors
	ArrayHelperTyped() {}
};
#endif

}


//=====================
// Shared Array-Helper
//=====================

namespace Details {

template <class T, class S>
class ArrayHelperShared: public ArrayHelperTyped<T, S, IsIntegratedType<T>::value>
{
public:
	// Assignment
	static S Assign(T** Items, S* Size, S* Count, UINT BlockSize, T const* NewItems, S NewCount)
		{
		T* pitems=*Items;
		DestroyItems(pitems, *Count);
		S usize=*Size;
		S unewsize=BlockAlign<S>(NewCount, BlockSize);
		if(unewsize>usize)
			{
			operator delete(pitems);
			pitems=(T*)operator new(unewsize*sizeof(T));
			usize=unewsize;
			*Items=pitems;
			if(Size)
				*Size=usize;
			}
		OverWriteItems(pitems, NewItems, NewCount);
		*Count=NewCount;
		return NewCount;
		}

	// Modification
	static T* Append(T* Items, S Size, S* Count)
		{
		UINT ucount=*Count;
		ASSERT(ucount<Size);
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
	static S Append(T* Items, S Size, S* Count, T const* Insert, S InsertCount, BOOL CopyOnly=false)
		{
		S ucount=*Count;
		S unewcount=ucount+InsertCount;
		ASSERT(unewcount<=Size);
		if(CopyOnly)
			{
			CopyMemory(&Items[ucount], Insert, InsertCount*sizeof(T));
			}
		else
			{
			OverWriteItems(&Items[ucount], Insert, InsertCount);
			}
		*Count=unewcount;
		return unewcount;
		}
	static S Append(T** Items, S* Size, S* Count, UINT BlockSize, T const* NewItems, S InsertCount)
		{
		S ucount=*Count;
		S unewcount=ucount+InsertCount;
		if(unewcount>*Size)
			SetSize(Items, Size, Count, unewcount, BlockSize);
		T* pitems=*Items;
		OverWriteItems(&pitems[ucount], NewItems, InsertCount);
		*Count=unewcount;
		return unewcount;
		}
	static VOID Clear(T** Items, S* Size, S* Count)
		{
		DestroyItems(*Items, *Count);
		operator delete(*Items);
		*Items=nullptr;
		*Size=0;
		*Count=0;
		}
	static T* InsertAt(T* Items, S Size, S* Count, S Position)
		{
		S ucount=*Count;
		ASSERT(ucount<Size);
		ASSERT(Position<=ucount);
		if(Position<ucount)
			MoveItems(&Items[Position+1], &Items[Position], ucount-Position);
		*Count=ucount+1;
		return &Items[Position];
		}
	static T* InsertAt(T** Items, S* Size, S* Count, UINT BlockSize, S Position)
		{
		S ucount=*Count;
		ASSERT(Position<=ucount);
		S unewcount=ucount+1;
		if(unewcount>*Size)
			SetSize(Items, Size, Count, BlockSize, unewcount);
		T* pitems=*Items;
		if(Position<ucount)
			MoveItems(&pitems[Position+1], &pitems[Position], ucount-Position);
		*Count=unewcount;
		return &pitems[Position];
		}
	static S InsertAt(T* Items, S Size, S* Count, S Position, T const* Insert, S InsertCount, BOOL CopyOnly=false)
		{
		S ucount=*Count;
		ASSERT(Position<=ucount);
		S unewcount=ucount+InsertCount;
		ASSERT(unewcount<=Size);
		if(Position<ucount)
			MoveItems(&Items[Position+InsertCount], &Items[Position], ucount-Position);
		if(CopyOnly)
			{
			CopyMemory(&Items[Position], Insert, InsertCount*sizeof(T));
			}
		else
			{
			OverWriteItems(&Items[Position], Insert, InsertCount);
			}
		*Count=unewcount;
		return unewcount;
		}
	static S InsertAt(T** Items, S* Size, S* Count, UINT BlockSize, S Position, T const* NewItems, S NewCount)
		{
		S ucount=*Count;
		S unewcount=ucount+NewCount;
		if(unewcount>*Size)
			SetSize(Items, Size, Count, unewcount, BlockSize);
		T* pitems=*Items;
		MoveItems(&pitems[Position+NewCount], &pitems[Position], ucount-Position);
		OverWriteItems(&pitems[Position], NewItems, NewCount);
		*Count=unewcount;
		return unewcount;
		}
	static BOOL Remove(T** Items, S* Size, S* Count, UINT BlockSize, T const* Remove)
		{
		T* pitems=*Items;
		S ucount=*Count;
		for(S u=0; u<ucount; u++)
			{
			if(pitems[u]==*Remove)
				{
				RemoveAt(Items, Size, Count, u, 1, BlockSize);
				return true;
				}
			}
		return false;
		}
	static S Remove(T** Items, S* Size, S* Count, UINT BlockSize, T const* Remove, S RemoveCount)
		{
		T* pitems=*Items;
		S ucount=*Count;
		for(S uitem=0; uitem<RemoveCount; uitem++)
			{
			for(S u=0; u<ucount; )
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
		Shrink(Items, Size, Count, BlockSize);
		return ucount;
		}
	static S RemoveAt(T* Items, S* Count, S Position, S RemoveCount=1, BOOL RemoveOnly=false)
		{
		S ucount=*Count;
		ASSERT(Position+RemoveCount<=ucount);
		if(!RemoveOnly)
			DestroyItems(&Items[Position], RemoveCount);
		MoveItems(&Items[Position], &Items[Position+RemoveCount], ucount-Position-RemoveCount);
		ucount-=RemoveCount;
		*Count=ucount;
		return ucount;
		}
	static S RemoveAt(T** Items, S* Size, S* Count, UINT BlockSize, S Position, S RemoveCount=1, BOOL RemoveOnly=false, BOOL DoShrink=true)
		{
		S ucount=*Count;
		ASSERT(Position+RemoveCount<=ucount);
		T* pitems=*Items;
		if(!RemoveOnly)
			DestroyItems(&pitems[Position], RemoveCount);
		MoveItems(&pitems[Position], &pitems[Position+RemoveCount], ucount-Position-RemoveCount);
		ucount-=RemoveCount;
		*Count=ucount;
		if(DoShrink)
			Shrink(Items, Size, Count, BlockSize);
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
			DestroyItems(&pitems[NewCount], ucount-NewCount);
		S unewsize=BlockAlign<S>(NewCount, BlockSize);
		if(unewsize!=*Size)
			{
			T* pold=pitems;
			pitems=(T*)operator new(unewsize*sizeof(T));
			S ucopy=min(ucount, NewCount);
			MoveItems(pitems, pold, ucopy);
			operator delete(pold);
			*Items=pitems;
			*Size=unewsize;
			}
		if(NewCount>ucount)
			InitItems(&pitems[ucount], NewCount-ucount);
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
		S unewcount=min(unewsize, ucount);
		if(unewcount<ucount)
			DestroyItems(&pitems[unewcount], ucount-unewcount);
		*Items=(T*)operator new(unewsize*sizeof(T));
		S ucopy=min(ucount, unewcount);
		MoveItems(*Items, pitems, ucopy);
		operator delete(pitems);
		*Size=unewsize;
		*Count=unewcount;
		}
	static VOID Shrink(T** Items, S* Size, S* Count, UINT BlockSize)
		{
		S usize=*Size;
		S ucount=*Count;
		S unewsize=BlockAlign(ucount, BlockSize);
		if(unewsize==usize)
			return;
		T* pitems=(T*)operator new(unewsize*sizeof(T));
		MoveItems(pitems, *Items, ucount);
		operator delete(*Items);
		*Items=pitems;
		*Size=unewsize;
		}

private:
	// Con-/Destructors
	ArrayHelperShared() {}
};

}


//==============
// Array-Helper
//==============

template <class T, class S>
class ArrayHelper: public ::Details::ArrayHelperShared<T, S>
{
private:
	// Con-/Destructors
	ArrayHelper() {}
};

// Array-Helper Pointers
template <class T, class S>
class ArrayHelper<T*, S>: public ::Details::ArrayHelperShared<T*, S>
{
public:
	// Modification
	static BOOL Release(T** Items, S* Count, T* Item)
		{
		S upos=0;
		if(GetItemPos(Items, *Count, Item, &upos))
			{
			ReleaseAt(Items, Count, upos);
			return true;
			}
		return false;
		}
	static BOOL Release(T*** Items, S* Size, S* Count, T* Item, UINT BlockSize)
		{
		S upos=0;
		if(GetItemPos(*Items, Count, Item, &upos))
			{
			ReleaseAt(Items, Size, Count, BlockSize, upos);
			return true;
			}
		return false;
		}
	static T* ReleaseAt(T** Items, S* Count, S Position)
		{
		S ucount=*Count;
		ASSERT(Position<ucount);
		T* pitem=Items[Position];
		MoveItems(&Items[Position], &Items[Position+1], ucount-Position-1);
		ucount--;
		*Count=ucount;
		return pitem;
		}
	static T* ReleaseAt(T*** Items, S* Size, S* Count, UINT BlockSize, S Position)
		{
		S ucount=*Count;
		ASSERT(Position+1<=ucount);
		T* pitems=*Items;
		T pitem=ppitems[Position];
		MoveItems(&pitems[Position], &pitems[Position+1], ucount-Position-1);
		*Count--;
		Shrink(Items, Size, Count, BlockSize);
		return pitem;
		}

private:
	// Con-/Destructors
	ArrayHelper() {}
};
