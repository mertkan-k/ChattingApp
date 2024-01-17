#pragma once

#include "../Core/TcpServerSocket.hpp"

class Client
{
public:
	Client(std::shared_ptr<TcpServerSocket::Client> tcpClient);
	~Client();

	void SendPacket(std::unique_ptr<TcpBuffer>&& packet);

public:
	std::string m_name;
	size_t		m_id;
	std::shared_ptr<TcpServerSocket::Client> m_tcpClient;
};

void Client::SendPacket(std::unique_ptr<TcpBuffer>&& packet)
{
	m_tcpClient.get()->SendPacket(std::move(packet));
}

Client::Client(std::shared_ptr<TcpServerSocket::Client> tcpClient):
	m_tcpClient(tcpClient)
{
	m_name = "UNNAMED";
	m_id = 123;
}

Client::~Client()
{
}
