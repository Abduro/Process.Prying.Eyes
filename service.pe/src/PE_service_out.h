#ifndef _PESERVICEOUT_H_35904E31_8907_41C2_988F_A8954B10D37C_INCLUDED
#define _PESERVICEOUT_H_35904E31_8907_41C2_988F_A8954B10D37C_INCLUDED
/*
	Created by Tech_dog (ebontrop@gmail.com) on 25-Feb-2023 at 19:43:49.2848040, UTC+7, Novosibirsk, Saturday;
	This is Process Prying Eyes project fake service output console interface declaration file;
*/
#include "_out.con.h"
#include "_out.record.h"
#include "_out.section.h"

#include "proc.perf.reg.h"
#include "log.csv.h"

namespace service { namespace pe {

	using _output::CConsole ;
	using _output::CRec_Err ;
	using _output::CRec_Info;
	using _output::CRec_Warn;
	using _output::CSection ;

	class CFake_Out : public CConsole {
	                 typedef CConsole TBase;
	public:
		 CFake_Out (void);
		~CFake_Out (void);
	};

	using shared::sys_core::string_t;

	using namespace shared::log;
	using namespace proc::perform::reg;

	class CSV_Header {
	public:
		 CSV_Header (void);
		~CSV_Header (void);

	public:
		string_t To_string(TSeparator) const; // queries names of performence counters;
	};

	class CSV {
	public:
		 CSV (void);
		~CSV (void);

	public:
		const
		CSV_Header& Header(void) const;

	private:
		CSV_Header m_header;
	};

}}

typedef service::pe::CFake_Out out_;

static out_ _out;

#endif/*_PESERVICEOUT_H_35904E31_8907_41C2_988F_A8954B10D37C_INCLUDED*/