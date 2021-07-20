#include "Rpch.h"
#include "RSwapchain.h"

#include "Devices/RWindow.h"

#include "Ray/Core/RRendererLoader.h"
#include "Ray/Graphics/Images/RImageView.h"
#include "Ray/Graphics/RGraphics.h"
#include "RPhysicalDevice.h"
#include "RLogicalDevice.h"
#include "RSurface.h"

#include "Ray/Utils/RException.h"
#include "Ray/Utils/RLogger.h"


namespace At0::Ray
{
	Swapchain::Swapchain(RrSwapchainKHR oldSwapchain, RrImageUsageFlags imageUsage)
	{
		SupportDetails supportDetails = QuerySwapchainSupport();
		RrSurfaceFormatKHR surfaceFormat = ChooseSurfaceFormat(supportDetails.Formats);
		m_Format = surfaceFormat.format;
		RrPresentModeKHR presentMode = ChoosePresentMode(supportDetails.PresentModes);
		m_Extent = ChooseExtent(supportDetails.Capabilities);

		uint32_t imageCount = supportDetails.Capabilities.minImageCount + 1;
		// Make sure we don't exceed the max image count. 0 means that there is no maximum limit
		if (supportDetails.Capabilities.maxImageCount > 0 &&
			imageCount > supportDetails.Capabilities.maxImageCount)
		{
			imageCount = supportDetails.Capabilities.maxImageCount;
		}

		Log::Info("[Swapchain] Creating {0} images", imageCount);

		RrSwapchainCreateInfoKHR createInfo{};
		createInfo.surface = Graphics::Get().GetSurface();
		createInfo.minImageCount = imageCount;
		createInfo.imageFormat = surfaceFormat.format;
		createInfo.imageColorSpace = surfaceFormat.colorSpace;
		createInfo.imageExtent = m_Extent;
		createInfo.imageArrayLayers = 1;
		createInfo.imageUsage = imageUsage;

		// Queue families
		uint32_t queueFamilyIndices[] = { Graphics::Get().GetDevice().GetGraphicsFamily(),
			Graphics::Get().GetDevice().GetPresentFamily() };

		// If graphics queue and presentation queue are not the same
		// we need to share images between them.
		if (Graphics::Get().GetDevice().GetGraphicsFamily() !=
			Graphics::Get().GetDevice().GetPresentFamily())
		{
			createInfo.imageSharingMode = RrSharingModeConcurrent;
			createInfo.queueFamilyIndexCount = 2;
			createInfo.pQueueFamilyIndices = queueFamilyIndices;
			Log::Warn(
				"[Swapchain] Enabling image sharing between the graphics and present families");
		}
		else
		{
			createInfo.imageSharingMode = RrSharingModeExclusive;
			createInfo.queueFamilyIndexCount = 0;
			createInfo.pQueueFamilyIndices = nullptr;
		}

		createInfo.preTransform = supportDetails.Capabilities.currentTransform;
		createInfo.compositeAlpha = RrCompositeAlphaOpaqueKHR;
		createInfo.presentMode = presentMode;
		createInfo.clipped = true;
		createInfo.oldSwapchain = oldSwapchain;

		ThrowRenderError(
			RendererAPI::CreateSwapchainKHR(Graphics::Get().GetDevice(), &createInfo, &m_Swapchain),
			"[Swapchain] Failed to create");

		// Retrieve swapchain images
		uint32_t swapchainImageCount = 0;
		RendererAPI::GetSwapchainImagesKHR(
			Graphics::Get().GetDevice(), m_Swapchain, &swapchainImageCount, nullptr);
		if (swapchainImageCount == 0)
			ThrowRuntime("[Swapchain] Image count is 0");

		m_Images.resize(swapchainImageCount);
		RendererAPI::GetSwapchainImagesKHR(
			Graphics::Get().GetDevice(), m_Swapchain, &swapchainImageCount, m_Images.data());

		// Create image views
		m_ImageViews.resize(m_Images.size());
		for (uint32_t i = 0; i < m_ImageViews.size(); ++i)
		{
			m_ImageViews[i] =
				MakeScope<ImageView>(m_Images[i], RrImageViewType2D, m_Format, RrImageAspectColor);
		}
	}

	Swapchain::~Swapchain()
	{
		RendererAPI::DestroySwapchainKHR(Graphics::Get().GetDevice(), m_Swapchain);
	}

	Swapchain::SupportDetails Swapchain::QuerySwapchainSupport() const
	{
		RrPhysicalDevice physicalDevice = Graphics::Get().GetPhysicalDevice();

		SupportDetails supportDetails;

		RendererAPI::GetPhysicalDeviceSurfaceCapabilitiesKHR(
			physicalDevice, Graphics::Get().GetSurface(), &supportDetails.Capabilities);

		uint32_t surfaceFormats;
		RendererAPI::GetPhysicalDeviceSurfaceFormatsKHR(
			physicalDevice, Graphics::Get().GetSurface(), &surfaceFormats, nullptr);

		supportDetails.Formats.resize(surfaceFormats);
		RendererAPI::GetPhysicalDeviceSurfaceFormatsKHR(physicalDevice,
			Graphics::Get().GetSurface(), &surfaceFormats, supportDetails.Formats.data());

		uint32_t presentModeCount;
		RendererAPI::GetPhysicalDeviceSurfacePresentModesKHR(
			physicalDevice, Graphics::Get().GetSurface(), &presentModeCount, nullptr);

		supportDetails.PresentModes.resize(presentModeCount);
		RendererAPI::GetPhysicalDeviceSurfacePresentModesKHR(physicalDevice,
			Graphics::Get().GetSurface(), &presentModeCount, supportDetails.PresentModes.data());

		return supportDetails;
	}

	RrSurfaceFormatKHR Swapchain::ChooseSurfaceFormat(
		const std::vector<RrSurfaceFormatKHR>& formats) const
	{
		for (const auto& format : formats)
		{
			if (format.format == RRFORMAT_B8G8R8A8_SRGB &&
				format.colorSpace == RrColorSpaceSRGBNonlinearKHR)
				return format;
		}

		return formats.front();
	}

	RrPresentModeKHR Swapchain::ChoosePresentMode(
		const std::vector<RrPresentModeKHR>& presentModes) const
	{
		for (const RrPresentModeKHR& presentMode : presentModes)
		{
			if (presentMode == RrPresentModeMailboxKHR)
			{
				Log::Info("[Swapchain] Choosing mailbox present mode");
				return presentMode;
			}
		}

		Log::Info("[Swapchain] Choosing FIFO present mode");
		return RrPresentModeFifoKHR;
	}

	RrExtent2D Swapchain::ChooseExtent(const RrSurfaceCapabilitiesKHR& capabilities) const
	{
		if (capabilities.currentExtent.width != UINT32_MAX)
			return capabilities.currentExtent;
		else
		{
			UInt2 size = Window::Get().GetFramebufferSize();
			RrExtent2D extent = { size.x, size.y };

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
