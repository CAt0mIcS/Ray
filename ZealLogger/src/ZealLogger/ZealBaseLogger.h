#pragma once

#include <string>
#include <mutex>
#include <vector>
#include <sstream>

#include "Formatter/BracketFormatter.h"
#include "Formatter/LogLevelFormatter.h"
#include "Formatter/DateTimeFormatter.h"

#include "Private/ZealHelper.h"

#include "Private/LogLevel.h"
#include "Private/Queue.h"


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
			if (!ShouldLog(LogMessageType::Trace))
				return;

			Log(FormatMessage(str, std::forward<Args>(args)...));
		}

		template<typename... Args>
		void Debug(const std::wstring& str, Args&&... args)
		{
			if (!ShouldLog(LogMessageType::Debug))
				return;

			Log(FormatMessage(str, std::forward<Args>(args)...));
		}

		template<typename... Args>
		void Info(const std::wstring& str, Args&&... args)
		{
			if (!ShouldLog(LogMessageType::Debug))
				return;

			Log(FormatMessage(str, std::forward<Args>(args)...));
		}

		template<typename... Args>
		void Warn(const std::wstring& str, Args&&... args)
		{
			if (!ShouldLog(LogMessageType::Debug))
				return;

			Log(FormatMessage(str, std::forward<Args>(args)...));
		}

		template<typename... Args>
		void Error(const std::wstring& str, Args&&... args)
		{
			if (!ShouldLog(LogMessageType::Debug))
				return;

			Log(FormatMessage(str, std::forward<Args>(args)...));
		}

		template<typename... Args>
		void Critical(const std::wstring& str, Args&&... args)
		{
			if (!ShouldLog(LogMessageType::Debug))
				return;

			Log(FormatMessage(str, std::forward<Args>(args)...));
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
		virtual void Log(const std::wstring& message) = 0;

		BaseLogger()
			: m_LogLevel(LogLevel::None)
		{
			BracketFormatter* pBracketFormatter = new BracketFormatter();
			m_Formatters.push_back(pBracketFormatter);

			LogLevelFormatter* pLogLevelFormatter = new LogLevelFormatter();
			m_Formatters.push_back(pLogLevelFormatter);

			DateTimeFormatter* pDateFormatter = new DateTimeFormatter();
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
		std::wstring SerializeString(Args&&... args)
		{
			int argCount = 0;
			std::wstring serializedStr = m_LogMessages.PopFront();
			(SerializeStringArg(serializedStr, args, argCount), ...);
			return serializedStr;
		}

		bool ShouldLog(LogMessageType msgType)
		{
#ifndef ZEAL_LOG_NON_THREAD_SAVE
			
			std::scoped_lock lock(s_Mutex);

#endif
			return msgType >= m_LogLevel;
		}

	private:
		template<typename T>
		void SerializeStringArg(std::wstring& message, T&& arg, int& argCount)
		{
			if (argCount == -1)
				return;

			std::wostringstream oss;
			oss << arg;
			
			const std::wstring toFind = L"{" + std::to_wstring(argCount) + L"}";
			size_t foundIdx = message.find(toFind);
			
			if (foundIdx == std::string::npos)
			{
				argCount = -1;
				return;
			}
			
			message.replace(message.begin() + foundIdx, message.begin() + foundIdx + std::size(toFind), oss.str());
			++argCount;
		}

		template<typename... Args>
		std::wstring FormatMessage(const std::wstring& str, Args&&... args)
		{
			m_LogMessages.PushBack(str);
			std::wstring msg = SerializeString(args...);

			for (auto formatter : m_Formatters)
			{
				formatter->Format(msg, m_LogLevel);
			}
			return msg;
		}

	protected:
#ifndef  ZEAL_LOG_NON_THREAD_SAVE
		std::mutex s_Mutex;
#endif // ! ZEAL_LOG_NON_THREAD_SAVE

		LogLevel m_LogLevel;
		Queue<std::wstring> m_LogMessages;
		std::vector<Formatter*> m_Formatters;

	};
}

