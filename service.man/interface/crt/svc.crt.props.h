#ifndef _SVCCRTPROPS_H_591361F0_1019_4464_8C7B_4CEDC79B5BDB_INCLUDED
#define _SVCCRTPROPS_H_591361F0_1019_4464_8C7B_4CEDC79B5BDB_INCLUDED
/*
	Created by Tech_dog (ebontrop@gmail.com) on 12-Feb-2023 at 05:08:13.7884053, UTC+7, Novosibirsk, Sunday;
	This is Peocess Prying Eyes project shared service create properties' interface declaration file;
*/
#include "shared.classes.h"
#include "shared.sys.err.h"
#include "shared.gen.path.h"

#include "shared.sys.err.h"
#include "shared.classes.h"

namespace service { namespace setting {

	using shared::sys_core::CSysError;
	using shared::sys_core::CGenericPath;
	using shared::sys_core::string_t;
	using shared::sys_core::CFormatter;

	class CProperty {
	public:
		 CProperty (void);
		 CProperty (const CProperty&);
		 CProperty (LPCTSTR _lp_sz_value);
		~CProperty (void);

	public:
		LPCTSTR Get(void) const;
		bool    Set(LPCTSTR _lp_sz_value); // return false if error occurred; error object can be checked for error details;

		bool    Is (void) const;  // checks a validity of a property value;

		TSysErrRef Error(void) const;

	public:
		CProperty& operator = (const CProperty&);
		CProperty& operator <<(const string_t& _value);
		CProperty& operator <<(LPCTSTR _lp_sz_value);
	
	public:
		operator LPCTSTR (void) const; // returns property value;

	public:
		static bool Is_valid(LPCTSTR _lp_sz_value);

	protected:
		string_t  m_value;
		CSysError m_error;
	};
	// ***important*** service manager sends nullptr to ::CreateService() in order to use system account by default;
	class CProp_Account : public CProperty {
	                     typedef CProperty TBase;
	public:
		 CProp_Account (void);
		~CProp_Account (void);
	public:
		bool Set(LPCTSTR _lp_sz_value);

		string_t To_string (void) const; // output: property name=property value;
	};

	class CProp_Desc : public CProperty {
	                  typedef CProperty TBase;
	public:
		 CProp_Desc (void);
		~CProp_Desc (void);
	public:
		bool Set(LPCTSTR _lp_sz_value); // service description may be empty;

		string_t To_string (void) const;
	};
		
	class CProp_Name : public CProperty { // this is a service name; can be checked for duplication during service start;
	                  typedef CProperty TBase;
	public:
		 CProp_Name (void);
		~CProp_Name (void);
	public:
		bool Set(LPCTSTR _lp_sz_value);

		string_t To_string (void) const;
	};

	class CProp_Path : public CProperty {
	                  typedef CProperty TBase;
	public:
		 CProp_Path (void);
		~CProp_Path (void);
	public:
		bool Set(LPCTSTR _lp_sz_value); // TODO: this method does not provide a check for executable binary type, i.e. any file type is accepted;

		string_t To_string (void) const;
	};

	class CProp_Title : public CProperty { // this is a service display name; can be checked for duplication during service start;
	                   typedef CProperty TBase;
	public:
		 CProp_Title (void);
		~CProp_Title (void);

	public:
		string_t To_string (void) const;
	};

	class CCrtProperties {
	public:
		 CCrtProperties (void);
		 CCrtProperties (const CCrtProperties&);
		~CCrtProperties (void);

	public:
		const
		CProp_Account& Account(void) const;
		CProp_Account& Account(void)      ;
		const
		CProp_Desc&    Desc (void) const;
		CProp_Desc&    Desc (void)      ;
		const
		CProp_Name&    Name (void) const;
		CProp_Name&    Name (void)      ;
		const
		CProp_Path&    Path (void) const;
		CProp_Path&    Path (void)      ;
		const
		CProp_Title&   Title(void) const;
		CProp_Title&   Title(void)      ;

	public:
		TSysErrRef Error (void) const;    // returns error details if one of the properties is invalid;
		bool       Is_valid (void) const; // returns true if all properties are valid;
		string_t   To_string (LPCTSTR _lp_sz_sep = _T("; ")) const;

	public:
		CCrtProperties& operator = (const CCrtProperties&);

	private:
		mutable
		CSysError     m_error;
		CProp_Account m_account;
		CProp_Desc    m_desc;
		CProp_Name    m_name;
		CProp_Path    m_path;
		CProp_Title   m_title;
	};
}}

typedef service::setting::CCrtProperties  TCrtProps;

#endif/*_SVCCRTPROPS_H_591361F0_1019_4464_8C7B_4CEDC79B5BDB_INCLUDED*/