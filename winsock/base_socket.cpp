#include "private.h"

namespace winsock
{
	BaseSocket::BaseSocket()
	{
	}

	void BaseSocket::ThrowNextError()
	{
		socket_error err;

		errorMutex.lock();
		if (!errors.empty())
		{
			err = errors.front();
			errors.pop();
		}
		errorMutex.unlock();

		if (err.errorCode != -1)
			throw err;
	}

	void BaseSocket::SetNextError(socket_error& error)
	{
		errorMutex.lock();
		errors.push(error);
		errorMutex.unlock();
	}
}