#pragma once

template <typename T>
class PacketConverter
{
public:
	PacketConverter() :
	{}
	~PacketConverter() {}

	T GetHeader(std::unique_ptr<TcpBuffer> packet)
	{
		return static_cast<T>(packet.get()->GetBuffer());
	}

private:
	size_t m_headerSize;
	T packetEnumClass;
};
