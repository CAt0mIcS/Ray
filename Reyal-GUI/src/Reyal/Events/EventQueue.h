#pragma once

#include "RlBase.h"
#include "Event.h"

#include <deque>


namespace Zeal::Reyal
{
	class RL_API EventQueue
	{
	public:
		/// <summary>
		/// Default EventQueue Constructor
		/// </summary>
		EventQueue() = default;

		/// <summary>
		/// Pushes an event to the front of the queue
		/// </summary>
		/// <param name="e">Is the event to push</param>
		void PushFront(_In_ const Event& e);

		/// <summary>
		/// Pushes an event to the back of the queue
		/// </summary>
		/// <param name="e">Is the event to push</param>
		void PushBack(_In_ const Event& e);

		/// <summary>
		/// Removes the first element from the queue
		/// </summary>
		void PopFront();

		/// <summary>
		/// Removes the last element form the queue
		/// </summary>
		void PopBack();

		/// <summary>
		/// Gets a reference to the last element in the queue
		/// </summary>
		/// <returns>The last element in the queue</returns>
		Event& Back();

		/// <summary>
		/// Gets a reference to the first element in the queue
		/// </summary>
		/// <returns>The first element in the queue</returns>
		Event& Front();

		/// <summary>
		/// Checks if the queue is empty
		/// </summary>
		/// <returns>True if the queue is empty, false otherwise</returns>
		bool Empty();

		/// <summary>
		/// Reads an event from the queue, returns an invalid event if the queue is empty
		/// </summary>
		/// <returns></returns>
		Event Read();

	private:
		std::deque<Event> m_Queue;
	};
}




