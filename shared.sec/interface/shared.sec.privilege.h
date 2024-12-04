#ifndef _SHAREDSECPRIVILEGE_H_C00402FD_0910_481E_B703_08E54514F3B2_INCLUDED
#define _SHAREDSECPRIVILEGE_H_C00402FD_0910_481E_B703_08E54514F3B2_INCLUDED
/*
	Created by Tech_dog (ebontrop@gmail.com) on 26-Jan-2015 at 00:00:00.0a, UTC+3, Taganrog, Monday;
	This is Web setup project generic security digital certificate privilege interface declaration file; (Internext Ltd, RO);
	-----------------------------------------------------------------------------
	Adopted to Ebo Pack security project on 21-Jan-2021 at 8:32:40.538 pm, UTC+7, Novosibirsk, Thursday;
	Adopted to Process Prying Eyes project on 10-Mar-2023 at 16:01:51.8695485, UTC+7, Novosibirsk, Friday;
*/
#include <wincrypt.h>
#include "shared.classes.h"
#include "shared.sys.err.h"
#include "shared.gen.hand.h"
#include "shared.raw.buf.h"

namespace shared { namespace security {

	using shared::sys_core::CSysError;
	using shared::common::CAutoHandle;
	using shared::common::CRawData;
	using shared::sys_core::string_t;

	// https://docs.microsoft.com/en-us/windows/win32/secauthz/privilege-constants
	// all privilege predifined system names are declared in <winnt.h>;
	// the name is mandatory for getting/setting previlege current state;
	// https://docs.microsoft.com/en-us/windows/win32/api/winnt/ne-winnt-token_information_class
	
#if (0)
	class CPriv_Name {
	public:
		enum e_type  { e_sortcut = 0, e_display };
	public:
		 CPriv_Name (void);
		 CPriv_Name (const CPriv_Name&);
		~CPriv_Name (void);

	public:
		LPCTSTR     Get (void) const;
		HRESULT     Set (const e_type, const LUID& _luid);

	public:
		CPriv_Name& operator = (const CPriv_Name&);
		CPriv_Name& operator <<(const CPriv_Name::e_type);
	private:
		string_t    m_name;

	};
#endif

	// https://docs.microsoft.com/en-us/windows/win32/api/winbase/nf-winbase-lookupprivilegenamew
	// https://docs.microsoft.com/en-us/windows/win32/api/winbase/nf-winbase-lookupprivilegevaluew

	class CPrivilege {
	public:
		enum e_state { e_none = 0, e_enable, e_disable };
		enum e_name  { e_sortcut = 0, e_display };
	public:
		 CPrivilege (void);
		 CPrivilege (const CPrivilege&);
		~CPrivilege (void);

	public:
		TSysErrRef Error  (void) const ;
		LPCTSTR    Names  (const e_name = e_name::e_sortcut) const ;
		HRESULT    Names  (const LUID&);           // gets name from given privilege LUID structure by means of ::LookupPrivilegeName();
		e_state    State  (void) const ;           // gets a cached state;
		HRESULT    State  (const bool _b_enable);  // sets a state of the privilege; privilege name must be set before setting a state;

	public:
		CPrivilege& operator = (const CPrivilege&);
		CPrivilege& operator <<(const LUID&);      // updates privilege names;

	private:
		CSysError  m_error;
		string_t   m_names[e_name::e_display + 1];
		e_state    m_state;          // current state of the privilege; is not defined before querying the privilege state;
	};

	typedef ::std::vector<CPrivilege> TRawPrivileges;

	// https://docs.microsoft.com/en-us/windows/win32/api/handleapi/nf-handleapi-gethandleinformation - is good enough for checking handle validity;
	// https://docs.microsoft.com/en-us/windows/win32/api/securitybaseapi/nf-securitybaseapi-gettokeninformation

	class CPrivileges {
	public:
		 CPrivileges (void);
		 CPrivileges (const CPrivileges&);
		~CPrivileges (void);

	public:
		TSysErrRef    Error (void) const;
		const
		TRawPrivileges& Get (void) const;
		HRESULT         Set (const HANDLE _owner); // gets all privileges for given handle, for example, for handle of current process; 

	public:
		CPrivileges&  operator = (const CPrivileges& );
		CPrivileges&  operator <<(const HANDLE _owner);

	private:
		TRawPrivileges
		            m_privileges;
		CSysError   m_error;
	};

}}
/*
typedef shared::security::CPriv_Name  TSecPrivName  ;*/
typedef shared::security::CPrivilege  TSecPrivilege ;
typedef shared::security::CPrivilege::e_state TSecPrivState;
typedef shared::security::CPrivileges TSecPrivileges;

#endif/*_SHAREDSECPRIVILEGE_H_C00402FD_0910_481E_B703_08E54514F3B2_INCLUDED*/