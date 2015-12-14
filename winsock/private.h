#pragma once
//WinSock2.h include must be before windows.h
#include <Ws2tcpip.h>
#include <WinSock2.h>
#include <windows.h>
#include <string>
#include <sstream>
#include <vector>
#include <mutex>
#include <thread>
#include <stdexcept>
#include <queue>
#include "winsock.h"

#pragma comment(lib, "ws2_32.lib")

namespace winsock
{
	using std::string;
	using std::vector;
	using std::thread;
	using std::mutex;
	using std::stringstream;

	string GetLastWinsockErrorMessage(PDWORD errorCode);
	string FormatErrorString(const char* header);

	struct BaseSocket
	{
		long long id;
		string name;
		sockaddr_in address;
		SOCKET handle;
		mutex errorMutex;
		std::queue<string> errors; // queue that grabs errors from other threads

		BaseSocket();

		string GetIP();

		void GetNextError();

		void SetNextError(string& error);
	};

	struct IntClient : public BaseSocket
	{
		IntServer* server; //used for server side client connection		
		vector<byte> receiveBuffer;
		thread receiveThread;
		mutex receiveMutex;
		bool connected;

		//constructor for actual client program
		IntClient(string ip, unsigned short port);

		//constructor for server client
		IntClient(SOCKET socket, sockaddr_in _address, IntServer* _server);

		//connect to server
		void Connect();

		//disconnect from server
		void Disconnect();

		//easy to use, writes message to dst
		//array bytes can be accessed directly by calling data() on the vector
		//it consumes the msg
		bool GetNextMessage(vector<byte>& it);

		//the most generic send method
		//msg: pointer to whatever bytes you want to send
		//len: length in bytes
		void Send(byte* msg, byte len);

		void Destroy();
	};

	struct IntServer : public BaseSocket
	{
		thread acceptThread;		
		mutex clientMutex;
		std::queue<SOCKET> clients;
		
		bool running;

		//server constructor
		IntServer(unsigned short port);

		//starts listening and accepting clients
		//maxQueue: max number of clients waiting for connection
		void Start(int maxQueue);

		//disconnect all clients and stop accepting new clients
		void Stop();		

		IntClient* GetNextClient();

		void Destroy();
	};
}