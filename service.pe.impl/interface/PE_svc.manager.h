#ifndef _PESVCMANAGER_H_5F24D277_FED7_4254_9E7D_C743167DB999_INCLUDED
#define _PESVCMANAGER_H_5F24D277_FED7_4254_9E7D_C743167DB999_INCLUDED
/*
	Created by Tech_dog (ebontrop@gmail.com) on 15-Feb-2023 at 08:30:45.8484037, UTC+7, Novosibirsk, Wednesday;
	This is Process Prying Eyes service manager interface declaration file;
*/
#include "shared.classes.h"
#include "shared.sys.err.h"

#include "svc.crt.data.h"
#include "svc.man.h"
#include "svc.service.h"

namespace service { namespace pe { // PE stands for Prying Eyes;

	using shared::sys_core::CFormatter;
	using shared::sys_core::CSysError;
	using shared::sys_core::string_t;

	using service::control::CCrtData;
	using service::control::CCtrlOption;
	using service::control::CSvcManager;
	using service::control::CService;

	class CPeManager : public CSvcManager {
	                  typedef CSvcManager TBase;
	public:
		 CPeManager (void);
		~CPeManager (void);

	private:
		CCrtData    m_crt;
		CService    m_svc;
	};
}}

#endif/*_PESVCMANAGER_H_5F24D277_FED7_4254_9E7D_C743167DB999_INCLUDED*/