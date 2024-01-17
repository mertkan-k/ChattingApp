#include "stdafx.h"

#include "Server.hpp"
#include "../Core/TcpServerSocket.hpp"

int main() {
	std::cout << "server start." << std::endl;

	Server server;
	server.StartConnectionListening();

	std::cout << "Server is listening.." << std::endl;

	Sleep(10000);

	std::cout << "server end." << std::endl;
	return 0;
}
