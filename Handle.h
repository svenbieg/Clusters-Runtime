//==========
// Handle.h
//==========

#pragma once


//=======
// Using
//=======

#include "StringHelper.h"


//========
// Handle
//========

template <class T>
class Handle
{
public:
	// Con-/Destructors
	Handle(T^ Object): hObject(Object) {}
	Handle(Handle<T> const& Handle): hObject(Handle.hObject) {}
	~Handle() { hObject=nullptr; }

	// Acces
	operator T^() { return hObject; }

	// Comparison
	BOOL operator==(Handle<T> const& Handle)const { return hObject==Handle.hObject; }
	BOOL operator!=(Handle<T> const& Handle)const { return hObject!=Handle.hObject; }
	BOOL operator>(Handle<T> const& Handle)const { return hObject>Handle.hObject; }
	BOOL operator>=(Handle<T> const& Handle)const { return hObject>=Handle.hObject; }
	BOOL operator<(Handle<T> const& Handle)const { return hObject<Handle.hObject; }
	BOOL operator<=(Handle<T> const& Handle)const { return hObject<=Handle.hObject; }

private:
	// Common
	T^ hObject;
};


//===============
// String-Handle
//===============

template <>
class Handle<Platform::String>
{
private:
	// Using
	using String=Platform::String;

public:
	// Con-/Destructors
	Handle(String^ String): hString(String) {}
	Handle(Handle<String> const& Handle): hString(Handle.hString) {}
	~Handle() { hString=nullptr; }

	// Acces
	operator String^() { return hString; }

	// Comparison
	BOOL operator==(Handle<String> const& Handle)const { return StringCompare(hString->Begin(), Handle.hString->Begin())==0; }
	BOOL operator!=(Handle<String> const& Handle)const { return StringCompare(hString->Begin(), Handle.hString->Begin())!=0; }
	BOOL operator>(Handle<String> const& Handle)const { return StringCompare(hString->Begin(), Handle.hString->Begin())>0; }
	BOOL operator>=(Handle<String> const& Handle)const { return StringCompare(hString->Begin(), Handle.hString->Begin())>=0; }
	BOOL operator<(Handle<String> const& Handle)const { return StringCompare(hString->Begin(), Handle.hString->Begin())<0; }
	BOOL operator<=(Handle<String> const& Handle)const { return StringCompare(hString->Begin(), Handle.hString->Begin())<=0; }

private:
	// Common
	String^ hString;
};

