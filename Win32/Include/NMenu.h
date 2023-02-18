/*
	NMenu
	
	Menus
	
	2002/03/25	Originated
	2003/04/21	Factored from cross-platform architecture
	2005/03/29	Support menu item marks
	2014/02/15	Template abstractions for character representation
	
	Copyright © 2002-2014 by: Ben Hekster
*/

#pragma once

#include <stdarg.h>					// needed by <WINBASE.H>

#include <WINDEF.H>
#include <WINBASE.H>
#include <WINUSER.H>


/*

	Win32 macro templates

*/


namespace Win32 {
	#undef AppendMenu
	template<typename C> BOOL AppendMenu(HMENU hMenu, UINT uFlags, UINT_PTR uIDNewItem, const C *lpNewItem);
	template<> inline BOOL AppendMenu(HMENU hMenu, UINT uFlags, UINT_PTR uIDNewItem, const char *lpNewItem) {
		return AppendMenuA(hMenu, uFlags, uIDNewItem, lpNewItem);
		}
	template<> inline BOOL AppendMenu(HMENU hMenu, UINT uFlags, UINT_PTR uIDNewItem, const wchar_t *lpNewItem) {
		return AppendMenuW(hMenu, uFlags, uIDNewItem, lpNewItem);
		}
	
	#undef InsertMenu
	template<typename C> BOOL InsertMenu(HMENU hMenu, UINT uPosition, UINT uFlags, UINT_PTR uIDNewItem, const C *lpNewItem);
	template<> inline BOOL InsertMenu(HMENU hMenu, UINT uPosition, UINT uFlags, UINT_PTR uIDNewItem, const char *lpNewItem) {
		return InsertMenuA(hMenu, uPosition, uFlags, uIDNewItem, lpNewItem);
		}
	template<> inline BOOL InsertMenu(HMENU hMenu, UINT uPosition, UINT uFlags, UINT_PTR uIDNewItem, const wchar_t *lpNewItem) {
		return InsertMenuW(hMenu, uPosition, uFlags, uIDNewItem, lpNewItem);
		}
	
	#undef GetMenuItemInfo
	template<typename MII> BOOL GetMenuItemInfo(HMENU hmenu, UINT item, BOOL fByPosition, MII *lpmii);
	template<> inline BOOL GetMenuItemInfo(HMENU hmenu, UINT item, BOOL fByPosition, MENUITEMINFOA *lpmii) {
		return GetMenuItemInfoA(hmenu, item, fByPosition, lpmii);
		}
	template<> inline BOOL GetMenuItemInfo(HMENU hmenu, UINT item, BOOL fByPosition, MENUITEMINFOW *lpmii) {
		return GetMenuItemInfoW(hmenu, item, fByPosition, lpmii);
		}
	
	
	/*	Menu
		Win32 menu
	*/
	class Menu {
	public:
		enum ItemMark {
			kMarkNone,
			kMarkCheck,
			kMarkOption
			};
	
	protected:
		typedef WORD ID;
	
	protected:
		HMENU		fMenu;
	
	public:
				Menu(HMENU);
				~Menu() { DestroyMenu(fMenu); }
		
		operator	HMENU() const { return fMenu; }
		
		template <typename C> void
				Append(UINT flags, UINT_PTR newItemID, const C *newItem);
		void		Check(UINT item, bool check);
		void		Delete(UINT position, UINT flags);
		void		EnableItem(UINT itemID, UINT enable);
		template <typename MENUITEMINFO> void GetItemInfo(UINT item, BOOL byPosition, MENUITEMINFO*);
		template <typename C> void
				Insert(UINT position, UINT flags, UINT_PTR newItemID, const C *newItem);
		void		Mark(UINT item, ItemMark mark) const;
		ItemMark	Mark(UINT item) const;
		
		unsigned	ItemTextLength(UINT position, BOOL byPosition);
		void		ItemText(UINT position, BOOL byPosition, wchar_t text[]);
		};
	}
