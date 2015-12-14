#include <stdexcept>
#include <vector>
#include <string>

#pragma once
namespace winsock
{
	using std::string;
	using std::vector;

	typedef unsigned char byte;

	struct IntServer;
	struct IntClient;
	class Server;
	class Client;

	//loads dll required by winsock
	//major and minor: version
	void LoadWinsock(int major, int minor);

	//unloads dll required by winsock
	void UnloadWinsock();

	class socket_error : public std::runtime_error
	{
	public:
		socket_error(const std::string& msg) : runtime_error(msg) {}
		socket_error(const char* msg) : runtime_error(msg) {}
	};

	class Server
	{
	private:
		IntServer* server;
	public:

		//default constructor
		Server();

		Server(unsigned short port);

		//starts listening and accepting clients
		//maxQueue: max number of clients waiting for connection
		void Start(int maxQueue);

		//starts listening and accepting clients
		void Start();

		//disconnect all clients and stop accepting new clients
		void Stop();

		bool GetNextClient(Client& client);

		void GetNextError();

		~Server();

		//getters and setters

		string GetIP();
		//no need to use this one, all ids are unique
		void SetId(long long id);
		long long GetId();
		void SetName(string name);
		string GetName();
		bool IsRunning();

		//rule of five

		Server& operator=(Server&& other);

		Server(Server&& other);
	};

	class Client
	{
	private:
		IntClient* client;
	public:
		//default constructor
		Client();

		//
		Client(string host, unsigned short port);

		// for internal use only
		Client(IntClient* c);

		//connect to server
		void Connect();

		//disconnect from server
		void Disconnect();

		//returns true if new message has been written to msg
		//returns false if there was no new message
		//bytes can be accessed directly by calling data() on the vector
		//it consumes the msg
		bool GetNextMessage(vector<byte>& msg);

		//the most generic send method
		//msg: pointer to whatever bytes you want to send
		//len: length in bytes
		void Send(byte* msg, byte len);

		void GetNextError();

		~Client();

		//getters and setters

		string GetIP();
		//no need to use this one, all ids are unique
		void SetId(long long id);
		long long GetId();
		void SetName(string name);
		string GetName();
		bool IsConnected();

		//rule of five

		Client& operator=(Client&& other);

		Client& operator=(const Client& other); // copy assignment

		Client(Client&& other);

		Client(const Client& other);
	};
}