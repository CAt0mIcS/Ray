#pragma once

#include "Ray/RBase.h"
#include "RayBase/RKeyCodes.h"


namespace At0::Ray
{
	class KeyPressedEvent
	{
	public:
		KeyPressedEvent(Key k) : m_Key(k) {}

		Key GetKey() const { return m_Key; }

		bool Handled = false;

	private:
		Key m_Key;
	};

	class KeyReleasedEvent
	{
	public:
		KeyReleasedEvent(Key k) : m_Key(k) {}

		Key GetKey() const { return m_Key; }

		bool Handled = false;

	private:
		Key m_Key;
	};

	class KeyRepeatedEvent
	{
	public:
		KeyRepeatedEvent(Key k, uint32_t repeatCount) : m_Key(k), m_RepeatCount(repeatCount) {}

		Key GetKey() const { return m_Key; }
		uint32_t GetRepeatCount() const { return m_RepeatCount; }

		bool Handled = false;

	private:
		Key m_Key;
		uint32_t m_RepeatCount;
	};

	class CharEvent
	{
	public:
		CharEvent(unsigned char ch) : m_Ch(ch) {}

		unsigned char GetChar() const { return m_Ch; }

		bool Handled = false;

	private:
		unsigned char m_Ch;
	};
}  // namespace At0::Ray
