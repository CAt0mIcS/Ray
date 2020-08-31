#include "Keyboard.h"


namespace NPE
{

	Keyboard::Keyboard()
		: m_AutorepeatEnabled(false)
	{

	}

	Keyboard::KeyboardEvent Keyboard::GetEvent()
	{
		if (m_KeyEventQueue.size() > 0u)
		{
			auto e = m_KeyEventQueue.front();
			m_KeyEventQueue.pop();
			return e;
		}

		return KeyboardEvent();
	}

	void Keyboard::OnKeyPressed(unsigned char keycode)
	{
		m_KeyStates[keycode] = true;
		m_KeyEventQueue.push(KeyboardEvent(KeyboardEvent::KeyboardEventType::KeyDown, keycode));
		TrimBuffer(m_KeyEventQueue);
	}

	void Keyboard::OnKeyReleased(unsigned char keycode)
	{
		m_KeyStates[keycode] = false;
		m_KeyEventQueue.push(KeyboardEvent(KeyboardEvent::KeyboardEventType::KeyUp, keycode));
		TrimBuffer(m_KeyEventQueue);
	}

	void Keyboard::OnChar(unsigned char character)
	{
		m_CharEventQueue.push(character);
		TrimBuffer(m_CharEventQueue);
	}

}
