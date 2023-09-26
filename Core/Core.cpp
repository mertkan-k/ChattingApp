#include "stdafx.h"
#include "Core.hpp"

void Core::StartWSA()
{
	assert(m_isWSAInit == false, "WSA has already initizlized!");

	WSADATA wsData;
	WORD ver = MAKEWORD(2, 2);

	int wsOK = WSAStartup(ver, &wsData);
	assert(wsOK == 0, "Can't initialize WSA!");

	m_isWSAInit = true;
}

void Core::CleanWSA()
{
	assert(m_isWSAInit == true, "WSA hasn't initizlized yet!");

	m_isWSAInit = false;

	WSACleanup();
}

Core::Core() :
	m_initTime(time(0)),
	m_isWSAInit(false)
{
	std::cout << "Core initialize: " << m_initTime << std::endl;

	// add
}

Core::~Core()
{
	// add
	if (m_isWSAInit)
	{
		CleanWSA();
	}

	std::cout << "Core destruct: " << std::endl;
}
