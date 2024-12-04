/*
	Created by Tech_dog (ebontrop@gmail.com) on 6-Aug-2020 at 3:39:53p, UTC+7, Novosibirsk, Thursday;
	This is Ebo Pack solution system service configuration information interface implementation file.
	-----------------------------------------------------------------------------
	Adopted to Process Prying Eyes project on 12-Feb-2023 at 11:44:57.5791614, UTC+7, Novosibirsk, Sunday;
*/
#include "svc.cfg.h"

using namespace service::control;

/////////////////////////////////////////////////////////////////////////////

CCfg_Base:: CCfg_Base(void) : m_owner(nullptr) { m_error << __MODULE__ << S_OK; }
CCfg_Base:: CCfg_Base(const CCfg_Base& _ref) : CCfg_Base() { *this = _ref; }
CCfg_Base::~CCfg_Base(void) {}

/////////////////////////////////////////////////////////////////////////////

TSysErrRef  CCfg_Base::Error (void) const { return m_error; }
SC_HANDLE   CCfg_Base::Owner (void) const { return m_owner; }
DWORD       CCfg_Base::Owner (const SC_HANDLE _sc_handle)   {
	m_error << __MODULE__ << S_OK;

	if (NULL == _sc_handle || INVALID_HANDLE_VALUE == _sc_handle)
		m_error << ERROR_INVALID_STATE;

	this->m_owner = _sc_handle;
	
	return (DWORD)m_error;
}

bool        CCfg_Base::Valid (void) const { return (nullptr != m_owner && INVALID_HANDLE_VALUE != m_owner); }

/////////////////////////////////////////////////////////////////////////////

CCfg_Base::operator const SC_HANDLE&(void) const { return m_owner; }

/////////////////////////////////////////////////////////////////////////////

CCfg_Base&  CCfg_Base::operator  = (const CCfg_Base& _ref) {
	this->m_owner = _ref.m_owner;
	this->m_error = _ref.m_error; return *this;
}
CCfg_Base&  CCfg_Base::operator << (const SC_HANDLE _sc_handle) { this->Owner(_sc_handle); return *this; }

/////////////////////////////////////////////////////////////////////////////

CCfg:: CCfg (void) : TBase() { }
CCfg:: CCfg (const CCfg& _ref) : CCfg() { *this = _ref; }
CCfg::~CCfg (void) {}

/////////////////////////////////////////////////////////////////////////////
const
CCfg::qry_svc_cfg& CCfg::Data (void) const { return  m_data; }
CCfg::qry_svc_cfg& CCfg::Data (void)       { return  m_data; }
bool      CCfg::Is   (void) const  { return  TBase::Valid(); }

DWORD   CCfg::Query(const SC_HANDLE _p_svc_handle) {
	m_error << __MODULE__ << S_OK;

	DWORD d_result = this->Owner(_p_svc_handle);
	if (this->Is())
		this->Reset();

	if (S_OK != d_result) { // TODO: needs to be reviewed for returning correct result;
		return (DWORD)m_error;
	}

	DWORD dwBytesNeeded = 0;

	BOOL b_result = ::QueryServiceConfig(m_owner, NULL, 0, &dwBytesNeeded);
	DWORD err_last = ::GetLastError();

	if (FALSE == b_result && ERROR_INSUFFICIENT_BUFFER != err_last) // cannot get required size of data buffer;
		return (m_error << err_last);

	TRawData raw_data;
	d_result = raw_data.Create(dwBytesNeeded);

	if (S_OK != d_result)
		return (m_error = raw_data.Error());

	LPQUERY_SERVICE_CONFIG p_data = reinterpret_cast<LPQUERY_SERVICE_CONFIG>(raw_data.GetData());

	b_result = ::QueryServiceConfig(m_owner, p_data, raw_data.GetSize(), &dwBytesNeeded);
	if (FALSE == b_result)
		m_error.Last();
	else {
		
		if (p_data) {
			m_data.dwServiceType    = p_data->dwServiceType;
			m_data.dwStartType      = p_data->dwStartType  ;
			m_data.szBinaryPathName = p_data->lpBinaryPathName;
			m_data.szDisplayName    = p_data->lpDisplayName;
			m_data.szLoadOrderGroup = p_data->lpLoadOrderGroup;
			m_data.szSericeName     = _T("#na"); // must be installed by SC manager or enumerator directly;
		}
	}
	return m_error;
}

DWORD   CCfg::Reset(void) {
	m_error << __MODULE__ << S_OK;

	if (this->Is() == false)
		return (m_error << ERROR_INVALID_DATA);

	return (DWORD)m_error;
}

/////////////////////////////////////////////////////////////////////////////

CCfg&     CCfg::operator = (const CCfg& _ref) { _ref;
	(TBase&)*this = (const TBase&)_ref;
	this->m_data  = _ref.m_data;
	return *this;
}
CCfg&     CCfg::operator << (const SC_HANDLE _p_sc_handle) {
	this->Query(_p_sc_handle);
	return *this;
}

/////////////////////////////////////////////////////////////////////////////

CCfg_Ex:: CCfg_Ex (void) {}
CCfg_Ex:: CCfg_Ex (const CCfg_Ex& _ref) : CCfg_Ex() { *this = _ref; }
CCfg_Ex::~CCfg_Ex (void) { this->Reset(); }

/////////////////////////////////////////////////////////////////////////////

LPCTSTR   CCfg_Ex::Description(void) const { return m_szDesc.c_str(); }
DWORD     CCfg_Ex::Description(LPCTSTR _lp_sz_desc) {
	m_error << __MODULE__ << S_OK;

	if (TBase::Valid() == false)
		return (m_error << ERROR_INVALID_STATE);

	SERVICE_DESCRIPTION desc_local = {0};
	string_t cs_desc(_lp_sz_desc);
	desc_local.lpDescription = const_cast<TCHAR*>(cs_desc.c_str());
	const 
	BOOL b_result  = ::ChangeServiceConfig2 ( m_owner, SERVICE_CONFIG_DESCRIPTION, &desc_local );

	if (FALSE == b_result)
		m_error.Last();
	else
		m_szDesc = _lp_sz_desc;

	return (DWORD)m_error;
}

bool     CCfg_Ex::Is   (void) const { return  TBase::Valid(); }
DWORD    CCfg_Ex::Query(const SC_HANDLE _p_svc_handle) {
	m_error << __MODULE__ << S_OK;

	DWORD d_result = this->Owner(_p_svc_handle);
	if (this->Is())
		this->Reset();

	if (S_OK != d_result) { // TODO: needs to be reviewed for returning correct result;
		return (DWORD)m_error;
	}

	DWORD dwBytesNeeded = 0;

	BOOL b_result = ::QueryServiceConfig2(m_owner, SERVICE_CONFIG_DESCRIPTION, NULL, 0, &dwBytesNeeded);
	DWORD err_last = ::GetLastError();

	if (FALSE == b_result && ERROR_INSUFFICIENT_BUFFER != err_last) // cannot get required size of data buffer;
		return (m_error << err_last);

	TRawData raw_data;
	d_result = raw_data.Create(dwBytesNeeded);

	if (S_OK != d_result)
		return (m_error = raw_data.Error());

	b_result = ::QueryServiceConfig2(m_owner, SERVICE_CONFIG_DESCRIPTION, raw_data.GetData(), raw_data.GetSize(), &dwBytesNeeded);
	if (FALSE == b_result)
		m_error.Last();
	else {
		LPSERVICE_DESCRIPTION p_data = reinterpret_cast<LPSERVICE_DESCRIPTION>(raw_data.GetData());
		if (p_data) {
			m_szDesc = p_data->lpDescription;
		}
	}

	return (DWORD)m_error;
}

DWORD   CCfg_Ex::Reset(void) {
	m_error << __MODULE__ << S_OK;

	if (this->Is() == false)
		return (m_error << ERROR_INVALID_DATA);

	m_szDesc  = _T("");
	return !(bool)m_error;
}

/////////////////////////////////////////////////////////////////////////////

CCfg_Ex&  CCfg_Ex::operator = (const CCfg_Ex& _ref) {
	(TBase&)*this = (const TBase&)_ref;
	this->m_szDesc = _ref.m_szDesc;
	return *this;
}
CCfg_Ex&  CCfg_Ex::operator <<(const SC_HANDLE _p_sc_handle) {
	this->Query(_p_sc_handle);
	return *this;
}

/////////////////////////////////////////////////////////////////////////////

CCfgToStr:: CCfgToStr (LPCTSTR _lp_sz_separator) : m_sepa(_lp_sz_separator) {}
CCfgToStr::~CCfgToStr(void) {}

/////////////////////////////////////////////////////////////////////////////

string_t  CCfgToStr::PrintError(const DWORD _svc_ctrl) {
	string_t cs_error;

	if (0 != (SERVICE_ERROR_CRITICAL & _svc_ctrl)) { if (cs_error.empty() == false) cs_error += m_sepa; cs_error += _T("SERVICE_ERROR_CRITICAL"); }
	if (0 != (SERVICE_ERROR_IGNORE   & _svc_ctrl)) { if (cs_error.empty() == false) cs_error += m_sepa; cs_error += _T("SERVICE_ERROR_IGNORE"); }
	if (0 != (SERVICE_ERROR_NORMAL   & _svc_ctrl)) { if (cs_error.empty() == false) cs_error += m_sepa; cs_error += _T("SERVICE_ERROR_NORMAL"); }
	if (0 != (SERVICE_ERROR_SEVERE   & _svc_ctrl)) { if (cs_error.empty() == false) cs_error += m_sepa; cs_error += _T("SERVICE_ERROR_SEVERE"); }

	return cs_error;
}

string_t  CCfgToStr::PrintStart(const DWORD _svc_start) {
	string_t cs_start;

	if (SERVICE_AUTO_START    == _svc_start) { if (cs_start.empty() == false) cs_start += m_sepa; cs_start += _T("SERVICE_AUTO_START"); }
	if (SERVICE_BOOT_START    == _svc_start) { if (cs_start.empty() == false) cs_start += m_sepa; cs_start += _T("SERVICE_BOOT_START"); }
	if (SERVICE_DEMAND_START  == _svc_start) { if (cs_start.empty() == false) cs_start += m_sepa; cs_start += _T("SERVICE_DEMAND_START"); }
	if (SERVICE_DISABLED      == _svc_start) { if (cs_start.empty() == false) cs_start += m_sepa; cs_start += _T("SERVICE_DISABLED"); }
	if (SERVICE_SYSTEM_START  == _svc_start) { if (cs_start.empty() == false) cs_start += m_sepa; cs_start += _T("SERVICE_SYSTEM_START"); }

	return cs_start;
}

string_t  CCfgToStr::PrintState(const DWORD _svc_state) {
	string_t cs_state;

	if (SERVICE_CONTINUE_PENDING == _svc_state) { if (cs_state.empty() == false) cs_state += m_sepa; cs_state += _T("SERVICE_CONTINUE_PENDING"); }
	if (SERVICE_PAUSE_PENDING    == _svc_state) { if (cs_state.empty() == false) cs_state += m_sepa; cs_state += _T("SERVICE_PAUSE_PENDING"); }
	if (SERVICE_PAUSED           == _svc_state) { if (cs_state.empty() == false) cs_state += m_sepa; cs_state += _T("SERVICE_PAUSED"); }
	if (SERVICE_RUNNING          == _svc_state) { if (cs_state.empty() == false) cs_state += m_sepa; cs_state += _T("SERVICE_RUNNING"); }
	if (SERVICE_START_PENDING    == _svc_state) { if (cs_state.empty() == false) cs_state += m_sepa; cs_state += _T("SERVICE_START_PENDING"); }
	if (SERVICE_STOP_PENDING     == _svc_state) { if (cs_state.empty() == false) cs_state += m_sepa; cs_state += _T("SERVICE_STOP_PENDING"); }
	if (SERVICE_STOPPED          == _svc_state) { if (cs_state.empty() == false) cs_state += m_sepa; cs_state += _T("SERVICE_STOPPED"); }

	return cs_state;
}
// https://docs.microsoft.com/en-us/windows/win32/api/winsvc/nf-winsvc-createservicea
#ifndef SERVICE_USER_OWN_PROCESS
#define SERVICE_USER_OWN_PROCESS 0x00000050
#endif

#ifndef SERVICE_USER_SHARE_PROCESS
#define SERVICE_USER_SHARE_PROCESS 0x00000060
#endif

string_t  CCfgToStr::PrintType (const DWORD _svc_type) {
	string_t cs_type;

	if (0 != (SERVICE_FILE_SYSTEM_DRIVER  & _svc_type)) { if (cs_type.empty() == false) cs_type += m_sepa; cs_type += _T("SERVICE_FILE_SYSTEM_DRIVER"); }
	if (0 != (SERVICE_KERNEL_DRIVER       & _svc_type)) { if (cs_type.empty() == false) cs_type += m_sepa; cs_type += _T("SERVICE_KERNEL_DRIVER"); }
	if (0 != (SERVICE_WIN32_OWN_PROCESS   & _svc_type)) { if (cs_type.empty() == false) cs_type += m_sepa; cs_type += _T("SERVICE_WIN32_OWN_PROCESS"); }
	if (0 != (SERVICE_WIN32_SHARE_PROCESS & _svc_type)) { if (cs_type.empty() == false) cs_type += m_sepa; cs_type += _T("SERVICE_WIN32_SHARE_PROCESS"); }
	if (0 != (SERVICE_USER_OWN_PROCESS    & _svc_type)) { if (cs_type.empty() == false) cs_type += m_sepa; cs_type += _T("SERVICE_USER_OWN_PROCESS"); }
	if (0 != (SERVICE_USER_SHARE_PROCESS  & _svc_type)) { if (cs_type.empty() == false) cs_type += m_sepa; cs_type += _T("SERVICE_USER_SHARE_PROCESS"); }
	if (0 != (SERVICE_INTERACTIVE_PROCESS & _svc_type)) { if (cs_type.empty() == false) cs_type += m_sepa; cs_type += _T("SERVICE_INTERACTIVE_PROCESS"); }

	return cs_type;
}