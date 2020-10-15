#pragma once

#include "Formatter.h"
#include <chrono>

namespace Zeal::Log
{
	class DateFormatter : public Formatter
	{
	public:
		DateFormatter() = default;

		virtual void Format(std::wstring& str, LogLevel logLvl) override
		{
#ifndef ZEAL_NO_DATE_OUT
			static constexpr unsigned int s_InsertIdx = 5;


			//time_t rawTime;
			//struct tm timeInfo;
			//wchar_t buff[80];

			//time(&rawTime);
			//localtime_s(&timeInfo, &rawTime);

			//wcsftime(buff, sizeof(buff), L"%d-%m-%Y %H:%M:%S", &timeInfo);

			//auto ms = (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()) % 1000).count();
			//std::wostringstream oss;
			//if (ms < 10)
			//{
			//	oss << buff << L":00" << ms;
			//}
			//else if (ms < 100)
			//{
			//	oss << buff << L":0" << ms;
			//}
			//else
			//{
			//	oss << buff << L':' << ms;
			//}

			//str.insert(s_InsertIdx, oss.str());

			//std::scoped_lock lock(m_Mutex);
			str.insert(s_InsertIdx, L"DummyData");

#endif // !ZEAL_NO_DATE_OUT

		}
	};
}

