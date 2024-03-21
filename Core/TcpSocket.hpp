#pragma once

#include "Buffer.hpp"
#include <WinSock2.h>

class TcpBuffer : public Buffer
{
public:
	char* GetPtr() const
	{
		return m_data.get();
	}
};

#pragma pack(push,1)
struct PacketKey
{
	uint64_t m_key1;

	bool operator!=(const PacketKey& other)
	{
		return memcmp(this, &other, sizeof(other) != 0);
	}

};
struct PacketHead
{
	size_t		m_size;
	PacketKey	m_key;
	PacketHead(size_t size) : m_size(size)
	{
		m_key.m_key1 = size;
		m_key.m_key1 ^= 12986231241213150462ULL;
	}
};
#pragma pack(pop)

class TcpSocket
{
public:
	bool Disconnect();
	virtual bool IsConnected() const;

	bool Recv(std::unique_ptr<TcpBuffer>& outPacket);
	void Send(const std::unique_ptr<const TcpBuffer>& packet) const;

	TcpSocket();
	TcpSocket(const SOCKET& sock);
	virtual ~TcpSocket();

protected:
	SOCKET		m_RealSocket;

	template <typename T>
	bool Recv(T& value);

	template <typename T>
	void Send(const T& value) const;
};
