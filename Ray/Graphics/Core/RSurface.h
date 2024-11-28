#pragma once

#include "../../RBase.h"
#include "../../Utils/RNonCopyable.h"

#include <vulkan/vulkan_core.h>


namespace At0::Ray
{
	class Window;
	class VulkanInstance;

	class RAY_EXPORT Surface : NonCopyable
	{
	public:
		Surface(const VulkanInstance& instance, Window& window);
		~Surface();

		operator const VkSurfaceKHR&() const { return m_Surface; }

	private:
		VkSurfaceKHR m_Surface = VK_NULL_HANDLE;
		const VulkanInstance& m_VulkanInstance;
	};
}  // namespace At0::Ray
