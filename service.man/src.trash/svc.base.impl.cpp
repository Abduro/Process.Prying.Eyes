/*
	Created by Tech_dog (ebontrop@gmail.com) on 5-Jun-2016 at 6:58:29p, UTC+7, Phuket, Rawai, Tuesday;
	This is Shared Lite Library System Service Base class implementation file.
	( project: thefileguardian.com )
	-----------------------------------------------------------------------------
	Adopted to Ebo Pack solution on 12-Jul-2020 at 5:58:17p, UTC+7, Novosibirsk, Sunday;
	-----------------------------------------------------------------------------
	Adopted to Process Prying Eyes project on 13-Feb-2023 at 05:50:51.7644148, UTC+7, Novosibirsk, Monday;
*/
#include "svc.base.impl.h"
#include "svc.log.h"

using namespace service::control;

/////////////////////////////////////////////////////////////////////////////

namespace service { namespace control { namespace _impl
{
	CServiceBaseImpl*&    ServiceBaseImpl_SingletonPtrRef(void)
	{
		static CServiceBaseImpl* pObject = nullptr;
		return pObject;
	}

	CServiceBaseImplEx*&  ServiceBaseImpl_SingletonPtrRefEx(void)
	{
		static CServiceBaseImplEx* pObject = nullptr;
		return pObject;
	}

	VOID  ServiceBaseImpl_DummyLog(LPCTSTR pszMessage)
	{
		::MessageBox(nullptr, pszMessage, _T("CServiceBaseImpl"), MB_SERVICE_NOTIFICATION);
	}
}}}
using namespace service::control::_impl;
/////////////////////////////////////////////////////////////////////////////

CServiceBaseImpl:: CServiceBaseImpl(const CCrtData& _crt_ref) : m_crt_data(_crt_ref)
{
	this->m_status << this->m_crt_data;
}

CServiceBaseImpl::~CServiceBaseImpl(void) { }

/////////////////////////////////////////////////////////////////////////////
const
CStatus& CServiceBaseImpl::Status (void) const { return this->m_status; }
CStatus& CServiceBaseImpl::Status (void)       { return this->m_status; }

/////////////////////////////////////////////////////////////////////////////

VOID     CServiceBaseImpl::OnContinue(void) {}
VOID     CServiceBaseImpl::OnPause   (void) {}
VOID     CServiceBaseImpl::OnShutdown(void) {}
VOID     CServiceBaseImpl::OnStart   (const DWORD, LPTSTR*){}
VOID     CServiceBaseImpl::OnStop    (void) {}

/////////////////////////////////////////////////////////////////////////////

VOID   CServiceBaseImpl::Continue(void)
{
	CEventLog log_(this->m_crt_data.Properties().Name());

	try
	{
		this->Status().State() << (SERVICE_CONTINUE_PENDING);
		OnContinue();
		this->Status().State() << (SERVICE_RUNNING);
	}
	catch(DWORD dwError){ log_.WriteError(__MODULE__, dwError);          this->Status().State() << (SERVICE_PAUSED); }
	catch(...)          { log_.WriteError(__MODULE__, ::GetLastError()); this->Status().State() << (SERVICE_PAUSED); }
}

VOID   CServiceBaseImpl::Pause(void)
{
	CEventLog log_(this->m_crt_data.Properties().Name());

	try
	{
		this->Status().State() << (SERVICE_PAUSE_PENDING);
		OnPause();
		this->Status().State() << (SERVICE_PAUSED);
	}
	catch(DWORD dwError){ log_.WriteError(__MODULE__, dwError);          this->Status().State() << (SERVICE_RUNNING);}
	catch(...)          { log_.WriteError(__MODULE__, ::GetLastError()); this->Status().State() << (SERVICE_RUNNING);}
}

VOID   CServiceBaseImpl::Exit (void) { this->Status() << (SERVICE_STOPPED); }

VOID   CServiceBaseImpl::Shutdown(void)
{
	CEventLog log_(this->m_crt_data.Properties().Name());

	try
	{
		OnShutdown();
		this->Status().State() << (SERVICE_STOPPED);
	}
	catch(DWORD dwError){ log_.WriteError(__MODULE__, dwError);          }
	catch(...)          { log_.WriteError(__MODULE__, ::GetLastError()); }
}

VOID   CServiceBaseImpl::Start(DWORD dwArgc, PTSTR* pszArgv)
{
	CEventLog log_(this->m_crt_data.Properties().Name());
	try
	{
		this->Status().State() << (SERVICE_START_PENDING);
		OnStart(dwArgc, pszArgv);
		this->Status().State() << (SERVICE_RUNNING);
	}
	catch(DWORD dwError){ log_.WriteError(__MODULE__, dwError);          this->Status().State() << (SERVICE_STOPPED); }
	catch(...)          { log_.WriteError(__MODULE__, ::GetLastError()); this->Status().State() << (SERVICE_STOPPED); }
}

VOID   CServiceBaseImpl::Stop(void)
{
	CEventLog log_(this->m_crt_data.Properties().Name());
	const DWORD dwOriginalState = m_status.Data().dwCurrentState;

	try
	{
		this->Status().State() << (SERVICE_STOP_PENDING);
		OnStop();
		this->Status().State() << (SERVICE_STOPPED);
	}
	catch(DWORD dwError){ log_.WriteError(__MODULE__, dwError);          this->Status().State() << (dwOriginalState); }
	catch(...)          { log_.WriteError(__MODULE__, ::GetLastError()); this->Status().State() << (dwOriginalState); }
}

/////////////////////////////////////////////////////////////////////////////

VOID WINAPI CServiceBaseImpl::ServiceCtrlHandler(DWORD dwControl)
{
	ServiceBaseImpl_DummyLog(__MODULE__);

	CServiceBaseImpl* pObject = ServiceBaseImpl_SingletonPtrRef();
	if (!pObject)
		return;

	switch (dwControl)
    {
	case SERVICE_CONTROL_STOP:     pObject->Stop();     break;
	case SERVICE_CONTROL_PAUSE:    pObject->Pause();    break;
	case SERVICE_CONTROL_CONTINUE: pObject->Continue(); break;
	case SERVICE_CONTROL_SHUTDOWN: pObject->Shutdown(); break;
	default: break;
    }
}

VOID WINAPI CServiceBaseImpl::ServiceMain(DWORD dwArgc, LPTSTR* lpszArgv)
{
	dwArgc; lpszArgv;
	ServiceBaseImpl_DummyLog(__MODULE__);

	CServiceBaseImpl* pObject = ServiceBaseImpl_SingletonPtrRef();
	if (!pObject)
		return;
	// https://learn.microsoft.com/en-us/windows/win32/api/winsvc/nf-winsvc-registerservicectrlhandlerexa ;
	pObject->Status() << ::RegisterServiceCtrlHandler( // ***important***: must be immediately called in this function; 
			_T("SharedServiceBase"),
			CServiceBaseImpl::ServiceCtrlHandler
		);
	if (!pObject->Status().Is() == false)
		return;

	pObject->Start(dwArgc, lpszArgv);
}

/////////////////////////////////////////////////////////////////////////////

BOOL WINAPI CServiceBaseImpl::Run(CServiceBaseImpl& obj_ref)
{
	ServiceBaseImpl_SingletonPtrRef() = &obj_ref;
	ServiceBaseImpl_DummyLog(__MODULE__);

	const CCrtData& crt_data = obj_ref.m_crt_data;

	TCHAR szServiceName[_MAX_PATH] = {0};
	::_tcscpy_s(szServiceName, _countof(szServiceName) - 1, crt_data.Properties().Name());

	string_t cs_name( _T("SharedServiceBase"));
	::std::vector<TCHAR> v_name(cs_name.begin(), cs_name.end());

	SERVICE_TABLE_ENTRY serviceTable[] = 
	{
		{ &v_name[0], CServiceBaseImpl::ServiceMain },
		{ nullptr, nullptr }
	};

	return ::StartServiceCtrlDispatcher(serviceTable);
}

/////////////////////////////////////////////////////////////////////////////

CServiceBaseImplEx::CServiceBaseImplEx(const CCrtData& _crt) : TBase(_crt), m_notify(nullptr), m_last_error(NO_ERROR)
{
}

/////////////////////////////////////////////////////////////////////////////

VOID  CServiceBaseImplEx::OnStart(const DWORD dwArgc, LPTSTR* pszArgv)
{
	dwArgc; pszArgv;
	m_last_error = NO_ERROR;
#if (0)
	DEV_BROADCAST_DEVICEINTERFACE filter_ = {0};
	filter_.dbcc_size       = sizeof(DEV_BROADCAST_DEVICEINTERFACE);
	filter_.dbcc_devicetype = DBT_DEVTYP_DEVICEINTERFACE;
	filter_.dbcc_reserved   = 0;
	filter_.dbcc_classguid  = GUID_DEVINTERFACE_USB_DEVICE;

	m_notify = ::RegisterDeviceNotification(
					TBase::m_statusHandle,
					&filter_,
					DEVICE_NOTIFY_SERVICE_HANDLE|DEVICE_NOTIFY_ALL_INTERFACE_CLASSES
				);
	if (m_notify == nullptr)
		m_last_error = ::GetLastError();
#endif
}

VOID  CServiceBaseImplEx::OnStop(void)
{
#if (0)
	if (m_notify)
	{
		::UnregisterDeviceNotification(m_notify);
		m_notify = nullptr;
	}
#endif
}

/////////////////////////////////////////////////////////////////////////////

DWORD CServiceBaseImplEx::HandlerEx(DWORD dwControl, DWORD dwEventType, LPVOID lpEventData, LPVOID lpContext)
{
	TBase::ServiceCtrlHandler(dwControl);

	CServiceBaseImplEx* pObject = reinterpret_cast<CServiceBaseImplEx*>(lpContext);
	if (!pObject)
		return ERROR_OBJECT_NOT_FOUND;

	switch (dwControl)
	{
	case SERVICE_CONTROL_DEVICEEVENT:
		{
			pObject->OnDeviceEvent(dwEventType, lpEventData);
		} break;
	}
	return NO_ERROR;
}

VOID  CServiceBaseImplEx::Main(DWORD dwArgc, LPTSTR* lpszArgv)
{
	dwArgc; lpszArgv;
	ServiceBaseImpl_DummyLog(__MODULE__);

	CServiceBaseImplEx* pObject = ServiceBaseImpl_SingletonPtrRefEx();
	if (!pObject)
		return;

	pObject->Status() << ::RegisterServiceCtrlHandlerEx(
			pObject->m_crt_data.Properties().Name(),
			CServiceBaseImplEx::HandlerEx,
			pObject
		);
	if (!pObject->Status().Is())
		return;

	pObject->Start(dwArgc, lpszArgv);
}

BOOL  CServiceBaseImplEx::Run (CServiceBaseImplEx& obj_ref)
{
	ServiceBaseImpl_SingletonPtrRef  () = &obj_ref;
	ServiceBaseImpl_SingletonPtrRefEx() = &obj_ref;
	ServiceBaseImpl_DummyLog(__MODULE__);

	const CCrtData& crt_data = obj_ref.m_crt_data;

	TCHAR szServiceName[_MAX_PATH] = {0};
	::_tcscpy_s(szServiceName, _countof(szServiceName) - 1, crt_data.Properties().Name());

	SERVICE_TABLE_ENTRY serviceTable[] = 
	{
		{ szServiceName, CServiceBaseImplEx::Main },
		{ nullptr, nullptr }
	};
	// https://docs.microsoft.com/en-us/windows/win32/api/winsvc/nf-winsvc-startservicectrldispatchera
	return ::StartServiceCtrlDispatcher(serviceTable);
}