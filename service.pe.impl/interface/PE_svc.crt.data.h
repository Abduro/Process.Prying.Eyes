#ifndef _SHAREDSVCCRTDATA_H_01908365_069D_476C_BD72_EA902764752C_INCLUDED
#define _SHAREDSVCCRTDATA_H_01908365_069D_476C_BD72_EA902764752C_INCLUDED
/*
	Created by Tech_dog (ebontrop@gmail.com) on 14-Feb-2023 at 10:39:15.8755213, UTC+7, Novosibirsk, Tuesday;
	This is Process Prying Eyes project service shared create data interface declaration file;
*/
#include "shared.classes.h"
#include "shared.sys.err.h"

#include "svc.crt.data.h"

namespace service { namespace pe { // it is important do not have 'shared' namespace or sub-namespace, otherwise, a compiler complains;

	using shared::sys_core::CFormatter;
	using shared::sys_core::CSysError;
	using shared::sys_core::string_t;

	using service::setting::CCrtData;
	using service::setting::CCtrlOption;

	// ***important***: to provide a path of service executable, test client must reside in the same folder with the service file;
	//                  otherwise, system service manager will receive incorrect path of service being created;
	//            note: the above statement concerns a test case only, in other cases, service setup must make necessary step(s);
	class CIniter {
	public:
		 CIniter (void);
		~CIniter (void);
	public:
		DWORD Do (CCrtData&);  // sets create data of service;
		TSysErrRef  Error (void) const;

	private:
		CSysError   m_error;
	};

}}

#endif/*_SHAREDSVCCRTDATA_H_01908365_069D_476C_BD72_EA902764752C_INCLUDED*/