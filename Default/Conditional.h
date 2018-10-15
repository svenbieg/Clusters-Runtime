//===============
// Conditional.h
//===============

#pragma once


//===============
// Helper-Struct
//===============

namespace Details
{
template <BOOL _Condition, class _True, class _False>
struct Conditional
	{
	using T=_True;
	};
template <class _True, class _False>
struct Conditional<false, _True, _False>
	{
	using T=_False;
	};
}


//=============
// Conditional
//=============

template <BOOL _Condition, class _True, class _False>
using Conditional=typename ::Details::Conditional<_Condition, _True, _False>::T;
