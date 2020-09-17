#include "pch.h"
#include "Exceptions.h"


namespace NPE
{
	Exception::Exception(unsigned int line, const char* file)
		: m_Line(line), m_File(file), m_WhatBuffer(L"")
	{
	}

	std::wstring Exception::GetOriginString() const
	{
		std::wstringstream oss;
		oss << "\t[File] " << m_File << '\n'
			<< "\t[Line] " << m_Line;

		return oss.str();
	}

	//WindowException
	WindowException::WindowException(HRESULT hr, unsigned int line, const char* file)
		: Exception(line, file), m_Hr(hr)
	{
	}

	const wchar_t* WindowException::what() const
	{
		std::wstringstream oss;
		oss << GetType() << '\n'
			<< "\t[Error Code] " << m_Hr << '\n'
			<< "\t[Description] " << GetErrorString() << '\n'
			<< GetOriginString();

		m_WhatBuffer = oss.str();
		return m_WhatBuffer.c_str();
	}

	std::wstring WindowException::GetErrorString() const
	{
		wchar_t* pMsgBuf = nullptr;
		DWORD nMsgLen = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			nullptr, m_Hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPWSTR)&pMsgBuf, 0, nullptr);

		if (nMsgLen == 0)
			return L"Unidentified error code";

		std::wstring errorString = pMsgBuf;
		LocalFree(pMsgBuf);
		return errorString;
	}

	//D2D1Exception
	GraphicsException::GraphicsException(HRESULT hr, unsigned int line, const char* file, const char* message)
		: Exception(line, file), m_Hr(hr), m_Message(message)
	{
	}

	const wchar_t* GraphicsException::what() const
	{
		std::wstringstream oss;
		oss << GetType() << '\n'
			<< "\t[Error Code] 0x" << std::hex << std::uppercase << m_Hr << '\n'
			<< "\t[Message] " << m_Message << '\n'
			<< GetOriginString();
		m_WhatBuffer = oss.str();
		return m_WhatBuffer.c_str();
	}
	
	MSGException::MSGException(const char* message, unsigned int line, const char* file)
		: Exception(line, file), m_Message(message)
	{

	}
	
	const wchar_t* MSGException::what() const
	{
		std::wstringstream oss;
		oss << GetType() << '\n'
			<< "\t[Message] " << m_Message << '\n'
			<< GetOriginString();
		m_WhatBuffer = oss.str();
		return m_WhatBuffer.c_str();
	}
}


