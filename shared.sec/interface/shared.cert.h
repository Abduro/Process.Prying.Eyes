#ifndef _SHAREDCERTIFICATEDATAPROVIDER_H_DB811BBA_7977_433a_8C2B_E7DD2C23E544_INCLUDED
#define _SHAREDCERTIFICATEDATAPROVIDER_H_DB811BBA_7977_433a_8C2B_E7DD2C23E544_INCLUDED
/*
	Created by Tech_dog (ebontrop@gmail.com) on 17-Dec-2014 at 00:00:00.0, UTC+3, Saint-Petersburg, Wednesday;
	This is Shared Lite Library System Certificate Data Provider interface declaration file.
	-----------------------------------------------------------------------------
	Adopted to Process Prying Eyes project on 2-Mar-2023 at 22:52:00.4909083, UTC+7, Novosibirsk, Thursday;
*/
#include <wincrypt.h>
#include <wintrust.h>
#include <softpub.h>  // wintrust constant is defined it this header (guids);

#include "shared.classes.h"
#include "shared.sys.err.h"
#include "shared.raw.buf.h"
#include "shared.gen.hand.h"

#pragma comment(lib, "crypt32.lib")
#pragma comment(lib, "wintrust")

namespace shared { namespace certificate
{
	using shared::sys_core::string_t  ;
	using shared::sys_core::CSysError ;
	using shared::sys_core::CFormatter;

	using shared::common::CRawData    ;
	using shared::common::CAutoHandle ;

	class CSubjectInfo
	{
		typedef ::std::vector<string_t> TValueArray;
		enum eFieldIndex{ eName = 0, eCountry, eLocation, eState, eAddress, eCompany, e_Count = eCompany + 1 };
	public:
		 CSubjectInfo(void);
		~CSubjectInfo(void);

	public:
		LPCTSTR   Address  (void) const;
		HRESULT   Address  (LPCTSTR, const bool bAppend = false);
		LPCTSTR   Company  (void) const;
		HRESULT   Company  (LPCTSTR)   ;
		LPCTSTR   Country  (void) const;
		HRESULT   Country  (LPCTSTR)   ;
		LPCTSTR   Location (void) const;
		HRESULT   Location (LPCTSTR)   ;
		HRESULT   ParseText(LPCTSTR)   ;
		LPCTSTR   Name     (void) const;
		HRESULT   Name     (LPCTSTR)   ;
		LPCTSTR   State    (void) const;
		HRESULT   State    (LPCTSTR)   ;
	private:
		string_t  m_values[CSubjectInfo::e_Count];
	};

	class CSignatureTime {
	public:
		  CSignatureTime (void);
		  CSignatureTime (const CSignatureTime&);
		  CSignatureTime (const SYSTEMTIME&);
		 ~CSignatureTime (void);

	public:
		bool    Is_valid (void) const;
		const
		SYSTEMTIME& Time (void) const;
		HRESULT     Time (const SYSTEMTIME&);
		LONGLONG    TimeAsUnix(void) const;

	public:
		CSignatureTime& operator = (const CSignatureTime&);
		CSignatureTime& operator <<(const SYSTEMTIME&);

		bool operator > (const CSignatureTime&) const;
		bool operator < (const CSignatureTime&) const;
		bool operator ==(const CSignatureTime&) const;

	public:
		operator const SYSTEMTIME& (void) const;

	private:
		SYSTEMTIME  m_time;
	};

}}

typedef shared::certificate::CSubjectInfo   TCertSubject;
typedef shared::certificate::CSignatureTime TCertTime;

namespace shared { namespace certificate {

	class CCertPeriod {
	public:
		 CCertPeriod (void);
		 CCertPeriod (const CCertPeriod&);
		~CCertPeriod (void);

	public:
		const
		TCertTime&  From (void) const;
		TCertTime&  From (void)      ;
		const
		TCertTime&  Upto (void) const;
		TCertTime&  Upto (void)      ;

	public:
		bool Is_valid (void) const;

	public:
		CCertPeriod& operator = (const CCertPeriod&);
		CCertPeriod& operator <<(const TCertTime&);   // from;
		CCertPeriod& operator >>(const TCertTime&);   // upto;

	private:
		TCertTime m_from;
		TCertTime m_upto;
	};

	class CSignatureInfo {
	public:
		 CSignatureInfo (void);
		 CSignatureInfo (const CSignatureInfo&);
		~CSignatureInfo (void);

	public:
		LPCTSTR        Issuer (void) const;
		HRESULT        Issuer (LPCTSTR);

		bool           Is_valid (void) const;
		const
		CCertPeriod&   Period (void) const;
		CCertPeriod&   Period (void)      ;
		
		LPCTSTR        SerialNumber(void) const;
		HRESULT        SerialNumber(LPCTSTR);
		const
		CSubjectInfo&  Signer(void) const;
		CSubjectInfo&  Signer(void)      ;

	public:
		CSignatureInfo& operator = (const CSignatureInfo&);

	private:
		string_t       m_issuer;
		string_t       m_serial;
		CSubjectInfo   m_signer;
		CCertPeriod    m_period;
	};
}}

typedef shared::certificate::CSignatureInfo TCertInfo; typedef TCertInfo TCertSignature;

namespace shared { namespace certificate {

	class CCertProvider {
	public:
		 CCertProvider (void);
		 CCertProvider (const CCertProvider&);
		 CCertProvider (LPCTSTR _lp_sz_file );
		~CCertProvider (void);

	public:
		TSysErrRef     Error(void) const;
		const
		TCertInfo&     Get  (void) const;
		HRESULT        Get  (LPCTSTR _lp_sz_file) ; // does not work and is not used;

		bool     Is_Trusted (LPCTSTR _lp_sz_file) ;

	public:
		CCertProvider& operator = (const CCertProvider&);
		CCertProvider& operator <<(LPCTSTR _lp_sz_file );

		operator const TCertInfo& (void) const;

	private:
		CSysError      m_error;
		CSignatureInfo m_cert ;
	};
}}

typedef shared::certificate::CCertProvider TCertProvider;

#endif/*_SHAREDCERTIFICATEDATAPROVIDER_H_DB811BBA_7977_433a_8C2B_E7DD2C23E544_INCLUDED*/