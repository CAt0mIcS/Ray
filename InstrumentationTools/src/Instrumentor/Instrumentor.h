#pragma once


#include <string>
#include <mutex>
#include <fstream>
#include <sstream>
#include <iomanip>

#include <ZealLogger/ZealLog.h>



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
		Instrumentor(const Instrumentor&) = delete;
		Instrumentor(Instrumentor&&) = delete;

		void BeginSession(const std::string name, const std::string& filepath = "results.json")
		{
			std::scoped_lock lock(m_Mutex);
			
			// Close session if one already exists
			if (m_CurrentSession)
			{
				ZL_LOG_ERROR("Instrumentor::BeginSession('{0}') when session '{1}' already open.", name, m_CurrentSession->Name);
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
				ZL_LOG_ERROR("Instrumentor could not open results file '{0}'.", filepath);
			}
		}

		void EndSession()
		{
			std::scoped_lock lock(m_Mutex);
			InternalEndSession();
		}

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
				ZL_LOG_DEBUG("[Instrumentor] Timer with function signature '{0}' took '{1}'us", result.Name, result.ElapsedTime.count());
			}
		}

		static Instrumentor& Get()
		{
			static Instrumentor instance;
			return instance;
		}

	private:
		Instrumentor()
			: m_CurrentSession(nullptr) {}

		~Instrumentor()
		{
			EndSession();
		}

		void WriteHeader()
		{
			m_Writer << "{\"otherData\": {},\"traceEvents\":[{}";
			m_Writer.flush();
		}

		void WriteFooter()
		{
			m_Writer << "]}";
			m_Writer.flush();
		}

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
		Timer(const char* name)
			: m_Name(name), m_Stopped(false)
		{
			m_StartTimepoint = std::chrono::steady_clock::now();
		}

		~Timer()
		{
			if (!m_Stopped)
				Stop();
		}

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


