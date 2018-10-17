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

template <class ITEM>
class ListGroup: public ::Clusters::Templates::Details::Cluster::Group<ITEM>
{
public:
	// Modification
	virtual ITEM* Append(BOOL Again)=0;
	virtual ITEM* InsertAt(UINT64 Position, BOOL Again)=0;
};

}}}}
