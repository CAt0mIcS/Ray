#pragma once

#include "RlBase.h"

#include <deque>


namespace Zeal::Reyal
{
	template<typename T, size_t MaxSize = -1>
	class RL_API Queue
	{
		using Iterator = typename std::deque<T>::iterator;
		using ReverseIterator = typename std::deque<T>::reverse_iterator;
		using ConstInterator = typename std::deque<T>::const_iterator;
		using ConstReverseIterator = typename std::deque<T>::const_reverse_iterator;
	public:
		Queue() = default;

		void PushFront(T&& elem)
		{
			std::scoped_lock lock(m_Mutex);
			m_Queue.push_front(std::move(elem));
			TrimBuffer();
		}

		void PushBack(T&& elem)
		{
			std::scoped_lock lock(m_Mutex);
			m_Queue.push_back(std::move(elem));
			TrimBuffer();
		}

		void PushFront(const T& elem)
		{
			std::scoped_lock lock(m_Mutex);
			m_Queue.push_front(std::move(elem));
			TrimBuffer();
		}

		void PushBack(const T& elem)
		{
			std::scoped_lock lock(m_Mutex);
			m_Queue.push_back(std::move(elem));
			TrimBuffer();
		}

		T PopFront()
		{
			std::scoped_lock lock(m_Mutex);
			auto front = std::move(m_Queue.front());
			m_Queue.pop_front();
			return front;
		}

		T PopBack()
		{
			std::scoped_lock lock(m_Mutex);
			auto back = std::move(m_Queue.back());
			m_Queue.pop_back();
			return back;
		}

		T& Front()
		{
			std::scoped_lock lock(m_Mutex);
			return m_Queue.front();
		}

		T& Back()
		{
			std::scoped_lock lock(m_Mutex);
			return m_Queue.back();
		}

		bool Empty() const
		{
			return m_Queue.empty();
		}

		size_t Size() const
		{
			return m_Queue.size();
		}

		Iterator begin()
		{
			std::scoped_lock lock(m_Mutex); 
			return m_Queue.begin();
		}

		Iterator end() 
		{
			std::scoped_lock lock(m_Mutex);
			return m_Queue.end();
		}
		ReverseIterator rbegin() 
		{
			std::scoped_lock lock(m_Mutex);
			return m_Queue.rbegin();
		}

		ReverseIterator rend() 
		{
			std::scoped_lock lock(m_Mutex);
			return m_Queue.rend();
		}

		ConstInterator cbegin() 
		{
			std::scoped_lock lock(m_Mutex);
			return m_Queue.cbegin();
		}
		ConstInterator cend() 
		{
			std::scoped_lock lock(m_Mutex);
			return m_Queue.cend();
		}

		ConstReverseIterator crbegin() 
		{
			std::scoped_lock lock(m_Mutex);
			return m_Queue.crbegin();
		}

		ConstReverseIterator crend() 
		{
			std::scoped_lock lock(m_Mutex);
			return m_Queue.crend();
		}

	private:
		void TrimBuffer()
		{
			while (m_Queue.size() > MaxSize)
			{
				m_Queue.pop_front();
			}
		}

	private:
		std::deque<T> m_Queue;
		std::mutex m_Mutex;
	};
}




