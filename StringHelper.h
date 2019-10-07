//================
// StringHelper.h
//================

#pragma once


//========
// Common
//========

INT CharCompare(WCHAR Value1, WCHAR Value2);
INT StringCompare(LPCWSTR Value1, LPCWSTR Value2);
DOUBLE StringToDouble(LPCWSTR Value, UINT* Length=nullptr);
