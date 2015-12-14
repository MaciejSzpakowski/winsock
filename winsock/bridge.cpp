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
		server = new IntServer(port);
	}

	void Server::Start(int maxQueue)
	{			
		server->Start(maxQueue);
	}

	void Server::Start()
	{
		server->Start(100);
	}

	void Server::Stop()
	{
		server->Stop();
	}

	bool Server::GetNextClient(Client& c)
	{
		IntClient* ic = server->GetNextClient();

		if (ic == nullptr)
			return false;
		else
		{
			c = Client(server->GetNextClient());
			return true;
		}
	}

	void Server::GetNextError()
	{
		server->GetNextError();
	}

	string Server::GetName()
	{
		return server->name;
	}

	void Server::SetName(string name)
	{
		server->name = name;
	}

	long long Server::GetId()
	{
		return server->id;
	}

	void Server::SetId(long long id)
	{
		server->id = id;
	}

	string Server::GetIP()
	{
		return server->GetIP();
	}

	bool Server::IsRunning()
	{
		return server->running;
	}

	Server::~Server()
	{
		if (server != nullptr)
		{
			server->Destroy();
			delete server;
			server = nullptr;
		}
	}

	Server& Server::operator=(Server&& other)
	{
		server = other.server;
		other.server = nullptr;
		return *this;
	}

	Server::Server(Server&& other)
	{
		server = other.server;
	}

	///////Client/////////////
	Client::Client()
	{
		client = nullptr;
	}

	Client::Client(string host, unsigned short port)
	{
		client = new IntClient(host, port);
	}

	Client::Client(IntClient* c)
	{
		client = c;
	}

	void Client::Connect()
	{
		client->Connect();
	}

	void Client::Disconnect()
	{
		client->Disconnect();
	}

	bool Client::GetNextMessage(vector<byte>& vec)
	{
		return client->GetNextMessage(vec);
	}

	void Client::Send(byte* msg, byte len)
	{
		client->Send(msg, len);
	}

	void Client::GetNextError()
	{
		client->GetNextError();
	}

	string Client::GetName()
	{
		return client->name;
	}

	void Client::SetName(string name)
	{
		client->name = name;
	}

	long long Client::GetId()
	{
		return client->id;
	}

	void Client::SetId(long long id)
	{
		client->id = id;
	}

	string Client::GetIP()
	{
		return client->GetIP();
	}

	bool Client::IsConnected()
	{
		return client->connected;
	}

	Client::~Client()
	{
		if (client != nullptr)
		{
			client->Destroy();
			delete client;
			client = nullptr;
		}
	}

	Client& Client::operator=(Client&& other) // move assignment
	{
		client = other.client;
		other.client = nullptr;
		return *this;
	}

	Client& Client::operator=(const Client& other) // copy assignment
	{
		client = other.client;
		return *this;
	}

	Client::Client(Client&& other)
	{
		client = other.client;
	}

	Client::Client(const Client& other)
	{
		client = other.client;
	}
}