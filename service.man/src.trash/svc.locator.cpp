/*
	Created by Tech_dog (ebontrop@gmail.com) on 11-Feb-2023 at 20:35:27.8172870, UTC+7, Novosibirsk, Saturday;
	This is Protocol Prying Eyes project service executable locator interface implementation file;
*/
#include "svc.locator.h"

namespace service::man;

/////////////////////////////////////////////////////////////////////////////

CLocator:: CLocator (void) { m_error << __d(S_OK) >> __MODULE__; }
CLocator:: CLocator (const CLocator& _ref) : CLocator() { *this = _ref; }
CLocator::~CLocator (void) {}

/////////////////////////////////////////////////////////////////////////////

bool  CLocator::IsAccepted (LPCTSTR _lp_sz_path)
{
	m_error << __d(S_OK) >> __MODULE__;

	

	return !(bool)m_error;
}

/////////////////////////////////////////////////////////////////////////////