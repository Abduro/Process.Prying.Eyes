/*
	Created by Tech_dog (ebontrop@gmail.com) on 18-Feb-2023 at 07:47:09.0528688, UTC+7, Novosibirsk, Saturday;
	This is Process Prying Eyes project service event handler interface implementation file;
*/
#include "svc.events.h"

using namespace service::events;
using namespace service::control;

/////////////////////////////////////////////////////////////////////////////

namespace service { namespace events { namespace _impl {

	CHandler*&  _only_one(void)
	{
		static CHandler* p_handler = nullptr;
		return p_handler;
	}

}}}
using namespace service::events::_impl;

/////////////////////////////////////////////////////////////////////////////

CEvents:: CEvents (void) { m_error << __MODULE__ << S_OK; }
CEvents::~CEvents (void) {}

/////////////////////////////////////////////////////////////////////////////

TSysErrRef CEvents::Error (void) const { return this->m_error; }

DWORD  CEvents::OnAhead   (void) {
	m_error << __MODULE__ << S_OK;
	return m_error;
}
DWORD  CEvents::OnPause   (void) {
	m_error << __MODULE__ << S_OK;
	return m_error;
}
DWORD  CEvents::OnTurnOff (void) {
	m_error << __MODULE__ << S_OK;
	return m_error;
}
DWORD  CEvents::OnStart   (const DWORD dwArgc, LPTSTR* pszArgv) { dwArgc; pszArgv;
	m_error << __MODULE__ << S_OK;
	return m_error;
}
DWORD  CEvents::OnStop    (void) {
	m_error << __MODULE__ << S_OK;
	return m_error;
}

/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////

CActions:: CActions (void) : m_p_events(new CEvents) { m_error << __MODULE__ << S_OK; }
CActions:: CActions (const CCrtData& _crt_data) : CActions() { *this << _crt_data; }
CActions::~CActions (void) { this->Set((CEvents*)nullptr); }

/////////////////////////////////////////////////////////////////////////////

DWORD  CActions::Do (DWORD _dw_ctrl_id) {
	m_error << __MODULE__ << S_OK;

	switch (_dw_ctrl_id)
	{
	case SERVICE_CONTROL_CONTINUE: m_error = this->GoAhead(); break;
	case SERVICE_CONTROL_PAUSE   : m_error = this->Pause();   break;
	case SERVICE_CONTROL_STOP    : m_error = this->Stop();    break;
	case SERVICE_CONTROL_SHUTDOWN: m_error = this->TurnOff(); break;
	default:;
	}

	return m_error;
}

DWORD  CActions::Set(const CCrtData& _crt_data) {
	m_error << __MODULE__ << S_OK;

	if (false == _crt_data.Is_valid())
		return (this->m_error = _crt_data.Error());

	return m_error;
}

DWORD  CActions::Set(const CEvents* _p_evts) {

	if (this->m_p_events) {
		delete this->m_p_events; this->m_p_events = nullptr;
	}

	this->m_p_events = const_cast<CEvents*>(_p_evts);

	return S_OK;
} 

/////////////////////////////////////////////////////////////////////////////

DWORD  CActions::GoAhead (void) {
	m_error << __MODULE__ << S_OK;

	if (nullptr == this->m_p_events)
		return (m_error << ERROR_INVALID_HANDLE);

	CEventLog log_(this->m_crt_data.Properties().Name());
	// specifying a long data type is required, otherwise motherfucking compiler complains to different declarations;
	try
	{
		this->Status().State() << (LONG)(SERVICE_CONTINUE_PENDING); m_error << this->Status().Update();
		this->m_error = this->m_p_events->OnAhead();
		this->Status().State() << (LONG)(SERVICE_RUNNING);          m_error << this->Status().Update();
	}
	catch(DWORD dwError){ log_.WriteError(__MODULE__, dwError);          this->Status().State() << (LONG)(SERVICE_PAUSED); }
	catch(...)          { log_.WriteError(__MODULE__, ::GetLastError()); this->Status().State() << (LONG)(SERVICE_PAUSED); }

	return m_error;
}
DWORD  CActions::Pause   (void) {
	m_error << __MODULE__ << S_OK;

	if (nullptr == this->m_p_events)
		return (m_error << ERROR_INVALID_HANDLE);

	CEventLog log_(this->m_crt_data.Properties().Name());
	try
	{
		this->Status().State() << (LONG)(SERVICE_PAUSE_PENDING); m_error << this->Status().Update();
		this->m_p_events->OnPause();
		this->Status().State() << (LONG)(SERVICE_PAUSED);        m_error << this->Status().Update();
	}
	catch(DWORD dwError){ log_.WriteError(__MODULE__, dwError);          this->Status().State() << (LONG)(SERVICE_RUNNING);}
	catch(...)          { log_.WriteError(__MODULE__, ::GetLastError()); this->Status().State() << (LONG)(SERVICE_RUNNING);}

	return m_error;
}
DWORD  CActions::Start   (DWORD dwArgc, PTSTR* pszArgv) { dwArgc; pszArgv;
	m_error << __MODULE__ << S_OK;

	if (nullptr == this->m_p_events)
		return (m_error << ERROR_INVALID_HANDLE);

	CEventLog log_(this->m_crt_data.Properties().Name());
	try
	{
		this->Status().State() << (LONG)(SERVICE_START_PENDING); m_error << this->Status().Update();
		this->m_p_events->OnStart(dwArgc, pszArgv);
		this->Status().State() << (LONG)(SERVICE_RUNNING);       m_error << this->Status().Update();
	}
	catch(DWORD dwError){ log_.WriteError(__MODULE__, dwError);          this->Status().State() << (LONG)(SERVICE_STOPPED); }
	catch(...)          { log_.WriteError(__MODULE__, ::GetLastError()); this->Status().State() << (LONG)(SERVICE_STOPPED); }

	return m_error;
}
DWORD  CActions::Stop    (void) {
	m_error << __MODULE__ << S_OK;

	if (nullptr == this->m_p_events)
		return (m_error << ERROR_INVALID_HANDLE);

	CEventLog log_(this->m_crt_data.Properties().Name());
	const DWORD dwOriginalState = m_status.Data().dwCurrentState;

	try
	{
		this->Status().State() << (LONG)(SERVICE_STOP_PENDING); m_error << this->Status().Update();
		this->m_p_events->OnStop();
		this->Status().State() << (LONG)(SERVICE_STOPPED);      m_error << this->Status().Update();
	}
	catch(DWORD dwError){ log_.WriteError(__MODULE__, dwError);          this->Status().State() << (LONG)(dwOriginalState); }
	catch(...)          { log_.WriteError(__MODULE__, ::GetLastError()); this->Status().State() << (LONG)(dwOriginalState); }

	return m_error;
}
DWORD  CActions::TurnOff (void) {
	m_error << __MODULE__ << S_OK;

	if (nullptr == this->m_p_events)
		return (m_error << ERROR_INVALID_HANDLE);

	CEventLog log_(this->m_crt_data.Properties().Name());
	try
	{
		this->m_p_events->OnTurnOff();
		this->Status().State() << (LONG) (SERVICE_STOPPED); m_error << this->Status().Update();
	}
	catch(DWORD dwError){ log_.WriteError(__MODULE__, dwError);          }
	catch(...)          { log_.WriteError(__MODULE__, ::GetLastError()); }

	return m_error;
}

/////////////////////////////////////////////////////////////////////////////
const
CStatus&  CActions::Status (void) const { return this->m_status; }
CStatus&  CActions::Status (void)       { return this->m_status; }

/////////////////////////////////////////////////////////////////////////////

TSysErrRef CActions::Error (void) const { return this->m_error; }

/////////////////////////////////////////////////////////////////////////////

DWORD  CActions::operator << (const CCrtData& _crt_data) { this->Set(_crt_data); return this->m_error; }
DWORD  CActions::operator << (DWORD _dw_ctrl_id) { return this->Do(_dw_ctrl_id); }

/////////////////////////////////////////////////////////////////////////////

CHandler:: CHandler (void) { m_error << __MODULE__ << S_OK; _only_one() = this; }
CHandler:: CHandler (const CCrtData& _crt_data) : CHandler() { *this << _crt_data; }
CHandler::~CHandler (void) {}

/////////////////////////////////////////////////////////////////////////////

namespace service { namespace events { namespace _impl {

	static DWORD __stdcall _svc_hanler(DWORD _dw_ctrl, DWORD _dw_type, LPVOID _lp_data, LPVOID _lp_ctx) {
		_dw_ctrl; _dw_type; _lp_data; _lp_ctx;
		CSysError err_(__MODULE__);

		CHandler* p_handler = static_cast<CHandler*>(_lp_ctx);
		if (nullptr == p_handler)
			err_ << ERROR_INVALID_HANDLE;
		else {
			p_handler->Actions() << _dw_ctrl;
			err_ = p_handler->Actions().Error();
		}

		return err_.Get(); // calling this method explicitly is required due to __stdcall specifier;
	}

	// https://learn.microsoft.com/en-us/windows/win32/api/winsvc/nc-winsvc-lpservice_main_functionw ;
	static void  __stdcall _svc_proc (DWORD dwArgc, LPTSTR* lpszArgv) {
		dwArgc; lpszArgv;

		CHandler* p_handler = _only_one();
		if (nullptr == p_handler)
			return;
	// https://learn.microsoft.com/en-us/windows/win32/api/winsvc/nf-winsvc-registerservicectrlhandlerexa ;
		p_handler->Actions().Status() << ::RegisterServiceCtrlHandlerEx( //*important*: must be immediately called in this function; 
			p_handler->Data().Properties().Name(),
			_svc_hanler,
			p_handler
		);
		if (p_handler->Actions().Status().Is() == false)
			return;
		else
			p_handler->Actions().Start(dwArgc, lpszArgv);
	}

}}}
using namespace service::events::_impl;
/////////////////////////////////////////////////////////////////////////////
const
CActions&  CHandler::Actions(void) const { return this->m_actions; }
CActions&  CHandler::Actions(void)       { return this->m_actions; }

const
CCrtData&  CHandler::Data   (void) const { return this->m_crt_data; }
CCrtData&  CHandler::Data   (void)       { return this->m_crt_data; }

TSysErrRef CHandler::Error (void) const { return this->m_error; }

DWORD  CHandler::Set (const CCrtData& _crt_data) {
	m_error << __MODULE__ << S_OK;

	if (false == _crt_data.Is_valid())
		return (m_error = _crt_data.Error());

	this->m_crt_data = _crt_data;
	this->m_actions << _crt_data;

	return m_error;
}

/////////////////////////////////////////////////////////////////////////////

DWORD CHandler::Run (void) {
	m_error << __MODULE__ << S_OK;
	
	if (false == this->m_crt_data.Properties().Name().Is())
		return (m_error = this->m_crt_data.Error());

	string_t cs_svec_name(this->m_crt_data.Properties().Name());

	::std::vector<TCHAR> v_svc_name(cs_svec_name.begin(), cs_svec_name.end());
	// https://learn.microsoft.com/en-us/windows/win32/api/winsvc/ns-winsvc-service_table_entrya ;
	SERVICE_TABLE_ENTRY svc_entry_[] = 
	{
		{ &v_svc_name[0], _svc_proc },
		{ nullptr, nullptr }
	};

	// https://docs.microsoft.com/en-us/windows/win32/api/winsvc/nf-winsvc-startservicectrldispatchera ;
	const bool b_result = !!::StartServiceCtrlDispatcher(svc_entry_);
	if (false == b_result)
		m_error.Last();

	return m_error;
}

/////////////////////////////////////////////////////////////////////////////

CHandler&  CHandler::operator << (const CCrtData& _crt_data) { this->Set(_crt_data); return *this; }

/////////////////////////////////////////////////////////////////////////////