#include "Rpch.h"
#include "RVulkanInstance.h"

#include "Devices/RWindow.h"

#include "Utils/RLogger.h"
#include "Utils/RAssert.h"
#include "Utils/RException.h"


namespace At0::Ray
{
	static VkResult CreateDebugUtilsMessengerEXT(VkInstance instance,
		const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
		const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger)
	{
		auto func = reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(
			vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT"));
		if (func)
			return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
		return VK_ERROR_EXTENSION_NOT_PRESENT;
	}

	static void DestroyDebugUtilsMessengerEXT(VkInstance instance,
		VkDebugUtilsMessengerEXT messenger, const VkAllocationCallbacks* pAllocator)
	{
		auto func = reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(
			vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT"));
		if (func)
			return func(instance, messenger, pAllocator);
	}


	static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageType,
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData)
	{
		switch (messageSeverity)
		{
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
			Log::Trace("[VulkanValidation] {0}", pCallbackData->pMessage);
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
		VkApplicationInfo appInfo{};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pEngineName = "Ray";
		appInfo.engineVersion = VK_MAKE_VERSION(0, 0, 1);
		appInfo.apiVersion = VK_API_VERSION_1_1;

		VkInstanceCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.flags = 0;
		createInfo.pApplicationInfo = &appInfo;

		// Get required Instance extensions from glfw
		auto instanceExtensions = GetRequiredExtensions();
		if (auto unsuportedExtensions = ExtensionsSupported(instanceExtensions);
			!unsuportedExtensions.empty())
		{
			RAY_THROW_RUNTIME("[VulkanInstance] VulkanExtension {0} not supported");
		}

		createInfo.enabledExtensionCount = (uint32_t)instanceExtensions.size();
		createInfo.ppEnabledExtensionNames = instanceExtensions.data();

		VkDebugUtilsMessengerCreateInfoEXT messengerCreateInfo = GetDebugMessengerCreateInfo();
		if (m_ValidationLayersEnabled && HasValidationLayerSupport())
		{
			createInfo.enabledLayerCount = s_ValidationLayers.size();
			createInfo.ppEnabledLayerNames = s_ValidationLayers.data();

			createInfo.pNext = &messengerCreateInfo;
		}
		else
		{
			Log::Warn("[VulkanInstance] Validation layers disabled.");
			m_ValidationLayersEnabled = false;

			createInfo.enabledLayerCount = 0;
		}

		RAY_VK_THROW_FAILED(vkCreateInstance(&createInfo, nullptr, &m_Instance),
			"[VulkanInstance] Creation failed.");

		if (m_ValidationLayersEnabled)
			CreateDebugMessenger();
	}

	VulkanInstance::~VulkanInstance()
	{
		if (m_DebugMessenger)
			DestroyDebugUtilsMessengerEXT(m_Instance, m_DebugMessenger, nullptr);

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
			"[VulkanInstance] Failed to enumerate instance extension properties.");

		std::vector<VkExtensionProperties> extProps(extPropCount);
		vkEnumerateInstanceExtensionProperties(nullptr, &extPropCount, extProps.data());
		RAY_MEXPECTS(!extProps.empty(),
			"[VulkanInstance] Failed to enumerate instance extension properties.");

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

		if (CreateDebugUtilsMessengerEXT(m_Instance, &createInfo, nullptr, &m_DebugMessenger) !=
			VK_SUCCESS)
			Log::Error("[VulkanInstance] Failed to create debug messenger");
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