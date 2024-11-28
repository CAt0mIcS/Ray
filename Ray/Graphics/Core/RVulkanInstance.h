#pragma once

#include "../../RBase.h"
#include "../../Utils/RNonCopyable.h"

#include <vulkan/vulkan_core.h>
#include <vector>


namespace At0::Ray
{
	class RAY_EXPORT VulkanInstance : NonCopyable
	{
	public:
		VulkanInstance();
		~VulkanInstance();

		/**
		 * Loads a extension function with name and returns it
		 */
		PFN_vkVoidFunction LoadFunction(const char* name);

		static const std::vector<const char*>& GetValidationLayers() { return s_ValidationLayers; }
		operator VkInstance() const { return m_Instance; }

		static constexpr bool ValidationLayersEnabled() { return !s_ValidationLayers.empty(); }

	private:
		bool HasValidationLayerSupport() const;

		/**
		 * Queries the required extensions from glfw and adds the debug utils extension if
		 * validation layers are enabled
		 */
		std::vector<const char*> GetRequiredExtensions() const;

		/**
		 * @returns All the unsuported extensions
		 */
		std::vector<const char*> ExtensionsSupported(
			const std::vector<const char*>& instanceExtensions);

		void CreateDebugMessenger();
		VkDebugUtilsMessengerCreateInfoEXT GetDebugMessengerCreateInfo() const;

	private:
		VkInstance m_Instance = VK_NULL_HANDLE;
		VkDebugUtilsMessengerEXT m_DebugMessenger = VK_NULL_HANDLE;

		static std::vector<const char*> s_ValidationLayers;
	};
}  // namespace At0::Ray
