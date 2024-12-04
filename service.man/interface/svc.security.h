#ifndef __SYSSVCSECURITY_H_3D8D0B95_1DE7_434A_AD84_C5D71384BD38_INCLUDED
#define __SYSSVCSECURITY_H_3D8D0B95_1DE7_434A_AD84_C5D71384BD38_INCLUDED
/*
	Created by Tech_dog (ebontrop@gmail.com) on 11-Aug-2020 at 7:20:09p, UTC+7, Novosibirsk, Tuesday;
	This is service manager security wrapper interface declaration file.
	-----------------------------------------------------------------------------
	Adopted to Process Prying Eyes project on 13-Feb-2023 at 02:16:23.6508750, UTC+7, Novosibirsk, Monday;
*/
#include <aclapi.h>

#include "shared.sys.err.h"
#include "shared.raw.buf.h"

#include "svc.service.h"

namespace service { namespace control {

	using shared::sys_core::CSysError;

	typedef PSECURITY_DESCRIPTOR p_sec_desc;
	// https://docs.microsoft.com/en-us/windows/win32/api/aclapi/nf-aclapi-buildexplicitaccesswithnamew
	class CExp_Access {
	private:
		CSysError   m_error;
		EXPLICIT_ACCESS m_ex_acc;

	public:
		 CExp_Access (void);
		 CExp_Access (const CExp_Access&);
		~CExp_Access (void);

	public:
		const
		EXPLICIT_ACCESS& Get(void) const;
		EXPLICIT_ACCESS& Get(void)      ;
		TSysErrRef  Error (void) const;
		DWORD       Query (const LPCTSTR _lp_sz_account);
		DWORD       Reset (void) ;

	public:
		CExp_Access&  operator  = (const CExp_Access&);
		CExp_Access&  operator << (const LPCTSTR _lp_sz_account);
	};
	// https://docs.microsoft.com/en-us/windows/win32/api/securitybaseapi/nf-securitybaseapi-getsecuritydescriptordacl
	class CSec_Desc {
	private:
		CSysError   m_error;
		PSECURITY_DESCRIPTOR m_p_desc;
		PACL        m_pdacl;
	public:
		 CSec_Desc (void);
		 CSec_Desc (const CSec_Desc&);
		~CSec_Desc (void);

	public:
		p_sec_desc  Get   (void) const;
		TSysErrRef  Error (void) const;
		const bool  Is    (void) const;
		DWORD       Query (const SC_HANDLE _p_service);
		DWORD       Reset (void) ;

	public:
		CSec_Desc&  operator  = (const CSec_Desc&); // works as move assign operator;
		CSec_Desc&  operator << (const SC_HANDLE _p_service);

	public:
		operator p_sec_desc const (void) const;
		operator PACL const (void) const;
	};
	// https://docs.microsoft.com/en-us/windows/win32/api/securitybaseapi/nf-securitybaseapi-initializesecuritydescriptor
	class CSecurity {
	private:
		CSysError   m_error ;
		CSec_Desc   m_sec_d ;
		CExp_Access m_access;

	public:
		 CSecurity (void);
		~CSecurity (void);

	public:
		TSysErrRef  Error (void) const;
		DWORD       Set   (SC_HANDLE _p_svc_man, LPCTSTR _lp_sz_svc_name);  // sets permissions for current user account;
		DWORD       Set   (SC_HANDLE _p_svc_man, LPCTSTR _lp_sz_svc_name, LPCTSTR _lp_sz_account);
	};

}}

#endif/*__SYSSVCSECURITY_H_3D8D0B95_1DE7_434A_AD84_C5D71384BD38_INCLUDED*/