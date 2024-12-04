/*
	Created by Tech_dog (ebontrop@gmail.com) on 09-Jul-2010 at 6:45:56p, GMT+3, Rostov-on-Don, Friday;
	This is Row27 project data model error class implementation file.
	-----------------------------------------------------------------------------
	Adopted to Pulspay server project on 15-May-2012 at 9:44:59pm, GMT+3, Rostov-on-Don, Tuesday;
	-----------------------------------------------------------------------------
	Adopted to File Guardian (thefileguardian.com) on 28-Jan-2018 at 02:39:54.5969584, UTC+7, Phuket, Rawai, Sunday;
	Adopted to FakeGPS driver project on 13-Dec-2019 at 10:20:24a, UTC+7, Novosibirsk, Friday;
	-----------------------------------------------------------------------------
	Adopted to Process Prying Eyes project on 7-Feb-2023 at 12:45:01.0353035, UTC+7, Novosibirsk, Tuesday;
*/
#include "shared.sys.err.h"
#include "shared.classes.h"

using namespace shared::sys_core;

/////////////////////////////////////////////////////////////////////////////

namespace shared { namespace sys_core { namespace _impl {

VOID    Error_NormalizeMessage(string_t& buffer_ref)
{
	if (0 == buffer_ref.length())
		return;

	shared::sys_core::_replace(buffer_ref, _T("\r"), _T(" "));
	shared::sys_core::_replace(buffer_ref, _T("\n"), _T(" "));
	shared::sys_core::_trim   (buffer_ref);
}

}}}

using namespace shared::sys_core::_impl;
/////////////////////////////////////////////////////////////////////////////

CErr_Base:: CErr_Base(void) : m_desc(_T("#n/a")), m_module(_T("#n/a")) {}
CErr_Base:: CErr_Base(const CErr_Base& _ref) : CErr_Base() { *this = _ref; }
CErr_Base::~CErr_Base(void) {}

/////////////////////////////////////////////////////////////////////////////

LPCTSTR  CErr_Base::Desc  (void) const { SAFE_LOCK(m_lock);  return m_desc.c_str(); }
HRESULT  CErr_Base::Desc  (LPCTSTR _lp_sz_desc)
{
	HRESULT hr_ = S_OK;

	if (nullptr == _lp_sz_desc || !::_tcslen(_lp_sz_desc))
		return (hr_ = E_INVALIDARG);

	this->m_desc = _lp_sz_desc;

	return  hr_;
}

LPCTSTR  CErr_Base::Module(void) const { SAFE_LOCK(m_lock); return m_module.c_str(); }
void     CErr_Base::Module(LPCTSTR _v) { SAFE_LOCK(m_lock); m_module = _v;   }

/////////////////////////////////////////////////////////////////////////////

CErr_Base& CErr_Base::operator =  (const CErr_Base& _ref) { this->Desc(_ref.Desc()); this->Module(_ref.Module()); return *this; }
CErr_Base& CErr_Base::operator =  (LPCTSTR _lp_sz_v) {  this->Desc  (_lp_sz_v); return *this; }
CErr_Base& CErr_Base::operator << (LPCTSTR _lp_sz_v) {  this->Module(_lp_sz_v); return *this; }

/////////////////////////////////////////////////////////////////////////////

CErr_Base::operator LPCTSTR(void) const { return this->Desc(); }

/////////////////////////////////////////////////////////////////////////////

CComError:: CComError(void) : TBase(), m_result(OLE_E_BLANK) {}
CComError:: CComError(const _com_error& _error) : CComError() { *this << _error; }
CComError:: CComError(const CComError& _ref) : CComError() { *this = _ref; }
CComError::~CComError(void) {}

/////////////////////////////////////////////////////////////////////////////

HRESULT    CComError::Get (void) const { SAFE_LOCK(TBase::m_lock); return this->m_result; }
HRESULT    CComError::Set (const _com_error& _error)
{
	SAFE_LOCK(TBase::m_lock);
	HRESULT hr_ = S_OK;
	// https://stackoverflow.com/questions/10153873/how-do-i-use-com-ptr-t ;
	// https://learn.microsoft.com/en-us/cpp/cpp/com-ptr-t-class ;
	IErrorInfoPtr err_info; // based on class _com_ptr_t;
	if (nullptr != _error.ErrorInfo())
		err_info = _error.ErrorInfo(); // TODO: perhaps an attachment would be better in comparison with a making copy;
	else
		// https://docs.microsoft.com/en-us/windows/win32/api/oleauto/nf-oleauto-geterrorinfo ;
		hr_ = ::GetErrorInfo(0, &err_info);

	if (S_OK == hr_)
	{
		//TODO: must be reviewed because this code line makes a doublicate of IErrorInfo;
		//      in case when income argument may already have a pointer to IErrorInfo;
		_com_error err_record(_error.Error(), err_info, true);

		if (err_record.Description().length()) TBase::Desc((LPCTSTR)err_record.Description()); else TBase::Desc(_T("#n/a"));
		if (err_record.Source().length()) TBase::Module((LPCTSTR)err_record.Source()); else TBase::Module(_T("#n/a"));
	}
	else // S_FALSE
	{
		if (_error.Description().length()) TBase::Desc((LPCTSTR)_error.Description()); else TBase::Desc(_T("#n/a"));
		if (_error.Source().length()) TBase::Module((LPCTSTR)_error.Source()); else TBase::Module(_T("#n/a"));
	}

	Error_NormalizeMessage(TBase::m_desc);

	return  hr_;
}

void       CComError::Set (HRESULT _result)
{
	SAFE_LOCK(m_lock);
	this->m_result = _result;
	if (S_OK == this->Get())
	{
		TBase::Desc(_T("No error")); return;
	}
	else if (TBase::m_desc.empty() == false) TBase::m_desc.clear();

	_com_error com_err(_result);

	this->Set(com_err);
}

bool       CComError::Is_failure (void) const { return (FAILED(this->Get())); }
string_t   CComError::To_string  (LPCTSTR _lp_sz_sep) const
{
	static LPCTSTR lp_sz_pat = _T("Hresult=0x%x; Desc=%s; Module=%s;"); // default pattern;
	string_t s_pat(lp_sz_pat);

	if (_lp_sz_sep && ::_tcslen(_lp_sz_sep))
		_replace(s_pat, _T("; "), _lp_sz_sep);

	return string_t(CFormatter().Accept(s_pat.c_str(), this->Get(), TBase::Desc(), TBase::Module()));
}

/////////////////////////////////////////////////////////////////////////////

CComError& CComError::operator = (const CComError& _ref) {
	(TBase&)*this = (const TBase&)_ref;
	{
		SAFE_LOCK(TBase::m_lock);
		this->m_result = _ref.Get(); // silently copies the result value in order do not parse new _com_error object;
	}
	return  *this;
}

CComError& CComError::operator <<(const _com_error& _err) { this->Set(_err); return *this; }
CComError& CComError::operator <<(HRESULT _result) { this->Set(_result); return *this; }

/////////////////////////////////////////////////////////////////////////////

CComError::operator bool (void) const { return this->Is_failure(); }
CComError::operator HRESULT (void) const { return this->Get(); }

/////////////////////////////////////////////////////////////////////////////

CSysError:: CSysError(void) : TBase(), m_err_code(NO_ERROR) {}
CSysError:: CSysError(const CSysError& _ref) : CSysError() { *this = _ref; }
CSysError:: CSysError(DWORD _dw_error_code) : CSysError() { *this <<_dw_error_code; }
CSysError:: CSysError (LPCTSTR _lp_sz_module) : CSysError()  { *this << _lp_sz_module; }
CSysError::~CSysError(void) {}

/////////////////////////////////////////////////////////////////////////////

CErr_Base& CSysError::Base(void) { return (TBase&)*this; }

DWORD  CSysError::Get(void) const { SAFE_LOCK(TBase::m_lock); return this->m_err_code; }
void   CSysError::Set(const DWORD _err_code)
{
	SAFE_LOCK(TBase::m_lock);
	static const
	size_t n_sz_req = _MAX_PATH * 2;

	TCHAR sz_local[n_sz_req] = {0}; // using local buffer in order to avoid applying FORMAT_MESSAGE_ALLOCATE_BUFFER flag;
	const
	DWORD dw_copied = ::FormatMessage( // https://learn.microsoft.com/en-us/windows/win32/api/winbase/nf-winbase-formatmessage ;
		FORMAT_MESSAGE_FROM_SYSTEM|FORMAT_MESSAGE_IGNORE_INSERTS,
		nullptr     ,
		_err_code   ,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL), // this is 0x0 flag; otherwise, a format does not work;
		sz_local    ,
		n_sz_req - 1,
		nullptr
	);
	if (0 == dw_copied) // getting formatted message error occurred;
	{
		this->m_err_code = ::GetLastError();
		TBase::Desc(_T("Unable to retrieve error formatted message;"));
		TBase::Module(__MODULE__);
	}
	else
	{
		this->m_err_code = _err_code;
		TBase::Desc(sz_local); // it is assumed the module name is already set through TBase::Module() accessor;

		Error_NormalizeMessage(TBase::m_desc);
	}
}

void     CSysError::Last (void) { *this << ::GetLastError(); }

bool     CSysError::Is_failure(void) const { return (NO_ERROR != this->Get()); }
string_t CSysError::To_string (LPCTSTR _lp_sz_sep) const
{
	static LPCTSTR lp_sz_pat = _T("Code=%u; Desc=%s; Module=%s;"); // default pattern;
	string_t s_pat(lp_sz_pat);

	if (_lp_sz_sep && ::_tcslen(_lp_sz_sep))
		_replace(s_pat, _T("; "), _lp_sz_sep);

	return string_t(CFormatter().Accept(s_pat.c_str(), this->Get(), TBase::Desc(), TBase::Module()));
}

/////////////////////////////////////////////////////////////////////////////

CSysError& CSysError::operator = (const CSysError& _ref) {
	(TBase&)*this = (const TBase&)_ref;
	{
		SAFE_LOCK(TBase::m_lock);
		this->m_err_code = _ref.Get(); // silently copies the error code value in order do not call system format message;
	}
	return  *this;
}
CSysError& CSysError::operator <<(DWORD _dw_err_code) { this->Set(_dw_err_code); return *this; }
CSysError& CSysError::operator <<(LONG _l_err_code) { this->Set(_l_err_code); return *this; } // *implicit* conversion to DWORD;
CSysError& CSysError::operator = (LPCTSTR _lp_sz_desc) { (TBase&)*this = _lp_sz_desc; return *this; }
CSysError& CSysError::operator <<(LPCTSTR _lp_sz_module) {
	(TBase&)*this << _lp_sz_module; return *this;
}

/////////////////////////////////////////////////////////////////////////////

CSysError::operator bool (void) const { return this->Is_failure(); }
CSysError::operator DWORD(void) const { return this->Get(); }
CSysError::operator HRESULT (void) const { return __DwordToHresult(this->Get()); }

/////////////////////////////////////////////////////////////////////////////