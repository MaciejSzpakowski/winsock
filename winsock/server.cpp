#include "private.h"

namespace winsock
{
	void AcceptThread(IntServer* server)
	{
		int size = sizeof(sockaddr_in);
		sockaddr_in address;
		string msg;
		SOCKET socket;

		while (true)
		{
			socket = accept(server->handle, (sockaddr*)(&address), &size);

			if (socket == INVALID_SOCKET)
			{
				server->Stop();
				msg = FormatErrorString("accept() in AcceptThread()");
				server->SetNextError(msg);
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
			string msg = FormatErrorString("socket() in IntServer::IntServer()");
			throw socket_error(msg);
		}

		id = (long long)handle;

		//address struct
		address.sin_port = htons(port);
		address.sin_addr.s_addr = htonl(INADDR_ANY);
		address.sin_family = AF_INET;
		sockaddr* paddress = (sockaddr*)&address;
		if (bind((SOCKET)handle, paddress, (int)sizeof(sockaddr)) == SOCKET_ERROR)
		{
			//auto err = GetLastError();
			string msg = FormatErrorString("bind() in IntServer::IntServer()");
			throw socket_error(msg);
		}
	}

	void IntServer::Start(int maxQueue)
	{
		if (listen(handle, maxQueue) == SOCKET_ERROR)
		{
			string msg = FormatErrorString("listen() in IntServer::Start()");
			throw socket_error(msg);
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
			string msg = FormatErrorString("closesocket() in IntServer::Stop()");
			throw socket_error(msg);
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
	}
}