/*
	Created by Tech_dog (ebontrop@gmail.com) on 8-Feb-2023 at 09:23:16.9494599, UTC+7, Novosibirsk, Wednesay;
	This is Process Prying Eyes project common implementation file;
*/
#include "shared.classes.h"

using namespace shared::sys_core;

/////////////////////////////////////////////////////////////////////////////

namespace shared { namespace sys_core { namespace _impl {

	bool _check_pattern(LPCTSTR _lp_sz_pat) { return !((nullptr == _lp_sz_pat || 0 == ::_tcslen(_lp_sz_pat))); }
}}}
using namespace shared::sys_core::_impl;
/////////////////////////////////////////////////////////////////////////////

CFormatter:: CFormatter (void) {}
CFormatter:: CFormatter (const CFormatter& _ref) : CFormatter() { *this = _ref; }
CFormatter::~CFormatter (void) {}

/////////////////////////////////////////////////////////////////////////////

LPCTSTR   CFormatter::Accept(LPCTSTR _lp_sz_pat, ...)
{
	if (false == _check_pattern(_lp_sz_pat))
		return *this;

	va_list  args_;
	va_start(args_, _lp_sz_pat);

	this->Accept(_lp_sz_pat, args_);

	va_end  (args_);

	return (LPCTSTR)(*this);
}
#if (1)
LPCTSTR   CFormatter::Accept(LPCTSTR _lp_sz_pat, va_list _args)
{
	size_t t_size = 0;
	HRESULT hr_ = S_OK;
	try {
		do {
			t_size += 512;
			TCHAR*   p_sz_buf = new TCHAR[t_size];
			::memset(p_sz_buf, 0, t_size * sizeof(TCHAR));
			// https://learn.microsoft.com/en-us/windows/win32/api/strsafe/nf-strsafe-stringcchvprintfexw ; looks like a replacement for _vsntprintf_s;
			hr_ = ::StringCchVPrintfEx(
				p_sz_buf, t_size, NULL, NULL, 0, _lp_sz_pat, _args
			);
			if (SUCCEEDED(hr_))
				this->m_sz_buf = p_sz_buf;

			if (p_sz_buf) {
				delete p_sz_buf; p_sz_buf = nullptr;
			}
		}
		while(STRSAFE_E_INSUFFICIENT_BUFFER == hr_);
	}
	catch (const ::std::bad_alloc&)
	{}
	return (LPCTSTR)(*this);	
}
#else
LPCTSTR   CFormatter::Accept(LPCTSTR _lp_sz_pat, va_list _args)
{
	if (this->m_sz_buf.empty() == false)
		this->m_sz_buf.clear();

	TCHAR buf_[_MAX_PATH] = {0};
	// https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/vsnprintf-s-vsnprintf-s-vsnprintf-s-l-vsnwprintf-s-vsnwprintf-s ;
	const int n_size = ::_vsntprintf_s(buf_, _countof(buf_), _TRUNCATE, _lp_sz_pat, _args);
	if (0 > n_size) {
		// https://stackoverflow.com/questions/2342162/stdstring-formatting-like-sprintf ; how to avoid a trunkating;
		/*truncated or invalid argument error*/
	}

	return (LPCTSTR)(*this);	
}
#endif
/////////////////////////////////////////////////////////////////////////////

CFormatter&  CFormatter::operator = (const CFormatter& _ref) { this->m_sz_buf = _ref.m_sz_buf; return *this; }

/////////////////////////////////////////////////////////////////////////////

CFormatter::operator LPCTSTR (void) const { return this->m_sz_buf.c_str(); }

namespace shared { namespace sys_core {
/////////////////////////////////////////////////////////////////////////////
// https://stackoverflow.com/questions/3418231/replace-part-of-a-string-with-another-string ;
HRESULT _replace (string_t& _content, const string_t& _s_search, const string_t& _s_replace)
{
	HRESULT hr_ = S_OK;
	size_t pos  = 0;
	while ((pos = _content.find(_s_search, pos)) != string_t::npos) {
		_content.replace(pos, _s_search.length(), _s_replace);
		pos += _s_replace.length();
	}

	return hr_;
}

HRESULT _replace (string_t& _content, LPCTSTR _lp_sz_search, LPCTSTR _lp_sz_replace)
{
	return shared::sys_core::_replace(_content, string_t(_lp_sz_search), string_t(_lp_sz_replace));
}

::std::vector<string_t> _split (const string_t& _text, const string_t& _separator) {

	::std::vector<string_t> v_result;

	//			if (_text.empty() || _separator.empty())
	//				return v_result;

	string_t text_ = _text;

	size_t pos_1st = 0;
	size_t pos_2nd = text_.find(_separator);

	while (pos_2nd!= ::std::string::npos)
	{
		if (pos_1st != pos_2nd)
		{
			string_t word_ = text_.substr(pos_1st, pos_2nd - pos_1st);
			v_result.push_back(word_);
		}
		text_ = text_.substr(pos_2nd + _separator.length());
		pos_2nd = text_.find(_separator);
	}

	v_result.push_back(text_);

	return v_result;
}

HRESULT _trim    (string_t& _content)
{
	HRESULT hr_ = S_OK;

	if (_content.empty())
		hr_ = E_INVALIDARG;

	// trimming; perhaps using IfThenElse<> would be better;
	// https://stackoverflow.com/questions/216823/how-to-trim-an-stdstring ;
#ifdef _UNICODE
	_content.erase(_content.begin(), ::std::find_if_not(_content.begin(), _content.end(), [](TCHAR c){ return ::std::iswspace(c); }));  // left ;
	_content.erase(::std::find_if_not(_content.rbegin(), _content.rend(), [](TCHAR c){ return ::std::iswspace(c); }).base(), _content.end()); // right;
#else
	_content.erase(_content.begin(), ::std::find_if_not(_content.begin(), _content.end(), [](TCHAR c){ return ::std::isspace(c); }));  // left ;
	_content.erase(::std::find_if_not(_content.rbegin(), _content.rend(), [](TCHAR c){ return ::std::isspace(c); }).base(), _content.end()); // right;
#endif

	return  hr_;
}

}}

/////////////////////////////////////////////////////////////////////////////

CResLoader:: CResLoader (void) { m_error << __MODULE__ << NO_ERROR; }
CResLoader::~CResLoader (void) {}

/////////////////////////////////////////////////////////////////////////////

TSysErrRef CResLoader::Error (void) const { return this->m_error; }

string_t   CResLoader::String(WORD _w_res_id) const {

	m_error << __MODULE__ << S_OK;

	// https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-loadstringa
	TCHAR sz_buffer[_MAX_PATH] = {0};

	if (0 == ::LoadString(::GetModuleHandle(nullptr), _w_res_id, sz_buffer, _countof(sz_buffer)))
	{
		this->m_error << ERROR_RESOURCE_NAME_NOT_FOUND;
		return string_t();
	}

	if (0 == ::_tcslen(sz_buffer)) {
		this->m_error << ERROR_RESOURCE_NOT_PRESENT;
		return string_t();
	}

	return string_t(sz_buffer);
}