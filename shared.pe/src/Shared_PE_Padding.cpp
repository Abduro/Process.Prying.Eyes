/*
	Created by Tech_dog (ebontrop@gmail.com) on 20-Apr-2015 at 00:00:00.0, UTC+3, Taganrog, Monday;
	This is Shared Lite Library PE (portable executable) file padding common implementation file.
*/
#include "Shared_PE_Padding.h"

using namespace shared::sys_file;

/////////////////////////////////////////////////////////////////////////////

namespace shared { namespace sys_file { namespace _impl
{
}}}
using namespace shared::sys_file::_impl;
/////////////////////////////////////////////////////////////////////////////

CPeFilePadding::CPeFilePadding(void)
{
}

CPeFilePadding::~CPeFilePadding(void)
{
}

/////////////////////////////////////////////////////////////////////////////

const CRawData& CPeFilePadding::CloseTag(void)const
{
	return m_tag_close;
}

HRESULT         CPeFilePadding::CloseTag(const UINT resId)
{
	if (m_tag_close.IsValid())
		m_tag_close.Reset();
	if (!resId)
		return E_INVALIDARG;

	string_t cs_tag(CResLoader().String((WORD)resId));

	if (!cs_tag.empty())
	{	// https://stackoverflow.com/questions/4804298/how-to-convert-wstring-into-string ;
		::std::string cs_tag_a(cs_tag.begin(), cs_tag.end());
		m_tag_close.Append((const PBYTE)cs_tag_a.c_str(), static_cast<DWORD>(cs_tag_a.size()));
	}
	return m_tag_close.Error();
}

const CRawData& CPeFilePadding::CreatePadBuffer(void)
{
	if (!m_tag_close.IsValid() || !m_tag_open.IsValid())
		return m_pad_buffer;

	HRESULT hr_ = m_pad_buffer.Create(CPeFilePadding::BufferSize());
	if (S_OK != hr_ || !m_pad_buffer.IsValid())
		return m_pad_buffer;
	PBYTE pData = m_pad_buffer.GetData();
	{
		::memcpy((void*)pData, m_tag_open.GetData(), m_tag_open.GetSize());
	}
	{
		pData += (CPeFilePadding::BufferSize() - m_tag_close.GetSize());
		::memcpy((void*)pData, m_tag_close.GetData(), m_tag_close.GetSize());
	}
	return m_pad_buffer;
}

const CRawData& CPeFilePadding::CreatePadBuffer(LPCTSTR pszStartTag, LPCTSTR pszEndTag)
{
	if (pszStartTag && ::_tcslen(pszStartTag))
	{
		string_t cs_start_w(pszStartTag);
		::std::string cs_start_a(cs_start_w.begin(), cs_start_w.end());
		m_tag_open.Reset();
		m_tag_open.Append((const PBYTE)cs_start_a.c_str(), static_cast<DWORD>(cs_start_a.size()));
	}
	if (pszEndTag && ::_tcslen(pszEndTag))
	{
		string_t cs_end_w(pszEndTag);
		::std::string cs_end_a(cs_end_w.begin(), cs_end_w.end());
		m_tag_close.Reset();
		m_tag_close.Append((const PBYTE)cs_end_a.c_str(), static_cast<DWORD>(cs_end_a.size()));
	}
	return this->CreatePadBuffer();
}

const CRawData& CPeFilePadding::ExtractData(const CRawData& raw_buffer)
{
	m_pad_buffer.Reset();
	if (!raw_buffer.IsValid() || !m_tag_close.IsValid() || !m_tag_open.IsValid())
		return m_pad_buffer;
	// (1) finding the open tag; we expect the tag at the beginning of the data buffer;
	PBYTE p_start = NULL;
	{
		const DWORD n_len = m_tag_open.GetSize();
		if (raw_buffer.GetSize() > n_len)
		{
			const INT n_result = ::memcmp(raw_buffer.GetData(), m_tag_open.GetData(), n_len);
			if (!n_result)
				p_start = raw_buffer.GetData() + n_len;
			else
				return m_pad_buffer;
		}
	}
	// (2) finding the close tag; we expect the tag at the end of the data buffer; also, all possible trailing zeros are skipped
	PBYTE p_end = NULL;
	{
		const DWORD n_len = m_tag_close.GetSize();
		if (raw_buffer.GetSize() > n_len)
		{
			DWORD dw_pos = raw_buffer.GetSize() - 1;
			p_end = raw_buffer.GetData() + dw_pos; 
			while (dw_pos && !*(--p_end))
				dw_pos--;
			if (dw_pos == 0 || !p_end) // we have reached the beginning of the data buffer or data is zero
				return m_pad_buffer;
			if (dw_pos < n_len)        // there is insufficient room for the closing tag and data
				return m_pad_buffer;
			p_end -=  (n_len - 1);
			const INT n_result = ::memcmp(p_end, m_tag_close.GetData(), n_len);
			if (n_result)
				p_end = NULL;
		}
	}
	// (3) creates the buffer containing the actual data (injected)
	if (p_start && p_end && p_start < p_end)
	{
		const DWORD dw_len = static_cast<DWORD>(p_end - p_start);
		m_pad_buffer.Create(dw_len);
		if (m_pad_buffer.IsValid())
		{
			const errno_t err_ = ::memcpy_s(m_pad_buffer.GetData(), m_pad_buffer.GetSize(), p_start, dw_len);
			if (err_)
				m_pad_buffer.Reset();
		}
	}
	return m_pad_buffer;
}

const CRawData& CPeFilePadding::OpenTag(void)const
{
	return m_tag_open;
}

HRESULT         CPeFilePadding::OpenTag(const UINT resId)
{
	if (m_tag_open.IsValid())
		m_tag_open.Reset();
	if (!resId)
		return E_INVALIDARG;

	string_t cs_tag(CResLoader().String((WORD)resId));

	if (!cs_tag.empty())
	{
		::std::string cs_tag_a(cs_tag.begin(), cs_tag.end());
		m_tag_open.Append((const PBYTE)cs_tag_a.c_str(), static_cast<DWORD>(cs_tag_a.size()));
	}
	return m_tag_open.Error();
}

void            CPeFilePadding::Reset(void)
{
	if (m_tag_close.IsValid()) m_tag_close.Reset();
	if (m_tag_open.IsValid())  m_tag_open.Reset();
	if (m_pad_buffer.IsValid())m_pad_buffer.Reset();
}

/////////////////////////////////////////////////////////////////////////////

DWORD           CPeFilePadding::BufferSize(void)
{
	return 8192;
}