#include "stdafx.h"

#include "TcpServerSocket.hpp"

TcpServerSocket::Client::Client(std::shared_ptr<TcpServerSocket> serverSocket, const SOCKET& socket) :
	m_ServerSocket(serverSocket),
	m_socket(socket),
	m_state(EClientState::INITIALIZED),
	m_packetSenderExit(true)
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
			serverSocket.get()->ReceivePacket(shared_from_this(), std::move(packet));
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

	m_packetSenderExit = false;
	m_packetSender = std::thread(&TcpServerSocket::Client::SendPacketAsync, this);

	return true;
}

void TcpServerSocket::Client::SendPacketAsync()
{
	while (m_packetSenderExit == false)
	{
		std::unique_ptr<const TcpBuffer> packet;
		if (m_packetsSend.PopWait(packet, m_packetSenderExit))
		{
			m_socket.Send(packet);
		}
	}
}

bool TcpServerSocket::Client::SendPacket(std::unique_ptr<TcpBuffer>&& packet)
{
	m_packetsSend.PushNotify(std::move(packet));
	return true;
}
/////////////////////////////////////////////////////////// End Of TcpServerSocket::Client

/////////////////////////////////////////////////////////// Start Of TcpServerSocket
bool TcpServerSocket::ProcessPacket(std::pair<std::shared_ptr<TcpServerSocket::Client>, std::unique_ptr<const TcpBuffer>>& packet)
{
	if (m_packetsReceive.Pop(packet))
	{
		return true;
	}
	return false;
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

bool TcpServerSocket::Accept(std::shared_ptr<TcpServerSocket::Client>& outClient)
{
	TcpServerSocket::ConnectionAcceptionResult acceptResult;

	int addrLen = sizeof(acceptResult.addr);
	acceptResult.socket = accept(m_RealSocket, reinterpret_cast<sockaddr*> (&acceptResult.addr), &addrLen);

	if (acceptResult.socket == INVALID_SOCKET)
		return false;

	InsertClient(acceptResult);

	return true;
}

std::shared_ptr<TcpServerSocket::Client> TcpServerSocket::InsertClient(const ConnectionAcceptionResult& acceptionResult)
{
	auto newClient = std::make_shared<Client>(shared_from_this(), acceptionResult.socket);
	m_clients.Insert(newClient);

	return newClient;
}

void TcpServerSocket::ReceivePacket(std::shared_ptr<Client> client, std::unique_ptr<const TcpBuffer>&& packet)
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

