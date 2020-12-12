#pragma once

#include <stdint.h>
#include <string>


namespace At0::Ray
{
	enum class MouseButton : uint16_t
	{
		Button0 = 0,
		Button1 = 1,
		Button2 = 2,
		Button3 = 3,
		Button4 = 4,
		Button5 = 5,
		Button6 = 6,
		Button7 = 7,

		Left = Button0,
		Right = Button1,
		Middle = Button2,

		FIRST = Button0, LAST = Button7
	};


	enum class Key : uint16_t
	{
		// From glfw3.h
		Space = 32,
		Apostrophe = 39, /* ' */
		Comma = 44, /* , */
		Minus = 45, /* - */
		Period = 46, /* . */
		Slash = 47, /* / */
	
		D0 = 48, /* 0 */
		D1 = 49, /* 1 */
		D2 = 50, /* 2 */
		D3 = 51, /* 3 */
		D4 = 52, /* 4 */
		D5 = 53, /* 5 */
		D6 = 54, /* 6 */
		D7 = 55, /* 7 */
		D8 = 56, /* 8 */
		D9 = 57, /* 9 */
	
		Semicolon = 59, /* ; */
		Equal = 61, /* = */
	
		A = 65,
		B = 66,
		C = 67,
		D = 68,
		E = 69,
		F = 70,
		G = 71,
		H = 72,
		I = 73,
		J = 74,
		K = 75,
		L = 76,
		M = 77,
		N = 78,
		O = 79,
		P = 80,
		Q = 81,
		R = 82,
		S = 83,
		T = 84,
		U = 85,
		V = 86,
		W = 87,
		X = 88,
		Y = 89,
		Z = 90,
	
		LeftBracket = 91,  /* [ */
		Backslash = 92,  /* \ */
		RightBracket = 93,  /* ] */
		GraveAccent = 96,  /* ` */
	
		World1 = 161, /* non-US #1 */
		World2 = 162, /* non-US #2 */
	
		/* Function keys */
		Escape = 256,
		Enter = 257,
		Tab = 258,
		Backspace = 259,
		Insert = 260,
		Delete = 261,
		Right = 262,
		Left = 263,
		Down = 264,
		Up = 265,
		PageUp = 266,
		PageDown = 267,
		Home = 268,
		End = 269,
		CapsLock = 280,
		ScrollLock = 281,
		NumLock = 282,
		PrintScreen = 283,
		Pause = 284,
		F1 = 290,
		F2 = 291,
		F3 = 292,
		F4 = 293,
		F5 = 294,
		F6 = 295,
		F7 = 296,
		F8 = 297,
		F9 = 298,
		F10 = 299,
		F11 = 300,
		F12 = 301,
		F13 = 302,
		F14 = 303,
		F15 = 304,
		F16 = 305,
		F17 = 306,
		F18 = 307,
		F19 = 308,
		F20 = 309,
		F21 = 310,
		F22 = 311,
		F23 = 312,
		F24 = 313,
		F25 = 314,
	
		/* Keypad */
		NumPad0 = 320,
		NumPad1 = 321,
		NumPad2 = 322,
		NumPad3 = 323,
		NumPad4 = 324,
		NumPad5 = 325,
		NumPad6 = 326,
		NumPad7 = 327,
		NumPad8 = 328,
		NumPad9 = 329,
		NumPadDecimal = 330,
		NumPadDivide = 331,
		NumPadMultiply = 332,
		NumPadSubtract = 333,
		NumPadAdd = 334,
		NumPadEnter = 335,
		NumPadEqual = 336,
	
		LeftShift = 340,
		LeftControl = 341,
		LeftAlt = 342,
		LeftSuper = 343,
		RightShift = 344,
		RightControl = 345,
		RightAlt = 346,
		RightSuper = 347,
		Menu = 348
	};

	static const char* KeyToString(Key k)
	{
		switch (k)
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
		case Key::F20:				return "F2"	;
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

		return "";
	}

	static const char* MouseButtonToString(MouseButton btn)
	{
		switch (btn)
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

		return "";
	}
}