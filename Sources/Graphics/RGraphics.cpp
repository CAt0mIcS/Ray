#include "Rpch.h"
#include "RGraphics.h"

#include "Utils/RLogger.h"
#include "Utils/RException.h"

#include "Graphics/Core/RVulkanInstance.h"
#include "Graphics/Core/RPhysicalDevice.h"
#include "Graphics/Core/RSurface.h"
#include "Graphics/Core/RLogicalDevice.h"


namespace At0::Ray
{
	Graphics::~Graphics()
	{
		m_LogicalDevice->WaitIdle();

		m_LogicalDevice.reset();
		m_Surface.reset();
		m_PhysicalDevice.reset();
		m_VulkanInstance.reset();

		delete s_Instance;
	}

	Graphics& Graphics::Get()
	{
		if (!s_Instance)
			new Graphics();
		return *s_Instance;
	}

	Graphics::Graphics()
	{
		if (s_Instance)
			RAY_THROW_RUNTIME("[Graphics] Object already creaed.");

		s_Instance = this;
		CreateVulkanObjects();
	}

	void Graphics::CreateVulkanObjects()
	{
		m_VulkanInstance = MakeScope<VulkanInstance>();
		m_PhysicalDevice = MakeScope<PhysicalDevice>();
		m_Surface = MakeScope<Surface>();
		m_LogicalDevice = MakeScope<LogicalDevice>();
	}
}  // namespace At0::Ray
