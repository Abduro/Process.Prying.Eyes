#ifndef _PROCESS_H_92BE19C2_322A_40EB_8CAC_3621CD8E3D35_INCLUDED
#define _PROCESS_H_92BE19C2_322A_40EB_8CAC_3621CD8E3D35_INCLUDED
/*
	Created by Tech_dog (ebontrop@gmail.com) on 27-Feb-2023 at 12:04:15.5064037, UTC+7, Novosibirsk, Monday;
	This is Process Prying Eyes project system process interface declaration file;
*/
#include "shared.classes.h"
#include "shared.sys.err.h"
#include "shared.gen.path.h"
#include "shared.gen.hand.h"
#include "shared.cert.h"

#include <psapi.h>
#pragma comment (lib, "psapi.lib")
#pragma comment (lib, "shlwapi.lib")

#include "proc.perf.gpu.h"
#include "proc.perf.reg.h"

namespace shared { namespace process {

	using shared::sys_core::CGenericPath;
	using shared::sys_core::CSysError ;
	using shared::sys_core::string_t  ;
	using shared::sys_core::CFormatter;

	using shared::common::CAutoHandle ;

	typedef ::std::vector<string_t> TModules; // process module list;

	class CProcess { // this class does not control a process life-cycle, thus its data can be copied;
	public:
		 CProcess (void) ;
		 CProcess (const CProcess&) ;
		~CProcess (void) ;

	public:
		const
		TCertInfo&Cert (void) const;
		DWORD     Id   (void) const ;
		bool      Is   (void) const ;   // is true when ID and path are valid, i.e. ID is not zero and process path is not empty;
		LPCTSTR   Name (void) const ;   // this is just executable name;
		LPCTSTR   Path (void) const ;   // this is full path to executable binary file;

		const
		TModules& Modules (void) const;
		TModules& Modules (void)      ;

	public:
		CProcess& operator = (const CProcess&);
		CProcess& operator <<(const TCertInfo&);
		CProcess& operator <<(DWORD _dw_id ); // no error check is performed when this attribute is set to given value;
		CProcess& operator <<(LPCTSTR _name); // no error check is performed when this attribute is set to given value;
		CProcess& operator >>(LPCTSTR _path); // no error check is performed when this attribute is set to given value;

		CProcess& operator <<(const TModules&); // input data does not have a lot of modules/dlls for one process;

	private:
		DWORD     m_id  ;
		string_t  m_name;
		string_t  m_args;
		string_t  m_path;
		TCertInfo m_cert;
		TModules  m_dlls; // is not used yet;
	};

	typedef ::std::map<DWORD, CProcess> TProc_map; // a key is process ID, a value - a process instance;

	class CProcess_List {
	public:
		 CProcess_List (void);
		 CProcess_List (const CProcess_List&);
		~CProcess_List (void);

	public:
		TSysErrRef Error (void) const;
		DWORD      Get_It(void); // synch method yet;

	public:
		string_t   Print (void) const;

	public:
		CProcess_List& operator = (const CProcess_List&);
	private:
		TProc_map  m_procs;
		CSysError  m_error;
	};

}}

#endif/*_PROCESS_H_92BE19C2_322A_40EB_8CAC_3621CD8E3D35_INCLUDED*/