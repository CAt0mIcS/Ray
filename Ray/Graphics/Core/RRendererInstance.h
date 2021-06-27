#pragma once

#include "Ray/RBase.h"
#include "RayBase/RNonCopyable.h"

#include <vulkan/vulkan_core.h>
#include <vector>


namespace At0::Ray
{
	class RAY_EXPORT RendererInstance : NonCopyable
	{
	public:
		RendererInstance();
		~RendererInstance();

		/**
		 * Loads a extension function with name and returns it
		 */
		PFN_vkVoidFunction LoadFunction(const char* name);

		static const std::vector<const char*>& GetValidationLayers() { return s_ValidationLayers; }
		operator const VkInstance&() const { return m_Instance; }

		bool ValidationLayersEnabled() const { return m_ValidationLayersEnabled; }

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

#ifndef NDEBUG
		bool m_ValidationLayersEnabled = true;
#else
		bool m_ValidationLayersEnabled = false;
#endif
	};
}  // namespace At0::Ray
