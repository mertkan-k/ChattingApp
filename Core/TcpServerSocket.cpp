#include "stdafx.h"

#include "TcpServerSocket.hpp"

bool TcpServerSocket::Listen()
{
	assert(m_RealSocket != 0, "already connected!");

	sockaddr_in serverAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(m_port);
	serverAddr.sin_addr.s_addr = INADDR_ANY;

	if (bind(m_RealSocket, reinterpret_cast<sockaddr*>(&serverAddr), sizeof(serverAddr)) != SOCKET_ERROR)
	{
		if (listen(m_RealSocket, 50) != SOCKET_ERROR)
		{
			m_isListening = true;
		}
		else
		{
			std::cerr << "Listening failed." << std::endl;
			m_isListening = false;
		}
	}
	else
	{
		std::cerr << "Binding failed." << std::endl;
		m_isListening = false;
	}

	return m_isListening;
}

bool TcpServerSocket::IsConnected() const
{
	if (TcpSocket::IsConnected() == false)
		return false;

	return m_isListening;
}

bool TcpServerSocket::Accept(ConnectionAcceptionResult& outResult)
{
	int addrLen = sizeof(outResult.addr);
	outResult.socket = accept(m_RealSocket, reinterpret_cast<sockaddr*> (&outResult.addr), &addrLen);

	return (outResult.socket != INVALID_SOCKET);
}

TcpServerSocket::TcpServerSocket(const std::string& ip, const WORD& port) :
	TcpSocket(ip, port),
	m_isListening(false)
{

}

TcpServerSocket::~TcpServerSocket()
{

}


