#pragma once

#include "../Core/Core.hpp"
#include "../Core/TcpClientSocket.hpp"

class Client : Core
{
public:
	bool	OpenConnectionToServer(const std::string& serverIp, const WORD& port);
	bool	CloseConnectinToServer();
	bool	IsConnectedToServer();
	
	template <typename EPacketClientToServer V>
	void	SendPacket(const Packet<EPacketClientToServer, V>& packet)
	{
		m_ServerSocket.get()->SendPacket(packet.Encode());
	}

	void	ProcessPacket(const EPacketServerToClient& m_header, const std::unique_ptr<const TcpBuffer>& buffer);
	size_t	ProcessPackets();
	void	SendMsg(const std::string& msg);

	Client();
	~Client();
private:
	std::unique_ptr<TcpClientSocket>	m_ServerSocket; // socket from client to server

};