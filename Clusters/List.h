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
	using BASE=Details::List::List<ITEM, _GroupSize>;
	using LIST=List<ITEM, _GroupSize>;

public:
	// Con-/Destructors
	List() {}
	List(LIST const& List): BASE(List) {}
};

}
