#pragma once

#include "../RBase.h"

#include <deque>
#include <mutex>
#include <condition_variable>
#include <functional>


namespace At0::Ray
{
	/**
	 * Thread-Safe Queue
	 */
	template<typename T, size_t MaxSize = (size_t)-1>
	class Queue
	{
	public:
		using Iterator = typename std::deque<T>::iterator;
		using ReverseIterator = typename std::deque<T>::reverse_iterator;
		using ConstIterator = typename std::deque<T>::const_iterator;
		using ConstReverseIterator = typename std::deque<T>::const_reverse_iterator;

	public:
		Queue() = default;

		/**
		 * Adds a new element to the front of the queue
		 * @param elem is the element to add
		 */
		void PushFront(T&& elem)
		{
			std::scoped_lock lock(m_Mutex);
			m_Queue.push_front(std::move(elem));
			TrimBuffer();
			m_Condition.notify_all();
		}

		/**
		 * Adds a new element to the back of the queue
		 * @param elem is the element to add
		 */
		void PushBack(T&& elem)
		{
			std::scoped_lock lock(m_Mutex);
			m_Queue.push_back(std::move(elem));
			TrimBuffer();
			m_Condition.notify_all();
		}

		/**
		 * Removes the first element of the queue and returns it
		 * @returns The element that was just removed from the queue
		 */
		T PopFront()
		{
			std::scoped_lock lock(m_Mutex);
			auto front = std::move(m_Queue.front());
			m_Queue.pop_front();
			return front;
		}

		/**
		 * Removes the last element of the queue and returns it
		 * @returns The element that was just removed from the queue
		 */
		T PopBack()
		{
			std::scoped_lock lock(m_Mutex);
			auto back = std::move(m_Queue.back());
			m_Queue.pop_back();
			return back;
		}

		/**
		 * @returns The first element in the queue
		 */
		T& Front() { return m_Queue.front(); }

		/**
		 * @returns The last element in the queue
		 */
		T& Back() { return m_Queue.back(); }

		/**
		 * @returns True if the queue has no elements in it and is empty
		 */
		bool Empty() const { return m_Queue.empty(); }

		/**
		 * @returns The element count in the queue
		 */
		size_t Size() const { return m_Queue.size(); }

		/**
		 * Waits until the predicate returns true. The wait condition is notified whenever
		 * an item is added to the queue
		 * @param pred Function to execute when wait condition is notified
		 */
		template<typename F,
			typename = std::enable_if_t<std::is_convertible_v<F, std::function<bool()>>>>
		void WaitFor(F&& pred)
		{
			std::unique_lock lock(m_Mutex);
			m_Condition.wait(lock, pred);
		}

		std::condition_variable& GetWaiter() { return m_Condition; }

		Iterator begin() { return m_Queue.begin(); }

		Iterator end() { return m_Queue.end(); }
		ReverseIterator rbegin() { return m_Queue.rbegin(); }

		ReverseIterator rend() { return m_Queue.rend(); }

		ConstIterator cbegin() { return m_Queue.cbegin(); }

		ConstIterator cend() { return m_Queue.cend(); }

		ConstReverseIterator crbegin() { return m_Queue.crbegin(); }

		ConstReverseIterator crend() { return m_Queue.crend(); }

	private:
		/**
		 * Trims the queue down to the maximum allowed size, mutex is already locked at this stage
		 */
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
}  // namespace At0::Ray