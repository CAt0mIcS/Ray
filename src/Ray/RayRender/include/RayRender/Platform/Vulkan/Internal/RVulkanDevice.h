#pragma once

#include "RVulkanDevice.h"

#include <string>
#include <vector>

#include <vulkan/vulkan_core.h>

struct GLFWwindow;

namespace At0::Ray
{
	struct QueueFamilyIndices
	{
		uint32_t GraphicsFamily = (uint32_t)-1;
		uint32_t PresentFamily = (uint32_t)-1;

		bool IsComplete() const
		{
			return GraphicsFamily != (uint32_t)-1 && PresentFamily != (uint32_t)-1;
		}
	};

	struct SwapChainSupportDetails
	{
		VkSurfaceCapabilitiesKHR Capabilities;
		std::vector<VkSurfaceFormatKHR> Formats;
		std::vector<VkPresentModeKHR> PresentModes;
	};


	class RR_API VulkanDevice
	{
	public:
		VulkanDevice(GLFWwindow* window);

	private:
		void CreateInstance();
		std::vector<const char*> GetRequiredExtensions() const;
		void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
		void SetupDebugMessenger();
		void CreateSurface();
		void PickPhysicalDevice();
		bool IsDeviceSuitable(VkPhysicalDevice device);
		QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device);
		bool CheckDeviceExtensionSupport(VkPhysicalDevice device);
		SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device);
		void CreateLogicalDevice();
		void CreateSwapChain();
		VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
		VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
		VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
		void CreateImageViews();
		void CreateRenderPass();
		void CreateGraphicsPipeline();

	private:
#ifndef NDEBUG
		static constexpr bool s_EnableValidationLayers = true;
#else
		static constexpr bool s_EnableValidationLayers = false;
#endif
		inline static const std::vector<const char*> s_ValidationLayers = {
			"VK_LAYER_KHRONOS_validation"
		};
		inline static const std::vector<const char*> s_DeviceExtensions = {
			"VK_KHR_swapchain"
		};

		GLFWwindow* m_hWnd;

		VkInstance m_VulkanInstance;
		VkDebugUtilsMessengerEXT m_DebugMessenger;
		VkSurfaceKHR m_Surface;

		VkPhysicalDevice m_PhysicalDevice;
		VkDevice m_LogicalDevice;

		VkSwapchainKHR m_SwapChain;
		std::vector<VkImage> m_SwapChainImages;
		VkFormat m_SwapChainImageFormat;
		VkExtent2D m_SwapChainExtent;
		std::vector<VkImageView> m_SwapChainImageViews;

		VkRenderPass m_RenderPass;
	};
}
