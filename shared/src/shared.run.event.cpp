/*
	Created by Tech_dog (ebontrop@gmail.com) on 26-Dec-2010 at 7:21:12pm, GMT+3, Rostov-on-Don, Sunday;
	This is SFX project Asynchronous notifying/Wait Object interface implementation file.
	-----------------------------------------------------------------------------
	Adopted to Pulsepay Server project on 2-May-2012 at 1:20:12pm, GMT+3, Rostov-on-Don, Wednesday;
	Adopted to Platinum Clocking project on 19-Mar-2014 at 11:21:09am, GMT+3, Taganrog, Wednesday;
	Adopted to E-Opinion project on 8-May-2014 at 7:20:22am, GMT+3, Saint-Petersburg, Thursday;
	Adopted to Internext project on 20-Aug-2014 at 8:0*:**pm, GMT+3, Taganrog, Wednesday;
	Adopted to Fake GPS project on 11-Dec-2019 at 8:02:19p, UTC+7, Novosibirsk, Wednesday;
*/
#include "shared.run.event.h"

using namespace shared::runnable;
using namespace shared::sys_core;

/////////////////////////////////////////////////////////////////////////////
#if (0)
namespace shared { namespace runnable { namespace details
{
	class CMessageHandler:
		public ::ATL::CWindowImpl<CMessageHandler>
	{
	public:
		enum {
			eInternalMsgId = WM_USER + 1,
		};
	private:
		IGenericEventNotify&    m_sink_ref;
		const _variant_t        m_event_id;
		DWORD                   m_threadId;
	public:
		BEGIN_MSG_MAP(CMessageHandler)
			MESSAGE_HANDLER(CMessageHandler::eInternalMsgId, OnGenericEventNotify)
		END_MSG_MAP()
	private:
		virtual LRESULT  OnGenericEventNotify(UINT, WPARAM, LPARAM, BOOL&)
		{
			HRESULT hr_ = __DwordToHresult(ERROR_INVALID_STATE);
			if (FAILED(hr_)) hr_ = m_sink_ref.GenericEvent_OnNotify(m_event_id);
			if (FAILED(hr_)) hr_ = m_sink_ref.GenericEvent_OnNotify((LONG)m_event_id);
			return 0;
		}
	public:
		CMessageHandler(IGenericEventNotify& sink_ref, const _variant_t& v_evt_id):
			m_sink_ref(sink_ref),
			m_event_id(v_evt_id),
			m_threadId(::GetCurrentThreadId())
		{
		}
		virtual ~CMessageHandler(void)
		{
		}
	public:
		DWORD  _OwnerThreadId(void)const
		{
			return m_threadId;
		}
	};
}}}

#define __handler_ptr()  reinterpret_cast<details::CMessageHandler*>(this->m_handler)
/////////////////////////////////////////////////////////////////////////////

CMarshaller::CMarshaller(IGenericEventNotify& sink_ref, const _variant_t& v_evt_id) : m_handler(NULL) {
	try
	{
		m_handler = new details::CMessageHandler(sink_ref, v_evt_id);
	}
	catch(::std::bad_alloc&){}
}

CMarshaller::~CMarshaller(void) {
	Destroy();
}

/////////////////////////////////////////////////////////////////////////////

HRESULT  CMarshaller::Create(void)
{
	if (!m_handler)
		return OLE_E_BLANK;

	if (__handler_ptr()->IsWindow())
		return S_OK;

	__handler_ptr()->Create(HWND_MESSAGE);
	return (__handler_ptr()->IsWindow() ? S_OK : HRESULT_FROM_WIN32(::GetLastError()));
}

HRESULT  CMarshaller::Destroy(void)
{
	if (!m_handler)
		return OLE_E_BLANK;

	if (!__handler_ptr()->IsWindow())
		return S_OK;

	if (__handler_ptr()->DestroyWindow())
		__handler_ptr()->m_hWnd = NULL;
	else
	{
		CError err_obj; err_obj.Last();
		ATLASSERT(0);
	}
	try
	{
		delete m_handler; m_handler = NULL;
	}
	catch(...){return E_OUTOFMEMORY;}
	return S_OK;
}

HWND     CMarshaller::GetHandle_Safe(void) const
{
	if (!m_handler)
		return NULL;
	return (__handler_ptr()->IsWindow() ? __handler_ptr()->m_hWnd : NULL);
}

/////////////////////////////////////////////////////////////////////////////

HRESULT  CMarshaller::Fire(const bool bAsynch)
{
	const HWND hHandler = this->GetHandle_Safe();
	return CMarshaller::Fire(hHandler, bAsynch);
}

/////////////////////////////////////////////////////////////////////////////

HRESULT  CMarshaller::Fire2(void)
{
	const HWND  hHandler = this->GetHandle_Safe();
	if (NULL == hHandler)
		return OLE_E_INVALIDHWND;
	if (!::PostThreadMessage(
			__handler_ptr()->_OwnerThreadId(),
			details::CMessageHandler::eInternalMsgId,
			NULL,
			NULL
		))
		return __HRESULT_FROM_WIN32(::GetLastError());
	HRESULT hr_ = S_OK;
	return  hr_;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT  CMarshaller::Fire(const HWND hHandler, const bool bAsynch)
{
	if (!::IsWindow(hHandler))
		return OLE_E_INVALIDHWND;
	if (bAsynch)
		::PostMessage(hHandler, details::CMessageHandler::eInternalMsgId, (WPARAM)0, (LPARAM)0);
	else
		::SendMessage(hHandler, details::CMessageHandler::eInternalMsgId, (WPARAM)0, (LPARAM)0);
	return S_OK;
}
#endif
/////////////////////////////////////////////////////////////////////////////

CDelayEvent:: CDelayEvent(const DWORD nTimeSlice, const DWORD nTimeFrame):
	m_nTimeSlice(nTimeSlice), m_nTimeFrame(nTimeFrame), m_nCurrent(0) {
}

CDelayEvent::~CDelayEvent(void) {}

/////////////////////////////////////////////////////////////////////////////

bool CDelayEvent::Elapsed(void) const { return (m_nCurrent >= m_nTimeFrame); }
bool CDelayEvent::IsReset(void) const { return (m_nCurrent == 0); }

void CDelayEvent::Reset  (const DWORD nTimeSlice, const DWORD nTimeFrame)
{
	if (0 < nTimeSlice) m_nTimeSlice = nTimeSlice;
	if (0 < nTimeFrame) m_nTimeFrame = nTimeFrame;

	m_nCurrent = 0;
}

void CDelayEvent::Wait(void) {
	::Sleep(m_nTimeSlice);
	m_nCurrent += m_nTimeSlice;
}

/////////////////////////////////////////////////////////////////////////////

CDelayEvent& CDelayEvent::operator= (const DWORD _v) { if (_v) { m_nTimeFrame = _v; }  return *this; }