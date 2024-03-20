#include "stdafx.h"

#include "Server.hpp"
#include "../Core/TcpServerSocket.hpp"

int main() {
	std::cout << "server start." << std::endl;

	Server server;
	server.StartConnectionListening();

	std::cout << "Server is listening.." << std::endl;

	while (true)
	{
		auto packet_count = server.ProcessPackets();
		//std::cout << "packet_count: " << packet_count << std::endl;
		Sleep(100);
	}

	std::cout << "server end." << std::endl;
	return 0;
}
