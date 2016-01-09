#include "private.h"
#include "winsock.h"

namespace winsock
{
	///////SERVER/////////////

	Server::Server()
	{
		server = nullptr;
	}

	Server::Server(unsigned short port)
	{
		server = std::shared_ptr<IntServer>(new IntServer(port));
	}

	void Server::Start(int maxQueue)
	{
		if (server == nullptr)
			throw socket_error("Server is null", -2);
		server->Start(maxQueue);
	}

	void Server::Start()
	{
		if (server == nullptr)
			throw socket_error("Server is null", -2);
		server->Start(100);
	}

	void Server::Stop()
	{
		if (server == nullptr)
			throw socket_error("Server is null", -2);
		server->Stop();
	}

	bool Server::GetNextClient(Client& c)
	{
		if (server == nullptr)
			throw socket_error("Server is null", -2);

		IntClient* ic = server->GetNextClient();

		if (ic == nullptr)
			return false;
		else
		{
			c = Client(ic);
			return true;
		}
	}

	void Server::ThrowNextError()
	{
		if (server == nullptr)
			throw socket_error("Server is null", -2);
		server->ThrowNextError();
	}

	string Server::GetName()
	{
		if (server == nullptr)
			throw socket_error("Server is null", -2);
		return server->name;
	}

	void Server::SetName(string name)
	{
		if (server == nullptr)
			throw socket_error("Server is null", -2);
		server->name = name;
	}

	long long Server::GetId()
	{
		if (server == nullptr)
			throw socket_error("Server is null", -2);
		return server->id;
	}

	void Server::SetId(long long id)
	{
		if (server == nullptr)
			throw socket_error("Server is null", -2);
		server->id = id;
	}

	string Server::GetIP()
	{
		if (server == nullptr)
			throw socket_error("Server is null", -2);
		return server->ip;
	}

	bool Server::IsRunning()
	{
		if (server == nullptr)
			throw socket_error("Server is null", -2);
		return server->running;
	}

	///////Client/////////////
	Client::Client()
	{
		client = nullptr;
	}

	Client::Client(string host, unsigned short port)
	{
		client = std::shared_ptr<IntClient>(new IntClient(host, port));
	}

	Client::Client(IntClient* c)
	{
		client = std::shared_ptr<IntClient>(c);
	}

	void Client::Connect(size_t timeout)
	{
		if(client == nullptr)
			throw socket_error("Client is null", -2);
		client->Connect(timeout);
	}

	void Client::Disconnect()
	{
		if (client == nullptr)
			throw socket_error("Client is null", -2);
		client->Disconnect();
	}

	bool Client::GetNextMessage(vector<byte>& vec)
	{
		if (client == nullptr)
			throw socket_error("Client is null", -2);
		return client->GetNextMessage(vec);
	}

	void Client::Send(byte* msg, byte len)
	{
		if (client == nullptr)
			throw socket_error("Client is null", -2);
		client->Send(msg, len);
	}

	void Client::ThrowNextError()
	{
		if (client == nullptr)
			throw socket_error("Client is null", -2);
		client->ThrowNextError();
	}

	string Client::GetName()
	{
		if (client == nullptr)
			throw socket_error("Client is null", -2);
		return client->name;
	}

	void Client::SetName(string name)
	{
		if (client == nullptr)
			throw socket_error("Client is null", -2);
		client->name = name;
	}

	long long Client::GetId()
	{
		if (client == nullptr)
			throw socket_error("Client is null", -2);
		return client->id;
	}

	void Client::SetId(long long id)
	{
		if (client == nullptr)
			throw socket_error("Client is null", -2);
		client->id = id;
	}

	string Client::GetIP()
	{
		if (client == nullptr)
			throw socket_error("Client is null", -2);
		return client->ip;
	}

	bool Client::IsConnected()
	{
		if (client == nullptr)
			throw socket_error("Client is null", -2);
		return client->connected;
	}
}