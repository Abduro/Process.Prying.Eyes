/*
	Created by Tech_dog (ebontrop@gmail.com) on 6-Jun-2016 at 12:00:15p, UTC+7, Phuket, Rawai, Wednesday;
	This is System Service Manager control state control interface implementation file. (thefileguardian.com);
	-----------------------------------------------------------------------------
	Adopted to Ebo Pack solution on 12-Jul-2020 at 9:38:21a, UTC+7, Novosibirsk, Sunday;
	-----------------------------------------------------------------------------
	Adopted to Process Prying Eyes project on 12-Feb-2023 at 09:45:46.5209824, UTC+7, Novosibirsk, Sunday;
*/
#include "svc.state.h"
#include "svc.status.h"

using namespace service::control;

/////////////////////////////////////////////////////////////////////////////

namespace service { namespace control { namespace _impl {
}}}
using namespace service::control::_impl;
/////////////////////////////////////////////////////////////////////////////

CState:: CState(void) : m_value(value::e_undef), m_synch(nullptr) { m_error << __MODULE__ << S_OK; }
CState:: CState(const CState& _ref) : CState() { *this = _ref ; }
CState:: CState (const LPTSvcStat _p_synch) : CState() { *this << _p_synch; }
CState:: CState(const SC_HANDLE _p_svc_handle) : CState() { *this  << _p_svc_handle; }
CState::~CState(void) {}

/////////////////////////////////////////////////////////////////////////////

TSvcStateValue CState::Current(void) const { return m_value; }
TSysErrRef CState::Error (void) const { return m_error; }
const bool CState::Is    (void) const { return (value::e_undef != m_value); }

DWORD  CState::Query (const SC_HANDLE _p_svc_handle ) {
	m_error << __MODULE__ << S_OK;

	if (NULL == _p_svc_handle || INVALID_HANDLE_VALUE == _p_svc_handle)
		return (m_error << ERROR_INVALID_PARAMETER);

	DWORD dw_req = 0;
	SERVICE_STATUS_PROCESS svc_sta = {0};
	// https://learn.microsoft.com/en-us/windows/win32/api/winsvc/nf-winsvc-queryservicestatusex ;
	const BOOL b_result = ::QueryServiceStatusEx(
		_p_svc_handle, SC_STATUS_PROCESS_INFO, reinterpret_cast<LPBYTE>(&svc_sta), sizeof(SERVICE_STATUS_PROCESS), &dw_req
	);
	if (FALSE == b_result)
		m_error.Last();
	else {
		m_value = static_cast<CState::value>(svc_sta.dwCurrentState);
		if (m_synch)
			m_synch->dwCurrentState = svc_sta.dwCurrentState;
	}
	return (DWORD)m_error;
}
					    
DWORD  CState::Reset (void) {
	m_value = value::e_undef; return S_OK; // implicit conversion;
}

DWORD  CState::Set (CState::value _state) { return this->Set(static_cast<LONG>(_state)); }
DWORD  CState::Set (LONG _l_state) {
	m_error << __MODULE__ << S_OK;

	if (_l_state == (long)this->m_value) // nothing is changed;
		return m_error;

	this->m_value = static_cast<CState::value>(_l_state);

	static DWORD dw_check_point = 1;

	switch (_l_state)
	{
	case value::e_paused :
	case value::e_running:
	case value::e_stopped:
	{
		dw_check_point = 0;
	} break;
	default:
		dw_check_point++; // indicates a process of long pending operations;
	}

	if (m_synch)
		m_synch->dwCheckPoint = dw_check_point;

	return m_error;
}

DWORD  CState::Synch (LPTSvcStat _p_synch) { this->m_synch = _p_synch; return S_OK; }

/////////////////////////////////////////////////////////////////////////////

bool  CState::Paused (void) const { return (CState::e_paused == m_value); }

bool  CState::Pending(void) const {
	return (CState::e_try_start == m_value
			|| CState::e_try_stop == m_value
				|| CState::e_try_going == m_value
					|| CState::e_try_pause == m_value);
}
bool  CState::Running(void) const { return (CState::e_running == m_value); }
bool  CState::Stopped(void) const { return (CState::e_stopped == m_value); }

/////////////////////////////////////////////////////////////////////////////

CState&   CState::operator = (const CState& _ref) {
	this->m_value  = _ref.Current();
	this->m_error  = _ref.Error()  ;
	if (this->m_synch)
		this->m_synch->dwCurrentState = this->m_value;
	return *this;
}

CState&  CState::operator << (const SC_HANDLE _p_svc_handle) {
	this->Query(_p_svc_handle);
	return *this;
}

CState&  CState::operator << (const LPTSvcStat _p_synch) { this->Synch(_p_synch); return *this; }
CState&  CState::operator << (LONG _l_state) { this->Set(_l_state); return *this; }

/////////////////////////////////////////////////////////////////////////////

CState::operator const CState::value(void) const { return this->Current(); }