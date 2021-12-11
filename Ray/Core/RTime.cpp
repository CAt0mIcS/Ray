#include "RTime.h"


namespace At0::Ray
{
	constexpr bool Time::operator==(const Time& rhs) const { return m_Value == rhs.m_Value; }

	constexpr bool Time::operator!=(const Time& rhs) const { return m_Value != rhs.m_Value; }

	constexpr bool Time::operator<(const Time& rhs) const { return m_Value < rhs.m_Value; }

	constexpr bool Time::operator<=(const Time& rhs) const { return m_Value <= rhs.m_Value; }

	constexpr bool Time::operator>(const Time& rhs) const { return m_Value > rhs.m_Value; }

	constexpr bool Time::operator>=(const Time& rhs) const { return m_Value >= rhs.m_Value; }

	constexpr Time Time::operator-() const { return Time(-m_Value); }

	constexpr Time& Time::operator+=(const Time& rhs) { return *this = *this + rhs; }

	constexpr Time& Time::operator-=(const Time& rhs) { return *this = *this - rhs; }

	constexpr Time& Time::operator*=(float rhs) { return *this = *this * rhs; }

	constexpr Time& Time::operator*=(int64_t rhs) { return *this = *this * rhs; }

	constexpr Time& Time::operator/=(float rhs) { return *this = *this / rhs; }

	constexpr Time& Time::operator/=(int64_t rhs) { return *this = *this / rhs; }

}  // namespace At0::Ray