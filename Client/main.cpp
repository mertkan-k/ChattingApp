#include <iostream>
#include <string>
#include <thread>

#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")


void eraseLastLine() {
	//std::cout << "\r";
	std::cout << "\033[F";  // Move cursor up one line
	std::cout << "\033[K";  // Clear the line
}


const char* SERVER_IP = "127.0.0.1";
const int PORT = 8080;

int main() {
	std::cout << "client start." << std::endl;

	WSADATA wsData;
	WORD ver = MAKEWORD(2, 2);

	int wsOK = WSAStartup(ver, &wsData);
	if (wsOK != 0) {
		std::cerr << "Can't initialize Winsock! Quitting" << std::endl;
		return 1;
	}

	SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (clientSocket == INVALID_SOCKET) {
		std::cerr << "Socket creation failed." << std::endl;
		WSACleanup();
		return 1;
	}

	std::cout << "Client socket created." << std::endl;

	sockaddr_in serverAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(PORT);
	inet_pton(AF_INET, SERVER_IP, &serverAddr.sin_addr);

	if (connect(clientSocket, reinterpret_cast<sockaddr*>(&serverAddr), sizeof(serverAddr)) == SOCKET_ERROR) {
		std::cerr << "Connection failed." << std::endl;
		closesocket(clientSocket);
		WSACleanup();
		return 1;
	}

	std::cout << "Connected to server." << std::endl;


	std::thread threadHandleMessagesFromServer([&clientSocket]() {
		char buffer[1024];
		while (true) {
			int bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
			if (bytesRead <= 0) {
				std::cerr << "Server disconnected." << std::endl;
				closesocket(clientSocket);
				WSACleanup();
				return;
			}
			buffer[bytesRead] = '\0';
			std::cout << buffer << std::endl;
		}
		});
	threadHandleMessagesFromServer.detach();

	while (true) {

		std::string message;
		std::getline(std::cin, message);
		eraseLastLine();  // Erase the last line


		send(clientSocket, message.c_str(), message.length(), 0);

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

	closesocket(clientSocket);
	WSACleanup();

	std::cout << "client end." << std::endl;
	return 0;
}
