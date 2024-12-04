#ifndef _SHAREDPECOMMONDEFINITIONS_H_68A9C71C_AB5D_4dd7_8464_DC80995D3712_INCLUDED
#define _SHAREDPECOMMONDEFINITIONS_H_68A9C71C_AB5D_4dd7_8464_DC80995D3712_INCLUDED
/*
	Created by Tech_dog (ebontrop@gmail.com) on 19-Apr-2015 at 17:30:56.4037669, UTC+3, Taganrog, Sunday;
	This is Shared PE (portable executable) format library commmon interface declaration file.
	https://learn.microsoft.com/en-us/windows/win32/debug/pe-format ;
*/
#include "shared.sys.err.h"

namespace shared { namespace sys_file
{
	using shared::sys_core::CSysError;

	HRESULT PeFile_ResolveAddressPtr(
			const PVOID pvBaseAddress,
			const PIMAGE_SECTION_HEADER,
			const DWORD dwRvaToResolve,
			PBYTE& pbPhysicalAddress
		);
	HRESULT PeFile_ResolveAddressPtr(
			const PVOID pvBaseAddress,
			const PIMAGE_NT_HEADERS,
			const DWORD dwRvaToResolve,
			PBYTE& pbPhysicalAddress
		);
	HRESULT PeFile_ResolveHeaderPtr(
			const PVOID pvBaseAddress,
			PIMAGE_NT_HEADERS&
		);
	HRESULT PeFile_ResolveSectionPtr(
			const PVOID pvBaseAddress,
			const PIMAGE_NT_HEADERS,
			const DWORD dwRvaToResolve,
			PIMAGE_SECTION_HEADER&
		);
}}

#endif/*_SHAREDPECOMMONDEFINITIONS_H_68A9C71C_AB5D_4dd7_8464_DC80995D3712_INCLUDED*/