#include "stdafx.h"

#include "Client.hpp"

int main() {
	std::cout << "client start." << std::endl;

	Client client;

	if (client.OpenConnectionToServer("127.0.0.1", 8080) == false)
	{
		std::cerr << "cannot connected to the server" << std::endl;
		return 1;
	}

	std::cout << "Connected to server." << std::endl;

	client.StartMessageHandlerThread();

	while (true) {

		std::string message;
		std::getline(std::cin, message);
		eraseLastLine();  // Erase the last line

		client.SendMsg(message);

		/*char buffer[1024];
		int bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
		if (bytesRead <= 0) {
			std::cerr << "Server disconnected." << std::endl;
			closesocket(clientSocket);
			WSACleanup();
			return 1;
		}
		buffer[bytesRead] = '\0';
		std::cout << "Server: " << buffer << std::endl;*/
	}

	std::cout << "client end." << std::endl;
	return 0;
}
