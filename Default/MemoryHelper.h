//================
// MemoryHelper.h
//================

#pragma once


//=======
// Using
//=======

#include <malloc.h>
#include <memory.h>
#include <new>
#include "Architecture.h"
#include "ErrorHelper.h"


//============
// Addressing
//============

#ifndef PAGE_SIZE
#define PAGE_SIZE 0x1000
#endif

#define ALIGN(n) __declspec(align(n))

#ifdef _BITS32
#define ALIGN_AUTO __declspec(4)
#else
#define ALIGN_AUTO __declspec(align(8))
#endif

#ifndef offsetof
#define offsetof(s,m) ((size_t)&(((s*)0)->m))
#endif

template <class S>
inline S BlockAlign(S Size, UINT BlockSize)
{
if(Size%BlockSize)
	return Size+(BlockSize-(Size%BlockSize));
return Size;
}

template <typename S>
inline SIZE_T ToSIZET(S Value)
{
ASSERT(Value<=MAX_SIZE_T);
return (SIZE_T)Value;
}

template <typename S>
inline UINT ToUINT(S Value)
{
ASSERT(Value<=0xFFFFFFFF);
return (UINT)Value;
}

inline UINT64 ToUINT64(UINT High, UINT Low)
{
return ((UINT64)High<<32)|Low;
}

inline WORD ToWORD(UINT Value)
{
ASSERT(Value<=0xFFFF);
return (WORD)Value;
}


//==========
// Pointers
//==========

template <class T>
inline VOID PointerAssign(T** Pointer, T const* Value)
{
if(*Pointer)
	delete *Pointer;
*Pointer=(T*)Value;
}

template <class T>
inline VOID PointerFree(T* Pointer)
{
if(Pointer)
	delete Pointer;
}

template <class T>
inline T* PointerRelease(T** Pointer)
{
T* p=*Pointer;
*Pointer=(T*)0;
return p;
}

template <class T>
inline BOOL PointerValid(T Pointer)
{
return (VOID*)Pointer>(VOID*)PAGE_SIZE;
}


//=======
// Flags
//=======

#undef ClearFlag
#undef SetFlag

template <class T, class V> inline VOID ClearFlag(T& Flags, V Clear) { (UINT&)Flags&=~(UINT)Clear; }
template <class T, class V> inline BOOL GetFlag(T& Flags, V Get) {return ((UINT)Flags&(UINT)Get)>0; }
template <class T, class V> inline VOID SetFlag(T& Flags, V Set) { (UINT&)Flags|=(UINT)Set; }
template <class T, class V> inline VOID SetFlag(T& Flags, V Change, BOOL Set) { Set? SetFlag(Flags, Change): ClearFlag(Flags, Change); }


//========
// Common
//========

inline INT CompareMemory(const VOID* Buffer1, const VOID* Buffer2, SIZE_T Size)
{
ASSERT(Buffer1&&Buffer2&&Size);
return memcmp(Buffer1, Buffer2, Size);
}

#undef CopyMemory
inline VOID CopyMemory(VOID* Destination, const VOID* Source, SIZE_T Size)
{
if(Size)
	{
	ASSERT(Destination&&Source);
	memcpy(Destination, Source, Size);
	}
}

#undef MoveMemory
inline VOID MoveMemory(VOID* Destination, const VOID* Source, SIZE_T Size)
{
if(Size)
	{
	ASSERT(Destination&&Source);
	memmove(Destination, Source, Size);
	}
}

#undef ZeroMemory
inline VOID ZeroMemory(VOID* Destination, SIZE_T Size)
{
if(Size)
	{
	ASSERT(Destination);
	memset(Destination, 0, Size);
	}
}


//==================
// Integrated Types
//==================

template <class T> struct IsIntegratedType { static const bool value=false; };
template <> struct IsIntegratedType<bool> { static const bool value=true; };
template <> struct IsIntegratedType<char> { static const bool value=true; };
template <> struct IsIntegratedType<wchar_t> { static const bool value=true; };
template <> struct IsIntegratedType<unsigned char> { static const bool value=true; };
template <> struct IsIntegratedType<short> { static const bool value=true; };
template <> struct IsIntegratedType<unsigned short> { static const bool value=true; };
template <> struct IsIntegratedType<int> { static const bool value=true; };
template <> struct IsIntegratedType<unsigned int> { static const bool value=true; };
template <> struct IsIntegratedType<long int> { static const bool value=true; };
template <> struct IsIntegratedType<unsigned long int> { static const bool value=true; };
