/*
	Created by Tech_dog (ebontrop@gmail.com) at 19-Mar-2014 at 12:12:15pm, GMT+4, Taganrog, Wednesday;
	This is Shared Lite Generic CSV data Provider class(es) implementation file.
	-----------------------------------------------------------------------------
	Adopted to v15 on 28-May-2018 at 12:15:53p, UTC+7, Phuket, Rawai, Monday;
	Adopted to v15a on 20-Sep-2018 at 3:13:13p, UTC+7, Novosibirsk, Thursday;
*/
#include "log.csv.h"

using namespace shared::sys_core;
using namespace shared::common;

using namespace shared::log;

/////////////////////////////////////////////////////////////////////////////

namespace shared { namespace log { namespace _impl
{
	const LPCTSTR CsvFile__Csv_Sep_Token  = _T("\t");
	const LPCTSTR CsvFile__Csv_Row_Token  = _T("\n");
	const LPCTSTR CsvFile__Csv_End_Token  = _T("\r");
	const LPCTSTR CsvFile__Csv_Empty_Data = _T("n/a");
	const LPCTSTR CsvFile__Csv_Sep_Token2 = _T(";");
	const LPCTSTR CsvFile__Csv_Sep_Token4 = _T(",");

}}}
using namespace shared::log::_impl;
/////////////////////////////////////////////////////////////////////////////

LPCTSTR CsvFileOption::EnumToText(const _ops e_ops) {

	if (_ops::eSemicolon == e_ops) return CsvFile__Csv_Sep_Token2;
	if (_ops::eComma     == e_ops) return CsvFile__Csv_Sep_Token4;

	return CsvFile__Csv_Sep_Token;
}

/////////////////////////////////////////////////////////////////////////////

CCsvHeader:: CCsvHeader(void) {}
CCsvHeader::~CCsvHeader(void) {}

/////////////////////////////////////////////////////////////////////////////

HRESULT     CCsvHeader::Append  (LPCTSTR lpszField) {

	HRESULT hr_ = S_OK;
	if (NULL == lpszField || !::_tcslen(lpszField))
		return (hr_ = E_INVALIDARG);

	try {
		m_raw_header.push_back(string_t(lpszField));
	}
	catch (::std::bad_alloc&) {
		hr_ = E_OUTOFMEMORY;
	}

	return  hr_;
}

VOID        CCsvHeader::Clear   (void) {
	if (m_raw_header.empty() == false)
		m_raw_header.clear();
}

UINT        CCsvHeader::Count   (void) const { return static_cast<UINT>(m_raw_header.size()); }
bool        CCsvHeader::IsEmpty (void) const { return (0 == this->Count()); }
const
TCsvHeader& CCsvHeader::RawData (void) const { return m_raw_header; }
TCsvHeader& CCsvHeader::RawData (void)       { return m_raw_header; }

string_t    CCsvHeader::ToString(const CsvFileOption::_ops _sep) const {

	string_t cs_head;
	string_t cs_sep = CsvFileOption::EnumToText(_sep);

	for (size_t i_ = 0; i_ < m_raw_header.size() ; i_++) {

		string_t cs_pat = CFormatter().Accept(
			_T("[%s]"), m_raw_header[i_].c_str()
		);
		cs_head += cs_pat;
		if (i_ < m_raw_header.size() - 1)
			cs_head += cs_sep;
	}
	cs_head += CsvFile__Csv_Row_Token;

	return cs_head;
}

/////////////////////////////////////////////////////////////////////////////

CCsvTable:: CCsvTable(CCsvHeader& _head_ref) : m_head_ref(_head_ref) {}
CCsvTable::~CCsvTable(void) {}

/////////////////////////////////////////////////////////////////////////////

HRESULT     CCsvTable::Append  (const TCsvRecord& _rec) {

	HRESULT hr_ = S_OK;
	if (m_head_ref.Count() != _rec.size())
		return (hr_ = HRESULT_FROM_WIN32(ERROR_INVALID_DATA));

	try {
		m_recs.push_back(_rec);
	}
	catch (::std::bad_alloc&) {
		hr_ = E_OUTOFMEMORY;
	}

	return  hr_;
}

LPCTSTR     CCsvTable::Cell    (const UINT _row, const UINT _col) const {

	if (_row > this->RowCount() - 1 || _col > m_head_ref.Count() - 1)
		return NULL;

	const TCsvRecord& rec_ = m_recs[_row];
	
	const string_t& val_ = rec_[_col];
	return  val_.c_str();

}

VOID        CCsvTable::Clear   (void) {
	if (m_recs.empty() == false)
		m_recs.clear();
}

DOUBLE      CCsvTable::Double  (const UINT _row, const UINT _col) const { return ::_tstof(this->Cell(_row, _col)); }
const
TCsvRecord& CCsvTable::Record  (const UINT _ndx) const{
	if (_ndx > this->RowCount() - 1) {
		static TCsvRecord na_;
		return na_;
	}
	else
		return m_recs[_ndx];
}

TCsvRecord& CCsvTable::Record  (const UINT _ndx) {
	if (_ndx > this->RowCount() - 1) {
		static TCsvRecord na_;
		return na_;
	}
	else
		return m_recs[_ndx];
}

ULONG       CCsvTable::RowCount(void) const { return static_cast<ULONG>(m_recs.size()); }
const
TCsvTable&  CCsvTable::Records (void) const { return m_recs; }
TCsvTable&  CCsvTable::Records (void)       { return m_recs; }

/////////////////////////////////////////////////////////////////////////////

#include <atlfile.h>
#include <atlstr.h>

namespace shared { namespace log { namespace _impl
{
	// this function is based on assumption that ANSI text (english base character set) is represented
	// by UTF-16; that means the follower byte is always 0x0;
	bool    CsvFile_IsUnicodeData(const CRawData& _data, const DWORD _byte_to_check = 0x64)
	{
		const PBYTE pbData = _data.GetData();
		const DWORD dwSize = _data.GetSize();
		for ( DWORD i_ = 1; i_ < dwSize && i_ < _byte_to_check; i_ += 2)
		{
			if (0 == (pbData[i_ - 1] & 0x80) &&
				0 == (pbData[i_ - 0]))
				return true; // UTF-16 is detected
		}
		return false;
	}

	HRESULT CsvFile_WriteLine(::ATL::CAtlFile& _file, const TCsvRecord& _rec, LPCTSTR _sep)
	{
		HRESULT hr_ = S_FALSE;
		::ATL::CAtlString cs_buffer;

		const LONG nFields = (LONG)_rec.size();
		for ( LONG i_ = 0; i_ < nFields; i_++)
		{
			cs_buffer += _rec[i_].c_str();
			if (i_ < nFields - 1)
				cs_buffer += _sep;
			else
				cs_buffer += CsvFile__Csv_Row_Token;
		}
		if (cs_buffer.IsEmpty() == false)
		{
			const DWORD dwSize = sizeof(TCHAR) * cs_buffer.GetLength();
			hr_ = _file.Write((LPCVOID)cs_buffer.GetBuffer(), dwSize);
		}
		else
			hr_ = HRESULT_FROM_WIN32(ERROR_INVALID_DATA);

		return hr_;
	}

	class CAtlFileEx : public CAtlFile {
	                  typedef CAtlFile TBase;
	public:
		 CAtlFileEx(const CCsvLocator& _loc) { TBase::Attach(_loc); }
		~CAtlFileEx(void) {
			TBase::m_h = NULL;
		}
	};
}}}

/////////////////////////////////////////////////////////////////////////////

CCsvLocator:: CCsvLocator(void) : m_file(NULL) { }
CCsvLocator::~CCsvLocator(void) { this->Clear(); }

/////////////////////////////////////////////////////////////////////////////

HRESULT     CCsvLocator::Clear (void) {
	m_error.Module(_T(__FUNCTION__));
	m_error << S_OK;

	if (NULL == m_file)
		return (m_error << OLE_E_BLANK);

	::CloseHandle(m_file); m_file = NULL;

	return  m_error;
}

HRESULT     CCsvLocator::Create(void) {
	m_error.Module(_T(__FUNCTION__));
	m_error << S_OK;
	if (this->IsValid())
		return (m_error << (DWORD)ERROR_ALREADY_INITIALIZED);

	if (m_path.empty() == true)
		return (m_error << E_INVALIDARG);

	HRESULT hr_ = S_OK;

	CAtlFile file_;
	hr_   =  file_.Create(m_path.c_str(), FILE_WRITE_DATA, FILE_WRITE_DATA, CREATE_ALWAYS);
	if (FAILED(hr_))
		return (m_error << hr_);

	hr_ = file_.Seek(0, FILE_BEGIN);
	if (FAILED(hr_))
		return (m_error << hr_);

	m_file = file_.Detach();

	return m_error;
}

TSysErrRef  CCsvLocator::Error (void) const { return m_error; }

bool        CCsvLocator::IsValid(void) const { return !!m_file; }

HRESULT     CCsvLocator::Open  (const _oper _for) {
	m_error.Module(_T(__FUNCTION__));
	m_error << S_OK;

	if (NULL != m_file)
		return (m_error << (DWORD)ERROR_INVALID_STATE);

	if (m_path.empty())
		return (m_error << (DWORD)ERROR_BAD_PATHNAME);

	HRESULT hr_ = S_OK;

	::ATL::CAtlFile file_;
	if (CCsvLocator::eOpenForRead == _for)
		hr_ = file_.Create(m_path.c_str(), FILE_READ_DATA, FILE_SHARE_READ, OPEN_EXISTING);
	else if (CCsvLocator::eOpenForWrite == _for)
		hr_ = file_.Create(m_path.c_str(), FILE_APPEND_DATA, FILE_SHARE_WRITE, OPEN_ALWAYS);
	else
		hr_ = HRESULT_FROM_WIN32(ERROR_INVALID_ACCESS);

	if (FAILED(hr_))
		return (m_error << hr_);

	m_file = file_.Detach();

	return m_error;
}

LPCTSTR     CCsvLocator::Path  (void)  const { return m_path.c_str(); }
HRESULT     CCsvLocator::Path  (LPCTSTR _v)  {
	m_error.Module(_T(__FUNCTION__));
	m_error << S_OK;

	if (this->IsValid())
		return (m_error << (DWORD)ERROR_INVALID_STATE);

	if (NULL == _v || ::_tcslen(_v) < _MAX_DRIVE * 2) //_max_drive equals 3 including 0-terminator; thus file name should be, as exmple, c:\\nn;
		return (m_error << E_INVALIDARG);
	else
		m_path = _v;

	return m_error;
}

DWORD       CCsvLocator::Size  (void)  const {
	DWORD  dw_sz = 0;
	if (this->IsValid() == false)
		return dw_sz;

	::ATL::CAtlFile file_(m_file);

	HRESULT hr_ = file_.Seek(0, FILE_BEGIN);
	if (SUCCEEDED(hr_)) {
		ULONGLONG ull_sz = 0;
		hr_ = file_.GetSize(ull_sz);
		if (SUCCEEDED(hr_)) {
			dw_sz = (ull_sz & 0xFFFFFFFF);
		}
	}

	file_.m_h = NULL;
	
	return dw_sz;
}

/////////////////////////////////////////////////////////////////////////////

CCsvLocator::operator const HANDLE(void)  const { return m_file; }

/////////////////////////////////////////////////////////////////////////////

bool       CCsvLocator::IsExist(LPCTSTR lpszFilePath)
{
	::ATL::CAtlFile file_;
	HRESULT hr_  =  file_.Create(lpszFilePath, FILE_READ_DATA, FILE_WRITE_DATA, OPEN_EXISTING);
	return !!SUCCEEDED(hr_);
}

/////////////////////////////////////////////////////////////////////////////

CCsvFile:: CCsvFile (const CsvFileOption::_ops eOption) : m_table(m_header), m_ops(eOption) { }
CCsvFile::~CCsvFile(void) { this->Clear(); }

/////////////////////////////////////////////////////////////////////////////

HRESULT    CCsvFile::Append(const CCsvLocator& _loc, const TCsvRecord& _rec) {

	m_error.Module(_T(__FUNCTION__));
	m_error << S_OK;

	if (_loc.IsValid() == false)
		return (m_error << E_INVALIDARG);

	CAtlFileEx file_(_loc);

	HRESULT hr_ = file_.Seek(0, FILE_END);
	if (FAILED(hr_))
		return (m_error = hr_);

	LPCTSTR lpsz_sep = CsvFileOption::EnumToText(m_ops);
	hr_ = CsvFile_WriteLine(file_, _rec, lpsz_sep);

	if (SUCCEEDED(hr_))
		hr_ = file_.Flush();
	else
		m_error << hr_;

	return m_error;
}

HRESULT    CCsvFile::Clear (void)
{
	m_error << OLE_E_BLANK;
	m_header.Clear();
	m_table.Clear();

	return S_OK;
}

HRESULT    CCsvFile::Create(const string_t& _data, const bool bHasHeader) {

	m_error.Module(_T(__FUNCTION__));
	m_error << S_OK;

	this->Clear();

	if (true == _data.empty())
		return (m_error << (DWORD)ERROR_INVALID_DATA);

	CAtlString data_(_data.c_str());

	LPCTSTR lpsz_sep = CsvFileOption::EnumToText(m_ops);
	try
	{
		INT row_ndx = NULL;
		::ATL::CAtlString row_token;
		row_token = data_.Tokenize(CsvFile__Csv_Row_Token, row_ndx);

		bool bHeaderIsInitialized = false;

		while (row_token.IsEmpty() == false)
		{
			TCsvRecord row_data;
			INT col_ndx = NULL;
			::ATL::CAtlString col_token;
			col_token = row_token.Tokenize(lpsz_sep, col_ndx);

			while (col_token.IsEmpty() == false)
			{
				row_data.push_back(string_t(col_token.GetString()));
				col_token = row_token.Tokenize(lpsz_sep, col_ndx);
			}

			if (bHasHeader && !bHeaderIsInitialized)
			{
				bHeaderIsInitialized = true;
				m_header.RawData() = row_data;
			}
			else {
				m_table.Append(row_data);
			}
			row_token = data_.Tokenize(CsvFile__Csv_Row_Token, row_ndx);
		}
	}
	catch(::std::bad_alloc&) {
		m_error << E_OUTOFMEMORY;
	}

	return m_error;
}

TSysErrRef CCsvFile::Error (void) const { return m_error; }

HRESULT    CCsvFile::Load(const CCsvLocator& _loc, const bool bHasHeader) {

	m_error.Module(_T(__FUNCTION__));
	m_error << S_OK;

	if (_loc.IsValid() == false)
		return (m_error << E_INVALIDARG);

	const DWORD n_sz  = _loc.Size();

	CRawData raw_data(n_sz);
	if (raw_data.IsValid() == false)
		return (m_error = raw_data.Error());

	CAtlFileEx file_(_loc);

	HRESULT hr_ = file_.Read((LPVOID)raw_data.GetData(), raw_data.GetSize());
	if (FAILED(hr_))
		return (m_error = hr_);

	const bool bIsUnicode = CsvFile_IsUnicodeData(raw_data);

	string_t cs_buffer = reinterpret_cast<TCHAR*>(raw_data.GetData());

	hr_ = this->Create(cs_buffer, bHasHeader);
	return hr_;
}

HRESULT    CCsvFile::Save(const CCsvLocator& _loc, const bool bHasHeader) {

	m_error.Module(_T(__FUNCTION__));
	m_error << S_OK;

	if (_loc.IsValid() == false)
		return (m_error << E_INVALIDARG);

	CAtlFileEx file_(_loc);

	HRESULT hr_ = file_.Seek(0, FILE_BEGIN);
	if (FAILED(hr_))
		return (m_error << hr_);

	LPCTSTR lpsz_sep = CsvFileOption::EnumToText(m_ops);
	if (bHasHeader && m_header.IsEmpty() == false) {

		hr_ = CsvFile_WriteLine(file_, m_header.RawData(), lpsz_sep);
		if (FAILED(hr_))
			return hr_;
	}

	const ULONG nRows = m_table.RowCount();
	for ( ULONG i_ = 0; i_ < nRows; i_++)
	{
		const TCsvRecord& rec_ = m_table.Record(i_);
		hr_ = CsvFile_WriteLine(file_, rec_, lpsz_sep);
		if (FAILED(hr_))
			return hr_;
	}

	hr_ = file_.Flush();
	if (FAILED(hr_))
		m_error << hr_;

	return m_error;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT    CCsvFile::Create(const CCsvLocator& _loc, const CCsvHeader& _head, const CsvFileOption::_ops e_ops) {

	HRESULT hr_ = S_OK;
	if (_loc.IsValid() == false)
		return (hr_ = _loc.Error());

	if (_head.IsEmpty())
		return (hr_ = E_INVALIDARG);

	CAtlFileEx file_(_loc);

	hr_ = file_.Seek(0, FILE_BEGIN);
	if (FAILED(hr_))
		return hr_;

	LPCTSTR lpsz_sep = CsvFileOption::EnumToText(e_ops);
	hr_ = CsvFile_WriteLine(file_, _head.RawData(), lpsz_sep);
	return hr_;
}