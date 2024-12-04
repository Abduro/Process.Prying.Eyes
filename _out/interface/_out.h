#ifndef _OUT_H_772D4B6B_9C52_4A87_BB29_38686C75EE2A_INCLUDED
#define _OUT_H_772D4B6B_9C52_4A87_BB29_38686C75EE2A_INCLUDED
/*
	Created by Tech_dog (ebontrop@gmail.com) on 10-Sep-2020 at 6:54:34p, UTC+7, Novosibirsk, Thursday;
	This is FakeGPS driver version 2 desktop client console interface declaration file;
	-----------------------------------------------------------------------------
	Adopted to Process Prying Eyes project on 14-Feb-2023 at 06:09:01.5556913, UTC+7, Novosibirsk, Tuesday;
*/
#include "_out.con.h"

namespace _output {

	class CPE_Out : public CConsole {
	               typedef CConsole TBase;
	public:
		 CPE_Out (void);
		~CPE_Out (void);
	};

}

typedef _output::CPE_Out out_;

static out_ _out;

#endif/*_OUT_H_772D4B6B_9C52_4A87_BB29_38686C75EE2A_INCLUDED*/