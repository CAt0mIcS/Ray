#pragma once

#include "../RBase.h"

#include <chrono>
#include <iomanip>
#include <sstream>


namespace At0::Ray
{
	class RAY_EXPORT Time
	{
	public:
		Time() = default;

		/*
		 * Creates a new time. This function is internal. To construct time values, use {@link
		 * Time::Seconds}, {@link Time::Milliseconds} or {@link Time::Microseconds} instead.
		 * @param duration The duration.
		 */
		template<typename Rep, typename Period>
		constexpr Time(const std::chrono::duration<Rep, Period>& duration)
			: m_Value(std::chrono::duration_cast<std::chrono::microseconds>(duration).count())
		{
		}

		/**
		 * Creates a time value from a number of seconds.
		 * @tparam Rep The type of value to be casted to.
		 * @param seconds Number of seconds.
		 * @return Time value constructed from the amount of seconds.
		 */
		template<typename Rep = float>
		static constexpr Time Seconds(const Rep& seconds)
		{
			return Time(std::chrono::duration<Rep>(seconds));
		}

		/**
		 * Creates a time value from a number of milliseconds
		 * @tparam Rep The type of value to be casted to.
		 * @param milliseconds Number of milliseconds.
		 * @return Time value constructed from the amount of milliseconds.
		 */
		template<typename Rep = float>
		static constexpr Time Milliseconds(const Rep& milliseconds)
		{
			return Time(std::chrono::duration<Rep, std::micro>(milliseconds));
		}

		/**
		 * Creates a time value from a number of microseconds.
		 * @tparam Rep The type of value to be casted to.
		 * @param microseconds Number of microseconds.
		 * @return Time value constructed from the amount of microseconds.
		 */
		template<typename Rep = float>
		static constexpr Time Microseconds(const Rep& microseconds)
		{
			return Time(std::chrono::duration<Rep, std::micro>(microseconds));
		}

		/**
		 * Gets the time value as a number of seconds.
		 * @tparam T The type of value to be casted to.
		 * @return Time in seconds.
		 */
		template<typename T = float>
		constexpr auto AsSeconds() const
		{
			return (T)(m_Value.count()) / (T)(1000000);
		}

		/**
		 * Gets the time value as a number of milliseconds.
		 * @tparam T The type of value to be casted to.
		 * @return Time in milliseconds.
		 */
		template<typename T = float>
		constexpr auto AsMilliseconds() const
		{
			return (T)(m_Value.count()) / (T)(1000);
		}

		/**
		 * Gets the time value as a number of microseconds.
		 * @tparam T The type of value to be casted to.
		 * @return Time in microseconds.
		 */
		template<typename T = float>
		constexpr auto AsMicroseconds() const
		{
			return (T)(m_Value.count());
		}

		/**
		 * Gets the current time of this application.
		 * @return The current time.
		 */
		static Time Now()
		{
			static const auto LocalEpoch = std::chrono::high_resolution_clock::now();
			return std::chrono::duration_cast<std::chrono::microseconds>(
				std::chrono::high_resolution_clock::now() - LocalEpoch);
		}

		/**
		 * Gets the current system time as a string. "%d-%m-%Y %I:%M:%S"
		 * @tparam format The format to put the time into.
		 * @return The date time as a string.
		 */
		static std::string GetDateTime(const std::string& format = "%Y-%m-%d %H:%M:%S")
		{
			auto now = std::chrono::system_clock::now();
			auto timeT = std::chrono::system_clock::to_time_t(now);

			std::stringstream ss;
			ss << std::put_time(std::localtime(&timeT), format.c_str());
			return ss.str();
		}

		template<typename Rep, typename Period>
		constexpr operator std::chrono::duration<Rep, Period>() const
		{
			return std::chrono::duration_cast<std::chrono::duration<Rep, Period>>(m_Value);
		}

		constexpr bool operator==(const Time& rhs) const;
		constexpr bool operator!=(const Time& rhs) const;
		constexpr bool operator<(const Time& rhs) const;
		constexpr bool operator<=(const Time& rhs) const;
		constexpr bool operator>(const Time& rhs) const;
		constexpr bool operator>=(const Time& rhs) const;

		constexpr Time operator-() const;

		friend constexpr Time operator+(const Time& lhs, const Time& rhs);
		friend constexpr Time operator-(const Time& lhs, const Time& rhs);
		friend constexpr Time operator*(const Time& lhs, float rhs);
		friend constexpr Time operator*(const Time& lhs, int64_t rhs);
		friend constexpr Time operator*(float lhs, const Time& rhs);
		friend constexpr Time operator*(int64_t lhs, const Time& rhs);
		friend constexpr Time operator/(const Time& lhs, float rhs);
		friend constexpr Time operator/(const Time& lhs, int64_t rhs);
		friend constexpr float operator/(const Time& lhs, const Time& rhs);

		constexpr Time& operator+=(const Time& rhs);
		constexpr Time& operator-=(const Time& rhs);
		constexpr Time& operator*=(float rhs);
		constexpr Time& operator*=(int64_t rhs);
		constexpr Time& operator/=(float rhs);
		constexpr Time& operator/=(int64_t rhs);

	private:
		std::chrono::microseconds m_Value{};
	};

	/**
	 * Represents the time between two updates
	 */
	class Delta
	{
	public:
		/**
		 * Updates time
		 */
		void Update()
		{
			m_CurrentFrameTime = Time::Now();
			m_Change = m_CurrentFrameTime - m_LastFrameTime;
			m_LastFrameTime = m_CurrentFrameTime;
		}

		/**
		 * Conversion operator for mathematical operations
		 */
		operator float() const { return m_Change.AsMilliseconds(); }

		/**
		 * @returns The deltatime between two updates
		 */
		Time Change() const { return m_Change; }

	private:
		Time m_CurrentFrameTime;
		Time m_LastFrameTime;
		Time m_Change;
	};

	class ChangePerSecond
	{
	public:
		void Update(Time time)
		{
			++m_ValueTemp;

			if (std::floor(time.AsSeconds()) > std::floor(m_ValueTime.AsSeconds()))
			{
				m_Value = m_ValueTemp;
				m_ValueTemp = 0;
			}

			m_ValueTime = time;
		}

		uint32_t Value() const { return m_Value; }

	private:
		uint32_t m_ValueTemp = 0, m_Value = 0;
		Time m_ValueTime;
	};

}  // namespace At0::Ray
