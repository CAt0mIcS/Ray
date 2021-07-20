#include "Rpch.h"
#include "RString.h"

#include "RAssert.h"
#include <RayRenderer/Core/RCore.h>


namespace At0::Ray
{
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t> g_UtfConverter;

	std::string String::Construct(Key key)
	{
		switch (key)
		{
		case Key::Space: return "Space";
		case Key::Apostrophe: return "Apostrophe";
		case Key::Comma: return "Comma";
		case Key::Minus: return "Minus";
		case Key::Period: return "Period";
		case Key::Slash: return "Slash";
		case Key::D0: return "D0";
		case Key::D1: return "D1";
		case Key::D2: return "D2";
		case Key::D3: return "D3";
		case Key::D4: return "D4";
		case Key::D5: return "D5";
		case Key::D6: return "D6";
		case Key::D7: return "D7";
		case Key::D8: return "D8";
		case Key::D9: return "D9";
		case Key::Semicolon: return "Semicolon";
		case Key::Equal: return "Equal";
		case Key::A: return "Args";
		case Key::B: return "B";
		case Key::C: return "c";
		case Key::D: return "D";
		case Key::E: return "E";
		case Key::F: return "F";
		case Key::G: return "G";
		case Key::H: return "H";
		case Key::I: return "I";
		case Key::J: return "J";
		case Key::K: return "K";
		case Key::L: return "L";
		case Key::M: return "M";
		case Key::N: return "N";
		case Key::O: return "O";
		case Key::P: return "P";
		case Key::Q: return "Q";
		case Key::R: return "R";
		case Key::S: return "S";
		case Key::T: return "T";
		case Key::U: return "U";
		case Key::V: return "V";
		case Key::W: return "W";
		case Key::X: return "X";
		case Key::Y: return "Y";
		case Key::Z: return "Z";
		case Key::LeftBracket: return "LeftBracket";
		case Key::Backslash: return "Backslash";
		case Key::RightBracket: return "RightBracket";
		case Key::GraveAccent: return "GraveAccent";
		case Key::World1: return "World1";
		case Key::World2: return "World2";
		case Key::Escape: return "Escape";
		case Key::Enter: return "Enter";
		case Key::Tab: return "Tab";
		case Key::Backspace: return "Backspace";
		case Key::Insert: return "Insert";
		case Key::Delete: return "Delete";
		case Key::Right: return "Right";
		case Key::Left: return "Left";
		case Key::Down: return "Down";
		case Key::Up: return "Up";
		case Key::PageUp: return "PageUp";
		case Key::PageDown: return "PageDown";
		case Key::Home: return "Home";
		case Key::End: return "End";
		case Key::CapsLock: return "CapsLock";
		case Key::ScrollLock: return "ScrollLock";
		case Key::NumLock: return "NumLock";
		case Key::PrintScreen: return "PrintScreen";
		case Key::Pause: return "Pause";
		case Key::F1: return "F1";
		case Key::F2: return "F2";
		case Key::F3: return "F3";
		case Key::F4: return "F4";
		case Key::F5: return "F5";
		case Key::F6: return "F6";
		case Key::F7: return "F7";
		case Key::F8: return "F8";
		case Key::F9: return "F9";
		case Key::F10: return "F10";
		case Key::F11: return "F11";
		case Key::F12: return "F12";
		case Key::F13: return "F13";
		case Key::F14: return "F14";
		case Key::F15: return "F15";
		case Key::F16: return "F16";
		case Key::F17: return "F17";
		case Key::F18: return "F18";
		case Key::F19: return "F19";
		case Key::F20: return "F2";
		case Key::F21: return "F21";
		case Key::F22: return "F22";
		case Key::F23: return "F23";
		case Key::F24: return "F24";
		case Key::F25: return "F25";
		case Key::NumPad0: return "NumPad0";
		case Key::NumPad1: return "NumPad1";
		case Key::NumPad2: return "NumPad2";
		case Key::NumPad3: return "NumPad3";
		case Key::NumPad4: return "NumPad4";
		case Key::NumPad5: return "NumPad5";
		case Key::NumPad6: return "NumPad6";
		case Key::NumPad7: return "NumPad7";
		case Key::NumPad8: return "NumPad8";
		case Key::NumPad9: return "NumPad9";
		case Key::NumPadDecimal: return "NumPadDecimal";
		case Key::NumPadDivide: return "NumPadDivide";
		case Key::NumPadMultiply: return "NumPadMultiply";
		case Key::NumPadSubtract: return "NumPadSubtract";
		case Key::NumPadAdd: return "NumPadAdd";
		case Key::NumPadEnter: return "NumPadEnter";
		case Key::NumPadEqual: return "NumPadEqual";
		case Key::LeftShift: return "LeftShift";
		case Key::LeftControl: return "LeftControl";
		case Key::LeftAlt: return "LeftAlt";
		case Key::LeftSuper: return "LeftSuper";
		case Key::RightShift: return "RightShift";
		case Key::RightControl: return "RightControl";
		case Key::RightAlt: return "RightAlt";
		case Key::RightSuper: return "RightSuper";
		case Key::Menu: return "Menu";
		}

		RAY_ASSERT(false, "[String] Key {0} is invalid", (uint32_t)key);
		return "INVALID KEY";
	}

	std::string String::Construct(MouseButton button)
	{
		switch (button)
		{
		case MouseButton::Button0: return "LeftMouseButton";
		case MouseButton::Button1: return "RightMouseButton";
		case MouseButton::Button2: return "MiddleMouseButton";
		case MouseButton::Button3: return "Button3";
		case MouseButton::Button4: return "Button4";
		case MouseButton::Button5: return "Button5";
		case MouseButton::Button6: return "Button6";
		case MouseButton::Button7: return "Button7";
		}

		RAY_ASSERT(false, "[String] Mouse button {0} is invalid", (uint32_t)button);
		return "INVALID MOUSE BUTTON";
	}

	std::string String::Construct(RrError res)
	{
		switch (res)
		{
		case RrErrorSuccess: return "Success";
		case RrErrorNotReady: return "Vulkan not ready";
		case RrErrorTimeout: return "Vulkan timeout";
		case RrErrorEventSet: return "Vulkan event set";
		case RrErrorEventReset: return "Vulkan event reset";
		case RrErrorIncomplete: return "Vulkan incomplete";
		case RrErrorOutOfHostMemory: return "Vulkan out of host memory";
		case RrErrorOutOfDeviceMemory: return "Vulkan out of device memory";
		case RrErrorInitializationFailed: return "Vulkan initialization failed";
		case RrErrorDeviceLost: return "Vulkan device lost";
		case RrErrorMemoryMapFailed: return "Vulkan memory map failed";
		case RrErrorLayerNotPresent: return "Vulkan layer not present";
		case RrErrorExtensionNotPresent: return "Vulkan extension not present";
		case RrErrorFeatureNotPresent: return "Vulkan feature not present";
		case RrErrorIncompatibleDriver: return "Vulkan incompatible driver";
		case RrErrorTooManyObjects: return "Vulkan too many objects";
		case RrErrorFormatNotSupported: return "Vulkan format not supported";
		case RrErrorFragmentedPool: return "Vulkan fragmented pool";
		case RrErrorUnknown: return "Vulkan error unknown";
		case RrErrorOutOfPoolMemory: return "Vulkan out of pool memory";
		case RrErrorInvalidExternalHandle: return "Vulkan invalid external handle";
		case RrErrorFragmentation: return "Vulkan fragmentation error";
		case RrErrorInvalidOpaqueCaptureAddress: return "Vulkan invalid opaque capture address";
		case RrErrorSurfaceLostKHR: return "Vulkan surface lost";
		case RrErrorNativeWindowInUseKHR: return "Vulkan native window in use";
		case RrErrorSuboptimalKHR: return "Vulkan suboptimal";
		case RrErrorOutOfDateKHR: return "Vulkan out of date";
		case RrErrorIncompatibleDisplayKHR: return "Vulkan incompatible display";
		case RrErrorInvalidShaderNV: return "Vulkan invalid shader";
		case RrErrorInvalidDRMFormatModifierPlaneLayoutEXT:
			return "Vulkan invalid DRM format modifier plane layout";
		case RrErrorNotPermittedEXT: return "Vulkan not permitted";
		case RrErrorFullScreenExclusiveModeLostEXT: return "Vulkan fullscreen exclusive mode lost";
		case RrErrorThreadIdleKHR: return "Vulkan thread idle";
		case RrErrorThreadDoneKHR: return "Vulkan thread done";
		case RrErrorOperationDeferredKHR: return "Vulkan operation deferred";
		case RrErrorOperationNotDeferredKHR: return "Vulkan operation not deferred";
		case RrErrorPipelineCompileRequiredEXT: return "Vulkan pipeline compile required";
		}

		return "Unknown Graphics Error";
	}

	std::string String::Construct(RrPhysicalDeviceType deviceType)
	{
		switch (deviceType)
		{
		case RrPhysicalDeviceTypeIntegratedGPU: return "Integrated GPU";
		case RrPhysicalDeviceTypeDiscreteGPU: return "Discrete GPU";
		case RrPhysicalDeviceTypeVirtualGPU: return "Virtual GPU";
		case RrPhysicalDeviceTypeCPU: return "CPU";
		}

		RAY_ASSERT(false, "[String] Physical Device type {0} is invalid", (uint32_t)deviceType);
		return "Unknown physical device type";
	}

	std::string String::Construct(RrShaderStageFlags stage)
	{
		switch (stage)
		{
		case RrShaderStageVertex: return "Vertex Shader";
		case RrShaderStageTessellationControl: return "Tesselation Control Shader";
		case RrShaderStageTessellationEvaluation: return "Tesselation Evaluation Shader";
		case RrShaderStageGeometry: return "Geometry Shader";
		case RrShaderStageFragment: return "Fragment Shader";
		case RrShaderStageCompute: return "Compute Shader";
		}

		RAY_ASSERT(false, "[String] Shader stage {0} is invalid", (uint32_t)stage);
		return "Unknown shader stage";
	}

	std::string String::ConvertUtf8(std::wstring_view string)
	{
		return g_UtfConverter.to_bytes(string.data(), string.data() + string.length());
	}

	char String::ConvertUtf8(wchar_t ch) { return g_UtfConverter.to_bytes(ch)[0]; }

	std::wstring String::ConvertUtf16(std::string_view string)
	{
		return g_UtfConverter.from_bytes(string.data(), string.data() + string.length());
	}

	wchar_t String::ConvertUtf16(char ch) { return g_UtfConverter.from_bytes(ch)[0]; }

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

	std::vector<std::string> String::Split(std::string str, char sep)
	{
		std::vector<std::string> tokens;

		size_t pos = 0;
		while ((pos = str.find(sep)) != std::string::npos)
		{
			tokens.emplace_back(str.substr(0, pos));
			str.erase(0, pos + 1);
		}

		tokens.emplace_back(str);

		return tokens;
	}

}  // namespace At0::Ray
