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
	virtual unsigned int GetChildCount()const=0;
	virtual size_t GetItemCount()const=0;
	virtual unsigned int GetLevel()const=0;

	// Modification
	virtual void RemoveAt(size_t Position)=0;

protected:
	// Con-Destructors
	virtual ~GroupBase() {}
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
	virtual _Item* GetAt(size_t Position)=0;
	virtual _Item const* GetAt(size_t Position)const=0;
};

// Group Pointer-Cluster
template <class _Item>
class Group<_Item*>: public GroupBase<_Item*>
{
public:
	// Access
	virtual _Item* GetAt(size_t Position)const=0;

	// Modification
	virtual _Item* ReleaseAt(size_t Position)=0;
};

#ifdef __cplusplus_winrt
// Group Handle-Cluster
template <class _Item>
class Group<_Item^>: public GroupBase<_Item^>
{
public:
	// Access
	virtual _Item^ GetAt(size_t Position)const=0;
};
#endif

// Group String-Cluster
template <class _Char, bool _Alloc>
class Group<String<_Char, _Alloc>>: public GroupBase<String<_Char, _Alloc>>
{
public:
	// Access
	virtual _Char const* GetAt(size_t Position)const=0;

	// Modification
	virtual _Char const* ReleaseAt(size_t Position)=0;
};

}}}}
