#pragma once

class TcpSocket
{
public:
	virtual bool Connect() { return false; }
	virtual bool Listen() { return false; }
	bool Disconnect();
	virtual bool IsConnected() const;

	size_t Send(const char* buf, size_t len);
	size_t Recv(char* buf, size_t len);


	TcpSocket(const std::string& ip, const WORD& port);
	virtual ~TcpSocket();

protected:
	std::string	m_ip;
	WORD		m_port;

	SOCKET		m_RealSocket;
};
