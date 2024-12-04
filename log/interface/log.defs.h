#ifndef _SHAREDLOGCOMMONDEFS_H_54D16A79_8A77_47fe_AB9A_4C1B57F4F3E7_INCLUDED
#define _SHAREDLOGCOMMONDEFS_H_54D16A79_8A77_47fe_AB9A_4C1B57F4F3E7_INCLUDED
/*
	Created by Tech_dog (ebontrop@gmail.com) on 11-Jul-2015 at 12:32:18pm, GMT+7, Phuket, Rawai, Saturday;
	This is Shared Log Library Common Definition declaration file.
	-----------------------------------------------------------------------------
	Adopted to Process Prying Eyes project on 22-Feb-2023 at 12:22:47.8597140, UTC+7, Novosibirsk, Wednessay;
*/
#include "shared.classes.h"
#include "shared.sys.err.h"
namespace shared { namespace log
{
	using shared::sys_core::CSysError;
	using shared::sys_core::string_t;
	using shared::sys_core::CFormatter;

	class CLogType {
	public:
		enum _value {
			eInfo    = 0,
			eWarning = 1,
			eError   = 2,
			eWaiting = 3,
		};
	};

	class CLogOption {
	public:
		enum _target {
			eNone       = 0x0,
			eUseJournal = 0x1, // system event journal;
			eUseFile    = 0x2  // file logger;
		};
	};

	interface ILogGenericCallback
	{
		virtual VOID ILog_OnGenericEvent(const CLogType::_value, string_t _evt_details) PURE;
	};
}}

#endif/*_SHAREDLOGCOMMONDEFS_H_54D16A79_8A77_47fe_AB9A_4C1B57F4F3E7_INCLUDED*/