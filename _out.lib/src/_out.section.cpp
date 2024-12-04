/*
	Created by Tech_dog (ebontrop@gmail.com) on 12-Dec-2022 at 09:37:00.2984945, UTC+7, Novosibirsk, Monday;
	This is system console output section interface implementation file;
	-----------------------------------------------------------------------------
	This code is the excerpt from console project of Ebo Pack solution static library code;
*/
#include "_out.section.h"

using namespace _output;

/////////////////////////////////////////////////////////////////////////////

CSection:: CSection (void) {}
CSection:: CSection (const CSection& _ref) : CSection() { *this = _ref; }
CSection:: CSection (LPCTSTR _lp_sz_caption) : CSection() { (*this).Text(_lp_sz_caption); }
CSection::~CSection (void) {}

/////////////////////////////////////////////////////////////////////////////

bool       CSection::Is  (void) const { return CSection::Is(this->Text()); }

HRESULT    CSection::Out (void) const
{
#if (0)
	TBase::m_error >> __MODULE__ << S_OK;

	if (false == CSection::Is(this->Text()))
		return (TBase::m_error << E_INVALIDARG);
#else
	HRESULT hr_ = S_OK;

	if (false == CSection::Is(this->Text()))
		return (hr_ = E_INVALIDARG);
#endif
//	static TCHAR cs_border[78] = {_T('-')}; doesn't work as expected;
	static TCHAR cs_border[78] = {0};
	if (0 == cs_border[0])
		for (size_t i_ = 0; i_ < _countof(cs_border); i_++)
			cs_border[i_] = _T('-');

	::_tprintf(_T("%s\n")  , cs_border);
	::_tprintf(_T("\t%s\n"), this->Text());
	::_tprintf(_T("%s")  , cs_border);
#if (0)
	return TBase::m_error;
#else
	return hr_;
#endif;
}

HRESULT    CSection::Out (LPCTSTR _lp_sz_caption)
{
#if (0)
	TBase::m_error >> __MODULE__ << S_OK;

	if (FAILED(this->Text(_lp_sz_caption)))
		return TBase::m_error;
	else
		return this->Out();
#else
	HRESULT hr_ = this->Text(_lp_sz_caption);

	if (FAILED(hr_))
		return hr_;
	else
		return this->Out();
#endif
}

LPCTSTR    CSection::Text(void) const { return this->m_text.c_str(); }
HRESULT    CSection::Text(LPCTSTR _lp_sz_caption) {
#if (0)
	TBase::m_error >> __MODULE__ << S_OK;

	if (false == CSection::Is(_lp_sz_caption))
		return (TBase::m_error = E_INVALIDARG);

	this->m_text = _lp_sz_caption;
	return TBase::m_error; 
#else
	HRESULT hr_ = (false == CSection::Is(_lp_sz_caption) ? E_INVALIDARG : S_OK);
	if (FAILED(hr_))
		return hr_;

	this->m_text = _lp_sz_caption;
	return hr_;
#endif
}

/////////////////////////////////////////////////////////////////////////////
#if (0)
CSection&  CSection::operator = (const CSection& _ref) { (TBase&)*this = (const TBase&)_ref; return *this; }
#else
CSection&  CSection::operator = (const CSection& _ref) {  *this << _ref.Text(); return *this; }
#endif
CSection&  CSection::operator <<(LPCTSTR _lp_sz_caption) { this->Out(_lp_sz_caption); return *this; }

/////////////////////////////////////////////////////////////////////////////

bool       CSection::Is  (LPCTSTR _lp_sz_text) { return (nullptr != _lp_sz_text && ::_tcslen(_lp_sz_text)); }