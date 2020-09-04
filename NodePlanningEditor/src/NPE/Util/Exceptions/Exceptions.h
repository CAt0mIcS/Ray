#pragma once

#include <string>


namespace NPE
{
	class Exception : public std::exception
	{
	public:
		Exception(unsigned int line, const char* file);

		Exception(const Exception&) = delete;
		Exception& operator=(const Exception&) = delete;

		std::string GetOriginString() const;

		virtual const char* GetType() const = 0;
		virtual const char* what() const override = 0;

		virtual ~Exception() = default;

	protected:
		mutable std::string m_WhatBuffer;
		unsigned int m_Line;
		const char* m_File;
	};

	//TODO: Add future exceptions here and test BaseException class
	class WindowException : public Exception
	{
	public:
		WindowException(HRESULT hr, unsigned int line, const char* file);

		virtual const char* GetType() const override { return "NPE WindowException"; }
		virtual const char* what() const override;
		std::string GetErrorString() const;

	private:
		HRESULT m_Hr;
	};

	class GraphicsException : public Exception
	{
	public:
		GraphicsException(HRESULT hr, unsigned int line, const char* file, const char* message);

		virtual const char* GetType() const override { return "NPE GraphicsException"; }
		virtual const char* what() const override;

	private:
		HRESULT m_Hr;
		const char* m_Message;
	};

}


#define NPE_THROW_EXCEPT() throw NPE::Exception(__LINE__, __FILE__)

#define NPE_THROW_WND_EXCEPT(hr) throw NPE::WindowException(hr, __LINE__, __FILE__)
#define NPE_THROW_GFX_EXCEPT(hr, msg) if(FAILED(hr)) throw NPE::GraphicsException(hr, __LINE__, __FILE__, msg)