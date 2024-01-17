#pragma once

#include "TcpSocket.hpp"
#include "Packet.hpp"
#include "TS_Set.hpp"
#include "TS_Queue.hpp"
#include "Packet.hpp"

class TcpServerSocket : public TcpSocket
{
public:
	struct ConnectionAcceptionResult
	{
		SOCKET		socket	= 0;
		sockaddr_in	addr	{};
	};
	class Client : public std::enable_shared_from_this<Client>
	{
		enum class EClientState
		{
			INITIALIZED,
			HANDSHAKE,
			ACCEPTED,
		};

	public:
		Client(std::shared_ptr<TcpServerSocket>	serverSocket, const SOCKET& socket);
		~Client();

		bool	StartPacketReceiving();
		bool	StartPacketSending();
	protected:
		void	ReceivePacketAsync();
		void	SendPacketAsync();

		bool	SendPacket(uint8_t header, const Packet& packet);
	private:
		std::weak_ptr<TcpServerSocket>		m_ServerSocket;
		TcpSocket		m_socket;
		EClientState	m_state;
		std::thread		m_packetReceiver;

		TS_Queue<std::unique_ptr<Buffer>>	m_packetsSend;
		std::thread			m_packetSender;
	};

public:
	bool			Listen();
	virtual bool	IsConnected() const override;
	bool			Accept(ConnectionAcceptionResult& outResult);
	std::shared_ptr<TcpServerSocket::Client> InsertClient(const ConnectionAcceptionResult& acceptionResult);

	void			ReceivePacket(std::shared_ptr<Client> client, std::unique_ptr<TcpBuffer>& packet);

	bool			ProcessPacket(std::pair<std::shared_ptr<TcpServerSocket::Client>, std::unique_ptr<TcpBuffer>>& packet);

	TcpServerSocket(const WORD& port);
	~TcpServerSocket();

private:
	WORD		m_port;
	bool		m_isListening;

	TS_Set<std::shared_ptr<Client>>		m_clients;
	TS_Queue<std::pair<std::shared_ptr<Client>, std::unique_ptr<TcpBuffer>>>	m_packetsReceive;
};