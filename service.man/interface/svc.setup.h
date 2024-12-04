#ifndef _SYSSVCSETUP_H_20A95C2A_6F69_48CA_AA52_F61CF5AEE0C5_INCLUDED
#define _SYSSVCSETUP_H_20A95C2A_6F69_48CA_AA52_F61CF5AEE0C5_INCLUDED
/*
	Created by Tech_dog (ebontrop@gmail.com) on 12-Jul-2020 at 5:44:36a, UTC+7, Novosibirsk, Sunday;
	This is Ebo Pack system service control control setup interface declaration file.
	-----------------------------------------------------------------------------
	Adopted to Process Prying Eyes project on 13-Feb-2023 at 06:23:58.0583168, UTC+7, Novosibirsk, Monday;
*/
#include "shared.classes.h"
#include "shared.sys.err.h"

#include "svc.crt.data.h"
#include "svc.service.h"

namespace service { namespace control {

	using shared::sys_core::string_t;
	using shared::sys_core::CSysError;

	using service::setting::CStartOption;
	using service::setting::CTypeOption;

	class CSetup {
	public:
		class CState {
		public:
			enum _value {
				eNotInstalled =  0x0,
				eInstalled    =  0x1,
			};
		public:
			 CState (void);
			 CState (const _value _curr);
			 CState (const CState&);
			~CState (void);

		public:
			_value   Current (void) const;
			_value&  Current (void)      ;

			string_t AsText (void) const;

		public:
			CState& operator = (const _value  );
			CState& operator = (const CState& );
		private:
			_value   m_current;
		};
	public:
		 CSetup(void);
		 CSetup(const CSetup&);
		 CSetup(const CSetup::CState::_value _state);
		~CSetup(void);

	public:
		// https://docs.microsoft.com/en-us/windows/win32/api/winsvc/nf-winsvc-openservicew
		// https://docs.microsoft.com/en-us/windows/win32/api/winsvc/nf-winsvc-openscmanagerw
		// https://docs.microsoft.com/en-us/windows/win32/services/service-security-and-access-rights
		TSysErrRef Error  (void) const;
		DWORD      Install(const CCrtData&, CService& _result);        // install a service by calling ::CreateService() function;
		DWORD      Is     (LPCTSTR _lp_sz_svc_name) const;             // it would be better to name this property as 'installed';
		DWORD      Is     (const CCrtData&) const;
		DWORD      Is     (const CCrtData&, CService& _result) const;
		const
		CState&    State    (void) const;
		CState&    State    (void)      ;
		DWORD      UnInstall(LPCTSTR _lp_sz_svc_name);                 // deletes a service by service name provided;

	public:
		CSetup& operator = (const CSetup&);
	private:
		mutable
		CSysError m_error;
		CState    m_state;
	};

	typedef ::std::vector<CSetup> TSvcSetups;
	typedef const CSetup&       TSvcSetupRef;
#if (0)
	class CSetupEnum {
	public:
		 CSetupEnum (void);
		~CSetupEnum (void);
	public:
		INT           Count(void) const;
		const
		TSvcSetups&   Raw  (void) const;
		TSvcSetupRef  Setup(const INT nIndex) const;
	};
#endif
}}

typedef service::control::CSetup::CState TSvcSetupState;

#endif/*_SYSSVCSETUP_H_20A95C2A_6F69_48CA_AA52_F61CF5AEE0C5_INCLUDED*/