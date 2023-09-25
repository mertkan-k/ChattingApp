#include "stdafx.h"

#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

const int PORT = 8080;
const int MAX_CONNECTIONS = 10;

int main() {
	std::cout << "server start." << std::endl;

	std::set<SOCKET> clients;
	std::mutex clientVectorMutex;

	WSADATA wsData;
	WORD ver = MAKEWORD(2, 2);

	int wsOK = WSAStartup(ver, &wsData);
	if (wsOK != 0) {
		std::cerr << "Can't initialize Winsock! Quitting" << std::endl;
		return 1;
	}

	SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (serverSocket == INVALID_SOCKET) {
		std::cerr << "Socket creation failed." << std::endl;
		return 1;
	}

	std::cout << "Server socket created." << std::endl;

	sockaddr_in serverAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(PORT);
	serverAddr.sin_addr.s_addr = INADDR_ANY;

	if (bind(serverSocket, reinterpret_cast<sockaddr*>(&serverAddr), sizeof(serverAddr)) == SOCKET_ERROR) {
		std::cerr << "Binding failed." << std::endl;
		closesocket(serverSocket);
		WSACleanup();
		return 1;
	}

	std::cout << "Binding success." << std::endl;

	if (listen(serverSocket, MAX_CONNECTIONS) == SOCKET_ERROR) {
		std::cerr << "Listening failed." << std::endl;
		closesocket(serverSocket);
		WSACleanup();
		return 1;
	}

	std::cout << "Server is listening on port " << PORT << std::endl;

	sockaddr_in clientAddr;
	int clientAddrSize = sizeof(clientAddr);

	while (true) {
		SOCKET newSocket = accept(serverSocket, reinterpret_cast<sockaddr*>(&clientAddr), &clientAddrSize);
		if (newSocket == INVALID_SOCKET) {
			std::cerr << "Acceptance failed." << std::endl;
			closesocket(serverSocket);
			WSACleanup();
			return 1;
		}

		clients.insert(newSocket);

		std::thread clientThread([&](SOCKET clientSocket)
		{
			std::cout << "[" << clientSocket << "] Client: " << "connected." << std::endl;

			char buffer[1024];
			while (true)
			{
				int bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
				if (bytesRead <= 0) {
					std::cout << "[" << clientSocket << "] Client: " << "disconnected." << std::endl;
					closesocket(clientSocket);
					{
						std::scoped_lock lock(clientVectorMutex);
						clients.erase(clientSocket);
					}
					return;
				}
				buffer[bytesRead] = '\0';
				std::stringstream message;
				message << "[" << clientSocket << "]: " << buffer;
				std::cout << "message: " << message.str() << std::endl;
				{ // send to others
					std::scoped_lock lock(clientVectorMutex);
					for (auto eachClientSocket : clients)
					{
						//if (eachClientSocket != clientSocket)
						{
							send(eachClientSocket, message.str().c_str(), message.str().length(), 0);
						}
					}
				}
			}
		}
		, newSocket);

		clientThread.detach();
	}

	closesocket(serverSocket);
	WSACleanup();

	std::cout << "server end." << std::endl;
	return 0;
}
