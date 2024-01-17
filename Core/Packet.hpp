#pragma once

#include "TcpSocket.hpp"

using PacketHeader = uint16_t;

enum class EPacketClientToServer : PacketHeader
{
	PING,
	HANDSHAKE,
};

enum class EPacketServerToClient : PacketHeader
{
	HANDSHAKE_ACK,
	PONG,
};

template <typename T, T V>
struct Packet
{
	const T header = V;

	virtual std::unique_ptr<TcpBuffer> Encode() const = 0;
	virtual bool Decode(std::unique_ptr<TcpBuffer>& buffer) = 0;
};

//template <typename T, T V>
//const T Packet<T, V>::header = V;

struct PingPacket : Packet<EPacketClientToServer, EPacketClientToServer::PING>
{
	time_t _time;
	std::unique_ptr<TcpBuffer> Encode() const override
	{
		std::unique_ptr<TcpBuffer> buffer = std::make_unique<TcpBuffer>();
		buffer.get()->Write(header);
		buffer.get()->Write(_time);
		return buffer;
	}
	bool Decode(std::unique_ptr<TcpBuffer>& buffer) override
	{
		buffer.get()->Read(header);
		buffer.get()->Read(_time);
		return true;
	}
};

struct PongPacket : Packet<EPacketServerToClient, EPacketServerToClient::PONG>
{
	time_t _time;
	std::unique_ptr<TcpBuffer> Encode() const override
	{
		std::unique_ptr<TcpBuffer> buffer = std::make_unique<TcpBuffer>();
		buffer.get()->Write(header);
		buffer.get()->Write(_time);
		return buffer;
	}
	bool Decode(std::unique_ptr<TcpBuffer>& buffer) override
	{
		buffer.get()->Read(header);
		buffer.get()->Read(_time);
		return true;
	}
};
