#ifndef _OUTCONRES_H_CA39BB13_43E1_4DF7_98D4_4E0E32EA6211_INCLUDED
#define _OUTCONRES_H_CA39BB13_43E1_4DF7_98D4_4E0E32EA6211_INCLUDED
/*
	Created by Tech_dog (ebontrop@gmail.com) on 21-Feb-2023 at 11:18:31.5718994, UTC+7, Novosibirsk, Tuesday;
	This is Process Prying Eyes project console resource interface declaration file;
*/
#include "shared.sys.err.h"

namespace _output {

	using shared::sys_core::CSysError;

	class CIcon {
	public:
		 CIcon (void);
		 CIcon (const CIcon&);
		~CIcon (void);
	public:
		TSysErrRef Error (void) const;
		HICON Load (const UINT nIconResId, const bool bTreatAsLargeIcon); // when an icon is loaded, it must be assigned to a window;
	public:
		CIcon& operator = (const CIcon&); // looks like useless;
	private:
		CSysError m_error;
	};

}

#endif/*_OUTCONRES_H_CA39BB13_43E1_4DF7_98D4_4E0E32EA6211_INCLUDED*/