/*
	Created by Tech_dog (ebontrop@gmail.com) on 10-Feb-2019 at 6:52:05a, UTC+7, Novosibirsk, Sunday;
	This is FIX Engine shared library configuration command line interface implementation file.
	-----------------------------------------------------------------------------
	Adopted to Process Prying Eyes  project on 14-Feb-2023 at 03:55:41.3920956, UTC+7, Novosibirsk, Tuesday;
*/
#include "shared.cmd.ln.h"

using namespace shared::input;

/////////////////////////////////////////////////////////////////////////////

namespace shared { namespace input { namespace _impl {

	class CResLoader {
	public:
		 CResLoader (void) { m_error << __MODULE__ << NO_ERROR; }
		~CResLoader (void) {}

	public:
		TSysErrRef Error (void) const { return this->m_error; }

		string_t   Load  (WORD _w_res_id) const {

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

	private:
		mutable
		CSysError m_error;
	};

}}}
using namespace shared::input::_impl;
/////////////////////////////////////////////////////////////////////////////

CCommandLine:: CCommandLine(void)
{
	string_t cs_cmd_line = ::GetCommandLine();
	string_t cs_key;
	string_t cs_arg;

	INT n_count = 0;
	bool bKey   = false;
	// https://learn.microsoft.com/en-us/windows/win32/api/shellapi/nf-shellapi-commandlinetoargvw ;
	LPWSTR* pCmdArgs = ::CommandLineToArgvW(cs_cmd_line.c_str(), &n_count);
	if (0 == n_count || nullptr == pCmdArgs)
		goto __end_of_story__;
	
	m_module_full_path = pCmdArgs[0];

	for (INT i_ = 1; i_ < n_count; i_+= 1) {
		string_t cs_val = pCmdArgs[i_];
		bKey = (/*0 == cs_val.Find(_T("-"))
		     || */0 == cs_val.find(_T("/")));

		if (bKey) {
			if (cs_key.empty() == false) // the previous key is not saved yet;
				m_args.insert(::std::make_pair(cs_key, cs_arg));

			cs_key = pCmdArgs[i_];
			shared::sys_core::_replace(cs_key, _T("-"), _T(""));
			shared::sys_core::_replace(cs_key, _T("/"), _T(""));
			cs_arg = _T("");
		}
		else {
			cs_arg+= pCmdArgs[i_];
		}

		const bool bLast = (i_ == n_count - 1);
		if (bLast && cs_key.empty() == false) {
			try {
				m_args.insert(::std::make_pair(cs_key, cs_arg));
			} catch (const ::std::bad_alloc&){}
		}
	}

__end_of_story__:
	if (nullptr != pCmdArgs) {
		::LocalFree(pCmdArgs); pCmdArgs = nullptr;
	}
}

CCommandLine::~CCommandLine(void) { this->Clear(); }

/////////////////////////////////////////////////////////////////////////////

HRESULT       CCommandLine::Append(LPCTSTR _lp_sz_nm, LPCTSTR _lp_sz_val)
{
	HRESULT hr_ = S_OK;
	try {
		m_args.insert(::std::make_pair(
			string_t(_lp_sz_nm), string_t(_lp_sz_val)
		));
	}
	catch(const ::std::bad_alloc&)
	{
		hr_ = E_OUTOFMEMORY;
	}
	return hr_;
}

string_t      CCommandLine::Arg   (LPCTSTR _lp_sz_nm)const
{
	TCmdLineArgs::const_iterator it_ = m_args.find(string_t(_lp_sz_nm));
	if (it_ == m_args.end())
		return string_t();
	else
		return it_->second;
}

LONG          CCommandLine::Arg   (LPCTSTR _lp_sz_nm, const LONG _def_val)const
{
	TCmdLineArgs::const_iterator it_ = m_args.find(string_t(_lp_sz_nm));
	if (it_ == m_args.end())
		return _def_val;
	else	// https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/atol-atol-l-wtol-wtol-l ;
		return ::_ttol(it_->second.c_str());
}

TCmdLineArgs  CCommandLine::Args  (void) const { return m_args; }
VOID          CCommandLine::Clear (void)       { if (m_args.empty() == false) m_args.clear(); }
INT           CCommandLine::Count (void)const  { return static_cast<INT>(m_args.size()); }
bool          CCommandLine::Has   (LPCTSTR pArgName) const
{
	TCmdLineArgs::const_iterator it__ = m_args.find(string_t(pArgName));
	return (it__ != m_args.end());
}

string_t      CCommandLine::ModuleFullPath(void) const { return m_module_full_path; }

string_t      CCommandLine::ToString(LPCTSTR _lp_sz_sep) const
{
	LPCTSTR  lp_sz_pat = _T("%s=%s");
	string_t cs_pat ;
	string_t cs_args;

	if (m_args.empty())
		return (cs_args = _T("command line has no argument;"));

	for (TCmdLineArgs::const_iterator it_ = m_args.begin(); it_ != m_args.end(); ++it_)
	{
		cs_args += CFormatter().Accept(lp_sz_pat, it_->first.c_str(), it_->second.c_str());
		cs_args +=(nullptr == _lp_sz_sep ? _T("; ") : _lp_sz_sep);
	}
	return cs_args;
}

/////////////////////////////////////////////////////////////////////////////

CArgument:: CArgument (void) : m_type(0) {}
CArgument:: CArgument (const CArgument& _arg) : CArgument() { *this = _arg; }
CArgument:: CArgument (const WORD _w_res_name, const WORD _w_res_verb, const DWORD _dw_type) : CArgument() {
	this->Name(_w_res_name); 
	this->Verb(_w_res_verb);
	this->Type(_dw_type);
}
CArgument:: CArgument (LPCTSTR _lp_sz_name, LPCTSTR _lp_sz_verb, const DWORD _dw_type) : m_name(_lp_sz_name), m_verb(_lp_sz_verb), m_type(_dw_type) {}
CArgument::~CArgument (void) {}

/////////////////////////////////////////////////////////////////////////////

bool      CArgument::Is  (void) const { return (m_name.empty() == false); }
LPCTSTR   CArgument::Name(void) const { return  m_name.c_str(); }
HRESULT   CArgument::Name(const WORD _w_res_id) {
	if (0 == _w_res_id)
		return E_INVALIDARG;

	CResLoader loader_;
	m_name = loader_.Load(_w_res_id);

	return __DwordToHresult(loader_.Error());
}
HRESULT   CArgument::Name(LPCTSTR _lp_sz_name ) {
	// https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/strlen-wcslen-mbslen-mbslen-l-mbstrlen-mbstrlen-l ;
	if (nullptr == _lp_sz_name || 0 == ::_tcslen(_lp_sz_name))
		return E_INVALIDARG;

	m_name = _lp_sz_name;
	return S_OK;
}
DWORD     CArgument::Type(void) const { return m_type; }
bool      CArgument::Type(const DWORD _type) { const bool b_changed = m_type != _type; m_type = _type; return b_changed; }
LPCTSTR   CArgument::Verb(void) const { return m_verb.c_str(); }
HRESULT   CArgument::Verb(const WORD _w_res_id) {
	if (0 == _w_res_id)
		return E_INVALIDARG;

	CResLoader loader_;
	m_verb = loader_.Load(_w_res_id);

	return __DwordToHresult(loader_.Error());
}
HRESULT   CArgument::Verb(LPCTSTR _lp_sz_desc ) {
	if (nullptr == _lp_sz_desc || 0 == ::_tcslen(_lp_sz_desc))
		return E_INVALIDARG;

	m_verb = _lp_sz_desc;
	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////

CArgument& CArgument::operator = (const CArgument& _ref) {
	this->Name(_ref.Name());
	this->Verb(_ref.Verb());
	this->Type(_ref.Type());
	return *this;
}

/////////////////////////////////////////////////////////////////////////////

bool CArgument::operator == (const CArgument& _ref) const { return (this->Type() == _ref.Type()); }
bool CArgument::operator == (LPCTSTR _lp_sz_name) const {
	if (nullptr == _lp_sz_name || 0 == ::_tcslen(_lp_sz_name))
		return false;
	if (this->m_name.size() != ::_tcslen(_lp_sz_name))
		return false;
	const size_t t_len_to_comp = (::_tcslen(_lp_sz_name) > this->m_name.size() ? this->m_name.size() : ::_tcslen(_lp_sz_name));
	// https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/strnicmp-wcsnicmp-mbsnicmp-strnicmp-l-wcsnicmp-l-mbsnicmp-l ;
	return (0 == ::_tcsncicmp(_lp_sz_name, this->m_name.c_str(), t_len_to_comp));
}