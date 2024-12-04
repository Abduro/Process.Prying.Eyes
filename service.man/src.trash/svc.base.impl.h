#ifndef _SHAREDSYSTEMSERVICEIMPL_H_A661449C_D435_49cc_AEB9_DE1FF278D5CD_INCLUDED
#define _SHAREDSYSTEMSERVICEIMPL_H_A661449C_D435_49cc_AEB9_DE1FF278D5CD_INCLUDED
/*
	Created by Tech_dog (ebontrop@gmail.com) on 5-Jun-2016 at 6:50:03p, UTC+7, Phuket, Rawai, Tuesday;
	This is shared lite library system service base interface declaration file.
	( project: thefileguardian.com )
	-----------------------------------------------------------------------------
	Adopted to Ebo Pack solution on 12-Jul-2020 at 4:50:10p, UTC+7, Novosibirsk, Sunday;
	-----------------------------------------------------------------------------
	Adopted to Process Prying Eyes project on 13-Feb-2023 at 05:44:01.0073665, UTC+7, Novosibirsk, Monday;
*/
#include <dbt.h>
#include "shared.classes.h"

#include "svc.crt.data.h"
#include "svc.service.h"
#include "svc.status.h"

namespace service { namespace control  { // actually, the word 'control' has abbreviations like these: mgmt or mngmt; but control looks much better ;)

	using shared::sys_core::string_t;

	class CServiceBaseImpl
	{
	protected:
		virtual
		~CServiceBaseImpl(void);
		 CServiceBaseImpl(const CCrtData&);

	public:
		const
		CStatus&  Status (void) const;
		CStatus&  Status (void)      ;

	protected:  // events handlers
		virtual VOID     OnContinue(void);
		virtual VOID     OnPause   (void);
		virtual VOID     OnShutdown(void);
		virtual VOID     OnStart   (const DWORD dwArgc, LPTSTR* pszArgv);
		virtual VOID     OnStop    (void);

	private:
		VOID   Continue(void);
		VOID   Pause   (void);
		VOID   Shutdown(void);

	public:
		VOID   Exit (void);  // exits itself and informs service manager about this
		VOID   Start(DWORD dwArgc, PTSTR* pszArgv);
		VOID   Stop (void);

	protected:
		static VOID WINAPI ServiceCtrlHandler(DWORD dwControl);
	private:
		static VOID WINAPI ServiceMain(DWORD dwArgc, LPTSTR* lpszArgv);
	public:
		static BOOL WINAPI Run(CServiceBaseImpl&);
	protected:
		CCrtData  m_crt_data;
		CStatus   m_status;
	};

	class CServiceBaseImplEx : public CServiceBaseImpl {
	                          typedef CServiceBaseImpl TBase;
	protected:
		CServiceBaseImplEx(const CCrtData&);

	protected:  // events handlers
		virtual VOID OnDeviceEvent(const DWORD dwEventType, LPVOID const lpEventData) PURE;
		virtual VOID OnStart(const DWORD dwArgc, LPTSTR* pszArgv) override;
		virtual VOID OnStop(void) override;

	private:   // service system callbacks;
		static DWORD WINAPI HandlerEx(DWORD dwControl, DWORD dwEventType, LPVOID lpEventData, LPVOID lpContext); // https://learn.microsoft.com/en-us/windows/win32/api/winsvc/nf-winsvc-setservicestatus ;
		static VOID  WINAPI Main(DWORD dwArgc, LPTSTR* lpszArgv); // https://learn.microsoft.com/en-us/windows/win32/api/winsvc/nc-winsvc-lpservice_main_functionw ;

	public:
		static BOOL  WINAPI Run(CServiceBaseImplEx&);
	protected:
		HDEVNOTIFY       m_notify;
		DWORD            m_last_error;
	};
}}

#endif/*_SHAREDSYSTEMSERVICEIMPL_H_A661449C_D435_49cc_AEB9_DE1FF278D5CD_INCLUDED*/