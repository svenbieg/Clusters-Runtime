//=========
// Index.h
//=========
//
// Copyright 2018, Sven Bieg (svenbieg@web.de)
// http://svenbieg.azurewebsites.net
//
// Implementation of a sorted list
// Items have unique ids and are in ascending order
//

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
	using BASE=Details::Index::Index<ID, ITEM, _GroupSize>;
	using INDEX=Index<ID, ITEM, _GroupSize>;

public:
	// Con-/Destructors
	Index() {}
	Index(INDEX const& Index): BASE(Index) {}
};

}
