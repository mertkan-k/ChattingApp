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
	std::cout << "recv template:: " << typeid(T).name() << " size: " << sizeof(value) << std::endl;

	auto recv_response = recv(m_RealSocket, reinterpret_cast<char*>(&value), sizeof(value), 0);

	if (recv_response == SOCKET_ERROR)
	{
		auto err = WSAGetLastError();
		std::cerr << "Invalid recv! sock err!: err: " << err << std::endl;
		return false;
	}
	if (recv_response != sizeof(value))
	{
		std::cerr << "Invalid recv!: response: " << recv_response << std::endl;
		return false;
	}
	return true;
}
bool TcpSocket::Recv(std::unique_ptr<TcpBuffer>& outPacket)
{
	PacketHead head(0);
	if (Recv(head) == false)
	{
		std::cerr << "Cannot recv head!" << std::endl;
		return false;
	}

	PacketHead test_head { head.m_size};

	if (test_head.m_key != head.m_key)
	{
		std::cerr << "Invalid key!" << "head: " << head.m_key.m_key1 << "test: " << test_head.m_key.m_key1 << std::endl;
		return false;
	}
	std::cout << "key accepted. size: " << head.m_size << std::endl;

	outPacket.get()->SetSize(head.m_size);
	auto recv_response = recv(m_RealSocket, outPacket.get()->GetPtr(), outPacket.get()->Size(), 0);

	if (recv_response == SOCKET_ERROR)
	{
		auto err = WSAGetLastError();
		std::cerr << "Invalid recv! sock err!: err: " << err << std::endl;
		return false;
	}

	std::cout << "recv done. size: " << outPacket.get()->Size() << std::endl;

	return true;
}

template <typename T>
void TcpSocket::Send(const T& value)
{
	std::cout << "send template:: " << typeid(T).name() << " size: " << sizeof(value) << std::endl;

	send(m_RealSocket, reinterpret_cast<const char*>(&value), sizeof(value), 0);
}
void TcpSocket::Send(const std::unique_ptr<TcpBuffer>& value)
{
	std::cout << "send buffer size:: " << value.get()->Size() << std::endl;
	
	std::cout << "send header.." << std::endl;
	PacketHead head(value.get()->Size());
	Send(head);

	std::cout << "send data.." << value.get()->Size() << std::endl;
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


