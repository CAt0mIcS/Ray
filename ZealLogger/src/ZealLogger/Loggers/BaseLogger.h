#pragma once

#include <string>
#include <mutex>
#include <vector>
#include <sstream>

#include "../Formatter/BracketFormatter.h"
#include "../Formatter/LogLevelFormatter.h"
#include "../Formatter/DateTimeFormatter.h"

#include "../Private/LogLevel.h"

#include <Util/TypeConvert.h>


namespace Zeal::Log
{

	template<typename T>
	using BaseRefType = typename std::remove_cv<typename std::remove_reference<T>::type>::type;

	template<typename T>
	using BasePtrType = typename std::remove_cv<typename std::remove_pointer<T>::type>::type;

	template<typename T>
	using BaseArrType = typename std::remove_cv<typename std::remove_all_extents<T>::type>::type;

	using LogMessageType = LogLevel;


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
		void Trace(const std::string& str, Args&&... args)
		{
			if (!ShouldLog(LogMessageType::Trace))
				return;

			Log(FormatMessage(str, std::forward<Args>(args)...));
		}

		template<typename... Args>
		void Debug(const std::string& str, Args&&... args)
		{
			if (!ShouldLog(LogMessageType::Debug))
				return;

			Log(FormatMessage(str, std::forward<Args>(args)...));
		}

		template<typename... Args>
		void Info(const std::string& str, Args&&... args)
		{
			if (!ShouldLog(LogMessageType::Debug))
				return;

			Log(FormatMessage(str, std::forward<Args>(args)...));
		}

		template<typename... Args>
		void Warn(const std::string& str, Args&&... args)
		{
			if (!ShouldLog(LogMessageType::Debug))
				return;

			Log(FormatMessage(str, std::forward<Args>(args)...));
		}

		template<typename... Args>
		void Error(const std::string& str, Args&&... args)
		{
			if (!ShouldLog(LogMessageType::Debug))
				return;

			Log(FormatMessage(str, std::forward<Args>(args)...));
		}

		template<typename... Args>
		void Critical(const std::string& str, Args&&... args)
		{
			if (!ShouldLog(LogMessageType::Debug))
				return;

			Log(FormatMessage(str, std::forward<Args>(args)...));
		}


		template<typename... Args>
		void Trace(const std::wstring& str, Args&&... args)
		{
			if (!ShouldLog(LogMessageType::Debug))
				return;

			Log(FormatMessage(Util::WideCharToMultiByte(str), std::forward<Args>(args)...));
		}

		template<typename... Args>
		void Debug(const std::wstring& str, Args&&... args)
		{
			if (!ShouldLog(LogMessageType::Debug))
				return;

			Log(FormatMessage(Util::WideCharToMultiByte(str), std::forward<Args>(args)...));
		}

		template<typename... Args>
		void Info(const std::wstring& str, Args&&... args)
		{
			if (!ShouldLog(LogMessageType::Debug))
				return;

			Log(FormatMessage(Util::WideCharToMultiByte(str), std::forward<Args>(args)...));
		}

		template<typename... Args>
		void Warn(const std::wstring& str, Args&&... args)
		{
			if (!ShouldLog(LogMessageType::Debug))
				return;

			Log(FormatMessage(Util::WideCharToMultiByte(str), std::forward<Args>(args)...));
		}

		template<typename... Args>
		void Error(const std::wstring& str, Args&&... args)
		{
			if (!ShouldLog(LogMessageType::Debug))
				return;

			Log(FormatMessage(Util::WideCharToMultiByte(str), std::forward<Args>(args)...));
		}

		template<typename... Args>
		void Critical(const std::wstring& str, Args&&... args)
		{
			if (!ShouldLog(LogMessageType::Debug))
				return;

			Log(FormatMessage(Util::WideCharToMultiByte(str), std::forward<Args>(args)...));
		}

	protected:
		virtual void Log(const std::string& message) = 0;

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
		std::string SerializeString(std::string serializedStr, Args&&... args)
		{
			int argCount = 0;
			(SerializeStringArg(serializedStr, args, argCount), ...);
			return serializedStr;
		}

		bool ShouldLog(LogMessageType msgType)
		{
			return msgType >= m_LogLevel;
		}

	private:
		template<typename T>
		void SerializeStringArg(std::string& message, T&& arg, int& argCount, 
			typename std::enable_if_t<std::is_same_v<BaseRefType<T>, std::wstring>>* = 0
		)
		{
			WideCharSerialize(message, arg, argCount);
		}

		template<typename T>
		void SerializeStringArg(std::string& message, T&& arg, int& argCount, 
			typename std::enable_if_t<std::is_convertible_v<T, std::wstring>>* = 0, 
			typename std::enable_if_t<!std::is_same_v<BaseRefType<T>, std::wstring>>* = 0
		)
		{
			WideCharSerialize(message, arg, argCount);
		}

		template<typename T>
		void SerializeStringArg(std::string& message, T&& arg, int& argCount,
			typename std::enable_if_t<!std::is_same_v<BaseRefType<T>, std::wstring>>* = 0,
			typename std::enable_if_t<!std::is_convertible_v<T, std::wstring>>* = 0
		)
		{
			MultiByteSerialize(message, arg, argCount);
		}

		template<typename T>
		void WideCharSerialize(std::string& message, T&& arg, int& argCount)
		{
			if (argCount == -1)
				return;

			std::wostringstream oss;
			oss << arg;

			const std::string toFind = "{" + std::to_string(argCount) + "}";
			size_t foundIdx = message.find(toFind);

			if (foundIdx == std::string::npos)
			{
				argCount = -1;
				return;
			}

			message.replace(message.begin() + foundIdx, message.begin() + foundIdx + std::size(toFind), Util::WideCharToMultiByte(oss.str()));
			++argCount;
		}

		template<typename T>
		void MultiByteSerialize(std::string& message, T&& arg, int& argCount)
		{
			if (argCount == -1)
				return;

			std::ostringstream oss;
			oss << arg;

			const std::string toFind = "{" + std::to_string(argCount) + "}";
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
		std::string FormatMessage(const std::string& str, Args&&... args)
		{
			std::string msg = SerializeString(str, args...);

			for (auto formatter : m_Formatters)
			{
				formatter->Format(msg, m_LogLevel);
			}
			return msg;
		}

	protected:
		LogLevel m_LogLevel;
		std::vector<Formatter*> m_Formatters;

	};
}

