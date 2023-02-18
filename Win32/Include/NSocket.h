/*
	NSocket
	
	WinSock abstractions
	
	2019/04/05	Originated
	
	Copyright © 2019 by: Ben Hekster
*/

#pragma once

#include <assert.h>

#include <WINDEF.H>
#include <WINBASE.H>
#include <WINSOCK2.H>
#include <WS2TCPIP.H>



namespace Win32::Socket {
	struct Library {
				Library();
				~Library() noexcept(false);
		};

	
	struct AddressInfo {
	protected:
		struct addrinfoW *fInfo;

	public:
		struct iterator {
			addrinfoW	*fAt;

					iterator(addrinfoW *at) : fAt(at) {}
					iterator(const iterator&) = default;

			iterator	&operator=(const iterator&) = default;
			addrinfoW	*operator->() { return fAt; }
			addrinfoW	&operator*() { return *fAt; }
			bool		operator==(const iterator &that) { return fAt == that.fAt; }
			bool		operator!=(const iterator &that) { return fAt != that.fAt; }
			iterator	&operator++() { fAt = fAt->ai_next; return *this; }
			};


				AddressInfo(const wchar_t *nodeName, const wchar_t *serviceName, const addrinfoW &hints);
				~AddressInfo();

		iterator	begin() { return iterator(fInfo); }
		iterator	end() { return iterator(nullptr); }
		};


	struct Socket {
	protected:
		SOCKET		fSocket;

	public:
				Socket(SOCKET);
				~Socket() noexcept(false);

		operator	SOCKET() const { return fSocket; }
		};


	inline Library::Library() {
		// initialize WinSock
		WSADATA wsaData;
		if (int error = WSAStartup(MAKEWORD(2,2), &wsaData); error != 0) throw error;
		}


	inline Library::~Library() noexcept(false) {
		if (WSACleanup() != 0) throw WSAGetLastError();
		}


	inline AddressInfo::AddressInfo(
		const wchar_t	*nodeName,
		const wchar_t	*serviceName,
		const addrinfoW	&hints
		) {
		if (const int error = GetAddrInfoW(nodeName, serviceName, &hints, &fInfo)) throw WSAGetLastError() /* instead of using 'gai' error code */;
		}


	inline AddressInfo::~AddressInfo() {
		FreeAddrInfoW(fInfo);
		}


	inline Socket::Socket(
		SOCKET		socket
		) :
		fSocket(socket)
		{
		if (fSocket == INVALID_SOCKET) throw WSAGetLastError();
		}


	inline Socket::~Socket() noexcept(false) {
		if (closesocket(fSocket)) throw WSAGetLastError();
		}
	}
