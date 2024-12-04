#ifndef _SVC_LOCATOR_H_E05A96E6_77F1_4A80_9B34_A998E75BEA10_INCLUDED
#define _SVC_LOCATOR_H_E05A96E6_77F1_4A80_9B34_A998E75BEA10_INCLUDED
/*
	Created by Tech_dog (ebontrop@gmail.com) on 11-Feb-2023 at 10:25:16.8896705, UTC+7, Novosibirsk, Saturday;
	This is Protocol Prying Eyes project service executable locator interface declaration file;
*/
#include "shared.sys.err.h"

namespace service { namespace man {

	using shared::sys_core::CSysError;

	class CLocator
	{
	public:
		 CLocator (void);
		 CLocator (const CLocator&);
		~CLocator (void);

	public:
		bool IsAccepted (LPCTSTR _lp_sz_path) const;  // checks the path provided for validity;
		TSysErrRef Error(void) const;

	public:
		CLocator& operator = (const CLocator&);

	private:
		mutable
		CSysError m_error;
	};
}}

#endif/*_SVC_LOCATOR_H_E05A96E6_77F1_4A80_9B34_A998E75BEA10_INCLUDED*/