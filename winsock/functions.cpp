#include "private.h"

namespace winsock
{
	bool libraryLoaded = false;
	WSAData wsadata;

	string GetLastWinsockErrorMessage(PDWORD errorCode)
	{
		DWORD err = WSAGetLastError();
		if(errorCode != 0)
			*errorCode = err;
		char str[300];
		SecureZeroMemory(str, 300);
		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, 0,
			err, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US),
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
			string msg = FormatErrorString("LoadWinsock()");
			throw socket_error(msg);
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
			string msg = FormatErrorString("UnloadWinsock()");
			throw socket_error(msg);
		}

		libraryLoaded = false;
	}

	string FormatErrorString(const char* header)
	{
		DWORD code;
		stringstream ss;
		ss << header << std::endl
			<< "Error code " << code << std::endl
			<< GetLastWinsockErrorMessage(&code);
		return ss.str();
	}
}