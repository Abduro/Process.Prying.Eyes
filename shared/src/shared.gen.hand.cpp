/*
	Created by Tech_dog (ebontrop@gmail.com) on 15-Mar-2011 at 00:00:32am, GMT+3, Rostov-on-Don, Tuesday;
	This is Row27 project shared library generic handle class(es) implementation file.
	-----------------------------------------------------------------------------
	Adopted to Platinum project on 29-Nov-2014 at 0:42:03am, GMT+3, Taganrog, Saturday;
	Adopted to File Guardian project on 27-May-2018 at 3:22:22p, UTC+7, Phuket, Rawai, Sunday;
	Adopted to Ebo Pack on 25-Jul-2018 at 9:44:02p, UTC+7, Novosibirsk, Wednesday;
*/
#include "shared.gen.hand.h"

using namespace shared::common;

/////////////////////////////////////////////////////////////////////////////

namespace shared { namespace common { namespace _impl
{
	static HRESULT  Shared_CloseHandleSafe(HANDLE& handle_)
	{
		if (!handle_ || INVALID_HANDLE_VALUE == handle_)
			return E_INVALIDARG;
		if (!::CloseHandle(handle_))
		{
			return HRESULT_FROM_WIN32(::GetLastError());
		}
		handle_ = INVALID_HANDLE_VALUE;
		return S_OK;
	}

	static HANDLE&  Shared_GetInvalidHandleRef(void)
	{
		static HANDLE handle = INVALID_HANDLE_VALUE;
		return handle;
	}
}}}
using namespace shared::common::_impl;
/////////////////////////////////////////////////////////////////////////////

CAutoHandle:: CAutoHandle (void) : m_handle(NULL) { }
CAutoHandle:: CAutoHandle (const CAutoHandle& _ref) : CAutoHandle() { *this = _ref; }
CAutoHandle:: CAutoHandle (const HANDLE handle_) : CAutoHandle() { *this = handle_; }
CAutoHandle::~CAutoHandle (void) { Shared_CloseHandleSafe(m_handle); }

/////////////////////////////////////////////////////////////////////////////

CAutoHandle& CAutoHandle::operator = (const CAutoHandle& _ref) {
	this->Clone(_ref.Handle());
	return *this;
}

CAutoHandle& CAutoHandle::operator = (const HANDLE _handle) {
	this->Clone(_handle);
	return *this;
}

CAutoHandle::operator HANDLE(void)       { return m_handle; }
CAutoHandle::operator HANDLE(void) const { return m_handle; }

/////////////////////////////////////////////////////////////////////////////

HRESULT    CAutoHandle::Attach (HANDLE _handle) {
	HRESULT hr_ = S_OK;
	if (this->Is())
		hr_ = Shared_CloseHandleSafe(this->m_handle);

	if (FAILED(hr_))
		return hr_;

	if (NULL == _handle || INVALID_HANDLE_VALUE == _handle)
		return (hr_ = E_HANDLE);

	this->m_handle = _handle;

	return  hr_;
}

/////////////////////////////////////////////////////////////////////////////

PHANDLE    CAutoHandle::operator&(void)  { return &m_handle;}

/////////////////////////////////////////////////////////////////////////////

HRESULT    CAutoHandle::Clone (const HANDLE _src) { HRESULT hr_ = S_OK;
	_src;
	if (NULL == _src || INVALID_HANDLE_VALUE == _src)
		return (hr_ = E_HANDLE);

	if (NULL != m_handle)
		hr_ = Shared_CloseHandleSafe(m_handle);
	// https://learn.microsoft.com/en-us/windows/win32/api/handleapi/nf-handleapi-duplicatehandle ;
	if (!::DuplicateHandle(::GetCurrentProcess(), _src, ::GetCurrentProcess, &this->m_handle, 0, FALSE, DUPLICATE_SAME_ACCESS))
		hr_ = __LastErrToHresult();

	return hr_;
}

HANDLE     CAutoHandle::Handle(void) const { return m_handle; }
bool       CAutoHandle::Is    (void) const { return (NULL != m_handle && INVALID_HANDLE_VALUE != m_handle); }
void       CAutoHandle::Reset (void) { Shared_CloseHandleSafe(m_handle); }

/////////////////////////////////////////////////////////////////////////////

CAutoHandleArray:: CAutoHandleArray(const DWORD dwSize) : m_handles(NULL), m_size(0) {
	m_error << __MODULE__ << S_OK;
	if (dwSize)
	{
		try
		{
			m_handles = new HANDLE[dwSize];
			m_size = dwSize;
		}
		catch(::std::bad_alloc&){ m_error << E_OUTOFMEMORY; }
	}
	else
		m_error << E_INVALIDARG;
}

CAutoHandleArray::~CAutoHandleArray(void)
{
	if (m_handles)
	{
		try { delete [] m_handles; m_handles = NULL; } catch(...){}
	}
	m_size = 0;
}

/////////////////////////////////////////////////////////////////////////////

TSysErrRef CAutoHandleArray::Error  (void)const { return m_error; }
bool       CAutoHandleArray::Is     (void)const { return (NULL != m_handles && m_size); }
PHANDLE    CAutoHandleArray::Handles(void)const { return m_handles; }
DWORD      CAutoHandleArray::Size   (void)const { return m_size; }

/////////////////////////////////////////////////////////////////////////////

HANDLE     CAutoHandleArray::operator[] (const INT nIndex) const
{
	return (!this->Is() || 0 > nIndex || INT(m_size - 1) < nIndex ? 0 : m_handles[nIndex]);
}

HANDLE&    CAutoHandleArray::operator[] (const INT nIndex)
{
	return (!this->Is() || 0 > nIndex || INT(m_size - 1) < nIndex ? Shared_GetInvalidHandleRef() : m_handles[nIndex]);
}

/////////////////////////////////////////////////////////////////////////////

#define SAFE_LOCK_HANDLE() SAFE_LOCK(this->m_sync_obj)
/////////////////////////////////////////////////////////////////////////////

CHandleSafe:: CHandleSafe(const HANDLE _handle) : m_handle(_handle) { }
CHandleSafe::~CHandleSafe(void)
{
	SAFE_LOCK_HANDLE();
	_ASSERT(INVALID_HANDLE_VALUE == m_handle);
}

/////////////////////////////////////////////////////////////////////////////

CHandleSafe& CHandleSafe::operator=(const HANDLE _handle)
{
	SAFE_LOCK_HANDLE();
	Shared_CloseHandleSafe(m_handle);
	m_handle = _handle;
	return *this;
}

CHandleSafe::operator HANDLE(void)
{
	SAFE_LOCK_HANDLE();
	HANDLE handle_ = m_handle;
	return handle_;
}

CHandleSafe::operator HANDLE(void) const
{
	SAFE_LOCK_HANDLE();
	HANDLE handle_ = m_handle;
	return handle_;
}

/////////////////////////////////////////////////////////////////////////////

PHANDLE    CHandleSafe::operator&(void)
{
	SAFE_LOCK_HANDLE();
	PHANDLE p_handle_ = &m_handle;
	return  p_handle_;
}

/////////////////////////////////////////////////////////////////////////////

HANDLE     CHandleSafe::Handle(void) const
{
	SAFE_LOCK_HANDLE();
	HANDLE handle_ = m_handle;
	return handle_;
}

bool       CHandleSafe::Is(void) const
{
	HANDLE handle_ = this->Handle();
	return (NULL != handle_ && INVALID_HANDLE_VALUE != handle_);
}

VOID       CHandleSafe::Reset(void)
{
	SAFE_LOCK_HANDLE();
	Shared_CloseHandleSafe(m_handle);
}

/////////////////////////////////////////////////////////////////////////////

CGlobalAlloca::CGlobalAlloca(const PVOID _data, const DWORD _size, const bool bMovable) : 
	m_global(NULL), m_dwSize(_size), m_pData(NULL), m_bMovable(bMovable)
{
	m_error << __MODULE__ << S_OK;

	if (0 == m_dwSize) {
		m_error << E_INVALIDARG; return;
	}

	m_global = ::GlobalAlloc(false == bMovable ? GPTR : GHND, m_dwSize);
	if (!m_global)
		m_error = ::GetLastError();
	else {

		if (m_bMovable) {
			m_pData = ::GlobalLock(m_global);
			if (NULL == m_pData)
				m_error = ::GetLastError();
			else if (_data) {
				const errno_t err_ = ::memcpy_s(m_pData, m_dwSize, _data, m_dwSize);
				if (err_)
					m_error << E_OUTOFMEMORY;
			}
		}
		else if (_data) {
			const errno_t err_ = ::memcpy_s(m_global, m_dwSize, _data, m_dwSize);
			if (err_)
				m_error << E_OUTOFMEMORY;
		}
	}
}

CGlobalAlloca::~CGlobalAlloca(void)
{
	if (m_pData) {
		::GlobalUnlock(m_global);
		m_pData = NULL;
	}
	if (m_global) {
		::GlobalFree(m_global);
		m_global = NULL;
	}
	m_error << OLE_E_BLANK;
}

/////////////////////////////////////////////////////////////////////////////

LPVOID     CGlobalAlloca::Data(void) const { return m_pData; }

HGLOBAL    CGlobalAlloca::Detach(void) {
	m_error << __MODULE__ << OLE_E_BLANK;

	HGLOBAL hGlobal = m_global;
	if (m_pData) {
		::GlobalUnlock(m_global);
		m_pData = NULL;
	}
	m_global = NULL;
	m_dwSize = 0;
	return  hGlobal;
}

TSysErrRef CGlobalAlloca::Error(void) const { return m_error;  }
HGLOBAL    CGlobalAlloca::Handle(void)const { return m_global; }

bool       CGlobalAlloca::IsValid(void)const
{
	if (m_bMovable)
		return (NULL != m_global && NULL != m_pData);
	else
		return (NULL != m_global);
}

DWORD      CGlobalAlloca::Size(void)const { return m_dwSize; }

/////////////////////////////////////////////////////////////////////////////

CStdHandle:: CStdHandle(const DWORD _handle_type) : THandle(), m_std_type(_handle_type) { m_error << __MODULE__ << S_OK; 
	switch (m_std_type) {
	case STD_OUTPUT_HANDLE: { THandle::m_handle = ::GetStdHandle(m_std_type); } break;
	case STD_ERROR_HANDLE : { THandle::m_handle = ::GetStdHandle(m_std_type); } break;
	}
	if (this->Is() == false)
		m_error = __LastErrToHresult();
}
CStdHandle::~CStdHandle(void) {
	this->Close();
}

/////////////////////////////////////////////////////////////////////////////

HRESULT    CStdHandle::Close(void) {
	m_error << __MODULE__ << S_OK;
	if (this->Is()) {
		if (::SetStdHandle(m_std_type, THandle::m_handle) == FALSE)
			m_error = __LastErrToHresult(); THandle::m_handle = NULL;
	}
	return m_error;
}
TSysErrRef CStdHandle::Error(void) const { return m_error; }
DWORD      CStdHandle::Type (void) const { return m_std_type; }

/////////////////////////////////////////////////////////////////////////////

CStdHandle& CStdHandle::operator=(const HANDLE _handle) { _handle;
	m_error << ERROR_INVALID_OPERATION << _T("Standard handler cannot be replaced;");
	
	return *this;
}