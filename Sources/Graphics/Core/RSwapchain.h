#pragma once

#include "../../RBase.h"
#include "../../Utils/RNonCopyable.h"

#include <vulkan/vulkan_core.h>


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
		Swapchain(VkSwapchainKHR oldSwapchain = VK_NULL_HANDLE);
		~Swapchain();

		SupportDetails QuerySwapchainSupport() const;
		VkFormat GetFormat() const { return m_Format; }
		const VkExtent2D& GetExtent() const { return m_Extent; }
		uint32_t GetNumberOfImages() const { return m_Images.size(); }
		const std::vector<Scope<ImageView>>& GetImageViews() const { return m_ImageViews; }

		operator const VkSwapchainKHR&() const { return m_Swapchain; }

	private:
		VkSurfaceFormatKHR ChooseSurfaceFormat(
			const std::vector<VkSurfaceFormatKHR>& formats) const;
		VkPresentModeKHR ChoosePresentMode(const std::vector<VkPresentModeKHR>& presentModes) const;
		VkExtent2D ChooseExtent(const VkSurfaceCapabilitiesKHR& capabilities) const;

	private:
		VkSwapchainKHR m_Swapchain;
		VkFormat m_Format;
		VkExtent2D m_Extent;

		std::vector<VkImage> m_Images;
		std::vector<Scope<ImageView>> m_ImageViews;
	};
}  // namespace At0::Ray