//================
// Architecture.h
//================

#pragma once


//==============
// Architecture
//==============

#ifdef _BITS32

#define IF32(x) x
#define IF64(x)

#define MAX_SIZE_T 0xFFFFFFFF
#else

#define IF32(x)
#define IF64(x) x

#define MAX_SIZE_T 0xFFFFFFFFFFFFFFFF

#endif
