//========
// List.h
//========

#pragma once


//=======
// Using
//=======

#include "Details\List\List.h"


//===========
// Namespace
//===========

namespace Clusters {


//======
// List
//======

template <class ITEM, UINT _GroupSize=100>
class List: public Details::List::List<ITEM, _GroupSize>
{
private:
	// Using
	using VECTOR=List<ITEM, _GroupSize>;

public:
	// Con-/Destructors
	List() {}
	List(VECTOR const& List)
		{
		for(auto it=List.First(); it.HasCurrent(); it.MoveNext())
			Append(it.GetCurrent());
		}
};

}
