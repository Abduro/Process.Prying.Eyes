/*
	Created by Tech_dog (ebontrop@gmail.com) on 25-Feb-2023 at 19:51:14.7526755, UTC+7, Novosibirsk, Saturday;
	This is Process Prying Eyes project fake service output console interface implementation file;
*/
#include "pe_service_out.h"

using namespace service::pe;

/////////////////////////////////////////////////////////////////////////////

CFake_Out:: CFake_Out (void) {}
CFake_Out::~CFake_Out (void) {}

/////////////////////////////////////////////////////////////////////////////

namespace service { namespace pe { namespace _impl {

}}}
using namespace service::pe::_impl;
/////////////////////////////////////////////////////////////////////////////

CSV_Header:: CSV_Header (void) {}
CSV_Header::~CSV_Header (void) {}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

string_t CSV_Header::To_string(TSeparator _sep) const { _sep;

	const
	::std::vector<string_t>& counters = CPerf_Pdh().Instance().Get_counters(); counters;

	string_t cs_header;
	return cs_header;
}

/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////

CSV:: CSV (void) {}
CSV::~CSV (void) {}

/////////////////////////////////////////////////////////////////////////////
const
CSV_Header& CSV::Header(void) const { return this->m_header; }

/////////////////////////////////////////////////////////////////////////////