#pragma once

#include "../Core/Core.hpp"

class TcpClientSocket;

class Client : Core
{
public:
	bool	OpenConnectionToServer(const std::string& serverIp, const WORD& port);
	bool	CloseConnectinToServer();
	bool	IsConnectedToServer();

	void	StartMessageHandlerThread();
	void	SendMsg(const std::string& msg);

	Client();
	~Client();
private:
	std::unique_ptr<TcpClientSocket>	m_ServerSocket; // socket from client to server

};