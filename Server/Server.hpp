#pragma once

#include "../Core/Core.hpp"
#include "../Core/TS_Map.hpp"

//class TcpServerSocket;
//class TcpServerSocket::Client;
#include "../Core/TcpServerSocket.hpp"

class Client;

class Server : Core
{
public:
	bool	StartConnectionListening();
	bool	WaitForNewConnection(std::shared_ptr<TcpServerSocket::Client>& outClient);

	size_t	ProcessPackets();

	Server();
	~Server();
private:
	std::shared_ptr<TcpServerSocket>	m_ServerSocket; // socket from client to server
	std::thread m_ConnectionListener;
	TS_Map<std::shared_ptr<TcpServerSocket::Client>, std::shared_ptr<Client>> m_Clients;
};