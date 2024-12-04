#ifndef _SHAREDLITESYSTEMSECURITYDATAPROVIDER_H_DB811BBA_7977_433a_8C2B_E7DD2C23E543_INCLUDED
#define _SHAREDLITESYSTEMSECURITYDATAPROVIDER_H_DB811BBA_7977_433a_8C2B_E7DD2C23E543_INCLUDED
/*
	Created by Tech_dog (ebontrop@gmail.com) on 12-Aug-2012 at 12:43:20 pm, GMT+3, Rostov-on-Don, Sunday;
	This is PulsePay UAC compatable project system security data provider interface declaration file.
	-----------------------------------------------------------------------------
	Adopted to Internext project on 21-Aug-2014 at 0:03:27.335 am, GMT+3, Taganrog, Thursday;
	Adopted to Ebo Pack security project on 20-Jan-2021 at 9:45:12.143 pm, GMT+7, Novosibirsk, Wednesday;
	-----------------------------------------------------------------------------
	Adopted to Process Prying Eyes project on 10-Mar-2023 at 20:00:40.8746099, UTC+7, Novosibirsk, Friday;
*/
#include "shared.cert.h"
#include "shared.sec.privilege.h"
#include "shared.sec.user.account.h"
#include "shared.sec.file.acc.h"

#include <shellapi.h>
#pragma comment(lib, "shell32.lib")

namespace shared { namespace security {

	using shared::sys_core::CSysError;

	class CSecurityProvider {
	public:
		 CSecurityProvider (void);
		 CSecurityProvider (const CSecurityProvider&);
		~CSecurityProvider (void);

	public:
		HRESULT    Elevate (void);       // returns S_OK if elevation is successful, otherwise error code;
		TSysErrRef Error   (void) const;
		HRESULT    GetSignature (LPCTSTR pszFilePath, TCertSignature& );
		bool       IsAccessible (LPCTSTR pszFilePath, const DWORD dwLevel);
		bool       IsAdminRole  (void) const;

	public:
		CSecurityProvider& operator = (const CSecurityProvider&); // is useless yet;

	private:
		CSysError  m_error;

	};

	enum  UACRole {
		UAC_Administrator = 0x0, // DOMAIN_ALIAS_RID_ADMINS
	};

	// https://docs.microsoft.com/en-us/openspecs/windows_protocols/ms-samr/565a6584-3061-4ede-a531-f5c53826504b <•••dwRelativeID

	class CUACCompatible {
	public:
		 CUACCompatible (void);
		 CUACCompatible (const CUACCompatible&);
		~CUACCompatible (void);

	public:
		TSysErrRef Error (void) const;
		HRESULT    IsGroup_member (const DWORD dwRelativeID, const BOOL bProcessRelative, BOOL& bIsMember_ref);
		HRESULT    IsRole(const UACRole eRole = UAC_Administrator);
		HRESULT    RunAs (const UACRole eRole = UAC_Administrator);

	public:
		CUACCompatible& operator = (const CUACCompatible&);
	private:
		CSysError  m_error;
	};
}}

typedef shared::security::CSecurityProvider  TSecProvider;
typedef shared::security::CUACCompatible     TUACCompatible;

#endif/*_SHAREDLITESYSTEMSECURITYDATAPROVIDER_H_DB811BBA_7977_433a_8C2B_E7DD2C23E543_INCLUDED*/