#pragma once

#include "CoreConfig.h"

#if RAY_ENABLE_INFO || RAY_ENABLE_WARNING
#include "RlUBase.h"

#include "Serialize.h"
#include "MessageBox.h"

#include <string>
#include <stdint.h>


namespace At0::Util
{
	class RU_API InfoManager
	{
	public:
		template<typename... Args>
		static void Info(const std::string_view baseStr, uint16_t line, const std::string_view file, const std::string_view type, Args&&... args)
		{
			std::ostringstream oss;
			oss << type << '\n'
				<< "[Message] " << SerializeString(baseStr.data(), std::forward<Args>(args)...) << '\n'
				<< GetDefaultString(line, file);

			Ray::ZMessageBox(type, oss.str(), Ray::ZMessageBox::Button::Ok, Ray::ZMessageBox::Icon::Info, Ray::ZMessageBox::Flags::DefaultDesktop);
		}

		template<typename... Args>
		static void Warn(const std::string_view baseStr, uint16_t line, const std::string_view file, const std::string_view type, Args&&... args)
		{
			std::ostringstream oss;
			oss << type << '\n'
				<< "[Message] " << SerializeString(baseStr.data(), std::forward<Args>(args)...) << '\n'
				<< GetDefaultString(line, file);

			Ray::ZMessageBox(type, oss.str(), Ray::ZMessageBox::Button::Ok, Ray::ZMessageBox::Icon::Warn, Ray::ZMessageBox::Flags::DefaultDesktop);
		}

	private:
		static std::string GetDefaultString(uint16_t line, const std::string_view file);

#ifdef _WIN32
	public:
		static void DxWarn(HRESULT hr, uint16_t line, const std::string_view file, const std::string_view type);
		static void DxInfo(HRESULT hr, uint16_t line, const std::string_view file, const std::string_view type);

	private:
		static std::string GetDxErrorString();
		static std::string GetDxErrorDescription();
		static std::string GetDxString(const std::string_view type, uint16_t line, std::string_view file);

	private:
		static HRESULT m_Hr;
#endif // _WIN32
	};


}


#if RAY_ENABLE_INFO
	#define RAY_DSPL_INFO(msg, type, ...)	::At0::Util::InfoManager::Info(msg, (uint16_t)__LINE__, __FILE__, type, __VA_ARGS__)
#else
	#define RAY_DSPL_INFO(msg, type, ...)
#endif
#if RAY_ENABLE_WARNING
	#define RAY_DSPL_WARN(msg, type, ...)	::At0::Util::InfoManager::Warn(msg, (uint16_t)__LINE__, __FILE__, type, __VA_ARGS__)
#else
	#define RAY_DSPL_WARN(msg, type, ...)
#endif

#ifdef _WIN32

#if RAY_ENABLE_INFO
	#define RAY_DX_INFO(hr)				if(FAILED(hr)) ::At0::Util::InfoManager::DxInfo(hr, (uint16_t)__LINE__, __FILE__, "DirectX11 Info")
	#define RAY_DX_MSG_INFO(msg, ...)	RAY_DSPL_INFO(msg, "DirectX11 Info", __VA_ARGS__)
#else
	#define RAY_DX_INFO(hr) hr
	#define RAY_DX_MSG_INFO(msg, ...)
#endif	

#if RAY_ENABLE_WARNING
	#define RAY_DX_WARN(hr)				if(FAILED(hr)) ::At0::Util::InfoManager::DxWarn(hr, (uint16_t)__LINE__, __FILE__, "DirectX11 Warning")
	#define RAY_DX_MSG_WARN(msg, ...)	RAY_DSPL_WARN(msg, "DirectX11 Warning", __VA_ARGS__)
#else
	#define RAY_DX_WARN(hr) hr
	#define RAY_DX_MSG_WARN(msg, ...)
#endif
#else
	#define RAY_DX_INFO(hr) hr
	#define RAY_DX_WARN(hr) hr
#endif

#endif // RAY_ENABLE_INFO && RAY_ENABLE_WARNING