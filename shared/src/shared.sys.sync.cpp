/*
	Created by Tech_dog (ebontrop@gmail.com) 21-Mar-2014 at 12:20:42pm, GMT+4, Taganrog, Friday;
	This is Platinum Payroll project shared generic synchronize object class implementation file. 
	(https://www.platinumpayroll.co.nz/) looks like the company is permanently closed;
	-----------------------------------------------------------------------------
	Adopted to File Guardian (thefileguardian.com) on 27-May-2018 at 1:20:35p, UTC+7, Phuket, Rawai, Sunday;
	Adopted to FakeGPS driver project on 13-Dec-2019 at 10:13:54a, UTC+7, Novosibirsk, Friday;
	Adopted to Process Prying Eyes project on 6-Jan-2023 at 06:55:32.2749658, UTC+7, Novosibirsk, Monday;
*/
#include "shared.sys.sync.h"

using namespace shared::sys_core;
// https://blog.csdn.net/Sycamore_Ma/article/details/103915917  ; otherwise, C1189 error: "no target architecture";
// at the same time, it is very important to set appropriate value in the field 'Preprocessor Definitions': either '_X86_' or '_AMD64_';
/////////////////////////////////////////////////////////////////////////////

CSyncObject::CSyncObject(void) // throw() // no XP support
{
#if (0)
	__try
	{
#endif
		::InitializeCriticalSection(&m_sec);
#if (0)
	}
	__except(STATUS_NO_MEMORY == ::GetExceptionCode())
	{}
#endif
}

CSyncObject::~CSyncObject(void)
{
	::DeleteCriticalSection(&m_sec);
	::memset((void*)&m_sec, 0, sizeof(CRITICAL_SECTION));
}

/////////////////////////////////////////////////////////////////////////////

VOID CSyncObject::Lock(void) const
{
	__try
	{
		::EnterCriticalSection(&m_sec);
	}
	__except(STATUS_NO_MEMORY == ::GetExceptionCode())
	{}
}

BOOL CSyncObject::TryLock(void) const
{
	const BOOL bResult = ::TryEnterCriticalSection(&m_sec);
	return bResult;
}

VOID CSyncObject::Unlock(void) const
{
	__try
	{
		::LeaveCriticalSection(&m_sec);
	}
	__except(STATUS_NO_MEMORY == ::GetExceptionCode())
	{}
}