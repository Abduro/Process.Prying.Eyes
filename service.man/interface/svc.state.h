#ifndef _SYSSVCSTATE_H_0FA4E18F_97DF_4E5F_AC7F_87CA3B0EEAC0_INCLUDED
#define _SYSSVCSTATE_H_0FA4E18F_97DF_4E5F_AC7F_87CA3B0EEAC0_INCLUDED
/*
	Created by Tech_dog (ebontrop@gmail.com) on 6-Jun-2016 at 11:25:42a, UTC+7, Phuket, Rawai, Monday;
	This is System Service Manager control state control interface declaration file. (thefileguardian.com);
	-----------------------------------------------------------------------------
	Adopter to Ebo Pack solution on 12-Jul-2020 at 8:20:33a, UTC+7, Novosibirsk, Sunday;
	-----------------------------------------------------------------------------
	Adopted to Process Prying Eyes project on 12-Feb-2023 at 09:32:19.9903007, UTC+7, Novosibirsk, Sunday;
*/
#include "shared.sys.err.h"

namespace service { namespace control {

	typedef SERVICE_STATUS* LPTSvcStat; // this is a pointer to service status structure;

	using shared::sys_core::CSysError;
	// https://softwareengineering.stackexchange.com/questions/219351/state-or-status-when-should-a-variable-name-contain-the-word-state-and-w ;
	// https://stackoverflow.com/questions/1162816/naming-conventions-state-versus-status ;
	// sets service handle and refreshes service current state data;
	class CState {
	public:
		// TODO: it very looks like the data type of this enumaration must be long; otherwise it is not recognized by a compiler:
		//       error C2679: binary '<<': no operator found which takes a right-hand operand of type 'int'
		enum value : DWORD {
			e_undef = 0x0,
			/*permanent stages*/
			e_paused    = SERVICE_PAUSED          ,  // a service is paused; permanent stage;
			e_running   = SERVICE_RUNNING         ,  // a service is running ; permanent stage;
			e_stopped   = SERVICE_STOPPED         ,  // a service is not running; permanent stage;
			/*transitives stages*/
			e_try_going = SERVICE_CONTINUE_PENDING,  // a service continue is pending; transitive stage;
			e_try_pause = SERVICE_PAUSE_PENDING   ,  // a service pause is pending; transitive stage;
			e_try_start = SERVICE_START_PENDING   ,  // a service is starting; transitive stage;
			e_try_stop  = SERVICE_STOP_PENDING    ,  // a service is stopping; transitive stage;
		};
	public:
		 CState (void);
		 CState (const CState&);
		 CState (const LPTSvcStat);
		 CState (const SC_HANDLE _p_svc_handle);
		~CState (void);

	public:
		value       Current  (void) const ;
		TSysErrRef  Error    (void) const ;
		const bool  Is       (void) const ;      // return true if the current state value is defined;
		DWORD       Query    (const SC_HANDLE _p_svc_handle); // sets service handle and refreshes service current state data;
		DWORD       Reset    (void);
		DWORD       Set      (CState::value);    // sets current state of a service; actually, sets a velue of a check point;
		DWORD       Set      (LONG _l_state);    // sets current state by values of system define that is recognized as LONG data type;
		DWORD       Synch    (LPTSvcStat)   ;    // sets a pointer to a structure which a data value is necessaty to synch with; null is acceptable;

	public:
		bool        Paused   (void) const ;
		bool        Pending  (void) const ;      // transitive state when service status control operation in pending state;
		bool        Running  (void) const ;
		bool        Stopped  (void) const ;

	public:
		CState&  operator  = (const CState&   );
		CState&  operator << (const SC_HANDLE );  // sets service handle and refreshes service current state data;
		CState&  operator << (const LPTSvcStat);  // sets a pointer to a structure a synch value is required with;
		CState&  operator << (LONG _l_state);
	public:
		operator const value (void) const;

	private:
		LPTSvcStat  m_synch;   // this is a pointer to outer service status structure; it is necessary for state current value synch;
		value       m_value;   // current state value;
		CSysError   m_error;
	};
}}

typedef       service::control::CState::value  TSvcStateValue;
typedef const service::control::CState&        TSvcStateRef;

#endif/*_SYSSVCSTATE_H_0FA4E18F_97DF_4E5F_AC7F_87CA3B0EEAC0_INCLUDED*/