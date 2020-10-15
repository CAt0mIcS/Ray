#pragma once

#include <string>
#include <mutex>
#include <vector>
#include <sstream>

#include "Formatter/LogLevelFormatter.h"
#include "Formatter/DateFormatter.h"

#include "Private/ZealHelper.h"

#include "Private/LogLevel.h"


namespace Zeal::Log
{

	class BaseLogger
	{
	public:
		/// <summary>
		/// Setts the log severity level of this logger
		/// </summary>
		/// <param name="level">Is the new log level</param>
		void SetLogLevel(LogLevel level) { m_LogLevel = level; }

		/// <summary>
		/// Getter for the current log level
		/// </summary>
		/// <returns>The current log level of this logger</returns>
		LogLevel GetLogLevel() const { return m_LogLevel; }

		virtual void Open() = 0;
		
		virtual void Flush() = 0;

		virtual void Close() = 0;

		template<typename... Args>
		void Trace(const std::wstring& str, Args&&... args)
		{
			if (!ShouldLog(LogMessageType::Debug))
				return;

			{
				std::scoped_lock lock(m_Mutex);
				m_LogMessage = str;
			}
			
			FormatMessage(args...);
			LogTrace();
		}

		template<typename... Args>
		void Debug(const std::wstring& str, Args&&... args)
		{
			if (!ShouldLog(LogMessageType::Debug))
				return;

			{
				std::scoped_lock lock(m_Mutex);
				m_LogMessage = str;
			}

			FormatMessage(args...);
			LogDebug();
		}

		template<typename... Args>
		void Info(const std::wstring& str, Args&&... args)
		{
			if (!ShouldLog(LogMessageType::Debug))
				return;

			{
				std::scoped_lock lock(m_Mutex);
				m_LogMessage = str;
			}

			FormatMessage(args...);
			LogInfo();
		}

		template<typename... Args>
		void Warn(const std::wstring& str, Args&&... args)
		{
			if (!ShouldLog(LogMessageType::Debug))
				return;

			{
				std::scoped_lock lock(m_Mutex);
				m_LogMessage = str;
			}

			FormatMessage(args...);
			LogWarn();
		}

		template<typename... Args>
		void Error(const std::wstring& str, Args&&... args)
		{
			if (!ShouldLog(LogMessageType::Debug))
				return;

			{
				std::scoped_lock lock(m_Mutex);
				m_LogMessage = str;
			}

			FormatMessage(args...);
			LogError();
		}

		template<typename... Args>
		void Critical(const std::wstring& str, Args&&... args)
		{
			if (!ShouldLog(LogMessageType::Debug))
				return;

			{
				std::scoped_lock lock(m_Mutex);
				m_LogMessage = str;
			}

			FormatMessage(args...);
			LogCritical();
		}


		template<typename... Args>
		void Trace(const std::string& str, Args&&... args)
		{
			if (!ShouldLog(LogMessageType::Debug))
				return;

			Trace(Helper::ToWideChar(str));
		}

		template<typename... Args>
		void Debug(const std::string& str, Args&&... args)
		{
			if (!ShouldLog(LogMessageType::Debug))
				return;

			Debug(Helper::ToWideChar(str));
		}

		template<typename... Args>
		void Info(const std::string& str, Args&&... args)
		{
			if (!ShouldLog(LogMessageType::Debug))
				return;

			Info(Helper::ToWideChar(str));
		}

		template<typename... Args>
		void Warn(const std::string& str, Args&&... args)
		{
			if (!ShouldLog(LogMessageType::Debug))
				return;

			Warn(Helper::ToWideChar(str));
		}

		template<typename... Args>
		void Error(const std::string& str, Args&&... args)
		{
			if (!ShouldLog(LogMessageType::Debug))
				return;

			Error(Helper::ToWideChar(str));
		}

		template<typename... Args>
		void Critical(const std::string& str, Args&&... args)
		{
			if (!ShouldLog(LogMessageType::Debug))
				return;

			Critical(Helper::ToWideChar(str));
		}

	protected:
		using LogMessageType = LogLevel;

	protected:
		virtual void LogTrace() = 0;

		virtual void LogDebug() = 0;

		virtual void LogInfo() = 0;

		virtual void LogWarn() = 0;

		virtual void LogError() = 0;

		virtual void LogCritical() = 0;

		BaseLogger()
			: m_LogLevel(LogLevel::None)
		{
			LogLevelFormatter* pLogLevelFormatter = new LogLevelFormatter();
			m_Formatters.push_back(pLogLevelFormatter);

			DateFormatter* pDateFormatter = new DateFormatter();
			m_Formatters.push_back(pDateFormatter);
		}

		virtual ~BaseLogger()
		{
			for (auto* formatter : m_Formatters)
			{
				if (formatter)
					delete formatter;
			}
		}

		template<typename... Args>
		void SerializeString(Args&&... args)
		{
			int argCount = 0;

			std::scoped_lock lock(m_Mutex);
			(SerializeStringArg(args, argCount), ...);
		}

		bool ShouldLog(LogMessageType msgType)
		{
			std::scoped_lock lock(m_Mutex);
			return msgType >= m_LogLevel;
		}

	private:
		template<typename T>
		void SerializeStringArg(T&& arg, int& argCount)
		{
			if (argCount == -1)
				return;

			std::wostringstream oss;
			oss << arg;
			
			const std::wstring toFind = L"{" + std::to_wstring(argCount) + L"}";
			size_t foundIdx = m_LogMessage.find(toFind);
			
			if (foundIdx == std::string::npos)
			{
				argCount = -1;
				return;
			}
			
			m_LogMessage.replace(m_LogMessage.begin() + foundIdx, m_LogMessage.begin() + foundIdx + std::size(toFind), oss.str());
			++argCount;
		}

		template<typename... Args>
		void FormatMessage(Args&&... args)
		{
			SerializeString(args...);

			std::scoped_lock lock(m_Mutex);
			for (auto formatter : m_Formatters)
			{
				formatter->Format(m_LogMessage, m_LogLevel);
			}
		}

	protected:
#ifndef  ZEAL_LOG_NON_THREAD_SAVE
		std::mutex m_Mutex;
#endif // ! ZEAL_LOG_NON_THREAD_SAVE

		LogLevel m_LogLevel;
		std::wstring m_LogMessage;
		std::vector<Formatter*> m_Formatters;

	};
}

