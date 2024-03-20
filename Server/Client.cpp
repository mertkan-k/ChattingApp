#include "stdafx.h"

#include "Client.hpp"

void Client::ProcessPacket(const EPacketClientToServer& m_header, std::unique_ptr<TcpBuffer>& buffer)
{
	switch (m_header)
	{
	case PingPacket::m_header:
	{
		PingPacket packet;
		packet.Decode(buffer);

		auto curTime = time(0);
		std::cout << "ping rec diff: " << curTime - packet.m_time << std::endl;

		PongPacket pongPacket;
		pongPacket.m_time = curTime;
		SendPacket(pongPacket);
		break;
	}

	default:
		std::cerr << "unknown packet header: " << m_header << " buffer size: " << buffer.get()->Size() << std::endl;
		break;
	}
}

Client::Client(std::shared_ptr<TcpServerSocket::Client> tcpClient) :
	m_tcpClient(tcpClient)
{
	m_name = "UNNAMED";
	m_id = 123;
}

Client::~Client()
{
}

