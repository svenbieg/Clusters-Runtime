//===============
// ErrorHelper.h
//===============

#pragma once


//=======
// Using
//=======

#include "TypeHelper.h"


//============
// Exceptions
//============

#define EXCEPT catch(UINT _exc)
#define GetExceptionStatus() _exc

enum class Exception: UINT
{
None,
Error,
InvalidArgument,
NotImplemented,
InvalidHandle,
NoLock,
DeadLock,
AccessDenied,
ReadError,
WriteError,
FileNotFound
};


//========
// Common
//========

#undef ASSERT
#define ASSERT(Condition) if(!(Condition))throw(Exception::Error)
