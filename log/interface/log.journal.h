#ifndef _SYSSVCLOG_H_3A52566E_BC09_4E0C_9161_6EE6BAF855F0_INCLUDED
#define _SYSSVCLOG_H_3A52566E_BC09_4E0C_9161_6EE6BAF855F0_INCLUDED
/*
	Created by Tech_dog (ebontrop@gmail.com) on 12-Jul-2020 at 5:49:05p, UTC+7, Novosibirsk, Sunday;
	This is Ebo Pack solution system service control event log interface declaration file.
	-----------------------------------------------------------------------------
	Adopted to Process Prying Eyes project on 13-Feb-2023 at 01:44:01.6023592, UTC+7, Novosibirsk, Monday;
*/
#include "shared.classes.h"

namespace shared { namespace log {

	using shared::sys_core::string_t;
	using shared::sys_core::CFormatter;

	class CEventType {
	public:
		enum _value : WORD {
			e_error   = EVENTLOG_ERROR_TYPE,
			e_info    = EVENTLOG_INFORMATION_TYPE,
			e_warning = EVENTLOG_WARNING_TYPE,
		};
	};

	class CEventLog {
	private:
		string_t   m_source;
	public:
		 CEventLog(LPCTSTR pszEventSource);
		~CEventLog(void);
	public:
		VOID   WriteError(LPCTSTR pszFunction, const DWORD dwError = ::GetLastError());
		VOID   WriteEvent(LPCTSTR pszMessage , const  WORD wType);
	};

}}

#endif/*_SYSSVCLOG_H_3A52566E_BC09_4E0C_9161_6EE6BAF855F0_INCLUDED*/