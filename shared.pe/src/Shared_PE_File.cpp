/*
	Created by Tech_dog (ebontrop@gmail.com) on 20-Apr-2015 at 13:31:26.1881875, UTC+3, Taganrog, Monday;
	This is Shared Lite Library PE (portable executable) Format Wrapper class implementation file.
*/
#include "Shared_PE_File.h"

using namespace shared::sys_file;

/////////////////////////////////////////////////////////////////////////////

namespace shared { namespace sys_file { namespace _impl
{
}}}
using namespace shared::sys_file::_impl;
/////////////////////////////////////////////////////////////////////////////

CPeFile_OptionalHeader::CPeFile_OptionalHeader(void) : m_pHeader(NULL), m_pv_ba(NULL)
{
	this->m_error << __MODULE__ << S_OK;
	::memset((void*)&m_opt_header, 0, sizeof(IMAGE_OPTIONAL_HEADER));
	try
	{ // these offsets are valid for 32-bit application only!!!
		m_offsets.insert(::std::make_pair(ePeFile_DataDirectory::eExportTable,  96));
		m_offsets.insert(::std::make_pair(ePeFile_DataDirectory::eImportTable, 104));
		m_offsets.insert(::std::make_pair(ePeFile_DataDirectory::eResource   , 112));
		m_offsets.insert(::std::make_pair(ePeFile_DataDirectory::eException  , 120));
		m_offsets.insert(::std::make_pair(ePeFile_DataDirectory::eCertificate, 128));
	}
	catch(::std::bad_alloc&){m_error << E_OUTOFMEMORY;}
}

CPeFile_OptionalHeader::~CPeFile_OptionalHeader(void)
{
	if (!m_offsets.empty())m_offsets.clear();
}

/////////////////////////////////////////////////////////////////////////////

IMAGE_DATA_DIRECTORY
           CPeFile_OptionalHeader::Directory(const ePeFile_DataDirectory::_e dir_type) const
{
	IMAGE_DATA_DIRECTORY img_dir = {0};

	if (!this->IsInitialized())
		return img_dir;
	if (m_opt_header.NumberOfRvaAndSizes - 1 < (DWORD)dir_type)
	{
		m_error << DISP_E_BADINDEX;
		return img_dir;
	}
	else
	{
		this->m_error << __MODULE__ << S_OK;
		img_dir = m_opt_header.DataDirectory[dir_type];
	}
	return img_dir;
}

HRESULT    CPeFile_OptionalHeader::Directory(const ePeFile_DataDirectory::_e dir_type, const DWORD dwSize)
{
	dwSize;
	if (!this->IsInitialized())
		return (m_error << OLE_E_BLANK);
	if (m_opt_header.NumberOfRvaAndSizes - 1 < (DWORD)dir_type)
		return (m_error << DISP_E_BADINDEX);

	switch (dir_type)
	{
	case ePeFile_DataDirectory::eCertificate:
		{
			TDirectoryOffsets::const_iterator it_ = m_offsets.find(dir_type);
			if (it_ == m_offsets.end())
				return (m_error << TYPE_E_ELEMENTNOTFOUND);
			const DWORD dwOffset = it_->second
					+ (DWORD)offsetof(IMAGE_NT_HEADERS, OptionalHeader)
					+ (DWORD)offsetof(IMAGE_DATA_DIRECTORY, Size);

			PBYTE pData = (PBYTE)m_pHeader + dwOffset;
			if (pData)
			{
				::memcpy((void*)pData, &dwSize, sizeof(DWORD));
			}
			this->m_error << __MODULE__ << S_OK;
		} break;
	default:
		m_error << E_NOTIMPL;
	}
	return m_error;
}

const
CSysError& CPeFile_OptionalHeader::Error(void)const
{
	return m_error;
}

CPeFile_ExportTable
           CPeFile_OptionalHeader::ExportTable(void)const
{
	const IMAGE_DATA_DIRECTORY exp_dir = this->Directory(ePeFile_DataDirectory::eExportTable);

	CPeFile_ExportTable exp_table(m_pv_ba);
	m_error = exp_table.Initialize(exp_dir);
	return exp_table;
}

HRESULT    CPeFile_OptionalHeader::Initialize(LPVOID pBaseAddress)
{
	if (!pBaseAddress)
		return (m_error << E_INVALIDARG);
	m_pv_ba = pBaseAddress;
	m_error = PeFile_ResolveHeaderPtr(pBaseAddress, m_pHeader);
	if (m_error)
		return m_error;
	m_opt_header = m_pHeader->OptionalHeader;

	this->m_error << __MODULE__ << S_OK;
	return m_error;
}

bool       CPeFile_OptionalHeader::IsInitialized(void)const
{
	return (NULL != m_pHeader);
}

void       CPeFile_OptionalHeader::Reset(void)
{
	this->m_error << __MODULE__ << S_OK;
	::memset((void*)&m_opt_header, 0, sizeof(IMAGE_OPTIONAL_HEADER));
	m_pHeader = NULL;
}

/////////////////////////////////////////////////////////////////////////////

bool       CPeFile_OptionalHeader::IsDirectoryValid(const IMAGE_DATA_DIRECTORY& dir_ref)
{
	return (dir_ref.Size && dir_ref.VirtualAddress);
}

/////////////////////////////////////////////////////////////////////////////

CPeFile::CPeFile(void) : m_lpBase(NULL)
{
	this->m_error << __MODULE__ << S_OK;
}

CPeFile::~CPeFile(void)
{
	this->Close();
}

/////////////////////////////////////////////////////////////////////////////

HRESULT    CPeFile::Close(void)
{
	this->m_error << __MODULE__ << S_OK;
	m_header.Reset();
	if (!this->IsOpened())
		return S_FALSE;
	if (m_lpBase)
	{
		::UnmapViewOfFile(m_lpBase);
		m_lpBase = NULL;
	}
	m_mapped = reinterpret_cast<HANDLE>(NULL);
	m_file   = reinterpret_cast<HANDLE>(NULL);
	return S_OK;
}

const
CSysError& CPeFile::Error(void)const
{
	return m_error;
}

bool       CPeFile::IsOpened(void)const
{
	return m_file.Is();
}

HRESULT    CPeFile::Open(LPCTSTR pFilePath)
{
	if (!pFilePath || ::_tcslen(pFilePath) < 1)
		return (m_error << E_INVALIDARG);
	this->Close();
	m_file = ::CreateFile(
				pFilePath,
				GENERIC_READ,
				FILE_SHARE_READ,
				NULL,
				OPEN_EXISTING,
				FILE_ATTRIBUTE_NORMAL,
				NULL
			);
	if (!m_file.Is())
		return (m_error = ::GetLastError());
	const DWORD dwFileSize = ::GetFileSize(m_file, NULL);
	m_mapped = ::CreateFileMapping(m_file, NULL, PAGE_WRITECOPY, 0, dwFileSize, NULL);
	if (!m_mapped.Is())
		return (m_error = ::GetLastError());
	m_lpBase = ::MapViewOfFile(m_mapped, FILE_MAP_COPY, 0, 0, 0);
	if (!m_lpBase)
		return (m_error = ::GetLastError());
	else
		this->m_error << __MODULE__ << S_OK;
	m_header.Initialize(m_lpBase);
	return m_error;
}

HRESULT    CPeFile::SaveAs(LPCTSTR pFilePath, const bool bOverride)const
{
	if (!pFilePath || ::_tcslen(pFilePath) < _MAX_DRIVE)
		return E_INVALIDARG;
	if (!this->IsOpened())
		return OLE_E_BLANK;
	CAutoHandle hFile = ::CreateFile(
			pFilePath,
			GENERIC_WRITE,
			0,
			NULL,
			(bOverride ? CREATE_ALWAYS : CREATE_NEW),
			0,
			NULL
		);
	if (!hFile.Is())
		return (m_error = ::GetLastError());
	const DWORD dwFileSize = ::GetFileSize(m_file, NULL);
	if (dwFileSize && m_lpBase)
	{
		DWORD dwBytes = 0;
		if (!::WriteFile(hFile, m_lpBase, dwFileSize, &dwBytes, NULL))
			m_error = ::GetLastError();
		else
			::FlushFileBuffers(m_file);
	}
	return m_error;
}

////////////////////////////////////////////////////////////////////////////

const CPeFile_OptionalHeader& CPeFile::OptionalHeader(void)const
{
	return m_header;
}

CPeFile_OptionalHeader&       CPeFile::OptionalHeader(void)
{
	return m_header;
}