/*
	Created by Tech_dog (ebontrop@gmail.com) on 2-Feb-2015 at 00:00:00.0a, UTC+3, Taganrog, Monday;
	This is Web setup project operating system security account interface implementation file;
	--------------------------------------------------------------------------
	Adopted to File Guardian project on 17-Apr-2016 at 12:06:36 am, GMT+7, Phuket, Rawai, Saturday; (thefileguardian.com project);
	Adopted to Ebo Pack project on 21-Jan-2021 at 11:51:17.152 pm, UTC+7, Novosibirsk, Thursday;
	Adopted to Process Prying Eyes project on 10-Mar-2023 at 18:08:11.4436980, UTC+7, Novosibirsk, Friday;
*/
#include "shared.sec.user.account.h"

using namespace shared::security;

/////////////////////////////////////////////////////////////////////////////

TSecAccSID:: CAccountSID (void) { m_error << __MODULE__ << S_OK; }
TSecAccSID:: CAccountSID (const TSecAccSID& _ref) : TSecAccSID() { *this = _ref; }
TSecAccSID::~CAccountSID (void) {}

/////////////////////////////////////////////////////////////////////////////

TSysErrRef  TSecAccSID::Error (void) const { return m_error; }
const PSID  TSecAccSID::Get   (void) const { return (reinterpret_cast<PSID>(m_raw_sid.GetData())); }
string_t    TSecAccSID::GetAsText (void) const {
	m_error << __MODULE__ << S_OK;

	string_t cs_sid;

	if (this->Is() == false) {
		m_error << OLE_E_BLANK; return cs_sid;
	}
	LPTSTR lpszSid = NULL;

	BOOL bSuccess = ::ConvertSidToStringSid( this->Get(), &lpszSid );
	if (!bSuccess)
	{
		m_error << __LastErrToHresult();
		return (cs_sid);
	}
	
	cs_sid = lpszSid;

	::LocalFree(lpszSid);
	lpszSid = NULL;

	return cs_sid;
}

bool        TSecAccSID::Is    (void) const { return m_raw_sid.IsValid(); }

HRESULT     TSecAccSID::Reset (void) {
	m_error << __MODULE__ << S_OK;

	if (this->Is() == false)
		return (m_error = __DwordToHresult(ERROR_INVALID_STATE));

	HRESULT hr_ = m_raw_sid.Reset();
	if (FAILED(hr_))
		m_error = m_raw_sid.Error();

	return m_error;
}

HRESULT     TSecAccSID::Set   (LPCTSTR _lp_sz_user_accout) {
	_lp_sz_user_accout;
	m_error << __MODULE__ << S_OK;

	if (NULL == _lp_sz_user_accout || 0 == ::_tcslen(_lp_sz_user_accout))
		return (m_error << E_INVALIDARG);

	if (this->Is())
		return (m_error << __DwordToHresult(ERROR_ALREADY_INITIALIZED));

	DWORD cbUserSID = 0;
	DWORD cbDomain  = 0;
	SID_NAME_USE eType = SidTypeUnknown;
	CRawData raw_domain;

	do {
		if (cbUserSID)
			m_error = m_raw_sid.Create(cbUserSID);
		if (m_error)
			break;
		if (cbDomain)
			m_error = raw_domain.Create(cbDomain * sizeof(TCHAR));
		if (m_error)
			break;

		const BOOL bRetVal = ::LookupAccountName(
			NULL,
			_lp_sz_user_accout,
			reinterpret_cast<PSID>(m_raw_sid.GetData()), &cbUserSID,
			reinterpret_cast<LPTSTR>(raw_domain.GetData()), &cbDomain,
			&eType
		);

		if (!bRetVal)
			m_error = ::GetLastError();

		if (m_error.Get() != ERROR_INSUFFICIENT_BUFFER)
			break;

		if (!m_error)
			break ;
	} while (true);

	return m_error;
}

/////////////////////////////////////////////////////////////////////////////

TSecAccSID& TSecAccSID::operator = (const TSecAccSID& _ref) { *this << _ref.m_raw_sid; return *this; }
TSecAccSID& TSecAccSID::operator <<(const CRawData& _sid) {
	m_error << __MODULE__ << S_OK;
	if (this->Is())
		this->Reset();

	if (_sid.IsValid() == false)
		m_error << E_INVALIDARG;

	this->m_raw_sid.Create(_sid.GetData(), _sid.GetSize());
	if (this->m_raw_sid.IsValid() == false)
		m_error = m_raw_sid.Error();

	return *this;
}

TSecAccSID& TSecAccSID::operator <<(LPCTSTR _lp_sz_user_account) {
	m_error << __MODULE__ << S_OK;
	if (this->Is()) this->Reset();
	if (m_error)
		return *this;
	this->Set(_lp_sz_user_account); return *this;
}

/////////////////////////////////////////////////////////////////////////////

string_t    CSystemSIDs::GetSidAsText(const CSystemSIDs::_e _sid)
{
	string_t sid_;
	switch (_sid)
	{
	case CSystemSIDs::eLocalSystem: sid_ = _T("S-1-5-18"); break;
	default:                        sid_ = _T("S-0-0-00"); break;
	}
	return sid_;
}

/////////////////////////////////////////////////////////////////////////////

TUserAccount:: CUserAccount (void) { m_error << __MODULE__ << S_OK; }
TUserAccount:: CUserAccount (const TUserAccount& _ref) : TUserAccount() { *this = _ref; }
TUserAccount:: CUserAccount (LPCTSTR _lp_sz_name) : TUserAccount() { *this << _lp_sz_name; }
TUserAccount::~CUserAccount (void) {}

/////////////////////////////////////////////////////////////////////////////
const
TSecAccSID&    TUserAccount::SID (void) const { return m_acc_sid; }
TSysErrRef     TUserAccount::Error (void) const { return m_error; }

bool  TUserAccount::HasAccessTo (const PACL _p_dacl, const DWORD dwAccessLevel) const {
	_p_dacl; dwAccessLevel;
	m_error << __MODULE__ << S_OK;

	bool b_has = false;
	if (NULL == _p_dacl) {
		m_error << E_INVALIDARG; return b_has;
	}

	if (this->Is() == false) {
		m_error << OLE_E_BLANK; return b_has;
	}
	// https://learn.microsoft.com/en-us/windows/win32/api/accctrl/ns-accctrl-trustee_w ;
	TRUSTEE_ trustee = {0};
	::BuildTrusteeWithSid(&trustee, m_acc_sid.Get());
	// https://docs.microsoft.com/en-us/windows/win32/api/aclapi/nf-aclapi-geteffectiverightsfromaclw ;
	ACCESS_MASK acc_msk = 0;
	m_error << __DwordToHresult(::GetEffectiveRightsFromAcl(_p_dacl, &trustee, &acc_msk));
	if (!m_error)
		b_has = (0 != (dwAccessLevel & acc_msk));

	return b_has;
}

bool  TUserAccount::Is (void) const { return m_acc_sid.Is() && m_name.empty() == false; }

LPCWSTR  TUserAccount::Name (void) const { return m_name.c_str(); }

HRESULT  TUserAccount::Name (LPCTSTR _lp_sz_name) {
	_lp_sz_name;
	m_error << __MODULE__ << S_OK;

	if (NULL == _lp_sz_name || 0 == ::_tcslen(_lp_sz_name))
		return (m_error << E_INVALIDARG);

	if (m_acc_sid.Is())
		m_acc_sid.Reset();

	HRESULT hr_ = m_acc_sid.Set(_lp_sz_name);
	if (FAILED(hr_))
		m_error = m_acc_sid.Error();
	else
		m_name = _lp_sz_name;

	return m_error;
}

/////////////////////////////////////////////////////////////////////////////

TUserAccount&  TUserAccount::operator = (const TUserAccount& _ref) { this->m_acc_sid = _ref.m_acc_sid; this->m_name = _ref.Name(); return *this; }
TUserAccount&  TUserAccount::operator <<(LPCWSTR _lp_sz_name) { this->Name(_lp_sz_name); return *this; }

/////////////////////////////////////////////////////////////////////////////

TUserCurrent:: CCurrentUser (void) : TUser() { TUser::m_error << __MODULE__;
	TCHAR szUser[_MAX_PATH] = {0};
	DWORD dwSize = _countof(szUser);
	// https://docs.microsoft.com/en-us/windows/win32/api/winbase/nf-winbase-getusernamew
	if (::GetUserName(szUser, &dwSize))
		TUser::Name(szUser);
	else
		TUser::m_error << __LastErrToHresult();
}
TUserCurrent::~CCurrentUser (void) {}