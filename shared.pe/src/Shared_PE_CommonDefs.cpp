/*
	Created by Tech_dog (ebontrop@gmail.com) on 19-Apr-2015 at 17:39:35.8439255, UTC+3, Taganrog, Sunday;
	This is Shared PE (portable executable) library commmon definition(s) implementation file.
*/
#include "Shared_PE_CommonDefs.h"

namespace shared { namespace sys_file
{
	HRESULT PeFile_ResolveAddressPtr(
			const PVOID pvBaseAddress,
			const PIMAGE_SECTION_HEADER pSection,
			const DWORD dwRvaToResolve,
			PBYTE& pbPhysicalAddress
		)
	{
		CSysError err_obj((DWORD)S_OK);
		if (!pSection || !pvBaseAddress || !dwRvaToResolve)
			return (err_obj << E_INVALIDARG);
		if (pbPhysicalAddress)
			return (err_obj = ERROR_OBJECT_ALREADY_EXISTS);

		pbPhysicalAddress = reinterpret_cast<PBYTE>(pvBaseAddress)
					+ pSection->PointerToRawData
					- pSection->VirtualAddress
					+ dwRvaToResolve;

		return err_obj;
	}

	HRESULT PeFile_ResolveAddressPtr(
			const PVOID pvBaseAddress,
			const PIMAGE_NT_HEADERS pHeader,
			const DWORD dwRvaToResolve,
			PBYTE& pbPhysicalAddress
		)
	{
		PIMAGE_SECTION_HEADER pSection = NULL;
		HRESULT hr_ = PeFile_ResolveSectionPtr(pvBaseAddress, pHeader, dwRvaToResolve, pSection);
		if (S_OK != hr_)
			return  hr_;

		hr_ = PeFile_ResolveAddressPtr(pvBaseAddress, pSection, dwRvaToResolve, pbPhysicalAddress);
		return hr_;
	}

	HRESULT PeFile_ResolveHeaderPtr(
			const PVOID pvBaseAddress,
			PIMAGE_NT_HEADERS& pHeader_ref
		)
	{
		CSysError err_obj((DWORD)S_OK);
		if (!pvBaseAddress)
			return (err_obj <<E_INVALIDARG);
		if (pHeader_ref)
			return (err_obj = ERROR_OBJECT_ALREADY_EXISTS);

		PIMAGE_DOS_HEADER p_dos_header = reinterpret_cast<PIMAGE_DOS_HEADER>(pvBaseAddress);
		if (!p_dos_header)
			return (err_obj = ERROR_INVALID_DATA);
		if (IMAGE_DOS_SIGNATURE != p_dos_header->e_magic)
			return (err_obj = ERROR_BAD_FORMAT);

		pHeader_ref  = reinterpret_cast<PIMAGE_NT_HEADERS>((PBYTE)(pvBaseAddress) + p_dos_header->e_lfanew);
		if (!pHeader_ref)
			return (err_obj = ERROR_INVALID_DATA);
		if (IMAGE_NT_SIGNATURE != pHeader_ref->Signature)
			return (err_obj = ERROR_BAD_FORMAT);

		return err_obj;
	}

	HRESULT PeFile_ResolveSectionPtr(
			const PVOID pvBaseAddress,
			const PIMAGE_NT_HEADERS pHeader,
			const DWORD dwRvaToResolve,
			PIMAGE_SECTION_HEADER& pSection_ref
		)
	{
		CSysError err_obj;
		if (!pHeader || !pvBaseAddress || !dwRvaToResolve)
			return (err_obj <<E_INVALIDARG);
		if (pSection_ref)
			return (err_obj = ERROR_OBJECT_ALREADY_EXISTS);

		PIMAGE_SECTION_HEADER pSections = reinterpret_cast<PIMAGE_SECTION_HEADER>(
				reinterpret_cast<PBYTE>(&pHeader->OptionalHeader) + pHeader->FileHeader.SizeOfOptionalHeader
			);
		if (!pSections)
			return (err_obj = ERROR_BAD_FORMAT);

		for (DWORD i_ = 0; i_ < pHeader->FileHeader.NumberOfSections; i_++)
		{
			const PIMAGE_SECTION_HEADER pSection = &pSections[i_];

			if (pSection->VirtualAddress <= dwRvaToResolve
				&& dwRvaToResolve < pSection->VirtualAddress + pSection->Misc.VirtualSize)
			{
				pSection_ref = pSection;
			}
		}
		if (!pSection_ref)
			err_obj = ERROR_INVALID_ADDRESS;
		return err_obj;
	}
}}