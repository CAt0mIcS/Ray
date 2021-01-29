#pragma once

#include "../RBase.h"

#include <vector>
#include <mutex>

#include <stack>

namespace At0::Ray
{
	template<typename T>
	class Stack
	{
	public:
		using Iterator = typename std::vector<T>::iterator;
		using ReverseIterator = typename std::vector<T>::reverse_iterator;
		using ConstIterator = typename std::vector<T>::const_iterator;
		using ConstReverseIterator = typename std::vector<T>::const_reverse_iterator;

	public:
		/**
		 * Initializes an empty stack
		 */
		Stack() = default;

		/**
		 * @returns If the stack is empty
		 */
		bool Empty() const { return m_Stack.empty(); }

		/**
		 * @returns the size of the stack
		 */
		size_t Size() const { return m_Stack.size(); }

		/**
		 * @returns Element at the top of the staci
		 */
		T& Top() { return m_Stack.back(); }

		/**
		 * @returns Element at the top of the staci
		 */
		const T& Top() const { return m_Stack.back(); }

		/**
		 * Adds a new element to the top of the stack
		 */
		void PushBack(T&& val)
		{
			std::scoped_lock lock(m_Mutex);
			m_Stack.push_back(std::move(val));
		}

		/**
		 * Adds a new in-place constructed element to the top of the stack
		 * @param args.. Are the arguments to create template T
		 * @returns The added element
		 */
		template<typename... Args>
		decltype(auto) EmplaceBack(Args&&... args)
		{
			std::scoped_lock lock(m_Mutex);
			return m_Stack.emplace_back(std::forward<Args>(args)...);
		}

		/**
		 * Finds the element and removes it from the stack. If the element is not found the function
		 * will do nothing
		 * @param elem The element to remove
		 */
		void Pop(T&& elem)
		{
			std::scoped_lock lock(m_Mutex);
			Iterator it = std::find(m_Stack.begin(), m_Stack.end(), elem);
			if (it != m_Stack.end())
			{
				m_Stack.erase(it);
			}
		}

		/**
		 * Removes the element at index i
		 * @param i The index of the elemen to remove
		 */
		void PopIndexed(uint32_t i)
		{
			std::scoped_lock lock(m_Mutex);
			m_Stack.erase(m_Stack.begin() + i);
		}

		/**
		 * Clears the entire stack
		 */
		void Clear()
		{
			std::scoped_lock lock(m_Mutex);
			m_Stack.clear();
		}

		~Stack()
		{
			std::scoped_lock lock(m_Mutex);
			m_Stack.clear();
		}

		decltype(auto) operator[](uint32_t i) { return std::as_const(*this).operator[](i); }

		decltype(auto) operator[](uint32_t i) const { return m_Stack[i]; }

		Iterator begin()
		{
			std::scoped_lock lock(m_Mutex);
			return m_Stack.begin();
		}

		Iterator end()
		{
			std::scoped_lock lock(m_Mutex);
			return m_Stack.end();
		}
		ReverseIterator rbegin()
		{
			std::scoped_lock lock(m_Mutex);
			return m_Stack.rbegin();
		}

		ReverseIterator rend()
		{
			std::scoped_lock lock(m_Mutex);
			return m_Stack.rend();
		}

		ConstIterator begin() const
		{
			std::scoped_lock lock(m_Mutex);
			return m_Stack.cbegin();
		}
		ConstIterator end() const
		{
			std::scoped_lock lock(m_Mutex);
			return m_Stack.cend();
		}

		ConstReverseIterator crbegin() const
		{
			std::scoped_lock lock(m_Mutex);
			return m_Stack.crbegin();
		}

		ConstReverseIterator crend() const
		{
			std::scoped_lock lock(m_Mutex);
			return m_Stack.crend();
		}

	private:
		std::vector<T> m_Stack;
		std::mutex m_Mutex;
	};
}  // namespace At0::Ray
