/*
	Created by Tech_dog (ebontrop@gmail.com) on 13-Dec-2022 at 13:54:13.8725445, UTC+7, Novosibirsk, Tuesday;
	This is service test client console module implementation file;
*/
#include "_out.h"
#include "_out.res.h"

using namespace _output;

INT _tmain (VOID) {

	INT n_res = 0;

	_out.State().Create(_T("Process Prying Eyes Service Client")); _out.Icon(IDR_OUT_CON_ICO);
	_out.State().Close();

	return n_res;
}