/*
	Created by Tech_dog (ebontrop@gmail.com) on 20-Apr-2015 at 00:00:00.0, UTC+3, Taganrog, Monday;
	This is Shared PE (portable executable) Format library tables' class(es) implementation file.
*/
#include "Shared_PE_Tables.h"

using namespace shared::sys_file;

/////////////////////////////////////////////////////////////////////////////

CPeFile_TableBase:: CPeFile_TableBase(PVOID pvBaseAddress) : m_pv_ba(pvBaseAddress), m_pSection(nullptr) {}
CPeFile_TableBase::~CPeFile_TableBase(void) {}

/////////////////////////////////////////////////////////////////////////////

HRESULT    CPeFile_TableBase::Initialize(const IMAGE_DATA_DIRECTORY& dir_ref)
{
	PIMAGE_NT_HEADERS pHeader = NULL;
	m_error = PeFile_ResolveHeaderPtr(m_pv_ba, pHeader);
	if (m_error)
		return m_error;

	m_pSection = NULL;
	m_error = PeFile_ResolveSectionPtr(m_pv_ba, pHeader, dir_ref.VirtualAddress, m_pSection);

	return m_error;
}

/////////////////////////////////////////////////////////////////////////////

TSysErrRef CPeFile_TableBase::Error(void) const { return this->m_error; }

/////////////////////////////////////////////////////////////////////////////

CPeFile_ExportTable:: CPeFile_ExportTable(PVOID pvBaseAddress) : CPeFile_TableBase(pvBaseAddress) { }
CPeFile_ExportTable::~CPeFile_ExportTable(void) {}

/////////////////////////////////////////////////////////////////////////////

HRESULT CPeFile_ExportTable::Initialize(const IMAGE_DATA_DIRECTORY& dir_ref)
{
	if (!m_fn_names.empty())m_fn_names.clear();
	if (!m_ex_module.empty())m_ex_module.clear();

	HRESULT hr_ = CPeFile_TableBase::Initialize(dir_ref);
	if (S_OK != hr_)
		return  hr_;

	PBYTE pbTablePtr = NULL;
	m_error = PeFile_ResolveAddressPtr(m_pv_ba, m_pSection, dir_ref.VirtualAddress, pbTablePtr);
	if (m_error)
		return m_error;

	PIMAGE_EXPORT_DIRECTORY pExportDirectory = reinterpret_cast<PIMAGE_EXPORT_DIRECTORY>(pbTablePtr);
	if (!pExportDirectory)
		return (m_error = ERROR_INVALID_DATA);

	PBYTE pbFnNameRvaArray = NULL;
	m_error = PeFile_ResolveAddressPtr(m_pv_ba, m_pSection, pExportDirectory->AddressOfNames, pbFnNameRvaArray);
	if (m_error)
		return m_error;

	const PDWORD pdwFnNameRvaArray = reinterpret_cast<PDWORD>(pbFnNameRvaArray);

	for (DWORD i_ = 0; i_ < pExportDirectory->NumberOfNames; i_++)
	{
		const DWORD dwFnRva = pdwFnNameRvaArray[i_];
		PBYTE pbFnName = NULL;
		m_error = PeFile_ResolveAddressPtr(m_pv_ba, m_pSection, dwFnRva, pbFnName);
		if (m_error)
			break;

		PCHAR pszFn = reinterpret_cast<PCHAR>(pbFnName);
		::std::string cs_fn = pszFn;
		try
		{
			m_fn_names.insert(::std::make_pair(pszFn, cs_fn));
		}
		catch (::std::bad_alloc&)
		{
			m_error << E_OUTOFMEMORY; break;
		}
	}

	PBYTE pbModuleName = NULL;
	m_error = PeFile_ResolveAddressPtr(m_pv_ba, m_pSection, pExportDirectory->Name, pbModuleName);
	if (m_error)
		return m_error;

	::std::string cs_module = reinterpret_cast<PCHAR>(pbModuleName);
	try
	{
		m_ex_module.insert(::std::make_pair(reinterpret_cast<PCHAR>(pbModuleName), cs_module));
	}
	catch (::std::bad_alloc&)
	{
		m_error << E_OUTOFMEMORY;
	}
	return m_error;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT CPeFile_ExportTable::Clear(void)
{
	for (TExportedNames::const_iterator it_ = m_fn_names.begin(); it_ != m_fn_names.end(); ++it_)
	{
		PCHAR pchAddress = it_->first;
		const ::std::string& cs_name = it_->second;

		if (pchAddress && !cs_name.empty())
			::memset((void*)pchAddress, 0, cs_name.size());
	}
	return S_OK;
}

const
TExportedNames& CPeFile_ExportTable::ExportedFunctions(void) const { return m_fn_names; }

HRESULT         CPeFile_ExportTable::ExportedFunctions(const TExportedNames& new_names)
{
	for (TExportedNames::const_iterator it_ = new_names.begin(); it_ != new_names.end(); ++it_)
	{
		PCHAR pchAddress = it_->first;
		const ::std::string& cs_name = it_->second;

		if (pchAddress && !cs_name.empty())
			::memcpy((void*)pchAddress, cs_name.c_str(), cs_name.size());
	}
	return S_OK;
}

const
TExportedNames& CPeFile_ExportTable::ExportedModule(void)const {
	return m_ex_module;
}

HRESULT         CPeFile_ExportTable::ExportedModule(const TExportedNames& exp_mod)
{
	for (TExportedNames::const_iterator it_ = exp_mod.begin(); it_ != exp_mod.end(); ++it_)
	{
		PCHAR pchAddress = it_->first;
		const ::std::string& cs_name = it_->second;

		if (pchAddress && !cs_name.empty())
			::memcpy((void*)pchAddress, cs_name.c_str(), cs_name.size());
	}
	return S_OK;
}