#ifndef _SHAREDSECFILEACC_H_09D5571A_DC9E_4BE6_9538_A14DEAE094D2_INCLUDED
#define _SHAREDSECFILEACC_H_09D5571A_DC9E_4BE6_9538_A14DEAE094D2_INCLUDED
/*
	Created by Tech_dog (ebontrop@gmail.com) on 4-Feb-2015 at 00:00:00.0a, UTC+3, Taganrog, Wednesday;
	This is Web setup project shared library generic file security access interface declaration file; (Internext Ltd, RO; closed permanently);
	-----------------------------------------------------------------------------
	Adopted to Ebo Pack security project on 22-Jan-2021 at 01:48:48.836 am, UTC+7, Novosibirsk, Friday;
	Adopted to Process Prying Eyes project on 10-Mar-2023 at 06:08:33.2557821, UTC+7, Novosibirsk, Friday;
*/
#include <aclapi.h>
#include "shared.classes.h"
#include "shared.sys.err.h"
#include "shared.gen.hand.h"

namespace shared { namespace security {

	using shared::sys_core::CSysError;
	using shared::common::CAutoHandle;
	using shared::sys_core::string_t ;

	typedef PSECURITY_DESCRIPTOR PSEC_DESC;

	// https://docs.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-createfilew   ;
	// https://docs.microsoft.com/en-us/windows/win32/api/aclapi/nf-aclapi-getsecurityinfo ;
	// https://docs.microsoft.com/en-us/windows/win32/secauthz/finding-the-owner-of-a-file-object-in-c-- ;
	class CFileAccess {		
	public:
		 CFileAccess (void);
		 CFileAccess (const CFileAccess&);
		 CFileAccess (LPCTSTR _lp_sz_file_path);
		~CFileAccess (void);

	public:
		const PACL  ControlList (void) const;
		const
		PSEC_DESC   Descriptor  (void) const;
		TSysErrRef  Error       (void) const;
		bool        Is   (void) const;                      // does not indicate file access, but just returns true if already initialized;
		LPCTSTR     Path (void) const;
		HRESULT     Path (LPCTSTR _lp_sz_file_path );       // creates/resets all internal attributes/fields of this class;
		HRESULT     Reset(void)      ;                      // resets/destroys internal pointers; especially, security desciptor pointer;

	public:
		CFileAccess& operator = (const CFileAccess&);       // actually this assign operator works as move one: no duplication of security data;
		CFileAccess& operator <<(LPCTSTR _lp_sz_file_path); // creates/resets all internal attributes/fields of this class;

	private:
		PSEC_DESC   m_psd  ;
		CAutoHandle m_file ;
		PACL        m_pdacl; // discretionary access control list;
		CSysError   m_error;
		string_t    m_path ;
	};

}}

typedef shared::security::CFileAccess  TFileAccess;

#endif/*_SHAREDSECFILEACC_H_09D5571A_DC9E_4BE6_9538_A14DEAE094D2_INCLUDED*/