#pragma once

#include "pch.h"


namespace NPE
{
	class Keyboard
	{
		friend class MainWindow;
	public:
		class KeyboardEvent
		{
		public:
			enum class KeyboardEventType
			{
				INVALID = 0,
				KeyUp, KeyDown
			};

			KeyboardEvent()
				: m_Type(KeyboardEventType::INVALID), m_Code(0u) {}

			KeyboardEvent(const KeyboardEventType type, unsigned char code)
				: m_Type(type), m_Code(code) {}

			unsigned char GetCode() const { return m_Code; }
			bool IsValid() const { return m_Type != KeyboardEventType::INVALID; }
			bool IsPress() const { return m_Type == KeyboardEventType::KeyDown; }
			bool IsRelease() const { return m_Type == KeyboardEventType::KeyUp; }

		private:
			unsigned char m_Code;
			KeyboardEventType m_Type;

		};

	public:
		Keyboard();

		void ClearStates() { m_KeyStates = std::bitset<m_nKeys>(); }

		KeyboardEvent GetEvent();

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
		std::queue<KeyboardEvent> m_KeyEventQueue;
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



