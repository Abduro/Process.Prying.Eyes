/*
	Created by Tech_dog (ebontrop@gmail.com) on 13-Aug-2012 at 0:34:07.495 am, GMT+3, Rostov-on-Don, Monday;
	This is PulsePay UAC compatable project system security data provider interface implementation file.
	-----------------------------------------------------------------------------
	Adopted to Internext project on 21-Aug-2014 at 0:03:27.335 am, GMT+3, Taganrog, Thursday; *needs more precise info;
	Adopted to Ebo Pack project on 21-Jan-2021 at 0:22:06.807 am, GMT+7, Novosibirsk, Thursday;
	-----------------------------------------------------------------------------
	Adopted to Process Prying Eyes project on 10-Mar-2023 at 20:12:42.8772651, UTC+7, Novosibirsk, Friday;
*/
#include "shared.sec.provider.h"

using namespace shared::security;

#ifndef _LONG_PATH
#define _LONG_PATH (_MAX_PATH + _MAX_DRIVE + _MAX_DIR + _MAX_FNAME + _MAX_EXT)
#endif

/////////////////////////////////////////////////////////////////////////////

namespace shared { namespace security { namespace _impl {
}}}
using namespace shared::security::_impl;

/////////////////////////////////////////////////////////////////////////////

TSecProvider:: CSecurityProvider (void) { m_error << __MODULE__ << S_OK; }
TSecProvider:: CSecurityProvider (const TSecProvider& _ref) : TSecProvider() { *this = _ref; }
TSecProvider::~CSecurityProvider (void) {}

/////////////////////////////////////////////////////////////////////////////

HRESULT    TSecProvider::Elevate (void) {
	m_error << __MODULE__ << S_OK;

	string_t cs_full_path;
	
	TCHAR buffer[_LONG_PATH] = {0};
	if (!::GetModuleFileName(NULL, buffer, _countof(buffer)))
		return (m_error << ::GetLastError());

	SHELLEXECUTEINFO sei = {0};
	sei.cbSize = sizeof(SHELLEXECUTEINFO);
	sei.lpVerb = _T("runas");
	sei.lpFile = cs_full_path.c_str();
	sei.hwnd   = HWND_DESKTOP;
	sei.nShow  = SW_NORMAL;
	const BOOL b_result = ::ShellExecuteEx(&sei); // https://docs.microsoft.com/en-us/windows/win32/api/shellapi/nf-shellapi-shellexecuteexw
	if (FALSE == b_result)
		m_error.Last();

	return m_error;
}

TSysErrRef TSecProvider::Error   (void) const { return m_error; }

HRESULT    TSecProvider::GetSignature(LPCTSTR pszFilePath, TCertSignature& _info)  {
	pszFilePath; _info;
	m_error << __MODULE__ << S_OK;

	TCertProvider cert_prov;

	if (S_OK != cert_prov.Get(pszFilePath))
		m_error = cert_prov.Error();

	return m_error;
}

bool       TSecProvider::IsAccessible(LPCTSTR pszFilePath, const DWORD dwLevel) {
	pszFilePath; dwLevel;
	m_error << __MODULE__ << S_OK;
	bool b_result = false;

	if (NULL == pszFilePath || 0 == ::_tcslen(pszFilePath)) {
		m_error << E_INVALIDARG;
		return b_result;
	}

	TUserCurrent user_acc;
	m_error = user_acc.Error();
	if (m_error)
		return b_result;

	TFileAccess file_acc(pszFilePath);
	m_error = file_acc.Error();
	if (m_error)
		return b_result;

	b_result = (NULL == file_acc.ControlList()); // no any restriction;
	if (b_result)
		return b_result;

	b_result = user_acc.HasAccessTo(file_acc.ControlList(), dwLevel);

	return b_result;
}

bool       TSecProvider::IsAdminRole (void) const {
	BOOL b_result = FALSE;

	SID_IDENTIFIER_AUTHORITY sid_nt_authority = SECURITY_NT_AUTHORITY;
	PSID pAdministratorsGroup = NULL;
	b_result = ::AllocateAndInitializeSid(
		&sid_nt_authority,
		2,
		SECURITY_BUILTIN_DOMAIN_RID,
		DOMAIN_ALIAS_RID_ADMINS    ,
		0,
		0,
		0,
		0,
		0,
		0,
		&pAdministratorsGroup
	);

	if (b_result) {
		if (!::CheckTokenMembership(NULL, pAdministratorsGroup, &b_result))
			b_result = FALSE;
		::FreeSid(pAdministratorsGroup);  pAdministratorsGroup = NULL;
	}

	return !!b_result;
}

/////////////////////////////////////////////////////////////////////////////

TSecProvider& TSecProvider::operator = (const TSecProvider& _ref) { this->m_error = _ref.Error(); return *this; }

/////////////////////////////////////////////////////////////////////////////

TUACCompatible:: CUACCompatible (void) { m_error << __MODULE__ << S_OK; }
TUACCompatible:: CUACCompatible (const TUACCompatible& _ref) : TUACCompatible() { *this = _ref; }
TUACCompatible::~CUACCompatible (void) {}

/////////////////////////////////////////////////////////////////////////////

TSysErrRef TUACCompatible::Error (void) const { return m_error; }

HRESULT    TUACCompatible::IsGroup_member (const DWORD dwRelativeID, const BOOL bProcessRelative, BOOL& bIsMember_ref) {
	dwRelativeID; bProcessRelative; bIsMember_ref;
	m_error << __MODULE__ << S_OK;

	bIsMember_ref     = FALSE;
	HANDLE  hToken    = NULL ; 
	HANDLE  hDupToken = NULL ;
	PSID    pSid      = NULL ;

	SID_IDENTIFIER_AUTHORITY SidAuthority = SECURITY_NT_AUTHORITY;

	if (bProcessRelative || !::OpenThreadToken(::GetCurrentThread(), TOKEN_QUERY | TOKEN_DUPLICATE, TRUE, &hToken))
	{
		if (!::OpenProcessToken(::GetCurrentProcess(), TOKEN_QUERY | TOKEN_DUPLICATE, &hToken))
		{
			return (m_error << __LastErrToHresult());
		}
	}

	if (!::DuplicateToken(hToken, SecurityIdentification, &hDupToken))
	{
		::CloseHandle(hToken);
		return (m_error << __LastErrToHresult());
	}

	::CloseHandle(hToken);
	hToken = hDupToken;

	if (!::AllocateAndInitializeSid(&SidAuthority, 2, SECURITY_BUILTIN_DOMAIN_RID, dwRelativeID, 0, 0, 0, 0, 0, 0, &pSid))
	{
		::CloseHandle(hToken);
		return (m_error << __LastErrToHresult());
	}

	if (!::CheckTokenMembership(hToken, pSid, &bIsMember_ref))
	{
		::CloseHandle(hToken);
		::FreeSid(pSid);
		bIsMember_ref = FALSE;
		return (m_error << __LastErrToHresult());
	}

	::CloseHandle(hToken);
	::FreeSid(pSid);

	return m_error;
}

HRESULT    TUACCompatible::IsRole(const UACRole eRole) {
	eRole;
	m_error << __MODULE__ << S_OK;

	switch (eRole) {
	case UAC_Administrator: {
		BOOL bIsMember = FALSE;
		this->IsGroup_member(DOMAIN_ALIAS_RID_ADMINS, FALSE, bIsMember);

		if ((bool)m_error == false) {
			m_error << (bIsMember ? S_OK : S_FALSE);
		}
	} break;
	default:
		m_error << CO_E_NOT_SUPPORTED = _T("This role is not supported yet;");
	}

	return m_error;
}

HRESULT    TUACCompatible::RunAs (const UACRole eRole) {
	eRole;
	m_error << __MODULE__ << S_OK;
	switch (eRole)
	{
	case UAC_Administrator: {
		TSecProvider sec_prov; sec_prov.Elevate();
		m_error = sec_prov.Error();
	} break;
	default:
		m_error << CO_E_NOT_SUPPORTED = _T("This role is not supported yet;");
	}

	return m_error;
}

/////////////////////////////////////////////////////////////////////////////

CUACCompatible& CUACCompatible::operator = (const CUACCompatible& _ref) { this->m_error = _ref.Error(); return *this; }