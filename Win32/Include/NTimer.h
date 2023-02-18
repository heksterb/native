/*
	NTimer
	
	Timers
	
	1999/08/01	Originated
	2003/05/13	Factored from cross-platform architecture
	
	Copyright © 2003 by: Ben Hekster
*/

#pragma once

#include <stdarg.h>

#include <WINDEF.H>
#include <WINBASE.H>
#include <WINUSER.H>


namespace Win32 {
	/*	Timer
		Win32 timer not associated with a window
	*/
	struct Timer {
		protected:
			const UINT_PTR	fID;
		
		public:
					Timer(TIMERPROC, UINT ms);
					~Timer();
			
			UINT_PTR	ID() const { return fID; }
			
			void		Reschedule(TIMERPROC, UINT ms);
		};
	}
