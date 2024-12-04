#ifndef _SHAREDLITEGENERICHANDLE_H_212F82EC_7E06_468a_8DEA_B2A98F4F2D6F_INCLUDED
#define _SHAREDLITEGENERICHANDLE_H_212F82EC_7E06_468a_8DEA_B2A98F4F2D6F_INCLUDED
/*
	Created by Tech_dog (ebontrop@gmail.com) on 22-Apr-2010 at 1:56:43am, Rostov-on-Don, GMT+3, Thursday;
	This is SfxSOA Base Service Memory management class(es) declaration file.
	-----------------------------------------------------------------------------
	Adopted to Platinum project on 29-Nov-2014 at 0:42:43am, GMT+3, Taganrog, Saturday;
	Adopted to File Guardian project on 27-May-2018 at 3:21:42p, UTC+7, Phuket, Rawai, Sunday; (thefileguardian.com)
*/
#include "shared.sys.err.h"
#include "shared.sys.sync.h"
/*
namespace SfxSOA { namespace Base { namespace System { namespace Memory {*/
namespace shared { namespace common {
	using shared::sys_core::CSysError;
	// https://docs.microsoft.com/en-us/windows/win32/api/handleapi/nf-handleapi-duplicatehandle
	class CAutoHandle {
	public:
		 CAutoHandle(void);
	explicit
		 CAutoHandle(const CAutoHandle&); // duplicates a handle;
		 CAutoHandle(const HANDLE);       // assigns a handle; it is not truth: trying to make a duplicate also;
		~CAutoHandle(void);

	public:
		CAutoHandle& operator = (const CAutoHandle&); // duplicates a handle;
		CAutoHandle& operator = (const HANDLE);       // assigns a handle; duplicates a handle too, mother fucking;

	public:
		operator   HANDLE(void);
		operator   HANDLE(void) const;

	public:
		PHANDLE    operator&(void);

	public:
		HRESULT    Clone (const HANDLE); // no given handle verification is done; duplication the handle is made anyway;
		HANDLE     Handle(void) const;
		bool       Is    (void) const;
		void       Reset (void);

	public:
		HRESULT    Attach(HANDLE);       // input handle varification is done, but no duplication is made, just assigning;

	protected:
		HANDLE     m_handle;
	};

	class CAutoHandleArray {
	public:
	    CAutoHandleArray(const DWORD dwSize);
	   ~CAutoHandleArray(void);

	public:
		TSysErrRef Error  (void)const;
		bool       Is     (void)const;
		PHANDLE    Handles(void)const;
		DWORD      Size   (void)const;
	public:
		HANDLE     operator[] (const INT) const;
		HANDLE&    operator[] (const INT)      ;
	private:
		PHANDLE    m_handles;
		DWORD      m_size;
		CSysError  m_error;
	private: // non-copyable;
		CAutoHandleArray(const CAutoHandleArray&);
		CAutoHandleArray& operator= (const CAutoHandleArray&);
	};

	using shared::sys_core::CSyncObject;

	//
	// TODO: actually we need the handle with reference counting, otherwise, protected access doesn't protect from
	//       destroying the handle while it is still used in one of the worker threads;
	//
	class CHandleSafe {
	public:
		 CHandleSafe(const HANDLE = INVALID_HANDLE_VALUE);
		~CHandleSafe(void);
	public:
		CHandleSafe& operator=(const HANDLE);
	public:
		operator    HANDLE(void);
		operator    HANDLE(void) const;
	public:
		PHANDLE     operator&(void);
	private:
		CHandleSafe(const CHandleSafe&);
		CHandleSafe& operator= (const CHandleSafe&);
	public:
		HANDLE      Handle (void) const;
		bool        Is     (void) const;
		VOID        Reset  (void);
	private:
		HANDLE      m_handle  ;
		CSyncObject m_sync_obj;
	};

	class CGlobalAlloca {
	public:
	    CGlobalAlloca(const PVOID _data, const DWORD _size, const bool bMovable = true);
	   ~CGlobalAlloca(void);
	public:
		LPVOID      Data   (void) const;
		HGLOBAL     Detach (void)      ;
		TSysErrRef  Error  (void) const;
		HGLOBAL     Handle (void) const;
		bool        IsValid(void) const;
		DWORD       Size   (void) const;
	private: // non-copyable;
		CGlobalAlloca(const CGlobalAlloca&);
		CGlobalAlloca& operator= (const CGlobalAlloca&);
	private:
		mutable
		CSysError   m_error;
		HGLOBAL     m_global;    // global memory block handle;
		DWORD       m_dwSize;    // global memory block size;
		LPVOID      m_pData;     // a pointer to global memory block to get access to;
		bool        m_bMovable;
	};

	class CStdHandle : public CAutoHandle {
	                  typedef CAutoHandle THandle;
	public:
		 CStdHandle (const DWORD _handle_type);
		~CStdHandle (void);

	public:
		HRESULT     Close(void)      ;
		TSysErrRef  Error(void) const;
		DWORD       Type (void) const;
	public:
		CStdHandle& operator=(const HANDLE);
	protected:
		DWORD       m_std_type;
		CSysError   m_error;
	};
}}/*
}}}}*/
#endif/*_SHAREDLITEGENERICHANDLE_H_212F82EC_7E06_468a_8DEA_B2A98F4F2D6F_INCLUDED*/