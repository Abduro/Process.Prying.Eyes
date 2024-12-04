/*
	Created by Tech_dog (ebontrop@gmail.com) on 10-Sep-2020 at 6:58:28p, UTC+7, Novosibirsk, Thursday;
	This is FakeGPS driver version 2 desktop client console interface implementation file;
	-----------------------------------------------------------------------------
	Adopted to Process Prying Eyes project on 21-Feb-2023 at 11:15:40.9403714, UTC+7, Novosibirsk, Tuesday;
	-----------------------------------------------------------------------------
	This code is the excerpt from console project of Ebo Pack solution static library code;
*/
#include "_out.con.h"

using namespace _output;

namespace _output { namespace _impl {
}}

using namespace _output::_impl;

/////////////////////////////////////////////////////////////////////////////

CState:: CState (void) {}
CState:: CState (const CState& _ref) : CState () { *this = _ref; }
CState::~CState (void) {}

/////////////////////////////////////////////////////////////////////////////

HRESULT  CState::Close  (void) const
{
	HRESULT hr_ = S_OK;
	const bool b_verb = true;
	if (b_verb) {
		::_tprintf(_T("\n\n\tPress any key or click [x] button to exit;")); // https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/printf-printf-l-wprintf-wprintf-l ;
		::_tprintf(_T("\n"));
		::_gettch();                                                        // https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/getch-getwch                      ;
	}
	return  hr_;
}

HRESULT  CState::Create (LPCTSTR _lp_sz_cap/*, LPCTSTR _lp_sz_title*/) const
{
	HRESULT hr_ = S_OK;
	_lp_sz_cap;
#if (0)
	if (!::AllocConsole())
		return (hr_ = HRESULT_FROM_WIN32(::GetLastError()));
#endif
	if (nullptr == _lp_sz_cap || 0 == ::_tcslen(_lp_sz_cap))
		return (hr_ = E_INVALIDARG);
	else
		::SetConsoleTitle(_lp_sz_cap);
#if (0)
	CONSOLE_SCREEN_BUFFER_INFO info_ = {0};
	::EnableMenuItem(::GetSystemMenu(::GetConsoleWindow(), FALSE), SC_CLOSE , MF_GRAYED);

	::DrawMenuBar(::GetConsoleWindow());
	::GetConsoleScreenBufferInfo(::GetStdHandle(STD_OUTPUT_HANDLE), &info_ );

	// (1) sets output/read handle
	{
		const intptr_t handle_ = reinterpret_cast<intptr_t>(::GetStdHandle(STD_INPUT_HANDLE));
		const INT input_ = ::_open_osfhandle(handle_, _O_TEXT );
		FILE* pFile = ::_fdopen(input_, "r");

		::setvbuf( pFile, NULL, _IONBF, 0 );
	}
	// (2) sets input/write handle
	{
		const intptr_t handle_ = reinterpret_cast<intptr_t>(::GetStdHandle(STD_OUTPUT_HANDLE));
		const INT output_ = ::_open_osfhandle(handle_, _O_TEXT );
		FILE* pFile = ::_fdopen(output_, "w");

		::setvbuf( pFile, NULL, _IONBF, 0 );
	}
	// (3) sets error handle
	{
		const intptr_t handle_ = reinterpret_cast<intptr_t>(::GetStdHandle(STD_ERROR_HANDLE));
		const INT error_ = ::_open_osfhandle(handle_, _O_TEXT );
		FILE* pFile = ::_fdopen(error_, "w");

		::setvbuf( pFile, NULL, _IONBF, 0 );
	}
#endif
	return  hr_;
}

HRESULT  CState::Wait (LPCTSTR _lp_sz_msg) const { HRESULT hr_ = S_OK;
	_lp_sz_msg;
	const bool b_verb = true;
	if (b_verb) {
		if (nullptr != _lp_sz_msg && !::_tcslen(_lp_sz_msg))
			::_tprintf(_T("\n\n\t%s"), _lp_sz_msg);
		else
			::_tprintf(_T("\n\n\tPress any key to continue;"));
		const int n_char = ::_gettch(); // https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/getch-getwch ;
	}
	return  hr_;
}

/////////////////////////////////////////////////////////////////////////////

CState&  CState::operator = (const CState& _ref) { _ref; return *this; }

/////////////////////////////////////////////////////////////////////////////

CConsole:: CConsole (void)
{
	::SetConsoleOutputCP(CP_UTF8);
}

CConsole:: CConsole (LPCTSTR _lp_sz_caption, const WORD nIconResId) : CConsole() { this->State().Create(_lp_sz_caption); this->Icon(nIconResId); }
CConsole::~CConsole (void) {}

/////////////////////////////////////////////////////////////////////////////

HRESULT    CConsole::Custom(void) const
{
	HRESULT hr_ = S_OK;

	// https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-getwindowlongptrw ;
	LONG_PTR lp_style = ::GetWindowLongPtr(::GetConsoleWindow(), GWL_STYLE); lp_style;
	lp_style &= ~(WS_CAPTION|WS_BORDER);
	lp_style |=   WS_CHILD;
	// https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-setwindowlongptrw ;
	::SetWindowLongPtr(::GetConsoleWindow(), GWL_STYLE, lp_style);
	// https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-setwindowpos ;
	::SetWindowPos(::GetConsoleWindow(), 0, 0, 0, 0, 0, SWP_FRAMECHANGED|SWP_NOACTIVATE|SWP_NOMOVE|SWP_NOSIZE|SWP_NOZORDER);

	return  hr_;
}

HRESULT    CConsole::Icon  (const WORD _n_res_id) {

	HRESULT hr_ = S_OK;

	::SendMessage(::GetConsoleWindow(), WM_SETICON, 0, (LPARAM)CIcon().Load(_n_res_id, 0 ));
	::SendMessage(::GetConsoleWindow(), WM_SETICON, 1, (LPARAM)CIcon().Load(_n_res_id, 1 ));

	return hr_;
}

const
CState&    CConsole::State (void) const { return this->m_state; } 

/////////////////////////////////////////////////////////////////////////////

CConsole&  CConsole::operator = (const CConsole& _ref) { this->m_state = _ref.State(); return *this; }

/////////////////////////////////////////////////////////////////////////////

CConsole&  CConsole::operator <<(const CRec_Err & _err ) { _err.Out (); return *this; }
CConsole&  CConsole::operator <<(const CRec_Info& _info) { _info.Out(); return *this; }
CConsole&  CConsole::operator <<(const CRec_Warn& _warn) { _warn.Out(); return *this; }

/////////////////////////////////////////////////////////////////////////////

CConsole&  CConsole::operator <<(const CRecord & _rec) { _rec.Out(); return *this; }
CConsole&  CConsole::operator <<(const CSection& _sec) { _sec.Out(); return *this; }

/////////////////////////////////////////////////////////////////////////////

CConsole&  CConsole::operator <<(LPCTSTR _lp_sz_text ) { ::_tprintf(_lp_sz_text); return *this; }