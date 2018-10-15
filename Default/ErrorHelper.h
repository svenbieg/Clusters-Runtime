//===============
// ErrorHelper.h
//===============

#pragma once


//=======
// Using
//=======

#include <errno.h>


//========
// Common
//========

#undef ASSERT
#define ASSERT(Condition) if(!(Condition))throw(EFAULT)


//============
// Exceptions
//============

#define EXCEPT catch(HRESULT _hexc)
#define GetExceptionStatus() _hexc
