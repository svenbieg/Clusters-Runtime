//==============
// CharHelper.h
//==============

#pragma once


//========
// Common
//========

BOOL CharIsAlpha(WCHAR Value);
BOOL CharIsDigit(WCHAR Value, BOOL Hex=false);
BOOL CharIsHex(WCHAR Value);


//============
// Conversion
//============

WCHAR CharToCapital(WCHAR Value);


//============
// Comparison
//============

INT CharCompare(WCHAR Value1, WCHAR Value2, BOOL CaseSensitive=true);
