#include "utpch.h"
#include "RayUtil/Exception.h"

#ifdef _WIN32
#include <DxErr/dxerr.h>
#endif


namespace At0::Ray
{
    Exception::Exception(uint16_t line, const char* file)
        : m_Line(line), m_File(file), m_WhatBuffer("")
    {
    }

    std::string Exception::GetDefaultString() const
    {
        std::stringstream oss;
        oss << "[File] " << m_File << '\n'
            << "[Line] " << m_Line;

        return oss.str();
    }

    const char* Exception::what() const noexcept
    {
        std::stringstream oss;
        oss << GetType() << '\n'
            << GetDefaultString();

        m_WhatBuffer = oss.str();
        return m_WhatBuffer.c_str();
    }
    

#ifdef _WIN32
    
    WindowsException::WindowsException(HRESULT hr, uint16_t line, const char* file)
        : Exception(line, file), m_Hr(hr)
    {
    }
    
    const char* WindowsException::what() const noexcept
    {
        std::stringstream oss;
        oss << GetType() << '\n'
            << "[Error Code] 0x" << std::hex << std::uppercase << m_Hr << '\n'
            << "[Description] " << GetErrorString() << '\n'
            << GetDefaultString();

        m_WhatBuffer = oss.str();
        return m_WhatBuffer.c_str();
    }

    std::string WindowsException::GetErrorString() const
    {
        char* pMsgBuf;
        DWORD msgLen = FormatMessageA(
            FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
            nullptr, m_Hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&pMsgBuf, 0, nullptr
        );

        if (msgLen == 0)
            return "Unidentified error code";

        std::string errorCode = pMsgBuf;
        LocalFree(pMsgBuf);
        return errorCode;
    }

    GraphicsException::GraphicsException(HRESULT hr, uint16_t line, const char* file)
        : Exception(line, file), m_Hr(hr)
    {
    }
    
    const char* GraphicsException::what() const noexcept
    {
        std::ostringstream oss;
        oss << GetType() << '\n'
            << "[Error Code] 0x" << std::hex << std::uppercase << m_Hr << '\n'
            << "[Error String] " << GetErrorString() << '\n'
            << "[Description] " << GetErrorDescription() << '\n'
            << GetDefaultString();

        m_WhatBuffer = oss.str();
        return m_WhatBuffer.c_str();
    }

    std::string GraphicsException::GetErrorString() const
    {
        return DXGetErrorStringA(m_Hr);
    }

    std::string GraphicsException::GetErrorDescription() const
    {
        char buff[512];
        DXGetErrorDescriptionA(m_Hr, buff, sizeof(buff));
        return buff;
    }

#endif
}
