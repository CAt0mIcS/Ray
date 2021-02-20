#include "Rpch.h"
#include "RGraphics.h"

#include "Utils/RLogger.h"
#include "Utils/RException.h"

#include "Graphics/Core/RVulkanInstance.h"
#include "Graphics/Core/RPhysicalDevice.h"
#include "Graphics/Core/RLogicalDevice.h"


namespace At0::Ray
{
	Graphics::~Graphics() {}

	Graphics& Graphics::Get()
	{
		static Graphics graphics;
		return graphics;
	}

	Graphics::Graphics() { CreateVulkanObjects(); }

	void Graphics::CreateVulkanObjects() { m_VulkanInstance = MakeScope<VulkanInstance>(); }
}  // namespace At0::Ray
