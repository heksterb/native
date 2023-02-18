/*
	NThread
	
	Thread support
	
	2012/10/15	Originated
	
	Copyright © 2001-2012 by: Ben Hekster
*/

#include "NThread.h"

using namespace Win32;


/*

	Win32::ThreadLocalStorage

*/

/*	ThreadLocalStorage
	Allocate a Thread Local Storage index
*/
ThreadLocalStorage::ThreadLocalStorage() :
	fIndex(TlsAlloc())
{
if (fIndex + 1 == 0) throw GetLastError();
}
