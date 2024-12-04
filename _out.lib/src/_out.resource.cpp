/*
	Created by Tech_dog (ebontrop@gmail.com) on 21-Feb-2023 at 11:24:10.0718572, UTC+7, Novosibirsk, Tuesday;
	This is Process Prying Eyes project console resource interface implementation file;
*/
#include "_out.resource.h"

using namespace _output;

/////////////////////////////////////////////////////////////////////////////

namespace _output { namespace _impl {

	class CIcon_Att {
	public:
		 CIcon_Att (void) {}
		~CIcon_Att (void) {}
	public:
		SIZE  Size(const bool bTreatAsLargeIcon)
		{
			const SIZE szIcon = {
				::GetSystemMetrics(bTreatAsLargeIcon ? SM_CXICON : SM_CXSMICON), 
				::GetSystemMetrics(bTreatAsLargeIcon ? SM_CYICON : SM_CYSMICON)
			};
			return szIcon;
		}
	};

}}
using namespace _output::_impl;
/////////////////////////////////////////////////////////////////////////////

CIcon:: CIcon (void) { m_error << __MODULE__ << S_OK; }
CIcon:: CIcon (const CIcon& _ref) : CIcon() { *this = _ref; }
CIcon::~CIcon (void) { /*there's no resource for being destroyed;*/ }

/////////////////////////////////////////////////////////////////////////////

TSysErrRef CIcon::Error (void) const { return this->m_error; }

HICON CIcon::Load (const UINT nIconResId, const bool bTreatAsLargeIcon)
{	// https://learn.microsoft.com/en-us/windows/win32/api/libloaderapi/nf-libloaderapi-getmodulehandlew ;
	// https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-loadimagew ;
	m_error << __MODULE__ << S_OK;
	const SIZE szIcon = CIcon_Att().Size(bTreatAsLargeIcon);
	const HINSTANCE hInstance = static_cast<HINSTANCE>(::GetModuleHandle(nullptr));
	const HICON hIcon = (HICON)::LoadImage(
		hInstance, MAKEINTRESOURCE(nIconResId), IMAGE_ICON, szIcon.cx, szIcon.cy, LR_DEFAULTCOLOR
	);
	if (nullptr == hIcon)
		m_error.Last();
	return hIcon;
}

/////////////////////////////////////////////////////////////////////////////

CIcon&  CIcon::operator = (const CIcon& _ref) { this->m_error = _ref.Error(); return *this; }