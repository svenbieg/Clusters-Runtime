//==========
// Guid.cpp
//==========

#include "pch.h"


//=======
// Using
//=======

#include "Guid.h"


//===========
// Namespace
//===========

namespace Clusters {
	namespace Details {


//========
// Common
//========

BOOL Guid::IsAbove(GUID const& id1, GUID const& id2, BOOL equal)
{
if(id1.Data1>id2.Data1)
	return true;
if(id1.Data1<id2.Data1)
	return false;
if(id1.Data2>id2.Data2)
	return true;
if(id1.Data2<id2.Data2)
	return false;
if(id1.Data3>id2.Data3)
	return true;
if(id1.Data3<id2.Data3)
	return false;
for(UINT u=0; u<8; u++)
	{
	if(id1.Data4[u]>id2.Data4[u])
		return true;
	if(id1.Data4[u]<id2.Data4[u])
		return false;
	}
return equal;
}

BOOL Guid::IsBelow(GUID const& id1, GUID const& id2, BOOL equal)
{
if(id1.Data1<id2.Data1)
	return true;
if(id1.Data1>id2.Data1)
	return false;
if(id1.Data2<id2.Data2)
	return true;
if(id1.Data2>id2.Data2)
	return false;
if(id1.Data3<id2.Data3)
	return true;
if(id1.Data3>id2.Data3)
	return false;
for(UINT u=0; u<8; u++)
	{
	if(id1.Data4[u]<id2.Data4[u])
		return true;
	if(id1.Data4[u]>id2.Data4[u])
		return false;
	}
return equal;
}

}}