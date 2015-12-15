#include "private.h"

namespace winsock
{
	void AcceptThread(IntServer* server)
	{
		int size = sizeof(sockaddr_in);
		sockaddr_in address;
		socket_error err;
		SOCKET socket;

		while (true)
		{
			socket = accept(server->handle, (sockaddr*)(&address), &size);

			if (socket == INVALID_SOCKET)
			{
				server->Stop();
				err = FormatError("accept() in AcceptThread()");
				server->SetNextError(err);
				break;
			}

			server->clientMutex.lock();
			server->clients.push(socket);
			server->clientMutex.unlock();
		}
	}

	IntServer::IntServer(unsigned short port)
	{
		running = false;
		name = "";
		SecureZeroMemory(&address, sizeof(address));
		LoadWinsock(2, 2);

		//new socket
		handle = socket(AF_INET, SOCK_STREAM, NULL);
		if (handle == INVALID_SOCKET)
		{
			throw FormatError("socket() in IntServer::IntServer()");
		}

		id = (long long)handle;

		//address struct
		address.sin_port = htons(port);
		address.sin_addr.s_addr = htonl(INADDR_ANY);
		address.sin_family = AF_INET;
		sockaddr* paddress = (sockaddr*)&address;
		if (bind((SOCKET)handle, paddress, (int)sizeof(sockaddr)) == SOCKET_ERROR)
		{
			throw FormatError("bind() in IntServer::IntServer()");
		}

		char buf[100];
		inet_ntop(AF_INET, &(address.sin_addr), buf, 100);
		ip = string(ip);
	}

	void IntServer::Start(int maxQueue)
	{
		if (listen(handle, maxQueue) == SOCKET_ERROR)
		{
			throw FormatError("listen() in IntServer::Start()");
		}

		running = true;
		acceptThread = thread(AcceptThread, this);
		acceptThread.detach();
	}

	void IntServer::Stop()
	{
		if (!running)
			return;

		running = false;
		if (closesocket(handle) == SOCKET_ERROR)
		{
			throw FormatError("closesocket() in IntServer::Stop()");
		}
	}	

	IntClient* IntServer::GetNextClient()
	{
		SOCKET socket = 0;

		clientMutex.lock();
		if (!clients.empty())
		{
			socket = clients.front();
			clients.pop();
		}
		clientMutex.unlock();

		if (socket == 0)
			return nullptr;

		sockaddr_in address;
		int size = sizeof(address);
		getsockname(socket,(sockaddr*)(&address), &size);

		return new IntClient(socket, address, this);
	}

	void IntServer::Destroy()
	{
		Stop();
	}
}