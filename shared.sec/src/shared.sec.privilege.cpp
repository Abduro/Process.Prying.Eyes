/*
	Created by Tech_dog (ebontrop@gmail.com) on 26-Jan-2015 at 00:00:00.0a, UTC+3, Taganrog, Monday;
	This is Web setup project generic security digital certificate privilege interface implementation file.
	-----------------------------------------------------------------------------
	Adopted to Ebo Pack security project on 21-Jan-2021 at 8:37:34.258 pm, UTC+7, Novosibirsk, Thursday;
	Adopted to Process Prying Eyes project on 10-Mar-2023 at 16:49:11.7056394, UTC+7, Novosibirsk, Friday;
*/
#include "shared.sec.privilege.h"

using namespace shared::security;
#if (0)
/////////////////////////////////////////////////////////////////////////////

TSecPrivName:: CPriv_Name (void) {}
TSecPrivName:: CPriv_Name (const TSecPrivName& _ref) : TSecPrivName() { *this = _ref; }
TSecPrivName::~CPriv_Name (void) {}

/////////////////////////////////////////////////////////////////////////////

LPCTSTR  TSecPrivName::Get(void) const { return m_name.c_str(); }
HRESULT  TSecPrivName::Set(const e_type _type) { HRESULT hr_ = S_OK ;
	_type;

	return hr_;
}
#endif
/////////////////////////////////////////////////////////////////////////////

CPrivilege:: CPrivilege (void) : m_state(e_state::e_none) { m_error << __MODULE__ << S_OK; }
CPrivilege:: CPrivilege (const CPrivilege& _ref) : CPrivilege() { *this = _ref; }
CPrivilege::~CPrivilege (void) {}

/////////////////////////////////////////////////////////////////////////////

TSysErrRef   CPrivilege::Error (void) const { return m_error; }
LPCTSTR      CPrivilege::Names (const e_name _type) const {
	_type;
	switch (_type) {
	case e_name::e_display:
	case e_name::e_sortcut: return m_names[_type].c_str();
	}
	return NULL;
}

HRESULT      CPrivilege::Names (const LUID& _luid) {
	_luid;
	m_error <<__MODULE__ << S_OK;

	LUID luid_ = { _luid.LowPart, _luid.HighPart };

	DWORD dw_req_size = 0;
	// looks for shortcut name first;
	{
		::LookupPrivilegeName(NULL, &luid_, NULL, &dw_req_size);
		if (0 == dw_req_size)
			return (m_error << __LastErrToHresult());

		CRawData raw_name(dw_req_size * sizeof(WCHAR));
		if (raw_name.IsValid() == false)
			return (m_error = raw_name.Error());

		if (!::LookupPrivilegeName(NULL, &luid_, reinterpret_cast<LPWSTR>(raw_name.GetData()), &dw_req_size))
			return (m_error << __LastErrToHresult());

		m_names[e_name::e_sortcut] = reinterpret_cast<LPCTSTR>(raw_name.GetData());
	}
	// looks for display name secondly;
	{
		TCHAR t_buffer[_MAX_PATH] = {0};
		DWORD d_lang_id = 0;
		dw_req_size = _countof(t_buffer);

		if (!::LookupPrivilegeDisplayName(NULL, m_names[e_name::e_sortcut].c_str(), t_buffer, &dw_req_size, &d_lang_id))
			return (m_error << __LastErrToHresult());

		m_names[e_name::e_display] = t_buffer;
	}
	return m_error;
}

TSecPrivState CPrivilege::State (void) const { return m_state; }
HRESULT       CPrivilege::State (const bool _b_enable) {
	_b_enable;

	if (m_names[e_name::e_sortcut].empty())
		return ((m_error << OLE_E_BLANK) = _T("Privilege name is not set;"));

	CAutoHandle hToken;

	if (!::OpenProcessToken(::GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES|TOKEN_QUERY, &hToken))
		return (m_error << __LastErrToHresult());

	TOKEN_PRIVILEGES tkp = {0};
	// https://docs.microsoft.com/en-us/windows/win32/api/winbase/nf-winbase-lookupprivilegevaluew
	if (!::LookupPrivilegeValue(NULL, m_names[e_name::e_sortcut].c_str(), &tkp.Privileges[0].Luid))
		return (m_error << __LastErrToHresult());

	tkp.PrivilegeCount = 1;
	tkp.Privileges[0].Attributes = (_b_enable ? SE_PRIVILEGE_ENABLED : 0);

	if (!::AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, NULL, 0))
		return (m_error << __LastErrToHresult());
	
	m_state = (_b_enable ? TSecPrivState::e_enable : TSecPrivState::e_disable);

	return m_error;
}

/////////////////////////////////////////////////////////////////////////////

CPrivilege& CPrivilege::operator = (const CPrivilege& _ref) {
	for (UINT i_ = 0; i_ < _countof(this->m_names) && i_ < _countof(_ref.m_names); i_++) {
		this->m_names[i_] = _ref.m_names[i_];
	}
	
	this->m_state = _ref.State(); return *this;
}
CPrivilege& CPrivilege::operator <<(const LUID& _luid) { this->Names(_luid); return *this; }

/////////////////////////////////////////////////////////////////////////////

CPrivileges:: CPrivileges (void) { m_error << __MODULE__ << S_OK; }
CPrivileges:: CPrivileges (const CPrivileges& _ref) : CPrivileges() { *this = _ref; }
CPrivileges::~CPrivileges (void) { if (m_privileges.empty() == false) m_privileges.clear(); }

/////////////////////////////////////////////////////////////////////////////

TSysErrRef    CPrivileges::Error (void) const { return m_error; }
const
TRawPrivileges& CPrivileges::Get (void) const { return m_privileges; }

HRESULT       CPrivileges::Set (const HANDLE _owner) {
	_owner;
	m_error << __MODULE__ << S_OK;

	DWORD d_handle_info = 0;

	if (NULL == _owner || FALSE == ::GetHandleInformation(_owner, &d_handle_info))
		return (m_error << E_HANDLE);

	DWORD dw_req_size = 0;

	::GetTokenInformation(_owner, TokenPrivileges, NULL, 0, &dw_req_size);
	if (0 == dw_req_size)
		return (m_error << __LastErrToHresult());

	CRawData raw_buffer(dw_req_size);
	if (raw_buffer.IsValid() == false)
		return (m_error = raw_buffer.Error());

	if (!::GetTokenInformation(_owner, TokenPrivileges, raw_buffer.GetData(), raw_buffer.GetSize(), &dw_req_size))
		return (m_error << __LastErrToHresult());

	const PTOKEN_PRIVILEGES p_privileges = reinterpret_cast<const PTOKEN_PRIVILEGES>(raw_buffer.GetData());
	if (NULL == p_privileges)
		return (m_error << __DwordToHresult(ERROR_UNSUPPORTED_TYPE));

	if (m_privileges.empty() == false)
		m_privileges.clear();

	for (DWORD i_ = 0; i_ < p_privileges->PrivilegeCount; i_++) {

		const LUID_AND_ATTRIBUTES& luid = p_privileges->Privileges[i_];
		CPrivilege sec_priv;
		if (FAILED(sec_priv.Names(luid.Luid))) {
			m_error = sec_priv.Error(); break;
		}
		try {
			m_privileges.push_back(sec_priv);
		}
		catch (const ::std::bad_alloc&) {
			m_error << E_OUTOFMEMORY; break;
		}
	}

	return m_error;
}

/////////////////////////////////////////////////////////////////////////////

CPrivileges&  CPrivileges::operator = (const CPrivileges& _ref) {
	try {
		this->m_privileges = _ref.Get();
	}
	catch (::std::bad_alloc&) {} // it looks like to be very useless here;
	return *this;
}
CPrivileges&  CPrivileges::operator <<(const HANDLE _owner) { this->Set(_owner); return *this; }