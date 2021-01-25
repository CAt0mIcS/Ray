#pragma once

#include <exception>
#include <stdint.h>
#include <string>

#include "RUBase.h"
#include "RSerialize.h"

#ifdef _WIN32
#include <Windows.h>
#endif

#pragma warning(disable : 4275)
#pragma warning(disable : 4251)


namespace At0::Ray
{
	/// <summary>
	/// Base Exception class
	/// </summary>
	class RU_API Exception : public std::exception
	{
	public:
		/// <summary>
		/// Exception contructor
		/// </summary>
		/// <param name="line">Is the line where the exception occured</param>
		/// <param name="file">Is the file where the exception occured</param>
		Exception(uint16_t line, const char* file);

		/// <summary>
		/// Getter for a string with line and file info
		/// </summary>
		/// <returns>Line/file string</returns>
		std::string GetDefaultString() const;

		/// <summary>
		/// Getter for string with Exception type
		/// </summary>
		/// <returns>The exception type string</returns>
		virtual const char* GetType() const { return "Exception"; };

		/// <summary>
		/// Getter for more information about the error
		/// </summary>
		/// <returns>More information about the error</returns>
		virtual const char* what() const noexcept override;

		/// <summary>
		/// Virtual Exception Deconstructor
		/// </summary>
		virtual ~Exception() = default;

	protected:
		/// <summary>
		/// Buffer to store string from Exception::what()
		/// </summary>
		mutable std::string m_WhatBuffer;

		/// <summary>
		/// Line where the exception occured
		/// </summary>
		unsigned int m_Line;

		/// <summary>
		/// File where the exception occured
		/// </summary>
		const char* m_File;
	};

	class RU_API RuntimeException : public Exception
	{
	public:
		/// <summary>
		/// Exception contructor
		/// </summary>
		/// <param name="message">Is a message associated with the exception</param>
		/// <param name="line">Is the line where the exception occured</param>
		/// <param name="file">Is the file where the exception occured</param>
		RuntimeException(const char* message, uint16_t line, const char* file);

		/// <summary>
		/// Getter for string with Exception type
		/// </summary>
		/// <returns>The exception type string</returns>
		virtual const char* GetType() const { return "Runtime Exception"; };

		/// <summary>
		/// Getter for more information about the error
		/// </summary>
		/// <returns>More information about the error</returns>
		virtual const char* what() const noexcept override;

	private:
		std::string m_Message;
	};

	class RU_API VulkanException : public RuntimeException
	{
	public:
		using RuntimeException::RuntimeException;

		/// <summary>
		/// Getter for string with Exception type
		/// </summary>
		/// <returns>The exception type string</returns>
		virtual const char* GetType() const { return "Vulkan Exception"; };
	};

#ifdef _WIN32

	class RU_API WindowsException : public Exception
	{
	public:
		/// <summary>
		/// WindowException Contructor
		/// </summary>
		/// <param name="hr">Is the error code</param>
		/// <param name="line">Is the line where the error occured</param>
		/// <param name="file">Is the file where the error occured</param>
		WindowsException(HRESULT hr, uint16_t line, const char* file);

		/// <summary>
		/// Getter for string with Exception type
		/// </summary>
		/// <returns>The exception type string</returns>
		virtual const char* GetType() const override { return "Windows Exception"; }

		/// <summary>
		/// Getter for more information about the error
		/// </summary>
		/// <returns>More information about the error</returns>
		virtual const char* what() const noexcept override;

		/// <summary>
		/// Uses format message to get the message string from Win32API
		/// </summary>
		/// <returns>The formated message</returns>
		std::string GetErrorString() const;

	private:
		/// <summary>
		/// HRESULT error code
		/// </summary>
		HRESULT m_Hr;
	};

	class RU_API GraphicsException : public Exception
	{
	public:
		/// <summary>
		/// WindowException Contructor
		/// </summary>
		/// <param name="hr">Is the error code</param>
		/// <param name="line">Is the line where the error occured</param>
		/// <param name="file">Is the file where the error occured</param>
		GraphicsException(HRESULT hr, uint16_t line, const char* file);

		/// <summary>
		/// Getter for more information about the error
		/// </summary>
		/// <returns>More information about the error</returns>
		virtual const char* what() const noexcept override;

		/// <summary>
		/// Uses format message to get the message string from Win32API
		/// </summary>
		/// <returns>The formated message</returns>
		std::string GetErrorString() const;

		/// <summary>
		/// Getter for string with Exception type
		/// </summary>
		/// <returns>The exception type string</returns>
		virtual const char* GetType() const override { return "DirectX11 Exception"; }

		/// <summary>
		/// Gets the DX11 error description
		/// </summary>
		/// <returns>Returns a description for the specific error</returns>
		std::string GetErrorDescription() const;
	private:
		HRESULT m_Hr;
	};

#endif
}

#ifdef _MSC_VER

#define RAY_WND_THROW_FAILED(expr) \
if(HRESULT RL___HRES___RL = expr; FAILED(RL___HRES___RL)) throw ::At0::Ray::WindowsException(RL___HRES___RL, (uint16_t)__LINE__, __FILE__)

#define RAY_WND_THROW_LAST_FAILED_NO_EXPR() \
throw ::At0::Ray::WindowsException(::GetLastError(), (uint16_t)__LINE__, __FILE__)

#define RAY_WND_THROW_LAST_FAILED(expr) if(!(expr)) RAY_WND_THROW_LAST_FAILED_NO_EXPR()

#define RAY_DX_THROW_FAILED(expr) if(HRESULT RL___HRES___RL = (expr); FAILED(RL___HRES___RL)) throw ::At0::Ray::GraphicsException(RL___HRES___RL, (uint16_t)__LINE__, __FILE__)
#define RAY_VK_THROW_NO_EXPR(msg, ...) throw ::At0::Ray::VulkanException(::At0::Ray::Util::SerializeString(msg, __VA_ARGS__).c_str(), (uint16_t)__LINE__, __FILE__)
#define RAY_VK_THROW_FAILED(expr, msg, ...) if((expr) != VK_SUCCESS) RAY_VK_THROW_NO_EXPR(msg, __VA_ARGS__)

#define RAY_THROW_RUNTIME(msg, ...) throw ::At0::Ray::RuntimeException(::At0::Ray::Util::SerializeString(msg, __VA_ARGS__).c_str(), (uint16_t)__LINE__, __FILE__)

#elif defined(__GNUC__)

#define RAY_WND_THROW_FAILED(expr) expr
#define RAY_WND_THROW_LAST_FAILED2()
#define RAY_WND_THROW_LAST_FAILED(expr) expr
// RAY_TODO: Enable for linux
// #define RAY_THROW_RUNTIME(msg, ...) throw ::At0::Ray::RuntimeException(::At0::Ray::Util::SerializeString(msg, ## __VA_ARGS__).c_str(), (uint16_t)__LINE__, __FILE__)
#defube RAY_VK_THROW_NO_EXPR(msg, ...);
#define RAY_VK_GFX_THROW_FAILED(expr, msg, ...)
#define RAY_THROW_RUNTIME(msg, ...)

#endif
