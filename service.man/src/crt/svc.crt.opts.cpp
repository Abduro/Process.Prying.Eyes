/*
	Created by Tech_dog (ebontrop@gmail.com) on 11-Feb-2023 at 21:42:27.2288174, UTC+7, Novosibirsk, Saturday;
	This is Peocess Prying Eyes project shared service create options' interface implementation file;
*/
#include "svc.crt.opts.h"

using namespace service::setting;

/////////////////////////////////////////////////////////////////////////////

COption:: COption (void) : m_value(0) {}
COption:: COption (const COption& _ref) : COption() { *this = _ref; }
COption:: COption (DWORD _dw_value) : COption() { *this << _dw_value; }
COption::~COption (void) {}

/////////////////////////////////////////////////////////////////////////////

DWORD  COption::Get (void) const { return this->m_value; }
void   COption::Set (DWORD _value) { this->m_value = _value; }

/////////////////////////////////////////////////////////////////////////////

COption& COption::operator = (const COption& _ref) { *this << _ref.Get(); return *this; }
COption& COption::operator <<(DWORD _dw_value) { this->Set(_dw_value); return *this; }
COption& COption::operator +=(DWORD _dw_value) { this->m_value |= _dw_value; return *this; }
COption& COption::operator -=(DWORD _dw_value) { this->m_value &=~_dw_value; return *this; }

/////////////////////////////////////////////////////////////////////////////

COption::operator DWORD (void) const { return this->Get(); }

/////////////////////////////////////////////////////////////////////////////

CCtrlOption:: CCtrlOption (void) : TBase() {}
CCtrlOption::~CCtrlOption (void) {}

/////////////////////////////////////////////////////////////////////////////

bool CCtrlOption::CanPause (void) const { return 0 != (TBase::Get() & value::eCanContinue); }
bool CCtrlOption::CanStop  (void) const { return 0 != (TBase::Get() & value::eStoppable  ); }
bool CCtrlOption::Shutdown (void) const { return 0 != (TBase::Get() & value::eCanShutdown); }

bool CCtrlOption::Is_Valid (void) const { return 0 != TBase::m_value; }

string_t CCtrlOption::To_string (void) const
{
	string_t s_result(_T("Control="));

	bool b_separator = false;

	if (this->CanPause()) { if (b_separator) s_result += _T("|"); s_result += _T("SERVICE_ACCEPT_PAUSE_CONTINUE"); b_separator = true; }
	if (this->CanStop())  { if (b_separator) s_result += _T("|"); s_result += _T("SERVICE_ACCEPT_STOP"); b_separator = true; }
	if (this->Shutdown()) { if (b_separator) s_result += _T("|"); s_result += _T("SERVICE_ACCEPT_SHUTDOWN"); b_separator = true; }

	return s_result;
}

/////////////////////////////////////////////////////////////////////////////

CStartOption:: CStartOption (void) : TBase() {}
CStartOption::~CStartOption (void) {}

/////////////////////////////////////////////////////////////////////////////

bool CStartOption::OnAuto (void) const { return 0 != (TBase::Get() & value::eOnAuto ); }
bool CStartOption::OnDemand (void) const { return 0 != (TBase::Get() & value::eOnDemand ); }

bool CStartOption::Is_Valid (void) const { return this->OnAuto() || this->OnDemand(); }

string_t CStartOption::To_string (void) const
{
	string_t s_result(_T("Control="));

	bool b_separator = false;

	if (this->OnAuto()) { if (b_separator) s_result += _T("|"); s_result += _T("SERVICE_AUTO_START"); b_separator = true; }
	if (this->OnDemand()) { if (b_separator) s_result += _T("|"); s_result += _T("SERVICE_DEMAND_START"); b_separator = true; }

	return s_result;
}

/////////////////////////////////////////////////////////////////////////////

CTypeOption:: CTypeOption (void) : TBase() {}
CTypeOption::~CTypeOption (void) {}

/////////////////////////////////////////////////////////////////////////////

bool CTypeOption::IsDriver (void) const { return 0 != (TBase::Get() & value::eKernelDrv ); }
bool CTypeOption::IsUserMode(void) const { return 0 != (TBase::Get() & value::eService ); }

bool CTypeOption::Is_Valid (void) const { return this->IsDriver() || this->IsUserMode(); }

string_t CTypeOption::To_string (void) const
{
	string_t s_result(_T("Control="));

	bool b_separator = false;

	if (this->IsDriver()) { if (b_separator) s_result += _T("|"); s_result += _T("SERVICE_KERNEL_DRIVER"); b_separator = true; }
	if (this->IsUserMode()) { if (b_separator) s_result += _T("|"); s_result += _T("SERVICE_WIN32_OWN_PROCESS"); b_separator = true; }

	return s_result;
}

/////////////////////////////////////////////////////////////////////////////

CCrtOptions:: CCrtOptions (void) { m_error << __MODULE__ << S_OK; }
CCrtOptions:: CCrtOptions (const CCrtOptions& _ref) : CCrtOptions() { *this = _ref; }
CCrtOptions::~CCrtOptions (void) {}

/////////////////////////////////////////////////////////////////////////////
const
CCtrlOption&  CCrtOptions::Ctrl (void) const { return this->m_ctrl; }
CCtrlOption&  CCrtOptions::Ctrl (void)       { return this->m_ctrl; }
const
CStartOption& CCrtOptions::Start(void) const { return this->m_start;}
CStartOption& CCrtOptions::Start(void)       { return this->m_start;}
const
CTypeOption&  CCrtOptions::Type (void) const { return this->m_type; }
CTypeOption&  CCrtOptions::Type (void)       { return this->m_type; }

/////////////////////////////////////////////////////////////////////////////

TSysErrRef    CCrtOptions::Error(void) const { return this->m_error; }

bool CCrtOptions::Is_valid (void) const {
	m_error << __MODULE__ << S_OK;

	static LPCTSTR lp_sz_pat = _T("%s option is not assigned or has invalid flags;");

	// looks for the first option that is not assigned yet;
	if (false) {}
	else if (false == this->Ctrl().Is_Valid())  { m_error << ERROR_INVALID_FLAGS << CFormatter().Accept(lp_sz_pat, _T("Control")); }
	else if (false == this->Start().Is_Valid()) { m_error << ERROR_INVALID_FLAGS << CFormatter().Accept(lp_sz_pat, _T("Start")); }
	else if (false == this->Type().Is_Valid())  { m_error << ERROR_INVALID_FLAGS << CFormatter().Accept(lp_sz_pat, _T("Type")); }

	return !(bool)this->Error();
}

string_t CCrtOptions::To_string (LPCTSTR _lp_sz_sep) const {

	string_t s_sep(_lp_sz_sep); if (s_sep.empty()) s_sep = _T("; ");
	string_t s_result(_T("Options: "));

	s_result += this->Ctrl().To_string(); s_result += s_sep;
	s_result += this->Start().To_string(); s_result += s_sep;
	s_result += this->Type().To_string(); s_result += s_sep;

	return s_result;
}

/////////////////////////////////////////////////////////////////////////////

CCrtOptions&  CCrtOptions::operator = (const CCrtOptions&  _ref) { *this << _ref.Ctrl() << _ref.Start() << _ref.Type(); return *this; }
CCrtOptions&  CCrtOptions::operator <<(const CCtrlOption&  _ref) {  this->Ctrl () = _ref; return *this; }
CCrtOptions&  CCrtOptions::operator <<(const CStartOption& _ref) {  this->Start() = _ref; return *this; }
CCrtOptions&  CCrtOptions::operator <<(const CTypeOption&  _ref) {  this->Type () = _ref; return *this; }