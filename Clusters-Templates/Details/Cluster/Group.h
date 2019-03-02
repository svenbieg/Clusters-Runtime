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

template <class _Item>
class GroupBase
{
public:
	// Access
	virtual UINT GetChildCount()const=0;
	virtual SIZE_T GetItemCount()const=0;
	virtual UINT GetLevel()const=0;

	// Modification
	virtual VOID RemoveAt(SIZE_T Position)=0;

protected:
	// Con-Destructors
	virtual ~GroupBase() {};
};


//===============
// Group Cluster
//===============

// Group Cluster
template <class _Item>
class Group: public GroupBase<_Item>
{
public:
	// Access
	virtual _Item* GetAt(SIZE_T Position)=0;
	virtual _Item const* GetAt(SIZE_T Position)const=0;
};

// Group Pointer-Cluster
template <class _Item>
class Group<_Item*>: public GroupBase<_Item*>
{
public:
	// Access
	virtual _Item* GetAt(SIZE_T Position)const=0;

	// Modification
	virtual _Item* ReleaseAt(SIZE_T Position)=0;
};

#ifdef __cplusplus_winrt
// Group Handle-Cluster
template <class _Item>
class Group<_Item^>: public GroupBase<_Item^>
{
public:
	// Access
	virtual _Item^ GetAt(SIZE_T Position)const=0;
};
#endif

// Group String-Cluster
template <class _Char, BOOL _Alloc>
class Group<StringItem<_Char, _Alloc>>: public GroupBase<StringItem<_Char, _Alloc>>
{
public:
	// Access
	virtual _Char const* GetAt(SIZE_T Position)const=0;

	// Modification
	virtual _Char const* ReleaseAt(SIZE_T Position)=0;
};

}}}}
