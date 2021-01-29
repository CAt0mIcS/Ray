#include "Rpch.h"
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

	constexpr Time operator+(const Time& lhs, const Time& rhs) { return lhs.m_Value + rhs.m_Value; }

	constexpr Time operator-(const Time& lhs, const Time& rhs) { return lhs.m_Value - rhs.m_Value; }

	constexpr Time operator*(const Time& lhs, float rhs) { return lhs.m_Value * rhs; }

	constexpr Time operator*(const Time& lhs, int64_t rhs) { return lhs.m_Value * rhs; }

	constexpr Time operator*(float lhs, const Time& rhs) { return rhs * lhs; }

	constexpr Time operator*(int64_t lhs, const Time& rhs) { return rhs * lhs; }

	constexpr Time operator/(const Time& lhs, float rhs) { return lhs.m_Value / rhs; }

	constexpr Time operator/(const Time& lhs, int64_t rhs) { return lhs.m_Value / rhs; }

	constexpr float operator/(const Time& lhs, const Time& rhs)
	{
		return float(lhs.m_Value.count()) / float(rhs.m_Value.count());
	}

	constexpr Time& Time::operator+=(const Time& rhs) { return *this = *this + rhs; }

	constexpr Time& Time::operator-=(const Time& rhs) { return *this = *this - rhs; }

	constexpr Time& Time::operator*=(float rhs) { return *this = *this * rhs; }

	constexpr Time& Time::operator*=(int64_t rhs) { return *this = *this * rhs; }

	constexpr Time& Time::operator/=(float rhs) { return *this = *this / rhs; }

	constexpr Time& Time::operator/=(int64_t rhs) { return *this = *this / rhs; }

}  // namespace At0::Ray