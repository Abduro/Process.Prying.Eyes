#ifndef _SYSSVCCFG_H_0BF9BD9E_6B9F_4B18_B485_D3A45730FD56_INCLUDED
#define _SYSSVCCFG_H_0BF9BD9E_6B9F_4B18_B485_D3A45730FD56_INCLUDED
/*
	Created by Tech_dog (ebontrop@gmail.com) on 6-Aug-2020 at 3:23:27p, UTC+7, Novosibirsk, Thursday;
	This is Ebo Pack solution system service configuration information interface declaration file.
	-----------------------------------------------------------------------------
	Adopted to Process Prying Eyes project on 12-Feb-2023 at 11:23:23.7809716, UTC+7, Novosibirsk, Sunday;
*/
#include "shared.classes.h"
#include "shared.sys.err.h"
#include "shared.raw.buf.h"

namespace service { namespace control  {

	using shared::sys_core::string_t;
	using shared::sys_core::CSysError;

	// it very looks like INVALID_HANDLE_VALUE is not acceptable for SC_HANDLE in some cases and NULL must be used;
	class CCfg_Base {
	protected:
		 CCfg_Base (void);
		 CCfg_Base (const CCfg_Base&);
		~CCfg_Base (void);

	public:
		TSysErrRef  Error (void) const;        // gets the last error object;
		SC_HANDLE   Owner (void) const;        // gets service handle; the service is an owner of configuration information;
		DWORD       Owner (const SC_HANDLE);   // sets service handle;
		bool        Valid (void) const;        // checks service/owner handle;

	public:
		operator const SC_HANDLE&(void) const; // used in calling win api functions;

	public:
		CCfg_Base& operator  = (const CCfg_Base&);
		CCfg_Base& operator << (const SC_HANDLE ); // sets owner/service handle; INVALID_HANDLE_VALUE|NULL are acceptable for making it invalid;
	protected:
		SC_HANDLE   m_owner;   // it is generic handle of a service; it is not managed internally by this class;
		                       // *note*: a service must be opened with the access right: SERVICE_QUERY_CONFIG;
		CSysError   m_error;
	};

	// https://docs.microsoft.com/en-us/windows/win32/api/winsvc/nf-winsvc-queryserviceconfigw
	// https://docs.microsoft.com/en-us/windows/win32/api/winsvc/nf-winsvc-changeserviceconfigw
	class CCfg : public CCfg_Base {
	            typedef CCfg_Base TBase;
	public:
		typedef struct __qry_svc_cfg {
			DWORD    dwServiceType ;
			DWORD    dwStartType   ;
			string_t szBinaryPathName;
			string_t szLoadOrderGroup;
			string_t szDisplayName ;
			string_t szSericeName  ;
		} qry_svc_cfg;
	public:
		 CCfg (void);
		 CCfg (const CCfg&);
		~CCfg (void);

	public:
		const
		qry_svc_cfg& Data (void) const;
		qry_svc_cfg& Data (void)      ;
		bool         Is   (void) const;                    // checks a data pointer;
		DWORD        Query(const SC_HANDLE _p_svc_handle); // sets owner service handle and refreshes configuration data;
		DWORD        Reset(void);                          // resets configuration data buffer; owner handle is not affected;

	public:
		CCfg&        operator  = (const CCfg&);
		CCfg&        operator << (const SC_HANDLE);        // sets owner service handle and refreshes configuration data;
	private:
	// https://docs.microsoft.com/en-us/windows/win32/api/winsvc/ns-winsvc-query_service_configw
	// LPQUERY_SERVICE_CONFIGW  m_p_data;
		qry_svc_cfg  m_data;
	};

	// https://docs.microsoft.com/en-us/windows/win32/api/winsvc/nf-winsvc-queryserviceconfig2w
	// https://docs.microsoft.com/en-us/windows/win32/api/winsvc/nf-winsvc-changeserviceconfig2w
	class CCfg_Ex : public CCfg_Base {
	               typedef CCfg_Base TBase;
	public:
		 CCfg_Ex (void);
		 CCfg_Ex (const CCfg_Ex&);
		~CCfg_Ex (void);

	public:
		LPCTSTR   Description(void) const;              // gets a service extended description;
		DWORD     Description(LPCTSTR)   ;              // sets a service extended description: NULL - no change|empty string - removal data;
		bool      Is   (void) const;                    // checks a data pointer;
		DWORD     Query(const SC_HANDLE _p_svc_handle); // sets owner service handle and refreshes description data;
		DWORD     Reset(void);                          // resets configuration data buffer;

	public:
		CCfg_Ex&  operator = (const CCfg_Ex& );
		CCfg_Ex&  operator <<(const SC_HANDLE);
	private:
	// https://docs.microsoft.com/en-us/windows/win32/api/winsvc/ns-winsvc-service_descriptionw
	// LPSERVICE_DESCRIPTIONW  m_p_desc;
		string_t  m_szDesc;
	};

	class CCfgToStr {
	private:
		string_t  m_sepa;  // separator;
	public:
		 CCfgToStr (LPCTSTR _lp_sz_separator = _T(";"));
		~CCfgToStr (void);
	public:
		string_t  PrintError(const DWORD _svc_ctrl );
		string_t  PrintStart(const DWORD _svc_start); // start type can have a one value only; no enumeration;
		string_t  PrintState(const DWORD _svc_state); // service state value is not a mask, but just number sequence;
		string_t  PrintType (const DWORD _svc_type );
	};
}}

typedef       service::control::CCfg     TSvcCfg   ;
typedef const service::control::CCfg&    TSvcCfgRef;
typedef       service::control::CCfg_Ex  TSvcCfg_Ex;
typedef const service::control::CCfg_Ex& TSvcCfg_ExRef;

#endif/*_SYSSVCCFG_H_0BF9BD9E_6B9F_4B18_B485_D3A45730FD56_INCLUDED*/