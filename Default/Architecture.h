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

#else

#define IF32(x)
#define IF64(x) x

#endif
