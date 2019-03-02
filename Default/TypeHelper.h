//==============
// TypeHelper.h
//==============

#pragma once


//==================
// Integrated Types
//==================

#ifndef _WINDOWS

typedef void VOID;
typedef bool BOOL;

// Memory-Types
typedef unsigned char BYTE;
typedef unsigned short int WORD;
typedef unsigned int DWORD;
typedef unsigned long long int QWORD;
#ifdef _BITS32
typedef unsigned int SIZE_T;
#else
typedef unsigned long long int SIZE_T;
#endif

// Numeric Types
typedef short int SHORT;
typedef unsigned short int USHORT;
typedef int INT;
typedef unsigned int UINT;
typedef long long int INT64;
typedef unsigned long long int UINT64;
typedef float FLOAT;
typedef double DOUBLE;

// Characters
typedef char CHAR;
typedef char TCHAR;
typedef wchar_t WCHAR;

// Strings
typedef char* LPSTR;
typedef char const* LPCSTR;
typedef char* LPTSTR;
typedef char const* LPCTSTR;
typedef wchar_t* LPWSTR;
typedef wchar_t const* LPCWSTR;

#endif


//========
// Limits
//========

#ifndef MAXUINT
#define MAXUINT 0xFFFFFFFF
#endif

#ifndef MAX_SIZE_T
#ifdef _BITS32
#define MAX_SIZE_T 0xFFFFFFFFFFFFFFFF
#else
#define MAX_SIZE_T 0xFFFFFFFF
#endif
#endif


//================
// Moveable Types
//================

template <class T> struct TypeNeedsConstruction { static const bool value=true; };
template <> struct TypeNeedsConstruction<bool> { static const bool value=false; };
template <> struct TypeNeedsConstruction<CHAR> { static const bool value=false; };
template <> struct TypeNeedsConstruction<WCHAR> { static const bool value=false; };
template <> struct TypeNeedsConstruction<BYTE> { static const bool value=false; };
template <> struct TypeNeedsConstruction<SHORT> { static const bool value=false; };
template <> struct TypeNeedsConstruction<WORD> { static const bool value=false; };
template <> struct TypeNeedsConstruction<INT> { static const bool value=false; };
template <> struct TypeNeedsConstruction<UINT> { static const bool value=false; };
template <> struct TypeNeedsConstruction<INT64> { static const bool value=false; };
