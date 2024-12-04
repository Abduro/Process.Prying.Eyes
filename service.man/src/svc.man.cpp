/*
	Created by Tech_dog (ebontrop@gmail.com) on 6-Jun-2016 at 2:19:01p, UTC+7, Phuket, Rawai, Wednesday;
	This is System Service Manager Wrapper class implementation file.
	( project: thefileguardian.com )
	-----------------------------------------------------------------------------
	Adopted to Ebo Pack solution on 13-Jul-2020 at 9:46:02a, UTC+7, Novosibirsk, Monday;
	-----------------------------------------------------------------------------
	Adopted to Process Prying Eyes project on 13-Feb-2023 at 05:15:02.1482343, UTC+7, Novosibirsk, Monday;
*/
#include "svc.man.h"

using namespace service::control;

/////////////////////////////////////////////////////////////////////////////

namespace service { namespace control { namespace _impl {
}}}
using namespace service::control::_impl;
/////////////////////////////////////////////////////////////////////////////

CService_Operable:: CService_Operable (void) { m_error << __MODULE__ << S_OK; }
CService_Operable:: CService_Operable (const CService_Operable& _ref) : CService_Operable() { *this = _ref; }
CService_Operable:: CService_Operable (LPCTSTR _lp_sz_svc_name) : CService_Operable() { *this << _lp_sz_svc_name; }
CService_Operable::~CService_Operable (void) {}

/////////////////////////////////////////////////////////////////////////////

TSysErrRef   CService_Operable::Error (void) const { return this->m_error; }
bool         CService_Operable::Is    (void) const {

	m_error << (m_operable.empty() ? ERROR_INVALID_DATA : S_OK);
	return (bool)m_error;
}

/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////

CSvcManager_Handle:: CSvcManager_Handle(void) : m_handle(nullptr) { m_error << __MODULE__ << ERROR_INVALID_STATE; }
CSvcManager_Handle:: CSvcManager_Handle(const SC_HANDLE _attachable) : CSvcManager_Handle() { this->Attach(_attachable); }
CSvcManager_Handle::~CSvcManager_Handle(void) { if (this->Is()) this->Close(); }

/////////////////////////////////////////////////////////////////////////////

DWORD   CSvcManager_Handle::Attach(const SC_HANDLE _handle) {

	if (CSvcManager_Handle::Is_acceptable(_handle) == false)
		return (m_error << ERROR_INVALID_PARAMETER);

	if (this->Is()) {
		this->Close();
	}

	m_handle = _handle;

	return m_error;
}

DWORD   CSvcManager_Handle::Close (void) {
	m_error << __MODULE__ << S_OK;

	if (this->Is() == false)
		return (m_error << ERROR_INVALID_STATE);

	const BOOL b_result = ::CloseServiceHandle(m_handle);
	if (FALSE == b_result)
		m_error.Last();
	else
		m_handle = nullptr;

	return m_error;
}

SC_HANDLE  CSvcManager_Handle::Detach(void) {
	m_error << __MODULE__ << ERROR_INVALID_DATA;

	if (this->Is() == false) {
		return nullptr;
	}

	SC_HANDLE handle_ = m_handle; m_handle = nullptr;
	return handle_;
}

TSysErrRef CSvcManager_Handle::Error  (void) const { return  m_error; }
const bool CSvcManager_Handle::Is     (void) const { return CSvcManager_Handle::Is_acceptable(m_handle); }

/////////////////////////////////////////////////////////////////////////////

DWORD      CSvcManager_Handle::Connect(const DWORD _dw_access) {
	m_error << __MODULE__ << S_OK;

	if (this->Is())
		return (m_error = __DwordToHresult(ERROR_ALREADY_INITIALIZED));
	// https://learn.microsoft.com/en-us/windows/win32/api/winsvc/nf-winsvc-openscmanagera ;
	m_handle = ::OpenSCManager(nullptr, nullptr, _dw_access);
	if (nullptr == m_handle)
		m_error.Last();

	return m_error;
}

/////////////////////////////////////////////////////////////////////////////

CSvcManager_Handle& CSvcManager_Handle::operator << (const SC_HANDLE _p_sc_handle) {
	this->Attach(_p_sc_handle);
	return *this;
}

CSvcManager_Handle& CSvcManager_Handle::operator >> (SC_HANDLE& _p_sc_handle) {
	_p_sc_handle = this->Detach();
	return *this;
}

/////////////////////////////////////////////////////////////////////////////

CSvcManager_Handle::operator const bool (void) const { return this->Is(); }
CSvcManager_Handle::operator const SC_HANDLE& (void) const { return this->m_handle; }

/////////////////////////////////////////////////////////////////////////////

bool CSvcManager_Handle::Is_acceptable(const SC_HANDLE _p_sc_handle) { return (nullptr != _p_sc_handle && INVALID_HANDLE_VALUE != _p_sc_handle); }

/////////////////////////////////////////////////////////////////////////////

CSvcManager:: CSvcManager(void) { m_error << __MODULE__ << S_OK; }
CSvcManager::~CSvcManager(void) {}

/////////////////////////////////////////////////////////////////////////////

TSysErrRef CSvcManager::Error(void) const { return m_error; }
bool       CSvcManager::Is   (void) const {
	m_error << __MODULE__ << S_OK;
	if (m_handle.Is() == false) m_error << ERROR_INVALID_DATA; return !m_error;
}

DWORD    CSvcManager::Open (const CCrtData& crt_data, const DWORD dwReason, CService& svc_obj)
{
	DWORD d_result = this->Open(crt_data.Properties().Name(), dwReason, svc_obj);
	return d_result;
}

DWORD    CSvcManager::Open (LPCTSTR _lp_sz_svc_name , const DWORD dwReason, CService& _service) {
	m_error << __MODULE__ << S_OK;

	if (nullptr == _lp_sz_svc_name || 0 == ::_tcslen(_lp_sz_svc_name))
		return (m_error << E_INVALIDARG);

	_service.Cfg().Data().szSericeName = _lp_sz_svc_name;

	_service << ::OpenService(m_handle, _lp_sz_svc_name, dwReason);
	if (_service.Is() == false) {
		m_error.Last();
	}
	return m_error;
}

const
TSvcOperate& CSvcManager::Operate(void) const { return this->m_operate; }
TSvcOperate& CSvcManager::Operate(void)       { return this->m_operate; }

const
CSetup&  CSvcManager::Setup (void) const { return this->m_setup; }
CSetup&  CSvcManager::Setup (void)       { return this->m_setup; }

/////////////////////////////////////////////////////////////////////////////
const
TSCHandle&  CSvcManager::Handle(void) const { return m_handle; }
TSCHandle&  CSvcManager::Handle(void)       { return m_handle; }
CSvcManager::operator SC_HANDLE(void) const { return m_handle; }

/////////////////////////////////////////////////////////////////////////////

CSvcManager&  CSvcManager::operator= (const SC_HANDLE _handle)
{
	DWORD d_result = this->Handle().Attach(_handle);
	if (FAILED(d_result))
		m_error = this->Handle().Error();
	return *this;
}