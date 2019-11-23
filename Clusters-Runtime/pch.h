//=======
// pch.h
//=======

#pragma once


//=======
// Using
//=======

#include <Windows.h>
#include <concrt.h>


//========
// Common
//========

#include "Handle.h"

using Object=Platform::Object;
using String=Platform::String;

using CriticalSection=Concurrency::critical_section;
using ScopedLock=Concurrency::critical_section::scoped_lock;
