/*
	NCOM
	
	Win32 Component Object Model
	
	2002/02/26	Originated
	
	Copyright © 2002 by: Ben Hekster
*/

#pragma once

#include <stdarg.h>					// needed by <WINBASE.H>

#include <OBJBASE.H>


namespace Win32 {
	/*	COM
		Component Object Model support
	*/
	struct COM {
				COM();
				~COM() { CoUninitialize(); }
		};
	
	
	/*	COMObject
		Component Object Model objects
	*/
	template <class T> struct COMObject {
	protected:
		T		*fObject;
	
	public:
				COMObject(T *t = NULL) : fObject(t) {}
				
				COMObject(
					IUnknown	*object,
					const IID	&interfaceID
					);
				
				COMObject(
					const CLSID	&classID,
					const IID	&interfaceID,
					DWORD		context,
					IUnknown	*controller = NULL
					);
				
				~COMObject() { if (fObject) fObject->Release(); }
		
		T		*operator->() const { return fObject; }
		operator	T*&() { return fObject; }
		operator	T*() const { return fObject; }
		
		void		*&Slot() { return reinterpret_cast<void*&>(fObject); }
		};
	
	
	/*	COMObject
		Create an instance of a COM class
	*/
	template <class T> inline COMObject<T>::COMObject(
		const CLSID	&classID,
		const IID	&interfaceID,
		DWORD		context,
		IUnknown	*controller
		) {
		// try to create the instance
		if (HRESULT result = CoCreateInstance(classID, controller, context, interfaceID, reinterpret_cast<void**>(&fObject)))
			throw result;
		}
	
	
	/*	COMObject
		Query for an interface of an existing COM object
	*/
	template <class T> inline COMObject<T>::COMObject(
		IUnknown	*object,
		const IID	&interfaceID
		) {
		if (HRESULT result = object->QueryInterface(interfaceID, reinterpret_cast<void**>(&fObject)))
			if (result != E_NOINTERFACE) throw result;
		}
	}
