#ifndef _PROCPERFGPU_H_8D73842D_E1DF_4672_8BFA_8A44B75E667C_INCLUDED
#define _PROCPERFGPU_H_8D73842D_E1DF_4672_8BFA_8A44B75E667C_INCLUDED
/*
	Created by Tech_dog (ebontrop@gmail.com) on 18-Mar-2023 at 06:58:44.7294553, UTC+7, Novosibirsk, Saturday;
	This is Process Prying Eyes project GPU consumption detect interface declaration file;
*/
#include <pdh.h>
#include <pdhmsg.h>

#pragma comment(lib, "pdh.lib")

#include <regex>
#include <chrono>

#include "shared.classes.h"
#include "shared.sys.err.h"
#include "shared.raw.buf.h"

namespace proc { namespace perform { namespace gpu {

	using shared::sys_core::CSysError;
	using shared::sys_core::CFormatter;
	using shared::sys_core::string_t;

	typedef ::std::basic_regex<TCHAR> regex_t;
	typedef ::std::match_results<TCHAR> cmatch_t;
	typedef ::std::match_results<string_t::const_iterator> smatch_t;

	typedef ::std::map<LONG, LONG>   TGpuTime;     // GPU usage time of running processes, nanoseconds;
	typedef ::std::map<LONG, DOUBLE> TGpuUsage;    // GPU usage time in percents;

	class CPerf_Gpu {
	public:
		 CPerf_Gpu (void);
		~CPerf_Gpu (void);

	public:
		TSysErrRef Error (void) const;
		DWORD      Get   (void)      ;  // tries to get GPU consumption (%) for all currently running processes;
		int64_t    Total (void)      ;
		double     Usage (void)      ;

		const
		TGpuUsage& Usage (void) const;

	private:
		CSysError  m_error;
		TGpuTime   m_time ;
		TGpuUsage  m_usage;
	};

}}}

#endif/*_PROCPERFGPU_H_8D73842D_E1DF_4672_8BFA_8A44B75E667C_INCLUDED*/