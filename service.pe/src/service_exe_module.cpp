/*
	Created by Tech_dog (ebontrop@gmail.com) on 13-Feb-2023 at 19:02:47.2252429, UTC+7, Novosibirsk, Monday;
	This is Process Prying Eyes service entry point implementation file;
*/
#include "shared.cmd.ln.h"

#pragma comment(lib, "prying.eyes.shared.lib")           // generic functions ;
#pragma comment(lib, "prying.eyes.service.man.lib")      // generic system service wrapper ;
#pragma comment(lib, "prying.eyes.log.lib")              // generic log wrapper ;

#pragma comment(lib, "PE_service.shared.lib")            // prying eyes specifics;
#pragma comment(lib, "PE_out.shared.lib")                // for fake service that acts like a console app;

#pragma comment(lib, "prying.eyes.procs.lib")
#pragma comment(lib, "prying.eyes.security.lib")

#include "PE_service.res.h"
#include "PE_service_out.h"
#include "PE_service_fake.h"

#include "shared.sec.provider.h"

#include <iostream>
#include "_out.record.h"

INT _tmain(void)
{
	using shared::input::CCommandLine;
	using _output::CConsole ;
	using _output::CRec_Info;

	using service::pe::CService_Fake;

	INT n_result = 0;
	const bool b_fake = TCmdLine().Has(_T("fake"));

	if (0 == TCmdLine().Count()) // it very looks like this module is being loaded by system service manager ;
	{
	}
	else {

		TSecProvider sec_prov;

		if (false == sec_prov.IsAdminRole())
		{
			if (SUCCEEDED(sec_prov.Elevate()))
				return n_result; // needs to restarting fake service;
		}

		CService_Fake svc_fake;

		_out.State().Create(_T("Process Prying Eyes Fake Service")); _out.Icon(IDR_SVC_ICO);
		svc_fake.Start();
		// https://stackoverflow.com/questions/14214753/how-to-read-until-esc-button-is-pressed-from-cin-in-c ;
		_out.State().Close();
		svc_fake.Stop();
	}

	return n_result;
}