#ifndef _SHAREDGENERICCSVPROVIDER_H_7E2C8E5E_F55B_40f8_9A14_AF78E1ABA691_INCLUDED
#define _SHAREDGENERICCSVPROVIDER_H_7E2C8E5E_F55B_40f8_9A14_AF78E1ABA691_INCLUDED
/*
	Created by Tech_dog (ebontrop@gmail.com) on 19-Mar-2014 at 11:44:29am, GMT+4, Taganrog, Wednesday;
	This is Shared Lite Generic CSV data Provider class(es) declaration file.
	-----------------------------------------------------------------------------
	Adopted to v15 on 28-May-2018 at 11:55:06a, UTC+7, Phuket, Rawai, Monday;
	Adopted to v15a on 20-Sep-2018 at 3:00:00p, UTC+7, Novosibirsk, Thursday;
*/
#include "shared.classes.h"
#include "shared.sys.err.h"
#include "shared.run.tpl.h"
#include "shared.raw.buf.h"

#include <cstdio>
#include <stdio.h>
#include <stdlib.h>

namespace shared { namespace log {

	using shared::sys_core::CSysError;
	using shared::sys_core::CThreadBase;
	using shared::sys_core::string_t;
	using shared::sys_core::CFormatter ;

	class CsvFileOption
	{
	public:
		enum _ops {
			eNone        = 0x0, // none option is applied;
			eComma       = 0x1, // comma symbol is used as a value separator;
			eSemicolon   = 0x2, // semicolon is useds as a value separator;
			eTab         = 0x3, // tab symbol is used as a value separator; this is default, if no option is provided;
		};
	public:
		static LPCTSTR   EnumToText(const _ops);
	};

	typedef CsvFileOption::_ops       TSeparator;

	typedef ::std::vector<string_t>   TCsvHeader;
	typedef ::std::vector<string_t>   TCsvRecord;
	typedef ::std::vector<TCsvRecord> TCsvTable;

	class CCsvHeader {
	public:
		 CCsvHeader(void);
		~CCsvHeader(void);

	public:
		HRESULT         Append  (LPCTSTR)   ; // appends new field to a header; no coincidence is checked;
		VOID            Clear   (void)      ; // clears a header;
		UINT            Count   (void) const; // gets field count;
		bool            IsEmpty (void) const; // checks for emptiness;
		const
		TCsvHeader&     RawData (void) const; // gets header raw data (read-only);
		TCsvHeader&     RawData (void)      ; // gets header raw data (read-write); (not recommended);
		string_t        ToString(const TSeparator) const;

	private:
		TCsvHeader      m_raw_header;
	};

	class CCsvTable {
	public:
		 CCsvTable(CCsvHeader&);
		~CCsvTable(void);

	public:
		HRESULT         Append  (const TCsvRecord&);
		LPCTSTR         Cell    (const UINT _row, const UINT _col) const;
		VOID            Clear   (void);
		DOUBLE          Double  (const UINT _row, const UINT _col) const;
		const
		TCsvRecord&     Record  (const UINT _ndx) const;
		TCsvRecord&     Record  (const UINT _ndx)      ;
		ULONG           RowCount(void) const;
		const
		TCsvTable&      Records (void) const;
		TCsvTable&      Records (void)      ;

	private:
		CCsvHeader&     m_head_ref;
		TCsvTable       m_recs;
	};

	class CCsvLocator {
	public:
		enum _oper {
			eUnspecified  = 0x0,
			eOpenForRead  = 0x1,
			eOpenForWrite = 0x2,
		};

	public:
		 CCsvLocator(void);
		~CCsvLocator(void);

	public:
		HRESULT         Clear (void)       ; // closes file handle;
		HRESULT         Create(void)       ; // creates a file for path specified; for writing data;
		HANDLE          Detach(void)       ; // detaches a handle;
		TSysErrRef      Error (void)  const; // gets last error object references;
		bool            IsValid(void) const; // validates locator state;
		HRESULT         Open  (const _oper); // opens a file by path specified;
		LPCTSTR         Path  (void)  const; // gets file path;
		HRESULT         Path  (LPCTSTR)    ; // sets file path; acceptable value length is _MAX_DRIVE * 2 or greater;
		DWORD           Size  (void)  const; // gets file size;

	public:
		operator const HANDLE(void)  const;

	public:
		static bool     IsExist(LPCTSTR lpszFilePath);

	private:
		CSysError       m_error;             // last error object;
		string_t        m_path ;             // source/target file path;
		HANDLE          m_file ;             // open/created file handle;
	};

	class CCsvFile {
		// https://tools.ietf.org/html/rfc4180
	public:
		 CCsvFile(const CsvFileOption::_ops = CsvFileOption::eNone);
		~CCsvFile(void);

	public:
		HRESULT         Append(const CCsvLocator&, const TCsvRecord&);   // appends record(s) to open file provided;
		HRESULT         Clear (void);                                    // clears header and table objects;
		HRESULT         Create(const string_t& _data, const bool bHasHeader = false);
		TSysErrRef      Error (void) const;
		HRESULT         Load  (const CCsvLocator&, const bool bHasHeader = false);
		HRESULT         Save  (const CCsvLocator&, const bool bHasHeader = false);

	public:
		static HRESULT  Create(const CCsvLocator&, const CCsvHeader&, const CsvFileOption::_ops);

	private:
		mutable
		CSysError       m_error;
		TSeparator      m_ops;
		CCsvHeader      m_header;
		CCsvTable       m_table ;
	};

}}

#endif/*_SHAREDGENERICCSVPROVIDER_H_7E2C8E5E_F55B_40f8_9A14_AF78E1ABA691_INCLUDED*/