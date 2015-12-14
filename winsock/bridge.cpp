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

	///////Client/////////////
	Client::Client()
	{
		mode = 2;
		client = nullptr;
	}

	Client::Client(string host, unsigned short port)
	{
		mode = 1;
		client = new IntClient(host, port);
	}

	Client::Client(IntClient* c)
	{
		mode = 2;
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
		if (mode == 1)
		{
			client->Destroy();
			delete client;
			client = nullptr;
		}
	}
}