#pragma once

#include "RBase.h"
#include "RString.h"
#include "RLogger.h"
#include "RAssert.h"

#include <exception>
#include <string>
#include <stdint.h>
#include <optional>


namespace At0::Ray
{
	class RAYBASE_API Exception : public std::exception
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

	class RAYBASE_API RuntimeException : public Exception
	{
	public:
		RuntimeException(const char* message, uint16_t line, const char* file);
		virtual const char* GetType() const { return "Runtime Exception"; };
		virtual const char* what() const noexcept override;

	protected:
		std::string m_Message;
	};

	class RAYBASE_API RenderException : public RuntimeException
	{
	public:
		RenderException(const char* message, uint16_t line, const char* file, int result);
		RenderException(const char* message, uint16_t line, const char* file, VkResult result);
		virtual const char* GetType() const { return "Vulkan Exception"; };
		virtual const char* what() const noexcept override;

	private:
		std::optional<int> m_Error;
		std::optional<VkResult> m_Error2;
	};


	template<typename... Args>
	struct ThrowRenderError
	{
		ThrowRenderError(VkResult result, std::string msg, Args&&... args,
			SourceLocation location = SourceLocation::current())
		{
			if (result != VK_SUCCESS)
			{
				RenderException exception(
					String::Serialize(msg, std::forward<Args>(args)...).c_str(),
					(uint16_t)location.line(), location.file_name(), result);
#ifdef NDEBUG
				Log::Critical(exception.what());
#else
				RAY_ASSERT(false, exception.what());
#endif
				throw exception;
			}
		}

		ThrowRenderError(int result, std::string msg, Args&&... args,
			SourceLocation location = SourceLocation::current())
		{
			if (result != VK_SUCCESS)
			{
				RenderException exception(
					String::Serialize(msg, std::forward<Args>(args)...).c_str(),
					(uint16_t)location.line(), location.file_name(), result);
#ifdef NDEBUG
				Log::Critical(exception.what());
#else
				RAY_ASSERT(false, exception.what());
#endif
				throw exception;
			}
		}

		ThrowRenderError(
			std::string msg, Args&&... args, SourceLocation location = SourceLocation::current())
		{
			RenderException exception(String::Serialize(msg, std::forward<Args>(args)...).c_str(),
				(uint16_t)location.line(), location.file_name(), VK_ERROR_UNKNOWN);
#ifdef NDEBUG
			Log::Critical(exception.what());
#else
			RAY_ASSERT(false, exception.what());
#endif
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
#ifdef NDEBUG
			Log::Critical(exception.what());
#else
			RAY_ASSERT(false, exception.what());
#endif
			throw exception;
		}
	};

	template<typename... Args>
	ThrowRenderError(VkResult, std::string, Args&&...) -> ThrowRenderError<Args...>;

	template<typename... Args>
	ThrowRenderError(int, std::string, Args&&...) -> ThrowRenderError<Args...>;

	template<typename... Args>
	ThrowRenderError(std::string, Args&&...) -> ThrowRenderError<Args...>;

	template<typename... Args>
	ThrowRuntime(std::string, Args&&...) -> ThrowRuntime<Args...>;
}  // namespace At0::Ray
