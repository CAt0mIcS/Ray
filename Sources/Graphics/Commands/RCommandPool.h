#pragma once

#include "../../RBase.h"

#include <thread>
#include <vulkan/vulkan_core.h>


namespace At0::Ray
{
	class RAY_EXPORT CommandPool
	{
	public:
		CommandPool(std::thread::id threadID = std::this_thread::get_id());
		~CommandPool();

		operator const VkCommandPool&() const { return m_CommandPool; }

		const VkCommandPool& GetCommandPool() const { return m_CommandPool; }
		std::thread::id GetThreadID() const { return m_ThreadID; }

	private:
		VkCommandPool m_CommandPool = VK_NULL_HANDLE;
		std::thread::id m_ThreadID;
	};
}  // namespace At0::Ray
