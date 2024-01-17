#pragma once

#include <queue>
#include <mutex>

template<typename T>
class TS_Queue
{
public:
	void Push(const T& item)
	{
		std::scoped_lock lock(m_mutex);
		m_realContainer.push(item);
	}
	/*void Push(TT& item)
	{
		std::scoped_lock lock(m_mutex);
		m_realContainer.emplace(std::move(item));
	}*/
	T Front()
	{
		std::scoped_lock lock(m_mutex);
		T item = m_realContainer.front();
		m_realContainer.pop();
		return item;
	}
	size_t Size()
	{
		std::scoped_lock lock(m_mutex);
		return m_realContainer.size();
	}

	TS_Queue() {}
	~TS_Queue() {}

private:
	std::queue<T>	m_realContainer;
	std::mutex		m_mutex;
};
