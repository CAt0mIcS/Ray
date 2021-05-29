#pragma once

#include "../RBase.h"
#include "RQueue.h"
#include "../Core/RMath.h"

#include <mutex>
#include <functional>
#include <thread>
#include <future>


namespace At0::Ray
{
	class RAY_EXPORT ThreadPool
	{
	public:
		ThreadPool();

		/**
		 * Submits a new function with arguments to the queue of tasks
		 */
		template<typename F, typename... Args,
			typename = std::enable_if_t<
				std::is_void_v<std::invoke_result_t<std::decay_t<F>, std::decay_t<Args>...>>>>
		/*std::future<bool>*/ void Submit(F&& func, Args&&... args);

		/**
		 * Submits a new function with arguments to the queue of tasks
		 */
		template<typename F, typename... Args,
			typename R = std::invoke_result_t<std::decay_t<F>, std::decay_t<Args>...>,
			typename = std::enable_if_t<!std::is_void_v<R>>>
		/*std::future<R>*/ void Submit(F&& task, Args&&... args);

		template<typename F>
		void PushTask(F&& func);

		template<typename F, typename... Args>
		void PushTask(F&& func, Args&&... args);

		/**
		 * Multithreads a loop
		 * @param firstIdx Starting index (inclusive)
		 * @param lastIdx Ending index (exclusive)
		 * @param loop Function to execute which takes the current loop index
		 */
		template<typename T, typename F>
		void SubmitLoop(T firstIdx, T lastIdx, const F& loop);

		/**
		 * Waits until all task have finished
		 */
		void WaitForTasks();

		/**
		 * @returns The number of threads
		 */
		uint32_t NumThreads() const { return s_MaxThreads; }

		/**
		 * Waits for all tasks to finish and destroys the pool
		 */
		void Shutdown();

		/**
		 * Automatically joins the threads and destroys the pool
		 */
		~ThreadPool();

	private:
		/**
		 * Waits for new tasks to arrivess
		 */
		void InfiniteWait();

	private:
		bool m_Shutdown;

		Scope<std::thread[]> m_Threads;
		Queue<std::function<void()>> m_TaskQueue;
		std::mutex m_QueueMutex;

		static const uint32_t s_MaxThreads;
	};

	template<typename F, typename... Args, typename>
	inline /*std::future<bool>*/ void ThreadPool::Submit(F&& func, Args&&... args)
	{
		PushTask(func, std::forward<Args>(args)...);
	}

	template<typename F, typename... Args, typename R, typename>
	inline /*std::future<R>*/ void ThreadPool::Submit(F&& func, Args&&... args)
	{
		PushTask(func, std::forward<Args>(args)...);
	}

	template<typename F>
	inline void ThreadPool::PushTask(F&& func)
	{
		m_TaskQueue.PushBack(func);
	}

	template<typename F, typename... Args>
	inline void ThreadPool::PushTask(F&& func, Args&&... args)
	{
		PushTask([&func, &args...]() { func(args...); });
	}

	template<typename T, typename F>
	inline void ThreadPool::SubmitLoop(T firstIdx, T lastIdx, const F& loop)
	{
		// Only enable if we can get at least two loops per thread
		std::vector<uint32_t> runsPerThread;
		if (lastIdx / s_MaxThreads >= 2)
			runsPerThread = GenerateChunks((uint32_t)lastIdx, s_MaxThreads);
		else
			runsPerThread.emplace_back((uint32_t)lastIdx);

		uint32_t endIdx = 0;
		for (uint32_t i = 0; i < runsPerThread.size(); ++i)
		{
			uint32_t startIdx = endIdx;
			endIdx += runsPerThread[i];

			PushTask([startIdx, endIdx, &loop]() {
				for (T i = startIdx; i < endIdx; ++i)
					loop(i);
			});
		}
	}

}  // namespace At0::Ray
