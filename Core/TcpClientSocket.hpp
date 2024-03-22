#pragma once

#include "TcpSocket.hpp"
#include "TS_Queue.hpp"
#include "Packet.hpp"

class TcpClientSocket : public TcpSocket
{
public:
	bool			Connect();
	bool	IsConnected() const override;

	TcpClientSocket(const std::string& ip, const WORD& port);
	~TcpClientSocket();

	bool	ProcessPacket(std::unique_ptr<const TcpBuffer>& packet);

	void	ReceivePacket(std::unique_ptr<TcpBuffer>& packet);
	void	SendPacket(std::unique_ptr<TcpBuffer>&& packet);

private:
	std::string	m_ip;
	WORD		m_port;
	bool		m_isConnected;

	TS_Queue<std::unique_ptr<const TcpBuffer>>	m_packetsReceive;
	TS_Queue<std::unique_ptr<const TcpBuffer>>	m_packetsSend;

	std::thread		m_packetReceiver;
	std::thread		m_packetSender;

	bool m_packetSenderExit;

protected:
	bool	StartPacketReceiving();
	void	ReceivePacketAsync();

	bool	StartPacketSending();
	void	SendPacketAsync();
};
