/*
	Created by Tech_dog (ebontrop@gmail.com) on 25-Feb-2023 at 19:07:05.5618894, UTC+7, Novosibirsk, Saturday;
	This is Process Prying Eyes project debug fake service interface implementation file;
*/
#include "StdAfx.h"
#include "PE_service_fake.h"

using namespace service::pe;

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

CService_Fake:: CService_Fake (void) : TThread() { m_error << __MODULE__ << S_OK; }
CService_Fake::~CService_Fake (void) {}

/////////////////////////////////////////////////////////////////////////////

TSysErrRef      CService_Fake::Error (void) const { return this->m_error; }
HRESULT         CService_Fake::Start (void) {
	m_error << __MODULE__ << S_OK;

	TThread::Start();
	_out << CRec_Info(CFormatter().Accept(_T("Thread fun %s has started;"), _T("CService_Fake::WorkFun")));

	return m_error;
}

HRESULT         CService_Fake::Stop  (void) {
	m_error << __MODULE__ << S_OK;

	TThread::Stop();
	_out << CRec_Info(CFormatter().Accept(_T("Thread fun %s has stopped;"), _T("CService_Fake::WorkFun")));

	return m_error;
}

/////////////////////////////////////////////////////////////////////////////

void CService_Fake::WorkFun   (void) {
	_out << CSection(__MODULE__);
//	_out << CRec_Info(_T("Press [ESC] key to stop monitoring processes;"));
	_out << CRec_Info(CFormatter().Accept(_T("Thread fun %s is working;"), __MODULE__));

	CDelayEvent waiter_(50, 500);
	CSV csv;
	::_tprintf(_T("\n"));
	::_tprintf(csv.Header().To_string(TSeparator::eTab).c_str());

	while (false == TThread::m_crt.IsStopped()) {
		waiter_.Wait();
		if (waiter_.Elapsed())
			waiter_.Reset();
		else
			continue;

		::proc::perform::gpu::CPerf_Gpu gpu_dat;

		gpu_dat.Usage();

#if (0)
		CProcess_List proc_list;
		proc_list.Get_It();
		::_tprintf(_T("\n\r"));
		::_tprintf(proc_list.Print().c_str());
#endif

#if defined(_DEBUG)
		break;
#endif
	}

	::SetEvent(TThread::m_crt.EventObject());
}

/////////////////////////////////////////////////////////////////////////////