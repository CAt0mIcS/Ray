#pragma once

#include "../RBase.h"

#include <vulkan/vulkan_core.h>


namespace At0::Ray
{
	class VulkanInstance;
	class PhysicalDevice;
	class LogicalDevice;

	class RAY_EXPORT Graphics
	{
	public:
		~Graphics();
		static Graphics& Get();

	private:
		Graphics();

		// -------------------------------------------------------------
		// Vulkan object creation functions
		void CreateVulkanObjects();

	private:
		Scope<VulkanInstance> m_VulkanInstance;
		// Scope<PhysicalDevice> m_PhysicalDevice;
		// Scope<LogicalDevice> m_LogicalDevice;
	};
}  // namespace At0::Ray
