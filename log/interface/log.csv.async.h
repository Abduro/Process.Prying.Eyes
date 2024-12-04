#ifndef _GENERICSTGCSVASYNC_H_1B49AF0D_EEFB_4A0F_A2DD_A65D23EBC006_INCLUDED
#define _GENERICSTGCSVASYNC_H_1B49AF0D_EEFB_4A0F_A2DD_A65D23EBC006_INCLUDED
/*
	Created by Tech_dog (ebontrop@gmail.com) on 21-Sep-2018 at 9:53:43a, UTC+7, Novosibirsk, Friday;
	This is generic storage shared library CSV data asynchronous data provider interface declaration file.
*/
#include "shared.classes.h"
#include "shared.sys.err.h"
#include "shared.run.tpl.h"

#include "log.csv.h"

namespace shared { namespace log {

	using shared::sys_core::CSysError;
	using shared::sys_core::CThreadBase;
	using shared::sys_core::string_t;
	using shared::sys_core::CFormatter ;

	interface ICsvDataStgCallback
	{
		virtual bool    CsvStg_CanContinue(void)     PURE;                         // asks for an ability to continue data process;
		virtual VOID    CsvStg_OnComplete (void)     PURE;                         // indicates successful completeness;
		virtual VOID    CsvStg_OnError    (const CSysError) PURE;                  // sends an error object of worker thread;
		virtual HRESULT CsvStg_OnProcess  (const INT _ndx, TCsvRecord& ) PURE;     // queries a next data record;
		virtual VOID    CsvStg_OnProgress (const INT _ndx, const INT _total) PURE; // indicates a current progress;
		virtual VOID    CsvStg_OnStart    (INT& _total) PURE;                      // queries expected amount of iterations;
#if (0) // assumed that data file is already set on;
		virtual HWND    CsvStg_RequestSourceCtrl(void) PURE;                       // deprecated;
		virtual CAtlString
		                CsvStg_RequestTargetPath(void) PURE;                       // deprecated;
#endif
	};

	class CCsvStorage    : public CThreadBase {
	                      typedef CThreadBase TBase;
	private:
		ICsvDataStgCallback&   m_callee;   // callback reference;
		CSysError              m_error ;   // the last error;
		CCsvFile               m_data;     // csv file data;
		CCsvLocator            m_locator;  // csv file locator;

	public:
	    CCsvStorage(ICsvDataStgCallback&);
	   ~CCsvStorage(void);

	public:
		const
		CCsvFile&     Data (void) const  ; // data reference, read only , non-thread-safe;
		CCsvFile&     Data (void)        ; // data reference, read-write, non-thread-safe;
		TSysErrRef    Error(void) const  ;
		const
		CCsvLocator&  Locator(void) const; // data file locator reference, read-only , non-thread-safe;
		CCsvLocator&  Locator(void)      ; // data file locator reference, read-write, non-thread-safe;

	private:
		virtual VOID  WorkFun(VOID) override;
	};
}}

#endif/*_GENERICSTGCSVASYNC_H_1B49AF0D_EEFB_4A0F_A2DD_A65D23EBC006_INCLUDED*/