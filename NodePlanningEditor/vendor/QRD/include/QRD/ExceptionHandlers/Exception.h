#pragma once

#include <exception>


namespace QRD
{
	/**
	* Base class for any QRD exception
	*/
	class Exception : public std::exception
	{
	public:
		/**
		* Overridden what function from std::exception
		* 
		* @returns More information about the exception
		*/
		virtual const char* what() const noexcept override;

	protected:
		/**
		* Exception constructor
		* 
		* @param file is the path to the file where the error occured
		* @param line is the line where the error was thrown
		* @param name is the name of the error (FileNotFoundException, ...)
		* @param message is a message for more information about the error
		* 
		* @see QRD::Exception::what() to get a string of information about the error
		*/
		Exception(const char* file, const unsigned int line, const char* name, const char* message) noexcept;

	private:
		const std::string m_File;
		const std::string m_Message;
		const std::string m_Name;
		const unsigned int m_Line;
		mutable std::string m_Buffer;
	};

	/**
	* Thrown if .dbs file was not found in specified path
	*/
	class FileNotFoundException : public Exception
	{
	public:
		/**
		* FileNotFoundException constructor
		*
		* @param file is the path to the file where the error occured
		* @param line is the line where the error was thrown
		* @param name is the name of the error (FileNotFoundException, ...)
		* @param message is a message for more information about the error
		*
		* @see QRD::Exception::what() to get a string of information about the error
		*/
		FileNotFoundException(const char* file, const unsigned int line, const std::string& name, const std::string& message) noexcept;
	};

	/**
	* Thrown when trying to find an element (when deleting by name or getting by name) but the element was not found
	*/
	class ObjectNotFoundException : public Exception
	{
	public:
		/**
		* ObjectNotFoundException constructor
		*
		* @param file is the path to the file where the error occured
		* @param line is the line where the error was thrown
		* @param name is the name of the error (FileNotFoundException, ...)
		* @param message is a message for more information about the error
		*
		* @see QRD::Exception::what() to get a string of information about the error
		*/
		ObjectNotFoundException(const char* file, const unsigned int line, const std::string& name, const std::string& message) noexcept;
	};

	/**
	* Thrown if something went wrong with reading the file
	*/
	class FileReadException : public Exception
	{
	public:
		/**
		* FileReadException constructor
		*
		* @param file is the path to the file where the error occured
		* @param line is the line where the error was thrown
		* @param name is the name of the error (FileNotFoundException, ...)
		* @param message is a message for more information about the error
		*
		* @see QRD::Exception::what() to get a string of information about the error
		*/
		FileReadException(const char* file, const unsigned int line, const std::string& name, const std::string& message) noexcept;
	};

	/**
	* Thrown if the syntax for the command is wrong
	*/
	class InvalidCommandException : public Exception
	{
	public:
		/**
		* InvalidCommandException constructor
		*
		* @param file is the path to the file where the error occured
		* @param line is the line where the error was thrown
		* @param name is the name of the error (FileNotFoundException, ...)
		* @param message is a message for more information about the error
		*
		* @see QRD::Exception::what() to get a string of information about the error
		*/
		InvalidCommandException(const char* file, const unsigned int line, const std::string& name, const std::string& message) noexcept;
	};

	/**
	* Thrown if the index for a record is out of range
	*/
	class OutOfRangeException : public Exception
	{
	public:
		/**
		* OutOfRangeException constructor
		*
		* @param file is the path to the file where the error occured
		* @param line is the line where the error was thrown
		* @param name is the name of the error (FileNotFoundException, ...)
		* @param message is a message for more information about the error
		*
		* @see QRD::Exception::what() to get a string of information about the error
		*/
		OutOfRangeException(const char* file, const unsigned int line, const std::string& name, const std::string& message) noexcept;
	};

}


#define QRD_THROW(name, message)	throw QRD::##name(__FILE__, __LINE__, #name, message)
