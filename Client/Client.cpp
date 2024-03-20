#include "stdafx.h"

#include "Client.hpp"
#include "../Core/TcpClientSocket.hpp"

void Client::ProcessPacket(const EPacketServerToClient& m_header, std::unique_ptr<TcpBuffer>& buffer)
{
	switch (m_header)
	{
	case PongPacket::m_header:
	{
		PongPacket packet;
		packet.Decode(buffer);

		auto curTime = time(0);
		std::cout << "pong rec diff: " << curTime - packet.m_time << std::endl;
		break;
	}

	default:
		std::cerr << "unknown packet header: " << m_header << " buffer size: " << buffer.get()->Size() << std::endl;
		break;
	}
}

size_t Client::ProcessPackets()
{
	size_t packet_count = 0;

	std::unique_ptr<TcpBuffer> buffer;
	while (m_ServerSocket.get()->ProcessPacket(buffer))
	{
		EPacketServerToClient m_header;
		buffer.get()->Read(m_header);

		ProcessPacket(m_header, buffer);

		++packet_count;
	}

	return packet_count;
}

void Client::SendPacket(std::unique_ptr<TcpBuffer>&& buffer)
{
	m_ServerSocket.get()->SendPacket(std::move(buffer));
}

void Client::SendMsg(const std::string& msg)
{
	std::unique_ptr<TcpBuffer> buffer;
	//m_ServerSocket.get()->Send();
	// m_ServerSocket.get()->Send(msg.c_str(), msg.length());
}

bool Client::OpenConnectionToServer(const std::string& serverIp, const WORD& port)
{
	if (IsConnectedToServer())
	{
		std::cerr << "Already connected to server!" << std::endl;
		return false;
	}

	if (!IsWSAInitialized())
	{
		std::cerr << "WSA not initialized!" << std::endl;
		return false;
	}

	m_ServerSocket = std::unique_ptr<TcpClientSocket>(new TcpClientSocket(serverIp, port));
	m_ServerSocket.get()->Connect();

	return m_ServerSocket.get()->IsConnected();
}

bool Client::CloseConnectinToServer()
{
	if (IsConnectedToServer() == false)
	{
		std::cerr << "Not connected to server yet!" << std::endl;
		return false;
	}

	return m_ServerSocket.get()->Disconnect();
}

bool Client::IsConnectedToServer()
{
	if (m_ServerSocket && m_ServerSocket.get()->IsConnected())
		return true;

	return false;
}

Client::Client() :
	Core()
{
	std::cout << "Client initialize: " << std::endl;

	StartWSA();
}

Client::~Client()
{
	CleanWSA();

	std::cout << "Client destruct: " << std::endl;
}
