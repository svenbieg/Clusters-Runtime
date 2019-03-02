//==================
// ExceptionClass.h
//==================

#pragma once


//=======
// Using
//=======

#include "TypeHelper.h"


//===========
// Exception
//===========

enum class Exception: UINT
{
None,
Error,
InvalidArgument,
NotImplemented,
OutOfMemory,
InvalidHandle,
NoLock,
DeadLock,
AccessDenied,
ReadError,
WriteError,
FileNotFound
};
