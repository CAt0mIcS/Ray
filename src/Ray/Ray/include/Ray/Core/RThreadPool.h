#pragma once

#include "RQueue.h"
#include <mutex>
#include <functional>
#include <thread>


namespace std
{
	class thread;
}


namespace At0::Ray
{
	class ThreadPool
	{
	public:

		/// <summary>
		/// Adds a new task to the queue
		/// </summary>
		/// <typeparam name="F">Is any callable function without parameters or return type</typeparam>
		/// <param name="func">Is the function to execute</param>
		template<typename F, typename = std::enable_if_t<std::is_convertible_v<F, std::function<void()>>>>
		void AddTask(F&& func);

		/// <summary>
		/// Getter for the ThreadPool
		/// </summary>
		/// <returns>The Thread Pool</returns>
		static ThreadPool& Get();

		/// <summary>
		/// Getter for the amount of available threads
		/// </summary>
		/// <returns>The amount of threads in the list</returns>
		uint32_t MaxThreads() const { return m_MaxThreads; }

		/// <summary>
		/// Waits until all threads have finnished
		/// </summary>
		void Shutdown();

		/// <summary>
		/// ThreadPool Deconstructor, automatically stops threads
		/// </summary>
		~ThreadPool();

	private:
		ThreadPool();

		/// <summary>
		/// Function will wait for incomming tasks and execute them
		/// </summary>
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
}


