#pragma once

#include <exception>
#include <stdint.h>
#include <string>

#include "RlUBase.h"

#ifdef _WIN32
	#include <Windows.h>
#endif

#pragma warning(disable : 4275)
#pragma warning(disable : 4251)


namespace At0::Reyal
{
	/// <summary>
	/// Base Exception class
	/// </summary>
	class RLU_API Exception : public std::exception
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

#ifdef _WIN32

	class RLU_API WindowsException : public Exception
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

	class RLU_API GraphicsException : public Exception
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

#ifdef _WIN32

#define RL_THROW_WND_EXCEPT(expr) \
if(HRESULT RL___HRES___RL = expr; FAILED(RL___HRES___RL)) throw ::At0::Reyal::WindowsException(RL___HRES___RL, (uint16_t)__LINE__, __FILE__)

#define RL_THROW_LAST_WND_EXCEPT2() \
throw ::At0::Reyal::WindowsException(::GetLastError(), (uint16_t)__LINE__, __FILE__)

#define RL_THROW_LAST_WND_EXCEPT(expr) if(!(expr)) RL_THROW_LAST_WND_EXCEPT2()

/// <TODO>
/// HRESULT expands to expression twice (calls e.g. create function twice because it's inserted in hr)
/// </TODO>
#define RL_GFX_THROW_FAILED(expr) if(HRESULT RL___HRES___RL = (expr); FAILED(RL___HRES___RL)) throw ::At0::Reyal::GraphicsException(RL___HRES___RL, (uint16_t)__LINE__, __FILE__)

#else

#define RL_THROW_WND_EXCEPT(hr)
#define RL_THROW_LAST_WND_EXCEPT2()
#define RL_THROW_LAST_WND_EXCEPT()

#endif

