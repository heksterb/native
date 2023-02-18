/*
	NThread
	
	Thread support
	
	2012/10/15	Originated
	
	Copyright © 2001-2012 by: Ben Hekster
*/

#pragma once

#include <stdarg.h>

#include <WINDEF.H>
#include <WINBASE.H>


namespace Win32 {
	/*	ThreadLocalStorage
		Win32 Thread Local Storage index
	*/
	struct ThreadLocalStorage {
	protected:
		const DWORD	fIndex;
	
	public:
				ThreadLocalStorage();
				~ThreadLocalStorage() { (void) TlsFree(fIndex); }
		
		operator	DWORD() const { return fIndex; }
		};
	}
