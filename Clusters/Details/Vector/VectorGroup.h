//===============
// VectorGroup.h
//===============

#pragma once


//=======
// Using
//=======

#include "..\Cluster\Group.h"


//===========
// Namespace
//===========

namespace Clusters {
	namespace Details {
		namespace Vector {


//==============
// Vector-Group
//==============

template <class ITEM>
class VectorGroup: public ::Clusters::Details::Cluster::Group<ITEM>
{
public:
	// Modification
	virtual ITEM* Append(BOOL Again)=0;
	virtual ITEM* InsertAt(UINT64 Position, BOOL Again)=0;
};

}}}
