#pragma once

class Buffer
{
public:
    // Constructor
    explicit Buffer(size_t size = kDefaultBufferSize);

    // Write data to the buffer
    template <typename T>
    void Write(const T& value);

    void Write(const char* data, size_t len);

    // Read data from the buffer
    template <typename T>
    void Read(T& value);

    void Read(char* data, size_t len);

    // Overload << operator to write data to the buffer
    template <typename T>
    Buffer& operator<<(const T& value);

    // Overload >> operator to read data from the buffer
    template <typename T>
    Buffer& operator>>(T& value);

    // Get the current size of the buffer
    size_t Size() const;

    // Get the capacity of the buffer
    size_t Capacity() const;

    // Increase the capacity of the buffer
    void UpdateSize(size_t size);
    // Extend the buffer to the new size
    void SetSize(size_t size);

    // Destructor
    ~Buffer() = default;

protected:
    static constexpr size_t kDefaultBufferSize = 1024; // Default buffer size

    std::unique_ptr<char[]> m_data;
    size_t m_size;     // Current size of data in the buffer
    size_t m_capacity; // Capacity of the buffer
    size_t m_readSeek; // seek of read position
};

// Implementation of member functions

template <typename T>
void Buffer::Write(const T& value)
{
    Write(reinterpret_cast<const char*>(&value), sizeof(value));
}

void Buffer::Write(const char* data, size_t len)
{
    if (m_size + len > m_capacity)
        SetSize(m_size + len);

    std::copy(data, data + len, m_data.get() + m_size);
    m_size += len;
}

template <typename T>
void Buffer::Read(T& value)
{
    Read(reinterpret_cast<char*>(&value), sizeof(value));
}

void Buffer::Read(char* data, size_t len)
{
    if (len > m_size + m_readSeek)
        throw std::out_of_range("Buffer::Read - Not enough data in buffer.");

    std::copy(m_data.get() + m_readSeek, m_data.get() + m_readSeek + len, data);

    m_readSeek += len;
}

// Overload << operator to write data to the buffer
template <typename T>
Buffer& Buffer::operator<<(const T& value)
{
    Write(value);
    return *this;
}

// Overload >> operator to read data from the buffer
template <typename T>
Buffer& Buffer::operator>>(T& value)
{
    Read(value);
    return *this;
}

void Buffer::UpdateSize(size_t size)
{
    SetSize(m_capacity + size);
}

size_t Buffer::Size() const
{
    return m_size;
}

size_t Buffer::Capacity() const
{
    return m_capacity;
}

Buffer::Buffer(size_t size)
    : m_data(std::make_unique<char[]>(size)), m_size(0), m_capacity(size) {}

void Buffer::SetSize(size_t size)
{
    if (size <= m_capacity)
        return;

    // Extend the capacity to the new size
    auto newData = std::make_unique<char[]>(size);

    // Copy the existing data to the new buffer
    std::copy(m_data.get(), m_data.get() + m_size, newData.get());

    // Update data and capacity
    m_data = std::move(newData);
    m_capacity = size;
}
