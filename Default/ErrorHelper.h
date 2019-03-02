//===============
// ErrorHelper.h
//===============

#pragma once


//=======
// Using
//=======

#include "ExceptionClass.h"


//============
// Exceptions
//============

#define EXCEPT catch(UINT _exc)
#define GetExceptionStatus() _exc


//========
// Common
//========

#undef ASSERT
#define ASSERT(Condition) if(!(Condition))throw(Exception::Error)
