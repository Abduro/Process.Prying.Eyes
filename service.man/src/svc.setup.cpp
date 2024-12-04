/*
	Created by Tech_dog (ebontrop@gmail.com) on 12-Jul-2020 at 5:58:58a, UTC+7, Novosibirsk, Sunday;
	This is Ebo Pack system service control control setup interface implementattion file.
	-----------------------------------------------------------------------------
	Adopted to Process Prying Eyes project on 13-Feb-2023 at 06:34:01.4277232, UTC+7, Novosibirsk, Monday;
*/
#include "svc.setup.h"
#include "svc.man.h"

using namespace service::control;

/////////////////////////////////////////////////////////////////////////////

CSetup::CState:: CState(void) : m_current(_value::eNotInstalled) {}
CSetup::CState:: CState(const _value _curr) : m_current(_curr) {}
CSetup::CState:: CState(const CState& _ref) : CState() { *this = _ref; }
CSetup::CState::~CState(void) {}

/////////////////////////////////////////////////////////////////////////////

CSetup::CState::_value CSetup::CState::Current(void) const { return m_current; }
CSetup::CState::_value&CSetup::CState::Current(void)       { return m_current; }

string_t CSetup::CState::AsText (void) const {

	switch (m_current)
	{
	case _value::eInstalled   : return string_t(_T("Installed"));
	case _value::eNotInstalled: return string_t(_T("Uninstalled")); break;
	}

	return string_t(_T("#n/a"));
}
/////////////////////////////////////////////////////////////////////////////

CSetup::CState& CSetup::CState::operator = (const _value _curr) { this->Current() = _curr; return *this; }
CSetup::CState& CSetup::CState::operator = (const CState& _ref) {
	this->Current() = _ref.Current();
	return *this;
}

/////////////////////////////////////////////////////////////////////////////

CSetup:: CSetup(void) { m_error << __MODULE__ << S_OK; }
CSetup:: CSetup(const CSetup& _ref) : CSetup() { *this = _ref; }
CSetup:: CSetup(const CSetup::CState::_value _state) : CSetup() { m_state = _state; }
CSetup::~CSetup(void) {}

/////////////////////////////////////////////////////////////////////////////

TSysErrRef CSetup::Error (void) const { return m_error; }

DWORD      CSetup::Install(const CCrtData& crt_ref, CService& svc_ref) {
		m_error << __MODULE__ << S_OK;

		if (crt_ref.Is_valid() == false)
			return (m_error = crt_ref.Error());

		if (svc_ref.Is())
			return (m_error << ERROR_ALREADY_INITIALIZED);

		CSvcManager_Handle man_hand;
		DWORD d_result = man_hand.Connect(STANDARD_RIGHTS_READ);
		if (FAILURE(d_result))
			return (m_error = man_hand.Error());

		svc_ref << ::CreateService(
			man_hand,
			crt_ref.Properties().Name(),
			crt_ref.Properties().Name(), /*crt_ref.Properties().Title()*/
			SERVICE_ALL_ACCESS           /*SERVICE_QUERY_STATUS|SERVICE_CHANGE_CONFIG*/,
			(crt_ref.Options().Type().IsDriver()  ?  CTypeOption::eKernelDrv  : CTypeOption::eService),
			(crt_ref.Options().Start().OnDemand() ?  CStartOption::eOnDemand  : CStartOption::eOnAuto),
			SERVICE_ERROR_NORMAL,
			crt_ref.Properties().Path(),
			nullptr  ,
			nullptr  ,
			nullptr  , /*_T("")*/
			nullptr  , //crt_ref.Properties().Account(), if nullptr then a local system account is used by default;
			nullptr
		);
		if (svc_ref.Is() == false)
			return (m_error = ::GetLastError());

		svc_ref.Cfg().Data().szSericeName = crt_ref.Properties().Name();

		d_result = svc_ref.Cfg_Ex().Description(crt_ref.Properties().Desc());
		if (FAILED(d_result))
			m_error = svc_ref.Error();

		return m_error;
}

DWORD   CSetup::Is    (const CCrtData& _crt_dat) const {
	if (_crt_dat.Is_valid() == false)
		return (m_error = _crt_dat.Error());
	else
		return this->Is(_crt_dat.Properties().Name());
}

DWORD   CSetup::Is    (LPCTSTR _lp_sz_svc_name) const {
	m_error << __MODULE__ << S_OK;

	if (NULL == _lp_sz_svc_name || 0 == ::_tcslen(_lp_sz_svc_name))
		return (m_error << ERROR_INVALID_PARAMETER);

	// https://docs.microsoft.com/en-us/windows/win32/api/winsvc/nf-winsvc-openscmanagerw
	CSvcManager_Handle man_hand;
	DWORD d_result = man_hand.Connect(SC_MANAGER_ALL_ACCESS);
	if (FAILURE(d_result))
		return (m_error = man_hand.Error());

	SC_HANDLE service_ = ::OpenService(man_hand, _lp_sz_svc_name, GENERIC_READ);
	if (NULL == service_)
		return (m_error << ::GetLastError());
	else
		::CloseServiceHandle(service_);

	return m_error;
}

DWORD   CSetup::Is    (const CCrtData& _crt_dat, CService& _result) const {
	m_error << __MODULE__ << S_OK;

	if (_crt_dat.Is_valid() == false)
		return (m_error = _crt_dat.Error());

	CSvcManager svc_man;
	DWORD  d_result = svc_man.Handle().Connect(CSvcManagerParams::eOpenForEnum);
	if (FAILURE(d_result))
		return (m_error = svc_man.Handle().Error());

	d_result = svc_man.Open(_crt_dat.Properties().Name(), GENERIC_READ, _result);
	if (FAILURE(d_result))
		m_error = svc_man.Handle().Error();

	return m_error;
}
const
TSvcSetupState& CSetup::State (void) const { return m_state; }
TSvcSetupState& CSetup::State (void)       { return m_state; }

DWORD   CSetup::UnInstall(LPCTSTR _lp_sz_svc_name) {
	m_error << __MODULE__ << S_OK;

	string_t cs_svc(_lp_sz_svc_name);
	if (cs_svc.empty())
		return (m_error << ERROR_INVALID_PARAMETER);

	CSvcManager_Handle man_hand;
	DWORD d_result = man_hand.Connect(CSvcManagerParams::eOpenForService);
	if (FAILURE(d_result))
		return (m_error = man_hand.Error());

	CService_Handle svc_hand;
	svc_hand << ::OpenService(man_hand, _lp_sz_svc_name, SERVICE_ALL_ACCESS);
	if (svc_hand.Is() == false)
		m_error.Last();

	if (!::DeleteService(svc_hand))
		m_error << ::GetLastError();

	return m_error;
}

/////////////////////////////////////////////////////////////////////////////

CSetup& CSetup::operator = (const CSetup& _ref) {
	this->State() = _ref.State();
	this->m_error = _ref.m_error;
	return *this;
}

/////////////////////////////////////////////////////////////////////////////

namespace shared { namespace service { namespace _impl {

	TSvcSetups&  SvcSetup_Objects (void) {

		static TSvcSetups setups;
		if (setups.empty() == true) {
			try {
				setups.push_back(CSetup(CSetup::CState::eNotInstalled));
				setups.push_back(CSetup(CSetup::CState::eInstalled));
			}
			catch (const ::std::bad_alloc&){}
		}

		return setups;
	}

}}}
using namespace shared::service::_impl;
/////////////////////////////////////////////////////////////////////////////
#if (0)
CSetupEnum:: CSetupEnum(void) {}
CSetupEnum::~CSetupEnum(void) {}

/////////////////////////////////////////////////////////////////////////////

INT          CSetupEnum::Count(void) const { return static_cast<INT>(SvcSetup_Objects().size()); }
const
TSvcSetups&  CSetupEnum::Raw  (void) const { return SvcSetup_Objects(); }
TSvcSetupRef CSetupEnum::Setup(const INT nIndex) const {

	if (0 > nIndex || nIndex > this->Count() - 1) {
		static CSetup invalid_;
		return invalid_;
	}
	else
		return SvcSetup_Objects()[nIndex];
}
#endif