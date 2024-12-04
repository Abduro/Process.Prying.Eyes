#ifndef _SVCSTATUS_H_835B8F7F_C719_4237_887D_AB7FF3284AA6_INCLUDED
#define _SVCSTATUS_H_835B8F7F_C719_4237_887D_AB7FF3284AA6_INCLUDED
/*
	Created by Tech_dog (ebontrop@gmail.com) on 6-Aug-2020 at 3:23:27p, UTC+7, Novosibirsk, Thursday;
	This is Ebo Pack solution system service status information interface declaration file.
	-----------------------------------------------------------------------------
	Adopted to Process Prying Eyes project on 12-Feb-2023 at 11:18:07.7130951, UTC+7, Novosibirsk, Sunday;
*/
#include "shared.sys.err.h"
#include "svc.cfg.h"        // CCfg_Base;
#include "svc.crt.data.h"
#include "svc.state.h"

namespace service { namespace control {

	using shared::sys_core::CSysError;
	using service::setting::CCrtData;
	using service::setting::CCtrlOption;
	using service::setting::CTypeOption;

	// http://acronymsandslang.com/abbreviation-for/STATUS.html ;
	// https://learn.microsoft.com/en-us/windows/win32/api/winsvc/ns-winsvc-service_status ;
	typedef SERVICE_STATUS TSvcStat;
	typedef SERVICE_STATUS_HANDLE TSvcStatPtr;

	// https://docs.microsoft.com/en-us/windows/win32/api/winsvc/nf-winsvc-enumservicesstatusw ;
	// https://docs.microsoft.com/en-us/windows/win32/api/winsvc/nf-winsvc-queryservicestatus  ;
	class CStatus : public CCfg_Base {
	               typedef CCfg_Base TBase;
	public:
		 CStatus (void);
		 CStatus (const CStatus&);
		~CStatus (void);

	public:
		TSvcStat  Data  (void) const;
		TSvcStat& Data  (void)      ;                    // TODO: might be superfluous reference; it's kept for now;
		bool      Is    (void) const;                    // returns true, if status data is set, otherwise, false;
		DWORD     Query (const SC_HANDLE _p_svc_handle); // sets owner service handle and refreshes status data;
		DWORD     Reset (void)      ;
		DWORD     Set   (const CCrtData&);               // applies input create data to status values;
		DWORD     Set   (const TSvcStatPtr);             // sets a pointer of registering service control handler;
		DWORD     Update(void);                          // updates/refreshes a service current status;

	public:
		const
		CState&   State (void) const;                    // gets current state wrapper;
		CState&   State (void)      ;                    // looks like necessary;

	public: // operators
		CStatus&  operator  = (const CStatus&);
		CStatus&  operator << (const SC_HANDLE);         // sets owner service handle and refreshes status data;
		CStatus&  operator << (const CCrtData&);
		CStatus&  operator << (const TSvcStatPtr);

		operator  TSvcStat& (void);
		operator  TSvcStatPtr& (void);

	// https://docs.microsoft.com/en-us/windows/win32/api/winsvc/ns-winsvc-service_status
	private:
		TSvcStat    m_stat;
		TSvcStatPtr m_stat_ptr;
		CState      m_state;
	};
}}
typedef       service::control::CStatus  TSvcStatus   ;
typedef const service::control::CStatus& TSvcStatusRef;

#endif/*_SVCSTATUS_H_835B8F7F_C719_4237_887D_AB7FF3284AA6_INCLUDED*/