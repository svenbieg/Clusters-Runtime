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
	namespace Templates {
		namespace Details {
			namespace Index {


//========================
// Base-Class Group Index
//========================

template <class _Id, class _Item>
class IndexGroupBase: public ::Clusters::Templates::Details::Cluster::Group<IndexItem<_Id, _Item>>
{
protected:
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
public:
	// Access
	virtual bool Contains(_Id const& Id)const=0;
	virtual int Find(_Id const& Item)const=0;
	virtual _IndexItem* Get(_Id const& Id)=0;
	virtual _IndexItem const* Get(_Id const& Id)const=0;

	// Modification
	virtual bool Add(_IndexItem** Item, _Id const& Id, bool Again)=0;
	virtual bool Remove(_Id const& Id)=0;

protected:
	// Common
	static inline bool IsAbove(_Id const& First, _Id const& Second) { return First>Second; }
	static inline bool IsBelow(_Id const& First, _Id const& Second) { return First<Second; }
	static inline bool IsEqual(_Id const& First, _Id const& Second) { return First==Second; }
};

// Group Pointer-Index
template <class _Id, class _Item>
class IndexGroup<_Id*, _Item>: public IndexGroupBase<_Id*, _Item>
{
public:
	// Access
	virtual bool Contains(_Id* Id)const=0;
	virtual int Find(_Id* Id)const=0;
	virtual _IndexItem* Get(_Id* Id)=0;
	virtual _IndexItem const* Get(_Id* Id)const=0;

	// Modification
	virtual bool Add(_IndexItem** Item, _Id* Id, bool Again)=0;
	virtual bool Remove(_Id* Id)=0;

protected:
	// Common
	static inline bool IsAbove(_Id* First, _Id* Second) { return First>Second; }
	static inline bool IsBelow(_Id* First, _Id* Second) { return First<Second; }
	static inline bool IsEqual(_Id* First, _Id* Second) { return First==Second; }
};

#ifdef __cplusplus_winrt
// Group Handle-Index
template <class _Id, class _Item>
class IndexGroup<_Id^, _Item>: public IndexGroupBase<_Id^, _Item>
{
public:
	// Access
	virtual bool Contains(_Id^ Id)const=0;
	virtual int Find(_Id^ Id)const=0;
	virtual _IndexItem* Get(_Id^ Id)=0;
	virtual _IndexItem const* Get(_Id^ Id)const=0;

	// Modification
	virtual bool Add(_IndexItem** Item, _Id^ Id, bool Again)=0;
	virtual bool Remove(_Id^ Id)=0;

protected:
	// Common
	static inline bool IsAbove(_Id^ First, _Id^ Second) { return First>Second; }
	static inline bool IsBelow(_Id^ First, _Id^ Second) { return First<Second; }
	static inline bool IsEqual(_Id^ First, _Id^ Second) { return First==Second; }
};
#endif

#ifdef __cplusplus_winrt
// Group Platform::String-Index
template <class _Item>
class IndexGroup<Platform::String^, _Item>: public IndexGroupBase<Platform::String^, _Item>
{
public:
	// Access
	virtual bool Contains(Platform::String^ Id)const=0;
	virtual int Find(Platform::String^ Id)const=0;
	virtual _IndexItem* Get(Platform::String^ Id)=0;
	virtual _IndexItem const* Get(Platform::String^ Id)const=0;

	// Modification
	virtual bool Add(_IndexItem** Item, Platform::String^ Id, bool Again)=0;
	virtual bool Remove(Platform::String^ Id)=0;

protected:
	// Common
	static inline bool IsAbove(Platform::String^ First, Platform::String^ Second) { return StringCompare(First->Begin(), Second->Begin())>0; }
	static inline bool IsBelow(Platform::String^ First, Platform::String^ Second) { return StringCompare(First->Begin(), Second->Begin())<0; }
	static inline bool IsEqual(Platform::String^ First, Platform::String^ Second) { return StringCompare(First->Begin(), Second->Begin())==0; }
};
#endif

// Group String-Index
template <class _Char, bool _AllocId, class _Item>
class IndexGroup<String<_Char, _AllocId>, _Item>: public IndexGroupBase<String<_Char, _AllocId>, _Item>
{
public:
	// Access
	virtual bool Contains(_Char const* Id, unsigned int Length, bool CaseSensitive)const=0;
	virtual int Find(_Char const* Id, unsigned int Length, bool CaseSensitive)const=0;
	virtual _IndexItem* Get(_Char const* Id, unsigned int Length, bool CaseSensitive)=0;
	virtual _IndexItem const* Get(_Char const* Id, unsigned int Length, bool CaseSensitive)const=0;

	// Modification
	virtual bool Add(_IndexItem** Item, _Char const* Id, unsigned int Length, bool CaseSensitive, bool Again)=0;
	virtual bool Remove(_Char const* Id, unsigned int Length, bool CaseSensitive)=0;

protected:
	// Common
	static inline bool IsAbove(_Char const* First, _Char const* Second, unsigned int Length, bool CaseSensitive) { return StringCompare(First, 0, Second, Length, CaseSensitive)>0; }
	static inline bool IsBelow(_Char const* First, _Char const* Second, unsigned int Length, bool CaseSensitive) { return StringCompare(First, 0, Second, Length, CaseSensitive)<0; }
	static inline bool IsEqual(_Char const* First, _Char const* Second, unsigned int Length, bool CaseSensitive) { return StringCompare(First, 0, Second, Length, CaseSensitive)==0; }
};

}}}}
