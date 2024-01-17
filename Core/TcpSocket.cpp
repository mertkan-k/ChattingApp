#include "stdafx.h"

#include "TcpSocket.hpp"
#include "Buffer.hpp"

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

template <typename T>
bool TcpSocket::Recv(T& value)
{
	if (recv(m_RealSocket, reinterpret_cast<char*>(&value), sizeof(value)) != sizeof(value))
	{
		std::cerr << "Invalid recv!: " << typeid(T).name() << std::endl;
		return false;
	}
	return true;
}
bool TcpSocket::Recv(std::unique_ptr<TcpBuffer>& outPacket)
{
	PacketHead head(0);
	Recv(head);

	PacketKey key;
	Recv(key);

	if (head.m_key != key)
	{
		std::cout << "Invalid key!" << std::endl;
		return false;
	}

	outPacket.get()->SetSize(head.m_size);
	int bytesRead = recv(m_RealSocket, outPacket.get()->GetPtr(), outPacket.get()->Size(), 0);

	if (bytesRead <= 0)
	{
		return false;
	}

	return true;
}

template <typename T>
void TcpSocket::Send(const T& value)
{
	send(m_RealSocket, reinterpret_cast<const char*>(&value), sizeof(value));
}
void TcpSocket::Send(const std::unique_ptr<TcpBuffer>& value)
{
	PacketHead head(value.get()->Size());

	Send(head);
	send(m_RealSocket, value.get()->GetPtr(), value.get()->Size(), 0);
}

TcpSocket::TcpSocket(const SOCKET& sock)
{
	m_RealSocket = sock;
}

TcpSocket::TcpSocket()
{
	m_RealSocket = socket(AF_INET, SOCK_STREAM, 0);
}

TcpSocket::~TcpSocket()
{
	if (IsConnected())
		Disconnect();
}


