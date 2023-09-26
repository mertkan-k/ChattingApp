#include "stdafx.h"

#include "Client.hpp"
#include "../Core/TcpClientSocket.hpp"

void Client::StartMessageHandlerThread()
{
	assert(m_ServerSocket != nullptr);

	std::thread threadHandleMessagesFromServer([this]() {
		char buffer[1024];
		while (true) {
			auto bytesRead = this->m_ServerSocket.get()->Recv(buffer, sizeof(buffer));
			buffer[bytesRead] = '\0';
			std::cout << buffer << std::endl;
		}
		});
	threadHandleMessagesFromServer.detach();
}

void Client::SendMsg(const std::string& msg)
{
	m_ServerSocket.get()->Send(msg.c_str(), msg.length());
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
