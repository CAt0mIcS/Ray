#pragma once

#include "../RBase.h"
#include "../Utils/RString.h"
#include "../Utils/RLogger.h"

#include <exception>
#include <string>
#include <stdint.h>

#include <vulkan/vulkan_core.h>


namespace At0::Ray
{
	class RAY_EXPORT Exception : public std::exception
	{
	public:
		Exception(uint16_t line, const char* file);
		std::string GetDefaultString() const;
		virtual const char* GetType() const { return "Exception"; };
		virtual const char* what() const noexcept override;
		virtual ~Exception() = default;

	protected:
		mutable std::string m_WhatBuffer;
		unsigned int m_Line;
		const char* m_File;
	};

	class RAY_EXPORT RuntimeException : public Exception
	{
	public:
		RuntimeException(const char* message, uint16_t line, const char* file);
		virtual const char* GetType() const { return "Runtime Exception"; };
		virtual const char* what() const noexcept override;

	protected:
		std::string m_Message;
	};

	class RAY_EXPORT VulkanException : public RuntimeException
	{
	public:
		VulkanException(const char* message, uint16_t line, const char* file, VkResult result);
		virtual const char* GetType() const { return "Vulkan Exception"; };
		virtual const char* what() const noexcept override;

	private:
		VkResult m_Error;
	};


	template<typename... Args>
	struct ThrowVulkanError
	{
		ThrowVulkanError(VkResult result, std::string msg, Args&&... args,
			SourceLocation location = SourceLocation::current())
		{
			if (result != VK_SUCCESS)
			{
				VulkanException exception(
					String::Serialize(msg, std::forward<Args>(args)...).c_str(),
					(uint16_t)location.line(), location.file_name(), result);

				Log::Critical(exception.what());
				throw exception;
			}
		}

		ThrowVulkanError(
			std::string msg, Args&&... args, SourceLocation location = SourceLocation::current())
		{
			VulkanException exception(String::Serialize(msg, std::forward<Args>(args)...).c_str(),
				(uint16_t)location.line(), location.file_name(), VK_ERROR_UNKNOWN);

			Log::Critical(exception.what());
			throw exception;
		}
	};

	template<typename... Args>
	struct ThrowRuntime
	{
		ThrowRuntime(
			std::string msg, Args&&... args, SourceLocation location = SourceLocation::current())
		{
			RuntimeException exception(String::Serialize(msg, std::forward<Args>(args)...).c_str(),
				(uint16_t)location.line(), location.file_name());
			Log::Critical(exception.what());
			throw exception;
		}
	};

	template<typename... Args>
	ThrowVulkanError(VkResult, std::string, Args&&...) -> ThrowVulkanError<Args...>;

	template<typename... Args>
	ThrowVulkanError(std::string, Args&&...) -> ThrowVulkanError<Args...>;

	template<typename... Args>
	ThrowRuntime(std::string, Args&&...) -> ThrowRuntime<Args...>;
}  // namespace At0::Ray
