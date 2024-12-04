/*
	Created by Tech_dog (ebontrop@gmail.com) on 4-Feb-2015 at 00:00:00.0a, UTC+3, Taganrog, Wednesday;
	This is Web setup project shared library generic file security access interface declaration file;
	-----------------------------------------------------------------------------
	Adopted to Ebo Pack solution on 22-Jan-2021 at 01:56:57.124 am, UTC+7, Novosibirsk, Friday;
	Adopted to Process Prying Eyes project on 10-Mar-2023 at 07:13:05.9251735, UTC+7, Novosibirsk, Friday;
*/
#include "shared.sec.file.acc.h"

using namespace shared::security;

/////////////////////////////////////////////////////////////////////////////

TFileAccess:: CFileAccess (void) : m_psd(NULL), m_pdacl(NULL) { m_error << __MODULE__ << S_OK; }
TFileAccess:: CFileAccess (const TFileAccess& _ref) : TFileAccess() { *this = _ref; }
TFileAccess:: CFileAccess (LPCTSTR _lp_sz_file_path) : TFileAccess() { *this << _lp_sz_file_path; }
TFileAccess::~CFileAccess (void) { if (this->Is()) this->Reset(); }

/////////////////////////////////////////////////////////////////////////////

const PACL  TFileAccess::ControlList (void) const { return m_pdacl; }
const
PSEC_DESC   TFileAccess::Descriptor  (void) const { return m_psd; }
TSysErrRef  TFileAccess::Error (void) const { return m_error; }
bool        TFileAccess::Is (void) const {
	return (this->m_file.Is () || m_psd != NULL);
}

LPCWSTR  TFileAccess::Path (void) const { return m_path.c_str(); }
HRESULT  TFileAccess::Path (LPCTSTR _lp_sz_file_path) {
	_lp_sz_file_path;
	m_error << __MODULE__ << S_OK;

	if (NULL == _lp_sz_file_path || _MAX_DRIVE > ::_tcslen(_lp_sz_file_path))
		return (m_error << E_INVALIDARG);

	if (this->Is())
		return (m_error << __DwordToHresult(ERROR_ALREADY_INITIALIZED));

	m_file = ::CreateFile(
		_lp_sz_file_path, READ_CONTROL, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_BACKUP_SEMANTICS, NULL
	);

	if (m_file.Is() == false)
		return (m_error << __LastErrToHresult());

	m_error = ::GetSecurityInfo(
		m_file, SE_FILE_OBJECT, DACL_SECURITY_INFORMATION, NULL, NULL, &m_pdacl, NULL, &m_psd
	);

	return m_error;
}

HRESULT  TFileAccess::Reset(void) {
	m_error << __MODULE__ << S_OK;

	if (m_psd) {
		::LocalFree(m_psd); m_psd = NULL;
	}

	if (m_file.Is()) {
		m_file.Reset();
	}

	if (m_pdacl)
		m_pdacl = NULL;

	if (m_path.empty() == false)
		m_path.clear();

	return m_error;
}

/////////////////////////////////////////////////////////////////////////////

TFileAccess&  TFileAccess::operator = (const TFileAccess& _ref) {
	if (this->Is())
		this->Reset();
	TFileAccess& ref_ = const_cast<TFileAccess&>(_ref);
	this->m_file.Clone(ref_.m_file);
	this->m_path  = ref_.m_path ;
	this->m_pdacl = ref_.m_pdacl; ref_.m_pdacl = NULL;
	this->m_psd   = ref_.m_psd  ; ref_.m_psd = NULL;
	return *this;
}

CFileAccess&  TFileAccess::operator <<(LPCTSTR _lp_sz_file_path) { if (this->Is()) this->Reset(); this->Path(_lp_sz_file_path); return *this; }