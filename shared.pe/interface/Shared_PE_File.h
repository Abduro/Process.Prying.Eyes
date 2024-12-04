#ifndef _SHAREDLITELIBRARYPEFORMATWRAPPER_H_CF3EB9A7_8301_48d0_AB3D_DE5C291E49B6_INCLUDED
#define _SHAREDLITELIBRARYPEFORMATWRAPPER_H_CF3EB9A7_8301_48d0_AB3D_DE5C291E49B6_INCLUDED
/*
	Created by Tech_dog (ebontrop@gmail.com) on 20-Apr-2015 at 13:54:05.0132604, UTC+3, Taganrog, Monday; 
	This is Shared Lite Library PE (portable executable) Format Wrapper interface declaration file.
*/
#include "shared.sys.err.h"
#include "shared.gen.hand.h"
#include "Shared_PE_CommonDefs.h"
#include "Shared_PE_Tables.h"

namespace shared { namespace sys_file
{
	using shared::sys_core::CSysError;
	using shared::common::CAutoHandle;

	class ePeFile_DataDirectory
	{
	public:
		enum _e{
			eExportTable  = IMAGE_DIRECTORY_ENTRY_EXPORT,
			eImportTable  = IMAGE_DIRECTORY_ENTRY_IMPORT,
			eResource     = IMAGE_DIRECTORY_ENTRY_RESOURCE,
			eException    = IMAGE_DIRECTORY_ENTRY_EXCEPTION,
			eCertificate  = IMAGE_DIRECTORY_ENTRY_SECURITY,
		};
	};

	typedef ::std::map<ePeFile_DataDirectory::_e, DWORD>  TDirectoryOffsets;

	class CPeFile_OptionalHeader {
	public:
		 CPeFile_OptionalHeader(void);
		~CPeFile_OptionalHeader(void);
	public:
		IMAGE_DATA_DIRECTORY   Directory(const ePeFile_DataDirectory::_e) const;
		HRESULT                Directory(const ePeFile_DataDirectory::_e, const DWORD dwSize);
		const CSysError&       Error(void)const;
		CPeFile_ExportTable    ExportTable(void)const;
		HRESULT                Initialize(LPVOID pBaseAddress);
		bool                   IsInitialized(void)const;
		void                   Reset(void);
	public:
		static bool            IsDirectoryValid(const IMAGE_DATA_DIRECTORY&);
	private:
		LPVOID                 m_pv_ba;      // image base address
		TDirectoryOffsets      m_offsets;
		PIMAGE_NT_HEADERS      m_pHeader;
		mutable CSysError      m_error;
		IMAGE_OPTIONAL_HEADER  m_opt_header;
	};

	class CPeFile {
	public:
		 CPeFile(void);
		~CPeFile(void);
	public:
		HRESULT                Close(void);
		const CSysError&       Error(void)const;
		bool                   IsOpened(void)const;
		HRESULT                Open(LPCTSTR pFilePath);
		HRESULT                SaveAs(LPCTSTR pFilePath, const bool bOverride = false)const;
	public:
		const CPeFile_OptionalHeader& OptionalHeader(void)const;
		CPeFile_OptionalHeader&       OptionalHeader(void);
	private: // non-copyable;
		CPeFile(const CPeFile&);
		CPeFile& operator= (const CPeFile&);
	private:
		mutable CSysError      m_error;
		CAutoHandle            m_file;
		CAutoHandle            m_mapped;
		LPVOID                 m_lpBase;
	private:
		CPeFile_OptionalHeader m_header;
	};
}}

#endif/*_SHAREDLITELIBRARYPEFORMATWRAPPER_H_CF3EB9A7_8301_48d0_AB3D_DE5C291E49B6_INCLUDED*/