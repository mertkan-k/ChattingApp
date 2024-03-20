#include "stdafx.h"

#include "Server.hpp"
#include "Client.hpp"
#include "../Core/TcpServerSocket.hpp"

bool Server::StartConnectionListening()
{
	auto isListening = m_ServerSocket.get()->Listen();
	if (isListening == false)
		return false;

	m_ConnectionListener = std::thread(
		[&]()
		{
			while (true) {

				std::shared_ptr<TcpServerSocket::Client> newTcpClient;
				if (WaitForNewConnection(newTcpClient))
				{
					//server.StartPacketAcceptence(newTcpClient);
					auto newClient = std::make_shared<Client>(newTcpClient);

					m_Clients.Insert(newTcpClient, newClient);

					newClient.get()->m_tcpClient.get()->StartPacketReceiving();
					newClient.get()->m_tcpClient.get()->StartPacketSending();

				}
				else
				{
					std::cerr << "Acceptance failed." << std::endl;
					break;
				}
			}
		}
	);

	return true;
}


bool Server::WaitForNewConnection(std::shared_ptr<TcpServerSocket::Client>& outClient)
{
	TcpServerSocket::ConnectionAcceptionResult acceptResult;

	if (m_ServerSocket.get()->Accept(acceptResult))
	{
		outClient = m_ServerSocket.get()->InsertClient(acceptResult);
		return true;
	}
	else
	{
		return false;
	}
}

size_t Server::ProcessPackets()
{
	size_t packetCount = 0;

	std::pair<std::shared_ptr<TcpServerSocket::Client>, std::unique_ptr<TcpBuffer>> packetPair;
	while (m_ServerSocket.get()->ProcessPacket(packetPair))
	{
		std::shared_ptr<Client> realClient;
		if (m_Clients.Get(packetPair.first, realClient) == false)
		{
			std::cerr << "Invalid client!" << std::endl;
			continue;
		}

		EPacketClientToServer m_header;
		packetPair.second.get()->Read(m_header);

		realClient.get()->ProcessPacket(m_header, packetPair.second);

		++packetCount;
	}

	return packetCount;
}

Server::Server() :
	Core()
{
	std::cout << "Server initialize." << std::endl;

	StartWSA();

	m_ServerSocket = std::make_shared<TcpServerSocket>(12345);
}

Server::~Server()
{
	CleanWSA();

	std::cout << "Server destruct." << std::endl;
}
