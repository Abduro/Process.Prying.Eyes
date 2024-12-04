/*
	Created by Tech_dog (ebontrop@gmail.com) on 11-Dec-2022 at 10:04:31.2533034, UTC+7, Novosibirsk, Sunday;
	This is system console output record implementation file;
	-----------------------------------------------------------------------------
	This code is the excerpt from console project of Ebo Pack solution static library code;
*/
#include "_out.record.h"

using namespace _output;

/////////////////////////////////////////////////////////////////////////////

namespace _output { namespace _impl {

	class CTimestamp  {
	public:
		 CTimestamp (void) {}
		~CTimestamp (void) {}

	public:
		string_t  out (void) const {

			static LPCTSTR  lp_sz_tm_pat = _T("%02d/%02d/%02d %02d:%02d:%02d.%03d");
			SYSTEMTIME st = { 0 };
			::GetLocalTime(&st);

			return  string_t( CFormatter().Accept(
				lp_sz_tm_pat,
				st.wMonth   ,
				st.wDay     ,
				st.wYear%100,  // only 2 digits
				st.wHour    ,
				st.wMinute  ,
				st.wSecond  ,
				st.wMilliseconds)
			);
			
		}
	};

}}
using namespace _output::_impl;
/////////////////////////////////////////////////////////////////////////////

CRecord:: CRecord (void) : m_rec_type(TRecType::eInfo) {}
CRecord:: CRecord (const CRecord& _ref) : CRecord() { *this = _ref; }
CRecord:: CRecord (TRecordType _type, LPCTSTR _lp_sz_text) : CRecord() { *this << _type << _lp_sz_text; }
CRecord::~CRecord (void) {}

/////////////////////////////////////////////////////////////////////////////

bool      CRecord::IsValid (void) const { return (nullptr != this->Text() && ::_tcslen(this->Text())); }

LPCTSTR   CRecord::Text (void) const { return this->m_rec_text.c_str(); }
HRESULT   CRecord::Text (LPCTSTR _lp_sz_text)
{
	HRESULT hr_ = S_OK;

	if (nullptr == _lp_sz_text || 0 == ::_tcslen(_lp_sz_text))
		return (hr_ = E_INVALIDARG);

	this->m_rec_text = _lp_sz_text;

	return (hr_);
}

TRecType  CRecord::Type (void) const { return this->m_rec_type; }
HRESULT   CRecord::Type (TRecType _type)
{
	HRESULT hr_ = (_type != this->m_rec_type ? S_OK : S_FALSE);

	this->m_rec_type = _type;

	return  hr_;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT   CRecord::Out(void) const
{
	HRESULT hr_ = S_OK;
	if (false == this->IsValid())
		return (hr_ = OLE_E_BLANK);

	string_t cs_out;
	switch(this->Type())
	{
	case TRecordType::eError    : cs_out = CFormatter().Accept(_T("\n\t[#ERR] [%s] %s"), CTimestamp().out().c_str(), this->Text()); break;
	case TRecordType::eInfo     : cs_out = CFormatter().Accept(_T("\n\t[INFO] [%s] %s"), CTimestamp().out().c_str(), this->Text()); break;
	case TRecordType::eWarning  : cs_out = CFormatter().Accept(_T("\n\t[WARN] [%s] %s"), CTimestamp().out().c_str(), this->Text()); break;
	}
	::_tprintf(cs_out.c_str());
	
	return hr_;
}

/////////////////////////////////////////////////////////////////////////////

CRecord&  CRecord::operator = (const CRecord& _ref) { *this << _ref.Type() << _ref.Text(); return *this; }
CRecord&  CRecord::operator <<(LPCTSTR _lp_sz_text) {  this->Text(_lp_sz_text); return *this; }
CRecord&  CRecord::operator <<(TRecordType   _type) {  this->Type(_type); return *this; }

/////////////////////////////////////////////////////////////////////////////

CRecord::operator LPCTSTR (void) const { return this->Text(); }
CRecord::operator TRecType(void) const { return this->Type(); }

/////////////////////////////////////////////////////////////////////////////

CRec_Err:: CRec_Err (LPCTSTR _lp_sz_text) : TBase(TRecType::eError, _lp_sz_text) { /*TBase::Out();*/ }
CRec_Err:: CRec_Err (TSysErrRef _error) : TBase(TRecType::eError, (string_t(_T("Error details:")) + _error.To_string(_T("\n\t\t"))).c_str()) { TBase::Out(); }
CRec_Err::~CRec_Err (void) {}

/////////////////////////////////////////////////////////////////////////////

CRec_Info:: CRec_Info (LPCTSTR _lp_sz_text) : TBase(TRecType::eInfo, _lp_sz_text) { /*TBase::Out();*/ } // this is the error that leads to duplication of output;
CRec_Info::~CRec_Info (void) {}

/////////////////////////////////////////////////////////////////////////////

CRec_Warn:: CRec_Warn (LPCTSTR _lp_sz_text) : TBase(TRecType::eWarning, _lp_sz_text) { /*TBase::Out();*/ }
CRec_Warn::~CRec_Warn (void) {}

/////////////////////////////////////////////////////////////////////////////
