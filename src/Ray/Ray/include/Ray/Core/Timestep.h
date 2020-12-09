#pragma once



namespace At0::Ray
{
	class Timestep
	{
	public:
		/// <summary>
		/// Timestep Constructor
		/// </summary>
		/// <param name="time">Is the timestep time in seconds</param>
		constexpr Timestep(float time = 0.0f)
			: m_Time(time) { }

		/// <summary>
		/// Conversion operator to float, returns the current time in seconds
		/// </summary>
		constexpr operator float() { return m_Time; }

		/// <summary>
		/// Getter for the deltatime in milliseconds
		/// </summary>
		/// <returns>The time in milliseconds</returns>
		constexpr float GetMilliseconds() const { return m_Time * 1000.0f; }
		
		/// <summary>
		/// Getter for the deltatime in seconds
		/// </summary>
		/// <returns>The time in seconds</returns>
		constexpr float GetSeconds() const { return m_Time; }

	private:
		float m_Time;
	};
}