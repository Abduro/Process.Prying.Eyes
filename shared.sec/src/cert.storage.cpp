/*
	Created by Tech_dog (ebontrop@gmail.com) on 17-Dec-2014 at 0:00:00.0, UTC+3, Saint-Petersburg, Wednesday;
	This is MS Windows certificate storage wrapper interface implementation file;
	-----------------------------------------------------------------------------
	Adopted to Process Prying Eyes project on 6-Mar-2023 at 16:04:55.2487536, UTC+7, Novosibirsk, Monday;
*/
#include "cert.storage.h"

using namespace shared::certificate::_impl;

/////////////////////////////////////////////////////////////////////////////

CStorage:: CStorage (void) : m_store(nullptr), m_message(nullptr) { m_error << __MODULE__ << S_OK; }
CStorage:: CStorage (LPCTSTR _lp_sz_file) : CStorage() { *this << _lp_sz_file; }
CStorage::~CStorage (void) { this->Close(); }

/////////////////////////////////////////////////////////////////////////////

HRESULT    CStorage::Close (void) {
	m_error << __MODULE__ << S_OK;

	// does not being intersted in possible errors of destroying these handles;
	if (this->m_store) { // https://learn.microsoft.com/en-us/windows/win32/api/wincrypt/nf-wincrypt-certclosestore ;
		if (false == !!::CertCloseStore(this->m_store, 0))
			m_error.Last();
		else
			this->m_store = nullptr;
	}
	if (this->m_message) { // https://learn.microsoft.com/en-us/windows/win32/api/wincrypt/nf-wincrypt-cryptmsgclose ;
		if (false == !!::CryptMsgClose(this->m_message))
			m_error.Last();
		else
			this->m_message = nullptr;
	}

	return m_error;
}

TSysErrRef CStorage::Error (void) const { return this->m_error; }

HRESULT    CStorage::Open  (LPCTSTR _lp_sz_file) {
	m_error << __MODULE__ << S_OK;

	if (nullptr == _lp_sz_file || 0 == ::_tcslen(_lp_sz_file))
		return (m_error << E_INVALIDARG);

	if (this->Is_valid() && this->m_file == string_t(_lp_sz_file)) // this certificate is already read;
		return (m_error);

	this->Close();
	if (m_error)
		return (m_error);
	else
		m_error << __MODULE__; // otherwise close() overrides;

	DWORD dw_out[3] = {0}; // 0 - encoding; 1 - content type; 2 - format type;
	dw_out;
	DWORD a_0 = 0;
	DWORD a_1 = 0;
	DWORD a_2 = 0;

	// gets message handle and store handle from the signed file;
	bool b_result = !!::CryptQueryObject( // https://learn.microsoft.com/en-us/windows/win32/api/wincrypt/nf-wincrypt-cryptqueryobject ;
		CERT_QUERY_OBJECT_FILE,
		_lp_sz_file,
		CERT_QUERY_CONTENT_FLAG_PKCS7_SIGNED_EMBED,
		CERT_QUERY_FORMAT_FLAG_BINARY,
		0          ,
		&a_0 ,
		&a_1 ,
		&a_2 ,
		&this->m_store  ,
		&this->m_message,
		nullptr
	);

	if (false == b_result)
		this->m_error.Last();
	else
		this->m_file = _lp_sz_file;

	return m_error;
}

/////////////////////////////////////////////////////////////////////////////

bool    CStorage::Is_valid (void) const { return (nullptr != this->m_message && nullptr != this->m_store); }

HCERTSTORE CStorage::Handle  (void) const { return this->m_store; }
HCRYPTMSG  CStorage::Message (void) const { return this->m_message; }

/////////////////////////////////////////////////////////////////////////////

CStorage&  CStorage::operator << (LPCTSTR _lp_sz_file) { this->Open(_lp_sz_file); return *this; }

/////////////////////////////////////////////////////////////////////////////

CStorage::operator HCERTSTORE (void) const { return this->m_store; }