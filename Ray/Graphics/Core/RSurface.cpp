#include "RSurface.h"

#include "RVulkanInstance.h"
#include "Devices/RWindow.h"


namespace At0::Ray
{
	Surface::Surface(const VulkanInstance& instance, Window& window) : m_VulkanInstance(instance)
	{
		window.CreateSurface(m_VulkanInstance, nullptr, &m_Surface);
	}

	Surface::~Surface()
	{
		vkDestroySurfaceKHR(m_VulkanInstance, m_Surface, nullptr);
	}
}  // namespace At0::Ray