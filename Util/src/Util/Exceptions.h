#pragma once

#include <string>
#include <sstream>
#include "UtilBase.h"

#include "Debug/Logger.h"
#include "Util/Util.h"

/**
* non dll-interface class used as base for dll-interface class
*/
#pragma warning(disable : 4275)


namespace NPE
{
	/// <summary>
	/// Base Exception class
	/// </summary>
	class UTIL_API Exception : public std::exception
	{
	public:
		/// <summary>
		/// Exception contructor
		/// </summary>
		/// <param name="line">Is the line where the exception occured</param>
		/// <param name="file">Is the file where the exception occured</param>
		Exception(unsigned int line, const char* file);

		/// <summary>
		/// Deleted Copy Constructor
		/// </summary>
		Exception(const Exception&) = delete;

		/// <summary>
		/// Deleted Copy Assignment operator
		/// </summary>
		Exception& operator=(const Exception&) = delete;

		/// <summary>
		/// Getter for a string with line and file info
		/// </summary>
		/// <returns>Line/file string</returns>
		std::string GetOriginString() const;

		/// <summary>
		/// Getter for string with Exception type
		/// </summary>
		/// <returns>The exception type string</returns>
		virtual const char* GetType() const { return "NPE Exception"; };
		
		/// <summary>
		/// Getter for more information about the error
		/// </summary>
		/// <returns>More information about the error</returns>
		virtual const char* what() const override;

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

	/// <summary>
	/// Exception class which handles Win32API HRESULT error codes
	/// </summary>
	class UTIL_API WindowException : public Exception
	{
	public:
		/// <summary>
		/// WindowException Contructor
		/// </summary>
		/// <param name="hr">Is the error code</param>
		/// <param name="line">Is the line where the error occured</param>
		/// <param name="file">Is the file where the error occured</param>
		WindowException(HRESULT hr, unsigned int line, const char* file);

		/// <summary>
		/// WindowException Constructor
		/// </summary>
		/// <param name="other">Is another window exception</param>
		WindowException(const WindowException& other);

		/// <summary>
		/// Getter for string with Exception type
		/// </summary>
		/// <returns>The exception type string</returns>
		virtual const char* GetType() const override { return "NPE WindowException"; }

		/// <summary>
		/// Getter for more information about the error
		/// </summary>
		/// <returns>More information about the error</returns>
		virtual const char* what() const override;
		
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

	class UTIL_API GraphicsException : public Exception
	{
	public:
		/// <summary>
		/// GraphicsException Contructor
		/// </summary>
		/// <param name="hr">Is the error code</param>
		/// <param name="line">Is the line where the error occured</param>
		/// <param name="file">Is the file where the error occured</param>
		/// <param name="message">Is a message to display additional information</param>
		GraphicsException(HRESULT hr, unsigned int line, const char* file, const char* message);

		/// <summary>
		/// GraphicsException Constructor
		/// </summary>
		/// <param name="other">Is another graphics exception</param>
		GraphicsException(const GraphicsException& other);

		/// <summary>
		/// Getter for string with Exception type
		/// </summary>
		/// <returns>The exception type string</returns>
		virtual const char* GetType() const override { return "NPE GraphicsException"; }

		/// <summary>
		/// Getter for more information about the error
		/// </summary>
		/// <returns>More information about the error</returns>
		virtual const char* what() const override;

		/// <summary>
		/// Checks which error was thrown and returns the error string for the HRESULT
		/// </summary>
		/// <returns>The error string specified on https://docs.microsoft.com/en-us/windows/win32/direct2d/direct2d-error-codes</returns>
		std::string GetErrorString() const;

	private:
		/// <summary>
		/// HRESULT error code
		/// </summary>
		HRESULT m_Hr;

		/// <summary>
		/// Custom message to add additional information
		/// </summary>
		const char* m_Message;
	};

	class MSGException : public Exception
	{
	public:
		/// <summary>
		/// Exception with message Contructor
		/// </summary>
		/// <param name="message">Is the message to display if the error occurs</param>
		/// <param name="line">Is the line where the error occured</param>
		/// <param name="file">Is the file where the error occured</param>
		MSGException(const char* message, unsigned int line, const char* file);

		/// <summary>
		/// MSGException Constructor
		/// </summary>
		/// <param name="other">Is another msg exception</param>
		MSGException(const MSGException& other);

		/// <summary>
		/// Getter for string with Exception type
		/// </summary>
		/// <returns>The exception type string</returns>
		virtual const char* GetType() const override { return "NPE Exception"; }

		/// <summary>
		/// Getter for more information about the error
		/// </summary>
		/// <returns>More information about the error</returns>
		virtual const char* what() const override;

	private:
		/// <summary>
		/// Custom message to add additional information
		/// </summary>
		const char* m_Message;
	};

}


//TODO: Add logging when exception occurs
#define NPE_THROW_EXCEPT() \
{ \
	NPE::Exception e(__LINE__, __FILE__); \
	throw e; \
}

#define NPE_THROW_WND_EXCEPT(hr) \
if(FAILED(hr)) \
{ \
	NPE::WindowException e(hr, __LINE__, __FILE__); \
	throw e; \
}

#define NPE_THROW_LAST_WND_EXCEPT() \
{ \
	NPE::WindowException e(GetLastError(), __LINE__, __FILE__); \
	throw e; \
}

#define NPE_THROW_GFX_EXCEPT(hr, msg) \
if(FAILED(hr)) \
{ \
	NPE::GraphicsException e(hr, __LINE__, __FILE__, msg); \
	throw e; \
}

#define NPE_THROW_EXCEPT_MSG(msg) \
{ \
	NPE::MSGException e(msg, __LINE__, __FILE__); \
	throw e; \
}
