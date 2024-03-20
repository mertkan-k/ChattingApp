#pragma once

#include "../Core/Core.hpp"

#include "../Core/TcpClientSocket.hpp"

class TcpClientSocket;

class Client : Core
{
public:
	bool	OpenConnectionToServer(const std::string& serverIp, const WORD& port);
	bool	CloseConnectinToServer();
	bool	IsConnectedToServer();

	void	SendPacket(std::unique_ptr<TcpBuffer>&& buffer);
	void	ProcessPacket(const EPacketServerToClient& m_header, std::unique_ptr<TcpBuffer>& buffer);
	size_t	ProcessPackets();
	void	SendMsg(const std::string& msg);

	Client();
	~Client();
private:
	std::unique_ptr<TcpClientSocket>	m_ServerSocket; // socket from client to server

};