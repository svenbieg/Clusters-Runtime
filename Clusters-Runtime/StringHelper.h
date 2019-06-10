//================
// StringHelper.h
//================

#pragma once


//=======
// Using
//=======

#include <stdarg.h>


//========
// Access
//========

UINT StringLength(LPCWSTR String, UINT Max=0);


//============
// Comparison
//============

INT StringCompare(LPCWSTR Value1, LPCWSTR Value2);


//============
// Conversion
//============

BOOL StringIsNumeric(LPCWSTR Value, UINT* Length=nullptr);
DOUBLE StringToDouble(LPCWSTR Value, UINT* Length=nullptr);
