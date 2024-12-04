#ifndef _OUT_SECTION_H_B7061AC8_1CA6_4853_85FB_1FB70FC8860F_INCLUDED
#define _OUT_SECTION_H_B7061AC8_1CA6_4853_85FB_1FB70FC8860F_INCLUDED
/*
	Created by Tech_dog (ebontrop@gmail.com) on 12-Dec-2022 at 08:37:39.2526269, UTC+7, Novosibirsk, Monday;
	This is system console output section interface declaration file;
	-----------------------------------------------------------------------------
	This code is the excerpt from console project of Ebo Pack solution static library code;
*/
#include "shared.classes.h"

namespace _output {

	using shared::sys_core::string_t;
	// base class and base class error attribute is excluded from this version of section code, because error object is not necessary for the time being;
	class CSection {
	public:
		 CSection (void);
		 CSection (const CSection&);
		 CSection (LPCTSTR _lp_sz_caption);
		~CSection (void);

	public:
		bool       Is  (void) const;             // checks a validity of caption string;
		HRESULT    Out (void) const;             // prints out the text;
		HRESULT    Out (LPCTSTR _lp_sz_caption); // prints out the given text; TODO: twice checking given caption occurs;
		LPCTSTR    Text(void) const;
		HRESULT    Text(LPCTSTR)   ;

	public:
		 CSection& operator = (const CSection&); // does not print section caption, but only copies base's error object and text content;
		 CSection& operator <<(LPCTSTR _lp_sz_caption);

	public:
		static bool Is (LPCTSTR);                // checks a validity of provided text;

	private:
		string_t   m_text;
	};
}

typedef _output::CSection TSection;

#endif/*_OUT_SECTION_H_B7061AC8_1CA6_4853_85FB_1FB70FC8860F_INCLUDED*/