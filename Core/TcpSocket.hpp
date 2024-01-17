#pragma once

#include "Buffer.hpp"

class TcpBuffer : public Buffer
{
public:
	char* GetRecvPtr()
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
		m_key.m_key1 ^= 12986231241213150462;
	}
};
#pragma pack(pop)

class TcpSocket
{
public:
	bool Disconnect();
	virtual bool IsConnected() const;

	size_t Send(TcpBuffer& packet);
	bool Recv(std::unique_ptr<TcpBuffer>& outPacket);

	TcpSocket();
	TcpSocket(const SOCKET& sock);
	virtual ~TcpSocket();

protected:
	SOCKET		m_RealSocket;

	template <typename T>
	bool Recv(T& value);

	template <typename T>
	void Send(const T& value);
};
