#ifndef _GENERICSTGDATA_H_4916FDBD_664D_48f6_A467_B1F03F356B69_INCLUDED
#define _GENERICSTGDATA_H_4916FDBD_664D_48f6_A467_B1F03F356B69_INCLUDED
/*
	Created by Tech_dog (ebontrop@gmail.com) on 22-Apr-2010 at 1:56:43am, GMT+3, Rostov-on-Don, Thursday;
	This is SfxSOA package shared library generic raw data class declaration file.
	-----------------------------------------------------------------------------
	Adopted to Ebo Pack solution on 28-May-2018 at 11:58:22a, UTC+7, Phuket, Rawai, Monday;
	-----------------------------------------------------------------------------
	Adopted to Process Prying Eyes project on 12-Feb-2023 at 12:11:10.5567566, UTC+7, Novosibirsk, Sunday;
*/
#include <vector>
#include "shared.sys.err.h"

namespace shared { namespace common
{
	using shared::sys_core::CSysError;

	typedef const PBYTE PCBYTE;

	class CRawBuffer {
	protected:
		 CRawBuffer(void);
		~CRawBuffer(void);

	public:
		DWORD  Clear  (void);
		bool   IsEmpty(void) const;
		bool   IsValid(void) const;
	protected:
		mutable
		CSysError m_error ;
		PBYTE     m_pData ;
		DWORD     m_dwSize;
	};
	// this is very simplified/trimmed version of original one;
	class CRawData : public  CRawBuffer {
	                 typedef CRawBuffer TBuffer;
	public:
		 CRawData (void);
		 CRawData (const CRawData&);
		 CRawData (const DWORD dwSize);
		~CRawData (void);

	public:
		DWORD       Append (const PBYTE pData, const DWORD dwSize); // nullptr is acceptable for pData address;
		DWORD       Create (const DWORD dwSize);
		DWORD       Create (const PBYTE pData, const DWORD dwSize);
		LPBYTE      Detach (void)      ;
		TSysErrRef  Error  (void) const;
		PCBYTE      GetData(void) const;
		PBYTE       GetData(void)      ;
		DWORD       GetSize(void) const;
		DWORD       Reset  (void)      ;

	public:
		operator const    bool (void)const;              // returns a validity state of the object;
		operator const    DWORD(void)const;              // returns a buffer size;
		operator const    PBYTE(void)const;              // returns a buffer data pointer (ra);
		operator          PBYTE(void)     ;              // returns a buffer data pointer (rw);

	public:
		CRawData& operator = (const CRawData&);
		CRawData& operator = (const ::std::vector<BYTE>&);
	};
}}

typedef shared::common::CRawData  TRawData;

#endif/*_GENERICSTGDATA_H_4916FDBD_664D_48f6_A467_B1F03F356B69_INCLUDED*/