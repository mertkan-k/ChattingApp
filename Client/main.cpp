#include "stdafx.h"

#include "Client.hpp"

int main() {
	std::cout << "client start." << std::endl;

	Client client;

	if (client.OpenConnectionToServer("127.0.0.1", 12345) == false)
	{
		std::cerr << "cannot connected to the server" << std::endl;
		return 1;
	}

	std::cout << "Connected to server." << std::endl;


	PingPacket pack;
	pack.m_time = time(0);

	std::cout << "time: " << pack.m_time << std::endl;
	client.SendPacket(pack);

	while (true)
	{
		auto packet_count = client.ProcessPackets();
		//std::cout << "packet_count: " << packet_count << std::endl;
		Sleep(100);
	}

	while (true) {

		std::string message;
		std::getline(std::cin, message);

		client.SendMsg(message);

	}

	std::cout << "client end." << std::endl;
	return 0;
}
