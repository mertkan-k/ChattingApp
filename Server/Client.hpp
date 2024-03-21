#pragma once

#include "../Core/TcpServerSocket.hpp"

class Client
{
public:
	Client(std::shared_ptr<TcpServerSocket::Client> tcpClient);
	~Client();

	template <typename EPacketServerToClient V>
	void	SendPacket(const Packet<EPacketServerToClient, V>& packet)
	{
		m_tcpClient.get()->SendPacket(packet.Encode());
	}

	void ProcessPacket(const EPacketClientToServer& m_header, std::unique_ptr<const TcpBuffer>& buffer);

public:
	std::string m_name;
	size_t		m_id;
	std::shared_ptr<TcpServerSocket::Client> m_tcpClient;
};
