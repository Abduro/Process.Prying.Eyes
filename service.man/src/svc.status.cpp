/*
	Created by Tech_dog (ebontrop@gmail.com) on 6-Aug-2020 at 3:39:53p, UTC+7, Novosibirsk, Thursday;
	This is Ebo Pack solution system service status information interface declaration file.
	-----------------------------------------------------------------------------
	Adopted to Process Prying Eyes project on 12-Feb-2023 at 11:29:43.4505858, UTC+7, Novosibirsk, Sunday;
*/
#include "svc.status.h"

using namespace service::control;

/////////////////////////////////////////////////////////////////////////////

CStatus:: CStatus(void) : TBase(), m_stat{0}, m_stat_ptr(nullptr), m_state(&this->m_stat) { this->Reset(); }
CStatus:: CStatus(const CStatus& _ref) : CStatus() { *this = _ref; }
CStatus::~CStatus(void) {}

/////////////////////////////////////////////////////////////////////////////

TSvcStat  CStatus::Data (void) const { return m_stat; }
TSvcStat& CStatus::Data (void)       { return m_stat; }

bool      CStatus::Is   (void) const {
	return (0 != m_stat.dwCheckPoint
	     || 0 != m_stat.dwControlsAccepted
	     || 0 != m_stat.dwCurrentState
	     || 0 != m_stat.dwServiceSpecificExitCode
	     || 0 != m_stat.dwServiceType
	     || 0 != m_stat.dwWaitHint
	     || 0 != m_stat.dwWin32ExitCode) && nullptr != this->m_stat_ptr;
}

DWORD     CStatus::Query(const SC_HANDLE _p_svc_handle) {
	m_error << __MODULE__ << S_OK;

	DWORD d_result = TBase::Owner(_p_svc_handle);
	if (S_OK != d_result) {
		return (DWORD)m_error;
	}

	const BOOL b_result = ::QueryServiceStatus(TBase::Owner(), &m_stat);
	if (FALSE == b_result)
		m_error.Last();

	return m_error;
}

DWORD     CStatus::Reset(void) {
	m_error << __MODULE__ << S_OK;

	ZeroMemory(&m_stat, sizeof(SERVICE_STATUS));

	this->m_stat_ptr = nullptr; // looks like no unregistering the service handler function is required;

	return m_error;
}

DWORD     CStatus::Set (const CCrtData& _crt_data) {
	m_error << __MODULE__ << S_OK;

	::memset(&m_stat, 0, sizeof(SERVICE_STATUS)); // calling this->reset() is not acceptable here due to status handle pointer must be kept;

	m_stat.dwServiceType  = _crt_data.Options().Type().IsDriver() ? CTypeOption::eKernelDrv : CTypeOption::eService;
	m_stat.dwCurrentState = SERVICE_START_PENDING;

	DWORD dw_accepted  = 0;

	if (_crt_data.Options().Ctrl().CanPause()) dw_accepted |= CCtrlOption::eCanContinue;
	if (_crt_data.Options().Ctrl().CanStop ()) dw_accepted |= CCtrlOption::eStoppable;
	if (_crt_data.Options().Ctrl().Shutdown()) dw_accepted |= CCtrlOption::eCanShutdown;

	m_stat.dwControlsAccepted = dw_accepted;
	m_stat.dwWin32ExitCode    = NO_ERROR;
	m_stat.dwServiceSpecificExitCode = 0;
	m_stat.dwCheckPoint       = 0;
	m_stat.dwWaitHint         = 0;

	return this->Update();
}

DWORD     CStatus::Set (const TSvcStatPtr _p_stat_ptr) {
	m_error << __MODULE__ << S_OK;
	this->m_stat_ptr = _p_stat_ptr;
	return m_error;
}

DWORD     CStatus::Update(void) {
	m_error << __MODULE__ << S_OK;

	if (nullptr == this->m_stat_ptr)
		return (m_error << ERROR_INVALID_HANDLE);

	const bool b_result = !!::SetServiceStatus(this->m_stat_ptr, &this->m_stat);
	if (false == b_result)
		m_error.Last();

	return m_error;
}

/////////////////////////////////////////////////////////////////////////////
const
CState&   CStatus::State (void) const { return this->m_state; }
CState&   CStatus::State (void)       { return this->m_state; }

/////////////////////////////////////////////////////////////////////////////

CStatus&  CStatus::operator  = (const CStatus& _ref) {
	this->m_owner    = _ref.Owner();
	this->m_stat     = _ref.m_stat ;
	this->m_stat_ptr = _ref.m_stat_ptr;
	this->m_state    = _ref.m_state;
	return *this;
}
CStatus&  CStatus::operator << (const SC_HANDLE _p_sc_handle) {
	this->Query(_p_sc_handle);
	return *this;
}

CStatus&  CStatus::operator << (const CCrtData& _crt_data) { this->Set(_crt_data); return *this; }
CStatus&  CStatus::operator << (const TSvcStatPtr _p_stat_ptr) { this->Set(_p_stat_ptr); return *this; }

/////////////////////////////////////////////////////////////////////////////

CStatus::operator  TSvcStat& (void) { return this->m_stat; }
CStatus::operator  TSvcStatPtr& (void) { return this->m_stat_ptr; }