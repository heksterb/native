/*
	NWindow
	
	Windows
	
	2014/02/15	Template abstractions for character representation
	
	Copyright © 2014 by: Ben Hekster
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
	#undef RegisterClass
	template<typename WC> ATOM RegisterClass(const WC *lpWndClass);
	template<> inline ATOM RegisterClass(const WNDCLASSA *lpWndClass) {
		return RegisterClassA(lpWndClass);
		}
	template<> inline ATOM RegisterClass(const WNDCLASSW *lpWndClass) {
		return RegisterClassW(lpWndClass);
		}
	
	#undef UnregisterClass
	template<typename C> BOOL UnregisterClass(const C *lpClassName, HINSTANCE hInstance);
	template<> inline BOOL UnregisterClass(const char *lpClassName, HINSTANCE hInstance) {
		return UnregisterClassA(lpClassName, hInstance);
		}
	template<> inline BOOL UnregisterClass(const wchar_t *lpClassName, HINSTANCE hInstance) {
		return UnregisterClassW(lpClassName, hInstance);
		}
	
	#undef CreateWindowEx
	template<typename C> HWND CreateWindowEx(DWORD dwExStyle, const C *lpClassName, const C *lpWindowName, DWORD dwStyle, int X, int Y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam);
	template<> inline HWND CreateWindowEx(DWORD dwExStyle, const char *lpClassName, const char *lpWindowName, DWORD dwStyle, int X, int Y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam) {
		return CreateWindowExA(dwExStyle, lpClassName, lpWindowName, dwStyle, X, Y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam);
		}
	template<> inline HWND CreateWindowEx(DWORD dwExStyle, const wchar_t *lpClassName, const wchar_t *lpWindowName, DWORD dwStyle, int X, int Y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam) {
		return CreateWindowExW(dwExStyle, lpClassName, lpWindowName, dwStyle, X, Y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam);
		}
	}
