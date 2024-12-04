/*
	Created by Tech_dog (ebontrop@gmail.com) on 12-Jul-2020 at 5:49:05p, UTC+7, Novosibirsk, Sunday;
	This is Ebo Pack system service control event log interface implementation file.
	-----------------------------------------------------------------------------
	Adopted to Process Prying Eyes project on 13-Feb-2023 at 01:48:54.1980263, UTC+7, Novosibirsk, Monday;
*/
#include "log.journal.h"

using namespace shared::log;

/////////////////////////////////////////////////////////////////////////////

CEventLog:: CEventLog(LPCTSTR pszEventSource) : m_source(pszEventSource) {}
CEventLog::~CEventLog(void) {}

/////////////////////////////////////////////////////////////////////////////

VOID   CEventLog::WriteError(LPCTSTR pszFunction, const DWORD dwError)
{
	string_t cs_error;
	;
	this->WriteEvent(
		CFormatter().Accept(_T("%s failed with error: 0x%08lx"), pszFunction, dwError), CEventType::_value::e_error
	);
}

VOID   CEventLog::WriteEvent(LPCTSTR pszMessage , const WORD wType)
{
	LPCTSTR lpszStrings[2] = { NULL, NULL };

	HANDLE hEventSource = ::RegisterEventSource(NULL, m_source.c_str());
	if (hEventSource)
	{
		lpszStrings[0] = m_source.c_str();
		lpszStrings[1] = pszMessage;
		::ReportEvent(
			hEventSource,
			wType,
			0,
			0,
			NULL,
			_countof(lpszStrings),
			0,
			lpszStrings,
			NULL
		);

		::DeregisterEventSource(hEventSource);
		hEventSource = NULL;
	}
}