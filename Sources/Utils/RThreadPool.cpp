#include "Rpch.h"
#include "RThreadPool.h"

#include <mutex>
#include <thread>

#include "Utils/RLogger.h"


namespace At0::Ray
{
	const uint32_t ThreadPool::s_MaxThreads = std::thread::hardware_concurrency();

	ThreadPool::ThreadPool() : m_Threads(MakeScope<std::thread[]>(s_MaxThreads)), m_Shutdown(false)
	{
		Log::Info("[ThreadPool] Initialized {0} threads", NumThreads());
		for (uint16_t i = 0; i < s_MaxThreads; ++i)
		{
			m_Threads[i] = std::thread([this]() { InfiniteWait(); });
		}
	}

	void ThreadPool::WaitForTasks()
	{
		// m_TaskQueue.WaitFor([this]() { return m_TaskQueue.Empty(); });
		while (!m_TaskQueue.Empty())
		{
		}
	}

	void ThreadPool::Shutdown()
	{
		Log::Info("[ThreadPool] Shutting down");
		m_Shutdown = true;
		m_TaskQueue.GetWaiter().notify_all();

		Log::Info("[ThreadPool] Joining Threads");
		for (uint16_t i = 0; i < s_MaxThreads; ++i)
		{
			auto id = m_Threads[i].get_id();
			Log::Info("[ThreadPool] Joining Thread {0}", id);
			m_Threads[i].join();
			Log::Info("[ThreadPool] Thread {0} joined", id);
		}
		Log::Info("[ThreadPool] Finished joining Threads");
	}

	ThreadPool::~ThreadPool()
	{
		if (!m_Shutdown)
		{
			Log::Debug("[ThreadPool] Calling ThreadPool::Shutdown from Deconstructor");
			Shutdown();
		}
		Log::Info("[ThreadPool] Destroyed");
	}

	void ThreadPool::InfiniteWait()
	{
		while (!m_Shutdown)
		{
			std::function<void()> task;
			{
				std::scoped_lock lock(m_QueueMutex);
				m_TaskQueue.WaitFor([this]() { return !m_TaskQueue.Empty() || m_Shutdown; });

				if (m_Shutdown)
					return;
				task = m_TaskQueue.PopFront();
			}
			task();
		}
	}
}  // namespace At0::Ray
