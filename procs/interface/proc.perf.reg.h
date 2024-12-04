#ifndef _PROCPERFREG_H_B5A78BB0_F769_4B4D_AF2A_33F9478DF5C6_INCLUDED
#define _PROCPERFREG_H_B5A78BB0_F769_4B4D_AF2A_33F9478DF5C6_INCLUDED
/*
	Created by Tech_dog (ebontrop@gmail.com) on 12-Mar-2023 at 04:43:10.5660551, UTC+7, Novosibirsk, Sunday;
	This is Process Prying Eyes project performance counter (in registry) interface declaration file;
*/
#include <pdh.h>
#include <pdhmsg.h>

#pragma comment(lib, "pdh.lib")

#include "shared.classes.h"
#include "shared.sys.err.h"
#include "shared.raw.buf.h"

namespace proc { namespace perform { namespace reg {

	using shared::sys_core::CSysError;
	using shared::sys_core::CFormatter;
	using shared::sys_core::string_t;

	class CPerf_Pdh {

	public:
		 CPerf_Pdh (void);
		~CPerf_Pdh (void);

	public:
		TSysErrRef Error (void) const;

		DWORD  Get (void);  // tries to enumerate all available counters for currently running processes;

	public:
		string_t Get_counter_name (size_t _ndx) const;
		string_t Get_process_name (size_t _ndx) const;

		const ::std::vector<string_t>& Get_counters (void) const; // for log file performence counter names;

	public:
		const CPerf_Pdh& Instance (void);

	private:
		CSysError  m_error;
		::std::vector<string_t> v_counters, v_instances;
	};

	// https://learn.microsoft.com/en-us/windows/win32/perfctrs/performance-data-format ;
	class CPerformance { // this class is for getting process counters from *virtual* registry by using HKEY_PERFORMANCE_DATA key;
	public:
		 CPerformance (void);
		~CPerformance (void);

	public:
		TSysErrRef Error (void) const;

		DWORD  Get (void);

	private:
		CSysError  m_error;
	};

}}}

#endif/*_PROCPERFREG_H_B5A78BB0_F769_4B4D_AF2A_33F9478DF5C6_INCLUDED*/