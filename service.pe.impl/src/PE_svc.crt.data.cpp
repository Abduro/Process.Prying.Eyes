/*
	Created by Tech_dog (ebontrop@gmail.com) on 14-Feb-2023 at 11:57:54.4946761, UTC+7, Novosibirsk, Tuesday;
	This is Process Prying Eyes project service shared create data interface implementation file;
*/
#include "PE_svc.crt.data.h"

using namespace service::pe;

/////////////////////////////////////////////////////////////////////////////

namespace service { namespace pe { namespace _impl {
	// this is just simplification of getting names of the service, just making an illusion of object oriented programming;
	class CTemplate {
	public:
		 CTemplate (void) {}
		~CTemplate (void) {}

	public:
		LPCTSTR  Name (void) const { static LPCTSTR lp_sz_name =_T("PE_service");  return lp_sz_name; }
		
	};

}}}
using namespace service::pe::_impl;
/////////////////////////////////////////////////////////////////////////////

CIniter:: CIniter (void) { m_error << __MODULE__ << S_OK; }
CIniter::~CIniter (void) {}

/////////////////////////////////////////////////////////////////////////////

//
// ***important***:
//               1) a service executable bitness must be the same as a bitness of a client executable;
//               2) the name of a service must be consistent with service executable file name;
//
//               otherwise, system error 'The system cannot find the file specified.' will be thrown during a creation of a service;
//

DWORD   CIniter::Do (CCrtData& _crt_data) {
	m_error << __MODULE__ << S_OK;

	// current directory path is not often the same as a path of executable is being run, thus ::GetCurrentDirectory() shouldn't be used;
	// calling ::GetModuleFileName() is much more applicable;

	TCHAR sz_buffer[_MAX_PATH] = {0};
	// https://learn.microsoft.com/en-us/windows/win32/api/libloaderapi/nf-libloaderapi-getmodulefilenamea ;
	// TODO: in case of failure, the last error must be checked for ERROR_INSUFFICIENT_BUFFER error code;
	if (!::GetModuleFileName(nullptr, sz_buffer, _countof(sz_buffer)))
	{
		return (m_error << ::GetLastError());
	}
	string_t sz_path(sz_buffer);
	sz_path.substr(0, string_t(sz_buffer).find_last_of(_T("\\/"))); // without file name;

	string_t sz_exe = CFormatter().Accept(_T("%s.exe"), CTemplate().Name());  // gets service executable file name;
	string_t sz_svc = sz_path + sz_exe; // gets a full path to service executable;
		
	_crt_data.Properties().Account() << _T("NT AUTHORITY\\LocalSystem");
	_crt_data.Properties().Desc()    << _T("Prying Eyes service collects an information of processes' activities;");
	_crt_data.Properties().Title()   << _T("Process Prying Eyes;");
	_crt_data.Properties().Name()    << sz_exe;
	_crt_data.Properties().Path()    << sz_svc;
	_crt_data.Options().Ctrl()       <<(CCtrlOption::eStoppable | CCtrlOption::eCanShutdown);

	return m_error;
}

TSysErrRef CIniter::Error (void) const { return this->m_error; }