#ifndef RL_EXCEPTION_H
#define RL_EXCEPTION_H

#include <exception>
#include "RlBase.h"

// non dll-interface class used as base for dll-interface class (std::exception, std::string)
#pragma warning(disable : 4275)
#pragma warning(disable : 4251)


namespace At0::Reyal
{
	/// <summary>
	/// Base Exception class
	/// </summary>
	class RL_API Exception : public std::exception
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


	class RL_API WindowsException : public Exception
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
}


#define RL_THROW_WND_EXCEPT(hr) \
if(FAILED(hr)) throw ::At0::Reyal::WindowsException(hr, (uint16_t)__LINE__, __FILE__)

#define RL_THROW_LAST_WND_EXCEPT(booleanResult) \
if(!(booleanResult)) throw ::At0::Reyal::WindowsException(::GetLastError(), (uint16_t)__LINE__, __FILE__)


#endif // RL_EXCEPTION_H
