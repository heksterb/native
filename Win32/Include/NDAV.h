/*
	NDAV
	
	Win32 WebDAV
	
	2018/08/25	Originated
	
	Copyright © 2018 by: Ben Hekster
*/

#pragma once

#include <stdint.h>

#include <WINDEF.H>
#include <DAVCLNT.H>


namespace Win32 {
	/*	DAVConnection
		Secure connection to WebDAV server
	*/
	struct DAVConnection {
	protected:
		HANDLE		fHandle;
	
	public:
				DAVConnection(
					const wchar_t remoteName[],
					const wchar_t userName[],
					const wchar_t password[],
					const unsigned char clientCertificate[], uint32_t clientCertificateL
					);
				~DAVConnection() noexcept(false);
		};
	
	
	/*	DAVConnection
		Secure connection to WebDAV server
	*/
	inline DAVConnection::DAVConnection(
		const wchar_t	remoteName[],
		const wchar_t	userName[],
		const wchar_t	password[],
		const unsigned char clientCertificate[],
		uint32_t	clientCertificateL
		) {
		// try to create secure connection to server
		if (const DWORD error = DavAddConnection(
			&fHandle,
			remoteName, userName, password,
			const_cast<PBYTE>(clientCertificate), clientCertificateL)
			) throw error;
		}
	
	
	/*	~DAVConnection
		Close connection to WebDAV server
	*/
	inline DAVConnection::~DAVConnection() noexcept(false) {
		// try to close connection
		if (const DWORD error = DavDeleteConnection(fHandle)) throw error;
		}
	}
