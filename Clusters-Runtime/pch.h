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

template <typename T> using IIterable=Windows::Foundation::Collections::IIterable<T>;
template <typename T> using IIterator=Windows::Foundation::Collections::IIterator<T>;
template <typename K, typename V> using IKeyValuePair=Windows::Foundation::Collections::IKeyValuePair<K, V>;
