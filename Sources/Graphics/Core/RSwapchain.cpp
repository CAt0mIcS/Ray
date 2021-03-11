#include "Rpch.h"
#include "RSwapchain.h"

#include "Devices/RWindow.h"

#include "Graphics/RGraphics.h"
#include "RPhysicalDevice.h"
#include "RLogicalDevice.h"
#include "RSurface.h"
#include "Graphics/Images/RImageView.h"

#include "Utils/RException.h"
#include "Utils/RLogger.h"


namespace At0::Ray
{
	Swapchain::Swapchain(VkSwapchainKHR oldSwapchain)
	{
		SupportDetails supportDetails = QuerySwapchainSupport();
		VkSurfaceFormatKHR surfaceFormat = ChooseSurfaceFormat(supportDetails.Formats);
		m_Format = surfaceFormat.format;
		VkPresentModeKHR presentMode = ChoosePresentMode(supportDetails.PresentModes);
		m_Extent = ChooseExtent(supportDetails.Capabilities);

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
		createInfo.surface = Graphics::Get().GetSurface();
		createInfo.minImageCount = imageCount;
		createInfo.imageFormat = surfaceFormat.format;
		createInfo.imageColorSpace = surfaceFormat.colorSpace;
		createInfo.imageExtent = m_Extent;
		createInfo.imageArrayLayers = 1;
		createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

		// Queue families
		uint32_t queueFamilyIndices[] = { Graphics::Get().GetDevice().GetGraphicsFamily(),
			Graphics::Get().GetDevice().GetPresentFamily() };

		// If graphics queue and presentation queue are not the same
		// we need to share images between them.
		if (Graphics::Get().GetDevice().GetGraphicsFamily() !=
			Graphics::Get().GetDevice().GetPresentFamily())
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

		RAY_VK_THROW_FAILED(
			vkCreateSwapchainKHR(Graphics::Get().GetDevice(), &createInfo, nullptr, &m_Swapchain),
			"[Swapchain] Failed to create");

		// Retrieve swapchain images
		uint32_t swapchainImageCount = 0;
		vkGetSwapchainImagesKHR(
			Graphics::Get().GetDevice(), m_Swapchain, &swapchainImageCount, nullptr);
		if (swapchainImageCount == 0)
			RAY_THROW_RUNTIME("[Swapchain] Image count is 0");

		m_Images.resize(swapchainImageCount);
		vkGetSwapchainImagesKHR(
			Graphics::Get().GetDevice(), m_Swapchain, &swapchainImageCount, m_Images.data());

		// Create image views
		m_ImageViews.resize(m_Images.size());
		for (uint32_t i = 0; i < m_ImageViews.size(); ++i)
		{
			m_ImageViews[i] =
				MakeScope<ImageView>(m_Images[i], m_Format, VK_IMAGE_ASPECT_COLOR_BIT);
		}
	}

	Swapchain::~Swapchain()
	{
		vkDestroySwapchainKHR(Graphics::Get().GetDevice(), m_Swapchain, nullptr);
	}

	Swapchain::SupportDetails Swapchain::QuerySwapchainSupport() const
	{
		SupportDetails supportDetails;

		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(Graphics::Get().GetPhysicalDevice(),
			Graphics::Get().GetSurface(), &supportDetails.Capabilities);

		uint32_t surfaceFormats;
		vkGetPhysicalDeviceSurfaceFormatsKHR(Graphics::Get().GetPhysicalDevice(),
			Graphics::Get().GetSurface(), &surfaceFormats, nullptr);

		supportDetails.Formats.resize(surfaceFormats);
		vkGetPhysicalDeviceSurfaceFormatsKHR(Graphics::Get().GetPhysicalDevice(),
			Graphics::Get().GetSurface(), &surfaceFormats, supportDetails.Formats.data());

		uint32_t presentModeCount;
		vkGetPhysicalDeviceSurfacePresentModesKHR(Graphics::Get().GetPhysicalDevice(),
			Graphics::Get().GetSurface(), &presentModeCount, nullptr);

		supportDetails.PresentModes.resize(presentModeCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR(Graphics::Get().GetPhysicalDevice(),
			Graphics::Get().GetSurface(), &presentModeCount, supportDetails.PresentModes.data());

		return supportDetails;
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

	VkExtent2D Swapchain::ChooseExtent(const VkSurfaceCapabilitiesKHR& capabilities) const
	{
		if (capabilities.currentExtent.width != UINT32_MAX)
			return capabilities.currentExtent;
		else
		{
			UInt2 size = Window::Get().GetFramebufferSize();
			VkExtent2D extent = { size.x, size.y };

			// Clamp values to allowed minimum and maximum implementation extents supported
			extent.width = std::clamp(
				extent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
			extent.height = std::clamp(extent.height, capabilities.minImageExtent.height,
				capabilities.maxImageExtent.height);

			Log::Info(
				"[Swapchain] Choosing extent [width={0}, height={1}]", extent.width, extent.height);

			return extent;
		}
	}
}  // namespace At0::Ray
