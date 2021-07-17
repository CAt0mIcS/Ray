#pragma once

#include "Ray/RBase.h"
#include "Ray/Utils/RNonCopyable.h"

#include <vulkan/vulkan_core.h>
#include <RayRenderer/Core/RImage.h>


namespace At0::Ray
{
	class ImageView;

	class RAY_EXPORT Swapchain : NonCopyable
	{
	public:
		struct SupportDetails
		{
			VkSurfaceCapabilitiesKHR Capabilities;
			std::vector<VkSurfaceFormatKHR> Formats;
			std::vector<VkPresentModeKHR> PresentModes;
		};

	public:
		Swapchain(VkSwapchainKHR oldSwapchain = nullptr,
			RrImageUsageFlags imageUsage = RrImageUsageColorAttachment);
		~Swapchain();

		SupportDetails QuerySwapchainSupport() const;
		RrFormat GetFormat() const { return m_Format; }
		const RrExtent2D& GetExtent() const { return m_Extent; }
		uint32_t GetNumberOfImages() const { return m_Images.size(); }
		const std::vector<Scope<ImageView>>& GetImageViews() const { return m_ImageViews; }

		operator const VkSwapchainKHR&() const { return m_Swapchain; }

	private:
		VkSurfaceFormatKHR ChooseSurfaceFormat(
			const std::vector<VkSurfaceFormatKHR>& formats) const;
		VkPresentModeKHR ChoosePresentMode(const std::vector<VkPresentModeKHR>& presentModes) const;
		RrExtent2D ChooseExtent(const VkSurfaceCapabilitiesKHR& capabilities) const;

	private:
		VkSwapchainKHR m_Swapchain = nullptr;
		RrFormat m_Format = RRFORMAT_UNDEFINED;
		RrExtent2D m_Extent{};

		std::vector<RrImage> m_Images;
		std::vector<Scope<ImageView>> m_ImageViews;
	};
}  // namespace At0::Ray
