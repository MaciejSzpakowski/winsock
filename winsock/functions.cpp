#include "private.h"

namespace winsock
{
	bool libraryLoaded = false;
	WSAData wsadata;

	string GetLastWinsockErrorMessage(DWORD errorCode)
	{		
		char str[300];
		SecureZeroMemory(str, 300);
		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, 0,
			errorCode, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US),
			str, 300, 0);
		return string(str);
	}

	void LoadWinsock(int major, int minor)
	{
		if (libraryLoaded)
			return;

		int res = WSAStartup(MAKEWORD(major, minor), &wsadata);
		if (res != 0)
		{
			SetLastError(res);
			throw FormatError("LoadWinsock()");
		}

		libraryLoaded = true;
	}

	void UnloadWinsock()
	{
		if (!libraryLoaded)
			return;

		int res = WSACleanup();
		if (res != 0)
		{
			SetLastError(res);
			throw FormatError("UnloadWinsock()");
		}

		libraryLoaded = false;
	}

	socket_error FormatError(const char* header)
	{
		DWORD err = WSAGetLastError();
		stringstream ss;
		string msg = GetLastWinsockErrorMessage(err);

		ss << header << std::endl
			<< "Error code " << err << std::endl
			<< msg;

		return socket_error(ss.str(), err);
	}
}