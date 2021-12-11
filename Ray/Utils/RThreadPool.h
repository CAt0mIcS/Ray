#pragma once

#include "../RBase.h"
#include "../Core/RMath.h"

#include <mutex>
#include <functional>
#include <thread>
#include <atomic>
#include <deque>


namespace At0::Ray
{
	class RAY_EXPORT ThreadPool
	{
	public:
		ThreadPool(uint32_t threadCount = std::thread::hardware_concurrency());

		/**
		 * Submits a new function with arguments to the queue of tasks
		 */
		template<typename F, typename... Args>
		/*std::future<bool>*/ void Submit(F&& task, Args&&... args) requires std::is_void_v<
			std::invoke_result_t<std::decay_t<F>, std::decay_t<Args>...>>;

		/**
		 * Submits a new function with arguments to the queue of tasks
		 */
		template<typename F, typename... Args,
			typename R = std::invoke_result_t<std::decay_t<F>, std::decay_t<Args>...>>
		/*std::future<R>*/ void Submit(F&& task, Args&&... args) requires(!std::is_void_v<R>);

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
		uint32_t GetThreadCount() const { return m_ThreadCount; }

		uint32_t GetTasksQueued() const;
		uint32_t GetTasksRunning() const { return m_TasksRunning; }

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
		std::deque<std::function<void()>> m_TaskQueue;

		mutable std::mutex m_QueueMutex;
		mutable std::mutex m_PoolMutex;
		std::condition_variable m_Condition;
		std::condition_variable m_TaskFinished;

		// Keeps track of the total number of unfinished tasks
		std::atomic<uint32_t> m_TasksRunning = 0;

		const uint32_t m_ThreadCount;
	};

	template<typename F, typename... Args>
	inline /*std::future<bool>*/ void
		ThreadPool::Submit(F&& task, Args&&... args) requires std::is_void_v<
			std::invoke_result_t<std::decay_t<F>, std::decay_t<Args>...>>
	{
		PushTask(task, std::forward<Args>(args)...);
	}

	template<typename F, typename... Args, typename R>
	/*std::future<R>*/ void ThreadPool::Submit(F&& task, Args&&... args) requires(
		!std::is_void_v<R>)
	{
		PushTask(task, std::forward<Args>(args)...);
	}

	template<typename F>
	inline void ThreadPool::PushTask(F&& func)
	{
		std::scoped_lock lock(m_QueueMutex);
		m_TaskQueue.emplace_back(std::forward<F>(func));
		m_Condition.notify_one();
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
		if (lastIdx / m_ThreadCount >= 2)
			runsPerThread = GenerateChunks((uint32_t)lastIdx, m_ThreadCount);
		else
			runsPerThread.emplace_back((uint32_t)lastIdx);

		uint32_t endIdx = 0;
		for (uint32_t i = 0; i < runsPerThread.size(); ++i)
		{
			uint32_t startIdx = endIdx;
			endIdx += runsPerThread[i];

			PushTask([startIdx, endIdx, i, &loop]() {
				for (T j = startIdx; j < endIdx; ++j)
				{
					if constexpr (std::is_invocable_v<decltype(loop), T, uint32_t>)
						loop(j, i);
					else
						loop(j);
				}
			});
		}
	}

}  // namespace At0::Ray
