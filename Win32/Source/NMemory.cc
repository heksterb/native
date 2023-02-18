/*
	NMemory
	
	Heap allocation abstractions
	
	2018/10/20	Originated
	
	Copyright © 2018 by: Ben Hekster
*/

#include "NMemory.h"

using namespace Win32;


/*	gProcessHeap
	Default process heap
*/
const HANDLE Win32::Memory::Handle::gProcessHeap = GetProcessHeap();
