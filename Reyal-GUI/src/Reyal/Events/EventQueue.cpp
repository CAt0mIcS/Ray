#include "rlpch.h"
#include "EventQueue.h"


namespace Zeal::Reyal
{
	void EventQueue::PushFront(_In_ const Event& e)
	{
		m_Queue.push_front(e);
	}

	void EventQueue::PushBack(_In_ const Event& e)
	{
		m_Queue.push_back(e);
	}

	void EventQueue::PopFront()
	{
		m_Queue.pop_front();
	}
	void EventQueue::PopBack()
	{
		m_Queue.pop_back();
	}

	Event& EventQueue::Back()
	{
		return m_Queue.back();
	}

	Event& EventQueue::Front()
	{
		return m_Queue.front();
	}

	bool EventQueue::Empty()
	{
		return m_Queue.empty();
	}

	Event EventQueue::Read()
	{
		if (!Empty())
		{
			auto front = Front();
			PopFront();
			return front;
		}
		return Event();
	}
}


