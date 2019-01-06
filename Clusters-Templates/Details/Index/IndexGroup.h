//==============
// IndexGroup.h
//==============

#pragma once


//=======
// Using
//=======

#include "../Cluster/Group.h"
#include "../StringClass.h"
#include "IndexItem.h"


//===========
// Namespace
//===========

namespace Clusters {
	namespace Templates {
		namespace Details {
			namespace Index {


//========================
// Base-Class Group Index
//========================

template <class _Id, class _Item>
class IndexGroupBase: public ::Clusters::Templates::Details::Cluster::Group<IndexItem<_Id, _Item>>
{
private:
	// Using
	using _IndexItem=IndexItem<_Id, _Item>;

public:
	// Access
	virtual _IndexItem* GetFirst()=0;
	virtual _IndexItem const* GetFirst()const=0;
	virtual _IndexItem* GetLast()=0;
	virtual _IndexItem const* GetLast()const=0;
};


//=============
// Group Index
//=============

// Group Index
template <class _Id, class _Item>
class IndexGroup: public IndexGroupBase<_Id, _Item>
{
private:
	// Using
	using _IndexItem=IndexItem<_Id, _Item>;

public:
	// Access
	virtual BOOL Contains(_Id const& Id)const=0;
	virtual INT Find(_Id const& Item)const=0;
	virtual _IndexItem* Get(_Id const& Id)=0;
	virtual _IndexItem const* Get(_Id const& Id)const=0;

	// Modification
	virtual BOOL Add(_IndexItem** Item, _Id const& Id, BOOL Again)=0;
	virtual BOOL Remove(_Id const& Id)=0;

protected:
	// Common
	static inline BOOL IsAbove(_Id const& First, _Id const& Second) { return First>Second; }
	static inline BOOL IsBelow(_Id const& First, _Id const& Second) { return First<Second; }
	static inline BOOL IsEqual(_Id const& First, _Id const& Second) { return First==Second; }
};

// Group Pointer-Index
template <class _Id, class _Item>
class IndexGroup<_Id*, _Item>: public IndexGroupBase<_Id*, _Item>
{
private:
	// Using
	using _IndexItem=IndexItem<_Id*, _Item>;

public:
	// Access
	virtual BOOL Contains(_Id* Id)const=0;
	virtual INT Find(_Id* Id)const=0;
	virtual _IndexItem* Get(_Id* Id)=0;
	virtual _IndexItem const* Get(_Id* Id)const=0;

	// Modification
	virtual BOOL Add(_IndexItem** Item, _Id* Id, BOOL Again)=0;
	virtual BOOL Remove(_Id* Id)=0;

protected:
	// Common
	static inline BOOL IsAbove(_Id* First, _Id* Second) { return First>Second; }
	static inline BOOL IsBelow(_Id* First, _Id* Second) { return First<Second; }
	static inline BOOL IsEqual(_Id* First, _Id* Second) { return First==Second; }
};

#ifdef __cplusplus_winrt
// Group Handle-Index
template <class _Id, class _Item>
class IndexGroup<_Id^, _Item>: public IndexGroupBase<_Id^, _Item>
{
private:
	// Using
	using _IndexItem=IndexItem<_Id^, _Item>;

public:
	// Access
	virtual BOOL Contains(_Id^ Id)const=0;
	virtual INT Find(_Id^ Id)const=0;
	virtual _IndexItem* Get(_Id^ Id)=0;
	virtual _IndexItem const* Get(_Id^ Id)const=0;

	// Modification
	virtual BOOL Add(_IndexItem** Item, _Id^ Id, BOOL Again)=0;
	virtual BOOL Remove(_Id^ Id)=0;

protected:
	// Common
	static inline BOOL IsAbove(_Id^ First, _Id^ Second) { return First>Second; }
	static inline BOOL IsBelow(_Id^ First, _Id^ Second) { return First<Second; }
	static inline BOOL IsEqual(_Id^ First, _Id^ Second) { return First==Second; }
};
#endif

#ifdef __cplusplus_winrt
// Group Platform::String-Index
template <class _Item>
class IndexGroup<Platform::String^, _Item>: public IndexGroupBase<Platform::String^, _Item>
{
private:
	// Using
	using _IndexItem=IndexItem<Platform::String^, _Item>;

public:
	// Access
	virtual BOOL Contains(Platform::String^ Id)const=0;
	virtual INT Find(Platform::String^ Id)const=0;
	virtual _IndexItem* Get(Platform::String^ Id)=0;
	virtual _IndexItem const* Get(Platform::String^ Id)const=0;

	// Modification
	virtual BOOL Add(_IndexItem** Item, Platform::String^ Id, BOOL Again)=0;
	virtual BOOL Remove(Platform::String^ Id)=0;

protected:
	// Common
	static inline BOOL IsAbove(Platform::String^ First, Platform::String^ Second) { return StringCompare(First->Begin(), Second->Begin())>0; }
	static inline BOOL IsBelow(Platform::String^ First, Platform::String^ Second) { return StringCompare(First->Begin(), Second->Begin())<0; }
	static inline BOOL IsEqual(Platform::String^ First, Platform::String^ Second) { return StringCompare(First->Begin(), Second->Begin())==0; }
};
#endif

// Group String-Index
template <class _Char, BOOL _AllocId, class _Item>
class IndexGroup<String<_Char, _AllocId>, _Item>: public IndexGroupBase<String<_Char, _AllocId>, _Item>
{
private:
	// Using
	using _IndexItem=IndexItem<String<_Char, _AllocId>, _Item>;

public:
	// Access
	virtual BOOL Contains(_Char const* Id, UINT Length, BOOL CaseSensitive)const=0;
	virtual INT Find(_Char const* Id, UINT Length, BOOL CaseSensitive)const=0;
	virtual _IndexItem* Get(_Char const* Id, UINT Length, BOOL CaseSensitive)=0;
	virtual _IndexItem const* Get(_Char const* Id, UINT Length, BOOL CaseSensitive)const=0;

	// Modification
	virtual BOOL Add(_IndexItem** Item, _Char const* Id, UINT Length, BOOL CaseSensitive, BOOL Again)=0;
	virtual BOOL Remove(_Char const* Id, UINT Length, BOOL CaseSensitive)=0;

protected:
	// Common
	static inline BOOL IsAbove(_Char const* First, _Char const* Second, UINT Length, BOOL CaseSensitive) { return StringCompare(First, 0, Second, Length, CaseSensitive)>0; }
	static inline BOOL IsBelow(_Char const* First, _Char const* Second, UINT Length, BOOL CaseSensitive) { return StringCompare(First, 0, Second, Length, CaseSensitive)<0; }
	static inline BOOL IsEqual(_Char const* First, _Char const* Second, UINT Length, BOOL CaseSensitive) { return StringCompare(First, 0, Second, Length, CaseSensitive)==0; }
};

}}}}
