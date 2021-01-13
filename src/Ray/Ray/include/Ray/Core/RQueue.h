#pragma once

#include "../RBase.h"

#include <deque>
#include <mutex>
#include <condition_variable>
#include <functional>


namespace At0::Ray
{
	/// <summary>
	/// Thread-Safe Queue
	/// </summary>
	template<typename T, size_t MaxSize = (size_t)-1>
	class RAY_API Queue
	{
	public:
		using Iterator = typename std::deque<T>::iterator;
		using ReverseIterator = typename std::deque<T>::reverse_iterator;
		using ConstIterator = typename std::deque<T>::const_iterator;
		using ConstReverseIterator = typename std::deque<T>::const_reverse_iterator;

	public:
		Queue() = default;

		void PushFront(T&& elem)
		{
			std::scoped_lock lock(m_Mutex);
			m_Queue.push_front(std::move(elem));
			TrimBuffer();
			m_Condition.notify_all();
		}

		void PushBack(T&& elem)
		{
			std::scoped_lock lock(m_Mutex);
			m_Queue.push_back(std::move(elem));
			TrimBuffer();
			m_Condition.notify_all();
		}

		void PushFront(const T& elem)
		{
			std::scoped_lock lock(m_Mutex);
			m_Queue.push_front(std::move(elem));
			TrimBuffer();
			m_Condition.notify_all();
		}

		void PushBack(const T& elem)
		{
			std::scoped_lock lock(m_Mutex);
			m_Queue.push_back(std::move(elem));
			TrimBuffer();
			m_Condition.notify_all();
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
			return m_Queue.front();
		}

		T& Back()
		{
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

		/// <summary>
		/// Function will wait until the predicate returns true
		/// </summary>
		/// <typeparam name="F">Is any callable object without parameters and returning bool</typeparam>
		/// <param name="pred">Is the function for which the condition will wait to return</param>
		template<typename F, typename = std::enable_if_t<std::is_convertible_v<F, std::function<bool()>>>>
		void WaitFor(F&& pred)
		{
			std::unique_lock lock(m_Mutex);
			m_Condition.wait(lock, pred);
		}

		std::condition_variable& GetWaiter()
		{
			return m_Condition;
		}

		Iterator begin()
		{
			return m_Queue.begin();
		}

		Iterator end()
		{
			return m_Queue.end();
		}
		ReverseIterator rbegin()
		{
			return m_Queue.rbegin();
		}

		ReverseIterator rend()
		{
			return m_Queue.rend();
		}

		ConstIterator cbegin()
		{
			return m_Queue.cbegin();
		}

		ConstIterator cend()
		{
			return m_Queue.cend();
		}

		ConstReverseIterator crbegin()
		{
			return m_Queue.crbegin();
		}

		ConstReverseIterator crend()
		{
			return m_Queue.crend();
		}

	private:
		/// <summary>
		/// Trims the queue down to the maximum allowed size, mutex is already locked at this stage
		/// </summary>
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
		std::condition_variable m_Condition;
	};
}


