/*
	NTimer
	
	Timers
	
	1999/08/01	Originated
	2003/05/13	Factored from cross-platform architecture
	
	Copyright © 2003 by: Ben Hekster
*/

#include "NTimer.h"

using namespace Win32;


/*

	Win32::Timer

*/

/*	Timer
	Create a new timer with the given time-out in milliseconds
*/
Timer::Timer(
	TIMERPROC	Callback,
	UINT		ms
	) :
	fID(
		/* UINT_PTR is an integer of pointer width?  Then use the object pointer as the timer ID.  */
		SetTimer(NULL, 0, ms, Callback)
		)
{
if (!fID) throw GetLastError();
}


/*	~Timer
	Destroy the timer
*/
Timer::~Timer()
{
// destroy the timer *** remove already-posted WM_TIMER messages from the application queue?
if (!KillTimer(NULL, fID)) throw GetLastError();
}


/*	Reschedule
	Reschedule the timer
*/
void Timer::Reschedule(
	TIMERPROC	Callback,
	UINT		ms
	)
{
// reschedule
if (!SetTimer(NULL, fID, ms, Callback)) throw GetLastError();
}
