#pragma once

#include "stdafx.h"
#include "TcpSocket.hpp"

using PacketHeader = uint16_t;

enum EPacketClientToServer : PacketHeader
{
	PING,
	HANDSHAKE,
};

enum EPacketServerToClient : PacketHeader
{
	HANDSHAKE_ACK,
	PONG,
};

template <typename T, T V>
struct Packet
{
	T m_header = V;

	virtual std::unique_ptr<TcpBuffer> Encode() const = 0;
	virtual bool Decode(std::unique_ptr<TcpBuffer>& buffer) = 0;
};

//template <typename T, T V>
//const T Packet<T, V>::m_header = V;

struct PingPacket : Packet<EPacketClientToServer, EPacketClientToServer::PING>
{
	time_t m_time;
	std::unique_ptr<TcpBuffer> Encode() const override
	{
		std::unique_ptr<TcpBuffer> buffer = std::make_unique<TcpBuffer>();
		buffer.get()->Write(m_header);
		buffer.get()->Write(m_time);
		return buffer;
	}
	bool Decode(std::unique_ptr<TcpBuffer>& buffer) override
	{
		buffer.get()->Read(m_header);
		buffer.get()->Read(m_time);
		return true;
	}
};

struct PongPacket : Packet<EPacketServerToClient, EPacketServerToClient::PONG>
{
	time_t m_time;
	std::unique_ptr<TcpBuffer> Encode() const override
	{
		std::unique_ptr<TcpBuffer> buffer = std::make_unique<TcpBuffer>();
		buffer.get()->Write(m_header);
		buffer.get()->Write(m_time);
		return buffer;
	}
	bool Decode(std::unique_ptr<TcpBuffer>& buffer) override
	{
		buffer.get()->Read(m_header);
		buffer.get()->Read(m_time);
		return true;
	}
};
