#ifndef _SHAREDPEFORMATTABLES_H_B0CC8CA6_4284_44ce_A2ED_CB54EE4999D9_INCLUDED
#define _SHAREDPEFORMATTABLES_H_B0CC8CA6_4284_44ce_A2ED_CB54EE4999D9_INCLUDED
/*
	Created by Tech_dog (ebontrop@gmail.com) on 5-Jun-2015 at 17:11:50.3277690, UTC+3, Rawai, Friday; 
	This is Shared PE (portable executable) Format library tables' interface declaration file.
*/
#include "shared.classes.h"
#include "shared.sys.err.h"

#include "Shared_PE_CommonDefs.h"

namespace shared { namespace sys_file
{
	using shared::sys_core::CSysError;
	using shared::sys_core::string_t ;

	class CPeFile_TableBase
	{
	protected:
		PVOID                 m_pv_ba;
		CSysError             m_error;
		PIMAGE_SECTION_HEADER m_pSection; // an image section which the table belongs to;
	protected:
		 CPeFile_TableBase(PVOID pvBaseAddress);
		~CPeFile_TableBase(void);
	protected:
		virtual HRESULT       Initialize(const IMAGE_DATA_DIRECTORY&);
	public:
		TSysErrRef            Error(void)const;
	};

	typedef ::std::map<const PCHAR, ::std::string> TExportedNames; // char-based nameing is required; no wide-char string is applicable;

	class CPeFile_ExportTable : public CPeFile_TableBase
	{
	private:
		TExportedNames        m_fn_names;
		TExportedNames        m_ex_module;
	public:
		 CPeFile_ExportTable(PVOID pvBaseAddress);
		~CPeFile_ExportTable(void);
	public:
		virtual HRESULT       Initialize(const IMAGE_DATA_DIRECTORY&) override final;
	public:
		HRESULT               Clear(void);
		const
		TExportedNames&       ExportedFunctions(void)const;
		HRESULT               ExportedFunctions(const TExportedNames&);    // applies new exported names
		const
		TExportedNames&       ExportedModule(void)const;
		HRESULT               ExportedModule(const TExportedNames&);       // applies new exported module's name(s)
	};
}}

#endif/*_SHAREDPEFORMATTABLES_H_B0CC8CA6_4284_44ce_A2ED_CB54EE4999D9_INCLUDED*/