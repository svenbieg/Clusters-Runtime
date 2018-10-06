//==============
// TaskHelper.h
//==============

#pragma once


//================
// Access Control
//================

class AccessControl
{
public:
	// Friends
	friend class ScopedRead;
	friend class ScopedWrite;

	// Con-/Destructors
	AccessControl(): uLock(0), uReadCount(0), uWriteCount(0) {}

	// Access
	VOID AccessRead()const
		{
		auto pthis=const_cast<AccessControl*>(this);
		UINT ulock=InterlockedIncrement(&pthis->uLock);
		if(ulock>1||uWriteCount>0)
			{
			pthis->uLock--;
			throw(EACCES);
			}
		pthis->uReadCount++;
		pthis->uLock--;
		}
	VOID AccessWrite()const
		{
		auto pthis=const_cast<AccessControl*>(this);
		UINT ulock=InterlockedIncrement(&pthis->uLock);
		if(ulock>1||uReadCount>0||uWriteCount>0)
			{
			pthis->uLock--;
			throw(EACCES);
			}
		pthis->uWriteCount++;
		pthis->uLock--;
		}
	VOID ReleaseRead()const
		{
		auto pthis=const_cast<AccessControl*>(this);
		pthis->uReadCount--;
		}
	VOID ReleaseWrite()const
		{
		auto pthis=const_cast<AccessControl*>(this);
		pthis->uWriteCount--;
		}

private:
	// Common
	volatile UINT uLock;
	UINT uReadCount;
	UINT uWriteCount;
};


//=============
// Scoped Read
//=============

class ScopedRead
{
public:
	// Con-/Destructors
	ScopedRead(AccessControl const& AccessControl): pAccessControl(nullptr)
		{
		AccessControl.AccessRead();
		pAccessControl=&AccessControl;
		}
	~ScopedRead()
		{
		if(pAccessControl!=nullptr)
			pAccessControl->ReleaseRead();
		}

	// Common
	VOID Release()
		{
		if(pAccessControl!=nullptr)
			{
			pAccessControl->ReleaseRead();
			pAccessControl=nullptr;
			}
		}
private:
	// Common
	AccessControl const* pAccessControl;
};


//==============
// Scoped Write
//==============

class ScopedWrite
{
public:
	// Con-/Destructors
	ScopedWrite(AccessControl& AccessControl): pAccessControl(nullptr)
		{
		AccessControl.AccessWrite();
		pAccessControl=&AccessControl;
		}
	~ScopedWrite()
		{
		if(pAccessControl!=nullptr)
			pAccessControl->ReleaseWrite();
		}

	// Common
	VOID Release()
		{
		if(pAccessControl!=nullptr)
			{
			pAccessControl->ReleaseWrite();
			pAccessControl=nullptr;
			}
		}
private:
	// Common
	AccessControl* pAccessControl;
};
