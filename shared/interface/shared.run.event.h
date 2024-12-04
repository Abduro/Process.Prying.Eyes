#ifndef _SHAREDLITELIBGENERICEVENT_H_DF255B0D_D53A_4917_B1EC_8F7D231AA515_INCLUDED
#define _SHAREDLITELIBGENERICEVENT_H_DF255B0D_D53A_4917_B1EC_8F7D231AA515_INCLUDED
/*
	Created by Tech_dog (ebontrop@gmail.com) 26-Dec-2010 at 7:00:02pm, GMT+3, Rostov-on-Don, Sunday;
	This is SFX project Asynchronous notifying/waiting object interface declartion file.
	-----------------------------------------------------------------------------
	Adopted to Pulspay Server project on 2-May-2012 at 1:09:18pm, GMT+3, Rostov-on-Don, Wednesday;
	Adopted to Platinum Clocking project on 19-Mar-2014 at 11:21:09am, GMT+3, Taganrog, Wednesday;
	Adopted to E-Opinion project on 8-May-2014 at 7:17:15am, GMT+3, Saint-Petersburg, Thursday;
	Adopted to Internext project on 20-Aug-2014 at 8:03:27pm, GMT+3, Taganrog, Wednesday;
	Adopted to Fake GPS project on 11-Dec-2019 at 7:59:09p, UTC+7, Novosibirsk, Wednesday;
*/
#include "shared.sys.err.h"
namespace shared { namespace runnable {

	using shared::sys_core::CSysError ;
#if (0)
	interface IGenericEventNotify
	{
		virtual HRESULT  GenericEvent_OnNotify(const _variant_t v_evt_id)  {v_evt_id; return E_NOTIMPL; }
		virtual HRESULT  GenericEvent_OnNotify(const LONG n_evt_id) PURE;
		virtual HRESULT  GenericEvent_OnNotify(const LONG n_evt_id, const _variant_t v_data) {n_evt_id; v_data; return E_NOTIMPL; }
	};
#endif
#if (0)
	class CMarshaller
	{
	protected:
		HANDLE   m_handler;
	public:
		CMarshaller(IGenericEventNotify&, const _variant_t& v_evt_id);
		virtual ~CMarshaller(void);
	public:
		virtual HRESULT  Create (void);
		virtual HRESULT  Destroy(void);
		virtual HWND     GetHandle_Safe(void) const;
	private:
		CMarshaller(const CMarshaller&);
		CMarshaller& operator= (const CMarshaller&);

	public:
		virtual HRESULT  Fire (const bool bAsynch = true);
		virtual HRESULT  Fire2(void);

	public:
		static  HRESULT  Fire (const HWND hHandler, const bool bAsynch = true);

	};
#endif
	class CDelayEvent
	{
	public:
		enum _frm : DWORD {
			e_na      = 0,
			eInfinite = (DWORD)-1
		};
	protected:
		volatile DWORD     m_nTimeSlice;    // time space in milliseconds;
		volatile DWORD     m_nTimeFrame;    // total time to wait for;
	protected:
		DWORD    m_nCurrent;                // current time;
	public:
		CDelayEvent(const DWORD nTimeSlice, const DWORD nTimeFrame);
		virtual ~CDelayEvent(void);
	public:
		virtual bool Elapsed(void) const;
		virtual bool IsReset(void) const;
		virtual void Reset  (const DWORD nTimeSlice = _frm::e_na, const DWORD nTimeFrame = _frm::e_na);
		virtual void Wait   (void);

	public:
		CDelayEvent& operator= (const DWORD) ; // sets time frame value for waiting for;
	};

}}

#endif/*_SHAREDLITELIBGENERICEVENT_H_DF255B0D_D53A_4917_B1EC_8F7D231AA515_INCLUDED*/