/*
	NDNS
	
	WinDNS abstractions
	
	2019/04/05	Originated
	
	Copyright © 2019 by: Ben Hekster
*/

#pragma once

#include <WINDEF.H>
#include <WINBASE.H>
#include <WINDNS.H>



namespace Win32::MyDNS {
	struct Query {
	protected:
		DNS_RECORDW	*fResults;

	public:
		struct iterator {
			DNS_RECORDW	*fAt;

					iterator(DNS_RECORDW *at) : fAt(at) {}
					iterator(const iterator&) = default;

			iterator	&operator=(const iterator&) = default;
			DNS_RECORDW	*operator->() { return fAt; }
			DNS_RECORDW	&operator*() { return *fAt; }
			bool		operator==(const iterator &that) { return fAt == that.fAt; }
			bool		operator!=(const iterator &that) { return fAt != that.fAt; }
			iterator	&operator++() { fAt = fAt->pNext; return *this; }
			};


				Query(const wchar_t name[], unsigned short type, unsigned long options);
				~Query();

		iterator	begin() { return iterator(fResults); }
		iterator	end() { return iterator(nullptr); }
		};


	inline Query::Query(
		const wchar_t name[],
		unsigned short type,
		unsigned long options
		) {
		switch (const DNS_STATUS error = DnsQuery_W(name, type, options, nullptr, &fResults, nullptr)) {
			case 0:
				break;

			case DNS_INFO_NO_RECORDS:
				fResults = nullptr;
				break;

			default:
				throw error;
			}
		}


	inline Query::~Query() {
		DnsRecordListFree(fResults, DnsFreeRecordList);
		}
	}
