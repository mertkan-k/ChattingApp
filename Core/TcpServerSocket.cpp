#include "stdafx.h"

#include "TcpServerSocket.hpp"

TcpServerSocket::Client::Client(std::shared_ptr<TcpServerSocket> serverSocket, const SOCKET& socket) :
	m_ServerSocket(serverSocket),
	m_socket(socket),
	m_state(EClientState::INITIALIZED)
{
}

TcpServerSocket::Client::~Client()
{
	if (m_packetReceiver.joinable()) {
		std::cout << "Packet receiver thread joined in the destructor." << std::endl;
		m_packetReceiver.join();
	}

	if (m_packetSender.joinable()) {
		std::cout << "Packet sender thread joined in the destructor." << std::endl;
		m_packetSender.join();
	}
}

bool TcpServerSocket::Client::StartPacketReceiving()
{
	if (m_packetReceiver.joinable())
		return false;

	m_packetReceiver = std::thread(&TcpServerSocket::Client::ReceivePacketAsync, this);

	return true;
}

void TcpServerSocket::Client::ReceivePacketAsync()
{
	while (true)
	{
		std::unique_ptr<TcpBuffer> packet = std::make_unique<TcpBuffer>();
		if (m_socket.Recv(packet))
		{
			auto serverSocket = m_ServerSocket.lock();
			serverSocket.get()->ReceivePacket(shared_from_this(), packet);
		}
		else
		{
			m_socket.Disconnect();
			break;
		}
	}
}

bool TcpServerSocket::Client::StartPacketSending()
{
	if (m_packetSender.joinable())
		return false;

	m_packetSender = std::thread(&TcpServerSocket::Client::SendPacketAsync, this);

	return true;
}

void TcpServerSocket::Client::SendPacketAsync()
{
	while (true)
	{
		std::unique_ptr<TcpBuffer> packet;
		if (m_packetsSend.Pop(packet))
		{
			m_socket.Send(packet);
		}
	}
}

bool TcpServerSocket::Client::SendPacket(std::unique_ptr<TcpBuffer>&& packet)
{
	m_packetsSend.Push(std::move(packet));
	return true;
}
/////////////////////////////////////////////////////////// End Of TcpServerSocket::Client

/////////////////////////////////////////////////////////// Start Of TcpServerSocket
bool TcpServerSocket::ProcessPacket(std::pair<std::shared_ptr<TcpServerSocket::Client>, std::unique_ptr<TcpBuffer>>& packet)
{
	if (m_packetsReceive.Pop(packet))
	{
		return true;
	}
	return false;
}

std::shared_ptr<TcpServerSocket::Client> TcpServerSocket::InsertClient(const ConnectionAcceptionResult& acceptionResult)
{
	auto newClient = std::make_shared<Client>(shared_from_this(), acceptionResult.socket);
	m_clients.Insert(newClient);

	return newClient;
}

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

void TcpServerSocket::ReceivePacket(std::shared_ptr<Client> client, std::unique_ptr<TcpBuffer>& packet)
{
	m_packetsReceive.Push(std::make_pair(client, std::move(packet)));
}

TcpServerSocket::TcpServerSocket(const WORD& port) :
	TcpSocket(),
	m_port(port),
	m_isListening(false)
{

}

TcpServerSocket::~TcpServerSocket()
{

}

