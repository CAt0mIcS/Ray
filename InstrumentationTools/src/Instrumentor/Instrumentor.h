#pragma once

#include <ZealLogger/Log.h>

#include <string>
#include <mutex>
#include <fstream>
#include <sstream>
#include <iomanip>


namespace Zeal::Instrumentation
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
		void BeginSession(const std::string name, const std::string& filepath = "results.json")
		{
			std::scoped_lock lock(m_Mutex);

			// Close session if one already exists
			if (m_CurrentSession)
			{
				//if (Log::Logger::GetFileLogger())
					//ZL_LOG_DEBUG("Instrumentor::BeginSession('{0}') when session '{1}' already open.", name, m_CurrentSession->Name);
				InternalEndSession();
			}

			m_Writer.open(filepath);
			if (m_Writer.is_open())
			{
				m_CurrentSession = new Session({ name });
				WriteHeader();
			}
			else
			{
				//if (Log::Logger::GetFileLogger())
					//ZL_LOG_DEBUG("Instrumentor could not open results file '{0}'.", filepath);
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
				//if (Log::Logger::GetFileLogger())
					//ZL_LOG_DEBUG("[Instrumentor] Timer with function signature '{0}' took '{1}'us", result.Name, result.ElapsedTime.count());
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
			: m_CurrentSession(nullptr) {}

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


