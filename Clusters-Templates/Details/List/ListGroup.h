//=============
// ListGroup.h
//=============

#pragma once


//=======
// Using
//=======

#include "..\Cluster\Group.h"


//===========
// Namespace
//===========

namespace Clusters {
	namespace Templates {
		namespace Details {
			namespace List {


//============
// List-Group
//============

template <class _Item>
class ListGroup: public ::Clusters::Templates::Details::Cluster::Group<_Item>
{
public:
	// Modification
	virtual _Item* Append(bool Again)=0;
	virtual _Item* InsertAt(size_t Position, bool Again)=0;
};

}}}}
