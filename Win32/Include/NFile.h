/*
	NFile
	
	Win32 native file system objects
	
	2013/01/05	Factored from Classes
*/

#pragma once

#include <WINDEF.H>
#include <WINBASE.H>
#include <WINNT.H>


namespace Win32 {
	/*	Fork
		Open file
	*/
	struct Fork {
	protected:
		HANDLE		fHandle;
	
	public:
				Fork(HANDLE);
				~Fork();
		
		operator	HANDLE() const { return fHandle; }

		template <typename L> L FileSize() const;
		unsigned long	Read(void *buffer, unsigned bufferL);
		};
	
	
	/*	FileMap
		Mapped file fork
	*/
	struct FileMap {
	public:
		/*	View
			
		*/
		struct View {
		protected:
			void		*const fAddress;
		
		public:
					View(void*);
					~View();
			
			operator	void*() const { return fAddress; }
			
			void		Flush() const;
			};
	
	protected:
		HANDLE		fHandle;
	
	public:
				FileMap(HANDLE);
				~FileMap();
		
		operator	HANDLE() const { return fHandle; }
		};
	
	

	/*

		Fork

	*/

	/*	Fork
		Represent an open file
	*/
	inline Fork::Fork(
		HANDLE		handle
		) :
		fHandle(handle)
		{
		if (fHandle == INVALID_HANDLE_VALUE) throw GetLastError();
		}


	/*	~Fork
		Close the open file
	*/
	inline Fork::~Fork() {
		// close the file handle
		(void) CloseHandle(fHandle);
		}


	/*	GetFileSize
		Return byte size of open file
	*/
	template <>
	inline unsigned Fork::FileSize<unsigned>() const {
		const DWORD lowWord = GetFileSize(fHandle, nullptr);
		if (lowWord == 0xFFFFFFFF) throw GetLastError();

		return lowWord;
		}

	template <>
	inline unsigned long long Fork::FileSize<unsigned long long>() const {
		DWORD
			highWord,
			lowWord = GetFileSize(fHandle, &highWord);
		if (lowWord == 0xFFFFFFFF /* could be an error */)
			if (const DWORD error = GetLastError(); error) throw error;
		
		return static_cast<unsigned long long>(highWord) << 32 | lowWord;
		}


	/*	Read
		Read file data into buffer
	*/
	inline unsigned long Fork::Read(
		void		*const buffer,
		const unsigned	bufferL
		) {
		DWORD read;
		if (!ReadFile(fHandle, buffer, bufferL, &read, nullptr /* overlapped */)) throw GetLastError();

		return read;
		}
	}
