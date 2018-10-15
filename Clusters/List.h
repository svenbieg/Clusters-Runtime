//========
// List.h
//========
//
// Copyright 2018, Sven Bieg (svenbieg@web.de)
// http://svenbieg.azurewebsites.net
//
// Implementation of an ordererd list
// Items can be inserted an removed at random positions
//

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
