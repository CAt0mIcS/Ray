#pragma once

#include "../RBase.h"
#include "../Utils/RNonCopyable.h"

#include <vulkan/vulkan_core.h>


namespace At0::Ray
{
	class VulkanInstance;
	class PhysicalDevice;
	class Surface;
	class LogicalDevice;
	class Swapchain;

	class RAY_EXPORT Graphics : NonCopyable
	{
	public:
		~Graphics();
		static Graphics& Get();

		const VulkanInstance& GetInstance() const { return *m_VulkanInstance; }
		const Surface& GetSurface() const { return *m_Surface; }
		const PhysicalDevice& GetPhysicalDevice() const { return *m_PhysicalDevice; }
		const LogicalDevice& GetDevice() const { return *m_LogicalDevice; }
		const Swapchain& GetSwapchain() const { return *m_Swapchain; }

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
		Scope<Swapchain> m_Swapchain;
	};
}  // namespace At0::Ray
