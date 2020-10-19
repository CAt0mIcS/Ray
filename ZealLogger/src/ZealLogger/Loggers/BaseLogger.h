#pragma once

#include <string>
#include <mutex>
#include <vector>
#include <sstream>

#include <Util/TypeConvert.h>

#include "../Formatter/BracketFormatter.h"
#include "../Formatter/LogLevelFormatter.h"
#include "../Formatter/DateTimeFormatter.h"
#include "../Formatter/NullTerminatorFormatter.h"

#include "../Private/LogLevel.h"


namespace Zeal::Log
{

	template<typename T>
	using BaseRefType = typename std::remove_cv<typename std::remove_reference<T>::type>::type;

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

		/// <summary>
		/// Opens the stream to write to
		/// </summary>
		virtual void Open() = 0;
		
		/// <summary>
		/// Flushes the output stream
		/// </summary>
		virtual void Flush() = 0;

		/// <summary>
		/// Closes the output stream
		/// </summary>
		virtual void Close() = 0;

		/// <summary>
		/// Logs a trace message, will only be logged if the loglevel is trace
		/// </summary>
		/// <typeparam name="...Args">Is any type which has output operator for std::stringstream defined</typeparam>
		/// <param name="str">Is the initial string</param>
		/// <param name="...args">Are arguments to insert into the string</param>
		template<typename... Args>
		void Trace(const std::string& str, Args&&... args)
		{
			if (!ShouldLog(LogMessageType::Trace))
				return;

			Log(FormatMessage(str, LogMessageType::Trace, std::forward<Args>(args)...));
		}

		/// <summary>
		/// Logs a debug message, will only be logged if the loglevel >= Debug
		/// </summary>
		/// <typeparam name="...Args">Is any type which has output operator for std::stringstream defined</typeparam>
		/// <param name="str">Is the initial string</param>
		/// <param name="...args">Are arguments to insert into the string</param>
		template<typename... Args>
		void Debug(const std::string& str, Args&&... args)
		{
			if (!ShouldLog(LogMessageType::Debug))
				return;

			Log(FormatMessage(str, LogMessageType::Debug, std::forward<Args>(args)...));
		}

		/// <summary>
		/// Logs a info message, will only be logged if the loglevel is >= Information
		/// </summary>
		/// <typeparam name="...Args">Is any type which has output operator for std::stringstream defined</typeparam>
		/// <param name="str">Is the initial string</param>
		/// <param name="...args">Are arguments to insert into the string</param>
		template<typename... Args>
		void Info(const std::string& str, Args&&... args)
		{
			if (!ShouldLog(LogMessageType::Information))
				return;

			Log(FormatMessage(str, LogMessageType::Information, std::forward<Args>(args)...));
		}

		/// <summary>
		/// Logs a warning message, will only be logged if the loglevel >= Warning
		/// </summary>
		/// <typeparam name="...Args">Is any type which has output operator for std::stringstream defined</typeparam>
		/// <param name="str">Is the initial string</param>
		/// <param name="...args">Are arguments to insert into the string</param>
		template<typename... Args>
		void Warn(const std::string& str, Args&&... args)
		{
			if (!ShouldLog(LogMessageType::Warning))
				return;

			Log(FormatMessage(str, LogMessageType::Warning, std::forward<Args>(args)...));
		}

		/// <summary>
		/// Logs a error message, will only be logged if the loglevel is >= "Error"
		/// </summary>
		/// <typeparam name="...Args">Is any type which has output operator for std::stringstream defined</typeparam>
		/// <param name="str">Is the initial string</param>
		/// <param name="...args">Are arguments to insert into the string</param>
		template<typename... Args>
		void Error(const std::string& str, Args&&... args)
		{
			if (!ShouldLog(LogMessageType::Error))
				return;

			Log(FormatMessage(str, LogMessageType::Error, std::forward<Args>(args)...));
		}

		/// <summary>
		/// Logs a critical message, will always be logged if the loglevel is not "None"
		/// </summary>
		/// <typeparam name="...Args">Is any type which has output operator for std::stringstream defined</typeparam>
		/// <param name="str">Is the initial string</param>
		/// <param name="...args">Are arguments to insert into the string</param>
		template<typename... Args>
		void Critical(const std::string& str, Args&&... args)
		{
			if (!ShouldLog(LogMessageType::Critical))
				return;

			Log(FormatMessage(str, LogMessageType::Critical, std::forward<Args>(args)...));
		}


		/// <summary>
		/// Logs a trace message, will only be logged if the loglevel is trace
		/// </summary>
		/// <typeparam name="...Args">Is any type which has output operator for std::stringstream defined</typeparam>
		/// <param name="str">Is the initial string</param>
		/// <param name="...args">Are arguments to insert into the string</param>
		template<typename... Args>
		void Trace(const std::wstring& str, Args&&... args)
		{
			if (!ShouldLog(LogMessageType::Trace))
				return;

			Log(FormatMessage(Util::WideCharToMultiByte(str), LogMessageType::Trace, std::forward<Args>(args)...));
		}

		/// <summary>
		/// Logs a debug message, will only be logged if the loglevel >= Debug
		/// </summary>
		/// <typeparam name="...Args">Is any type which has output operator for std::stringstream defined</typeparam>
		/// <param name="str">Is the initial string</param>
		/// <param name="...args">Are arguments to insert into the string</param>
		template<typename... Args>
		void Debug(const std::wstring& str, Args&&... args)
		{
			if (!ShouldLog(LogMessageType::Debug))
				return;

			Log(FormatMessage(Util::WideCharToMultiByte(str), LogMessageType::Debug, std::forward<Args>(args)...));
		}

		/// <summary>
		/// Logs a info message, will only be logged if the loglevel is >= Information
		/// </summary>
		/// <typeparam name="...Args">Is any type which has output operator for std::stringstream defined</typeparam>
		/// <param name="str">Is the initial string</param>
		/// <param name="...args">Are arguments to insert into the string</param>
		template<typename... Args>
		void Info(const std::wstring& str, Args&&... args)
		{
			if (!ShouldLog(LogMessageType::Information))
				return;

			Log(FormatMessage(Util::WideCharToMultiByte(str), LogMessageType::Information, std::forward<Args>(args)...));
		}

		/// <summary>
		/// Logs a warning message, will only be logged if the loglevel >= Warning
		/// </summary>
		/// <typeparam name="...Args">Is any type which has output operator for std::stringstream defined</typeparam>
		/// <param name="str">Is the initial string</param>
		/// <param name="...args">Are arguments to insert into the string</param>
		template<typename... Args>
		void Warn(const std::wstring& str, Args&&... args)
		{
			if (!ShouldLog(LogMessageType::Warning))
				return;

			Log(FormatMessage(Util::WideCharToMultiByte(str), LogMessageType::Warning, std::forward<Args>(args)...));
		}

		/// <summary>
		/// Logs a error message, will only be logged if the loglevel is >= "Error"
		/// </summary>
		/// <typeparam name="...Args">Is any type which has output operator for std::stringstream defined</typeparam>
		/// <param name="str">Is the initial string</param>
		/// <param name="...args">Are arguments to insert into the string</param>
		template<typename... Args>
		void Error(const std::wstring& str, Args&&... args)
		{
			if (!ShouldLog(LogMessageType::Error))
				return;

			Log(FormatMessage(Util::WideCharToMultiByte(str), LogMessageType::Error, std::forward<Args>(args)...));
		}

		/// <summary>
		/// Logs a critical message, will always be logged if the loglevel is not "None"
		/// </summary>
		/// <typeparam name="...Args">Is any type which has output operator for std::stringstream defined</typeparam>
		/// <param name="str">Is the initial string</param>
		/// <param name="...args">Are arguments to insert into the string</param>
		template<typename... Args>
		void Critical(const std::wstring& str, Args&&... args)
		{
			if (!ShouldLog(LogMessageType::Critical))
				return;

			Log(FormatMessage(Util::WideCharToMultiByte(str), LogMessageType::Critical, std::forward<Args>(args)...));
		}

	protected:
		/// <summary>
		/// Writes message to output buffer
		/// </summary>
		/// <param name="message">Is the message to write</param>
		virtual void Log(const std::string& message) = 0;

		/// <summary>
		/// Base Logger Constructor
		/// </summary>
		BaseLogger()
			: m_LogLevel(LogLevel::None)
		{
			BracketFormatter* pBracketFormatter = new BracketFormatter();
			m_Formatters.push_back(pBracketFormatter);

			LogLevelFormatter* pLogLevelFormatter = new LogLevelFormatter();
			m_Formatters.push_back(pLogLevelFormatter);

			DateTimeFormatter* pDateFormatter = new DateTimeFormatter();
			m_Formatters.push_back(pDateFormatter);

			NullTerminatorFormatter* pNullTerminatorFormatter = new NullTerminatorFormatter();
			m_Formatters.push_back(pNullTerminatorFormatter);
		}

		/// <summary>
		/// Virtual BaseLogger Deconstructor
		/// </summary>
		virtual ~BaseLogger()
		{
			for (auto* formatter : m_Formatters)
			{
				if (formatter)
					delete formatter;
			}
		}

		/// <summary>
		/// Takes a correct string possibly containing "{0}", "{1}", ... and fills these in with the corresponding argument
		/// </summary>
		/// <typeparam name="...Args">Is any list of arguments that have a output operator defined</typeparam>
		/// <param name="serializedStr">Is the string which should be serialized</param>
		/// <param name="...args">Are the arguments to insert into the string</param>
		/// <returns>The serialized string</returns>
		template<typename... Args>
		std::string SerializeString(std::string serializedStr, Args&&... args)
		{
			int argCount = 0;
			(SerializeStringArg(serializedStr, args, argCount), ...);
			return serializedStr;
		}

		/// <summary>
		/// Checks if a message with the msgType message type should be logged
		/// </summary>
		/// <param name="msgType">Is the log message type of the message</param>
		/// <returns>True if the message should be logged, false otherwise</returns>
		bool ShouldLog(LogMessageType msgType)
		{
			return msgType >= m_LogLevel;
		}

	private:
		/// <summary>
		/// Inserts one argument into the string, this function overload is called when the argument type is a std::wstring
		/// </summary>
		/// <typeparam name="T">Is any list of arguments that have a output operator defined</typeparam>
		/// <param name="message">Is the string to insert the argument into</param>
		/// <param name="arg">Is the argument to insert</param>
		/// <param name="argCount">Is the current number where the argument will be inserted ({0} = 0, {3} = 3, ...)</param>
		template<typename T>
		void SerializeStringArg(std::string& message, T&& arg, int& argCount, 
			typename std::enable_if_t<std::is_same_v<BaseRefType<T>, std::wstring>>* = 0
		)
		{
			WideCharSerialize(message, arg, argCount);
		}

		/// <summary>
		/// Inserts one argument into the string, this function overload is called when the argument type is convertible to a std::wstring
		/// </summary>
		/// <typeparam name="T">Is any list of arguments that have a output operator defined</typeparam>
		/// <param name="message">Is the string to insert the argument into</param>
		/// <param name="arg">Is the argument to insert</param>
		/// <param name="argCount">Is the current number where the argument will be inserted ({0} = 0, {3} = 3, ...)</param>
		template<typename T>
		void SerializeStringArg(std::string& message, T&& arg, int& argCount, 
			typename std::enable_if_t<std::is_convertible_v<T, std::wstring>>* = 0, 
			typename std::enable_if_t<!std::is_same_v<BaseRefType<T>, std::wstring>>* = 0
		)
		{
			WideCharSerialize(message, arg, argCount);
		}

		/// <summary>
		/// Inserts one argument into the string, this function is called for any type that is not std::wstring and not castable to std::wstring
		/// </summary>
		/// <typeparam name="T">Is any list of arguments that have a output operator defined</typeparam>
		/// <param name="message">Is the string to insert the argument into</param>
		/// <param name="arg">Is the argument to insert</param>
		/// <param name="argCount">Is the current number where the argument will be inserted ({0} = 0, {3} = 3, ...)</param>
		template<typename T>
		void SerializeStringArg(std::string& message, T&& arg, int& argCount,
			typename std::enable_if_t<!std::is_same_v<BaseRefType<T>, std::wstring>>* = 0,
			typename std::enable_if_t<!std::is_convertible_v<T, std::wstring>>* = 0
		)
		{
			MultiByteSerialize(message, arg, argCount);
		}

		/// <summary>
		/// Inserts the argument into the string
		/// </summary>
		/// <typeparam name="T">Is any argument type which should be serialized using wide character strings</typeparam>
		/// <param name="message">Is the string to insert the argument into</param>
		/// <param name="arg">Is the argument to insert</param>
		/// <param name="argCount">Is the current number where the argument will be inserted ({0} = 0, {3} = 3, ...)</param>
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

		/// <summary>
		/// Inserts the argument into the string
		/// </summary>
		/// <typeparam name="T">Is any argument type which should be serialized using multi byte strings</typeparam>
		/// <param name="message">Is the string to insert the argument into</param>
		/// <param name="arg">Is the argument to insert</param>
		/// <param name="argCount">Is the current number where the argument will be inserted ({0} = 0, {3} = 3, ...)</param>
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

		/// <summary>
		/// Formats the message, inserts all arguments and calls the formatters
		/// </summary>
		/// <typeparam name="...Args">Are a list of any typed arguments</typeparam>
		/// <param name="str">Is the base string to insert the arguments into</param>
		/// <param name="...args">Are the arguments to insert</param>
		/// <returns>The formatted string ready for logging</returns>
		template<typename... Args>
		std::string FormatMessage(const std::string& str, LogMessageType msgLvl, Args&&... args)
		{
			std::string msg = SerializeString(str, args...);

			for (auto formatter : m_Formatters)
			{
				formatter->Format(msg, msgLvl);
			}
			return msg;
		}

	protected:
		LogLevel m_LogLevel;
		std::vector<Formatter*> m_Formatters;

	};
}

