#ifndef _SYSSVCCREATE_H_F6FED3CC_D476_4d88_82AC_06C9BC8745C9_INCLUDED
#define _SYSSVCCREATE_H_F6FED3CC_D476_4d88_82AC_06C9BC8745C9_INCLUDED
/*
	Created by Tech_dog (ebontrop@gmail.com) on 6-Jun-2016 at 11:25:42a, UTC+7, Phuket, Rawai, Monday;
	This is Shared system service create data interface declaration file.
	( project: thefileguardian.com )
	-----------------------------------------------------------------------------
	Adopted to Ebo Pack on 11-Jul-2020 at 6:27:42p, UTC+7, Novosibirsk, Saturday;
	-----------------------------------------------------------------------------
	Adopted to Process Prying Eyes project on 11-Feb-2023 at 08:59:18.7052022, UTC+7, Novosibirsk, Saturday;
*/
#include <AccCtrl.h>
#include <Aclapi.h>
#include <winsvc.h>

#include "shared.classes.h"
#include "shared.sys.err.h"
#include "shared.gen.path.h"

#include "svc.crt.opts.h"    // declares {control|start|type} service options;
#include "svc.crt.props.h"   // declares {account|description|name|path|title} service properties;

namespace service { namespace setting {

	using shared::sys_core::CSysError;
	using shared::sys_core::CGenericPath;
	using shared::sys_core::string_t;

	class CCrtData {
	public:
		 CCrtData (void);
		 CCrtData (const CCrtData&);
		~CCrtData (void);

	public:
		TSysErrRef Error (void) const;
		const
		TCrtOpts&  Options (void) const;
		TCrtOpts&  Options (void) ;
		const
		TCrtProps& Properties(void) const;
		TCrtProps& Properties(void) ;

	public:
		bool       Is_valid(void) const;  // checks option and property collections;
		string_t   To_string(LPCTSTR _lp_sz_sep = _T("; ")) const;

	public:
		CCrtData&  operator = (const CCrtData&);
	
	private:
		mutable
		CSysError m_error;
		TCrtOpts  m_opts;
		TCrtProps m_props;
	};
}}

#endif/*_SYSSVCCREATE_H_F6FED3CC_D476_4d88_82AC_06C9BC8745C9_INCLUDED*/