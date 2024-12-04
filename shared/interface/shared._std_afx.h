#ifndef _SHARED__STD_AFX_H_BAD9BE65_0E1B_48AF_9F94_AF855ABB1307_INCLUDED
#define _SHARED__STD_AFX_H_BAD9BE65_0E1B_48AF_9F94_AF855ABB1307_INCLUDED
/*
	Created by Tech_dog (ebontrop@gmail.com) on 9-Feb-2023 at 15:02:44.2228022, UTC+7, Novosibirsk, Thursday;
	This is Process Prying Eyes project common include precompiled header declaraion file;
	-----------------------------------------------------------------------------
	This file is not included explicitly to cpp files because the following project option is used:
	C/C++ >> Advanced >> Forced Include Files = shared._std_afx.h;%(ForcedIncludeFiles) ;
*/

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>
#include <strsafe.h>  // for string format support;

#include <stdio.h>    // vsnprintf_s ;
#include <tchar.h>    // for creating typedef by using std::string or std::wstring;

#include <string>

#include <comdef.h>   // _com_error  ;
#include <oaidl.h>    // IErrorInfo  ;

#include <map>
#include <vector>

#endif/*_SHARED__STD_AFX_H_BAD9BE65_0E1B_48AF_9F94_AF855ABB1307_INCLUDED*/