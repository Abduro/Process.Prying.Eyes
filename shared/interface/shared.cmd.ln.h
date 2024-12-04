#ifndef _SHAREDGENCMDLN_H_AF7E605A_D692_4F3E_A601_DD8DB84A6516_INCLUDED
#define _SHAREDGENCMDLN_H_AF7E605A_D692_4F3E_A601_DD8DB84A6516_INCLUDED
/*
	Created by Tech_dog (ebontrop@gmail.com) on 10-Feb-2019 at 6:52:05a, UTC+7, Novosibirsk, Sunday;
	This is FIX Engine shared library configuration command line interface declaration file.
	-----------------------------------------------------------------------------
	Adopted to Process Prying Eyes on 14-Feb-2023 at 03:33:36.8866339, UTC+7, Novosibirsk, Tuesday;
*/
#include <shellapi.h>
#include "shared.classes.h"
#include "shared.sys.err.h"

namespace shared { namespace input {

	using shared::sys_core::string_t;
	using shared::sys_core::CSysError;
	using shared::sys_core::CFormatter;

	typedef ::std::map<string_t, string_t> TCmdLineArgs; // a key is argument name, a value is argument value;

	class CCommandLine {
	private:
		string_t     m_module_full_path;
		TCmdLineArgs m_args;
	public:
		 CCommandLine(void);
		~CCommandLine(void);
	public:
		HRESULT      Append(LPCTSTR _lp_sz_nm, LPCTSTR _lp_sz_val);
		string_t     Arg   (LPCTSTR _lp_sz_nm) const;
		LONG         Arg   (LPCTSTR _lp_sz_nm, const LONG _def_val)const;
		TCmdLineArgs Args  (void)const;                                  // returns a copy of command line argument collection;
		VOID         Clear (void)     ;
		INT          Count (void)const;
		bool         Has   (LPCTSTR pszArgName)const;
		string_t     ModuleFullPath(void)const;                          // must be deprecated, the id no reason to use it;
		string_t     ToString (LPCTSTR _lp_sz_sep = NULL) const;
	};

	class CArgument {
	protected:
		DWORD     m_type;
		string_t  m_name;
		string_t  m_verb;

	public:
		 CArgument (void);
		 CArgument (const CArgument&);
		 CArgument (const WORD _w_res_name, const WORD _w_res_verb, const DWORD _dw_type);
		 CArgument (LPCTSTR _lp_sz_name, LPCTSTR _lp_sz_verb, const DWORD _dw_type);
		~CArgument (void);

	public:
		bool      Is  (void) const;
		LPCTSTR   Name(void) const;
		HRESULT   Name(const WORD _w_res_id);
		HRESULT   Name(LPCTSTR _lp_sz_name );
		DWORD     Type(void) const;
		bool      Type(const DWORD);
		LPCTSTR   Verb(void) const;
		HRESULT   Verb(const WORD _w_res_id);
		HRESULT   Verb(LPCTSTR _lp_sz_desc );

	public:
		CArgument& operator = (const CArgument&);

	public:
		bool operator == (const CArgument&) const;
		bool operator == (LPCWSTR _lp_sz_name) const;
	};
}}

typedef ::std::vector<shared::input::CArgument> TArguments;

typedef shared::input::CCommandLine TCmdLine;

#endif/*_SHAREDGENCMDLN_H_AF7E605A_D692_4F3E_A601_DD8DB84A6516_INCLUDED*/