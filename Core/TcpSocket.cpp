#include "stdafx.h"

#include "TcpSocket.hpp"

bool TcpSocket::Disconnect()
{
	assert(m_RealSocket != 0, "not valid socket!");

	closesocket(m_RealSocket);
	m_RealSocket = 0;

	return true;
}

bool TcpSocket::IsConnected() const
{
	return m_RealSocket != 0;
}

size_t TcpSocket::Send(const char* buf, size_t len)
{
	send(m_RealSocket, buf, len, 0);

	return true;
}

size_t TcpSocket::Recv(char* buf, size_t len)
{
	int bytesRead = recv(m_RealSocket, buf, len, 0);

	if (bytesRead <= 0)
	{
		Disconnect();
		return 0;
	}

	return bytesRead;
}

TcpSocket::TcpSocket(const std::string& ip, const WORD& port) :
	m_ip(ip),
	m_port(port)
{
	m_RealSocket = socket(AF_INET, SOCK_STREAM, 0);
}

TcpSocket::~TcpSocket()
{
	if (IsConnected())
		Disconnect();
}


