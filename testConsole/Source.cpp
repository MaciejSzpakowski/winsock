#include <vector>
#include <iostream>
#include "../winsock/winsock.h"
#pragma comment(lib, "../x64/Release/winsock.lib")

void server()
{
	std::string in;
	unsigned short port;
	std::vector<winsock::Client> clients;
	winsock::Client newClient;
	std::vector<unsigned char> msg;
	winsock::Server server;
	std::string text;

	printf("Port: ");
	std::getline(std::cin, in);
	std::stoi(in);

	try
	{
		server = winsock::Server(port);
		server.Start();
	}
	catch (winsock::socket_error e)
	{
		printf("%s\n", e.what());
		return;
	}

	while (true)
	{
		if (server.GetNextClient(newClient))
		{
			clients.push_back(newClient);
			printf("New client: %s\n", newClient.GetIP().c_str());
		}
		
		for (auto& c : clients)
		{
			if (c.GetNextMessage(msg))
			{
				text = std::string(msg.begin(), msg.end());
				printf("%s: %s\n", c.GetIP().c_str(), text.c_str());

				for (auto& other : clients) // resend
				{
					if (other.GetId() != c.GetId())
						other.Send(msg.data(), msg.size());
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

	printf("IP: ");
	std::getline(std::cin, ip);

	printf("Port: ");
	std::getline(std::cin, in);
	std::stoi(in);

	try
	{
		client = winsock::Client(ip, port);
		client.Connect();
	}
	catch (winsock::socket_error e)
	{
		printf("%s\n", e.what());
		return;
	}

	while (true)
	{
		printf("1: read next msg, 2: write message\n");
		std::getline(std::cin, in);

		if (in == "1")
		{
			if (client.GetNextMessage(msg))
			{
				text = std::string(msg.begin(), msg.end());
				printf("%s: %s\n", client.GetIP().c_str(), text.c_str());
				msg.clear();
			}
		}

		else if (in == "2")
		{
			printf("Message: ");
			std::getline(std::cin, in);
			client.Send((unsigned char*)in.c_str(), in.size());
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

	return 0;
}