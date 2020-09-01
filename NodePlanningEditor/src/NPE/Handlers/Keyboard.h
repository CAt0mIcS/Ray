#pragma once

#include "pch.h"


namespace NPE
{
	class Keyboard
	{
		friend class MainWindow;
	public:
		class Event
		{
		public:
			enum class Type
			{
				INVALID = 0,
				KeyUp, KeyDown
			};

			Event()
				: m_Type(Type::INVALID), m_Code(0u) {}

			Event(const Type type, unsigned char code)
				: m_Type(type), m_Code(code) {}

			unsigned char GetCode() const { return m_Code; }
			bool IsValid() const { return m_Type != Type::INVALID; }
			bool IsPress() const { return m_Type == Type::KeyDown; }
			bool IsRelease() const { return m_Type == Type::KeyUp; }

			Type GetType() const { return m_Type; }

		private:
			unsigned char m_Code;
			Type m_Type;

		};

	public:
		Keyboard();

		void ClearStates() { m_KeyStates = std::bitset<m_nKeys>(); }

		Event GetEvent();

		bool IsKeyPressed(unsigned char keycode) const { return m_KeyStates[keycode]; }

		bool IsAutorepeatEnabled() const { return m_AutorepeatEnabled; }
		void EnableAutorepeat() { m_AutorepeatEnabled = true; }
		void DisableAutorepeat() { m_AutorepeatEnabled = false; }

	private:
		void OnKeyPressed(unsigned char keycode);
		void OnKeyReleased(unsigned char keycode);
		void OnChar(unsigned char character);

		template<typename T>
		void TrimBuffer(std::queue<T>& buff);

	private:
		static constexpr unsigned int m_nKeys = 256;
		static constexpr unsigned int m_BufferSize = 32;
		bool m_AutorepeatEnabled;
		std::bitset<m_nKeys> m_KeyStates;
		std::queue<Event> m_KeyEventQueue;
		std::queue<char> m_CharEventQueue;

	};

	template<typename T>
	inline void Keyboard::TrimBuffer(std::queue<T>& buff)
	{
		while (buff.size() > m_BufferSize)
		{
			buff.pop();
		}
	}

}



