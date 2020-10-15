#pragma once

#include <string>
#include <mutex>


namespace Zeal::Log
{
	class String
	{
	public:
		String(const std::wstring& str)
			: m_Data(str)
		{

		}

		String& Insert(size_t pos, const wchar_t* data)
		{
			std::scoped_lock lock(m_Mutex);
			m_Data.insert(pos, data);
			return *this;
		}

		size_t Find(const wchar_t* data)
		{

		}

		String& Replace(size_t first, size_t last, const wchar_t* data)
		{
			std::scoped_lock lock(m_Mutex);
			m_Data.replace(m_Data.begin() + first, m_Data.begin() + last, data);
			return *this;
		}

	private:
		std::wstring m_Data;
		std::mutex m_Mutex;
	};
}
