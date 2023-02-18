/*
	machine
	
	Machine, system, and compiler dependencies
	Win32
	
	1999/03/26	Originated
	2013/08/12	Update for C99 <stdint.h>
	
	Copyright © 1999-2013 by: Ben Hekster
*/

#pragma once

#include <stdint.h>


/*

	compiler dependencies

*/

#define snprintf _snprintf



/*

	byte ordering
	
*/

#if __cplusplus
extern "C" {
#endif

static __inline int8_t intlint8(const int8_t i) { return i; }
static __inline uint8_t intlint8u(const uint8_t i) { return i; }
static __inline int16_t intlint16(const int16_t i) { return i; }
static __inline uint16_t intlint16u(const uint16_t i) { return i; }
static __inline int32_t intlint32(const int32_t i) { return i; }
static __inline uint32_t intlint32u(const uint32_t i) { return i; }

static __inline int8_t lintint8(const int8_t i) { return i; }
static __inline uint8_t lintint8u(const uint8_t i) { return i; }
static __inline int16_t lintint16(const int16_t i) { return i; }
static __inline uint16_t lintint16u(const uint16_t i) { return i; }
static __inline int32_t lintint32(const int32_t i) { return i; }
static __inline uint32_t lintint32u(const uint32_t i) { return i; }

__inline static int16_t __fastcall intbint16(const int16_t i) {
	__asm {
		MOV	AX,i
		XCHG	AL,AH
		}
	}

__inline static uint16_t __fastcall intbint16u(const uint16_t i) {
	__asm {
		MOV	AX,i
		XCHG	AL,AH
		}
	}

__inline static int32_t __fastcall intbint32(const int32_t i) {
	__asm {
		MOV	EAX,i
		BSWAP	EAX
		}
	}

__inline static uint32_t __fastcall intbint32u(const uint32_t i) {
	__asm {
		MOV	EAX,i
		BSWAP	EAX
		}
	}

__inline static int16_t __fastcall bintint16(const int16_t i) {
	__asm {
		MOV	AX,i
		XCHG	AL,AH
		}
	}

__inline static uint16_t __fastcall bintint16u(const uint16_t i) {
	__asm {
		MOV	AX,i
		XCHG	AL,AH
		}
	}

__inline static int32_t __fastcall bintint32(const int32_t i) {
	__asm {
		MOV	EAX,i
		BSWAP	EAX
		}
	}

__inline static uint32_t __fastcall bintint32u(const uint32_t i) {
	__asm {
		MOV	EAX,i
		BSWAP	EAX
		}
	}

#if __cplusplus
	}
#endif



/*

	system dependencies

*/

/*	_X86_
	_AMD64_
	Platform macros usually defined by <WINDOWS.H> and used by <WINNT.H>.  These use the
	predefined macros documented in:
	
		http://msdn.microsoft.com/en-us/library/b0084kay%28v=vs.80%29.aspx
*/
#if defined _M_IX86
#ifndef _X86_
#define _X86_
#endif

#elif defined _M_AMD64
#ifndef _AMD64_
#define _AMD64_
#endif

#endif


/*	IFCFM
	Return the second parameter only under Code Fragment Manager development,
	else the first parameter
*/
// obviously, Win32 does not use Code Fragment Manager
#define IFCFM(notuses,uses) (notuses)



/*

	C++

*/

#ifdef __cplusplus

/*	containerof
	Given a type T containing a field f and a pointer p to this field, return
	a pointer to the container
*/
#define containerof(T,f,p) ((T*) ((char*) (p) - offsetof(T,f)))

#endif
