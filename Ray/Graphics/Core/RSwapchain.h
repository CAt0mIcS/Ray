#pragma once

#include "Ray/RBase.h"
#include "Ray/Utils/RNonCopyable.h"

#include <vulkan/vulkan_core.h>
#include <RayRenderer/Core/RSwapchain.h>


namespace At0::Ray
{
	class ImageView;

	class RAY_EXPORT Swapchain : NonCopyable
	{
	public:
		struct SupportDetails
		{
			RrSurfaceCapabilitiesKHR Capabilities;
			std::vector<RrSurfaceFormatKHR> Formats;
			std::vector<RrPresentModeKHR> PresentModes;
		};

	public:
		Swapchain(RrSwapchainKHR oldSwapchain = nullptr,
			RrImageUsageFlags imageUsage = RrImageUsageColorAttachment);
		~Swapchain();

		SupportDetails QuerySwapchainSupport() const;
		RrFormat GetFormat() const { return m_Format; }
		const RrExtent2D& GetExtent() const { return m_Extent; }
		uint32_t GetNumberOfImages() const { return m_Images.size(); }
		const std::vector<Scope<ImageView>>& GetImageViews() const { return m_ImageViews; }

		operator RrSwapchainKHR() const { return m_Swapchain; }
		operator VkSwapchainKHR() const { return (VkSwapchainKHR)m_Swapchain; }

	private:
		RrSurfaceFormatKHR ChooseSurfaceFormat(
			const std::vector<RrSurfaceFormatKHR>& formats) const;
		RrPresentModeKHR ChoosePresentMode(const std::vector<RrPresentModeKHR>& presentModes) const;
		RrExtent2D ChooseExtent(const RrSurfaceCapabilitiesKHR& capabilities) const;

	private:
		RrSwapchainKHR m_Swapchain = nullptr;
		RrFormat m_Format = RRFORMAT_UNDEFINED;
		RrExtent2D m_Extent{};

		std::vector<RrImage> m_Images;
		std::vector<Scope<ImageView>> m_ImageViews;
	};
}  // namespace At0::Ray
