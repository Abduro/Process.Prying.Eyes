#ifndef _CERTSTORAGE_H_A2A5C954_FD22_4F4A_B62D_70A823065285_INCLUDED
#define _CERTSTORAGE_H_A2A5C954_FD22_4F4A_B62D_70A823065285_INCLUDED
/*
	Created by Tech_dog (ebontrop@gmail.com) on 17-Dec-2014 at 0:00:00.0, UTC+3, Saint-Petersburg, Wednesday;
	This is MS Windows certificate storage wrapper interface declaration file;
	-----------------------------------------------------------------------------
	Adopted to Process Prying Eyes project on 6-Mar-2023 at 15:37:15.2089691, UTC+7, Novosibirsk, Monday;
*/
#include <wincrypt.h>
#include <wintrust.h>
#include "shared.classes.h"
#include "shared.sys.err.h"
namespace shared { namespace certificate { namespace _impl {

	using shared::sys_core::CSysError ;
	using shared::sys_core::CFormatter;
	using shared::sys_core::string_t  ;
	// there is very different meanings of words 'store' and 'storage', but I want to store something in my storage;
	// https://learn.microsoft.com/en-us/windows/win32/seccrypto/using-certificates?redirectedfrom=MSDN ;
	class CStorage {
	public:
		 CStorage (void);
		 CStorage (LPCTSTR _lp_sz_file);
		~CStorage (void);

	public:
		HRESULT    Close (void) ;
		TSysErrRef Error (void) const;
		HRESULT    Open  (LPCTSTR _lp_sz_file); // CryptQueryObject() cannot be used for opening Windows certificate store;

		bool    Is_valid (void) const;

	public:
		HCERTSTORE Handle  (void) const;
		HCRYPTMSG  Message (void) const;

	public:
		CStorage&  operator << (LPCTSTR _lp_sz_file);

	public:
		operator HCERTSTORE (void) const;
//		operator HCRYPTMSG  (void) const; // duplication of data type error is thrown by compiler occurs;

	private: // non-copyable;
		CStorage (const CStorage&);
		CStorage& operator =  (const CStorage&);

	private:
		string_t   m_file   ;
		CSysError  m_error  ;
		HCERTSTORE m_store  ;
		HCRYPTMSG  m_message;
	};

}}}

#endif/*_CERTSTORAGE_H_A2A5C954_FD22_4F4A_B62D_70A823065285_INCLUDED*/