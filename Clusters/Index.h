//=========
// Index.h
//=========

#pragma once


//=======
// Using
//=======

#include "Details\Index\Index.h"


//===========
// Namespace
//===========

namespace Clusters {


//=======
// Index
//=======

template <class ID, class ITEM=VOID, UINT _GroupSize=100>
class Index: public Details::Index::Index<ID, ITEM, _GroupSize>
{
private:
	// Using
	using INDEX=Index<ID, ITEM, _GroupSize>;

public:
	// Con-/Destructors
	Index() {}
	Index(INDEX const& Index)
		{
		for(auto it=Index.First(); it.HasCurrent(); it.MoveNext())
			Add(it.GetCurrentId(), it.GetCurrentItem());
		}
};

}
