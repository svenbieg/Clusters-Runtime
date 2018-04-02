//==========
// Vector.h
//==========

#pragma once


//=======
// Using
//=======

#include "Details\Vector\Vector.h"


//===========
// Namespace
//===========

namespace Clusters {


//========
// Vector
//========

template <class ITEM, UINT _GroupSize=100>
class Vector: public Details::Vector::Vector<ITEM, _GroupSize>
{
private:
	// Using
	using VECTOR=Vector<ITEM, _GroupSize>;

public:
	// Con-/Destructors
	Vector() {}
	Vector(VECTOR const& Vector)
		{
		for(auto it=Vector.First(); it.HasCurrent(); it.MoveNext())
			Append(it.GetCurrent());
		}
};

}
