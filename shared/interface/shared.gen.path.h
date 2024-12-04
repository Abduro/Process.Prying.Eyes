#ifndef _SHAREDGENPATH_H_A12AF5D1_E69F_4D83_A08D_B6994603FADE_INCLUDED
#define _SHAREDGENPATH_H_A12AF5D1_E69F_4D83_A08D_B6994603FADE_INCLUDED
/*
	Created by Tech_dog (ebontrop@gmail.com) on 11-Feb-2023 at 12:47:22.1077034, UTC+7, Novosibirsk, Saturday;
	This is Process Prying Eyes project shared generic path interface declaration file;
*/
#include "shared.classes.h"
#include "shared.sys.err.h"

#include <shlwapi.h>

namespace shared { namespace sys_core {

	class CGenericPath
	{
	public:
		 CGenericPath (void);
		 CGenericPath (const CGenericPath&);
		 CGenericPath (LPCTSTR _lp_sz_path);
		~CGenericPath (void);

	public: // accessors;
		TSysErrRef Error (void) const;

		bool Exists (void) const;          // checks an existence of file or folder; if false is returned, error object must be checked;
		bool IsFile (void) const;          // checks a path of file; if false is returned, error object must be checked, maybe something goes wrong;
		bool IsFolder (void) const;        // checks a path points to folder; if false is returned, error object must be checked for details;
	public: // mutators;
		LPCTSTR Get (void) const;
		bool    Set (LPCTSTR _lp_sz_path); // sets a path string, if returns false, error object must be checked for getting details of the reason;

	public:
		CGenericPath& operator = (const CGenericPath&);
		CGenericPath& operator <<(LPCTSTR _lp_sz_path);

	private:
		mutable
		CSysError m_error;
		string_t  m_path;
	};

}}

#endif/*_SHAREDGENPATH_H_A12AF5D1_E69F_4D83_A08D_B6994603FADE_INCLUDED*/