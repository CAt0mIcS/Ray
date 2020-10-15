#pragma once

#include <fstream>
#include "ZealBaseLogger.h"

#include "Private/ZealHelper.h"


namespace Zeal::Log
{
	class FileLogger : public BaseLogger
	{
	public:
		/// <summary>
		/// Default FileLogger Constructor
		/// </summary>
		FileLogger()
			: m_FilePath(L"")
		{

		}

		/// <summary>
		/// File Logger Constructor
		/// </summary>
		/// <param name="filepath">Is the path to the log file. Note: The file will be cleared when the logger starts</param>
		FileLogger(const std::wstring& filepath)
			: m_FilePath(filepath)
		{

		}

		/// <summary>
		/// FileLogger Deconstructor, automatically closes the stream
		/// </summary>
		~FileLogger()
		{
			Close();
		}

		/// <summary>
		/// Sets the file path to the log file. Note:
		/// This should only be used if the logger was created with the default Constructor to set the path once
		/// </summary>
		/// <param name="filepath"></param>
		void SetFilePath(const std::wstring& filepath) { m_FilePath = filepath; }

		/// <summary>
		/// Opens the FileStream
		/// </summary>
		virtual void Open() override
		{
			m_Writer.open(m_FilePath);
		}

		/// <summary>
		/// Flushes the FileStream
		/// </summary>
		virtual void Flush() override
		{
			m_Writer.flush();
		}

		/// <summary>
		/// Closes the FileStream
		/// </summary>
		virtual void Close() override
		{
			m_Writer.close();
		}

	private:
		/// <summary>
		/// Writes the message to the output stream
		/// </summary>
		/// <param name="message">Is the message to output</param>
		virtual void Log(const std::wstring& message) override
		{
			std::scoped_lock lock(m_StreamMutex);
			m_Writer << message << '\n';
		}

	private:
		std::wofstream m_Writer;
		std::wstring m_FilePath;
		std::mutex m_StreamMutex;
	};
}
