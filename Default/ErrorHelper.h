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
#define ASSERT(Condition) if(!(Condition))throw EINVAL
