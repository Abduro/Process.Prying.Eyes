/*
	Created by Tech_dog (ebontrop@gmail.com) on 27-Feb-2023 at 12:13:57.5818053, UTC+7, Novosibirsk, Monday;
	This is Process Prying Eyes project system process interface implementation file;
*/
#include "process.h"

using namespace shared::process;

/////////////////////////////////////////////////////////////////////////////

namespace shared { namespace process { namespace _impl {

}}}
using namespace shared::process::_impl;
/////////////////////////////////////////////////////////////////////////////

CProcess:: CProcess (void) : m_id(0) {}
CProcess:: CProcess (const CProcess& _ref) : CProcess() { *this = _ref; }
CProcess::~CProcess (void) {}

/////////////////////////////////////////////////////////////////////////////
const
TCertInfo& CProcess::Cert (void) const { return this->m_cert; }
DWORD      CProcess::Id   (void) const { return this->m_id; }
bool       CProcess::Is   (void) const { return this->Id() > 0 /*&& CGenericPath(this->Path()).Exists()*/; }
LPCTSTR    CProcess::Name (void) const { return this->m_name.c_str(); }
LPCTSTR    CProcess::Path (void) const { return this->m_path.c_str(); }

/////////////////////////////////////////////////////////////////////////////

const
TModules&  CProcess::Modules (void) const { return this->m_dlls; }
TModules&  CProcess::Modules (void)       { return this->m_dlls; }

/////////////////////////////////////////////////////////////////////////////

CProcess&  CProcess::operator = (const CProcess& _ref) { *this << _ref.Id() << _ref.Name() >> _ref.Path() << _ref.Cert() << _ref.Modules(); return *this; }
CProcess&  CProcess::operator <<(const TCertInfo& _cert) { this->m_cert = _cert; return *this; }
CProcess&  CProcess::operator <<(DWORD _id) { this->m_id = _id; return *this; }
CProcess&  CProcess::operator <<(LPCTSTR _name) { this->m_name = _name; return *this; }
CProcess&  CProcess::operator >>(LPCTSTR _path) { this->m_path = _path; return *this; }

CProcess&  CProcess::operator <<(const TModules& _dlls) { this->Modules() = _dlls; return *this; }

/////////////////////////////////////////////////////////////////////////////

CProcess_List:: CProcess_List (void) { m_error << __MODULE__ << S_OK; }
CProcess_List:: CProcess_List (const CProcess_List& _ref) : CProcess_List() { *this = _ref; }
CProcess_List::~CProcess_List (void) {}

/////////////////////////////////////////////////////////////////////////////

TSysErrRef CProcess_List::Error (void) const { return this->m_error; }
DWORD      CProcess_List::Get_It(void) {
	m_error << __MODULE__ << S_OK;

	if (m_procs.empty() == false)
		m_procs.clear();

#define dw_how_many_procs (1024)

	DWORD dw_proc_ids[dw_how_many_procs] = {0};
	DWORD dw_proc_req = 0;
	DWORD dw_proc_got = 0;

	// https://learn.microsoft.com/en-us/windows/win32/psapi/enumerating-all-processes ;
	// https://learn.microsoft.com/en-us/windows/win32/api/psapi/nf-psapi-enumprocesses;
	if (false == !!::EnumProcesses(dw_proc_ids, sizeof(dw_proc_ids), &dw_proc_req))
		return (this->m_error << ::GetLastError());
#if (0)
	// gets a list of all available devices ;
	// https://learn.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-querydosdevicew ;
	DWORD d_size = 1024;
	TRawData raw_data;
	while (true)
	{
		if (S_OK != raw_data.Append(nullptr, d_size * sizeof(TCHAR))) {
			this->m_error = raw_data.Error(); break;
		}
		DWORD d_copied = ::QueryDosDevice(nullptr, (LPTSTR)raw_data.GetData(), raw_data.GetSize() / sizeof(TCHAR));
		if ( d_copied == 0) {
			m_error.Last();
			if (ERROR_INSUFFICIENT_BUFFER != m_error.Get())
				break;
			d_size *= 2;
		}
		else
			break;
	}
	if (false == (bool)m_error) {
		::std::vector<string_t> volumes = shared::sys_core::_split((LPTSTR)raw_data.GetData(), _T('\0'));
	}
#endif
	dw_proc_got = dw_proc_req / sizeof (DWORD);

	for (DWORD i_ = 0; i_ < dw_proc_got; i_++) {
		if (0 != dw_proc_ids[i_]) {
			// https://learn.microsoft.com/en-us/windows/win32/api/processthreadsapi/nf-processthreadsapi-openprocess ;
			CAutoHandle h_proc; h_proc.Attach(::OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, dw_proc_ids[i_]));
			if (false == h_proc.Is()) {
				m_error.Last();
				continue;
			}

			HMODULE h_module = nullptr;
			DWORD dw_require = 0; // *attention*: value in bytes ;
			// https://learn.microsoft.com/en-us/windows/win32/api/psapi/nf-psapi-enumprocessmodules ; this function is for 32-bit app;
			// https://learn.microsoft.com/en-us/windows/win32/api/psapi/nf-psapi-enumprocessmodulesex ; this function is for 64-bit app;
			if (false == !!::EnumProcessModulesEx(h_proc, &h_module, sizeof(h_module), &dw_require, 0))
				continue;
#if (0)
			dw_require /= sizeof(HMODULE);

			HMODULE* p_modules = nullptr;
			try {
				p_modules = new HMODULE[dw_require];
			}
			catch (const ::std::bad_alloc&)
			{ continue; }

			if (false == !!::EnumProcessModules(h_proc, p_modules, dw_require, &dw_require))
			{
				delete p_modules; p_modules = nullptr;
				continue;
			}
#endif
			::std::vector<string_t> v_mod_paths;
#if (0)
			for (size_t j_ = 0; j_ < dw_require / sizeof(HMODULE); j_++) {

				TCHAR mod_path[_MAX_PATH] = {0};
				DWORD d_qry_size = _countof(mod_path);
				// https://learn.microsoft.com/en-us/windows/win32/api/libloaderapi/nf-libloaderapi-getmodulefilenamew ;
				if (false == !!::GetModuleFileNameEx(h_proc, p_modules[j_], mod_path, _countof(mod_path)) &&
					// https://learn.microsoft.com/en-us/windows/win32/api/winbase/nf-winbase-queryfullprocessimagenamea
					false == !!::QueryFullProcessImageName(h_proc, 0, mod_path, &d_qry_size)) {
						m_error.Last();
						continue;
				}
				else {
					v_mod_paths.push_back(string_t(mod_path));
				}
				// suspended processes;
				// https://devblogs.microsoft.com/oldnewthing/20150716-00/?p=45131#comments ; great article written by Raymond Chen;
			}
			delete p_modules;
#else
			TCHAR mod_path[_MAX_PATH] = {0};
			DWORD d_qry_size = _countof(mod_path);

			if (false == !!::QueryFullProcessImageName(h_proc, 0, mod_path, &d_qry_size)) {
				m_error.Last();
				continue;
			}
			else {
				v_mod_paths.push_back(string_t(mod_path));
			}
#endif
			TCHAR proc_path[_MAX_PATH] = {0};
			// https://learn.microsoft.com/en-us/windows/win32/api/psapi/nf-psapi-getprocessimagefilenamew  ;
			if (false == !!::GetProcessImageFileName(h_proc, proc_path, _countof(proc_path))) {
				m_error.Last();
				continue;
			}

			// https://learn.microsoft.com/en-us/windows/win32/api/psapi/nf-psapi-getmodulebasenamew ;
			TCHAR proc_name[_MAX_PATH] = {0};
			if (false == !!::GetModuleBaseName(h_proc, h_module, proc_name, _countof(proc_name)))
				continue;

			TCertProvider cert_prov;

			if (false == cert_prov.Is_Trusted(v_mod_paths[0].c_str()))
				m_error = cert_prov.Error();

			CProcess proc;
			proc << dw_proc_ids[i_] << proc_name >> (v_mod_paths.empty() ? proc_path :  v_mod_paths[0].c_str()) << cert_prov.Get() << v_mod_paths;

			m_procs.insert(::std::make_pair(dw_proc_ids[i_], proc));
		};
	}
#if (0)
	proc::perform::reg::CPerf_Pdh counters;
	counters.Enumerate();
#else
	proc::perform::reg::CPerformance perf;
	perf.Get();
#endif
	return m_error;
}

/////////////////////////////////////////////////////////////////////////////

string_t   CProcess_List::Print (void) const {

	string_t s_result;

	s_result += _T("\n\tId\tName\tPath");
	for (TProc_map::const_iterator it_ = m_procs.begin(); it_ != this->m_procs.end(); ++it_) {

		const DWORD proc_id  = it_->first ;
		const CProcess& proc = it_->second;

		s_result += CFormatter().Accept(_T("\n\t%u\t%s\t%s"), proc_id, proc.Name(), proc.Path());

		const TModules& dlls = proc.Modules();
		if (dlls.empty())
			continue;
#if (0)
		s_result += _T("\n\t\tModules");

		for (size_t j_ = 0; j_ < dlls.size(); j_++) {
			s_result += CFormatter().Accept(_T("\n\t\t%s"), dlls[j_].c_str());
		}
#endif
	}
	return   s_result;
}

/////////////////////////////////////////////////////////////////////////////

CProcess_List&  CProcess_List::operator = (const CProcess_List& _ref) { this->m_procs = _ref.m_procs; return *this; }