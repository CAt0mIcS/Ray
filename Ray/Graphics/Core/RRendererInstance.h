#pragma once

#include "Ray/RBase.h"
#include "Ray/Utils/RNonCopyable.h"

#include <RayRenderer/Core/RInstance.h>
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
		RrPFNVoidFunction LoadFunction(const char* name) const;

		static const std::vector<const char*>& GetValidationLayers() { return s_ValidationLayers; }
		operator RrInstance() const { return m_Instance; }

		bool ValidationLayersEnabled() const { return m_ValidationLayersEnabled; }

	private:
		/**
		 * Queries the required extensions from glfw and adds the debug utils extension if
		 * validation layers are enabled
		 */
		std::vector<std::string> GetRequiredExtensions() const;

		/**
		 * @returns All the unsuported extensions
		 */
		bool ExtensionsSupported(const std::vector<const char*>& instanceExtensions);

	private:
		RrInstance m_Instance = nullptr;
		RrDebugMessenger m_DebugMessenger = nullptr;

		static std::vector<const char*> s_ValidationLayers;

#ifndef NDEBUG
		bool m_ValidationLayersEnabled = true;
#else
		bool m_ValidationLayersEnabled = false;
#endif
	};
}  // namespace At0::Ray
