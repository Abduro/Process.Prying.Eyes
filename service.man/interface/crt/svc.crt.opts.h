#ifndef _SVCCRTOPTS_H_9828D42B_4E92_48AE_A6D3_3C6D4F583064_INCLUDED
#define _SVCCRTOPTS_H_9828D42B_4E92_48AE_A6D3_3C6D4F583064_INCLUDED
/*
	Created by Tech_dog (ebontrop@gmail.com) on 11-Feb-2023 at 21:26:03.7398886, UTC+7, Novosibirsk, Saturday;
	This is Peocess Prying Eyes project shared service create options' interface declaration file;
*/
#include <winsvc.h>
#include "shared.sys.err.h"
#include "shared.classes.h"

namespace service { namespace setting {

	using shared::sys_core::CSysError;
	using shared::sys_core::string_t;
	using shared::sys_core::CFormatter;

	// https://docs.microsoft.com/en-us/windows-server/administration/windows-commands/sc-create

	class COption {
	public:
		 COption (void);
		 COption (const COption&);
		 COption (DWORD _dw_value);
		~COption (void);

	public:
		DWORD Get (void) const;   // gets property value;
		void  Set (DWORD _value); // sets property value; the value must be checked by particular option itself;

	public:
		COption& operator = (const COption&);
		COption& operator <<(DWORD _dw_value);
		COption& operator +=(DWORD _dw_value);
		COption& operator -=(DWORD _dw_value);

	public:
		operator DWORD (void) const;

	protected:
		DWORD m_value;
	};

	class CCtrlOption : public COption {
	                   typedef COption TBase;
	public:
		enum value : DWORD {
			eStoppable   = SERVICE_ACCEPT_STOP          ,   // accepts a stop of a service;
			eCanContinue = SERVICE_ACCEPT_PAUSE_CONTINUE,   // pause and continue control are available through properties of SC console;
			eCanShutdown = SERVICE_ACCEPT_SHUTDOWN      ,   // unloading is acceptable;
		};
	public:
		 CCtrlOption (void);
		~CCtrlOption (void);

	public:
		bool CanPause(void) const;
		bool CanStop (void) const;
		bool Shutdown(void) const;

		bool Is_Valid(void) const;       // a value can be composed from several flags;
		string_t To_string (void) const; // output: option name=option value;
	};

	class CStartOption : public COption {
	                    typedef COption TBase;
	public:
		enum value : DWORD {
			eOnAuto      = SERVICE_AUTO_START           ,   // starts on operating system loading;
			eOnDemand    = SERVICE_DEMAND_START         ,   // manual start is acceptable;
		};
	public:
		 CStartOption (void);
		~CStartOption (void);

	public:
		bool OnAuto  (void) const;
		bool OnDemand(void) const;

		bool Is_Valid(void) const;       // a value can *not* be composed from several flags;
		string_t To_string (void) const; // output: option name=option value;
	};

	class CTypeOption : public COption {
	                   typedef COption TBase;
	public:
		enum value : DWORD {
			eKernelDrv   = SERVICE_KERNEL_DRIVER        ,   // runs as kernel mode driver;
			eService     = SERVICE_WIN32_OWN_PROCESS    ,   // runs as user mode service;
		};
	public:
		 CTypeOption (void);
		~CTypeOption (void);

	public:
		bool IsDriver (void) const;
		bool IsUserMode(void) const;

		bool Is_Valid(void) const;       // a value can *not* be composed from several flags;
		string_t To_string (void) const; // output: option name=option value;
	};

	class CCrtOptions { // service create option enumeration;
	public:
		 CCrtOptions (void);
		 CCrtOptions (const CCrtOptions&);
		~CCrtOptions (void);

	public: // options;
		const
		CCtrlOption&  Ctrl (void) const;
		CCtrlOption&  Ctrl (void)      ;
		const
		CStartOption& Start(void) const;
		CStartOption& Start(void)      ;
		const
		CTypeOption&  Type (void) const;
		CTypeOption&  Type (void)      ;

	public:
		TSysErrRef Error (void) const; // returns error details if one of the options is invalid;
		bool     Is_valid (void) const; // returns true if all options are valid;
		string_t To_string (LPCTSTR _lp_sz_sep = _T("; ")) const;

	public:
		CCrtOptions& operator = (const CCrtOptions& );
		CCrtOptions& operator <<(const CCtrlOption& );
		CCrtOptions& operator <<(const CStartOption&);
		CCrtOptions& operator <<(const CTypeOption& );

	protected:
		mutable
		CSysError     m_error;
		CCtrlOption   m_ctrl ;
		CStartOption  m_start;
		CTypeOption   m_type ;
	};
}}

typedef service::setting::CCrtOptions  TCrtOpts;

#endif/*_SVCCRTOPTS_H_9828D42B_4E92_48AE_A6D3_3C6D4F583064_INCLUDED*/