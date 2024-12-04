#ifndef _SHAREDSECUSERACCOUNT_H_C7A76F8D_88C4_4867_943F_0920CF3331CA_INCLUDED
#define _SHAREDSECUSERACCOUNT_H_C7A76F8D_88C4_4867_943F_0920CF3331CA_INCLUDED
/*
	Created by Tech_dog (ebontrop@gmail.com) on 2-Feb-2015 at 00:00:00.0a, UTC+3, Taganrog, Monday;
	This is Web setup project operating system security account interface declaration file; (Internext Ltd, RO);
	-----------------------------------------------------------------------------
	Adopted to File Guardian project on 16-Apr-2016 at 11:55:49 pm, GMT+7, Phuket, Rawai, Saturday; (thefileguardian.com project);
	Adopted to Ebo Pack project on 21-Jan-2021 at 11:51:17.152 pm, UTC+7, Novosibirsk, Thursday;
	Adopted to Process Prying Eyes project on 10-Mar-2023 at 17:42:31.2436211, UTC+7, Novosibirsk, Friday;
*/
#include <Sddl.h>
#include <Aclapi.h>
#include <psapi.h>

#include "shared.classes.h"
#include "shared.sys.err.h"
#include "shared.raw.buf.h"

namespace shared { namespace security {

	using shared::sys_core::CSysError;
	using shared::sys_core::string_t;
	using shared::common::CRawData;

	class CAccountSID {
	public:
		 CAccountSID (void);
		 CAccountSID (const CAccountSID&);
		~CAccountSID (void);

	public:
		TSysErrRef  Error    (void) const;
		const PSID  Get      (void) const;
		string_t    GetAsText(void) const;
		bool        Is       (void) const;
		HRESULT     Reset    (void)      ;                     // destroys an internal buffer if any;
		HRESULT     Set      (LPCTSTR _lp_sz_user_accout);     // gets user account descriptor from user account name, i.e. user name;

	public:
		CAccountSID& operator = (const CAccountSID&);
		CAccountSID& operator <<(const CRawData& _p_sid);      // makes a copy of raw data of SID provided;
		CAccountSID& operator <<(LPCTSTR _lp_sz_user_account); // performs a set operation;

	private:
		mutable
		CSysError  m_error  ;
		CRawData   m_raw_sid;
	}; 

	// https://support.microsoft.com/en-us/kb/243330
	class CSystemSIDs
	{
	public:
		enum _e {
			eUnknown     = 0x0,
			eLocalSystem = 0x1, // A service account that is used by the operating system.
		};
	public:
		static string_t GetSidAsText(const CSystemSIDs::_e);
	};

	// https://docs.microsoft.com/en-us/windows/win32/api/aclapi/nf-aclapi-geteffectiverightsfromaclw
	class CUserAccount {
	public:
		 CUserAccount (void);
		 CUserAccount (const CUserAccount&);
		 CUserAccount (LPCTSTR pszUserName);
		~CUserAccount (void);

	public:
		TSysErrRef   Error(void) const;
		bool         HasAccessTo(const PACL _p_dacl, const DWORD dwAccessLevel) const; // https://docs.microsoft.com/en-us/windows/win32/secauthz/access-mask
		bool         Is   (void) const;
		LPCTSTR      Name (void) const;
		HRESULT      Name (LPCTSTR)   ;  // resets the user account SID actually;
		const
		CAccountSID& SID  (void) const;  // is set by new name of user account; no direct write access is provided;

	public:
		CUserAccount& operator = (const CUserAccount& );
		CUserAccount& operator <<(LPCTSTR _lp_sz_name );

	protected:
		mutable
		CSysError   m_error  ;
		CAccountSID m_acc_sid;
		string_t    m_name   ;
	};
	// https://docs.microsoft.com/en-us/windows/win32/api/winbase/nf-winbase-getusernamew
	class CCurrentUser : public CUserAccount {
	                    typedef CUserAccount TUser;
	public:
		 CCurrentUser (void);
		~CCurrentUser (void);
	};

}}

typedef shared::security::CAccountSID      TSecAccSID;
typedef shared::security::CSystemSIDs      TSecSysSIDs ;
typedef shared::security::CSystemSIDs::_e  TSecSysSID_Type;
typedef shared::security::CUserAccount     TUserAccount;
typedef shared::security::CCurrentUser     TUserCurrent;   // naming style is not good; must be reviewed;

#endif/*_SHAREDSECUSERACCOUNT_H_C7A76F8D_88C4_4867_943F_0920CF3331CA_INCLUDED*/