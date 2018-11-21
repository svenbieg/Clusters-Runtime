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
	namespace Templates {


//======
// List
//======

template <class _Item, unsigned int _GroupSize=100>
class List: public Details::List::List<_Item, _GroupSize>
{
private:
	// Using
	using _Base=Details::List::List<_Item, _GroupSize>;
	using _List=List<_Item, _GroupSize>;

public:
	// Con-/Destructors
	List() {}
	List(_List const& List): _Base(List) {}
};

}}
