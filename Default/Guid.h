//========
// Guid.h
//========

#pragma once


//=======
// Using
//=======

#include "pch.h"


//======
// Guid
//======

class Guid
{
public:
	// Con-/Destructors
	Guid(GUID const& Id): uId(Id) {}
	#ifdef __cplusplus_winrt
	Guid(Platform::Guid Id): uId(Id) {}
	#endif

	// Conversion
	operator GUID const&()const { return uId; }
	#ifdef __cplusplus_winrt
	operator Platform::Guid()const { return uId; }
	#endif

	// Comparison
	inline bool operator==(Guid const& Id)const { return IsEqual(uId, Id.uId); }
	inline bool operator>(Guid const& Id)const { return IsAbove(uId, Id.uId); }
	inline bool operator>=(Guid const& Id)const { return IsAbove(uId, Id.uId, true); }
	inline bool operator<(Guid const& Id)const { return IsBelow(uId, Id.uId); }
	inline bool operator<=(Guid const& Id)const { return IsBelow(uId, Id.uId, true); }

private:
	// Common
	static BOOL IsAbove(GUID const& Id1, GUID const& Id2, BOOL OrEqual=false);
	static BOOL IsBelow(GUID const& Id1, GUID const& Id2, BOOL OrEqual=false);
	static inline BOOL IsEqual(GUID const& Id1, GUID const& Id2) { return memcmp(&Id1, &Id2, sizeof(GUID))==0; }
	GUID uId;
};
