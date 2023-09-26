#pragma once

#include "TcpSocket.hpp"

class TcpClientSocket : public TcpSocket
{
public:
	bool			Connect() override;
	virtual bool	IsConnected() const override;

	TcpClientSocket(const std::string& ip, const WORD& port);
	~TcpClientSocket();

private:
	bool		m_isConnected;
};
