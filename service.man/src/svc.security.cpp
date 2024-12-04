/*
	Created by Tech_dog (ebontrop@gmail.com) on 11-Aug-2020 at 7:27:44p, UTC+7, Novosibirsk, Tuesday;
	This is service manager security wrapper interface implementation file.
	-----------------------------------------------------------------------------
	Adopted to Process Prying Eyes project on 13-Feb-2023 at 03:22:14.8621216, UTC+7, Novosibirsk, Monday;
*/
#include "svc.security.h"

using namespace service::control;

/////////////////////////////////////////////////////////////////////////////

CExp_Access:: CExp_Access(void) { m_error << __MODULE__ << S_OK; }
CExp_Access:: CExp_Access(const CExp_Access& _ref) : CExp_Access() { *this = _ref; }
CExp_Access::~CExp_Access(void) { }

/////////////////////////////////////////////////////////////////////////////
const
EXPLICIT_ACCESS& CExp_Access::Get(void) const { return m_ex_acc; }
EXPLICIT_ACCESS& CExp_Access::Get(void)       { return m_ex_acc; }
TSysErrRef CExp_Access::Error  (void) const { return m_error ; }
DWORD      CExp_Access::Query  (const LPCTSTR _lp_sz_account)  {
	m_error << __MODULE__ << S_OK;

	if (nullptr == _lp_sz_account || 0 == ::_tcslen(_lp_sz_account))
		return (m_error << ERROR_INVALID_PARAMETER);

	string_t cs_acc(_lp_sz_account);
	::std::vector<TCHAR> v_acc(cs_acc.begin(), cs_acc.end()); // https://stackoverflow.com/questions/7352099/stdstring-to-char ;

	::BuildExplicitAccessWithName(&m_ex_acc, &v_acc[0], SERVICE_START | SERVICE_STOP | READ_CONTROL | DELETE, SET_ACCESS, NO_INHERITANCE);
	return (DWORD)m_error;
}

DWORD      CExp_Access::Reset  (void) {
	m_error << __MODULE__ << S_OK;

	RtlZeroMemory(&m_ex_acc, sizeof(EXPLICIT_ACCESS));
	return (DWORD)m_error;
}

/////////////////////////////////////////////////////////////////////////////

CExp_Access&  CExp_Access::operator  = (const CExp_Access& _ref) {
	this->Get() = _ref.Get();
	return *this;
}
CExp_Access&  CExp_Access::operator << (const LPCTSTR _lp_sz_account) {
	this->Query(_lp_sz_account);
	return *this;
}

/////////////////////////////////////////////////////////////////////////////

CSec_Desc:: CSec_Desc(void) : m_p_desc(NULL), m_pdacl(NULL) {  m_error << __MODULE__ << S_OK; }
CSec_Desc::~CSec_Desc(void) { if (this->Is()) { this->Reset(); } }

/////////////////////////////////////////////////////////////////////////////

p_sec_desc  CSec_Desc::Get   (void) const { return m_p_desc; }
TSysErrRef  CSec_Desc::Error (void) const { return m_error ; }
const bool  CSec_Desc::Is    (void) const { return (NULL != m_p_desc); }
DWORD       CSec_Desc::Query (const SC_HANDLE _p_service) {
	m_error << __MODULE__ << S_OK;

	if (NULL == _p_service)
		return (m_error << ERROR_INVALID_PARAMETER);

	if (this->Is())
		return (m_error << ERROR_ALREADY_INITIALIZED);

	DWORD dw_req = 0;

	if (FALSE == ::QueryServiceObjectSecurity(
		_p_service, DACL_SECURITY_INFORMATION, m_p_desc, 0, &dw_req)) {
		m_error.Last();

		if (ERROR_INSUFFICIENT_BUFFER != m_error.Get())
			return m_error;
		else
			m_error << S_OK;
	}

	TRawData raw_sd;
	DWORD dr_ = raw_sd.Create(dw_req);

	if (FAILURE(dr_)) {
		m_error = raw_sd.Error();
		return m_error;
	}

	m_p_desc = reinterpret_cast<PSECURITY_DESCRIPTOR>(raw_sd.GetData());

	if (FALSE == ::QueryServiceObjectSecurity(
		_p_service, DACL_SECURITY_INFORMATION, m_p_desc, raw_sd.GetSize(), &dw_req)) {
		m_error.Last();
		raw_sd.Clear(); m_p_desc = NULL;
	}
	else {
		m_p_desc = raw_sd.Detach();

		BOOL b_prensent = FALSE;
		BOOL b_default  = FALSE;

		const BOOL b_result = ::GetSecurityDescriptorDacl(
				m_p_desc, &b_prensent, &m_pdacl, &b_default
			);
		if (FALSE == b_result)
			m_error.Last();
	}
	return m_error;
}

DWORD       CSec_Desc::Reset (void) {
	m_error << __MODULE__ << S_OK;

	if (this->Is() == false)
		return m_error;

	const BOOL b_result = ::HeapFree(GetProcessHeap(), 0, (LPVOID)m_p_desc);
	if (FALSE == b_result)
		m_error.Last();
	else {
		m_p_desc = NULL;
		m_pdacl  = NULL;
	}

	return m_error;
}

/////////////////////////////////////////////////////////////////////////////

CSec_Desc&  CSec_Desc::operator  = (const CSec_Desc& _ref) {
	this->m_error  = _ref.Error();
	this->m_p_desc = _ref.Get () ;
	this->m_pdacl  = _ref.m_pdacl;

	CSec_Desc& ref_ = const_cast<CSec_Desc&>(_ref);
	ref_.m_p_desc = NULL;
	ref_.m_pdacl  = NULL;

	return *this;
}

CSec_Desc&  CSec_Desc::operator << (const SC_HANDLE _p_service) { this->Query(_p_service); return *this; }

/////////////////////////////////////////////////////////////////////////////

CSec_Desc::operator p_sec_desc const (void) const { return this->Get(); }
CSec_Desc::operator PACL const (void) const { return this->m_pdacl; }

/////////////////////////////////////////////////////////////////////////////

CSecurity:: CSecurity(void) { m_error << __MODULE__ << S_OK; }
CSecurity::~CSecurity(void) {}

/////////////////////////////////////////////////////////////////////////////

TSysErrRef  CSecurity::Error (void) const { return m_error; }

DWORD       CSecurity::Set   (SC_HANDLE _p_svc_man, LPCTSTR _lp_sz_svc_name) {
	m_error << __MODULE__ << S_OK;

	TCHAR t_buffer[_MAX_FNAME] = {0};
	DWORD d_req = _countof(t_buffer);

	if (FALSE == ::GetUserName(t_buffer, &d_req))
		m_error.Last();
	else
		this->Set(_p_svc_man, _lp_sz_svc_name, t_buffer);

	return m_error;
}

DWORD       CSecurity::Set   (SC_HANDLE _p_svc_man, LPCTSTR _lp_sz_svc_name, LPCTSTR _lp_sz_account) {
	m_error << __MODULE__ << S_OK;

	if (NULL == _p_svc_man)
		return (m_error << ERROR_INVALID_PARAMETER);

	if (NULL == _lp_sz_account || 0 == ::_tcslen(_lp_sz_account))
		return (m_error << ERROR_INVALID_PARAMETER);

	if (NULL == _lp_sz_svc_name || 0 == ::_tcslen(_lp_sz_svc_name))
		return (m_error << ERROR_INVALID_PARAMETER);

	CService_Handle h_service(::OpenService(_p_svc_man, _lp_sz_svc_name, READ_CONTROL | WRITE_DAC));
	if (h_service.Is() == false)
		return (m_error << ::GetLastError());

	DWORD dw_result = ERROR_SUCCESS;
	PACL pdacl_new  = NULL ;
	SECURITY_DESCRIPTOR sd = {0};

	DWORD dr_ = m_sec_d.Query(h_service);
	if (FAILURE(dr_)) {
		return (m_error = m_sec_d.Error());
	}

	dr_ = m_access.Query(_lp_sz_account);
	if (FAILURE(dr_)) {
		return (m_error = m_access.Error());
	}

	dw_result = ::SetEntriesInAcl(1, &m_access.Get(), m_sec_d, &pdacl_new);
	if (dw_result) {
		return (m_error << dw_result);
	}

	if (FALSE == ::InitializeSecurityDescriptor(&sd, SECURITY_DESCRIPTOR_REVISION)) {
		m_error.Last();
		goto __end_of_story;
	}

	if (FALSE == ::SetSecurityDescriptorDacl(&sd, TRUE, pdacl_new, FALSE)) {
		m_error.Last();
		goto __end_of_story;
	}

	if (FALSE == ::SetServiceObjectSecurity(h_service, DACL_SECURITY_INFORMATION, &sd)) {
		m_error.Last();
		goto __end_of_story;
	}

	__end_of_story:
	if (NULL != pdacl_new)
	::LocalFree(pdacl_new);

	return m_error;
}

/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////