#pragma once

#include "TcpSocket.hpp"

class TcpServerSocket : public TcpSocket
{
public:
	struct ConnectionAcceptionResult
	{
		SOCKET		socket	= 0;
		sockaddr_in	addr	{};
	};


public:
	bool			Listen() override;
	virtual bool	IsConnected() const override;
	bool			Accept(ConnectionAcceptionResult& outResult);

	TcpServerSocket(const std::string& ip, const WORD& port);
	~TcpServerSocket();

private:

	bool		m_isListening;
};
