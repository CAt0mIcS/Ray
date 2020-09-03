#include "pch.h"
#include "Exceptions.h"


namespace NPE
{
	Exception::Exception(unsigned int line, const char* file)
		: m_Line(line), m_File(file), m_WhatBuffer("")
	{
	}

	std::string Exception::GetOriginString() const
	{
		std::ostringstream oss;
		oss << "\t[File] " << m_File << '\n'
			<< "\t[Line] " << m_Line;

		return oss.str();
	}

	//WindowException
	WindowException::WindowException(HRESULT hr, unsigned int line, const char* file)
		: Exception(line, file), m_Hr(hr)
	{
	}

	const char* WindowException::what() const
	{
		std::ostringstream oss;
		oss << GetType() << '\n'
			<< "\t[Error Code] " << m_Hr << '\n'
			<< "\t[Description] " << GetErrorString() << '\n'
			<< GetOriginString();

		m_WhatBuffer = oss.str();
		return m_WhatBuffer.c_str();
	}

	std::string WindowException::GetErrorString() const
	{
		char* pMsgBuf = nullptr;
		DWORD nMsgLen = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			nullptr, m_Hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPWSTR)&pMsgBuf, 0, nullptr);

		if (nMsgLen == 0)
			return "Unidentified error code";

		std::string errorString = pMsgBuf;
		LocalFree(pMsgBuf);
		return errorString;
	}

	//D2D1Exception
	GraphicsException::GraphicsException(HRESULT hr, unsigned int line, const char* file, const char* message)
		: Exception(line, file), m_Hr(hr), m_Message(message)
	{
	}

	const char* GraphicsException::what() const
	{
		std::ostringstream oss;
		oss << GetType() << '\n'
			<< "\t[Error Code] " << m_Hr << '\n'
			<< "\t[Message] " << m_Message << '\n'
			<< GetOriginString();
		m_WhatBuffer = oss.str();
		return m_WhatBuffer.c_str();
	}
}


