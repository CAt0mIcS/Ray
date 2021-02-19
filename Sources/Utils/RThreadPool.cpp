#include "Rpch.h"
#include "RThreadPool.h"

#include <mutex>
#include <thread>

#include "Utils/RLogger.h"


namespace At0::Ray
{
	ThreadPool::ThreadPool() : m_Threads(MakeScope<std::thread[]>(m_MaxThreads)), m_Shutdown(false)
	{
		Log::Info("[ThreadPool] Initialized {0} threads", MaxThreads());
		for (uint16_t i = 0; i < m_MaxThreads; ++i)
		{
			m_Threads[i] = std::thread([this]() { InfiniteWait(); });
		}
	}

	ThreadPool& ThreadPool::Get()
	{
		static ThreadPool instance;
		return instance;
	}

	void ThreadPool::Shutdown()
	{
		Log::Info("[ThreadPool] Shutting down");
		{
			std::scoped_lock lock(m_PoolMutex);
			m_Shutdown = true;
		}
		m_TaskQueue.GetWaiter().notify_all();

		Log::Info("[ThreadPool] Joining Threads");
		for (uint16_t i = 0; i < m_MaxThreads; ++i)
		{
			auto id = m_Threads[i].get_id();
			Log::Info("[ThreadPool] Joining Thread {0}", id);
			m_Threads[i].join();
			Log::Info("[ThreadPool] Thread {0} joined", id);
		}
		Log::Info("[ThreadPool] Finnished joining Threads");
	}

	ThreadPool::~ThreadPool()
	{
		if (!m_Shutdown)
		{
			Log::Info("[ThreadPool] Calling ThreadPool::Shutdown from Deconstructor");
			Shutdown();
		}
		Log::Info("[ThreadPool] Destroyed");
	}

	void ThreadPool::InfiniteWait()
	{
		while (!m_Shutdown)
		{
			Log::Info("[ThreadPool] Thread {0} entered ThreadPool::InfiniteWait",
				std::this_thread::get_id());
			std::function<void()> task;
			{
				std::scoped_lock lock(m_QueueMutex);
				m_TaskQueue.WaitFor([this]() { return !m_TaskQueue.Empty() || m_Shutdown; });

				// Need to check again if waiting thread was restored because of m_Shutdown
				if (!m_TaskQueue.Empty())
					task = m_TaskQueue.PopFront();
			}
			if (task)
			{
				Log::Info("[ThreadPool] Thread {0} Task {1} Execution started",
					std::this_thread::get_id(), &task);
				task();
				Log::Info("[ThreadPool] Thread {0} Task {1} Execution finnished",
					std::this_thread::get_id(), &task);
			}
		}
	}
}  // namespace At0::Ray
