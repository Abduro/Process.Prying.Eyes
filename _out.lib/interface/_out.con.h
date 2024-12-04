#ifndef _OUTCON_H_361370DE_2CBF_46EA_B35B_23A2AD5032AF_INCLUDED
#define _OUTCON_H_361370DE_2CBF_46EA_B35B_23A2AD5032AF_INCLUDED
/*
	Created by Tech_dog (ebontrop@gmail.com) on 21-Feb-2023 at 08:35:40.3280532, UTC+7, Novosibirsk, Tuesday;
	This is Process Prying Eyes project shared console interface declaration file;
*/
#include <iostream>

#include "_out.record.h"
#include "_out.section.h"
#include "_out.resource.h"

namespace _output  {
        
	class CState // this class is just for outputing appropriate messages and waiting an input from keayboard;
	{
	public:
		 CState (void);
		 CState (const CState&);
		~CState (void);

	public:
		HRESULT    Close  (void) const;
		// very tricky game: a caption on a title bar of the window;
		// https://learn.microsoft.com/en-us/office/vba/api/project.window.caption ;
		// thus, OnCreate method is made to be easier for use, without a title argument;
		HRESULT    Create (LPCTSTR _lp_sz_cap/*, LPCTSTR _lp_sz_title*/) const; // cannot understand the playing with buffers;
		// https://stackoverflow.com/questions/14214753/how-to-read-until-esc-button-is-pressed-from-cin-in-c ;
		HRESULT    Wait   (LPCTSTR _lp_sz_msg = nullptr) const; // does not work a while loop is necessary for checking particular key being pressed;

	public:
		CState&    operator = (const CState&);   // useless for the time being;
	};

	class CConsole {
	public:
		 CConsole (void);
		 CConsole (LPCTSTR _lp_sz_caption, const WORD nIconResId);
		~CConsole (void);

	public:
		HRESULT   Custom(void) const;
		HRESULT   Icon  (const WORD _n_res_id);
		const
		CState&   State (void) const;

	public:
		CConsole& operator = (const CConsole&);

	public: // specialized operators to message type;
		CConsole& operator <<(const CRec_Err &); // prints out an error details;
		CConsole& operator <<(const CRec_Info&); // prints out an information;
		CConsole& operator <<(const CRec_Warn&); // prints out an warning;
	public:
		CConsole& operator <<(const CRecord &);  // generic record output;
		CConsole& operator <<(const CSection&);  // section's print out;
	public: // generic output;
		CConsole& operator <<(LPCTSTR sz_text);  // outputs text without record header, for example, _T('\n');

	private:
		CState  m_state; // just for separating the code between writing messages and console state;
	};

}

#endif/*_OUTCON_H_361370DE_2CBF_46EA_B35B_23A2AD5032AF_INCLUDED*/