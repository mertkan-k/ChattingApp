#pragma once

#include "TcpSocket.hpp"
#include "TS_Queue.hpp"
#include "Packet.hpp"

class TcpClientSocket : public TcpSocket
{
public:
	bool			Connect();
	virtual bool	IsConnected() const override;

	TcpClientSocket(const std::string& ip, const WORD& port);
	~TcpClientSocket();

	void	ProcessPackets();

private:
	std::string	m_ip;
	WORD		m_port;
	bool		m_isConnected;

	void			ReceivePacket(std::unique_ptr<Packet>& packet);
	void			SendPacket(std::unique_ptr<Packet>& packet);

	TS_Queue<std::unique_ptr<Packet>>	m_packetsReceive;
	TS_Queue<std::unique_ptr<Packet>>	m_packetsSend;

	std::thread		m_packetReceiver;
	std::thread		m_packetSender;

protected:
	bool	StartPacketReceiving();
	void	ReceivePacketAsync();

	bool	StartPacketSending();
	void	SendPacketAsync();
};
