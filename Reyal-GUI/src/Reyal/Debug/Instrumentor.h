#pragma once

#include "Reyal/CoreConfig.h"

// class 'std::basic_string<char,std::char_traits<char>,std::allocator<char>>' needs to have dll-interface to be used by clients
#pragma warning(disable : 4564)
// class 'std::chrono::time_point<std::chrono::steady_clock,std::chrono::nanoseconds>' needs to have dll-interface to be used by clients
#pragma warning(disable : 4251)


#if ZL_ENABLE_PROFILING


#include "RlBase.h"

#include <ZealLogger/Log.h>

#include <string>
#include <mutex>
#include <fstream>


namespace Zeal::Reyal
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

	class RL_API Instrumentor
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
		void BeginSession(const std::string_view name, const std::string_view filepath = "results.json");

		/// <summary>
		/// Closes the Session
		/// </summary>
		void EndSession();

		/// <summary>
		/// Writes the profiling result to the output file
		/// </summary>
		/// <param name="result">Is the result to write</param>
		void WriteProfile(const ProfileResult& result);

		/// <summary>
		/// Getter for the Instrumentor instance
		/// </summary>
		/// <returns></returns>
		static Instrumentor& Get();

	private:
		/// <summary>
		/// Private Instrumentor Constructor
		/// </summary>
		Instrumentor();

		/// <summary>
		/// Instrumentor Deconstructor, automatically closes any open sessions
		/// </summary>
		~Instrumentor();

		/// <summary>
		/// Writes the json header to the output file
		/// </summary>
		void WriteHeader();

		/// <summary>
		/// Writes the json footer to the output file
		/// </summary>
		void WriteFooter();

		/// <summary>
		/// Closes the session.
		/// Writes the footer, closes the stream and deletes the current session
		/// </summary>
		void InternalEndSession();

	private:
		std::mutex m_Mutex;
		Session* m_CurrentSession;
		std::ofstream m_Writer;
	};


	class RL_API Timer
	{
	public:
		/// <summary>
		/// Timer Constructor
		/// </summary>
		/// <param name="name">Is the name of the timer</param>
		Timer(const char* name);

		/// <summary>
		/// Timer Deconstructor, automatically stops the current timer
		/// </summary>
		~Timer();

		/// <summary>
		/// Stops the timer, calculates the result and writes it to the session profile file
		/// </summary>
		void Stop();

	private:
		const char* m_Name;
		std::chrono::time_point<std::chrono::steady_clock> m_StartTimepoint;
		bool m_Stopped;
	};
}



/// <summary>
/// Define profiling macros here
/// </summary>
#if defined(__GNUC__) || (defined(__MWERKS__) && (__MWERKS__ >= 0x3000)) || (defined(__ICC) && (__ICC >= 600)) || defined(__ghs__)
#define ZL_FUNC_SIG __PRETTY_FUNCTION__
#elif defined(__DMC__) && (__DMC__ >= 0x810)
#define ZL_FUNC_SIG __PRETTY_FUNCTION__
#elif (defined(__FUNCSIG__) || (_MSC_VER))
#define ZL_FUNC_SIG __FUNCSIG__
#elif (defined(__INTEL_COMPILER) && (__INTEL_COMPILER >= 600)) || (defined(__IBMCPP__) && (__IBMCPP__ >= 500))
#define ZL_FUNC_SIG __FUNCTION__
#elif defined(__BORLANDC__) && (__BORLANDC__ >= 0x550)
#define ZL_FUNC_SIG __FUNC__
#elif defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901)
#define ZL_FUNC_SIG __func__
#elif defined(__cplusplus) && (__cplusplus >= 201103)
#define ZL_FUNC_SIG __func__
#else
#define ZL_FUNC_SIG "ZL_FUNC_SIG unknown!"
#endif


#define ZL_PROFILE_BEGIN_SESSION(name, filepath)	::Zeal::Reyal::Instrumentor::Get().BeginSession(name, filepath)
#define ZL_PROFILE_END_SESSION()					::Zeal::Reyal::Instrumentor::Get().EndSession()
#define ZL_PROFILE_LINE_2(name, line)				::Zeal::Reyal::Timer timer__ZeaL_##line(name)
#define ZL_PROFILE_LINE(name, line)					ZL_PROFILE_LINE_2(name, line)
#define ZL_PROFILE_SCOPE(name)						ZL_PROFILE_LINE(name, __LINE__)
#define ZL_PROFILE_FUNCTION()						ZL_PROFILE_SCOPE(ZL_FUNC_SIG)

#else

#define ZL_PROFILE_BEGIN_SESSION(name, filepath)
#define ZL_PROFILE_END_SESSION()
#define ZL_PROFILE_SCOPE(name)
#define ZL_PROFILE_FUNCTION()

#endif