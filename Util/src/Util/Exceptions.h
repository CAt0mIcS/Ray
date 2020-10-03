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
	class UTIL_API Exception : public std::exception
	{
	public:
		/**
		* Exception contructor
		* 
		* @param line is the line where the exception occured
		* @param file is the file where the exception occured
		*/
		Exception(unsigned int line, const char* file);

		/**
		* Deleted copy-asignment operators
		*/
		Exception(const Exception&) = delete;
		Exception& operator=(const Exception&) = delete;

		/**
		* Getter for a string with line and file info
		* 
		* @returns line/file string
		*/
		std::string GetOriginString() const;

		/**
		* Getter for string with type
		* 
		* @returns string with type
		*/
		virtual const char* GetType() const { return "NPE Exception"; };

		/**
		* Getter for more information about the error
		* 
		* @returns more information about the erro
		*/
		virtual const char* what() const override;

		/**
		* Virtual Exception destructor
		*/
		virtual ~Exception() = default;

	protected:
		mutable std::string m_WhatBuffer;
		unsigned int m_Line;
		const char* m_File;
	};


	class UTIL_API WindowException : public Exception
	{
	public:
		/**
		* WindowException contructor
		* 
		* @param hr is the error code
		* @param line is the line where the error occured
		* @param file is the file where the error occured
		*/
		WindowException(HRESULT hr, unsigned int line, const char* file);

		/**
		* WindowException constructor
		*
		* @param other is another window exception
		*/
		WindowException(const WindowException& other);

		/**
		* Getter for string with type
		*
		* @returns string with type
		*/
		virtual const char* GetType() const override { return "NPE WindowException"; }

		/**
		* Getter for more information about the error
		*
		* @returns more information about the erro
		*/
		virtual const char* what() const override;

		/**
		* Uses format message to get the message string from Win32API
		* 
		* @returns the formated message
		*/
		std::string GetErrorString() const;

	private:
		HRESULT m_Hr;
	};

	class UTIL_API GraphicsException : public Exception
	{
	public:
		/**
		* GraphicsException contructor
		*
		* @param hr is the error code
		* @param line is the line where the error occured
		* @param file is the file where the error occured
		*/
		GraphicsException(HRESULT hr, unsigned int line, const char* file, const char* message);

		/**
		* GraphicsException constructor
		* 
		* @param other is another graphics exception
		*/
		GraphicsException(const GraphicsException& other);

		/**
		* Getter for string with type
		*
		* @returns string with type
		*/
		virtual const char* GetType() const override { return "NPE GraphicsException"; }

		/**
		* Getter for more information about the error
		*
		* @returns more information about the erro
		*/
		virtual const char* what() const override;

		/**
		* Checks which error was thrown and returns the error string for the HRESULT
		* 
		* @returns the error string specified on https://docs.microsoft.com/en-us/windows/win32/direct2d/direct2d-error-codes
		*/
		std::string GetErrorString() const;

	private:
		HRESULT m_Hr;
		const char* m_Message;
	};

	class MSGException : public Exception
	{
	public:
		/**
		* Exception with message contructor
		*
		* @param message is the message to display if the error occurs
		* @param line is the line where the error occured
		* @param file is the file where the error occured
		*/
		MSGException(const char* message, unsigned int line, const char* file);

		/**
		* MSGException constructor
		*
		* @param other is another msg exception
		*/
		MSGException(const MSGException& other);

		/**
		* Getter for string with type
		*
		* @returns string with type
		*/
		virtual const char* GetType() const override { return "NPE Exception"; }

		/**
		* Getter for more information about the error
		*
		* @returns more information about the erro
		*/
		virtual const char* what() const override;

	private:
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
