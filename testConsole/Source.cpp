#include <vector>
#include <iostream>
#include <thread>
#include <chrono>
#include "../winsock/winsock.h"
#ifdef _DEBUG
#pragma comment(lib, "../x64/Debug/winsock.lib")
#else
#pragma comment(lib, "../x64/Release/winsock.lib")
#endif

#include <Windows.h>

void server()
{
	std::string in;
	unsigned short port;
	std::vector<winsock::Client> clients;
	winsock::Client newClient;
	std::vector<unsigned char> msg;
	winsock::Server server;
	std::string text;

	printf("Port:\n>>");
	std::getline(std::cin, in);

	if (in == "")
		port = 10000;
	else
		port = std::stoi(in);

	try
	{
		server = winsock::Server(port);
		server.Start();
		printf("Started\n");
	}
	catch (winsock::socket_error e)
	{
		printf("%s\n", e.what());
		return;
	}

	while (true)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(10));

		if (GetAsyncKeyState('Q'))
			break;

		try
		{
			server.ThrowNextError();
		}
		catch (winsock::socket_error err)
		{
			printf("%s\n", err.what());
		}

		if (server.GetNextClient(newClient))
		{
			clients.push_back(newClient);
			printf("New client: %lld@%s\n", newClient.GetId(), newClient.GetIP().c_str());
		}

		for (int i = (int)clients.size() - 1; i >= 0; i--)
			if (!clients[i].IsConnected())
			{
				printf("%lld@%s disconnected\n",clients[i].GetId(), clients[i].GetIP().c_str());
				clients.erase(clients.begin() + i);
			}

		for (auto& c : clients)
		{
			c.ThrowNextError();

			if (c.GetNextMessage(msg))
			{
				text = std::string(msg.begin(), msg.end());
				printf("%s: %s\n", c.GetIP().c_str(), text.c_str());

				for (auto& other : clients) // resend
				{
					try
					{
						if (other.GetId() != c.GetId())
							other.Send(msg.data(), msg.size());
					}
					catch (winsock::socket_error err)
					{
						printf("%s\n", err.what());
					}
				}

				msg.clear();
			}
		}
	}
}

void client()
{
	std::string in;
	std::string ip;
	unsigned short port;
	winsock::Client client;
	std::vector<unsigned char> msg;
	std::string text;

	printf("IP:\n>>");
	std::getline(std::cin, ip);
	if (ip == "")
		ip = "127.0.0.1";

	printf("Port:\n>>");	
	std::getline(std::cin, in);
	if (in == "")
		port = 10000;
	else
		port = std::stoi(in);

	try
	{
		client = winsock::Client(ip, port);
		client.Connect(1000);
		printf("Connected\n");
	}
	catch (winsock::socket_error e)
	{
		printf("%s\n", e.what());
		return;
	}

	while (true)
	{
		try
		{
			client.ThrowNextError();
		}
		catch (winsock::socket_error e)
		{
			printf("%s\n", e.what());
		}

		if (!client.IsConnected())
		{
			printf("You are disconnected from the server\n");
			return;
		}

		printf("1: read next msg, 2: write message\n>>");
		std::getline(std::cin, in);

		if (in == "1")
		{
			if (client.GetNextMessage(msg))
			{
				text = std::string(msg.begin(), msg.end());
				printf("%s: %s\n", client.GetIP().c_str(), text.c_str());
				msg.clear();
			}
			else
			{
				printf("No new messages\n");
			}
		}

		else if (in == "2")
		{
			printf("Message: ");
			std::getline(std::cin, in);

			try
			{
				client.Send((unsigned char*)in.c_str(), in.size());
			}
			catch (winsock::socket_error e)
			{
				printf("%s\n", e.what());
			}
		}
	}
}

int main()
{
	std::string in;

	printf("1: start server, 2: connect to server\n");
	std::getline(std::cin, in);

	if (in == "1")
		server();
	else if(in == "2")
		client();

	printf("End\n");
	return 0;
}