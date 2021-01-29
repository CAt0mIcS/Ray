#pragma once

#include "../RBase.h"

#include <chrono>


namespace At0::Ray
{
	class RAY_EXPORT Time
	{
	public:
		constexpr Time();

		/**
		 * Time object from years
		 */
		static constexpr Time Years(float year);

		/**
		 * Time object from weeks
		 */
		static constexpr Time Weeks(float w);

		/**
		 * Time object from days
		 */
		static constexpr Time Days(float d);

		/**
		 * Time object from hours
		 */
		static constexpr Time Hours(float h);

		/**
		 * Time object from minutes
		 */
		static constexpr Time Minutes(float min);

		/**
		 * Time object from seconds
		 */
		static constexpr Time Seconds(float secs);

		/**
		 * Time object from milliseconds
		 */
		static constexpr Time Milliseconds(float ms);

		/**
		 * Time object from microseconds
		 */
		static constexpr Time Microseconds(float ms);

		/**
		 * Time object from nanoseconds
		 */
		static constexpr Time Nanoseconds(float ns);

		/**
		 * Current time since epoch
		 */
		static Time Now();

		float AsYears() const;
		float AsWeeks() const;
		float AsDays() const;
		float AsHours() const;
		float AsMinutes() const;
		float AsSeconds() const;
		float AsMilliseconds() const;
		float AsMicroseconds() const;
		float AsNanoseconds() const;

		Time& operator+=(Time other);
		Time& operator-=(Time other);
		Time& operator*=(Time other);
		Time& operator/=(Time other);
		Time operator+(Time other) const;
		Time operator-(Time other) const;
		Time operator*(Time other) const;
		Time operator/(Time other) const;

	private:
		constexpr Time(float secs);

	private:
		float m_Seconds;
	};

	/**
	 * Represents the time between two updates
	 */
	class Delta
	{
	public:
		Delta();

		/**
		 * Updates time
		 */
		void Update();

		/**
		 * Conversion operator for mathematical operations
		 */
		operator float() const;

		/**
		 * @returns The deltatime between two updates
		 */
		Time Change() const;

	private:
		Time m_Time;
		Time m_LastTime;
		Time m_Change;
	};

}  // namespace At0::Ray
