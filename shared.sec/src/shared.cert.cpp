/*
	Created by Tech_dog (ebontrop@gmail.com) on 17-Dec-2014 at 00:00:00.0, UTC+3, Saint-Petersburg, Wednesday;
	This is Shared Lite Library System Certificate Data Provider class implementation file.
*/
#include "shared.cert.h"
#include "cert.storage.h"

using namespace shared::certificate;

/////////////////////////////////////////////////////////////////////////////

CSubjectInfo:: CSubjectInfo(void) {}
CSubjectInfo::~CSubjectInfo(void) {}

/////////////////////////////////////////////////////////////////////////////

LPCTSTR   CSubjectInfo::Address (void) const { return m_values[CSubjectInfo::eAddress].c_str(); }
HRESULT   CSubjectInfo::Address (LPCTSTR pszValue, const bool bAppend) {
	if (false == bAppend)
		m_values[CSubjectInfo::eAddress] = pszValue;
	else
	{
		if (pszValue && 0 < ::_tcslen(pszValue))
			m_values[CSubjectInfo::eAddress]+= _T(", ");

		m_values[CSubjectInfo::eAddress]+= pszValue;
	}
	return S_OK;
}

LPCTSTR   CSubjectInfo::Company (void) const { return m_values[CSubjectInfo::eCompany].c_str(); }
HRESULT   CSubjectInfo::Company (LPCTSTR pszValue) {
	m_values[CSubjectInfo::eCompany] = pszValue;
	return S_OK;
}

LPCTSTR   CSubjectInfo::Country (void) const { return m_values[CSubjectInfo::eCountry].c_str(); }
HRESULT   CSubjectInfo::Country (LPCTSTR pszValue) {
	m_values[CSubjectInfo::eCountry] = pszValue;
	return S_OK;
}

LPCTSTR   CSubjectInfo::Location(void) const { return m_values[CSubjectInfo::eLocation].c_str();}
HRESULT   CSubjectInfo::Location(LPCTSTR pszValue) {
	m_values[CSubjectInfo::eLocation] = pszValue;
	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////

namespace shared { namespace certificate { namespace _impl {}}}
using namespace shared::certificate::_impl;
/////////////////////////////////////////////////////////////////////////////

HRESULT   CSubjectInfo::ParseText(LPCTSTR pszText)
{
	HRESULT hr_ = S_FALSE;

	if (!pszText || !::_tcslen(pszText))
		return (hr_ = E_INVALIDARG);

	::std::vector<string_t> v_pairs = shared::sys_core::_split(string_t(pszText), string_t(_T(",")));

	for (size_t i_ = 0; i_ < v_pairs.size(); i_++)
	{
		size_t n_sep = v_pairs[i_].find(_T("="));
		if (::std::string::npos == n_sep)
			continue;
		string_t cs_key = v_pairs[i_].substr(0, n_sep - 1);
		string_t cs_val = v_pairs[i_].substr(n_sep + 1, v_pairs[i_].length());

		if (false == cs_key.empty()) ::shared::sys_core::_trim(cs_key);
		if (false == cs_val.empty()) ::shared::sys_core::_trim(cs_val);
		if (false == cs_val.empty()) ::shared::sys_core::_replace(cs_val, _T("\""), _T(""));
		if (false) {}
		else if (0 == ::_tcsicmp(cs_key.c_str(), _T(szOID_COMMON_NAME))) hr_ = this->Name(cs_val.c_str());
		else if (0 == ::_tcsicmp(cs_key.c_str(), _T(szOID_COUNTRY_NAME))) hr_ = this->Country(cs_val.c_str());
		else if (0 == ::_tcsicmp(cs_key.c_str(), _T(szOID_LOCALITY_NAME))) hr_ = this->Location(cs_val.c_str());
		else if (0 == ::_tcsicmp(cs_key.c_str(), _T(szOID_STREET_ADDRESS))) hr_ = this->Address(cs_val.c_str());
		else if (0 == ::_tcsicmp(cs_key.c_str(), _T(szOID_ORGANIZATION_NAME))) hr_ = this->Company(cs_val.c_str());
		else if (0 == ::_tcsicmp(cs_key.c_str(), _T(szOID_STATE_OR_PROVINCE_NAME))) hr_ = this->State(cs_val.c_str());
	}
	return  hr_;
}

LPCTSTR   CSubjectInfo::Name (void) const { return m_values[CSubjectInfo::eName].c_str(); }
HRESULT   CSubjectInfo::Name (LPCTSTR pszValue) {
	m_values[CSubjectInfo::eName] = pszValue;
	return S_OK;
}

LPCTSTR   CSubjectInfo::State(void) const { return m_values[CSubjectInfo::eState].c_str();}
HRESULT   CSubjectInfo::State(LPCTSTR pszValue) {
	m_values[CSubjectInfo::eState] = pszValue;
	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////

namespace shared { namespace certificate { namespace _impl {

	LONGLONG SignatureInfo_ConvertFileTimeToUnixStamp(const FILETIME& file_time)
	{
		#define WINDOWS_TICK 10000000
		#define SEC_TO_UNIX_EPOCH 11644473600LL

		LARGE_INTEGER li_period = {0};
		li_period.HighPart      = file_time.dwHighDateTime;
		li_period.LowPart       = file_time.dwLowDateTime;
		const LONGLONG ll_secs  = li_period.QuadPart;

		return ll_secs / WINDOWS_TICK - SEC_TO_UNIX_EPOCH;
	}

}}}
using namespace shared::certificate::_impl;
/////////////////////////////////////////////////////////////////////////////

CSignatureTime:: CSignatureTime (void) : m_time{0} {}
CSignatureTime:: CSignatureTime (const CSignatureTime& _ref) : CSignatureTime() { *this = _ref; }
CSignatureTime:: CSignatureTime (const SYSTEMTIME& _time) : CSignatureTime() { *this << _time; }
CSignatureTime::~CSignatureTime (void) {}

/////////////////////////////////////////////////////////////////////////////

bool CSignatureTime::Is_valid (void) const { return !(!this->Time().wDay || !this->Time().wMonth || !this->Time().wYear); }

const
SYSTEMTIME& CSignatureTime::Time (void) const { return this->m_time; }
HRESULT     CSignatureTime::Time (const SYSTEMTIME& _time) { this->m_time = _time; return S_OK; }
LONGLONG    CSignatureTime::TimeAsUnix(void) const {
	LONGLONG l_result = {0};

	if (false == this->Is_valid())
		return l_result;

	FILETIME f_time   ={0};
	::SystemTimeToFileTime(&this->Time(), &f_time);

	return (l_result = SignatureInfo_ConvertFileTimeToUnixStamp(f_time));
}

/////////////////////////////////////////////////////////////////////////////

CSignatureTime& CSignatureTime::operator = (const CSignatureTime& _ref) { *this << _ref.Time(); return *this; }
CSignatureTime& CSignatureTime::operator <<(const SYSTEMTIME& _time) { this->m_time = _time; return *this; }

/////////////////////////////////////////////////////////////////////////////

namespace shared { namespace certificate { namespace _impl {

	bool CSystemTime_IsEqual(const SYSTEMTIME& _left, const SYSTEMTIME& _right, const WORD _msec_threshold)
	{
		if (_left.wYear   != _right.wYear  ) return false;
		if (_left.wMonth  != _right.wMonth ) return false;
		if (_left.wDay    != _right.wDay   ) return false;
		if (_left.wHour   != _right.wHour  ) return false;
		if (_left.wMinute != _right.wMinute) return false;
		if (_left.wSecond != _right.wSecond) return false;
		if (_left.wMilliseconds > _right.wMilliseconds
		 && _left.wMilliseconds - _right.wMilliseconds > _msec_threshold)
				return false;
		if (_right.wMilliseconds > _left.wMilliseconds
		 && _right.wMilliseconds - _left.wMilliseconds > _msec_threshold)
				return false;

		return true;
	}

	bool CSystemTime_LeftAfterRight(const SYSTEMTIME& _left, const SYSTEMTIME& _right, const WORD _msec_threshold)
	{
		if (_left.wYear   >  _right.wYear) return true;
		if (_left.wYear   == _right.wYear   && _left.wMonth  > _right.wMonth ) return true;
		if (_left.wMonth  == _right.wMonth  && _left.wDay    > _right.wDay   ) return true;
		if (_left.wDay    == _right.wDay    && _left.wHour   > _right.wHour  ) return true;
		if (_left.wHour   == _right.wHour   && _left.wMinute > _right.wMinute) return true;
		if (_left.wMinute == _right.wMinute && _left.wSecond > _right.wSecond) return true;

		if (_left.wMilliseconds >= _right.wMilliseconds
		 && _left.wMilliseconds  - _right.wMilliseconds <= _msec_threshold)
			return true;

		if (_right.wMilliseconds  > _left.wMilliseconds
		 && _right.wMilliseconds  - _left.wMilliseconds <= _msec_threshold)
			return true;

		return false;
	}

	bool CSystemTime_LeftLaterRight(const SYSTEMTIME& _left, const SYSTEMTIME& _right, const WORD _msec_threshold) {
		return CSystemTime_LeftAfterRight(_left, _right, _msec_threshold);
	}

}}}

/////////////////////////////////////////////////////////////////////////////

bool CSignatureTime::operator > (const CSignatureTime& _ref) const { return CSystemTime_LeftAfterRight(this->Time(), _ref.Time(), 0); }
bool CSignatureTime::operator < (const CSignatureTime& _ref) const { return CSystemTime_LeftLaterRight(this->Time(), _ref.Time(), 0); }
bool CSignatureTime::operator ==(const CSignatureTime& _ref) const { return CSystemTime_IsEqual(this->Time(), _ref.Time(), 0); }

/////////////////////////////////////////////////////////////////////////////

CCertPeriod:: CCertPeriod (void) {}
CCertPeriod:: CCertPeriod (const CCertPeriod& _ref) : CCertPeriod() { *this = _ref; }
CCertPeriod::~CCertPeriod (void) {}

/////////////////////////////////////////////////////////////////////////////
const
TCertTime&  CCertPeriod::From (void) const { return this->m_from; }
TCertTime&  CCertPeriod::From (void)       { return this->m_from; }
const
TCertTime&  CCertPeriod::Upto (void) const { return this->m_upto; }
TCertTime&  CCertPeriod::Upto (void)       { return this->m_upto; }

/////////////////////////////////////////////////////////////////////////////

bool    CCertPeriod::Is_valid (void) const { return (this->From().Is_valid() && this->Upto().Is_valid() && this->Upto() > this->From()); }

/////////////////////////////////////////////////////////////////////////////

CCertPeriod& CCertPeriod::operator = (const CCertPeriod& _ref) { *this << _ref.From() >> _ref.Upto(); return *this; }
CCertPeriod& CCertPeriod::operator <<(const TCertTime&  _from) {  this->m_from = _from; return *this; }
CCertPeriod& CCertPeriod::operator >>(const TCertTime&  _upto) {  this->m_upto = _upto; return *this; }

/////////////////////////////////////////////////////////////////////////////

CSignatureTime::operator const SYSTEMTIME& (void) const { return this->Time(); }

/////////////////////////////////////////////////////////////////////////////

CSignatureInfo:: CSignatureInfo (void) {}
CSignatureInfo:: CSignatureInfo (const CSignatureInfo& _ref) : CSignatureInfo() { *this = _ref; }
CSignatureInfo::~CSignatureInfo (void) {}

/////////////////////////////////////////////////////////////////////////////

LPCTSTR  CSignatureInfo::Issuer (void) const { return m_issuer.c_str(); }
HRESULT  CSignatureInfo::Issuer (LPCTSTR pszIssuer) {

	if (!pszIssuer || ::_tcslen(pszIssuer) < 1)
		return E_INVALIDARG;
	
	this->m_issuer = pszIssuer;
	
	return S_OK;
}

bool     CSignatureInfo::Is_valid (void) const { return (!this->Signer().Company() && !this->Issuer()); }
const
CCertPeriod&   CSignatureInfo::Period (void) const { return this->m_period; }
CCertPeriod&   CSignatureInfo::Period (void)       { return this->m_period; }

LPCTSTR  CSignatureInfo::SerialNumber (void) const { return m_serial.c_str(); }
HRESULT  CSignatureInfo::SerialNumber (LPCTSTR pszSerial)
{
	if (!pszSerial || !::_tcslen(pszSerial))
		return E_INVALIDARG;

	m_serial = pszSerial;

	return S_OK;
}

const
CSubjectInfo&  CSignatureInfo::Signer(void) const { return this->m_signer; }
CSubjectInfo&  CSignatureInfo::Signer(void)       { return this->m_signer; }

/////////////////////////////////////////////////////////////////////////////

CSignatureInfo& CSignatureInfo::operator = (const CSignatureInfo& _ref) {
	this->Issuer(_ref.Issuer());
	this->Period() = _ref.Period();
	this->SerialNumber(_ref.SerialNumber());
	this->Signer() = _ref.Signer();

	return *this;
}

/////////////////////////////////////////////////////////////////////////////

namespace shared { namespace certificate { namespace _impl {

	HRESULT SignatureInfo_GetIssuerName(PCCERT_CONTEXT pCertContext, string_t& cs_name)
	{
		if (!pCertContext)
			return E_INVALIDARG;

		HRESULT hr_ = S_OK;

		const DWORD dwDataSize = ::CertGetNameString(
				pCertContext,
				CERT_NAME_SIMPLE_DISPLAY_TYPE,
				CERT_NAME_ISSUER_FLAG,
				NULL,
				NULL,
				0
			);
		if (!dwDataSize)
			hr_ = __LastErrToHresult();
		else {
			const DWORD dwBufferLen = dwDataSize * sizeof(TCHAR);

			CRawData raw_issuer(dwBufferLen);

			if (!raw_issuer.IsValid())
				return raw_issuer.Error();

			if (!::CertGetNameString(
					pCertContext,
					CERT_NAME_SIMPLE_DISPLAY_TYPE,
					CERT_NAME_ISSUER_FLAG,
					NULL,
					reinterpret_cast<LPTSTR>(raw_issuer.GetData()),
					dwDataSize
				)) return __LastErrToHresult();

			cs_name = reinterpret_cast<LPTSTR>(raw_issuer.GetData());
		}
		return  hr_;
	}

	HRESULT SignatureInfo_GetSerialNumber(PCCERT_CONTEXT pCertContext, string_t& cs_serial)
	{
		if (!pCertContext)
			return E_INVALIDARG;

		const DWORD dwDataSize = pCertContext->pCertInfo->SerialNumber.cbData;
		if (!dwDataSize)
			return S_FALSE;

		for (DWORD i_ = 0; i_ < dwDataSize; i_++)
		{
			cs_serial += CFormatter().Accept(_T("%02x"), pCertContext->pCertInfo->SerialNumber.pbData[dwDataSize - (i_ + 1)]);
		}
		return S_OK;
	}

	HRESULT SignatureInfo_GetSubject(PCCERT_CONTEXT pCertContext, string_t& cs_subject)
	{
		if (!pCertContext)
			return E_INVALIDARG;

		HRESULT hr_ = S_OK;

		const DWORD dwDataSize = ::CertGetNameString(
			pCertContext,
			CERT_NAME_SIMPLE_DISPLAY_TYPE,
			0,
			NULL,
			NULL,
			0
		);
		if (!dwDataSize)
			hr_ = __LastErrToHresult();
		else
		{
			const DWORD dwBufferLen = dwDataSize * sizeof(TCHAR);

			CRawData raw_subject(dwBufferLen);

			if (!raw_subject.IsValid())
				return raw_subject.Error();

			if (!::CertGetNameString(
				pCertContext,
				CERT_NAME_SIMPLE_DISPLAY_TYPE,
				0,
				NULL,
				reinterpret_cast<LPTSTR>(raw_subject.GetData()),
				dwDataSize
			)) return __LastErrToHresult();

			cs_subject = reinterpret_cast<LPTSTR>(raw_subject.GetData());
		}
		return  hr_;
	}

	HRESULT SignatureInfo_GetSubject(PCCERT_CONTEXT pCertContext, CSignatureInfo& sign_info)
	{
		if (!pCertContext || !pCertContext->pCertInfo)
			return E_INVALIDARG;

		CERT_NAME_BLOB& blob_subject = pCertContext->pCertInfo->Subject;

		if (!blob_subject.cbData || !blob_subject.pbData)
			return E_INVALIDARG;

		HRESULT hr_ = S_OK;

		const DWORD dwDataSize = ::CertNameToStr(
			pCertContext->dwCertEncodingType,
			&blob_subject,
			CERT_OID_NAME_STR,
			NULL,
			0
		);
		if (dwDataSize < 2)
			return S_FALSE;

		const DWORD dwBufferLen = dwDataSize * sizeof(TCHAR);

		CRawData raw_subject(dwBufferLen);
		if (!raw_subject.IsValid())
			return raw_subject.Error();

		if (!::CertNameToStr(
			pCertContext->dwCertEncodingType,
			&blob_subject,
			CERT_OID_NAME_STR,
			reinterpret_cast<LPTSTR>(raw_subject.GetData()),
			dwDataSize
		)) return __LastErrToHresult();

		string_t cs_subject = reinterpret_cast<LPTSTR>(raw_subject.GetData());
		if (cs_subject.empty())
			return S_FALSE;

		hr_ = sign_info.Signer().ParseText(cs_subject.c_str());
		return  hr_;
	}
}}}

/////////////////////////////////////////////////////////////////////////////

CCertProvider:: CCertProvider (void) { m_error << __MODULE__ << S_OK; }
CCertProvider:: CCertProvider (const CCertProvider& _ref) : CCertProvider() { *this = _ref; }
CCertProvider:: CCertProvider (LPCTSTR _lp_sz_file) : CCertProvider() { *this << _lp_sz_file; }
CCertProvider::~CCertProvider (void) {}

/////////////////////////////////////////////////////////////////////////////

TSysErrRef  CCertProvider::Error (void) const { return this->m_error; }
const
TCertInfo&  CCertProvider::Get   (void) const { return this->m_cert ; }

HRESULT     CCertProvider::Get   (LPCTSTR _lp_sz_file) {
	m_error << __MODULE__ << S_OK;

	CStorage store(_lp_sz_file);

	if ((bool)store.Error())
		return (m_error = store.Error());

	// gets signer information size;
	DWORD dwSignerInfoSize = 0;
	bool b_result = !!::CryptMsgGetParam( // https://learn.microsoft.com/en-us/windows/win32/api/wincrypt/nf-wincrypt-cryptmsggetparam ;
		store.Message(),
		CMSG_SIGNER_INFO_PARAM,
		0,
		NULL,
		&dwSignerInfoSize
	);
	
	if (false == b_result)
	   (m_error << ::GetLastError());
	else {
		CRawData raw_signature(dwSignerInfoSize);

		if (raw_signature.IsValid()) {
			// gets a signer information ;
			b_result = !!::CryptMsgGetParam (
				store.Message(),
				CMSG_SIGNER_INFO_PARAM,
				0,
				(PVOID)raw_signature.GetData(),
				&dwSignerInfoSize
			);
			if (b_result) {
				const PCMSG_SIGNER_INFO pSignerInfo = reinterpret_cast<PCMSG_SIGNER_INFO>(raw_signature.GetData());

				CERT_INFO cert_info    = {0};
				cert_info.Issuer       = pSignerInfo->Issuer;
				cert_info.SerialNumber = pSignerInfo->SerialNumber;

				PCCERT_CONTEXT pCertContext = ::CertFindCertificateInStore(
					store.Handle(),
					X509_ASN_ENCODING | PKCS_7_ASN_ENCODING,
					0,
					CERT_FIND_SUBJECT_CERT,
					(PVOID)&cert_info,
					NULL
				);

				if (pCertContext) {

					string_t cs_buffer;
					if (S_OK == SignatureInfo_GetIssuerName(pCertContext, cs_buffer))
						this->m_cert.Issuer(cs_buffer.c_str());

					if (S_OK == SignatureInfo_GetSerialNumber(pCertContext, cs_buffer))
						this->m_cert.SerialNumber(cs_buffer.c_str());

					PCERT_INFO pCertInfo = pCertContext->pCertInfo;
					if (pCertInfo) {
						SYSTEMTIME sys_date = {0};
						if (::FileTimeToSystemTime(&(pCertInfo->NotBefore), &sys_date))
							this->m_cert.Period().From().Time(sys_date);
						if (::FileTimeToSystemTime(&(pCertInfo->NotAfter) , &sys_date))
							this->m_cert.Period().Upto().Time(sys_date);

						SignatureInfo_GetSubject(pCertContext, this->m_cert);
					}
					else if (S_OK == SignatureInfo_GetSubject(pCertContext, cs_buffer)) {
						this->m_cert.Signer().Company(cs_buffer.c_str());
					}

					::CertFreeCertificateContext(pCertContext); pCertContext = nullptr; // no checking an error of freeing the context ;
				}
			}
			else
				m_error.Last();
		}
		else
			m_error = raw_signature.Error();
	}
	return m_error;
}

/////////////////////////////////////////////////////////////////////////////

bool     CCertProvider::Is_Trusted (LPCTSTR _lp_sz_file) {
	// https://hg.mozilla.org/mozilla-central/file/tip/other-licenses/nsis/Contrib/CertCheck/CertCheck.cpp
	m_error << __MODULE__ << S_OK;

	if (nullptr == _lp_sz_file || 0 == ::_tcslen(_lp_sz_file)) {
		return (bool)(m_error << E_INVALIDARG);
	}
	// setups the file to check ;
	WINTRUST_FILE_INFO file_info = {0};
		
	file_info.cbStruct      = sizeof(WINTRUST_FILE_INFO);
	file_info.pcwszFilePath = _lp_sz_file; // must be wide string;

	// setups what to check: it is signed and trusted;
	WINTRUST_DATA trust_dat = {0};

	trust_dat.cbStruct      = sizeof(WINTRUST_DATA);
	trust_dat.dwUIChoice    = WTD_UI_NONE;
	trust_dat.dwUnionChoice = WTD_CHOICE_FILE;
	trust_dat.pFile         = &file_info;

	GUID policyGUID = WINTRUST_ACTION_GENERIC_VERIFY_V2;
	// https://learn.microsoft.com/en-us/windows/win32/seccrypto/example-c-program--verifying-the-signature-of-a-pe-file ;
	// https://learn.microsoft.com/en-us/windows/win32/api/wintrust/nf-wintrust-winverifytrust
	LONG l_result = ::WinVerifyTrust(NULL, &policyGUID, &trust_dat);

	switch (l_result) {
	case TRUST_E_NOSIGNATURE        : { m_error.Last(); m_error = _T("No signature;"); } break;
	case TRUST_E_EXPLICIT_DISTRUST  : { m_error.Last(); m_error = _T("Signature is disallowed;" ); } break;
	case TRUST_E_SUBJECT_NOT_TRUSTED: { m_error.Last(); m_error = _T("Signature is not trusted;"); } break;
	case CRYPT_E_SECURITY_SETTINGS  : { m_error.Last(); m_error = _T("Hash is not trusted;"); } break;
	case ERROR_SUCCESS: {} break;
	default: { (m_error << l_result) = _T("Publisher or timestamp is invalid;"); }
	}

	return (false == (bool)m_error);
}

/////////////////////////////////////////////////////////////////////////////

CCertProvider& CCertProvider::operator = (const CCertProvider& _ref) { this->m_error = _ref.Error(); this->m_cert = _ref.Get(); return *this; }
CCertProvider& CCertProvider::operator <<(LPCTSTR _lp_sz_file) { this->Get(_lp_sz_file); return *this; }