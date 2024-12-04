/*
	Created by Tech_dog (ebontrop@gmail.com) on 12-Feb-2023 at 05:15:28.5603758, UTC+7, Novosibirsk, Sunday;
	This is Peocess Prying Eyes project shared service create properties' interface implementation file;
*/
#include "svc.crt.props.h"

//#include <type_traits>
//#include <typeinfo>

using namespace service::setting;

/////////////////////////////////////////////////////////////////////////////

namespace service { namespace control { namespace _impl {

	static LPCTSTR lp_sz_prop_pattern = _T("%s property = %s");
	static string_t property_to_string (LPCTSTR _prop_name, LPCTSTR _value) {
		return string_t(CFormatter().Accept(lp_sz_prop_pattern, _prop_name, nullptr != _value ? _value : _T("#n/a")));
	}
}}}

using namespace service::control::_impl;
/////////////////////////////////////////////////////////////////////////////

CProperty:: CProperty (void) { m_error << __MODULE__ << S_OK; }
CProperty:: CProperty (const CProperty& _ref) : CProperty() { *this = _ref; }
CProperty:: CProperty (LPCTSTR _lp_sz_value) : CProperty() { *this << _lp_sz_value; }
CProperty::~CProperty (void) {}

/////////////////////////////////////////////////////////////////////////////

LPCTSTR CProperty::Get (void) const { return this->m_value.c_str(); }
bool    CProperty::Set (LPCTSTR _lp_sz_value)
{
	m_error << __MODULE__ << S_OK;

	if (false == CProperty::Is_valid(_lp_sz_value))
		return (m_error << ERROR_INVALID_PARAMETER);

	this->m_value = _lp_sz_value;

	return !(bool)this->Error();
}

bool CProperty::Is (void) const { return CProperty::Is_valid(this->Error()); }

TSysErrRef CProperty::Error (void) const { return this->m_error; }

/////////////////////////////////////////////////////////////////////////////

bool CProperty::Is_valid(LPCTSTR _lp_sz_value) { return !(nullptr == _lp_sz_value || 0 == ::_tcslen(_lp_sz_value)); }

/////////////////////////////////////////////////////////////////////////////

CProperty& CProperty::operator = (const CProperty& _ref) {
	this->m_value = _ref.Get();
	this->m_error = _ref.m_error;
	return *this;
}
CProperty& CProperty::operator <<(const string_t& _value) { *this << _value.c_str(); return *this; }
CProperty& CProperty::operator <<(LPCTSTR _lp_sz_value) { this->Set(_lp_sz_value); return *this; }

/////////////////////////////////////////////////////////////////////////////

CProperty::operator LPCTSTR (void) const { return this->Get(); }

/////////////////////////////////////////////////////////////////////////////

CProp_Account:: CProp_Account (void) : TBase() { TBase::m_error << __MODULE__; }
CProp_Account::~CProp_Account (void) {}

/////////////////////////////////////////////////////////////////////////////

bool CProp_Account::Set (LPCTSTR _lp_sz_value)
{
	TBase::m_error << __MODULE__ << S_OK;
	const bool b_result = TBase::Set(_lp_sz_value);
	if (false == b_result)
		TBase::m_error << __d(ERROR_INVALID_ACCOUNT_NAME);

	return b_result;
}

string_t CProp_Account::To_string (void) const {
	return ::property_to_string(_T("Account"), TBase::Get());
}

/////////////////////////////////////////////////////////////////////////////

CProp_Desc:: CProp_Desc (void) : TBase() { TBase::m_error << __MODULE__; }
CProp_Desc::~CProp_Desc (void) {}

/////////////////////////////////////////////////////////////////////////////

bool CProp_Desc::Set (LPCTSTR _lp_sz_value)
{
	TBase::m_error << __MODULE__ << S_OK;

	const bool b_result = TBase::Set(_lp_sz_value); // TODO: perhaps direct setting a value would be better;
	if (false == b_result)
		TBase::m_error << __d(NO_ERROR);

	return b_result;
}

string_t CProp_Desc::To_string (void) const {
	return property_to_string(_T("Description"), TBase::Get());
}

/////////////////////////////////////////////////////////////////////////////

CProp_Name:: CProp_Name (void) : TBase() { TBase::m_error << __MODULE__; }
CProp_Name::~CProp_Name (void) {}

/////////////////////////////////////////////////////////////////////////////

bool CProp_Name::Set (LPCTSTR _lp_sz_value)
{
	TBase::m_error << __MODULE__ << S_OK;
	const bool b_result = TBase::Set(_lp_sz_value);
	if (false == b_result)
		TBase::m_error << __d(ERROR_INVALID_SERVICENAME); // TODO: base class just checks a string, not format of name;

	return b_result;
}

string_t CProp_Name::To_string (void) const {
	return property_to_string(_T("Name"), TBase::Get());
}

/////////////////////////////////////////////////////////////////////////////

CProp_Path:: CProp_Path (void) : TBase() { TBase::m_error << __MODULE__; }
CProp_Path::~CProp_Path (void) {}

/////////////////////////////////////////////////////////////////////////////

bool CProp_Path::Set (LPCTSTR _lp_sz_value)
{
	TBase::m_error << __MODULE__ << S_OK;
	const bool b_result = TBase::Set(_lp_sz_value);
	if (b_result) {
		CGenericPath path_(_lp_sz_value);
		if ((bool)path_.Error())
			return (TBase::m_error = path_.Error());
		if (path_.IsFolder())
			return (TBase::m_error << ERROR_BAD_PATHNAME << _T("Executable path cannot be a folder;"));
		if (path_.Exists() == false)
			return (TBase::m_error = path_.Error());
	}

	return b_result;
}

string_t CProp_Path::To_string (void) const {
	return property_to_string(_T("Path"), TBase::Get());
}

/////////////////////////////////////////////////////////////////////////////

CProp_Title:: CProp_Title (void) : TBase() { TBase::m_error << __MODULE__; }
CProp_Title::~CProp_Title (void) {}

/////////////////////////////////////////////////////////////////////////////

string_t CProp_Title::To_string (void) const {
	return property_to_string(_T("Display Name"), TBase::Get());
}

/////////////////////////////////////////////////////////////////////////////

CCrtProperties:: CCrtProperties (void) { m_error << __MODULE__ << S_OK; }
CCrtProperties:: CCrtProperties (const CCrtProperties& _ref) : CCrtProperties() { *this = _ref; }
CCrtProperties::~CCrtProperties (void) {}

/////////////////////////////////////////////////////////////////////////////
const
CProp_Account& CCrtProperties::Account(void) const { return this->m_account; }
CProp_Account& CCrtProperties::Account(void)       { return this->m_account; }
const
CProp_Desc&    CCrtProperties::Desc (void) const { return this->m_desc; }
CProp_Desc&    CCrtProperties::Desc (void)       { return this->m_desc; }
const
CProp_Name&    CCrtProperties::Name (void) const { return this->m_name; }
CProp_Name&    CCrtProperties::Name (void)       { return this->m_name; }
const
CProp_Path&    CCrtProperties::Path (void) const { return this->m_path; }
CProp_Path&    CCrtProperties::Path (void)       { return this->m_path; }
const
CProp_Title&   CCrtProperties::Title(void) const { return this->m_title; }
CProp_Title&   CCrtProperties::Title(void)       { return this->m_title; }

/////////////////////////////////////////////////////////////////////////////

TSysErrRef CCrtProperties::Error (void) const { return this->m_error; }
bool       CCrtProperties::Is_valid (void) const {
	m_error << __MODULE__ << S_OK;

	// looks for the first property in error state;
	if (false) {}
	else if ((bool)this->Account().Error()) this->m_error = this->Account().Error();
	else if ((bool)this->Desc().Error()) this->m_error = this->Desc().Error();
	else if ((bool)this->Name().Error()) this->m_error = this->Name().Error();
	else if ((bool)this->Title().Error()) this->m_error = this->Title().Error();

	return !(bool)this->Error();
}

string_t CCrtProperties::To_string (LPCTSTR _lp_sz_sep) const {

	string_t s_sep(_lp_sz_sep); if (s_sep.empty()) s_sep = _T("; ");
	string_t s_result(_T("Properties: "));

	s_result += this->Account().To_string(); s_result += s_sep;
	s_result += this->Desc().To_string(); s_result += s_sep;
	s_result += this->Name().To_string(); s_result += s_sep;
	s_result += this->Title().To_string(); s_result += s_sep;

	return s_result;
}

/////////////////////////////////////////////////////////////////////////////

CCrtProperties& CCrtProperties::operator = (const CCrtProperties& _ref) {

	this->Account() = _ref.Account();
	this->Desc()    = _ref.Desc();
	this->Name()    = _ref.Name();
	this->Title()   = _ref.Title();

	return *this;
}