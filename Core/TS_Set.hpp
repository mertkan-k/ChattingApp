#pragma once

#include "stdafx.h"

template<typename T>
class TS_Set
{
public:
	void Insert(const T& item)
	{
		std::scoped_lock lock(m_mutex);
		m_realContainer.insert(item);
	}
	void Erase(const T& item)
	{
		std::scoped_lock lock(m_mutex);
		m_realContainer.erase(item);
	}
	template<class Func>
	void ForEach(Func f)
	{
		std::scoped_lock lock(m_mutex);
		
		for (auto& item : m_realContainer)
		{
			f(item);
		}
	}

	TS_Set() {}
	~TS_Set() {}

private:
	std::set<T>	m_realContainer;
	std::mutex	m_mutex;
};
