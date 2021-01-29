#include "Rpch.h"
#include "RTime.h"


namespace At0::Ray
{
	constexpr Time::Time() : m_Seconds{ 0.0f }
	{
	}

	constexpr Time Time::Years(float year)
	{
		return { year * 365 * 24 * 60 * 60 };
	}

	constexpr Time Time::Weeks(float w)
	{
		return { w * 7 * 24 * 60 * 60 };
	}

	constexpr Time Time::Days(float d)
	{
		return { d * 24 * 60 * 60 };
	}

	constexpr Time Time::Hours(float h)
	{
		return { h * 60 * 60 };
	}

	constexpr Time Time::Minutes(float min)
	{
		return { min * 60 };
	}

	constexpr Time Time::Seconds(float secs)
	{
		return { secs };
	}

	constexpr Time Time::Milliseconds(float ms)
	{
		return { ms / 1000.0f };
	}

	constexpr Time Time::Microseconds(float ms)
	{
		return { ms / 1e+6f };
	}

	constexpr Time Time::Nanoseconds(float ns)
	{
		return { ns / 1e+9f };
	}

	Time Time::Now()
	{
		return Time::Nanoseconds(
			std::chrono::high_resolution_clock::now().time_since_epoch().count());
	}

	float Time::AsYears() const
	{
		return m_Seconds / 60.0f / 60.0f / 24.0f / 365.0f;
	}

	float Time::AsWeeks() const
	{
		return m_Seconds / 60.0f / 60.0f / 24.0f / 7.0f;
	}

	float Time::AsDays() const
	{
		return m_Seconds / 60.0f / 60.0f / 24.0f;
	}

	float Time::AsHours() const
	{
		return m_Seconds / 60.0f / 60.0f;
	}

	float Time::AsMinutes() const
	{
		return m_Seconds / 60.0f;
	}

	float Time::AsSeconds() const
	{
		return m_Seconds;
	}

	float Time::AsMilliseconds() const
	{
		return m_Seconds * 1000.0f;
	}

	float Time::AsMicroseconds() const
	{
		return m_Seconds * 1e+6;
	}

	float Time::AsNanoseconds() const
	{
		return m_Seconds * 1e+9;
	}

	Time& Time::operator+=(Time other)
	{
		m_Seconds += other.AsSeconds();
		return *this;
	}

	Time& Time::operator-=(Time other)
	{
		m_Seconds -= other.AsSeconds();
		return *this;
	}

	Time& Time::operator*=(Time other)
	{
		m_Seconds *= other.AsSeconds();
		return *this;
	}

	Time& Time::operator/=(Time other)
	{
		m_Seconds /= other.AsSeconds();
		return *this;
	}

	Time Time::operator+(Time other) const
	{
		return m_Seconds + other.AsSeconds();
	}

	Time Time::operator-(Time other) const
	{
		return m_Seconds - other.AsSeconds();
	}

	Time Time::operator*(Time other) const
	{
		return m_Seconds * other.AsSeconds();
	}

	Time Time::operator/(Time other) const
	{
		return m_Seconds / other.AsSeconds();
	}

	constexpr Time::Time(float secs) : m_Seconds(secs)
	{
	}


	Delta::Delta() : m_LastTime(Time::Now())
	{
	}

	void Delta::Update()
	{
		Time currentFrameTime = Time::Now();
		m_Change = currentFrameTime - m_LastTime;
		m_LastTime = currentFrameTime;
	}

	Delta::operator float() const
	{
		return m_Time.AsMilliseconds();
	}

	Time Delta::Change() const
	{
		return m_Change;
	}
}  // namespace At0::Ray