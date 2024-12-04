#ifndef _SHAREDSYSTEMSERVICEMAN_H_A9A4D670_9050_4562_9D73_9457363B8347_INCLUDED
#define _SHAREDSYSTEMSERVICEMAN_H_A9A4D670_9050_4562_9D73_9457363B8347_INCLUDED
/*
	Created by Tech_dog (ebontrop@gmail.com) on 6-Jun-2016 at 2:02:10p, UTC+7, Phuket, Rawai, Monday;
	This is Shared system service manager wrapper class declaration file.
	( project: thefileguardian.com )
	-----------------------------------------------------------------------------
	Adopted to Ebo Pack solution on 13-Jul-2020 at 9:46:02a, UTC+7, Novosibirsk, Monday;
	-----------------------------------------------------------------------------
	Adopted to Process Prying Eyes project on 13-Feb-2023 at 05:03:03.7732840, UTC+7, Novosibirsk, Monday;
*/
#include "shared.sys.err.h"
#include "shared.raw.buf.h"

#include "svc.crt.data.h"
#include "svc.service.h"
#include "svc.security.h"
#include "svc.setup.h"

namespace service { namespace control {

	using::shared::sys_core::CSysError;
	using::shared::sys_core::string_t;

	// https://docs.microsoft.com/en-us/windows/win32/services/service-security-and-access-rights
	class CSvcManagerParams {
	public:
		enum _e : DWORD {
			eOpenForService = SC_MANAGER_ALL_ACCESS,
			eOpenForEnum    = SC_MANAGER_ENUMERATE_SERVICE
		};
	};

	class CService_Operable {
	public:
		 CService_Operable (void);
		 CService_Operable (const CService_Operable&);
		 CService_Operable (LPCTSTR _lp_sz_svc_name);
		~CService_Operable (void);

	public:
		TSysErrRef  Error (void) const;
		bool        Is    (void) const;

		DWORD       Start (void) const;
		DWORD       Stop  (void) const;

	public:
		CService_Operable& operator = (const CService_Operable&);
		CService_Operable& operator <<(LPCTSTR _lp_sz_svc_name);

	private:
		mutable
		CSysError m_error;
		string_t  m_operable;
	};

	typedef CService_Operable TSvcOperate;

	class CSvcManager_Handle {
	public:
		 CSvcManager_Handle (void);
		 CSvcManager_Handle (const SC_HANDLE);
		~CSvcManager_Handle (void);

	public:
		DWORD      Attach (const SC_HANDLE _p_man_handle);
		DWORD      Close  (void) ;
		SC_HANDLE  Detach (void) ;
		TSysErrRef Error  (void) const;
		const bool Is     (void) const;

	public:
		DWORD      Connect(const DWORD _dw_access = SC_MANAGER_ALL_ACCESS) ; // opens a connection to system service manager;

	public:
		CSvcManager_Handle& operator << (const SC_HANDLE);  // attaches to service manager handle;
		CSvcManager_Handle& operator >> (SC_HANDLE&);       // detaches from service manager handle handle;

	public:
		operator const bool (void) const;
		operator const SC_HANDLE& (void) const;

	public:
		static bool Is_acceptable(const SC_HANDLE);

	private: // noncopyable;
		CSvcManager_Handle (CSvcManager_Handle&);
		CSvcManager_Handle& operator= (const CSvcManager_Handle&);
	private:
		mutable
		CSysError     m_error  ;
		SC_HANDLE     m_handle ;
	};
	//
	// https://docs.microsoft.com/en-us/windows/win32/services/service-security-and-access-rights
	// GENERIC_READ - to get service current state;
	//
	typedef CSvcManager_Handle TSCHandle;
	//
	// *Note*: sample of deleting a service:
	//         https://docs.microsoft.com/en-us/windows/win32/services/deleting-a-service
	//         lies regarding open service security right - when DELETE is specified, access denied error is returned;
	//         the right way is to specify SERVICE_ALL_ACCESS security level for getting success;
	//
	class CSvcManager {
	public:
		 CSvcManager(void);
		~CSvcManager(void);

	public:
		TSysErrRef  Error  (void) const;
		bool        Is     (void) const;
		DWORD       Open   (const CCrtData&, const DWORD dwAccess, CService&);
		DWORD       Open   (LPCTSTR _lp_sz_svc_name , const DWORD dwAccess, CService&);
		const
		TSvcOperate&Operate(void) const;
		TSvcOperate&Operate(void)      ;
		const
		CSetup&     Setup  (void) const;   // gets a service setup object reference; (read);
		CSetup&     Setup  (void)      ;   // gets a service setup object reference; (read-write);
		
	public:
		const
		TSCHandle&  Handle(void) const;
		TSCHandle&  Handle(void)      ;
		operator SC_HANDLE(void) const;

	private: // non-copyable;
		CSvcManager(const CSvcManager&);
		CSvcManager&  operator= (const CSvcManager&);

	public:
		CSvcManager&  operator= (const SC_HANDLE);
	protected:
		mutable
		CSysError   m_error ;
		TSCHandle   m_handle;
		CSetup      m_setup ;
		TSvcOperate m_operate;
	};
}}

typedef service::control::CSvcManager   TServiceMan;

#endif/*_SHAREDSYSTEMSERVICEMAN_H_A9A4D670_9050_4562_9D73_9457363B8347_INCLUDED*/