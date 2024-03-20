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
	const static T m_header = V;

	virtual std::unique_ptr<TcpBuffer> Encode() const = 0;
	virtual bool Decode(const std::unique_ptr<TcpBuffer>& buffer) = 0;
};

#define ENCODE_BEGIN() \
	std::unique_ptr<TcpBuffer> buffer = std::make_unique<TcpBuffer>(); \
	buffer.get()->Write(m_header)

#define ENCODE_DATA(data) \
	buffer.get()->Write(data)

#define ENCODE_END() \
	return buffer

#define DECODE_BEGIN()

#define DECODE_DATA(data) \
	buffer.get()->Read(data)

#define DECODE_END() \
	return true

struct PingPacket : Packet<EPacketClientToServer, EPacketClientToServer::PING>
{
	time_t m_time;
	std::unique_ptr<TcpBuffer> Encode() const override
	{
		ENCODE_BEGIN();
		ENCODE_DATA(m_time);
		ENCODE_END();
	}
	bool Decode(const std::unique_ptr<TcpBuffer>& buffer) override
	{
		DECODE_BEGIN();
		DECODE_DATA(m_time);
		DECODE_END();
	}
};

struct PongPacket : Packet<EPacketServerToClient, EPacketServerToClient::PONG>
{
	time_t m_time;
	std::unique_ptr<TcpBuffer> Encode() const override
	{
		ENCODE_BEGIN();
		ENCODE_DATA(m_time);
		ENCODE_END();
	}
	bool Decode(const std::unique_ptr<TcpBuffer>& buffer) override
	{
		DECODE_BEGIN();
		DECODE_DATA(m_time);
		DECODE_END();
	}
};

#undef ENCODE_BEGIN
#undef ENCODE_DATA
#undef ENCODE_END
#undef DECODE_BEGIN
#undef DECODE_DATA
#undef DECODE_END
