//==============
// IndexGroup.h
//==============

#pragma once


//=======
// Using
//=======

#include "..\Cluster\Group.h"
#include "..\StringClass.h"
#include "IndexItem.h"


//===========
// Namespace
//===========

namespace Clusters {
	namespace Details {
		namespace Index {


//========================
// Base-Class Group Index
//========================

template <class ID, class ITEM>
class IndexGroupBase: public ::Clusters::Details::Cluster::Group<IndexItem<ID, ITEM>>
{
protected:
	// Using
	using INDEXITEM=IndexItem<ID, ITEM>;

public:
	// Access
	virtual INDEXITEM* GetFirst()=0;
	virtual INDEXITEM const* GetFirst()const=0;
	virtual INDEXITEM* GetLast()=0;
	virtual INDEXITEM const* GetLast()const=0;
};


//=============
// Group Index
//=============

// Group Index
template <class ID, class ITEM>
class IndexGroup: public IndexGroupBase<ID, ITEM>
{
public:
	// Access
	virtual BOOL Contains(ID const& Id)const=0;
	virtual INT Find(ID const& Item)const=0;
	virtual INDEXITEM* Get(ID const& Id)=0;
	virtual INDEXITEM const* Get(ID const& Id)const=0;

	// Modification
	virtual BOOL Add(INDEXITEM** Item, ID const& Id, BOOL Again)=0;
	virtual BOOL Remove(ID const& Id)=0;

protected:
	// Common
	static inline BOOL IsAbove(ID const& First, ID const& Second) { return First>Second; }
	static inline BOOL IsBelow(ID const& First, ID const& Second) { return First<Second; }
	static inline BOOL IsEqual(ID const& First, ID const& Second) { return First==Second; }
};

// Group Pointer-Index
template <class ID, class ITEM>
class IndexGroup<ID*, ITEM>: public IndexGroupBase<ID*, ITEM>
{
public:
	// Access
	virtual BOOL Contains(ID* Id)const=0;
	virtual INT Find(ID* Id)const=0;
	virtual INDEXITEM* Get(ID* Id)=0;
	virtual INDEXITEM const* Get(ID* Id)const=0;

	// Modification
	virtual BOOL Add(INDEXITEM** Item, ID* Id, BOOL Again)=0;
	virtual BOOL Remove(ID* Id)=0;

protected:
	// Common
	static inline BOOL IsAbove(ID* First, ID* Second) { return First>Second; }
	static inline BOOL IsBelow(ID* First, ID* Second) { return First<Second; }
	static inline BOOL IsEqual(ID* First, ID* Second) { return First==Second; }
};

#ifdef __cplusplus_winrt
// Group Handle-Index
template <class ID, class ITEM>
class IndexGroup<ID^, ITEM>: public IndexGroupBase<ID^, ITEM>
{
public:
	// Access
	virtual BOOL Contains(ID^ Id)const=0;
	virtual INT Find(ID^ Id)const=0;
	virtual INDEXITEM* Get(ID^ Id)=0;
	virtual INDEXITEM const* Get(ID^ Id)const=0;

	// Modification
	virtual BOOL Add(INDEXITEM** Item, ID^ Id, BOOL Again)=0;
	virtual BOOL Remove(ID^ Id)=0;

protected:
	// Common
	static inline BOOL IsAbove(ID^ First, ID^ Second) { return First>Second; }
	static inline BOOL IsBelow(ID^ First, ID^ Second) { return First<Second; }
	static inline BOOL IsEqual(ID^ First, ID^ Second) { return First==Second; }
};
#endif

#ifdef __cplusplus_winrt
// Group Platform::String-Index
template <class ITEM>
class IndexGroup<Platform::String^, ITEM>: public IndexGroupBase<Platform::String^, ITEM>
{
public:
	// Access
	virtual BOOL Contains(Platform::String^ Id)const=0;
	virtual INT Find(Platform::String^ Id)const=0;
	virtual INDEXITEM* Get(Platform::String^ Id)=0;
	virtual INDEXITEM const* Get(Platform::String^ Id)const=0;

	// Modification
	virtual BOOL Add(INDEXITEM** Item, Platform::String^ Id, BOOL Again)=0;
	virtual BOOL Remove(Platform::String^ Id)=0;

protected:
	// Common
	static inline BOOL IsAbove(Platform::String^ First, Platform::String^ Second) { return StringCompare(First->Begin(), Second->Begin())>0; }
	static inline BOOL IsBelow(Platform::String^ First, Platform::String^ Second) { return StringCompare(First->Begin(), Second->Begin())<0; }
	static inline BOOL IsEqual(Platform::String^ First, Platform::String^ Second) { return StringCompare(First->Begin(), Second->Begin())==0; }
};
#endif

// Group String-Index
template <class CHAR, BOOL _AllocId, class ITEM>
class IndexGroup<String<CHAR, _AllocId>, ITEM>: public IndexGroupBase<String<CHAR, _AllocId>, ITEM>
{
public:
	// Access
	virtual BOOL Contains(CHAR const* Id, UINT Length, BOOL CaseSensitive)const=0;
	virtual INT Find(CHAR const* Id, UINT Length, BOOL CaseSensitive)const=0;
	virtual INDEXITEM* Get(CHAR const* Id, UINT Length, BOOL CaseSensitive)=0;
	virtual INDEXITEM const* Get(CHAR const* Id, UINT Length, BOOL CaseSensitive)const=0;

	// Modification
	virtual BOOL Add(INDEXITEM** Item, CHAR const* Id, UINT Length, BOOL CaseSensitive, BOOL Again)=0;
	virtual BOOL Remove(CHAR const* Id, UINT Length, BOOL CaseSensitive)=0;

protected:
	// Common
	static inline BOOL IsAbove(CHAR const* First, CHAR const* Second, UINT Length, BOOL CaseSensitive) { return StringCompare(First, 0, Second, Length, CaseSensitive)>0; }
	static inline BOOL IsBelow(CHAR const* First, CHAR const* Second, UINT Length, BOOL CaseSensitive) { return StringCompare(First, 0, Second, Length, CaseSensitive)<0; }
	static inline BOOL IsEqual(CHAR const* First, CHAR const* Second, UINT Length, BOOL CaseSensitive) { return StringCompare(First, 0, Second, Length, CaseSensitive)==0; }
};

}}}
