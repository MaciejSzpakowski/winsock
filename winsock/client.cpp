#include "private.h"

namespace winsock
{
	void ReceiveThread(IntClient* client)
	{
		byte tempBuffer[100];
		socket_error err;

		while (client->connected)
		{
			int len = recv(client->handle, (char*)tempBuffer, 100, NULL);

			if (len == SOCKET_ERROR)
			{
				err = FormatError("accept() in AcceptThread()");
				client->Disconnect();				
				client->SetNextError(err);
				break;
			}
			else if (len == 0)
			{
				client->Disconnect();
				break;
			}

			client->receiveMutex.lock();
			for (int i = 0; i < len; i++)
				client->receiveBuffer.push_back(tempBuffer[i]);
			client->receiveMutex.unlock();
		}
	}

	IntClient::IntClient(string ip, unsigned short port)
	{
		name = "";
		connected = false;
		server = nullptr;

		LoadWinsock(2, 2);
		//create socket, AF_INET,SOCK_STREAM,NULL for tcp/ip v4, AF_INET6 for ip v6
		handle = socket(AF_INET, SOCK_STREAM, NULL);
		if (handle == INVALID_SOCKET)
		{
			throw FormatError("socket() in Client::IntClient()");
		}
		id = (long long)handle;
		SecureZeroMemory(&address, sizeof(address));
		address.sin_family = AF_INET;
		//this is how to create ip address from string
		inet_pton(AF_INET, ip.c_str(), &(address.sin_addr));
		//port
		address.sin_port = htons(port);

		char buf[100];
		inet_ntop(AF_INET, &(address.sin_addr), buf, 100);
		ip = string(ip);
	}

	IntClient::IntClient(SOCKET socket, sockaddr_in _address, IntServer* _server)
	{
		server = _server;
		connected = true;
		handle = socket;
		address = _address;
		receiveThread = thread(ReceiveThread, this);
		receiveThread.detach();
		id = (long long)handle;
		name = "";

		char buf[100];
		inet_ntop(AF_INET, &(address.sin_addr), buf, 100);
		ip = string(ip);
	}

	void IntClient::Connect()
	{
		if(connect(handle, (sockaddr*)&(address), sizeof(sockaddr_in)) == SOCKET_ERROR)
		{
			throw FormatError("connect() in Client::Connect()");
		}

		connected = true;
		receiveThread = thread(ReceiveThread, this);
		receiveThread.detach();
	}

	void IntClient::Disconnect()
	{
		if (!connected)
			return;

		connected = false;
		if (closesocket(handle) == SOCKET_ERROR)
		{
			throw FormatError("closesocket() in Client::Disconnect()");
		}

		handle = INVALID_SOCKET;
	}

	bool IntClient::GetNextMessage(vector<byte>& vec)
	{
		byte size; // how long is the next message in the buffer
		bool result = false; // was there a message ?

		receiveMutex.lock();
		if (receiveBuffer.size() > 1)
		{
			size = receiveBuffer[0];
			if (receiveBuffer.size() > size)
			{
				result = true;
				vec.insert(vec.end(), receiveBuffer.begin() + 1, receiveBuffer.begin() + 1 + size);

				receiveBuffer.erase(receiveBuffer.begin(), receiveBuffer.begin() + 1 + size);
			}
		}
		receiveMutex.unlock();

		return result;
	}

	void IntClient::Send(byte* msg, byte len)
	{
		int index = 0;
		vector<byte> smsg;	

		if (!connected)
			return;

		smsg.reserve(len + 1);

		smsg.push_back(len);
		smsg.insert(smsg.begin() + 1, msg, msg + len);
		//send function isnt guaranteed to send everything
		//compare return value with msg len and resend if necessary
		while (index < len + 1)
		{
			int sent = send(handle, (const char*)(smsg.data() + index), len - index + 1, 0);

			if (sent == SOCKET_ERROR)
			{
				throw FormatError("send() in Server::Send()");
			}

			index += sent;
		}
	}

	void IntClient::Destroy()
	{
		Disconnect();
	}
}