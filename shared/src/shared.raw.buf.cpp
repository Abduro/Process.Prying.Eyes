/*
	Created by Tech_dog (ebontrop@gmail.com) on 22-Apr-2010 at 2:03:22am, GMT+3, Rostov-on-Don, Thursday;
	This is Shared Lite Library raw data class implementation file.
	-----------------------------------------------------------------------------
	Adopted to Ebo Pack solution on 28-May-2018 at 12:00:40p, UTC+7, Phuket, Rawai, Monday;
	-----------------------------------------------------------------------------
	Adopted to Process Prying Eyes project on 12-Feb-2023 at 12:24:55.2641336, UTC+7, Novosibirsk, Sundsy;
*/
#include "shared.raw.buf.h"

using namespace shared::common;
/////////////////////////////////////////////////////////////////////////////

CRawBuffer:: CRawBuffer(void) : m_pData(nullptr), m_dwSize(0) { m_error << __MODULE__ << ERROR_INVALID_DATA; }
CRawBuffer::~CRawBuffer(void) {}

/////////////////////////////////////////////////////////////////////////////

DWORD   CRawBuffer::Clear(void) {
	m_error << __MODULE__ << S_OK;

	if (this->IsValid() == false) {
		return (m_error  << ERROR_INVALID_STATE);
	}

	::memset(m_pData, 0, sizeof(BYTE) * m_dwSize);

	return m_error;
}

bool      CRawBuffer::IsEmpty(void) const {

	for (UINT i_ = 0; i_ < m_dwSize; i_ ++) {
		if (m_pData[i_] != 0)
			return true;
	}
	return false;
}

bool      CRawBuffer::IsValid(void) const { return (NULL != m_pData && 0 != m_dwSize); }

/////////////////////////////////////////////////////////////////////////////

CRawData:: CRawData (void) : TBuffer() {}
CRawData:: CRawData (const CRawData& raw_ref) : CRawData() { *this = raw_ref; }
CRawData:: CRawData (const DWORD dwSize) : CRawData() { this->Create(dwSize); }
CRawData::~CRawData (void) { this->Reset(); }

/////////////////////////////////////////////////////////////////////////////

DWORD      CRawData::Append (const PBYTE pData, const DWORD dwSize) {
	m_error << __MODULE__ << S_OK;
	if (/*nullptr == pData ||*/ 0 == dwSize)
		return (m_error << ERROR_INVALID_PARAMETER);

	PBYTE pCurrentLocator = nullptr;

	if (this->IsValid())
	{
		const DWORD dwNewSize = m_dwSize + dwSize;
		pCurrentLocator = reinterpret_cast<PBYTE>(
					::HeapReAlloc(::GetProcessHeap(), HEAP_ZERO_MEMORY, m_pData, dwNewSize)
				);
		if (pCurrentLocator)
		{
			m_pData = pCurrentLocator;
			pCurrentLocator += m_dwSize;
			m_dwSize = dwNewSize;
		}
		else
			m_error << ERROR_NOT_ENOUGH_MEMORY;
	}
	else {
		m_dwSize = dwSize;
		m_pData = reinterpret_cast<PBYTE>(
					::HeapAlloc(::GetProcessHeap(), HEAP_ZERO_MEMORY, dwSize)
				);
		if (m_pData)
			pCurrentLocator = m_pData;
		else
			m_error << ERROR_NOT_ENOUGH_MEMORY;
	}
	if ((bool)m_error == false)
	{
		if (pData) {
		const errno_t err_ = ::memcpy_s(pCurrentLocator, dwSize, pData, dwSize);
		if (err_)
			m_error = ERROR_NOT_ENOUGH_MEMORY;
		}
	}
	return (DWORD)m_error;
}

DWORD      CRawData::Create (const DWORD dwSize) {
	m_error << __MODULE__ << S_OK;

	if (this->IsValid())
		this->Reset();

	if (dwSize) {
		TBuffer::m_pData = reinterpret_cast<PBYTE>(
					::HeapAlloc(::GetProcessHeap(), HEAP_ZERO_MEMORY, dwSize)
				);
		if (NULL == m_pData)
			m_error.Last();
		else {
			TBuffer::m_dwSize = dwSize;
		}
	}
	return (DWORD)m_error;
}

DWORD      CRawData::Create(const PBYTE pData, const DWORD dwSize) {
	m_error << __MODULE__ << S_OK;
	this->Reset();
	return this->Append(pData, dwSize);
}

LPBYTE     CRawData::Detach (void) {
	m_error << __MODULE__ << S_OK;

	if (this->IsValid() == false) {
		m_error << ERROR_INVALID_DATA; return nullptr;
	}
	LPBYTE p_ret = TBuffer::m_pData; TBuffer::m_pData = nullptr; TBuffer::m_dwSize = 0; m_error << ERROR_INVALID_DATA;
	return p_ret;
}

TSysErrRef CRawData::Error  (void) const { return m_error ; }

PCBYTE     CRawData::GetData(void) const { return m_pData ; }
PBYTE      CRawData::GetData(void)       { return m_pData ; }
DWORD      CRawData::GetSize(void) const { return m_dwSize; }

DWORD      CRawData::Reset  (void) {
	m_error << __MODULE__ << S_OK;
	m_dwSize = 0;
	if (m_pData) {
		if (!::HeapFree(::GetProcessHeap(), 0, m_pData))
			m_error.Last();

		m_pData = nullptr;
		if ((bool)m_error)
			return (DWORD)m_error;
	}

	m_error << ERROR_INVALID_DATA = _T("No data");
	
	return  m_error;
}

/////////////////////////////////////////////////////////////////////////////

CRawData::operator const    bool (void)const { return this->IsValid(); }
CRawData::operator const    DWORD(void)const { return m_dwSize; }
CRawData::operator const    PBYTE(void)const { return m_pData ; }
CRawData::operator          PBYTE(void)      { return m_pData ; }

/////////////////////////////////////////////////////////////////////////////

CRawData& CRawData::operator = (const CRawData& raw_ref) { this->Create(raw_ref.GetData(), raw_ref.GetSize()); return *this; }

CRawData& CRawData::operator = (const ::std::vector<BYTE>& _vec) {
	if (_vec.empty() == true)
		return *this;

	this->Create(
			(const PBYTE)_vec.data(), static_cast<DWORD>(_vec.size()) * sizeof(BYTE)
		);
	return *this;
}