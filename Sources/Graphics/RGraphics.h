#pragma once

#include "../RBase.h"

#include <vulkan/vulkan_core.h>


namespace At0::Ray
{
	class VulkanInstance;
	class PhysicalDevice;
	class Surface;
	class LogicalDevice;

	class RAY_EXPORT Graphics
	{
	public:
		~Graphics();
		static Graphics& Get();

		const VulkanInstance& GetInstance() const { return *m_VulkanInstance; }
		const Surface& GetSurface() const { return *m_Surface; }
		const PhysicalDevice& GetPhysicalDevice() const { return *m_PhysicalDevice; }

	private:
		Graphics();

		// -------------------------------------------------------------
		// Vulkan object creation functions
		void CreateVulkanObjects();

	private:
		inline static Graphics* s_Instance = nullptr;

		Scope<VulkanInstance> m_VulkanInstance;
		Scope<PhysicalDevice> m_PhysicalDevice;
		Scope<Surface> m_Surface;
		Scope<LogicalDevice> m_LogicalDevice;
	};
}  // namespace At0::Ray
