#include "Rpch.h"
#include "RVulkanInstance.h"

#include "Devices/RWindow.h"

#include "RayBase/RLogger.h"
#include "RayBase/RAssert.h"
#include "RayBase/RException.h"

#include <RayRenderer/Core/RCore.h>


namespace At0::Ray
{
#ifndef NDEBUG
	std::vector<const char*> VulkanInstance::s_ValidationLayers{ "VK_LAYER_KHRONOS_validation" };
#else
	std::vector<const char*> VulkanInstance::s_ValidationLayers{};
#endif

	static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageType,
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData)
	{
		switch (messageSeverity)
		{
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
			Log::Debug("[VulkanValidation] {0}", pCallbackData->pMessage);
			break;
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
			Log::Info("[VulkanValidation] {0}", pCallbackData->pMessage);
			break;
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
			Log::Warn("[VulkanValidation] {0}", pCallbackData->pMessage);
			break;
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
			Log::Error("[VulkanValidation] {0}", pCallbackData->pMessage);
			break;
		}

		return VK_FALSE;
	}

	VulkanInstance::VulkanInstance()
	{
		RrInitializeInfo initInfo{};

		// VkApplicationInfo appInfo{};
		// appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		// appInfo.pEngineName = "Ray";
		// appInfo.engineVersion = VK_MAKE_VERSION(0, 0, 1);
		// appInfo.apiVersion = VK_API_VERSION_1_1;

		// VkInstanceCreateInfo createInfo{};
		// createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		// createInfo.flags = 0;
		// createInfo.pApplicationInfo = &appInfo;

		// Get required Instance extensions from glfw
		auto instanceExtensions = GetRequiredExtensions();
		if (auto unsuportedExtensions = ExtensionsSupported(instanceExtensions);
			!unsuportedExtensions.empty())
		{
			ThrowRuntime("[VulkanInstance] VulkanExtension {0} not supported");
		}

		initInfo.enabledExtensionCount = (uint32_t)instanceExtensions.size();
		initInfo.ppEnabledExtensions = instanceExtensions.data();

		VkDebugUtilsMessengerCreateInfoEXT messengerCreateInfo = GetDebugMessengerCreateInfo();
		if (m_ValidationLayersEnabled && HasValidationLayerSupport())
		{
			initInfo.enabledLayerCount = s_ValidationLayers.size();
			initInfo.ppEnabledLayers = s_ValidationLayers.data();

			initInfo.pNext = &messengerCreateInfo;
		}
		else
		{
			Log::Info("[VulkanInstance] Validation layers disabled");
			m_ValidationLayersEnabled = false;

			initInfo.enabledLayerCount = 0;
		}

		ThrowRenderError(
			RrInitialize(&initInfo, (RrInstance*)&m_Instance), "[Instance] Creation failed");

		if (m_ValidationLayersEnabled)
			CreateDebugMessenger();
	}

	VulkanInstance::~VulkanInstance()
	{
		if (m_DebugMessenger)
			if (auto destroyDebugMessenger = (PFN_vkDestroyDebugUtilsMessengerEXT)LoadFunction(
					"vkDestroyDebugUtilsMessengerEXT"))
				destroyDebugMessenger(m_Instance, m_DebugMessenger, nullptr);
			else
				Log::Error("[VulkanInstance] Unable to find vkDestroyDebugUtilsMessengerEXT");

		vkDestroyInstance(m_Instance, nullptr);
	}

	PFN_vkVoidFunction VulkanInstance::LoadFunction(const char* name)
	{
		return vkGetInstanceProcAddr(m_Instance, name);
	}

	bool VulkanInstance::HasValidationLayerSupport() const
	{
		uint32_t layerPropCount = 0;
		vkEnumerateInstanceLayerProperties(&layerPropCount, nullptr);
		if (layerPropCount == 0)
			return false;

		std::vector<VkLayerProperties> layerProps(layerPropCount);
		vkEnumerateInstanceLayerProperties(&layerPropCount, layerProps.data());

		for (const char* layerName : s_ValidationLayers)
		{
			bool layerFound = false;

			for (const VkLayerProperties& layerProps : layerProps)
			{
				if (strcmp(layerName, layerProps.layerName) == 0)
				{
					layerFound = true;
					break;
				}
			}

			if (!layerFound)
				return false;
		}

		return true;
	}

	std::vector<const char*> VulkanInstance::GetRequiredExtensions() const
	{
		auto glfwExtensions = Window::GetInstanceExtensions();

		std::vector<const char*> extensions(glfwExtensions.second);

		// Add all the glfwExtensions to the return extensions array
		for (uint32_t i = 0; i < glfwExtensions.second; ++i)
			extensions[i] = glfwExtensions.first[i];

		if (m_ValidationLayersEnabled)
			extensions.emplace_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

		return extensions;
	}

	std::vector<const char*> VulkanInstance::ExtensionsSupported(
		const std::vector<const char*>& instanceExtensions)
	{
		std::vector<const char*> unsupportedExtensions;

		uint32_t extPropCount = 0;
		vkEnumerateInstanceExtensionProperties(nullptr, &extPropCount, nullptr);
		RAY_MEXPECTS(extPropCount != 0,
			"[VulkanInstance] Failed to enumerate instance extension properties");

		std::vector<VkExtensionProperties> extProps(extPropCount);
		vkEnumerateInstanceExtensionProperties(nullptr, &extPropCount, extProps.data());
		RAY_MEXPECTS(!extProps.empty(),
			"[VulkanInstance] Failed to enumerate instance extension properties");

		for (const char* extension : instanceExtensions)
		{
			bool extensionFound = false;

			for (const VkExtensionProperties& prop : extProps)
			{
				if (strcmp(extension, prop.extensionName) == 0)
				{
					extensionFound = true;
					break;
				}
			}

			if (!extensionFound)
				unsupportedExtensions.emplace_back(extension);
		}

		return unsupportedExtensions;
	}

	void VulkanInstance::CreateDebugMessenger()
	{
		VkDebugUtilsMessengerCreateInfoEXT createInfo = GetDebugMessengerCreateInfo();

		if (auto createDebugMessenger =
				(PFN_vkCreateDebugUtilsMessengerEXT)LoadFunction("vkCreateDebugUtilsMessengerEXT"))
		{
			if (createDebugMessenger(m_Instance, &createInfo, nullptr, &m_DebugMessenger) !=
				VK_SUCCESS)
				Log::Error("[VulkanInstance] Failed to create debug messenger");
		}
		else
			Log::Error("[VulkanInstance] Unable to find vkCreateDebugUtilsMessengerEXT");
	}

	VkDebugUtilsMessengerCreateInfoEXT VulkanInstance::GetDebugMessengerCreateInfo() const
	{
		VkDebugUtilsMessengerCreateInfoEXT createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
		createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
									 VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT |
									 VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
									 VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
		createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
								 VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
								 VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
		createInfo.pfnUserCallback = &DebugCallback;

		return createInfo;
	}
}  // namespace At0::Ray
