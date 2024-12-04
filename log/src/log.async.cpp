/*
	Created by Tech_dog (ebontrop@gmail.com) on 21-Sep-2018 at 10:24:50a, UTC+7, Novosibirsk, Friday;
	This is generic storage shared library CSV data asynchronous data provider interface implementation file.
*/
#include "log.csv.async.h"

using namespace shared::log;

/////////////////////////////////////////////////////////////////////////////

CCsvStorage:: CCsvStorage(ICsvDataStgCallback& _callee) : m_callee(_callee) { m_error << __MODULE__ << S_OK; }
CCsvStorage::~CCsvStorage(void) {}

/////////////////////////////////////////////////////////////////////////////

const
CCsvFile&     CCsvStorage::Data (void) const { return m_data;  }
CCsvFile&     CCsvStorage::Data (void)       { return m_data;  }
TSysErrRef    CCsvStorage::Error(void) const { return m_error; }
const
CCsvLocator&  CCsvStorage::Locator(void) const { return m_locator; }
CCsvLocator&  CCsvStorage::Locator(void)       { return m_locator; }

/////////////////////////////////////////////////////////////////////////////

VOID  CCsvStorage::WorkFun(VOID) {
	m_error << __MODULE__ << S_OK;

	if (m_locator.IsValid() == false) {
		m_error << OLE_E_BLANK;
		return;
	}

	HRESULT hr_ = S_OK;
	INT n_total = 0;

	m_callee.CsvStg_OnStart(n_total);

	for (INT i_ = 0; i_ < n_total && m_crt.IsStopped() == false; i_++) {
		if (m_callee.CsvStg_CanContinue() == false)
			break;
		else
			::Sleep(0); // TODO: must be controlled via wait object;

		TCsvRecord record_;

		hr_ = m_callee.CsvStg_OnProcess (i_, record_);
		if (FAILED(hr_)) {
			m_error = hr_; break;
		}

		hr_ = m_data.Append(m_locator, record_);
		if (FAILED(hr_)) {
			m_error = m_data.Error();
			m_callee.CsvStg_OnError(m_error);break;
		}

		m_callee.CsvStg_OnProgress(i_, n_total);
	}

	if (SUCCEEDED(hr_) && m_crt.IsStopped() == false) {
		hr_ = m_data.Save(m_locator);
		if (SUCCEEDED(hr_))
			m_callee.CsvStg_OnComplete();
		else
			m_callee.CsvStg_OnError(m_data.Error());
	}

	::SetEvent(m_crt.EventObject());
}