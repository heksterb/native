/*
	Menu
	
	Win32 menu management extensions
	
	2005/03/29	Originated
*/

#pragma once

#include <stdarg.h>					// needed by <WINBASE.H>

#include <WINDEF.H>
#include <WINBASE.H>
#include <WINUSER.H>

#ifdef __cplusplus
extern "C" {
#endif



/*	FindMenuItem
	Find the menu and item corresponding to a menu item identifier in
	the given menu or its submenus
*/
extern HMENU FindMenuItem(
	HMENU		menu,
	WORD		id,
	unsigned short	*itemR
	);


#ifdef __cplusplus
}
#endif
