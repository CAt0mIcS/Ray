#pragma once

#include "../../../RRBase.h"
#include <atomic>

struct GLFWwindow;

namespace At0::Ray
{
	class VulkanDevice;

	class RR_API VulkanGraphicsResources
	{
	protected:
		VulkanGraphicsResources(GLFWwindow* window = nullptr);
		virtual ~VulkanGraphicsResources();

		VulkanDevice& GetDevice() { return *m_VulkanDevice; }

	private:
		static std::atomic<uint64_t> s_RefCount;
		static VulkanDevice* m_VulkanDevice;
	};
}
