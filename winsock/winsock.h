#include <stdexcept>
#include <vector>
#include <string>
#include <memory>

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
		int errorCode;
		socket_error() : runtime_error("Nothing"), errorCode(-1) {}
		socket_error(const std::string& msg, int err) : runtime_error(msg), errorCode(err) {}
		socket_error(const char* msg, int err) : runtime_error(msg), errorCode(err) {}
	};

	class Server
	{
	private:
		std::shared_ptr<IntServer> server;
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

		void ThrowNextError();

		//getters and setters

		string GetIP();
		//no need to use this one, all ids are unique
		void SetId(long long id);
		long long GetId();
		void SetName(string name);
		string GetName();
		bool IsRunning();
	};

	class Client
	{
	private:
		std::shared_ptr<IntClient> client;
	public:
		//default constructor
		Client();

		//
		Client(string host, unsigned short port);

		// for internal use only
		Client(IntClient* c);

		//connect to server
		void Connect(size_t timeout);

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

		void ThrowNextError();

		//getters and setters

		string GetIP();
		//no need to use this one, all ids are unique
		void SetId(long long id);
		long long GetId();
		void SetName(string name);
		string GetName();
		bool IsConnected();
	};
}