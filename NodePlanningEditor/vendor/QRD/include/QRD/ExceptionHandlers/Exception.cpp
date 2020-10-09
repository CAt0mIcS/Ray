#include <sstream>
#include "Exception.h"



namespace QRD
{
    Exception::Exception(const char* file, const unsigned int line, const char* name, const char* message) noexcept
        : m_File(file), m_Line(line), m_Message(message), m_Name(name)
    {
    }

    const char* Exception::what() const noexcept
    {
        std::ostringstream ss;
        ss << "[Exception]:\t " << m_Name
            << "\n[File]:\t\t " << m_File
            << "\n[Line]:\t\t " << m_Line
            << "\n[Message]:\t " << m_Message;
        
        /** Returning ss.str().c_str() would return an unusable pointer to memory
         ** because ss is a local variable */
        m_Buffer = ss.str();
        return m_Buffer.c_str();
    }


    FileNotFoundException::FileNotFoundException(const char* file, const unsigned int line, const std::string& name, const std::string& message) noexcept
        : Exception(file, line, name.c_str(), message.c_str())
    {
    }

    ObjectNotFoundException::ObjectNotFoundException(const char* file, const unsigned int line, const std::string& name, const std::string& message) noexcept
        : Exception(file, line, name.c_str(), message.c_str())
    {
    }

    FileReadException::FileReadException(const char* file, const unsigned int line, const std::string& name, const std::string& message) noexcept
        : Exception(file, line, name.c_str(), message.c_str())
    {
    }

    InvalidCommandException::InvalidCommandException(const char* file, const unsigned int line, const std::string& name, const std::string& message) noexcept
        : Exception(file, line, name.c_str(), message.c_str())
    {
    }

    OutOfRangeException::OutOfRangeException(const char* file, const unsigned int line, const std::string& name, const std::string& message) noexcept
        : Exception(file, line, name.c_str(), message.c_str())
    {
    }
}


