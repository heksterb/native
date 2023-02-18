/*
	NFile
	
	Win32 native file system objects
	
	2013/01/05	Factored from Classes
*/

#include "NFile.h"

using namespace Win32;



/*

	FileMap

*/

/*	FileMap
	Represent a file mapping
*/
FileMap::FileMap(
	HANDLE		handle
	) :
	fHandle(handle)
{
if (!fHandle) throw GetLastError();
}


/*	~FileMap
	Unmap a fork
*/
FileMap::~FileMap()
{
// close the file mapping handle
if (fHandle) CloseHandle(fHandle);
}



/*

	FileMap::View

*/

/*	View
	View a portion of the fork
*/
FileMap::View::View(
	void		*mapping
	) :
	fAddress(mapping)
{
if (!fAddress) throw GetLastError();
}


/*	~View
	Close the view
*/
FileMap::View::~View()
{
if (fAddress)
	if (!UnmapViewOfFile(fAddress)) throw GetLastError();
}


/*	Flush
	Flush modified data to disk
*/
void FileMap::View::Flush() const
{
if (!FlushViewOfFile(fAddress, 0)) throw GetLastError();
}
