#include "private.h"

namespace winsock
{
	void ReceiveThread(IntClient* client)
	{
		byte tempBuffer[100];
		string msg;

		while (client->connected)
		{
			int len = recv(client->handle, (char*)tempBuffer, 100, NULL);

			if (len == SOCKET_ERROR)
			{
				client->Disconnect();
				msg = FormatErrorString("accept() in AcceptThread()");
				client->SetNextError(msg);
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
			string msg = FormatErrorString("socket() in Client::IntClient()");
			throw socket_error(msg);
		}
		id = (long long)handle;
		SecureZeroMemory(&address, sizeof(address));
		address.sin_family = AF_INET;
		//this is how to create ip address from string
		inet_pton(AF_INET, ip.c_str(), &(address.sin_addr));
		//port
		address.sin_port = htons(port);
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
	}

	void IntClient::Connect()
	{
		if(connect(handle, (sockaddr*)&(address), sizeof(sockaddr_in)) == SOCKET_ERROR)
		{
			string msg = FormatErrorString("connect() in Client::Connect()");
			throw socket_error(msg);
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
			string msg = FormatErrorString("closesocket() in Client::Disconnect()");
			throw socket_error(msg);
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
				for (int i = 1; i < size + 1; i++)
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

		smsg[0] = len;
		smsg.insert(smsg.begin() + 1, msg, msg + len);
		//send function isnt guaranteed to send everything
		//compare return value with msg len and resend if necessary
		while (index < len + 1)
		{
			int sent = send(handle, (const char*)(smsg.data() + index), len - index + 1, 0);

			if (sent == SOCKET_ERROR)
			{
				string msg = FormatErrorString("send() in Server::Send()");
				throw socket_error(msg);
			}

			index += sent;
		}
	}

	void IntClient::Destroy()
	{
	}
}