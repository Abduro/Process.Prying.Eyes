#ifndef _SHARED_DEFS_H_F69675C3_3711_4BD2_A9FF_991570284E09_INCLUDED
#define _SHARED_DEFS_H_F69675C3_3711_4BD2_A9FF_991570284E09_INCLUDED
/*
	Created by Tech_dog (ebontrop@gmail.com) on 6-Feb-2023 at 20:04:13.9603379, UTC+7, Novosibirsk, Monday;
	This is Process Prying Eyes project common defenition and interface declaration file;
*/
#include "shared._std_afx.h"  // includes <tchar.h> and <string>
#include "shared.sys.err.h"

namespace shared { namespace sys_core {
	// https://en.cppreference.com/w/cpp/string/basic_string ;
	// https://stackoverflow.com/questions/258050/how-do-you-convert-cstring-and-stdstring-stdwstring-to-each-other ;

typedef ::std::basic_string<TCHAR> string_t;

#ifndef SUCCESS
#define SUCCESS(x) (((DWORD)(x)) == 0)
#endif

#ifndef FAILURE
#define FAILURE(x) (((DWORD)(x)) != 0)
#endif

	class CFormatter
	{
	public:
		 CFormatter (void) ;
		 CFormatter (const CFormatter&); // looks like unnecessary for this version;
		~CFormatter (void) ;

	public:
		LPCTSTR   Accept(LPCTSTR _lp_sz_pat, ...);
		LPCTSTR   Accept(LPCTSTR _lp_sz_pat, va_list); // TODO: placeholder count is not checked to list argument count;

	public:
		CFormatter& operator = (const CFormatter&);
	public:
		operator LPCTSTR (void) const;

	private:
		string_t m_sz_buf;  // a buffer that contains formatted string;
	};
	// https://stackoverflow.com/questions/3418231/replace-part-of-a-string-with-another-string ;
	HRESULT _replace (string_t& _content, const string_t& _s_search, const string_t& _s_replace); // has no checking error yet;
	HRESULT _replace (string_t& _content, LPCTSTR _lp_sz_search, LPCTSTR _lp_sz_replace);         // not optimized, copying input arguments occurs;
	// https://stackoverflow.com/questions/53849/how-do-i-tokenize-a-string-in-c ;
	::std::vector<string_t> _split (const string_t& _text, const string_t& _separator);
	// https://stackoverflow.com/questions/216823/how-to-trim-an-stdstring ;
	HRESULT _trim    (string_t& _content);

	class CResLoader {
	public:
		 CResLoader (void);
		~CResLoader (void);

	public:
		TSysErrRef Error (void) const;
		string_t   String(WORD _w_res_id) const;

	private:
		mutable
		CSysError  m_error;
	};
}}

#endif/*_SHARED_DEFS_H_F69675C3_3711_4BD2_A9FF_991570284E09_INCLUDED*/