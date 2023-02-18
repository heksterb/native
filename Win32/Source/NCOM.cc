/*
	NCOM
	
	Win32 Component Object Model
	
	2002/02/26	Originated
	
	Copyright © 2002 by: Ben Hekster
*/

#include "NCOM.h"

using namespace Win32;



/*

	COM

*/

/*	COM
	Initialize Component Object Model support
*/
COM::COM()
{
// initialize COM
if (HRESULT result = CoInitialize(NULL))
	// failure was not that it was initialized already?
	if (result != S_FALSE) throw result;
}
