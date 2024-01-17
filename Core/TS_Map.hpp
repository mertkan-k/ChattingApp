#pragma once

#include <map>
#include <mutex>

template<typename K, typename V>
class TS_Map
{
public:
	void Insert(const K& key, const V& value)
	{
		std::scoped_lock lock(m_mutex);
		m_realContainer[key] = value;
	}
	bool Get(const K& key, V& value)
	{
		std::scoped_lock lock(m_mutex);
		auto it = m_realContainer.find();
		if (it == m_realContainer.end())
		{
			return false;
		}
		else
		{
			value = m_realContainer[key];
			return true;
		}
	}
	void Erase(const K& key)
	{
		std::scoped_lock lock(m_mutex);
		m_realContainer.erase(key)
	}

	TS_Map() {}
	~TS_Map() {}

private:
	std::map<K, V>	m_realContainer;
	std::mutex		m_mutex;
};
