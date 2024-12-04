/*
	Created by Tech_dog (ebontrop@gmail.com) on 11-Jan-2016 at 3:37:45pm, GMT+7, Phuket, Rawai, Monday;
	This is Shared Lite library system thread pool related class(s) implementation file.
*/
#include "shared.run.tpl.h"

using namespace shared::sys_core;

/////////////////////////////////////////////////////////////////////////////

CThreadCrtDataBase:: CThreadCrtDataBase(void) : m_bStopped(false), m_hStopEvent(NULL) { }
CThreadCrtDataBase::~CThreadCrtDataBase(void) { }

/////////////////////////////////////////////////////////////////////////////
const
HANDLE&    CThreadCrtDataBase::EventObject(void)const { return m_hStopEvent; }
HANDLE&    CThreadCrtDataBase::EventObject(void)      { return m_hStopEvent; }
HRESULT    CThreadCrtDataBase::Initialize(void)
{
	if (m_hStopEvent)
		return (HRESULT_FROM_WIN32(ERROR_ALREADY_INITIALIZED));

	m_hStopEvent = ::CreateEvent(NULL, TRUE, FALSE, NULL);
	if (!m_hStopEvent)
		return (HRESULT_FROM_WIN32(::GetLastError()));
	else
		return S_OK;
}

bool       CThreadCrtDataBase::IsStopped(void)const { return m_bStopped; }
VOID       CThreadCrtDataBase::IsStopped(const bool _stop) {
	m_bStopped = _stop;
}

bool       CThreadCrtDataBase::IsValid  (void) const { return (NULL != m_hStopEvent && INVALID_HANDLE_VALUE != m_hStopEvent); }

HRESULT    CThreadCrtDataBase::Terminate(void)
{
	if (m_hStopEvent)
	{
		::CloseHandle(m_hStopEvent);
		m_hStopEvent = NULL;
	}
	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////

CThreadCrtData:: CThreadCrtData(void) { TBase::Initialize(); }
CThreadCrtData::~CThreadCrtData(void) { TBase::Terminate();  }

/////////////////////////////////////////////////////////////////////////////

CThreadBase:: CThreadBase(void) : m_state(CThreadState::eStopped) { }
CThreadBase::~CThreadBase(void) {
	if (this->IsRunning())
		this->Stop();
}

/////////////////////////////////////////////////////////////////////////////

bool       CThreadBase::IsComplete(void)const {
	const DWORD dwResult = ::WaitForSingleObject(m_crt.EventObject(), 0);
	return (dwResult == WAIT_OBJECT_0);
}

HRESULT    CThreadBase::MarkToStop(void) {
	m_crt.IsStopped(true);
	HRESULT hr_ = S_OK;
	return  hr_;
}

HRESULT    CThreadBase::Start(void) {
	HRESULT hr_ = S_OK;

	if (this->IsRunning()) {
		return (hr_ = __DwordToHresult(ERROR_INVALID_STATE));
	}

	m_crt.IsStopped(false);
	::ResetEvent(m_crt.EventObject());

	if (!CThreadPool::QueueUserWorkItem(&CThreadBase::WorkFun, this))
		hr_ = __LastErrToHresult();
	
	if (SUCCEEDED(hr_))
		m_state = CThreadState::eWorking;
	else
		m_state = CThreadState::eError;

	return hr_;
}

HRESULT    CThreadBase::Stop (DWORD _dw_wait_msec) {
	HRESULT hr_ = S_OK;

	if (this->IsRunning() == false) {
		hr_ = __DwordToHresult(ERROR_INVALID_STATE);
		return hr_;
	}
	m_crt.IsStopped(true);

	const
	DWORD dwResult = ::WaitForSingleObject(m_crt.EventObject(), _dw_wait_msec);

	if (dwResult != WAIT_OBJECT_0)
		hr_ = __LastErrToHresult();
	
	if (SUCCEEDED(hr_))
		m_state = CThreadState::eStopped;
	else
		m_state = CThreadState::eError;

	return hr_;
}

/////////////////////////////////////////////////////////////////////////////

const bool  CThreadBase::IsRunning(void) const 
{
	bool bRunning = false;
	{
		SAFE_LOCK(m_lock);
		bRunning = !!(CThreadState::eWorking & m_state);
	}
	return bRunning;
}

const bool  CThreadBase::IsValid(void) const
{
	return (m_crt.IsValid());
}

const DWORD CThreadBase::State(void) const
{
	DWORD state_ = 0;
	{
		SAFE_LOCK(m_lock);
		state_ =  m_state;
	}
	return state_;
}