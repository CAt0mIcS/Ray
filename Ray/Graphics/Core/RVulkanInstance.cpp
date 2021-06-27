#include "Rpch.h"
#include "RVulkanInstance.h"

#include "Devices/RWindow.h"

#include "RayBase/RLogger.h"
#include "RayBase/RAssert.h"
#include "RayBase/RException.h"

#include "Core/RRendererLoader.h"


namespace At0::Ray
{
#ifndef NDEBUG
	std::vector<const char*> VulkanInstance::s_ValidationLayers{ "VK_LAYER_KHRONOS_validation" };
#else
	std::vector<const char*> VulkanInstance::s_ValidationLayers{};
#endif

	static bool DebugCallback(RrLogMessageSeverity severity, const char* pMessage)
	{
		switch (severity)
		{
		case RrLogMessageSeverityDebug: Log::Debug("[Renderer] {0}", pMessage); break;
		case RrLogMessageSeverityInfo: Log::Info("[Renderer] {0}", pMessage); break;
		case RrLogMessageSeverityWarning: Log::Warn("[Renderer] {0}", pMessage); break;
		case RrLogMessageSeverityError: Log::Error("[Renderer] {0}", pMessage); break;
		}

		return false;
	}

	VulkanInstance::VulkanInstance()
	{
		RrInitializeInfo initInfo{};

		// Get required Instance extensions from glfw
		auto instanceExtensions = GetRequiredExtensions();
		if (auto unsuportedExtensions = ExtensionsSupported(instanceExtensions);
			!unsuportedExtensions.empty())
		{
			ThrowRuntime("[VulkanInstance] VulkanExtension {0} not supported");
		}

		initInfo.enabledExtensionCount = (uint32_t)instanceExtensions.size();
		initInfo.ppEnabledExtensions = instanceExtensions.data();

		initInfo.enableValidationLayers = m_ValidationLayersEnabled;
		initInfo.pfnValidationCallback = DebugCallback;
		initInfo.enabledLayerCount = s_ValidationLayers.size();
		initInfo.ppEnabledLayers = s_ValidationLayers.data();

		initInfo.pfnLoader = (void*)glfwGetProcAddress;

		ThrowRenderError(RendererAPI::Initialize(&initInfo, (RrInstance*)&m_Instance,
							 (RrDebugMessenger*)&m_DebugMessenger),
			"[Instance] Creation failed");

		m_ValidationLayersEnabled = initInfo.enableValidationLayers;
	}

	VulkanInstance::~VulkanInstance()
	{
		RendererAPI::DestroyInstance(m_Instance, m_DebugMessenger);
	}

	PFN_vkVoidFunction VulkanInstance::LoadFunction(const char* name)
	{
		return vkGetInstanceProcAddr(m_Instance, name);
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
}  // namespace At0::Ray
