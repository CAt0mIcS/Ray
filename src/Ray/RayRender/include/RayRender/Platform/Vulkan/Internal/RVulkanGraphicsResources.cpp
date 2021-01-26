#include "../../../RRpch.h"
#include "RVulkanGraphicsResources.h"

#include "RVulkanDevice.h"

#include <RayDebug/RAssert.h>


namespace At0::Ray
{
	std::atomic<uint64_t> VulkanGraphicsResources::s_RefCount = 0;
	VulkanDevice* VulkanGraphicsResources::m_VulkanDevice = nullptr;

	VulkanGraphicsResources::VulkanGraphicsResources(GLFWwindow* window)
	{
		if (s_RefCount == 0)
		{
			RAY_MEXPECTS(window, "[VulkanGraphicsResources] First object constructing resources must provide GLFWwindow*");
			m_VulkanDevice = new VulkanDevice(window);
		}

		++s_RefCount;
	}

	VulkanGraphicsResources::~VulkanGraphicsResources()
	{
		--s_RefCount;

		if (s_RefCount == 0)
		{
			delete m_VulkanDevice;
		}
	}
}
