#pragma once

#include <list>
#include <mutex>
#include <chrono>
#include <condition_variable>

template<typename T>
class CSyncQueue
{
private:
	std::mutex m_mutex;
	std::condition_variable m_cond;
	std::list<T> m_queue;
public:
	CSyncQueue()
	{}
	~CSyncQueue()
	{}

	bool put(const T& val)
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		m_queue.push_back(val);
		m_cond.notify_one();
		return true;
	}
		
	bool put(const T& val, int sec)
	{
		return put(val, std::chrono::seconds(sec), 1024);
	}
	bool put(const T& val, std::chrono::steady_clock::duration d, int max_size = 1024);

	bool get(T& val)
	{
		std::unique_lock<std::mutex> lock(m_mutex);
		m_cond.wait(lock, [this] {return !m_queue.empty(); });
		val = m_queue.front();
		m_queue.pop_front();
		return true;
	}

	bool get(T& val, int sec)
	{
		return get(val, std::chrono::seconds(sec));
	}
	bool get(T& val, std::chrono::steady_clock::duration d);
};

template<typename T>
bool CSyncQueue<typename T>::put(const T& val, std::chrono::steady_clock::duration d, int max_size)
{
	std::unique_lock<std::mutex> lock(m_mutex);
	bool not_full = m_cond.wait_for(lock, d, [=, this] {return m_queue.size() < max_size; });
	if (not_full)
	{
		m_queue.push_back(val);
		m_cond.notify_one();
		return true;
	}
	//
	m_cond.notify_all();
	return false;
}

template<typename T>
bool CSyncQueue<typename T>::get(T& val, std::chrono::steady_clock::duration d)
{
	std::unique_lock<std::mutex> lock(m_mutex);
	bool not_empty = m_cond.wait_for(lock, d, [this] {return !m_queue.empty(); });
	if (not_empty)
	{
		val = m_queue.front();
		m_queue.pop_front();
		return true;
	}
	//
	return false;
}


