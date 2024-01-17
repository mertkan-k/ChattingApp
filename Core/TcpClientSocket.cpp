#include "stdafx.h"

#include "TcpClientSocket.hpp"

void TcpClientSocket::ProcessPackets()
{
	while (m_packetsReceive.Size())
	{
		std::unique_ptr<Buffer> packet = m_packetsReceive.Front();
		//
	}
}

void TcpClientSocket::ReceivePacket(std::unique_ptr<Buffer>& packet)
{
	m_packetsReceive.Push(std::move(packet));
}

void TcpClientSocket::SendPacket(std::unique_ptr<Buffer>& packet)
{
	m_packetsSend.Push(std::move(packet));
}

bool TcpClientSocket::StartPacketReceiving()
{
	if (m_packetReceiver.joinable())
		return false;

	m_packetReceiver = std::thread(&TcpClientSocket::ReceivePacketAsync, this);

	return true;
}

void TcpClientSocket::ReceivePacketAsync()
{
	while (true)
	{
		std::unique_ptr<Buffer> packet;
		if (Recv(packet))
		{
			ReceivePacket(packet);
		}
		else
		{
			Disconnect();
			break;
		}
	}
}

bool TcpClientSocket::StartPacketSending()
{
	if (m_packetSender.joinable())
		return false;

	m_packetSender = std::thread(&TcpClientSocket::SendPacketAsync, this);

	return true;
}

void TcpClientSocket::SendPacketAsync()
{
	while (true)
	{
		if (m_packetsSend.Size() > 0)
		{
			std::unique_ptr<Buffer> packet = m_packetsSend.Front();
			Send(*packet.get());
		}
	}
}

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
	TcpSocket(),
	m_ip(ip),
	m_port(port),
	m_isConnected(false)
{
	
}

TcpClientSocket::~TcpClientSocket()
{
	if (m_packetReceiver.joinable()) {
		m_packetReceiver.join();
		std::cout << "Packet receiver thread joined in the destructor." << std::endl;
	}

	if (m_packetSender.joinable()) {
		m_packetSender.join();
		std::cout << "Packet receiver thread joined in the destructor." << std::endl;
	}
}


