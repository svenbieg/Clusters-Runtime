//=========
// Group.h
//=========

#pragma once


//===========
// Namespace
//===========

namespace Clusters {
	namespace Templates {
		namespace Details {
			namespace Cluster {


//==========================
// Base-Class Group Cluster
//==========================

template <class ITEM>
class GroupBase
{
public:
	// Access
	virtual UINT GetChildCount()const=0;
	virtual UINT64 GetItemCount()const=0;
	virtual UINT GetLevel()const=0;

	// Modification
	virtual VOID RemoveAt(UINT64 Position)=0;

protected:
	// Con-Destructors
	virtual ~GroupBase() {}
};


//===============
// Group Cluster
//===============

// Group Cluster
template <class ITEM>
class Group: public GroupBase<ITEM>
{
public:
	// Access
	virtual ITEM* GetAt(UINT64 Position)=0;
	virtual ITEM const* GetAt(UINT64 Position)const=0;
};

// Group Pointer-Cluster
template <class ITEM>
class Group<ITEM*>: public GroupBase<ITEM*>
{
public:
	// Access
	virtual ITEM* GetAt(UINT64 Position)const=0;

	// Modification
	virtual ITEM* ReleaseAt(UINT64 Position)=0;
};

#ifdef __cplusplus_winrt
// Group Handle-Cluster
template <class ITEM>
class Group<ITEM^>: public GroupBase<ITEM^>
{
public:
	// Access
	virtual ITEM^ GetAt(UINT64 Position)const=0;
};
#endif

// Group String-Cluster
template <class CHAR, BOOL _Alloc>
class Group<String<CHAR, _Alloc>>: public GroupBase<String<CHAR, _Alloc>>
{
public:
	// Access
	virtual CHAR const* GetAt(UINT64 Position)const=0;

	// Modification
	virtual CHAR const* ReleaseAt(UINT64 Position)=0;
};

}}}}
