/*
	Created by Tech_dog (ebontrop@gmail.com) on 25-Jan-2015 at 8:58:55p, UTC+3, Taganrog, Sunday;
	This is shared system service interface implementation file.
	-----------------------------------------------------------------------------
	Adopted to v15 on 28-May-2018 at 5:36:44p, UTC+7, Phuket, Rawai, Monday;
	Adopted to Ebo Pack solution on 12-Jul-2020 at 10:38:02a, UTC+7, Novosibirsk, Sunday;
	-----------------------------------------------------------------------------
	Adopted to Process Prying Eyes project on 12-Feb-2023 at 17:16:22.4627127, UTC+7, Novosibirsk, Sunday;
*/
#include "svc.service.h"

using namespace service::control;

/////////////////////////////////////////////////////////////////////////////

CService_Handle:: CService_Handle(CService* _p_owner) : m_handle(nullptr), m_owner(_p_owner) { m_error << __MODULE__ << S_OK; }
CService_Handle:: CService_Handle(const CService_Handle& _ref) : CService_Handle() { *this = _ref; }
CService_Handle:: CService_Handle(const SC_HANDLE _p_svc_hand) : CService_Handle() { *this << _p_svc_hand; }
CService_Handle::~CService_Handle(void) { if (this->Is()) this->Close(); }

/////////////////////////////////////////////////////////////////////////////

DWORD   CService_Handle::Attach (const SC_HANDLE _p_svc_hand) {
	m_error << __MODULE__ << S_OK;

	if (this->Is())
		this->Close();

	if ((bool)m_error)
		return m_error;

	m_handle = _p_svc_hand; // NULL is acceptable;

	if (m_owner != nullptr)
		m_owner->Update();

	return (DWORD)m_error;
}

DWORD   CService_Handle::Close  (void)  {
	m_error << __MODULE__ << S_OK;

	if (this->Is() == false)
		return (m_error << ERROR_INVALID_STATE);

	if (m_owner != nullptr)
		m_owner->Reset();

	const BOOL b_result = ::CloseServiceHandle(m_handle);
	if (FALSE == b_result)
		m_error.Last();
	else
		m_handle = nullptr;

	return (DWORD)m_error;
}

SC_HANDLE   CService_Handle::Detach (void) {

	if (m_owner != nullptr)
		m_owner->Reset();

	SC_HANDLE p_return = m_handle;
	m_handle = nullptr;
	return p_return;
}

TSysErrRef  CService_Handle::Error  (void) const { return m_error; }
const bool  CService_Handle::Is     (void) const { return (nullptr != m_handle && INVALID_HANDLE_VALUE != m_handle); }

/////////////////////////////////////////////////////////////////////////////

CService_Handle::operator SC_HANDLE (void) const { return m_handle; }
CService_Handle::operator SC_HANDLE&(void)       { return m_handle; }

/////////////////////////////////////////////////////////////////////////////

CService_Handle& CService_Handle::operator = (const CService_Handle& _ref) {
	this->m_error = _ref.Error();
	this->m_handle = _ref.m_handle;
	this->m_owner = _ref.m_owner;

	CService_Handle& ref_ = const_cast<CService_Handle&>(_ref);
	this->Attach(ref_.Detach());

	return *this;
}

CService_Handle& CService_Handle::operator <<(const SC_HANDLE _p_svc_hand) { this->Attach(_p_svc_hand); return *this; }
CService_Handle& CService_Handle::operator >>(SC_HANDLE& _p_svc_hand) {
	_p_svc_hand = this->Detach(); return *this;
}

/////////////////////////////////////////////////////////////////////////////

CService:: CService (void) : m_handle(this) { m_error << __MODULE__ << S_OK; }
CService:: CService (const  CService& _ref) : CService() { *this = _ref; }
CService:: CService (const  SC_HANDLE _han) : CService() { *this <<_han; }
CService::~CService (void) {}

/////////////////////////////////////////////////////////////////////////////
const
CCfg&      CService::Cfg    (void) const { return m_cfg; }
CCfg&      CService::Cfg    (void)       { return m_cfg; }
const
CCfg_Ex&   CService::Cfg_Ex (void) const { return m_cfg_ex; }
CCfg_Ex&   CService::Cfg_Ex (void)       { return m_cfg_ex; }

TSysErrRef CService::Error  (void) const { return m_error; }
const bool CService::Is     (void) const { return m_handle.Is(); }
DWORD      CService::Reset  (void) {
	m_error << __MODULE__ << S_OK;

	DWORD d_result = S_OK;

	if (m_cfg.Is())    d_result = m_cfg.Reset();
	if (m_cfg_ex.Is()) d_result = m_cfg_ex.Reset();
	if (m_status.Is()) d_result = m_status.Reset();
	if (m_state.Is())  d_result = m_state.Reset();

	return (DWORD)m_error;
}
const
CState&    CService::State  (void) const { return m_state ; }
CState&    CService::State  (void)       { return m_state ; }
const
service::control::CStatus&   CService::Status (void) const { return m_status; }
service::control::CStatus&   CService::Status (void)       { return m_status; }

DWORD      CService::Update (void) {
	m_error << __MODULE__ << S_OK;

	if (this->Is() == false)
		return (m_error << ERROR_INVALID_STATE);

	DWORD d_result = (DWORD)m_error;
	
	if (S_OK == d_result) { d_result = m_cfg.Query(m_handle)   ; if (S_OK != d_result) m_error = m_cfg.Error(); }
	if (S_OK == d_result) { d_result = m_cfg_ex.Query(m_handle); if (S_OK != d_result) m_error = m_cfg_ex.Error(); }
	if (S_OK == d_result) { d_result = m_status.Query(m_handle); if (S_OK != d_result) m_error = m_status.Error(); }
	if (S_OK == d_result) { d_result = m_state.Query(m_handle) ; if (S_OK != d_result) m_error = m_state.Error(); }

	return m_error;
}

/////////////////////////////////////////////////////////////////////////////

CService&  CService::operator = (const CService& _ref) {
	
	CService& ref_  = const_cast<CService&>(_ref);

	this->m_cfg     = ref_.m_cfg;
	this->m_cfg_ex  = ref_.m_cfg_ex;
	this->m_status  = ref_.m_status;
	this->m_state   = ref_.m_state ;
	this->m_error   = ref_.m_error ;
	ref_.Reset();
	this->m_handle << ref_.Handle().Detach();

	return *this;
}

CService&  CService::operator <<(SC_HANDLE _handle) {
	this->m_handle << _handle;
	return *this;
}

/////////////////////////////////////////////////////////////////////////////
const
TSvcHandle& CService::Handle(void) const { return m_handle; }
TSvcHandle& CService::Handle(void)       { return m_handle; }
CService::operator SC_HANDLE(void) const { return m_handle; }