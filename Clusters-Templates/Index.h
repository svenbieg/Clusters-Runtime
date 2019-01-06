//=========
// Index.h
//=========

#pragma once


//=======
// Using
//=======

#include "Details/Index/Index.h"


//===========
// Namespace
//===========

namespace Clusters {
	namespace Templates {


//=======
// Index
//=======

template <class _Id, class _Item=VOID, UINT _GroupSize=100>
class Index: public Details::Index::Index<_Id, _Item, _GroupSize>
{
private:
	// Using
	using _Base=Details::Index::Index<_Id, _Item, _GroupSize>;
	using _Index=Index<_Id, _Item, _GroupSize>;

public:
	// Con-/Destructors
	Index() {}
	Index(_Index const& Index): _Base(Index) {}
};

}}
