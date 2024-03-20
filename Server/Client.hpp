#pragma once

#include "../Core/TcpServerSocket.hpp"

class Client
{
public:
	Client(std::shared_ptr<TcpServerSocket::Client> tcpClient);
	~Client();

	void SendPacket(std::unique_ptr<TcpBuffer>&& buffer);
	void ProcessPacket(const EPacketClientToServer& m_header, std::unique_ptr<TcpBuffer>& buffer);

public:
	std::string m_name;
	size_t		m_id;
	std::shared_ptr<TcpServerSocket::Client> m_tcpClient;
};
