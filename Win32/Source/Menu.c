/*
	Menu
	
	Win32 menu management extensions
	
	2005/03/29	Originated
*/

#include <MenuExt.h>


/*	FindMenuItem
	Find the menu and item corresponding to a menu item identifier in
	the given menu or its submenus
*/
HMENU FindMenuItem(
	HMENU		menu,
	WORD		id,
	unsigned short	*itemR
	)
{
HMENU result = NULL;
const int itemN = GetMenuItemCount(menu);
unsigned item;

if (itemN == -1) goto Error;

// for all the items in the menu
for (item = 0; item < (unsigned) itemN; item++) {
	// get the menu item identifier
	const UINT iid = GetMenuItemID(menu, item);
	
	// matches?
	if (iid == id) {
		// return the menu and item
		result = menu;
		*itemR = item;
		
		break;
		}
	
	// submenu?
	else if (iid == -1) {
		const HMENU submenu = GetSubMenu(menu, item);
		
		if (submenu)
			// recurse
			if (result = FindMenuItem(submenu, id, itemR))
				break;
		}
	}

Error:
return result;
}
