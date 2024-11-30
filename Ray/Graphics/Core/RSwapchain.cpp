#include "RSwapchain.h"

#include "RPhysicalDevice.h"
#include "RLogicalDevice.h"
#include "RSurface.h"
#include "Graphics/Images/RImageView.h"
#include "Graphics/Core/RRenderContext.h"


namespace At0::Ray
{
	Swapchain::Swapchain(RenderContext& context, UInt2 framebufferSize, VkSwapchainKHR oldSwapchain,
		VkImageUsageFlags imageUsage)
		: m_Context(context)
	{
		SupportDetails supportDetails = QuerySwapchainSupport();
		VkSurfaceFormatKHR surfaceFormat = ChooseSurfaceFormat(supportDetails.Formats);
		m_Format = surfaceFormat.format;
		VkPresentModeKHR presentMode = ChoosePresentMode(supportDetails.PresentModes);
		m_Extent = ChooseExtent(supportDetails.Capabilities, framebufferSize);

		uint32_t imageCount = supportDetails.Capabilities.minImageCount + 1;
		// Make sure we don't exceed the max image count. 0 means that there is no maximum limit
		if (supportDetails.Capabilities.maxImageCount > 0 &&
			imageCount > supportDetails.Capabilities.maxImageCount)
		{
			imageCount = supportDetails.Capabilities.maxImageCount;
		}

		Log::Info("[Swapchain] Creating {0} images", imageCount);

		VkSwapchainCreateInfoKHR createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		createInfo.surface = m_Context.surface;
		createInfo.minImageCount = imageCount;
		createInfo.imageFormat = surfaceFormat.format;
		createInfo.imageColorSpace = surfaceFormat.colorSpace;
		createInfo.imageExtent = { m_Extent.x, m_Extent.y };
		createInfo.imageArrayLayers = 1;
		createInfo.imageUsage = imageUsage;

		// Queue families
		uint32_t queueFamilyIndices[] = { m_Context.device.GetGraphicsFamily(),
			m_Context.device.GetPresentFamily() };

		// If graphics queue and presentation queue are not the same
		// we need to share images between them.
		if (m_Context.device.GetGraphicsFamily() != m_Context.device.GetPresentFamily())
		{
			createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
			createInfo.queueFamilyIndexCount = 2;
			createInfo.pQueueFamilyIndices = queueFamilyIndices;
			Log::Warn(
				"[Swapchain] Enabling image sharing between the graphics and present families");
		}
		else
		{
			createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
			createInfo.queueFamilyIndexCount = 0;
			createInfo.pQueueFamilyIndices = nullptr;
		}

		createInfo.preTransform = supportDetails.Capabilities.currentTransform;
		createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		createInfo.presentMode = presentMode;
		createInfo.clipped = VK_TRUE;
		createInfo.oldSwapchain = oldSwapchain;

		ThrowVulkanError(vkCreateSwapchainKHR(m_Context.device, &createInfo, nullptr, &m_Swapchain),
			"[Swapchain] Failed to create");

		// Retrieve swapchain images
		uint32_t swapchainImageCount = 0;
		vkGetSwapchainImagesKHR(m_Context.device, m_Swapchain, &swapchainImageCount, nullptr);
		if (swapchainImageCount == 0)
			ThrowRuntime("[Swapchain] Image count is 0");

		m_Images.resize(swapchainImageCount);
		vkGetSwapchainImagesKHR(
			m_Context.device, m_Swapchain, &swapchainImageCount, m_Images.data());

		// Create image views
		m_ImageViews.resize(m_Images.size());
		for (uint32_t i = 0; i < m_ImageViews.size(); ++i)
		{
			m_ImageViews[i] = MakeScope<ImageView>(
				m_Context, m_Images[i], VK_IMAGE_VIEW_TYPE_2D, m_Format, VK_IMAGE_ASPECT_COLOR_BIT);
		}
	}

	Swapchain::~Swapchain()
	{
		vkDestroySwapchainKHR(m_Context.device, m_Swapchain, nullptr);
	}

	Swapchain::SupportDetails Swapchain::QuerySwapchainSupport() const
	{
		SupportDetails supportDetails;

		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(
			m_Context.physicalDevice, m_Context.surface, &supportDetails.Capabilities);

		uint32_t surfaceFormats;
		vkGetPhysicalDeviceSurfaceFormatsKHR(
			m_Context.physicalDevice, m_Context.surface, &surfaceFormats, nullptr);

		supportDetails.Formats.resize(surfaceFormats);
		vkGetPhysicalDeviceSurfaceFormatsKHR(m_Context.physicalDevice, m_Context.surface,
			&surfaceFormats, supportDetails.Formats.data());

		uint32_t presentModeCount;
		vkGetPhysicalDeviceSurfacePresentModesKHR(
			m_Context.physicalDevice, m_Context.surface, &presentModeCount, nullptr);

		supportDetails.PresentModes.resize(presentModeCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR(m_Context.physicalDevice, m_Context.surface,
			&presentModeCount, supportDetails.PresentModes.data());

		return supportDetails;
	}

	float Swapchain::GetAspectRatio() const
	{
		return (float)m_Extent.x / (float)m_Extent.y;
	}

	VkSurfaceFormatKHR Swapchain::ChooseSurfaceFormat(
		const std::vector<VkSurfaceFormatKHR>& formats) const
	{
		for (const auto& format : formats)
		{
			if (format.format == VK_FORMAT_B8G8R8A8_SRGB &&
				format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
				return format;
		}

		return formats.front();
	}

	VkPresentModeKHR Swapchain::ChoosePresentMode(
		const std::vector<VkPresentModeKHR>& presentModes) const
	{
		for (const VkPresentModeKHR& presentMode : presentModes)
		{
			if (presentMode == VK_PRESENT_MODE_MAILBOX_KHR)
			{
				Log::Info("[Swapchain] Choosing mailbox present mode");
				return presentMode;
			}
		}

		Log::Info("[Swapchain] Choosing FIFO present mode");
		return VK_PRESENT_MODE_FIFO_KHR;
	}

	UInt2 Swapchain::ChooseExtent(const VkSurfaceCapabilitiesKHR& capabilities, UInt2 extent) const
	{
		if (capabilities.currentExtent.width != UINT32_MAX)
			return { capabilities.currentExtent.width, capabilities.currentExtent.height };
		else
		{
			// Clamp values to allowed minimum and maximum implementation extents supported
			extent.x = std::clamp(
				extent.x, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
			extent.y = std::clamp(
				extent.y, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

			Log::Info("[Swapchain] Choosing extent [width={0}, height={1}]", extent.x, extent.y);

			return extent;
		}
	}
}  // namespace At0::Ray
