/*
	NHTTP
	
	WinHTTP abstractions
	
	2018/09/01	Originated
	
	Copyright © 2018 by: Ben Hekster
*/

#pragma once

#include <assert.h>
#include <stdint.h>
#include <stdlib.h>

#include <optional>

#include <WINDEF.H>
#include <WINBASE.H>
#include <WINHTTP.H>


/*

	WinHTTP functions

*/


namespace Win32::HTTP {
	/*	Handle
		Generic handle type
	*/
	struct Handle {
	protected:
		HINTERNET	const fHandle;
	
				Handle(HINTERNET handle);
	
	public:
				Handle(const Handle&) = delete;
				~Handle() noexcept(false);
		
		operator	HINTERNET() const { return fHandle; }
		};
	
	
	/*	Session
		Initialization and availability of the Windows HTTP library
	*/
	struct Session : public Handle {
	public:
				Session(
					const wchar_t userAgent[],
					uint32_t accessType,
					const wchar_t proxy[], const wchar_t proxyBypass[],
					uint32_t flags
					);
		};
	
	
	/*	Connection
		Windows HTTP connection
	*/
	struct Connection : public Handle {
	public:
				Connection(Session&, const wchar_t hostName[], uint16_t port, uint32_t = 0);
		};


	/*	Request
		Windows HTTP request on a connection
	*/
	struct Request : public Handle {
	public:
				Request(
					Connection&,
					const wchar_t verb[],
					const wchar_t objectName[],
					const wchar_t *version,
					const wchar_t *referrer,
					const wchar_t *const *acceptTypes,
					uint32_t flags
					);

		void		AddHeaders(const wchar_t headers[], uint32_t modifiers),
				AddHeaders(const wchar_t headers[], uint32_t headersL, uint32_t modifiers);
		struct {
			unsigned long supportedSchemes, firstScheme;
			unsigned long authTarget;
			}	QueryAuthSchemes();
		unsigned long	QueryDataAvailable() const;
		uint32_t	QueryHeader(uint32_t infoLevel, void *buffer, unsigned long bufferL),
				QueryHeaderByName(uint32_t infoLevel, void *buffer, unsigned long bufferL, const wchar_t name[]);
		SYSTEMTIME	QueryHeaderAsSystemTime(const uint32_t infoLevel);
		unsigned	QueryHeaderAsUnsigned(const uint32_t infoLevel);
		std::optional<unsigned> QueryHeaderAsUnsignedOptional(const uint32_t infoLevel);
		uint32_t	QueryHeaderL(uint32_t infoLevel),
				QueryHeaderLByName(uint32_t infoLevel, const wchar_t name[]);
		uint32_t	Read(void *buffer, uint32_t bufferL);
		void		Receive();
		void		Send(const wchar_t headers[], uint32_t headersL, const void *data, uint32_t dataL, uint32_t length, uintptr_t context),
				Send(const wchar_t headers[], const void *data, uint32_t dataL, uint32_t length, uintptr_t context);
		void		SetCredentials(unsigned long authTargets, unsigned long authScheme, const wchar_t userName[], const wchar_t password[]);
		uint32_t	Write(void *data, uint32_t dataL);
		};
	
	
	/*
	
		inline function definitions
	
	*/
	
	/*	Handle
		Manage Windows HTTP library handle
	*/
	inline Handle::Handle(
		HINTERNET	const handle
		) :
		fHandle(handle)
		{
		if (!fHandle) throw GetLastError();
		}
	
	
	/*	~Handle
		Close access to the Windows HTTP library
	*/
	inline Handle::~Handle() noexcept(false) {
		// close the session handle (always synchronous)
		if (!WinHttpCloseHandle(fHandle)) throw GetLastError();
		}
	
	
	/*	Session
		Access the Windows HTTP library
	*/
	inline Session::Session(
		const wchar_t	userAgent[],
		const uint32_t	accessType,
		const wchar_t	proxy[],
		const wchar_t	proxyBypass[],
		const uint32_t	flags
		) :
		Handle(WinHttpOpen(userAgent, accessType, proxy, proxyBypass, flags))
		{}
	
	
	/*	Connection
		Connection to HTTP resource
	*/
	inline Connection::Connection(
		Session		&session,
		const wchar_t	hostName[],
		const uint16_t	port,
		const uint32_t	reserved
		) :
		Handle(WinHttpConnect(session, hostName, port, reserved))
		{}


	/*	Request
		Request on a connection
	*/
	inline Request::Request(
		Connection	&connection,
		const wchar_t	verb[],
		const wchar_t	objectName[],
		const wchar_t	*version,
		const wchar_t	*referrer,
		const wchar_t *const *acceptTypes,
		uint32_t flags
		) :
		Handle(WinHttpOpenRequest(connection, verb, objectName, version, referrer, const_cast<const wchar_t**>(acceptTypes), flags))
		{}

	
	/*	AddHeaders
		Add HTTP headers to request
		The version without explicit header string length assumes the string is NUL-terminated
	*/
	inline void Request::AddHeaders(
		const wchar_t	headers[],
		const uint32_t	modifiers
		) {
		if (!WinHttpAddRequestHeaders(*this, headers, -1L /* NUL-terminated */, modifiers)) throw GetLastError();
		}

	inline void Request::AddHeaders(
		const wchar_t	headers[],
		const uint32_t	headersL,
		const uint32_t	modifiers
		) {
		if (!WinHttpAddRequestHeaders(*this, headers, headersL, modifiers)) throw GetLastError();
		}

	
	/*	QueryAuthSchemes

	*/
	inline decltype(static_cast<Request*>(nullptr)->QueryAuthSchemes()) Request::QueryAuthSchemes() {
		unsigned long supportedSchemes, firstScheme;
		unsigned long authTarget;
		if (!WinHttpQueryAuthSchemes(*this, &supportedSchemes, &firstScheme, &authTarget)) throw GetLastError();

		return { supportedSchemes, firstScheme, authTarget };
		}


	/*	QueryDataAvailable

	*/
	inline unsigned long Request::QueryDataAvailable() const {
		unsigned long result;
		if (!WinHttpQueryDataAvailable(*this, &result)) throw GetLastError();

		return result;
		}


	/*	QueryHeader
		Get headers into the specified buffer; return the length of data returned
	*/
	inline uint32_t Request::QueryHeader(
		const uint32_t	infoLevel,
		void		*const buffer,
		unsigned long	bufferL
		) {
		if (!WinHttpQueryHeaders(*this, infoLevel, nullptr /* name */, buffer, &bufferL, nullptr)) throw GetLastError();

		return bufferL;
		}

	inline uint32_t Request::QueryHeaderByName(
		const uint32_t	infoLevel,
		void		*const buffer,
		unsigned long	bufferL,
		const wchar_t	name[]
		) {
		if (!WinHttpQueryHeaders(*this, infoLevel | WINHTTP_QUERY_CUSTOM, name, buffer, &bufferL, nullptr)) throw GetLastError();

		return bufferL;
		}


	/*	QueryHeadersTry
		Like QueryHeaders, but conditionally treating certain conditions not to be errors:
		maybe		If the header isn't present, return zero
		fit		If the header doesn't fit in the supplied buffer, return the number of bytes needing to be allocated
	*/
	#if 0
	template <bool maybe, bool fit>
	inline uint32_t Request::QueryHeadersTry<maybe, fit>(
		const uint32_t	infoLevel,
		void		*const buffer,
		unsigned long	bufferL
		) {
		// failed for some reason?
		if (!WinHttpQueryHeaders(*this, infoLevel, nullptr /* name */, buffer, &bufferL, nullptr)) {
			const DWORD error = GetLastError();

			// header isn't present?
			if (maybe && error == ERROR_WINHTTP_HEADER_NOT_FOUND)
				// return zero to indicate
				bufferL = 0;

			// not enough space?
			else if (fit && error == ERROR_INSUFFICIENT_BUFFER)
				// bufferL already contains the number of bytes needing to be allocated
				;

			else
				throw error;
			}

		return bufferL;
		}
	#endif


	/*	QueryHeaderAsSystemTime
		Return the specified header as a SYSTEMTIME
	*/
	inline SYSTEMTIME Request::QueryHeaderAsSystemTime(
		const uint32_t	infoLevel
		) {
		SYSTEMTIME result;
		DWORD resultL = sizeof result;
		if (!WinHttpQueryHeaders(*this, infoLevel | WINHTTP_QUERY_FLAG_SYSTEMTIME, nullptr /* name */, &result, &resultL, nullptr)) throw GetLastError();

		return result;
		}


	/*	QueryHeaderAsUnsigned
		Return the specified header as a DWORD
	*/
	inline unsigned Request::QueryHeaderAsUnsigned(
		const uint32_t	infoLevel
		) {
		DWORD result;
		DWORD resultL = sizeof result;
		if (!WinHttpQueryHeaders(*this, infoLevel | WINHTTP_QUERY_FLAG_NUMBER, nullptr /* name */, &result, &resultL, nullptr)) throw GetLastError();

		return result;
		}

	inline std::optional<unsigned> Request::QueryHeaderAsUnsignedOptional(
		const uint32_t	infoLevel
		) {
		std::optional<unsigned> result;

		DWORD value;
		DWORD valueL = sizeof value;
		if (WinHttpQueryHeaders(*this, infoLevel | WINHTTP_QUERY_FLAG_NUMBER, nullptr /* name */, &value, &valueL, nullptr))
			result = value;

		else
			switch (const DWORD error = GetLastError()) {
				case ERROR_WINHTTP_HEADER_NOT_FOUND:
					break;

				default:
					throw error;
				}

		return result;
		}


	/*	QueryHeaderL
		Get length of header
	*/
	inline uint32_t Request::QueryHeaderL(
		const uint32_t	infoLevel
		) {
		DWORD headersL;

		// should fail
		const bool succeeded = WinHttpQueryHeaders(*this, infoLevel, nullptr, nullptr /* no output buffer */, &headersL, nullptr);
		assert(!succeeded && GetLastError() == ERROR_INSUFFICIENT_BUFFER);

		// return the number of bytes needed to return the headers
		return headersL;
		}


	/*	QueryHeaderLByName
		Get length of header specified by name
	*/
	inline uint32_t Request::QueryHeaderLByName(
		const uint32_t	infoLevel,
		const wchar_t	name[]
		) {
		DWORD headersL;

		// should fail
		const bool succeeded = WinHttpQueryHeaders(*this, infoLevel | WINHTTP_QUERY_CUSTOM, name, nullptr /* no output buffer */, &headersL, nullptr);
		assert(!succeeded && GetLastError() == ERROR_INSUFFICIENT_BUFFER);

		// return the number of bytes needed to return the headers
		return headersL;
		}


	/*	Read
		Read response body data
	*/
	inline uint32_t Request::Read(
		void		*buffer,
		uint32_t	bufferL
		) {
		DWORD length;
		if (!WinHttpReadData(*this, buffer, bufferL, &length)) throw GetLastError();

		// return number of bytes actually read
		return length;
		}


	/*	Receive
		Receive a response to the request
	*/
	inline void Request::Receive() {
		if (!WinHttpReceiveResponse(*this, nullptr)) throw GetLastError();
		}


	/*	Send
		Send the request
	*/
	inline void Request::Send(
		const wchar_t	headers[],
		const void	*const data,
		const uint32_t	dataL,
		const uint32_t	length,
		const uintptr_t	context
		) {
		if (!WinHttpSendRequest(*this, headers, -1L /* NUL-terminated */, const_cast<void*>(data), dataL, length, context)) throw GetLastError();
		}

	inline void Request::Send(
		const wchar_t	headers[],
		const uint32_t	headersL,
		const void	*const data,
		const uint32_t	dataL,
		const uint32_t	length,
		const uintptr_t	context
		) {
		if (!WinHttpSendRequest(*this, headers, headersL, const_cast<void*>(data), dataL, length, context)) throw GetLastError();
		}

	
	/*	SetCredentials
		Pass authorization credentials to server
	*/
	inline void Request::SetCredentials(
		unsigned long	authTargets,
		unsigned long	authScheme,
		const wchar_t	userName[],
		const wchar_t	password[]
		) {
		if (!WinHttpSetCredentials(*this, authTargets, authScheme, userName, password, nullptr)) throw GetLastError();
		}


	/*	Write
		Write body data
	*/
	inline uint32_t Request::Write(
		void		*const data,
		const uint32_t	dataL
		) {
		DWORD length = dataL;
		if (!WinHttpWriteData(*this, data, dataL, &length)) throw GetLastError();

		// return number of bytes written
		return length;
		}
	}
