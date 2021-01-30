#pragma once

#include "../../RBase.h"

#include <vulkan/vulkan_core.h>
#include <vector>


namespace At0::Ray
{
	class RAY_EXPORT Swapchain
	{
	public:
		Swapchain(const VkExtent2D& extent, const Swapchain* oldSwapchain = nullptr);
		~Swapchain();

		/**
		 * Acquires the next image in the swapchain. The function will always wait until the next
		 * image is aquired
		 * @param presentCompleteSemaphore A optional semaphore that is signaled when the image is
		 * ready for use.
		 * @param fence A optional fence that is signaled once the previous command buffer has
		 * completed.
		 * @returns Result of the image acquisition.
		 */
		VkResult AcquireNextImage(const VkSemaphore& presentCompleteSemaphore = VK_NULL_HANDLE,
			VkFence fence = VK_NULL_HANDLE);

		/**
		 * Queue an image for presentation using the internally stored acquired image for
		 * presentation.
		 * @param presentQueue Presentation queue for presenting the image.
		 * @param wawitSemaphore A optional semaphore that is waited on before the image is
		 * presented.
		 * @returns The result of the queue presentation.
		 */
		VkResult QueuePresent(
			const VkQueue& presentQueue, const VkSemaphore& waitSemaphore = VK_NULL_HANDLE);

		/**
		 * @returns If the currently stored swapchain extent and the parameter one are the same
		 */
		bool IsSameExtent(const VkExtent2D& extent)
		{
			return m_Extent.width == extent.width && m_Extent.height == extent.height;
		}

		/**
		 * Conversion operator to underlying type
		 */
		operator const VkSwapchainKHR&() const { return m_Swapchain; }

		const VkExtent2D& GetExtent() const { return m_Extent; }
		uint32_t GetImageCount() const { return m_ImageCount; }
		VkSurfaceTransformFlagsKHR GetPreTransform() const { return m_PreTransform; }
		VkCompositeAlphaFlagBitsKHR GetCompositeAlpha() const { return m_CompositeAlpha; }
		const std::vector<VkImage>& GetImages() const { return m_Images; }
		const VkImage& GetActiveImage() const { return m_Images[m_ActiveImageIndex]; }
		const std::vector<VkImageView>& GetImageViews() const { return m_ImageViews; }
		const VkSwapchainKHR& GetSwapchain() const { return m_Swapchain; }
		uint32_t GetActiveImageIndex() const { return m_ActiveImageIndex; }

	private:
		VkExtent2D m_Extent;
		VkPresentModeKHR m_PresentMode;

		uint32_t m_ImageCount = 0;
		VkSurfaceTransformFlagsKHR m_PreTransform;
		VkCompositeAlphaFlagBitsKHR m_CompositeAlpha;
		std::vector<VkImage> m_Images;
		std::vector<VkImageView> m_ImageViews;
		VkSwapchainKHR m_Swapchain = VK_NULL_HANDLE;

		VkFence m_FenceImage = VK_NULL_HANDLE;
		uint32_t m_ActiveImageIndex;
	};
}  // namespace At0::Ray
