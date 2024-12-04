#ifndef _STDAFX_H_A2AD1503_5554_4E5D_9B38_341E6E697B7E_INCLUDED
#define _STDAFX_H_A2AD1503_5554_4E5D_9B38_341E6E697B7E_INCLUDED
/*
	Created by Tech_dog (ebontrop@gmail.com) on 5-Apr-2019 at 10:12:24p, UTC+7, Phuket, Rawai, Friday;
	This is sound-bin-trans receiver desktop console application precompiled header include file.
*/
#ifndef WINVER                  // Specifies that the minimum required platform is Windows Vista.
#define WINVER          0x0600  // Change this to the appropriate value to target other versions of Windows.
#endif

#ifndef _WIN32_WINNT            // Specifies that the minimum required platform is Windows Vista.
#define _WIN32_WINNT    0x0600  // Change this to the appropriate value to target other versions of Windows.
#endif

#ifndef _WIN32_WINDOWS          // Specifies that the minimum required platform is Windows Vista.
#define _WIN32_WINDOWS  0x0600  // Change this to the appropriate value to target Windows Me or later.
#endif

#ifndef _WIN32_IE               // Specifies that the minimum required platform is Internet Explorer 9.0 (Windows Vista, service pack 2).
#define _WIN32_IE       0x0900  // Change this to the appropriate value to target other versions of IE.
#endif

#ifndef STRICT
#define STRICT
#endif

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#pragma warning(disable: 4481)  // nonstandard extension used: override specifier 'override'
#pragma warning(disable: 4996)  // security warning: function or variable may be unsafe

#define _ATL_APARTMENT_THREADED
#define _ATL_NO_AUTOMATIC_NAMESPACE

#include <atlbase.h>
#include <atlcom.h>
#include <comdef.h>
#include <atlstr.h>
#include <atlsafe.h>
#include <atlwin.h>

using namespace ATL;

#ifdef _DEBUG
	#define _ATL_DEBUG_INTERFACES
	#define _CRTDBG_MAP_ALLOC
	#include <stdlib.h>
	#include <crtdbg.h>
#endif

#include <vector>
#include <map>
#include <time.h>
#include <typeinfo>

#if (0)
#if defined WIN64
  #pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined WIN32
  #pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
  #pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
  #pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif

#pragma comment(lib, "prying.eyes.shared.lib")
#pragma comment(lib, "PE_out.shared.lib")

#ifndef __DwordToHresult
#define __DwordToHresult(err_code)  HRESULT_FROM_WIN32(err_code)
#endif

#ifndef __LastErrToHresult
#define __LastErrToHresult()  __DwordToHresult(::GetLastError())
#endif

#endif/*_STDAFX_H_A2AD1503_5554_4E5D_9B38_341E6E697B7E_INCLUDED*/