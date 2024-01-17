#pragma once

#include "../Core/TcpServerSocket.hpp"

class Client
{
public:
	Client(std::shared_ptr<TcpServerSocket::Client> tcpClient);
	~Client();

	void SendPacket();

public:
	std::string m_name;
	size_t		m_id;
	std::shared_ptr<TcpServerSocket::Client> m_tcpClient;
};

Client::Client(std::shared_ptr<TcpServerSocket::Client> tcpClient):
	m_tcpClient(tcpClient)
{
	m_name = "UNNAMED";
	m_id = 123;
}

Client::~Client()
{
}
