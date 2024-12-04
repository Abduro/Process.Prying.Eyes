/*
	Created by Tech_dog (ebontrop@gmail.com) on 18-Mar-2023 at 07:22:06.5642006, UTC+7, Novosibirsk, Saturday;
	This is Process Prying Eyes project GPU consumption detect interface implementation file;
*/
#include "proc.perf.gpu.h"

using namespace proc::perform::gpu;

/////////////////////////////////////////////////////////////////////////////

namespace proc { namespace perform { namespace gpu { namespace _impl {

	class CQuery {
	public:
		 CQuery (void) : m_query(nullptr) {}
		~CQuery (void) { this->Close(); }

	public:
		PDH_STATUS Close (void) {

			PDH_STATUS l_status = S_OK;
			// https://learn.microsoft.com/en-us/windows/win32/api/pdh/nf-pdh-pdhclosequery ;
			if (this->Is_valid()) {
				l_status = ::PdhCloseQuery(this->m_query); this->m_query = nullptr; // close return code is not checked for error;
			}
			return l_status;
		}
		bool  Is_valid (void) const { return (nullptr !=  this->m_query); }

	public:
		PDH_HQUERY  Get (void) const { return  this->m_query; }
		const
		PDH_HQUERY& Ref (void) const { return  this->m_query; }
		PDH_HQUERY& Ref (void)       { return  this->m_query; }
		const
		PDH_HQUERY* Ptr (void) const { return &this->m_query; }
		PDH_HQUERY* Ptr (void)       { return &this->m_query; }

	public:
		operator PDH_HQUERY* (void) { return &m_query; }
		operator PDH_HQUERY  (void) const { return this->m_query; }

	public:
		CQuery&  operator << (const HQUERY _qry) { this->Close(); this->m_query = _qry; return *this; }

	private:
		HQUERY m_query;
	};

}}}}
using namespace proc::perform::gpu::_impl;
/////////////////////////////////////////////////////////////////////////////

CPerf_Gpu:: CPerf_Gpu (void) { m_error << __MODULE__ << S_OK; }
CPerf_Gpu::~CPerf_Gpu (void) {}

/////////////////////////////////////////////////////////////////////////////

TSysErrRef  CPerf_Gpu::Error (void) const { return this->m_error; }

DWORD       CPerf_Gpu::Get   (void) {
	m_error << __MODULE__ << S_OK;

	if (m_time.empty() == false)
		m_time.clear();

	static LPCTSTR lp_sz_source = _T("GPU Engine");

	DWORD dw_cnt_size = 0;
	DWORD dw_inst_size = 0;

	PDH_STATUS l_status = ::PdhEnumObjectItems(
		nullptr, nullptr, lp_sz_source, nullptr, &dw_cnt_size, nullptr, &dw_inst_size, PERF_DETAIL_WIZARD, 0
	);

	if (PDH_MORE_DATA != l_status)
		return (m_error << l_status);

	::std::vector<TCHAR> v_counters(dw_cnt_size);
	::std::vector<TCHAR> v_instances(dw_inst_size);

	l_status = ::PdhEnumObjectItems(
		nullptr, nullptr, lp_sz_source, v_counters.data(), &dw_cnt_size, v_instances.data(), &dw_inst_size, PERF_DETAIL_WIZARD, 0
	);

	if (S_OK != l_status)
		return (m_error << l_status);

	for (PTCHAR p_iter = v_instances.data(); 0 != *p_iter; p_iter += ::_tcslen(p_iter) + 1) {

		if (0 == ::_tcsstr(p_iter, _T("engtype_3D")))
			continue;
		// https://learn.microsoft.com/en-us/windows/win32/api/pdh/nf-pdh-pdhopenquerya ;
		CQuery query;
		l_status = ::PdhOpenQuery(NULL, 0, query.Ptr());

		if (S_OK != l_status)
			continue;

		string_t cs_query = CFormatter().Accept(_T("\\GPU Engine(%s)\\Running time"), p_iter);
		// https://learn.microsoft.com/en-us/windows/win32/api/pdh/nf-pdh-pdhaddcountera ;
		HCOUNTER h_counter = nullptr;
		l_status = ::PdhAddCounter(query.Get(), cs_query.c_str(), 0, &h_counter);

		if (S_OK != l_status)
			continue;
		// https://learn.microsoft.com/en-us/windows/win32/api/pdh/nf-pdh-pdhcollectquerydata ;
		l_status = ::PdhCollectQueryData(query.Get()); if (S_OK != l_status) continue;
		l_status = ::PdhCollectQueryData(query.Get()); if (S_OK != l_status) continue;

		PDH_FMT_COUNTERVALUE l_value = {0};

		// https://learn.microsoft.com/en-us/windows/win32/api/pdh/nf-pdh-pdhgetformattedcountervalue ;
		l_status = ::PdhGetFormattedCounterValue(h_counter, PDH_FMT_LONG, nullptr, &l_value);
		if (S_OK != l_status)
			continue;

		if (1 > l_value.longValue)
			continue;

		regex_t  re(_T("pid_(\\d+)"));
		smatch_t sm;
		string_t str(p_iter);

		if (::std::regex_search(str, sm, re)) {

			DWORD pid = ::std::stol(sm[1]);
			m_time.insert(::std::make_pair(pid, l_value.longValue));

		}

	}

	return m_error;
}

int64_t     CPerf_Gpu::Total (void) {

	int64_t n_total = 0;

	this->Get();

	for (TGpuTime::const_iterator it_(this->m_time.begin()); it_ != this->m_time.end(); ++it_) {
		if (it_->second > 0)
			n_total += it_->second;
	}

	return  n_total;
}

double      CPerf_Gpu::Usage (void) {

	static ::std::chrono::steady_clock::time_point prev_called = ::std::chrono::steady_clock::now();
	static int64_t prev_running_time = this->Total();

	::std::chrono::steady_clock::time_point now = ::std::chrono::steady_clock::now();
	::std::chrono::steady_clock::duration elapsed = now - prev_called;

	int64_t elapsed_sec  = ::std::chrono::duration_cast<std::chrono::nanoseconds>(elapsed).count();
	int64_t running_time = this->Total();

	double percentage = 0.0;

	if (0 != elapsed_sec) {
		percentage = (double)(running_time - prev_running_time) / elapsed_sec * 100;
	}

	prev_called = now;
	prev_running_time = running_time;

	if (percentage > 1.0)
		percentage = 1.0;
	else if (percentage < 0.0)
		percentage = 0.0;

	return percentage;
}

/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////