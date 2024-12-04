#ifndef _PESERVICEFAKE_H_F045BFC0_6161_418C_9E5B_E28496E20932_INCLUDED
#define _PESERVICEFAKE_H_F045BFC0_6161_418C_9E5B_E28496E20932_INCLUDED
/*
	Created by Tech_dog (ebontrop@gmail.com) on 24-Feb-2023 at 16:38:35.4817549, UTC+7, Novosibirsk, Friday;
	This is Process Prying Eyes project debug fake service interface declaration file;
*/
#include <iostream>

#include "shared.classes.h"
#include "shared.sys.err.h"
#include "shared.run.tpl.h"
#include "shared.run.event.h"

#include "PE_service_out.h"
#include "process.h"

namespace service { namespace pe {

	using shared::sys_core::CSysError  ;
	using shared::sys_core::CThreadBase;
	using shared::sys_core::CFormatter ;
	using shared::sys_core::string_t   ;
	using shared::runnable::CDelayEvent;

	using shared::process::CProcess;
	using shared::process::CProcess_List;

	class CService_Fake : public CThreadBase {
	                     typedef CThreadBase TThread;
	public:
		 CService_Fake (void);
		~CService_Fake (void);

	public:
		TSysErrRef Error (void) const;
		HRESULT    Start (void)      ;
		HRESULT    Stop  (void)      ;

	private:
		void WorkFun   (void) override final;

	private: // non-copyable;
		CService_Fake  (const CService_Fake&);
		CService_Fake& operator = (const CService_Fake&);

	private:
		CSysError   m_error;
	};

}}

#endif/*_PESERVICEFAKE_H_F045BFC0_6161_418C_9E5B_E28496E20932_INCLUDED*/