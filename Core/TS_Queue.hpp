#pragma once

#include "stdafx.h"

template<typename T>
class TS_Queue
{
public:
	void Push(T&& item)
	{
		std::scoped_lock<std::mutex> lock(m_mutex);
		m_realContainer.emplace(std::move(item));
	}
	size_t Size()
	{
		std::scoped_lock<std::mutex> lock(m_mutex);
		return m_realContainer.size();
	}
	bool Pop(T&item)
	{
		std::scoped_lock<std::mutex> lock(m_mutex);
		if (m_realContainer.size())
		{
			item = std::move(m_realContainer.front());
			m_realContainer.pop();
			return true;
		}
		else
			return false;
	}
	bool PopWait(T& item, bool& exitVar)
	{
		std::unique_lock<std::mutex> lock(m_mutex);
		cv.wait(lock, [&] { return m_realContainer.size() != 0 || exitVar; } );

		if (exitVar) return false;

		item = std::move(m_realContainer.front());
		m_realContainer.pop();
		return true;
	}
	void PushNotify(T&& item)
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		m_realContainer.emplace(std::move(item));
		cv.notify_one();
	}

	TS_Queue() {}
	~TS_Queue() {}

private:
	std::condition_variable cv;
	std::queue<T>	m_realContainer;
	std::mutex		m_mutex;
};
