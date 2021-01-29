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
		/**
		 * Sets up the connection to vulkan
		 */
		VulkanInstance();
		~VulkanInstance();

		/**
		 * Conversion operator to underlying type
		 */
		operator const VkInstance&() const { return m_Instance; }

		/**
		 * @returns If the validation layers are enabled
		 */
		bool ValidationLayersEnabled() const { return m_ValidationLayerEnabled; }

		/**
		 * @returns The currently used validation layers
		 */
		static const std::vector<const char*>& ValidationLayers() { return s_ValidationLayers; }

	private:
		void CreateInstance();
		void CreateDebugMessenger();
		bool CheckValidationLayerSupport() const;
		std::vector<const char*> GetExtensions() const;

	private:
		VkInstance m_Instance;
		VkDebugUtilsMessengerEXT m_DebugMessenger;

#ifndef NDEBUG
		bool m_ValidationLayerEnabled = true;
		inline static const std::vector<const char*> s_ValidationLayers = {
			"VK_LAYER_KHRONOS_validation"
		};
#else
		bool m_ValidationLayerEnabled = false;
#endif
	};
}  // namespace At0::Ray
