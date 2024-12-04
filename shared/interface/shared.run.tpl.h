#ifndef _SHAREDSYSTEMTHREADPOOL_H_88D00B0A_6D7B_4a9c_BBDE_3A546C279DEE_INCLUDED
#define _SHAREDSYSTEMTHREADPOOL_H_88D00B0A_6D7B_4a9c_BBDE_3A546C279DEE_INCLUDED
/*
 *******************************************************************************
 * Copyright (c) Microsoft Corporation.                                        *
 *                                                                             *
 * The class was designed by Kenny Kerr. It provides the ability to queue      *
 * simple member functions of a class to the Windows thread pool.              *
 *                                                                             *
 * Kenny Kerr spends most of his time designing and building distributed       *
 * applications for the Microsoft Windows platform. He also has a particular   *
 * passion for C++ and security programming. Reach Kenny at                    *
 * http://weblogs.asp.net/kennykerr/ or visit his Web site:                    *
 * http://www.kennyandkarin.com/Kenny/.                                        *
 *                                                                             *
 * This source is subject to the Microsoft Public License.                     *
 * See http://www.microsoft.com/en-us/openness/resources/licenses.aspx#MPL.    *
 * All other rights reserved.                                                  *
 *                                                                             *
 * THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND, * 
 * EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED       *
 * WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR PURPOSE.      *
 *                                                                             *
 *-----------------------------------------------------------------------------*
 *  This is Ebo Pack shared system thread pool interface declaration file.     *
 *-----------------------------------------------------------------------------*
 *                                                                             *
 * Adopted and extended by Tech_dog (ebontrop@gmail.com)                       *
 * on 21-Sep-2015 at 12:48:06am, GMT+7, Phuket, Rawai, Monday;                 *
 *-----------------------------------------------------------------------------*
 *                                                                             *
 *******************************************************************************
*/
#pragma warning(push)
#pragma warning(disable:4995)
#include <memory>
#pragma warning(pop)

#include "shared.sys.err.h"
#include "shared.sys.sync.h"

namespace shared { namespace sys_core
{
	class CThreadType {
	public:
		enum {
			eDefault  = WT_EXECUTEDEFAULT     ,
			eIOThread = WT_EXECUTEINIOTHREAD  ,
			eUIThread = WT_EXECUTEINUITHREAD  ,
			eLongOper = WT_EXECUTELONGFUNCTION,
		};
	};
	// https://docs.microsoft.com/en-us/windows/win32/api/threadpoollegacyapiset/
	// https://docs.microsoft.com/en-us/windows/win32/api/threadpoollegacyapiset/nf-threadpoollegacyapiset-queueuserworkitem
	class CThreadPool {
	public:
		template <typename T>
		static BOOL QueueUserWorkItem(
					void (T::*pfn)(void), 
					T* pObject,
					ULONG flags = CThreadType::eLongOper
				)
		{
			typedef std::pair<void (T::*)(), T*> CallbackType;
			std::auto_ptr<CallbackType> ptr_(new CallbackType(pfn, pObject));

			if (::QueueUserWorkItem(ThreadProc<T>, ptr_.get(), flags))
			{
				ptr_.release();
				return TRUE;
			}
			else
				return FALSE;
		}

		template <typename T>
		static BOOL QueueUserWorkItemEx(
					void (T::*pfn)(T*),
					T* pObject,
					ULONG flags = CThreadType::eLongOper
				)
		{
			typedef std::pair<void (T::*)(T*), T*> CallbackType;
			std::auto_ptr<CallbackType> ptr_(new CallbackType(pfn, pObject));

			if (::QueueUserWorkItem(ThreadProcEx<T>, ptr_.get(), flags))
			{
				ptr_.release();
				return TRUE;
			}
			else
				return FALSE;
		}
	private:
		template <typename T>
		static DWORD WINAPI ThreadProc(PVOID context)
		{
			typedef std::pair<void (T::*)(), T*> CallbackType;

			std::auto_ptr<CallbackType> p(static_cast<CallbackType*>(context));

			(p->second->*p->first)();
			return 0;
		}

		template <typename T>
		static DWORD WINAPI ThreadProcEx(PVOID context)
		{
			typedef std::pair<void (T::*)(T*), T*> CallbackType;

			std::auto_ptr<CallbackType> ptr_(static_cast<CallbackType*>(context));

			(ptr_->second->*ptr_->first)(ptr_->second);
			return 0;
		}
	};

	class CThreadCrtDataBase {
	public:
		virtual
		~CThreadCrtDataBase(void);
		 CThreadCrtDataBase(void);
		
	public:
		const
		HANDLE&   EventObject(void) const;
		HANDLE&   EventObject(void);
		HRESULT   Initialize (void);
		bool      IsStopped  (void) const;
		void      IsStopped  (const bool);
		bool      IsValid    (void) const;  // checks stop event for valid value;
		HRESULT   Terminate  (void);

	private: // non-copyable;
		CThreadCrtDataBase(const CThreadCrtDataBase&);
		CThreadCrtDataBase& operator= (const CThreadCrtDataBase&);

	protected:
		volatile 
		bool      m_bStopped;
		HANDLE    m_hStopEvent;
	};

	class CThreadCrtData : public CThreadCrtDataBase {
	                      typedef CThreadCrtDataBase TBase;
	public:
		 CThreadCrtData(void);
		~CThreadCrtData(void);
	};

	class CThreadState {
	public:
		enum {
			eStopped   =  0x0,
			eError     =  0x1,
			eWorking   =  0x2,
		};
	};

	using shared::sys_core::CSysError;
	using shared::sys_core::CSyncObject;

	class CThreadBase {
	protected:
		CThreadCrtData  m_crt ;
		CSyncObject     m_lock;

	protected:
		volatile
		DWORD           m_state;

	protected:
		 CThreadBase(void);
		~CThreadBase(void);

	public:
		virtual bool    IsComplete(void)const;
		virtual HRESULT MarkToStop(void);
		virtual HRESULT Start(void);
		virtual HRESULT Stop (DWORD _dw_wait_msec = INFINITE);

	public:
		const bool      IsRunning(void) const;
		const bool      IsValid  (void) const;
		const DWORD     State    (void) const;

	private:
		virtual void    WorkFun  (void) PURE; // derived class must provide thread function implementation;
	};
}}

typedef shared::sys_core::CThreadCrtData  TThreadCrtData;
typedef shared::sys_core::CThreadBase     TThreadBase;

#endif/*_SHAREDSYSTEMTHREADPOOL_H_88D00B0A_6D7B_4a9c_BBDE_3A546C279DEE_INCLUDED*/