/*
	NMemory
	
	Heap allocation abstractions
	
	2018/10/20	Originated
	
	Copyright © 2018 by: Ben Hekster
*/

#pragma once

#include <assert.h>
#include <stdint.h>
#include <stdlib.h>

#include <utility>

#include <WINDEF.H>
#include <WINBASE.H>


/*

	Heap functions

*/

namespace Win32::Memory {
	/*	Handle
		Process heap allocation (preferred method)
		Allocations are non-moveable

		Could generalize this to support other heaps; just would like to avoid the overhead
		of having to pass the process heap in those cases
	*/
	struct Handle {
	protected:
		static const HANDLE gProcessHeap;

		void		*fMemory;
	
	public:
				Handle(size_t length, unsigned flags);
				Handle(const Handle&) = delete;
				Handle(Handle&&) noexcept;
				~Handle() noexcept(false);
		
		operator	void*() const { return fMemory; }

		void		Reallocate(size_t length, unsigned flags);
		size_t		Size() const;
		HANDLE		Take();
		};
	
	
	/*	Handle

	NOTE
		A zero-length GMEM_MOVEABLE handle is allocated as 'discarded'
	*/
	inline Handle::Handle(
		const size_t	length,
		const unsigned	flags
		) :
		fMemory(HeapAlloc(gProcessHeap, flags, length))
		{
		// *** should use SEH?
		if (!fMemory) throw 0 /* cannot call GetLastError */;
		}

	inline Handle::Handle(
		Handle		&&that
		) noexcept :
		fMemory(that.fMemory)
		{
		that.fMemory = nullptr;
		}


	/*	~Handle
		Deallocate handle
	*/
	inline Handle::~Handle() noexcept(false) {
		if (fMemory)
			if (HeapFree(gProcessHeap, 0 /* flags */, fMemory) == 0) throw GetLastError() /* can call */;
		}


	/*	Reallocate
		Change the size or flags of the allocation
	*/
	inline void Handle::Reallocate(
		const size_t	length,
		const unsigned	flags
		) {
		// reallocate
		void *const memory = HeapReAlloc(gProcessHeap, flags, fMemory, length);
			if (!memory) throw 0 /* cannot call GetLastError */;

		fMemory = memory;
		}


	/*	Size
		Returns the *allocated* size of the handle
	*/
	inline size_t Handle::Size() const {
		const SIZE_T size = HeapSize(gProcessHeap, 0 /* flags */, fMemory);
			if (size == static_cast<SIZE_T>(-1)) throw 0 /* cannot call GetLastError */;

		return size;
		}


	/*	Take
		Assume ownership of the handle
	*/
	inline HGLOBAL Handle::Take() {
		return std::exchange(fMemory, nullptr);
		}



	/*	Global
		Global heap allocation (should generally not use!)

	NOTE
		"CreateStreamOnHGlobal function: HGLOBALs allocated with GMEM_FIXED are not really handles"
	*/
	struct Global {
	public:
		/*	Lock
			Lock handle and access
		*/
		struct Lok {
		protected:
			void		*const fPointer;

		public:
					Lok(HGLOBAL handle);
					~Lok();

			operator	void*() { return fPointer; }
			};

	protected:
		HGLOBAL		fHandle;
	
	public:
				Global(size_t length, unsigned flags);
				Global(const Global&) = delete;
				Global(Global&&) noexcept;
				~Global() noexcept(false);
		
		operator	HGLOBAL() const { return fHandle; }

		void		*Lock() const;
		bool		Unlock() const;
		void		Reallocate(size_t length, unsigned flags);
		size_t		Size() const;
		HGLOBAL		Take();
		};
	
	
	/*	Global

	NOTE
		A zero-length GMEM_MOVEABLE handle is allocated as 'discarded'
	*/
	inline Global::Global(
		const size_t	length,
		const unsigned	flags
		) :
		fHandle(GlobalAlloc(flags, length))
		{
		if (!fHandle) throw GetLastError();
		}

	inline Global::Global(
		Global		&&that
		) noexcept :
		fHandle(that.fHandle)
		{
		that.fHandle = 0;
		}


	/*	~Global

	*/
	inline Global::~Global() noexcept(false) {
		if (fHandle) if (GlobalFree(fHandle)) throw GetLastError();
		}


	/*	Lock
		Lock the handle and access its data
	*/
	inline void *Global::Lock() const {
		void *const pointer = GlobalLock(fHandle);
		if (!pointer) throw GetLastError();

		return pointer;
		}


	/*	Unlock
		Unlock the handle; return whether it is still locked
	*/
	inline bool Global::Unlock() const {
		return GlobalUnlock(fHandle);
		}


	/*	Reallocate
		Change the size or flags of the allocation
	*/
	inline void Global::Reallocate(
		const size_t	length,
		const unsigned	flags
		) {
		if (!GlobalReAlloc(fHandle, length, flags)) throw GetLastError();
		}


	/*	Size
		Returns the *allocated* size of the handle
	*/
	inline size_t Global::Size() const {
		const DWORD size = GlobalSize(fHandle);
		if (!size) throw GetLastError(); // *** what if the handle is just zero-length?

		return size;
		}


	/*	Take
		Assume ownership of the handle
	*/
	inline HGLOBAL Global::Take() {
		return std::exchange(fHandle, nullptr);
		}


	/*	Global::Lok
		Lock the handle
	*/
	inline Global::Lok::Lok(
		HGLOBAL		handle
		) :
		fPointer(GlobalLock(handle))
		{
		if (!fPointer) throw GetLastError();
		}


	/*	Global::~Lock
		Unlock the handle
	*/
	inline Global::Lok::~Lok() {
		(void) GlobalUnlock(GlobalHandle(fPointer));
		}
	}
