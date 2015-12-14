#include "private.h"

namespace winsock
{
	BaseSocket::BaseSocket()
	{
	}

	string BaseSocket::GetIP()
	{
		char ip[100];
		inet_ntop(AF_INET, &(address.sin_addr), ip, 100);
		return string(ip);
	}

	void BaseSocket::GetNextError()
	{
		string err = "";

		errorMutex.lock();
		if (!errors.empty())
			err = errors.front();
		errorMutex.unlock();

		if (err != "")
			throw socket_error(err);
	}

	void BaseSocket::SetNextError(string& error)
	{
		errorMutex.lock();
		errors.push(error);
		errorMutex.unlock();
	}
}