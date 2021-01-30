#include "Rpch.h"
#include "RSwapchain.h"

#include "Devices/Vulkan/RPhysicalDevice.h"
#include "Devices/Vulkan/RLogicalDevice.h"
#include "Devices/Vulkan/RSurface.h"

#include "Debug/RException.h"
#include "Resources/RImage.h"


namespace At0::Ray
{
	static const std::vector<VkCompositeAlphaFlagBitsKHR> g_CompositeAlphaFlags = {
		VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
		VK_COMPOSITE_ALPHA_PRE_MULTIPLIED_BIT_KHR,
		VK_COMPOSITE_ALPHA_POST_MULTIPLIED_BIT_KHR,
		VK_COMPOSITE_ALPHA_INHERIT_BIT_KHR,
	};

	Swapchain::Swapchain(const VkExtent2D& extent, const Swapchain* oldSwapchain)
		: m_Extent(extent), m_PresentMode(VK_PRESENT_MODE_FIFO_KHR),
		  m_PreTransform(VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR),
		  m_CompositeAlpha(VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR),
		  m_ActiveImageIndex(std::numeric_limits<uint32_t>::max())
	{
		const PhysicalDevice& physicalDevice = Graphics::Get().GetPhysicalDevice();
		const Surface& surface = Graphics::Get().GetSurface();
		const LogicalDevice& logicalDevice = Graphics::Get().GetLogicalDevice();

		const VkSurfaceFormatKHR& surfaceFormat = surface.GetFormat();
		const VkSurfaceCapabilitiesKHR& surfaceCapabilities = surface.GetCapabilities();
		uint32_t graphicsFamily = logicalDevice.GetGraphicsFamily();
		uint32_t presentFamily = logicalDevice.GetPresentFamily();

		// Get the best present mode
		uint32_t physicalPresentModeCount;
		vkGetPhysicalDeviceSurfacePresentModesKHR(
			physicalDevice, surface, &physicalPresentModeCount, nullptr);
		std::vector<VkPresentModeKHR> physicalPresentModes(physicalPresentModeCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR(
			physicalDevice, surface, &physicalPresentModeCount, physicalPresentModes.data());

		for (const auto& presentMode : physicalPresentModes)
		{
			if (presentMode == VK_PRESENT_MODE_MAILBOX_KHR)
			{
				m_PresentMode = presentMode;
				break;
			}

			if (presentMode != VK_PRESENT_MODE_MAILBOX_KHR &&
				presentMode == VK_PRESENT_MODE_IMMEDIATE_KHR)
				m_PresentMode = presentMode;
		}

		uint32_t desiredImageCount = surfaceCapabilities.minImageCount + 1;

		if (surfaceCapabilities.maxImageCount > 0 &&
			desiredImageCount > surfaceCapabilities.maxImageCount)
			desiredImageCount = surfaceCapabilities.maxImageCount;

		if (surfaceCapabilities.supportedTransforms & VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR)
			m_PreTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
		else
			m_PreTransform = surfaceCapabilities.currentTransform;

		for (const auto& compositeAlphaFlag : g_CompositeAlphaFlags)
		{
			if (surfaceCapabilities.supportedCompositeAlpha & compositeAlphaFlag)
			{
				m_CompositeAlpha = compositeAlphaFlag;
				break;
			}
		}

		VkSwapchainCreateInfoKHR createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		createInfo.surface = surface;
		createInfo.minImageCount = desiredImageCount;
		createInfo.imageFormat = surfaceFormat.format;
		createInfo.imageColorSpace = surfaceFormat.colorSpace;
		createInfo.imageExtent = m_Extent;
		createInfo.imageArrayLayers = 1;
		createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
		createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		createInfo.preTransform = (VkSurfaceTransformFlagBitsKHR)m_PreTransform;
		createInfo.compositeAlpha = m_CompositeAlpha;
		createInfo.presentMode = m_PresentMode;
		createInfo.clipped = VK_TRUE;
		createInfo.oldSwapchain = *oldSwapchain;

		if (surfaceCapabilities.supportedUsageFlags & VK_IMAGE_USAGE_TRANSFER_SRC_BIT)
			createInfo.imageUsage |= VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
		if (surfaceCapabilities.supportedUsageFlags & VK_IMAGE_USAGE_TRANSFER_DST_BIT)
			createInfo.imageUsage |= VK_IMAGE_USAGE_TRANSFER_DST_BIT;

		if (graphicsFamily != presentFamily)
		{
			std::array<uint32_t, 2> queueFamily = { graphicsFamily, presentFamily };
			createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
			createInfo.queueFamilyIndexCount = (uint32_t)queueFamily.size();
			createInfo.pQueueFamilyIndices = queueFamily.data();
		}

		// Get swap chain images
		RAY_VK_THROW_FAILED(vkCreateSwapchainKHR(logicalDevice, &createInfo, nullptr, &m_Swapchain),
			"[Swapchain] Failed to create.");

		RAY_VK_THROW_FAILED(
			vkGetSwapchainImagesKHR(logicalDevice, m_Swapchain, &m_ImageCount, nullptr),
			"[Swapchain] Failed to get image count.");

		m_Images.resize(m_ImageCount);
		m_ImageViews.resize(m_ImageCount);

		RAY_VK_THROW_FAILED(
			vkGetSwapchainImagesKHR(logicalDevice, m_Swapchain, &m_ImageCount, m_Images.data()),
			"[Swapchain] Failed to get image count.");

		for (uint32_t i = 0; i < m_ImageCount, ++i)
		{
			Image::CreateImageViews(m_Images[i], m_ImageViews[i], VK_IMAGE_VIEW_TYPE_2D,
				surfaceFormat.format, VK_IMAGE_ASPECT_COLOR_BIT, 1, 0, 1, 0);
		}

		VkFenceCreateInfo fenceCreateInfo{};
		fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		RAY_VK_THROW_FAILED(vkCreateFence(logicalDevice, &fenceCreateInfo, nullptr, &m_FenceImage),
			"[Swapchain] Failed to create fence.");
	}

	Swapchain::~Swapchain()
	{
		auto logicalDevice = Graphics::Get()->GetLogicalDevice();
		vkDestroySwapchainKHR(*logicalDevice, m_Swapchain, nullptr);

		for (const VkImageView& imageView : m_ImageViews)
		{
			vkDestroyImageView(*logicalDevice, imageView, nullptr);
		}

		vkDestroyFence(*logicalDevice, m_FenceImage, nullptr);
	}

	VkResult Swapchain::AcquireNextImage(const VkSemaphore& presentCompleteSemaphore, VkFence fence)
	{
		const LogicalDevice& logicalDevice = Graphics::Get().GetLogicalDevice();

		if (fence != VK_NULL_HANDLE)
		{
			RAY_VK_THROW_FAILED(vkWaitForFences(logicalDevice, 1, &fence, VK_TRUE,
									std::numeric_limits<uint64_t>::max()),
				"[Swapchain] Failed to wait for fences.");
		}

		auto acquireResult =
			vkAcquireNextImageKHR(logicalDevice, m_Swapchain, std::numeric_limits<uint64_t>::max(),
				presentCompleteSemaphore, VK_NULL_HANDLE, &m_ActiveImageIndex);

		if (acquireResult != VK_SUCCESS && acquireResult != VK_SUBOPTIMAL_KHR &&
			acquireResult != VK_ERROR_OUT_OF_DATE_KHR)
			RAY_VK_THROW_NO_EXPR("[Swapchain] Failed to acquire next image.");
	}

	VkResult Swapchain::QueuePresent(const VkQueue& presentQueue, const VkSemaphore& waitSemaphore)
	{
		VkPresentInfoKHR presentInfo{};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pWaitSemaphores = &waitSemaphore;
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = &m_Swapchain;
		presentInfo.pImageIndices = &m_ActiveImageIndex;
		return vkQueuePresentKHR(presentQueue, &presentInfo);
	}
}  // namespace At0::Ray
