#pragma once

#include "../RBase.h"
#include "../Core/RMath.h"

#include <source_location>


namespace At0::Ray
{
	inline std::ostream& operator<<(std::ostream& os, const Matrix& matrix)
	{
		for (uint32_t row = 0; row < matrix.length(); ++row)
		{
			if (row + 1 < matrix.length())
				os << matrix[row].x << " | " << matrix[row].y << " | " << matrix[row].z << " | "
				   << matrix[row].w << "\n";
			else
				os << matrix[row].x << " | " << matrix[row].y << " | " << matrix[row].z << " | "
				   << matrix[row].w;
		}
		return os;
	}
}  // namespace At0::Ray

#include "../../Extern/Violent/include/Violent/Violent.h"

#include <string_view>


namespace At0::Ray
{
	class RAY_EXPORT Log
	{
	public:
		static void Open(const char* filepath);
		static void Close();
		static void SetLogLevel(Violent::LogLevel lvl);
		static void Flush();

		template<typename... Args>
		struct Trace
		{
			Trace(Args&&... args, std::source_location loc = std::source_location::current())
			{
				s_FileLogger.Trace(std::forward<Args>(args)...);
				s_ConsoleLogger.Trace(std::forward<Args>(args)...);
			}
		};

		template<typename... Args>
		struct Debug
		{
			Debug(Args&&... args, std::source_location loc = std::source_location::current())
			{
				s_FileLogger.Debug(std::forward<Args>(args)...);
				s_ConsoleLogger.Debug(std::forward<Args>(args)...);
			}
		};

		template<typename... Args>
		struct Info
		{
			Info(Args&&... args, std::source_location loc = std::source_location::current())
			{
				s_FileLogger.Info(std::forward<Args>(args)...);
				s_ConsoleLogger.Info(std::forward<Args>(args)...);
			}
		};

		template<typename... Args>
		struct Warn
		{
			Warn(Args&&... args, std::source_location loc = std::source_location::current())
			{
				s_FileLogger.Warn(std::forward<Args>(args)...);
				s_ConsoleLogger.Warn(std::forward<Args>(args)...);
			}
		};

		template<typename... Args>
		struct Error
		{
			Error(Args&&... args, std::source_location loc = std::source_location::current())
			{
				s_FileLogger.Error(std::forward<Args>(args)...);
				s_ConsoleLogger.Error(std::forward<Args>(args)...);
			}
		};

		template<typename... Args>
		struct Critical
		{
			Critical(Args&&... args, std::source_location loc = std::source_location::current())
			{
				s_FileLogger.Critical(std::forward<Args>(args)...);
				s_ConsoleLogger.Critical(std::forward<Args>(args)...);
			}
		};

		template<typename... Args>
		Trace(Args&&...) -> Trace<Args...>;

		template<typename... Args>
		Debug(Args&&...) -> Debug<Args...>;

		template<typename... Args>
		Info(Args&&...) -> Info<Args...>;

		template<typename... Args>
		Warn(Args&&...) -> Warn<Args...>;

		template<typename... Args>
		Error(Args&&...) -> Error<Args...>;

		template<typename... Args>
		Critical(Args&&...) -> Critical<Args...>;

	private:
		static Violent::FileLogger s_FileLogger;
		static Violent::ConsoleLogger s_ConsoleLogger;
	};
}  // namespace At0::Ray
