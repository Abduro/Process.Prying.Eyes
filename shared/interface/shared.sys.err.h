#ifndef _SHAREDGENERROR_H_064B9394_D422_429C_A390_948FDB26D4BC_INCLUDED
#define _SHAREDGENERROR_H_064B9394_D422_429C_A390_948FDB26D4BC_INCLUDED
/*
	Created by Tech_dog (ebontrop@gmail.com) on 09-Jul-2010 at 6:36:26p, GMT+3, Rostov-on-Don, Friday;
	This is row27 project generic error class interface declaration file;
	-----------------------------------------------------------------------------
	Adopted to Pulsepay server project on 15-May-2012 at 9:39:48pm, GMT+3, Rostov-on-Don, Tuesday;
	Adopted to FakeGPS driver project on 13-Dec-2019 at 10:19:37am, UTC+7, Novosibirsk, Friday;
	-----------------------------------------------------------------------------
	Adopted to Process Prying Eyes project on 7-Feb-2023 at 12:34:56.6978698, UTC+7, Novosibirsk, Tuesday;
*/
#include "shared.sys.sync.h"

#if defined(_UNICODE)
#define __MODULE__  __FUNCTIONW__
#else
#define __MODULE__  __FUNCTION__
#endif

#ifndef __DwordToHresult
#define __DwordToHresult(_word)  HRESULT_FROM_WIN32(_word)
#endif
#ifndef __LastErrToHresult
#define __LastErrToHresult()  __DwordToHresult(::GetLastError())
#endif

#include <algorithm> // https://en.cppreference.com/w/cpp/algorithm/ranges/find ;
#include <cwctype>   // https://en.cppreference.com/w/cpp/string/wide/iswspace  ;

// error C2666: 'shared::sys_core::CSysError::operator <<': 3 overloads have similar conversions;
// TODO: unfortunately system error constants have LONG data type that coincides with HRESULT and c++ compiler throws the error;
#define __d(x) (DWORD)(x) // a workaround is found, thus it seems like unusefull;

namespace shared { namespace sys_core {

#ifndef string_t
typedef ::std::basic_string<TCHAR> string_t;
#endif

	class CErr_Base
	{
	protected:
		 CErr_Base (void);
		 CErr_Base (const CErr_Base&);
		~CErr_Base (void);

	public:
		LPCTSTR     Desc  (void) const ;
		HRESULT     Desc  (LPCTSTR)    ;      // overrides the error description that is gotten from system service;
		LPCTSTR     Module(void) const ;      // gets module name that produces the error, if any;
		void        Module(LPCTSTR)    ;      // sets module name that produces the error;

	public:
		CErr_Base&  operator = (const CErr_Base&);
		CErr_Base&  operator = (LPCTSTR);     // sets error description;
		CErr_Base&  operator <<(LPCTSTR);     // sets error module;

	public:
		operator LPCTSTR (void) const;        // returns an error description/details; maybe it is not useful, but nevertheless;

	protected:
		CSyncObject m_lock;                   // non-copyable;
		string_t    m_desc;
		string_t    m_module;                 // actually, this is a source of an error;
	};

	class CComError : public CErr_Base {
	                 typedef CErr_Base TBase;
	public:
		 CComError (void);
		 CComError (const _com_error&);
		 CComError (const CComError&);
		~CComError (void);

	public: // https://www.merriam-webster.com/words-at-play/ie-vs-eg-abbreviation-meaning-usage-difference ;
		HRESULT Get(void) const ;        // gets error code as hresult;
		HRESULT Set(const _com_error&);  // sets com error info; TODO: this method implementation must be reviewed; returned value is not valuable;
		void    Set(const HRESULT);      // sets com error hresult;

		bool     Is_failure(void) const; // just applies FAILED macros to m_result;
		string_t To_string(LPCTSTR _lp_sz_sep = nullptr) const; // format pattern is declared internally; a separator may be provided;

	public:
		CComError& operator = (const CComError&);
		CComError& operator <<(const _com_error&);
		CComError& operator <<(HRESULT);

	public:
		operator bool (void) const;      // return true if error object result is set to failure;
		operator HRESULT (void) const;   // return result value;

	private:
		HRESULT m_result;
	};

	class CSysError : public CErr_Base {
	                 typedef CErr_Base TBase;
	public:
		 CSysError (void);
		 CSysError (const CSysError&);
		 CSysError (DWORD _dw_err_code);
		 CSysError (LPCTSTR _lp_sz_module);
		~CSysError (void);

	public:
		CErr_Base& Base(void);    // base assign operators are not visible for direct call, and compile throws the error: C2679;
		DWORD  Get (void) const ; // gets system error code;
		void   Set (const DWORD); // sets system error code;

		void   Last(void);        // gets information of the last error through calling ::GetLastError(); 

		bool     Is_failure(void) const; // returns true if error code is *not* set to {NO_ERROR|0};
		string_t To_string(LPCTSTR _lp_sz_sep = nullptr) const; // format pattern is declared internally; a separator may be provided;

	public:
		CSysError&  operator = (const CSysError&);
		CSysError&  operator <<(DWORD _err_code );
		CSysError&  operator <<(LONG  _err_code );      // it is *necessary* for cases when error code definition is assigned and is recognized as LONG;
		CSysError&  operator = (LPCTSTR _lp_sz_desc);   // otherwise, base class operator is not visible;
		CSysError&  operator <<(LPCTSTR _lp_sz_module); // otherwise, base class operator is not visible;

	public:
		operator bool   (void) const;  // return true if error code is set to win_32 system error code;
		operator DWORD  (void) const;  // returns win32 error code value;
		operator HRESULT(void) const;

	private:
		DWORD  m_err_code;
	};
}}

typedef const shared::sys_core::CComError&   TComErrRef;
typedef const shared::sys_core::CSysError&   TSysErrRef;

#endif/*_SHAREDGENERROR_H_064B9394_D422_429C_A390_948FDB26D4BC_INCLUDED*/