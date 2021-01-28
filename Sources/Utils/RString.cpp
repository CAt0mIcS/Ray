#include "Rpch.h"
#include "RString.h"


namespace At0::Ray
{
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t> g_UtfConverter;

	std::string String::Construct(Key key)
	{
		switch (key)
		{
		case Key::Space:			return "Space";
		case Key::Apostrophe:		return "Apostrophe";
		case Key::Comma:			return "Comma";
		case Key::Minus:			return "Minus";
		case Key::Period:			return "Period";
		case Key::Slash:			return "Slash";
		case Key::D0:				return "D0";
		case Key::D1:				return "D1";
		case Key::D2:				return "D2";
		case Key::D3:				return "D3";
		case Key::D4:				return "D4";
		case Key::D5:				return "D5";
		case Key::D6:				return "D6";
		case Key::D7:				return "D7";
		case Key::D8:				return "D8";
		case Key::D9:				return "D9";
		case Key::Semicolon:		return "Semicolon";
		case Key::Equal:			return "Equal";
		case Key::A:				return "A";
		case Key::B:				return "B";
		case Key::C:				return "c";
		case Key::D:				return "D";
		case Key::E:				return "E";
		case Key::F:				return "F";
		case Key::G:				return "G";
		case Key::H:				return "H";
		case Key::I:				return "I";
		case Key::J:				return "J";
		case Key::K:				return "K";
		case Key::L:				return "L";
		case Key::M:				return "M";
		case Key::N:				return "N";
		case Key::O:				return "O";
		case Key::P:				return "P";
		case Key::Q:				return "Q";
		case Key::R:				return "R";
		case Key::S:				return "S";
		case Key::T:				return "T";
		case Key::U:				return "U";
		case Key::V:				return "V";
		case Key::W:				return "W";
		case Key::X:				return "X";
		case Key::Y:				return "Y";
		case Key::Z:				return "Z";
		case Key::LeftBracket:		return "LeftBracket";
		case Key::Backslash:		return "Backslash";
		case Key::RightBracket:		return "RightBracket";
		case Key::GraveAccent:		return "GraveAccent";
		case Key::World1:			return "World1";
		case Key::World2:			return "World2";
		case Key::Escape:			return "Escape";
		case Key::Enter:			return "Enter";
		case Key::Tab:				return "Tab";
		case Key::Backspace:		return "Backspace";
		case Key::Insert:			return "Insert";
		case Key::Delete:			return "Delete";
		case Key::Right:			return "Right";
		case Key::Left:				return "Left";
		case Key::Down:				return "Down";
		case Key::Up:				return "Up";
		case Key::PageUp:			return "PageUp";
		case Key::PageDown:			return "PageDown";
		case Key::Home:				return "Home";
		case Key::End:				return "End";
		case Key::CapsLock:			return "CapsLock";
		case Key::ScrollLock:		return "ScrollLock";
		case Key::NumLock:			return "NumLock";
		case Key::PrintScreen:		return "PrintScreen";
		case Key::Pause:			return "Pause";
		case Key::F1:				return "F1";
		case Key::F2:				return "F2";
		case Key::F3:				return "F3";
		case Key::F4:				return "F4";
		case Key::F5:				return "F5";
		case Key::F6:				return "F6";
		case Key::F7:				return "F7";
		case Key::F8:				return "F8";
		case Key::F9:				return "F9";
		case Key::F10:				return "F10";
		case Key::F11:				return "F11";
		case Key::F12:				return "F12";
		case Key::F13:				return "F13";
		case Key::F14:				return "F14";
		case Key::F15:				return "F15";
		case Key::F16:				return "F16";
		case Key::F17:				return "F17";
		case Key::F18:				return "F18";
		case Key::F19:				return "F19";
		case Key::F20:				return "F2";
		case Key::F21:				return "F21";
		case Key::F22:				return "F22";
		case Key::F23:				return "F23";
		case Key::F24:				return "F24";
		case Key::F25:				return "F25";
		case Key::NumPad0:			return "NumPad0";
		case Key::NumPad1:			return "NumPad1";
		case Key::NumPad2:			return "NumPad2";
		case Key::NumPad3:			return "NumPad3";
		case Key::NumPad4:			return "NumPad4";
		case Key::NumPad5:			return "NumPad5";
		case Key::NumPad6:			return "NumPad6";
		case Key::NumPad7:			return "NumPad7";
		case Key::NumPad8:			return "NumPad8";
		case Key::NumPad9:			return "NumPad9";
		case Key::NumPadDecimal:	return "NumPadDecimal";
		case Key::NumPadDivide:		return "NumPadDivide";
		case Key::NumPadMultiply:	return "NumPadMultiply";
		case Key::NumPadSubtract:	return "NumPadSubtract";
		case Key::NumPadAdd:		return "NumPadAdd";
		case Key::NumPadEnter:		return "NumPadEnter";
		case Key::NumPadEqual:		return "NumPadEqual";
		case Key::LeftShift:		return "LeftShift";
		case Key::LeftControl:		return "LeftControl";
		case Key::LeftAlt:			return "LeftAlt";
		case Key::LeftSuper:		return "LeftSuper";
		case Key::RightShift:		return "RightShift";
		case Key::RightControl:		return "RightControl";
		case Key::RightAlt:			return "RightAlt";
		case Key::RightSuper:		return "RightSuper";
		case Key::Menu:				return "Menu";
		}

		return "INVALID KEY";
	}

	std::string String::Construct(MouseButton button)
	{
		switch (button)
		{
		case MouseButton::Button0:	return "LeftMouseButton";
		case MouseButton::Button1:	return "RightMouseButton";
		case MouseButton::Button2:	return "MiddleMouseButton";
		case MouseButton::Button3:	return "Button3";
		case MouseButton::Button4:	return "Button4";
		case MouseButton::Button5:	return "Button5";
		case MouseButton::Button6:	return "Button6";
		case MouseButton::Button7:	return "Button7";
		}

		return "INVALID MOUSE BUTTON";
	}

	std::string String::ConvertUtf8(std::wstring_view string)
	{
		return g_UtfConverter.to_bytes(string.data(), string.data() + string.length());
	}

	char String::ConvertUtf8(wchar_t ch)
	{
		return g_UtfConverter.to_bytes(ch)[0];
	}

	std::wstring String::ConvertUtf16(std::string_view string)
	{
		return g_UtfConverter.from_bytes(string.data(), string.data() + string.length());
	}

	wchar_t String::ConvertUtf16(char ch)
	{
		return g_UtfConverter.from_bytes(ch)[0];
	}

	bool String::StartsWith(std::string_view str, std::string_view token)
	{
		if (token.length() > str.length())
			return false;

		return str.compare(0, token.length(), token) == 0;
	}

	bool String::EndsWith(std::string_view str, std::string_view token)
	{
		if (token.length() > str.length())
			return false;

		return str.compare(str.length() - token.length(), token.length(), token) == 0;
	}

	bool String::Contains(std::string_view str, std::string_view token)
	{
		return str.find(token) != std::string::npos;
	}

}
