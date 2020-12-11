#pragma once

#include "../RBase.h"

#include <vector>
#include <mutex>

#include <stack>

namespace At0::Ray
{
	template<typename T>
	class RAY_API Stack
	{
		using Iterator = typename std::vector<T>::iterator;
		using ReverseIterator = typename std::vector<T>::reverse_iterator;
		using ConstInterator = typename std::vector<T>::const_iterator;
		using ConstReverseIterator = typename std::vector<T>::const_reverse_iterator;

	public:
		/// <summary>
		/// Default Stack Constructor
		/// </summary>
		Stack() = default;
		
		/// <summary>
		/// Checks if the stack is empty
		/// </summary>
		/// <returns>True if the stack is empty, false otherwise</returns>
		bool Empty() const
		{
			return m_Stack.empty();
		}

		/// <summary>
		/// Getter for the stack size
		/// </summary>
		/// <returns>The stack size</returns>
		size_t Size() const
		{
			return m_Stack.size();
		}

		/// <summary>
		/// Getter for the top element in the stack
		/// </summary>
		/// <returns>The top stack element</returns>
		T& Top()
		{
			return m_Stack.back();
		}

		/// <summary>
		/// Getter for the top element in the stack
		/// </summary>
		/// <returns>The top stack element</returns>
		const T& Top() const
		{
			return m_Stack.back();
		}

		/// <summary>
		/// Adds an element to the stack
		/// </summary>
		/// <param name="val">Is the object to add</param>
		void PushBack(T&& val)
		{
			std::scoped_lock lock(m_Mutex);
			m_Stack.push_back(std::move(val));
		}
		/// <summary>
		/// Adds an element to the stack
		/// </summary>
		/// <param name="val">Is the object to add</param>
		void PushBack(const T& val)
		{
			std::scoped_lock lock(m_Mutex);
			m_Stack.push_back(val);
		}

		/// <summary>
		/// Adds an element to the stack
		/// </summary>
		/// <typeparam name="...Args">Are initialization arguments for the object</typeparam>
		/// <param name="...args">Are the arguments to pass to the contructor of T</param>
		/// <returns>The added object</returns>
		template <typename... Args>
		decltype(auto) EmplaceBack(Args&&... args) 
		{
			std::scoped_lock lock(m_Mutex);
			return m_Stack.emplace_back(std::forward<Args>(args)...);
		}

		/// <summary>
		/// Finds the element and removes it from the stack
		/// </summary>
		/// <param name="elem">Is the element to remove</param>
		void Pop(const T& elem)
		{
			std::scoped_lock lock(m_Mutex);
			Iterator it = std::find(m_Stack.begin(), m_Stack.end(), elem);
			if (it != m_Stack.end())
			{
				m_Stack.erase(it);
			}
		}

		/// <summary>
		/// Removes element at index i
		/// </summary>
		/// <param name="i">Is the element index in the stack</param>
		void PopIndexed(uint32_t i)
		{
			std::scoped_lock lock(m_Mutex);
			m_Stack.erase(m_Stack.begin() + i);
		}

		/// <summary>
		/// Erases all elements in the stack
		/// </summary>
		void Clear()
		{
			std::scoped_lock lock(m_Mutex);
			m_Stack.clear();
		}

		/// <summary>
		/// Index operator
		/// </summary>
		/// <param name="i">Is the index of the object</param>
		/// <returns>The object at index i</returns>
		decltype(auto) operator[](uint32_t i)
		{
			return m_Stack[i];
		}

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

		ConstInterator cbegin() const
		{
			std::scoped_lock lock(m_Mutex);
			return m_Stack.cbegin();
		}
		ConstInterator cend() const
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
}



