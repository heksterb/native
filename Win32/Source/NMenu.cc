/*
	NMenu
	
	Menus
	
	2002/03/25	Originated
	2003/04/21	Factored from cross-platform architecture
	2005/03/29	Support menu item marks
	2010/03/13	Visual Studio
	2014/02/15	Template abstractions for character representation
	
	Copyright © 2002-2014 by: Ben Hekster
*/

#include <assert.h>
#include <malloc.h>

#include "NMenu.h"

using namespace Win32;



/*

	Menu

*/

/*	Menu
	Represent an existing Win32 menu
*/
Menu::Menu(
	HMENU		menu
	) :
	fMenu(menu)
{
if (!fMenu) throw GetLastError();
}


/*	Append
	Append a new item to the end of the menu
*/
template<typename C> void Menu::Append(
	UINT		flags,
	UINT_PTR	newItemID,
	const C		*newItem
	)
{
// append the item
if (!AppendMenu(fMenu, flags, newItemID, newItem)) throw GetLastError();
}

template void Menu::Append(UINT, UINT_PTR, const char*);
template void Menu::Append(UINT, UINT_PTR, const wchar_t*);


/*	Check
	Set or clear a check mark on a menu item
*/
void Menu::Check(
	UINT		item,
	bool		check
	)
{
// set the menu item check mark
if (
	CheckMenuItem(
		fMenu,
		item,
		MF_BYPOSITION |
			(check ? MF_CHECKED : MF_UNCHECKED)
		) == -1
	) throw GetLastError();
}


/*	Mark
	Set a mark on the menu item
	
	This function is complicated because before API Level 5, the flag
	that specifies a bullet check mark is mixed with the flag that controls
	setting the name of the menu item.

	*** Should optimize for API Level 5, nontrivial because setting WINVER
	changes the size of struct MENUITEMINFO.
*/
void Menu::Mark(
	UINT		item,
	ItemMark	mark
	) const
{
MENUITEMINFO info;
info.cbSize = sizeof info;
info.fMask = MIIM_STATE;

info.fMask |= MIIM_TYPE;

// get the size of the menu item title
info.dwTypeData = NULL;
info.cch = 0;
if (!GetMenuItemInfo(fMenu, item, true, &info)) throw GetLastError();

// menu item is of type MFT_STRING?
if (info.cch)
	// allocate a buffer in which the title can be returned
	info.dwTypeData = static_cast<TCHAR*>(alloca((info.cch + 1) * sizeof(TCHAR)));

// get the current type and state of the menu item, and its title
if (!GetMenuItemInfo(fMenu, item, true, &info)) throw GetLastError();

// clear and set the radio check type bit
info.fType =
	info.fType & ~MFT_RADIOCHECK |
	(mark == kMarkOption ? MFT_RADIOCHECK : 0);

// clear and set the checked type bit
info.fState =
	info.fState & ~(MFS_CHECKED | MFS_UNCHECKED) |
	(mark != kMarkNone ? MFS_CHECKED : MFS_UNCHECKED);

// set the menu item mark
if (!SetMenuItemInfo(fMenu, item, true, &info)) throw GetLastError();
}


/*	Mark
	Get a mark on a menu item
*/
Menu::ItemMark Menu::Mark(
	UINT		item
	) const
{
MENUITEMINFO info;
info.cbSize = sizeof info;
info.fMask = MIIM_TYPE | MIIM_STATE;

// get information about the menu item
info.dwTypeData = NULL;
info.cch = 0;
if (!GetMenuItemInfo(fMenu, item, true, &info)) throw GetLastError();

return
	// has a mark?
	info.fState & MFS_CHECKED ?
		// is a radio mark?
		info.fType & MFT_RADIOCHECK ?
			kMarkOption : kMarkCheck
		:
	kMarkNone;
}


/*	Delete
	Delete an item from the menu
*/
void Menu::Delete(
	UINT		position,
	UINT		flags
	)
{
// delete the item
if (!DeleteMenu(fMenu, position, flags)) throw GetLastError();
}


/*	EnableItem
	Enable or disable a menu item
*/
void Menu::EnableItem(
	UINT		itemID,
	UINT		enable
	)
{
// set the item state
if (!EnableMenuItem(fMenu, itemID, enable)) throw GetLastError();
}


/*	GetItemInfo
	Retrieve information about a menu item
*/
template<typename MII> void Menu::GetItemInfo(
	UINT		item,
	BOOL		byPosition,
	MII		*info
	)
{
// get the menu item information
if (!GetMenuItemInfo(fMenu, item, byPosition, info)) throw GetLastError();
}

template void Menu::GetItemInfo(UINT, BOOL, MENUITEMINFOA*);
template void Menu::GetItemInfo(UINT, BOOL, MENUITEMINFOW*);


/*	Insert
	Insert an item into the menu
*/
template<typename C> void Menu::Insert(
	UINT		position,
	UINT		flags,
	UINT_PTR	newItemID,
	const C		*newItem
	)
{
// insert the item
if (!InsertMenu(fMenu, position, flags, newItemID, newItem)) throw GetLastError();
}

template void Menu::Insert(UINT, UINT, UINT_PTR, const char*);
template void Menu::Insert(UINT, UINT, UINT_PTR, const wchar_t*);


/*	ItemTextLength
	Return the length in characters of the text of the specified menu item
*/
unsigned Menu::ItemTextLength(
	UINT		position,
	BOOL		byPosition
	)
{
// get the character length of the menu item
MENUITEMINFO info;
info.cbSize = sizeof info;
info.fMask = MIIM_TYPE;
info.dwTypeData = NULL;
info.cch = 0;
if (!GetMenuItemInfo(fMenu, position, byPosition, &info)) throw GetLastError();

// make sure the returned item in fact refers to a text item
if (info.fType != MFT_STRING) throw 0;

// return the character length of the text item
return info.cch;
}


/*	ItemText
	Return the text of the specified menu item
	
	The text buffer should be as many characters long as specified by the
	result of ItemTextLength, and the result is not NUL-terminated.
*/
void Menu::ItemText(
	UINT		position,
	BOOL		byPosition,
	wchar_t		text[]
	)
{
// get the character length of the menu item
MENUITEMINFO info;
info.cbSize = sizeof info;
info.fMask = MIIM_TYPE;
info.dwTypeData = text;
if (!GetMenuItemInfo(fMenu, position, byPosition, &info)) throw GetLastError();
}
