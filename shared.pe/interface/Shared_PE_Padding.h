#ifndef _SHAREDLITELIBRARYPEFILEPADDING_H_7E6E5FC3_E191_4f3f_9604_5C31A19523BF_INCLUDED
#define _SHAREDLITELIBRARYPEFILEPADDING_H_7E6E5FC3_E191_4f3f_9604_5C31A19523BF_INCLUDED
/*
	Created by Tech_dog (ebontrop@gmail.com) on 8-Oct-2014 at 20:32:46.9740868, UTC+3, Taganrog, Wednesday;
	This is Shared Lite Library PE (portable executable) common file padding interface declaration file.
*/
#include "shared.classes.h"
#include "shared.raw.buf.h"

namespace shared { namespace sys_file
{
	using shared::common::CRawData;
	using shared::sys_core::string_t;
	using shared::sys_core::CResLoader;

	class CPeFilePadding {
	public:
		 CPeFilePadding(void);
		~CPeFilePadding(void);
	public:
		const CRawData&   CloseTag(void)const;
		HRESULT           CloseTag(const UINT resId);
		const CRawData&   CreatePadBuffer(void);
		const CRawData&   CreatePadBuffer(LPCTSTR pszStartTag, LPCTSTR pszEndTag);
		const CRawData&   ExtractData(const CRawData& raw_buffer);
		const CRawData&   OpenTag(void)const;
		HRESULT           OpenTag(const UINT resId);
		void              Reset(void);
	public:
		static  DWORD     BufferSize(void); // in bytes ;
	private:
		CRawData          m_pad_buffer;
		CRawData          m_tag_close;
		CRawData          m_tag_open;
	};
}}

#endif/*_SHAREDLITELIBRARYPEFILEPADDING_H_7E6E5FC3_E191_4f3f_9604_5C31A19523BF_INCLUDED*/