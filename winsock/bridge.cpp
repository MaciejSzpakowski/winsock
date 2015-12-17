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
			c = Client(ic);
			return true;
		}
	}

	void Server::ThrowNextError()
	{
		server->ThrowNextError();
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
		return server->ip;
	}

	bool Server::IsRunning()
	{
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
		client->Connect(timeout);
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

	void Client::ThrowNextError()
	{
		client->ThrowNextError();
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
		return client->ip;
	}

	bool Client::IsConnected()
	{
		return client->connected;
	}
}