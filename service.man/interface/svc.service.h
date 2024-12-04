#ifndef _SYSSVCOBJECT_H_54EA6A52_EE05_4685_8E50_AB7EDCF70B94_INCLUDED
#define _SYSSVCOBJECT_H_54EA6A52_EE05_4685_8E50_AB7EDCF70B94_INCLUDED
/*
	Created by Tech_dog (ebontrop@gmail.com) on 21-Jan-2015 on 8:45:58p, UTC+3, Taganrog, Wednesday;
	This is shared system service interface declaration file.
	-----------------------------------------------------------------------------
	Adopted to Ebo Pack solution on 12-Jul-2020 at 10:00:43a, UTC+7, Novosibirsk, Sunday;
	-----------------------------------------------------------------------------
	Adopted to Process Prying Eyes project on 12-Feb-2023 at 16:50:27.1551969, Novosibirsk, Sunday;
*/
#include "shared.sys.err.h"
#include "svc.cfg.h"
#include "svc.state.h"
#include "svc.status.h"

namespace service { namespace control {

	using shared::sys_core::CSysError;

	typedef LPHANDLER_FUNCTION_EX TSvcFuncPtr;

	class CService;
	// this class is for closing handle automatically when going out of scope;
	// https://docs.microsoft.com/en-us/windows/win32/api/winsvc/nf-winsvc-closeservicehandle
	class CService_Handle {
	public:
		 CService_Handle (CService* _p_owner = nullptr);
	explicit CService_Handle (const CService_Handle&);
	explicit CService_Handle (const SC_HANDLE);
		~CService_Handle (void);

	public:
		DWORD       Attach (const SC_HANDLE); // NULL is acceptable;
		DWORD       Close  (void)      ;
		SC_HANDLE   Detach (void)      ;
		TSysErrRef  Error  (void) const;
		const bool  Is     (void) const;

	public:
		operator SC_HANDLE (void) const;
		operator SC_HANDLE&(void)      ;

	public:
		CService_Handle& operator = (const CService_Handle&); // works like move assignment;
		CService_Handle& operator <<(const SC_HANDLE); // attach;
		CService_Handle& operator >>(SC_HANDLE&)     ; // detach;
	private:
		CService*   m_owner ; // this is a pointer to a service object for making a reset or an update of service properties;
		SC_HANDLE   m_handle;
		CSysError   m_error ;
	};

	typedef CService_Handle TSvcHandle;

	class CService
	{
	public:
		 CService (void);
	explicit CService (const CService& );
	explicit CService (const SC_HANDLE );
		~CService (void);

	public:
		const
		CCfg&       Cfg    (void) const;
		CCfg&       Cfg    (void)      ;
		const
		CCfg_Ex&    Cfg_Ex (void) const;
		CCfg_Ex&    Cfg_Ex (void)      ;
		TSysErrRef  Error  (void) const;
		const bool  Is     (void) const;
		DWORD       Reset  (void)      ;      // resets attributes to empty|none state;
		const
		CState&     State  (void) const;
		CState&     State  (void)      ;
		const
		CStatus&    Status (void) const;
		CStatus&    Status (void)      ;
		DWORD       Update (void)      ;      // updates all internal properties: cfg|cfg_ex|status|state;

	public:
		CService&   operator = (const CService&); // works as move assignment;
		CService&   operator <<(const SC_HANDLE); // makes a handle attachment;

	public:
		const
		TSvcHandle& Handle(void) const;
		TSvcHandle& Handle(void)      ;
		operator SC_HANDLE(void) const;

	private:
		mutable
		CSysError   m_error ;
		TSvcHandle  m_handle; // sets this object pointer as an owner when handle object being constructed;
		CCfg        m_cfg   ;
		CCfg_Ex     m_cfg_ex;
		CStatus     m_status;
		CState      m_state ; // current state of the service;
	};
}}

#endif/*_SYSSVCOBJECT_H_54EA6A52_EE05_4685_8E50_AB7EDCF70B94_INCLUDED*/