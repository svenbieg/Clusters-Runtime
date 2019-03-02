//=============
// ListGroup.h
//=============

#pragma once


//=======
// Using
//=======

#include "../Cluster/Group.h"


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
	virtual _Item* Append(BOOL Again)=0;
	virtual SIZE_T Append(_Item const* Items, SIZE_T Count)=0;
	virtual _Item* InsertAt(SIZE_T Position, BOOL Again)=0;
};

}}}}
