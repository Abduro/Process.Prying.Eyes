#ifndef _OUT_RECORD_H_62AA5CCD_2D4C_4D46_B1EB_CE6C8F0D5DCB_INCLUDED
#define _OUT_RECORD_H_62AA5CCD_2D4C_4D46_B1EB_CE6C8F0D5DCB_INCLUDED
/*
	Created by Tech_dog (ebontrop@gmail.com) on 11-Dec-2022 at 08:00:21.7006502, UTC+7, Novosibirsk, Sunday;
	This is system console output record interface declaration file;
	-----------------------------------------------------------------------------
	This code is the excerpt from console project of Ebo Pack solution static library code;
*/
#include "shared.classes.h"
#include "shared.sys.err.h"

#include "log.defs.h"

namespace _output {

	using shared::log::CLogType;

	class CRecordType : public CLogType {
	};
}

typedef _output::CRecordType::_value TRecordType; typedef TRecordType TRecType;

namespace _output {

	using shared::sys_core::CSysError;
	using shared::sys_core::CFormatter;
	using shared::sys_core::string_t;

	class CRecord
	{
	public:
		 CRecord (void);
		 CRecord (const CRecord&);
		 CRecord (TRecordType, LPCTSTR _lp_sz_text);
		~CRecord (void);

	public:
		LPCTSTR  Text (void) const;       // gets a text of the record;
		HRESULT  Text (LPCTSTR)   ;       // sets a text to the record; returns E_INVALIDARG in case when given text is nullptr or is empty;

		TRecType Type (void) const;
		HRESULT  Type (TRecType)  ;       // returns S_OK if current type is not equal to given value, otherwise S_FALSE is returned;

		bool  IsValid (void) const;       // checks the record text for validity;

	public:
		HRESULT    Out(void) const;       // just outputs the text, timestamp and record header;

	public:
		CRecord&   operator = (const CRecord&);
		CRecord&   operator <<(LPCTSTR _lp_sz_text);
		CRecord&   operator <<(TRecordType);

	public:
		operator LPCTSTR (void) const;
		operator TRecType(void) const; 

	protected:
		TRecordType m_rec_type;           // by default it is info record type;
		string_t    m_rec_text;           // actually, caching the message text for output is not required and must be deprecated;
	};

	class CRec_Err : public CRecord {
	                typedef CRecord TBase;
	public:
		 CRec_Err (LPCTSTR _lp_sz_text);  // calls base class error output function automatically;
		 CRec_Err (TSysErrRef);           // applies an error output template;
		~CRec_Err (void);
	};

	class CRec_Info : public CRecord {
	                 typedef CRecord TBase;
	public:
		 CRec_Info (LPCTSTR _lp_sz_text); // calls base class info output function automatically;
		~CRec_Info (void);
	};

	class CRec_Warn : public CRecord {
	                 typedef CRecord TBase;
	public:
		 CRec_Warn (LPCTSTR _lp_sz_text); // calls base class warning output function automatically;
		~CRec_Warn (void);
	};
}

typedef _output::CRecord TRecord;

#endif/*_OUT_RECORD_H_62AA5CCD_2D4C_4D46_B1EB_CE6C8F0D5DCB_INCLUDED*/