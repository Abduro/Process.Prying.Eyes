/*
	Created by Tech_dog (ebontrop@gmail.com) on 12-Mar-2023 at 05:01:16.7732228, UTC+7, Novosibirsk, Sunday;
	This is Process Prying Eyes project performance counter registry values interface implementation file;
*/
#include "proc.perf.reg.h"

using namespace proc::perform::reg;

/////////////////////////////////////////////////////////////////////////////

CPerf_Pdh:: CPerf_Pdh (void) { m_error << __MODULE__ << S_OK; }
CPerf_Pdh::~CPerf_Pdh (void) {}

/////////////////////////////////////////////////////////////////////////////

TSysErrRef  CPerf_Pdh::Error (void) const { return this->m_error; }

DWORD CPerf_Pdh::Get (void) { // https://learn.microsoft.com/en-us/windows/win32/perfctrs/enumerating-process-objects ;

	m_error << __MODULE__ << S_OK;

	static LPCTSTR lp_sz_object  = _T("Process");

	LPTSTR lp_sz_counter_lst_buffer  = nullptr;
	LPTSTR lp_sz_instance_lst_buffer = nullptr;
	DWORD  dw_counter_lst_size  = 0;
	DWORD  dw_instance_lst_size = 0;

	PDH_STATUS
	l_status = ::PdhEnumObjectItems(
				nullptr, // real-time source ;
				nullptr, // local machine ;
				lp_sz_object, lp_sz_counter_lst_buffer, &dw_counter_lst_size, lp_sz_instance_lst_buffer, &dw_instance_lst_size, PERF_DETAIL_WIZARD, 0
			);
	if (PDH_MORE_DATA != l_status)
		return (m_error << l_status);

	lp_sz_counter_lst_buffer  = (LPTSTR)::malloc(dw_counter_lst_size  * sizeof(TCHAR));
	lp_sz_instance_lst_buffer = (LPTSTR)::malloc(dw_instance_lst_size * sizeof(TCHAR));

	if (nullptr != lp_sz_counter_lst_buffer && nullptr != lp_sz_instance_lst_buffer) {

		l_status = ::PdhEnumObjectItems(
				nullptr, // real-time source ;
				nullptr, // local machine ;
				lp_sz_object, lp_sz_counter_lst_buffer, &dw_counter_lst_size, lp_sz_instance_lst_buffer, &dw_instance_lst_size, PERF_DETAIL_WIZARD, 0
			);
		if (S_OK == l_status) {

			LPTSTR lp_sz_buffer = nullptr;

			for (lp_sz_buffer = lp_sz_counter_lst_buffer ; *lp_sz_buffer != 0; lp_sz_buffer += ::_tcslen(lp_sz_buffer) + 1) {
				v_counters.push_back(string_t(lp_sz_buffer));
			}

			for (lp_sz_buffer = lp_sz_instance_lst_buffer; *lp_sz_buffer != 0; lp_sz_buffer += ::_tcslen(lp_sz_buffer) + 1) {
				v_instances.push_back(string_t(lp_sz_buffer));
			}

			if (false == v_counters.empty())
			{}
		}
		else
			m_error << l_status;
	}
	else
		m_error << E_OUTOFMEMORY;

	if (nullptr != lp_sz_counter_lst_buffer ) { ::free(lp_sz_counter_lst_buffer ); lp_sz_counter_lst_buffer  = nullptr; }
	if (nullptr != lp_sz_instance_lst_buffer) { ::free(lp_sz_instance_lst_buffer); lp_sz_instance_lst_buffer = nullptr; }

	return m_error;
}

/////////////////////////////////////////////////////////////////////////////

string_t CPerf_Pdh::Get_counter_name (size_t _ndx) const { if (_ndx < v_counters.size())  return this->v_counters[_ndx];  else return string_t(); }
string_t CPerf_Pdh::Get_process_name (size_t _ndx) const { if (_ndx < v_instances.size()) return this->v_instances[_ndx]; else return string_t(); }

const ::std::vector<string_t>& CPerf_Pdh::Get_counters (void) const { return this->v_counters; }

/////////////////////////////////////////////////////////////////////////////

static CPerf_Pdh _proc_perfs;

const CPerf_Pdh& CPerf_Pdh::Instance (void) {
	if (_proc_perfs.v_counters.empty())
		_proc_perfs.Get(); 
	return _proc_perfs;
}

/////////////////////////////////////////////////////////////////////////////

namespace proc { namespace perform { namespace reg { namespace _impl {

	class CString_Convertor {
	public:
		enum length {
			e_name_short = 256,
			e_name_long  = e_name_short * 2,
		};
	public:
		 CString_Convertor (void) { m_error << __MODULE__ << S_OK; }
		~CString_Convertor (void) {}

	public:
		TSysErrRef Error(void) const { return this->m_error; }
		DWORD MbcsToWide(DWORD dwCodePage, LPCSTR lpToConvert, DWORD dwToConvertLen, LPWSTR lpConverted) {
			m_error << __MODULE__ << S_OK;
			// https://learn.microsoft.com/en-us/windows/win32/api/stringapiset/nf-stringapiset-multibytetowidechar ;
			INT n_result = ::MultiByteToWideChar(dwCodePage, 0, lpToConvert, dwToConvertLen, lpConverted, dwToConvertLen);
			if (n_result == 0)
				m_error.Last();

			return m_error;
		}

	private:
		CSysError  m_error;
	};

	// https://learn.microsoft.com/en-us/windows/win32/perfctrs/displaying-object-instance-and-counter-names ;
	typedef PERF_COUNTER_DEFINITION  PERF_COUNTER_DEF;
	typedef PERF_INSTANCE_DEFINITION PERF_INST_DEF;

	class CCounter {
	public:
		 CCounter (void) : m_def{0}{}
		 CCounter (const CCounter& _ref) : CCounter() { *this = _ref; }
		 CCounter (const PERF_COUNTER_DEF& _def) : CCounter() { *this << _def; }
		~CCounter (void) {}

	public:
		const
		PERF_COUNTER_DEF& Get (void) const { return m_def; }
		PERF_COUNTER_DEF& Set (void)       { return m_def; }

		LPCTSTR Name (void) const { return m_name.c_str(); }
		DWORD   Name (LPCTSTR _value) { this->m_name = _value; return S_OK; }

	public:
		CCounter& operator = (const CCounter& _ref) { *this << _ref.Get(); this->Name(_ref.Name()); return *this; }
		CCounter& operator <<(const PERF_COUNTER_DEF& _def) { this->Set() = _def; return *this; }

	private:
		PERF_COUNTER_DEF m_def;
		string_t m_name;
	};

	typedef ::std::vector<CCounter> TCounters;

	class CCounters {
	public:
		 CCounters (void) { m_error << __MODULE__ << S_OK; }
		 CCounters (const CCounters& _ref) : CCounters() { *this  = _ref; }
		 CCounters (const TCounters& _raw) : CCounters() { *this << _raw; }
		~CCounters (void) {}

	public:
		TSysErrRef Error (void) const { return this->m_error; }
		const
		TCounters& Raw (void) const { return this->m_counters; }
		TCounters& Raw (void)       { return this->m_counters; }

		DWORD Set (LPBYTE pStart, DWORD dwNumberOfCounters) {
			m_error << __MODULE__ << S_OK;

			if (nullptr == pStart || 0 == dwNumberOfCounters)
				return (m_error << E_INVALIDARG);

			if (m_counters.empty() == false)
				m_counters.clear();

			for (DWORD i_ = 0; i_ < dwNumberOfCounters; i_++) {

				PERF_COUNTER_DEFINITION cnt_def = *(reinterpret_cast<PPERF_COUNTER_DEFINITION>(pStart));
				// https://learn.microsoft.com/en-us/windows/win32/wmisdk/base-counter-types ;
				if ((cnt_def.CounterType & PERF_COUNTER_BASE) != PERF_COUNTER_BASE) {

					CCounter counter(cnt_def); counter.Name(_proc_perfs.Get_counter_name(i_).c_str());

					m_counters.push_back(counter);

				}

				pStart += cnt_def.ByteLength;
			}

			return m_error;
		}

	public:
		CCounters& operator = (const CCounters& _ref) { *this << _ref.Raw(); return *this; }
		CCounters& operator <<(const TCounters& _raw) {  this->Raw() = _raw; return *this; }

	private:
		CSysError m_error;
		TCounters m_counters;
	};

	class CInstance {
	public:
		 CInstance (void) : m_inst{0}, m_ID(0){}
		 CInstance (const CInstance& _ref) : CInstance() { *this = _ref; }
		 CInstance (const PERF_INST_DEF& _inst) : CInstance() { *this << _inst; }
		~CInstance (void) {}

	public:
		const
		CCounters& Counters (void) const { return this->m_counters; }

		DWORD ID (void) const { return this->m_ID; }
		void  ID (DWORD _value) { this->m_ID = _value; }

		const
		PERF_INST_DEF& Get (void) const { return m_inst; }
		PERF_INST_DEF& Set (void)       { return m_inst; }

		LPCTSTR Name (void) const { return m_name.c_str(); }
		DWORD   Name (LPCTSTR _value) { this->m_name = _value; return S_OK; }

	public:
		CInstance& operator = (const CInstance& _ref) {
			*this << _ref.Get() << _ref.Counters(); this->Name(_ref.Name()); this->ID(_ref.ID()); return *this;
		}
		CInstance& operator <<(const PERF_INST_DEF& _inst) { this->Set() = _inst; return *this; }
		CInstance& operator <<(const CCounters& _counters) { m_counters  = _counters; return *this; }

	private:
		PERF_INST_DEF m_inst;
		string_t      m_name;
		CCounters     m_counters;
		DWORD         m_ID;
	};

	typedef ::std::vector<CInstance> TInstances;

	class CInstances {
	public:
		 CInstances (void) { m_error << __MODULE__ << S_OK; }
		 CInstances (const CInstances& _ref) : CInstances() { *this  = _ref; }
		 CInstances (const TInstances& _raw) : CInstances() { *this << _raw; }
		~CInstances (void) {}

	public:
		TSysErrRef Error (void) const { return this->m_error; }
		const
		TInstances& Raw (void) const { return this->m_instances; }
		TInstances& Raw (void)       { return this->m_instances; }

		DWORD Set (LPBYTE pStart, DWORD dwNumberOfInstances, const CCounters& _counters) {
			m_error << __MODULE__ << S_OK;

			if (nullptr == pStart || 0 == dwNumberOfInstances)
				return (m_error << E_INVALIDARG);

			if (m_instances.empty() == false)
				m_instances.clear();

			for (DWORD i_ = 0; i_ < dwNumberOfInstances; i_++) {

				PERF_INST_DEF inst_def = *(reinterpret_cast<PERF_INST_DEF*>(pStart));
				PPERF_COUNTER_BLOCK p_cnt_data = reinterpret_cast<PPERF_COUNTER_BLOCK>(pStart + inst_def.ByteLength);

				CInstance instance(inst_def); instance.Name(_proc_perfs.Get_process_name(i_).c_str());

				for (size_t j_ = 0; j_ < _counters.Raw().size(); j_++) {

					const CCounter& cnt_ = _counters.Raw()[j_];

					if (cnt_.Get().CounterNameTitleIndex == 784) {

						LPBYTE pData = (LPBYTE)p_cnt_data + cnt_.Get().CounterOffset;
						DWORD* pID   = (DWORD*)pData;
						instance.ID(*pID);
					}
				}

				m_instances.push_back(instance);

				pStart += inst_def.ByteLength + p_cnt_data->ByteLength;
			}

			return m_error;
		}

	public:
		CInstances& operator = (const CInstances& _ref) { *this << _ref.Raw(); return *this; }
		CInstances& operator <<(const TInstances& _raw) {  this->Raw() = _raw; return *this; }

	private:
		CSysError  m_error;
		TInstances m_instances;
	};

	class CTypeObject {
	public:
		 CTypeObject (void) : m_type{0}{ m_error << __MODULE__ << S_OK; }
		 CTypeObject (const CTypeObject& _ref) : CTypeObject() { *this = _ref; }
		~CTypeObject (void) {}

	public:
		const
		CCounters& Counters (void) const { return this->m_counters; }

		TSysErrRef Error (void) const { return this->m_error; }
		const
		PERF_OBJECT_TYPE& Get (void) const { return m_type; }
		DWORD  Set (LPBYTE pStart, const PERF_OBJECT_TYPE& _type) {
			m_error << __MODULE__ << S_OK;

			this->m_type = _type;

			if (S_OK != this->m_counters.Set(pStart + _type.HeaderLength, _type.NumCounters))
				return (m_error = m_counters.Error());

			if (_type.NumInstances == 0 || _type.NumInstances == PERF_NO_INSTANCES) // type objects that have instances are considered this time;
				return (m_error);

			if (S_OK != this->m_instances.Set(pStart + _type.DefinitionLength, _type.NumInstances, this->Counters()))
				return (m_error = m_instances.Error());
			
			return m_error;
		}

		const
		CInstances& Instances (void) const { return this->m_instances; }

	public:
		CTypeObject& operator = (const CTypeObject& _ref) {
			this->m_type = _ref.Get();  this->m_counters = _ref.Counters(); this->m_instances = _ref.m_instances; return *this;
		}

	private:
		PERF_OBJECT_TYPE 
		           m_type ;
		CSysError  m_error;
		CCounters  m_counters ;
		CInstances m_instances;
	};

	typedef ::std::vector<CTypeObject> TTypeObjects;

	class CTypeObjects {

	public:
		 CTypeObjects (void) { m_error << __MODULE__ << S_OK; }
		~CTypeObjects (void) {}

	public:
		TSysErrRef   Error (void) const { return this->m_error; }
		const
		CTypeObject* Find  (DWORD dwNameTitleIndex) const { // TODO: pointer must be replaced by reference and cache; invalid object is accessible;

			const
			CTypeObject* pFound = nullptr;

			for (TTypeObjects::const_iterator it_(m_objects.begin()); it_ != m_objects.end(); ++it_) {

				if (it_->Get().ObjectNameTitleIndex == dwNameTitleIndex) {
					// https://stackoverflow.com/questions/641864/returning-a-pointer-to-a-vector-element-in-c ;
					pFound = &(*it_); break;

				}

			}

			return pFound;
		}

		DWORD Set (LPBYTE pStart, DWORD dwNumberOfObjects, LPBYTE pEnd) {
			m_error << __MODULE__ << S_OK;

			if (nullptr == pStart || nullptr == pEnd || 0 == dwNumberOfObjects)
				return (m_error << E_INVALIDARG);

			if (this->m_objects.empty() == false)
				this->m_objects.clear();

			for (DWORD i_ = 0; i_ < dwNumberOfObjects; i_++) {

				PERF_OBJECT_TYPE obj_type = *(reinterpret_cast<PPERF_OBJECT_TYPE>(pStart));

				CTypeObject type_;
				if (S_OK != type_.Set(pStart, obj_type)) {
					m_error = type_.Error(); break;
				}
				this->m_objects.push_back(type_);

				pStart += obj_type.TotalByteLength;
				if (pStart >= pEnd)
					break;
			}

			return m_error;
		}

	private:
		CSysError    m_error  ;
		TTypeObjects m_objects;
	};

}}}}
using namespace proc::perform::reg::_impl;
/////////////////////////////////////////////////////////////////////////////

CPerformance:: CPerformance (void) { m_error << __MODULE__ << S_OK; }
CPerformance::~CPerformance (void) {}

/////////////////////////////////////////////////////////////////////////////

TSysErrRef  CPerformance::Error (void) const { return this->m_error; }

DWORD CPerformance::Get (void) {

	m_error << __MODULE__ << S_OK;

#define data_block_sz (65536)

	TRawData raw_data(data_block_sz);
	if (false == raw_data.IsValid())
		return (m_error = raw_data.Error());

	static LPCTSTR lp_sz_proc_ndx = _T("230");
	static DWORD dw_proc_obj_ndx = 230;

	DWORD raw_data_len = raw_data.GetSize();
#pragma region __get_data

	LSTATUS l_result = S_OK;
	while (true) {
		// https://learn.microsoft.com/en-us/windows/win32/api/winreg/nf-winreg-regqueryvalueexa ;
		l_result = ::RegQueryValueEx(
			HKEY_PERFORMANCE_DATA, lp_sz_proc_ndx, nullptr, nullptr, raw_data.GetData(), &raw_data_len
		);

		if (S_OK == l_result)
			break;
		else if (ERROR_MORE_DATA == l_result) {
			raw_data.Append(nullptr, data_block_sz);
			if (false  == raw_data.IsValid()) {
				m_error = raw_data.Error();
				break;
			}
			else {
				raw_data_len = raw_data.GetSize(); continue;
			}
		}
		else {
			m_error << l_result;
			break;
		}
	}
	::RegCloseKey(HKEY_PERFORMANCE_DATA); // mandatory;
#pragma endregion
	if ((bool)m_error)
		return m_error;

	_proc_perfs.Get();

	// https://learn.microsoft.com/en-us/cpp/cpp/try-except-statement ;
	// https://learn.microsoft.com/en-us/troubleshoot/developer/visualstudio/cpp/language-compilers/catch-exceptions ;
	try {
		PPERF_DATA_BLOCK  pPerfData = reinterpret_cast<PPERF_DATA_BLOCK>(raw_data.GetData());
		if (pPerfData) {
			// *important*: start and end addresses must be raw_data, otherwise types cannot be defined;
			CTypeObjects types;
			types.Set((LPBYTE)(raw_data.GetData() + pPerfData->HeaderLength), pPerfData->NumObjectTypes, (LPBYTE)(raw_data.GetData() + pPerfData->TotalByteLength));
		}
	}
	catch (...) {
	}
	
#if (0)
	PPERF_COUNTER_DEFINITION  pPerfCntr  = nullptr;
	PPERF_COUNTER_DEFINITION  pCurrCntr  = nullptr;
	PPERF_INSTANCE_DEFINITION pPerfInst  = nullptr;

	PERF_COUNTER_BLOCK counter_block = {0};

	
	PPERF_OBJECT_TYPE pObjType  = reinterpret_cast<PPERF_OBJECT_TYPE>(pPerfData + (DWORD)sizeof(PERF_DATA_BLOCK) + pPerfData->HeaderLength);

	for (DWORD i_ = 0; i_ < pPerfData->NumObjectTypes; i_++) {

		if (dw_proc_obj_ndx == pObjType->ObjectNameTitleIndex) {

			pPerfCntr = reinterpret_cast<PPERF_COUNTER_DEFINITION>(pObjType + (DWORD)sizeof(PERF_OBJECT_TYPE) + pObjType->HeaderLength);

			if (0 < pObjType->NumInstances) {

				pPerfInst = reinterpret_cast<PPERF_INSTANCE_DEFINITION>(pObjType + (DWORD)sizeof(PERF_OBJECT_TYPE) + pObjType->DefinitionLength);

				for (LONG j_ = 0; j_ < pObjType->NumInstances; j_++) {
				}
			}
		}
	}
#endif
	return m_error;
}

/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////