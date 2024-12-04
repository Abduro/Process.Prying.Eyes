#ifndef _SVCHANDLER_H_408D8495_025B_418C_BBDF_95DBACBA36EF_INCLUDED
#define _SVCHANDLER_H_408D8495_025B_418C_BBDF_95DBACBA36EF_INCLUDED
/*
	Created by Tech_dog (ebontrop@gmail.com) on 18-Feb-2023 at 07:12:54.1872040, UTC+7, Novosibirsk, Saturday;
	This is Process Prying Eyes project service event handler interface declaration file;
*/
#include "shared.classes.h"
#include "shared.sys.err.h"

#include "svc.crt.data.h"
#include "svc.status.h"

#include "log.journal.h"

namespace service { namespace events {

	using shared::sys_core::CSysError;
	using shared::sys_core::string_t ;

	using service::control::CStatus  ;
	using service::setting::CCrtData ;

	using shared::log::CEventLog ;
	using shared::log::CEventType;

	class CEvents {
	public:
		virtual
		~CEvents (void);
		 CEvents (void);

	public:
		TSysErrRef Error (void) const;

	public: // event handlers
		virtual DWORD  OnAhead   (void);
		virtual DWORD  OnPause   (void);
		virtual DWORD  OnStart   (const DWORD dwArgc, LPTSTR* pszArgv);
		virtual DWORD  OnStop    (void);
		virtual DWORD  OnTurnOff (void);

	protected:
		CSysError  m_error;
	};

	class CActions {
	public:
		 CActions (void);
		 CActions (const CCrtData&);
		~CActions (void);

	public:
		DWORD   Do (DWORD _dw_ctrl_id); // service control defines are used;
		DWORD   Set (const CCrtData&);  // sets create service data, especially, service name is important;
		DWORD   Set (const CEvents*);   // nullptr is accessible;
		DWORD   Start (DWORD dwArgc, PTSTR* pszArgv); // making a start is easy, but what would be faced later;

	protected:
		DWORD   GoAhead (void);  // an action to continue of service operation; just go ahead by right way;  >>SERVICE_CONTROL_CONTINUE;
		DWORD   Pause   (void);  // watch every step and sometimes get a time for getting right decision;    >>SERVICE_CONTROL_PAUSE;
		DWORD   Stop    (void);  // stop and take a look around;                                             >>SERVICE_CONTROL_STOP;
		DWORD   TurnOff (void);  // killing a service like making a service shutting down;                   >>SERVICE_CONTROL_SHUTDOWN;

	public:
		const
		CSysError&Error (void) const;
		const
		CStatus&  Status(void) const; // a status of an action shouldn't be used outside of service handler, so it is protected;
		CStatus&  Status(void)      ;

	public:
		DWORD  operator << (const CCrtData&);   // actually, sets a create data for service name identification;
		DWORD  operator << (DWORD _dw_ctrl_id); // calls an action in accordance with control operate identifier; returns an error code;

	protected:
		CCrtData  m_crt_data;
		CEvents*  m_p_events;    // is set to empty implementation by default; no smart pointer is used in this version;
		CStatus   m_status;      // for changing a service status before and after performing particular action;
		CSysError m_error ;
	};

	class CHandler {
	public:
		 CHandler (void);
		 CHandler (const CCrtData&);
		~CHandler (void);

	public:
		const
		CActions&   Actions(void) const;
		CActions&   Actions(void)      ;
		const
		CCrtData&   Data   (void) const;
		CCrtData&   Data   (void)      ;
		TSysErrRef  Error  (void) const;
		DWORD       Set    (const CCrtData&); // refreshes action create data too;

	public:
		DWORD __stdcall Run (void);  // entry point for creating a service and registering all required event and action handlers;

	public:
		CHandler&   operator << (const CCrtData&);

	protected:
		CCrtData    m_crt_data;
		CSysError   m_error;
		CActions    m_actions;
	};
}}


#endif/*_SVCHANDLER_H_408D8495_025B_418C_BBDF_95DBACBA36EF_INCLUDED*/