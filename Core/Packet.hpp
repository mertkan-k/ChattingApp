#pragma once

#include "TcpSocket.hpp"

enum class EPacketClientToServer : uint16_t
{
	HANDSHAKE,
};

enum class EPacketServerToClient : uint16_t
{
	HANDSHAKE_ACK,
};

struct Packet
{


	virtual std::unique_ptr<TcpBuffer> Encode() const = 0;
	virtual bool Decode(std::unique_ptr<TcpBuffer> packet) = 0;
};

struct PingPacket : Packet
{
	time_t _time;
	std::unique_ptr<TcpBuffer> Encode() const override
	{
		std::unique_ptr<TcpBuffer> buffer = std::make_unique<TcpBuffer>(sizeof(*this));
		buffer.get()->Write(_time);
		return buffer;
	}
	bool Decode(std::unique_ptr<TcpBuffer> buffer) override
	{
		buffer.get()->Read(_time);
		return true;
	}
};
