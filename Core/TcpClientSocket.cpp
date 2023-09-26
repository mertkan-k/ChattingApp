#include "stdafx.h"

#include "TcpClientSocket.hpp"

bool TcpClientSocket::Connect()
{
	assert(m_RealSocket != 0, "already connected!");

	sockaddr_in serverAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(m_port);
	inet_pton(AF_INET, m_ip.c_str(), &serverAddr.sin_addr);

	if (connect(m_RealSocket, reinterpret_cast<sockaddr*>(&serverAddr), sizeof(serverAddr)) == SOCKET_ERROR)
	{
		std::cerr << "Connection failed." << std::endl;
		m_isConnected = false;
	}
	else
	{
		m_isConnected = true;
	}

	return m_isConnected;
}

bool TcpClientSocket::IsConnected() const
{
	if (TcpSocket::IsConnected() == false)
		return false;

	return m_isConnected;
}

TcpClientSocket::TcpClientSocket(const std::string& ip, const WORD& port) :
	TcpSocket(ip, port),
	m_isConnected(false)
{
	
}

TcpClientSocket::~TcpClientSocket()
{
	
}


