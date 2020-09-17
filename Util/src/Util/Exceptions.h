#pragma once

#include <string>
#include "UtilBase.h"

/**
* non dll-interface class used as base for dll-interface class
*/
#pragma warning(disable : 4275)


namespace NPE
{
	class UTIL_API Exception
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
		std::wstring GetOriginString() const;

		/**
		* Getter for string with type
		* 
		* @returns string with type
		*/
		virtual const wchar_t* GetType() const = 0;

		/**
		* Getter for more information about the error
		* 
		* @returns more information about the erro
		*/
		virtual const wchar_t* what() const = 0;

		/**
		* Virtual Exception destructor
		*/
		virtual ~Exception() = default;

	protected:
		mutable std::wstring m_WhatBuffer;
		unsigned int m_Line;
		const char* m_File;
	};

	//TODO: Add future exceptions here and test BaseException class
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
		* Getter for string with type
		*
		* @returns string with type
		*/
		virtual const wchar_t* GetType() const override { return L"NPE WindowException"; }

		/**
		* Getter for more information about the error
		*
		* @returns more information about the erro
		*/
		virtual const wchar_t* what() const override;

		/**
		* Uses format message to get the message string from Win32API
		* 
		* @returns the formated message
		*/
		std::wstring GetErrorString() const;

	private:
		HRESULT m_Hr;
	};

	class UTIL_API GraphicsException : public Exception
	{
	public:
		/**
		* WindowException contructor
		*
		* @param hr is the error code
		* @param line is the line where the error occured
		* @param file is the file where the error occured
		*/
		GraphicsException(HRESULT hr, unsigned int line, const char* file, const char* message);

		/**
		* Getter for string with type
		*
		* @returns string with type
		*/
		virtual const wchar_t* GetType() const override { return L"NPE GraphicsException"; }

		/**
		* Getter for more information about the error
		*
		* @returns more information about the erro
		*/
		virtual const wchar_t* what() const override;

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
		* Getter for string with type
		*
		* @returns string with type
		*/
		virtual const wchar_t* GetType() const override { return L"NPE Exception with message"; }

		/**
		* Getter for more information about the error
		*
		* @returns more information about the erro
		*/
		virtual const wchar_t* what() const override;

	private:
		const char* m_Message;
	};

}


#define NPE_THROW_EXCEPT() throw NPE::Exception(__LINE__, __FILE__)

#define NPE_THROW_WND_EXCEPT(hr) throw NPE::WindowException(hr, __LINE__, __FILE__)
#define NPE_THROW_GFX_EXCEPT(hr, msg) if(FAILED(hr)) throw NPE::GraphicsException(hr, __LINE__, __FILE__, msg)
#define NPE_THROW_EXCEPT_MSG(msg) throw NPE::MSGException(msg, __LINE__, __FILE__)
