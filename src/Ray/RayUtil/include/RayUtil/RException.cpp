#include "Rupch.h"
#include "RayUtil/RException.h"

#ifdef _WIN32
#include <DxErr/dxerr.h>
#endif

#include <vulkan/vulkan_core.h>


namespace At0::Ray
{
	Exception::Exception(uint16_t line, const char* file)
		: m_Line(line), m_File(file), m_WhatBuffer("")
	{
	}

	std::string Exception::GetDefaultString() const
	{
		std::ostringstream oss;
		oss << "[File] " << m_File << '\n'
			<< "[Line] " << m_Line;

		return oss.str();
	}

	const char* Exception::what() const noexcept
	{
		std::ostringstream oss;
		oss << GetType() << '\n'
			<< GetDefaultString();

		m_WhatBuffer = oss.str();
		return m_WhatBuffer.c_str();
	}


	RuntimeException::RuntimeException(const char* message, uint16_t line, const char* file)
		: Exception(line, file), m_Message(message)
	{
	}

	const char* RuntimeException::what() const noexcept
	{
		std::ostringstream oss;
		oss << GetType() << '\n'
			<< "[Description] " << m_Message << '\n'
			<< GetDefaultString();

		m_WhatBuffer = oss.str();
		return m_WhatBuffer.c_str();
	}

	VulkanException::VulkanException(const char* message, uint16_t line, const char* file, VkResult error)
		: RuntimeException(message, line, file), m_Error(error)
	{
	}

	const char* VulkanException::what() const noexcept
	{
		std::ostringstream oss;
		oss << GetType() << '\n'
			<< "[Error Code] " << VkResultToString() << "(" << (int32_t)m_Error << ")" << '\n'
			<< "[Description] " << m_Message << '\n'
			<< GetDefaultString();

		m_WhatBuffer = oss.str();
		return m_WhatBuffer.c_str();
	}

	std::string VulkanException::VkResultToString() const
	{
#define VK_ERR_TOSTR(x) case x: return #x

		switch (m_Error)
		{
			VK_ERR_TOSTR(VK_NOT_READY);
			VK_ERR_TOSTR(VK_TIMEOUT);
			VK_ERR_TOSTR(VK_EVENT_SET);
			VK_ERR_TOSTR(VK_EVENT_RESET);
			VK_ERR_TOSTR(VK_INCOMPLETE);
			VK_ERR_TOSTR(VK_ERROR_OUT_OF_HOST_MEMORY);
			VK_ERR_TOSTR(VK_ERROR_OUT_OF_DEVICE_MEMORY);
			VK_ERR_TOSTR(VK_ERROR_INITIALIZATION_FAILED);
			VK_ERR_TOSTR(VK_ERROR_DEVICE_LOST);
			VK_ERR_TOSTR(VK_ERROR_MEMORY_MAP_FAILED);
			VK_ERR_TOSTR(VK_ERROR_LAYER_NOT_PRESENT);
			VK_ERR_TOSTR(VK_ERROR_EXTENSION_NOT_PRESENT);
			VK_ERR_TOSTR(VK_ERROR_FEATURE_NOT_PRESENT);
			VK_ERR_TOSTR(VK_ERROR_INCOMPATIBLE_DRIVER);
			VK_ERR_TOSTR(VK_ERROR_TOO_MANY_OBJECTS);
			VK_ERR_TOSTR(VK_ERROR_FORMAT_NOT_SUPPORTED);
			VK_ERR_TOSTR(VK_ERROR_FRAGMENTED_POOL);
			VK_ERR_TOSTR(VK_ERROR_UNKNOWN);
			VK_ERR_TOSTR(VK_ERROR_OUT_OF_POOL_MEMORY);
			VK_ERR_TOSTR(VK_ERROR_INVALID_EXTERNAL_HANDLE);
			VK_ERR_TOSTR(VK_ERROR_FRAGMENTATION);
			VK_ERR_TOSTR(VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS);
			VK_ERR_TOSTR(VK_ERROR_SURFACE_LOST_KHR);
			VK_ERR_TOSTR(VK_ERROR_NATIVE_WINDOW_IN_USE_KHR);
			VK_ERR_TOSTR(VK_SUBOPTIMAL_KHR);
			VK_ERR_TOSTR(VK_ERROR_OUT_OF_DATE_KHR);
			VK_ERR_TOSTR(VK_ERROR_INCOMPATIBLE_DISPLAY_KHR);
			VK_ERR_TOSTR(VK_ERROR_INVALID_SHADER_NV);
			VK_ERR_TOSTR(VK_ERROR_INVALID_DRM_FORMAT_MODIFIER_PLANE_LAYOUT_EXT);
			VK_ERR_TOSTR(VK_ERROR_NOT_PERMITTED_EXT);
			VK_ERR_TOSTR(VK_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT);
			VK_ERR_TOSTR(VK_THREAD_IDLE_KHR);
			VK_ERR_TOSTR(VK_THREAD_DONE_KHR);
			VK_ERR_TOSTR(VK_OPERATION_DEFERRED_KHR);
			VK_ERR_TOSTR(VK_OPERATION_NOT_DEFERRED_KHR);
			VK_ERR_TOSTR(VK_PIPELINE_COMPILE_REQUIRED_EXT);
		}

		return "Unknown";

#undef VK_ERR_TOSTR
	}

#ifdef _WIN32

	WindowsException::WindowsException(HRESULT hr, uint16_t line, const char* file)
		: Exception(line, file), m_Hr(hr)
	{
	}

	const char* WindowsException::what() const noexcept
	{
		std::ostringstream oss;
		oss << GetType() << '\n'
			<< "[Error Code] 0x" << std::hex << std::uppercase << m_Hr << '\n'
			<< "[Description] " << GetErrorString() << '\n'
			<< GetDefaultString();

		m_WhatBuffer = oss.str();
		return m_WhatBuffer.c_str();
	}

	std::string WindowsException::GetErrorString() const
	{
		char* pMsgBuf;
		DWORD msgLen = FormatMessageA(
			FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			nullptr, m_Hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&pMsgBuf, 0, nullptr
		);

		if (msgLen == 0)
			return "Unidentified error code";

		std::string errorCode = pMsgBuf;
		LocalFree(pMsgBuf);
		return errorCode;
	}

	GraphicsException::GraphicsException(HRESULT hr, uint16_t line, const char* file)
		: Exception(line, file), m_Hr(hr)
	{
	}

	const char* GraphicsException::what() const noexcept
	{
		std::ostringstream oss;
		oss << GetType() << '\n'
			<< "[Error Code] 0x" << std::hex << std::uppercase << m_Hr << '\n'
			<< "[Error String] " << GetErrorString() << '\n'
			<< "[Description] " << GetErrorDescription() << '\n'
			<< GetDefaultString();

		m_WhatBuffer = oss.str();
		return m_WhatBuffer.c_str();
	}

	std::string GraphicsException::GetErrorString() const
	{
		return DXGetErrorStringA(m_Hr);
	}

	std::string GraphicsException::GetErrorDescription() const
	{
		char buff[512];
		DXGetErrorDescriptionA(m_Hr, buff, sizeof(buff));
		return buff;
	}

#endif
}
