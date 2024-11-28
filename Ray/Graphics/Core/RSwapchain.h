#pragma once

#include "../../RBase.h"
#include "../../Utils/RNonCopyable.h"
#include "../../Core/RMath.h"

#include <vulkan/vulkan_core.h>

#include <vector>


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
		Swapchain(UInt2 framebufferSize, VkSwapchainKHR oldSwapchain = VK_NULL_HANDLE,
			VkImageUsageFlags imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT);
		~Swapchain();

		SupportDetails QuerySwapchainSupport() const;
		VkFormat GetFormat() const { return m_Format; }
		const UInt2& GetExtent() const { return m_Extent; }
		uint32_t GetNumberOfImages() const { return m_Images.size(); }
		const std::vector<Scope<ImageView>>& GetImageViews() const { return m_ImageViews; }

		float GetAspectRatio() const;

		operator const VkSwapchainKHR&() const { return m_Swapchain; }

	private:
		VkSurfaceFormatKHR ChooseSurfaceFormat(
			const std::vector<VkSurfaceFormatKHR>& formats) const;
		VkPresentModeKHR ChoosePresentMode(const std::vector<VkPresentModeKHR>& presentModes) const;
		UInt2 ChooseExtent(const VkSurfaceCapabilitiesKHR& capabilities, UInt2 extent) const;

	private:
		VkSwapchainKHR m_Swapchain = VK_NULL_HANDLE;
		VkFormat m_Format = VK_FORMAT_UNDEFINED;
		UInt2 m_Extent{};

		std::vector<VkImage> m_Images;
		std::vector<Scope<ImageView>> m_ImageViews;
	};
}  // namespace At0::Ray
