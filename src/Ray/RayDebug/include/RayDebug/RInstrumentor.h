#pragma once

#include <../../RayUtil/include/RayUtil/RCoreConfig.h>

// class 'std::basic_string<char,std::char_traits<char>,std::allocator<char>>' needs to have dll-interface to be used by clients
#pragma warning(disable : 4564)
// class 'std::chrono::time_point<std::chrono::steady_clock,std::chrono::nanoseconds>' needs to have dll-interface to be used by clients
#pragma warning(disable : 4251)

#include <string>


#if RAY_ENABLE_PROFILING

#include "RLogger.h"

#include <mutex>
#include <fstream>
#include <thread>
#include <chrono>
#include <sstream>
#include <iomanip>
#include <filesystem>


namespace At0::Ray::Debug
{
	struct Session
	{
		std::string Name;
	};

	struct ProfileResult
	{
		std::string Name;

		std::chrono::duration<double, std::micro> Start;
		std::chrono::microseconds ElapsedTime;
		std::thread::id ThreadID;
	};

	class Instrumentor
	{
	public:
		/// <summary>
		/// Deleted Copy Constructor
		/// </summary>
		Instrumentor(const Instrumentor&) = delete;

		/// <summary>
		/// Deleted Copy-Asignment operator
		/// </summary>
		/// <param name=""></param>
		Instrumentor(Instrumentor&&) = delete;

		/// <summary>
		/// Starts a session, closes the previous session if one was already open
		/// </summary>
		/// <param name="name">Is the session name</param>
		/// <param name="filepath">Is a path to a file to export the profile result to</param>
		void BeginSession(std::string_view name, std::string_view filepath = "results.json")
		{
			std::scoped_lock lock(m_Mutex);

			std::string dirPath = filepath.data();
			size_t pos = dirPath.find_last_of("\\/");
			if (pos != std::string::npos)
				dirPath.erase(dirPath.begin() + pos, dirPath.end());
			std::filesystem::create_directories(dirPath);

			// Close session if one already exists
			if (m_CurrentSession)
			{
				if (Log::IsOpen())
					Log::Debug("Instrumentor::BeginSession('{0}') when session '{1}' already open.", name, m_CurrentSession->Name);
				InternalEndSession();
			}

			m_Writer.open(filepath.data());
			if (m_Writer.is_open())
			{
				m_CurrentSession = new Session{ name.data() };
				WriteHeader();
			}
			else
			{
				if (Log::IsOpen())
					Log::Debug("Instrumentor could not open results file '{0}'.", filepath);
			}
		}

		/// <summary>
		/// Closes the Session
		/// </summary>
		void EndSession()
		{
			std::scoped_lock lock(m_Mutex);
			InternalEndSession();
		}

		/// <summary>
		/// Writes the profiling result to the output file
		/// </summary>
		/// <param name="result">Is the result to write</param>
		void WriteProfile(const ProfileResult& result)
		{
			std::ostringstream json;

			json << std::setprecision(3) << std::fixed;
			json << ",{";
			json << "\"cat\":\"function\",";
			json << "\"dur\":" << (result.ElapsedTime.count()) << ',';
			json << "\"name\":\"" << result.Name << "\",";
			json << "\"ph\":\"X\",";
			json << "\"pid\":0,";
			json << "\"tid\":" << result.ThreadID << ",";
			json << "\"ts\":" << result.Start.count();
			json << "}";

			std::scoped_lock lock(m_Mutex);
			if (m_CurrentSession)
			{
				m_Writer << json.str();
				m_Writer.flush();
			}
			else
			{
				if (Log::IsOpen())
					Log::Debug("[Instrumentor] Timer with function signature '{0}' took '{1}'us", result.Name, result.ElapsedTime.count());
			}
		}

		/// <summary>
		/// Getter for the Instrumentor instance
		/// </summary>
		/// <returns></returns>
		static Instrumentor& Get()
		{
			static Instrumentor instance;
			return instance;
		}

	private:
		/// <summary>
		/// Private Instrumentor Constructor
		/// </summary>
		Instrumentor()
			: m_CurrentSession(nullptr)
		{

		}

		/// <summary>
		/// Instrumentor Deconstructor, automatically closes any open sessions
		/// </summary>
		~Instrumentor()
		{
			EndSession();
		}

		/// <summary>
		/// Writes the json header to the output file
		/// </summary>
		void WriteHeader()
		{
			m_Writer << "{\"otherData\": {},\"traceEvents\":[{}";
			m_Writer.flush();
		}

		/// <summary>
		/// Writes the json footer to the output file
		/// </summary>
		void WriteFooter()
		{
			m_Writer << "]}";
			m_Writer.flush();
		}

		/// <summary>
		/// Closes the session.
		/// Writes the footer, closes the stream and deletes the current session
		/// </summary>
		void InternalEndSession()
		{
			if (m_CurrentSession)
			{
				WriteFooter();
				m_Writer.close();
				delete m_CurrentSession;
				m_CurrentSession = nullptr;
			}
		}

	private:
		std::mutex m_Mutex;
		Session* m_CurrentSession;
		std::ofstream m_Writer;
	};


	class Timer
	{
	public:
		/// <summary>
		/// Timer Constructor
		/// </summary>
		/// <param name="name">Is the name of the timer</param>
		Timer(const char* name)
			: m_Name(name), m_Stopped(false)
		{
			m_StartTimepoint = std::chrono::steady_clock::now();
		}

		/// <summary>
		/// Timer Deconstructor, automatically stops the current timer
		/// </summary>
		~Timer()
		{
			if (!m_Stopped)
				Stop();
		}

		/// <summary>
		/// Stops the timer, calculates the result and writes it to the session profile file
		/// </summary>
		void Stop()
		{
			auto endPoint = std::chrono::steady_clock::now();
			auto highResStart = std::chrono::duration<double, std::micro>{ m_StartTimepoint.time_since_epoch() };
			auto elapsedTime = std::chrono::time_point_cast<std::chrono::microseconds>(endPoint).time_since_epoch() - std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimepoint).time_since_epoch();

			Instrumentor::Get().WriteProfile({ m_Name, highResStart, elapsedTime, std::this_thread::get_id() });
			m_Stopped = true;
		}

	private:
		const char* m_Name;
		std::chrono::time_point<std::chrono::steady_clock> m_StartTimepoint;
		bool m_Stopped;
	};
}

#endif

/// <summary>
/// Define profiling macros here
/// </summary>
#if defined(__GNUC__) || (defined(__MWERKS__) && (__MWERKS__ >= 0x3000)) || (defined(__ICC) && (__ICC >= 600)) || defined(__ghs__)
#define RAY_FUNC_SIG __PRETTY_FUNCTION__
#elif defined(__DMC__) && (__DMC__ >= 0x810)
#define RAY_FUNC_SIG __PRETTY_FUNCTION__
#elif (defined(__FUNCSIG__) || (_MSC_VER))
#define RAY_FUNC_SIG __FUNCSIG__
#elif (defined(__INTEL_COMPILER) && (__INTEL_COMPILER >= 600)) || (defined(__IBMCPP__) && (__IBMCPP__ >= 500))
#define RAY_FUNC_SIG __FUNCTION__
#elif defined(__BORLANDC__) && (__BORLANDC__ >= 0x550)
#define RAY_FUNC_SIG __FUNC__
#elif defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901)
#define RAY_FUNC_SIG __func__
#elif defined(__cplusplus) && (__cplusplus >= 201103)
#define RAY_FUNC_SIG __func__
#else
#define RAY_FUNC_SIG "RAY_FUNC_SIG unknown!"
#endif

#if RAY_ENABLE_PROFILING

namespace At0::Ray::Profile
{
	inline void BeginSession(std::string_view sessionName, std::string_view filepath)
	{
		::At0::Ray::Debug::Instrumentor::Get().BeginSession(sessionName, filepath);
	}

	inline void EndSession()
	{
		::At0::Ray::Debug::Instrumentor::Get().EndSession();
	}
}

#define RAY_PROFILE_LINE_2(name, line)				::At0::Ray::Debug::Timer timer__ZeaL_##line(name)
#define RAY_PROFILE_LINE(name, line)				RAY_PROFILE_LINE_2(name, line)
#define RAY_PROFILE_SCOPE(name)						RAY_PROFILE_LINE(name, __LINE__)
#define RAY_PROFILE_FUNCTION()						RAY_PROFILE_SCOPE(RAY_FUNC_SIG)

#else

namespace At0::Ray::Profile
{
	inline void BeginSession(std::string_view sessionName, std::string_view filepath) {}

	inline void EndSession() {}
}

#define RAY_PROFILE_BEGIN_SESSION(name, filepath)
#define RAY_PROFILE_END_SESSION()
#define RAY_PROFILE_SCOPE(name)
#define RAY_PROFILE_FUNCTION()

#endif

