/*
	Created by Tech_dog (ebontrop@gmail.com) on 6-Jun-2016 at 12:00:15p, UTC+7, Phuket, Rawai, Wednesday;
	This is Shared system service create data interface implementation file.
	( project: thefileguardian.com )
	-----------------------------------------------------------------------------
	Adopted to Ebo Pack on 11-Jul-2020 at 7:21:54p, UTC+7, Novosibirsk, Saturday;
	-----------------------------------------------------------------------------
	Adopted to Process Prying Eyes project on 11-Feb-2023 at 09:22:02.0500138, UTC+7, Novosibirsk, Saturday;
*/
#include "svc.crt.data.h"

using namespace service::setting;

/////////////////////////////////////////////////////////////////////////////

CCrtData:: CCrtData(void) { m_error << __MODULE__ << S_OK; }
CCrtData:: CCrtData(const CCrtData& _ref) : CCrtData() { *this = _ref; }
CCrtData::~CCrtData(void) {}

/////////////////////////////////////////////////////////////////////////////

TSysErrRef CCrtData::Error (void) const { return this->m_error; }
const
TCrtOpts&  CCrtData::Options (void) const { return this->m_opts; }
TCrtOpts&  CCrtData::Options (void)       { return this->m_opts; }
const
TCrtProps& CCrtData::Properties(void) const { return this->m_props; }
TCrtProps& CCrtData::Properties(void)       { return this->m_props; }

/////////////////////////////////////////////////////////////////////////////

bool     CCrtData::Is_valid(void) const {
	m_error << __MODULE__ << S_OK;

	if (false) {}
	else if (false == this->Options().Is_valid()) this->m_error = this->Options().Error();
	else if (false == this->Properties().Is_valid()) this->m_error = this->Properties().Error();

	return !(bool)this->Error();
}

string_t CCrtData::To_string (LPCTSTR _lp_sz_sep) const {

	string_t s_sep(_lp_sz_sep); if (s_sep.empty()) s_sep = _T("; ");
	string_t s_result(_T("Create Data: ")); s_result += s_sep; 

	s_result += this->Options().To_string(); s_result += s_sep;
	s_result += this->Properties().To_string(); s_result += s_sep;

	return s_result;
}

/////////////////////////////////////////////////////////////////////////////

CCrtData&  CCrtData::operator = (const CCrtData& _ref) {
	this->m_error  = _ref.Error();
	this->Options() = _ref.Options();
	this->Properties() = _ref.Properties();

	return *this;
}

/////////////////////////////////////////////////////////////////////////////