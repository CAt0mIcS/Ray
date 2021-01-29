#pragma once

#include "../RBase.h"
#include "RQueue.h"

#include <mutex>
#include <functional>
#include <thread>


namespace At0::Ray
{
	class RAY_EXPORT ThreadPool
	{
	public:
		/// <summary>
		/// Adds a new task to the queue
		/// </summary>
		/// <typeparam name="F">Is any callable function without parameters or return type</typeparam>
		/// <param name="func">Is the function to execute</param>

		/**
		 * Adds a new task to the queue to execute
		 * @tparam F Any callable function without parameters
		 * @param func The function concurrently
		 */
		template<typename F,
			typename = std::enable_if_t<std::is_convertible_v<F, std::function<void()>>>>
		void AddTask(F&& func);

		/**
		 * @returns The static ThreadPool object
		 */
		static ThreadPool& Get();

		/**
		 * @returns The number of threads
		 */
		uint32_t MaxThreads() const { return m_MaxThreads; }

		/**
		 * Waits for all tasks to finnish and destroys the pool
		 */
		void Shutdown();

		/**
		 * Automatically joins the threads and destroys the pool
		 */
		~ThreadPool();

	private:
		ThreadPool();

		/**
		 * Waits for new tasks to arrivess
		 */
		void InfiniteWait();

	private:
		bool m_Shutdown;

		Scope<std::thread[]> m_Threads;
		Queue<std::function<void()>> m_TaskQueue;

		std::mutex m_PoolMutex;
		std::mutex m_QueueMutex;

		inline static const uint32_t m_MaxThreads = std::thread::hardware_concurrency();
	};


	template<typename F, typename>
	inline void ThreadPool::AddTask(F&& func)
	{
		m_TaskQueue.PushBack(func);
	}
}  // namespace At0::Ray
