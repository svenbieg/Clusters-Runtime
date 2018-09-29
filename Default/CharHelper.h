//==============
// CharHelper.h
//==============

#pragma once


//========
// Common
//========

BOOL CharIsAlpha(CHAR Value);
BOOL CharIsAlpha(WCHAR Value);
BOOL CharIsBreak(CHAR Value);
BOOL CharIsBreak(WCHAR Value);
BOOL CharIsCapital(CHAR Value);
BOOL CharIsCapital(WCHAR Value);
BOOL CharIsDigit(CHAR Value, BOOL Hex=false);
BOOL CharIsDigit(WCHAR Value, BOOL Hex=false);
BOOL CharIsHex(CHAR Value);
BOOL CharIsHex(WCHAR Value);
BOOL CharIsSmall(CHAR Value);
BOOL CharIsSmall(WCHAR Value);
BOOL CharIsSpecial(CHAR Value);
BOOL CharIsSpecial(WCHAR Value);


//============
// Conversion
//============

CHAR CharToAnsi(WCHAR Value);
CHAR CharToCapital(CHAR Value);
WCHAR CharToCapital(WCHAR Value);
CHAR CharToSmall(CHAR Value);
WCHAR CharToSmall(WCHAR Value);
WCHAR CharToUnicode(CHAR Value);


//============
// Comparison
//============

INT CharCompare(CHAR Value1, CHAR Value2, BOOL CaseSensitive=true);
INT CharCompare(WCHAR Value1, WCHAR Value2, BOOL CaseSensitive=true);
inline INT CharCompare(CHAR Value1, WCHAR Value2, BOOL CaseSensitive=true) { return CharCompare(CharToUnicode(Value1), Value2, CaseSensitive); }
inline INT CharCompare(WCHAR Value1, CHAR Value2, BOOL CaseSensitive=true) { return CharCompare(Value1, CharToUnicode(Value2), CaseSensitive); }


//============
// Assignment
//============

inline VOID CharAssign(CHAR& Destination, CHAR Value) { Destination=Value; }
inline VOID CharAssign(CHAR& Destination, WCHAR Value) { Destination=CharToAnsi(Value); }
inline VOID CharAssign(WCHAR& Destination, CHAR Value) { Destination=CharToUnicode(Value); }
inline VOID CharAssign(WCHAR& Destination, WCHAR Value) { Destination=Value; }
