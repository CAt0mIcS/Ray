﻿#include "Rpch.h"
#include "RRendererInstance.h"

#include "Devices/RWindow.h"

#include "Ray/Utils/RLogger.h"
#include "Ray/Utils/RAssert.h"
#include "Ray/Utils/RException.h"

#include "Core/RRendererLoader.h"

#include <GLFW/glfw3.h>

namespace At0::Ray
{
#ifndef NDEBUG
	std::vector<const char*> RendererInstance::s_ValidationLayers{ "VK_LAYER_KHRONOS_validation" };
#else
	std::vector<const char*> RendererInstance::s_ValidationLayers{};
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

	RendererInstance::RendererInstance()
	{
		RrInitializeInfo initInfo{};

		// RAY_TODO: Check latest available version
		initInfo.apiVersion = RR_API_VERSION_1_2;

		// Get required Instance extensions from glfw
		auto instanceExtensions = GetRequiredExtensions();
		if (auto unsuportedExtensions = ExtensionsSupported(instanceExtensions);
			!unsuportedExtensions.empty())
		{
			ThrowRuntime("[RendererInstance] VulkanExtension {0} not supported");
		}

		std::vector<const char*> charInstanceExtensions(instanceExtensions.size());
		for (uint32_t i = 0; i < charInstanceExtensions.size(); ++i)
			charInstanceExtensions[i] = instanceExtensions[i].c_str();

		initInfo.enabledExtensionCount = (uint32_t)charInstanceExtensions.size();
		initInfo.ppEnabledExtensions = charInstanceExtensions.data();

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

	RendererInstance::~RendererInstance()
	{
		RendererAPI::DestroyInstance(m_Instance, m_DebugMessenger);
	}

	RrPFNVoidFunction RendererInstance::LoadFunction(const char* name) const
	{
		return RendererAPI::GetInstanceProcAddr(m_Instance, name);
	}

	std::vector<std::string> RendererInstance::GetRequiredExtensions() const
	{
		auto extensions = Window::GetInstanceExtensions();
		if (m_ValidationLayersEnabled)
			extensions.emplace_back("VK_EXT_debug_utils");

		return extensions;
	}

	std::vector<std::string> RendererInstance::ExtensionsSupported(
		const std::vector<std::string>& instanceExtensions)
	{
		std::vector<std::string> unsupportedExtensions;

		uint32_t extPropCount = 0;
		RendererAPI::EnumerateInstanceExtensionProperties(nullptr, &extPropCount, nullptr);
		RAY_MEXPECTS(extPropCount != 0,
			"[RendererInstance] Failed to enumerate instance extension properties");

		std::vector<RrExtensionProperties> extProps(extPropCount);
		RendererAPI::EnumerateInstanceExtensionProperties(nullptr, &extPropCount, extProps.data());
		RAY_MEXPECTS(!extProps.empty(),
			"[RendererInstance] Failed to enumerate instance extension properties");

		for (std::string_view extension : instanceExtensions)
		{
			bool extensionFound = false;

			for (const RrExtensionProperties& prop : extProps)
			{
				if (strcmp(extension.data(), prop.extensionName) == 0)
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
