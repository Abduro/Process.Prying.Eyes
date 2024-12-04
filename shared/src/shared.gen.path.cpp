/*
	Created by Tech_dog (ebontrop@gmail.com) on 11-Feb-2023 at 12:56:22.4894771, UTC+7, Novosibirsk, Saturday;
	This is Process Prying Eyes project shared generic path interface implementation file;
*/
#include "shared.gen.path.h"

using namespace shared::sys_core;

/////////////////////////////////////////////////////////////////////////////

CGenericPath:: CGenericPath (void) { m_error << __MODULE__ << S_OK; }
CGenericPath:: CGenericPath (const CGenericPath& _ref) : CGenericPath() { *this = _ref; }
CGenericPath:: CGenericPath (LPCTSTR _lp_sz_path) : CGenericPath() { *this << _lp_sz_path; }
CGenericPath::~CGenericPath (void) {}

/////////////////////////////////////////////////////////////////////////////

TSysErrRef     CGenericPath::Error (void) const { return this->m_error; }

bool   CGenericPath::Exists (void) const
{
	m_error << __MODULE__ << S_OK;
	// https://learn.microsoft.com/en-us/windows/win32/api/shlwapi/nf-shlwapi-pathfileexistsa ;
	if (false == !!::PathFileExists(this->Get()))
		this->m_error.Last();

	return !(bool)m_error;
}

bool   CGenericPath::IsFile (void) const
{
	m_error << __MODULE__ << S_OK;

	// https://learn.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-findfirstfilea ;
	WIN32_FIND_DATA data_ = {0};
	HANDLE hFind = ::FindFirstFile(this->Get(), &data_);

	if (INVALID_HANDLE_VALUE == hFind)
		return (bool)m_error << ERROR_FILE_NOT_FOUND;

	::FindClose(hFind);

	return true;
}

bool   CGenericPath::IsFolder (void) const
{
	m_error << __MODULE__ << S_OK;
	// https://learn.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-getfileattributesa ;
	const DWORD atts_ = ::GetFileAttributes(m_path.c_str());

	if (INVALID_FILE_ATTRIBUTES == atts_) {
		m_error.Last();
		return false;
	}

	return (0 != (FILE_ATTRIBUTE_DIRECTORY & atts_));
}

/////////////////////////////////////////////////////////////////////////////

LPCTSTR CGenericPath::Get (void) const { return this->m_path.c_str(); }
bool    CGenericPath::Set (LPCTSTR _lp_sz_path)
{
	m_error << __MODULE__ << S_OK;

	if (nullptr == _lp_sz_path || 0 == ::_tcslen(_lp_sz_path))
		return (bool)(m_error << __d(ERROR_INVALID_PARAMETER));
	
	this->m_path = _lp_sz_path;

	return !(bool)m_error;
}

/////////////////////////////////////////////////////////////////////////////

CGenericPath&  CGenericPath::operator = (const CGenericPath& _ref) { *this << _ref.Get(); this->m_error = _ref.m_error; return *this; }
CGenericPath&  CGenericPath::operator <<(LPCTSTR _lp_sz_path) { this->Set(_lp_sz_path); return *this; }

/////////////////////////////////////////////////////////////////////////////