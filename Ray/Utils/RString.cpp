#include "RString.h"

#include "Graphics/Pipelines/Shader/RShader.h"

#include <vulkan/vulkan_core.h>


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

	std::string String::Construct(VkResult res)
	{
		switch (res)
		{
		case VK_SUCCESS: return "Vulkan Success";
		case VK_NOT_READY: return "Vulkan not ready";
		case VK_TIMEOUT: return "Vulkan timeout";
		case VK_EVENT_SET: return "Vulkan event set";
		case VK_EVENT_RESET: return "Vulkan event reset";
		case VK_INCOMPLETE: return "Vulkan incomplete";
		case VK_ERROR_OUT_OF_HOST_MEMORY: return "Vulkan out of host memory";
		case VK_ERROR_OUT_OF_DEVICE_MEMORY: return "Vulkan out of device memory";
		case VK_ERROR_INITIALIZATION_FAILED: return "Vulkan initialization failed";
		case VK_ERROR_DEVICE_LOST: return "Vulkan device lost";
		case VK_ERROR_MEMORY_MAP_FAILED: return "Vulkan memory map failed";
		case VK_ERROR_LAYER_NOT_PRESENT: return "Vulkan layer not present";
		case VK_ERROR_EXTENSION_NOT_PRESENT: return "Vulkan extension not present";
		case VK_ERROR_FEATURE_NOT_PRESENT: return "Vulkan feature not present";
		case VK_ERROR_INCOMPATIBLE_DRIVER: return "Vulkan incompatible driver";
		case VK_ERROR_TOO_MANY_OBJECTS: return "Vulkan too many objects";
		case VK_ERROR_FORMAT_NOT_SUPPORTED: return "Vulkan format not supported";
		case VK_ERROR_FRAGMENTED_POOL: return "Vulkan fragmented pool";
		case VK_ERROR_UNKNOWN: return "Vulkan error unknown";
		case VK_ERROR_OUT_OF_POOL_MEMORY: return "Vulkan out of pool memory";
		case VK_ERROR_INVALID_EXTERNAL_HANDLE: return "Vulkan invalid external handle";
		case VK_ERROR_FRAGMENTATION: return "Vulkan fragmentation error";
		case VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS:
			return "Vulkan invalid opaque capture address";
		case VK_ERROR_SURFACE_LOST_KHR: return "Vulkan surface lost";
		case VK_ERROR_NATIVE_WINDOW_IN_USE_KHR: return "Vulkan native window in use";
		case VK_SUBOPTIMAL_KHR: return "Vulkan suboptimal";
		case VK_ERROR_OUT_OF_DATE_KHR: return "Vulkan out of date";
		case VK_ERROR_INCOMPATIBLE_DISPLAY_KHR: return "Vulkan incompatible display";
		case VK_ERROR_INVALID_SHADER_NV: return "Vulkan invalid shader";
		case VK_ERROR_INVALID_DRM_FORMAT_MODIFIER_PLANE_LAYOUT_EXT:
			return "Vulkan invalid DRM format modifier plane layout";
		case VK_ERROR_NOT_PERMITTED_EXT: return "Vulkan not permitted";
		case VK_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT:
			return "Vulkan fullscreen exclusive mode lost";
		case VK_THREAD_IDLE_KHR: return "Vulkan thread idle";
		case VK_THREAD_DONE_KHR: return "Vulkan thread done";
		case VK_OPERATION_DEFERRED_KHR: return "Vulkan operation deferred";
		case VK_OPERATION_NOT_DEFERRED_KHR: return "Vulkan operation not deferred";
		case VK_PIPELINE_COMPILE_REQUIRED_EXT: return "Vulkan pipeline compile required";
		}

		return "UNKNOWN VULKAN RESULT CODE";
	}

	std::string String::Construct(ShaderStage stage)
	{
		switch (stage)
		{
		case ShaderStage::Vertex: return "Vertex Shader";
		case ShaderStage::TesselationControl: return "Tesselation Control Shader";
		case ShaderStage::TesselationEvaluation: return "Tesselation Evaluation Shader";
		case ShaderStage::Geometry: return "Geometry Shader";
		case ShaderStage::Fragment: return "Fragment Shader";
		case ShaderStage::Compute: return "Compute Shader";
		}

		RAY_ASSERT(false, "[String] Shader stage {0} is invalid", (uint32_t)stage);
		return "INVALID SHADER STAGE";
	}

	std::string String::Construct(VkPhysicalDeviceType deviceType)
	{
		switch (deviceType)
		{
		case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU: return "Integrated GPU";
		case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU: return "Discrete GPU";
		case VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU: return "Virtual GPU";
		case VK_PHYSICAL_DEVICE_TYPE_CPU: return "CPU";
		}

		RAY_ASSERT(false, "[String] Physical Device type {0} is invalid", (uint32_t)deviceType);
		return "INVALID PHYSICAL DEVICE TYPE";
	}

	std::string String::Construct(ShaderDataType type)
	{
		switch (type)
		{
		case ShaderDataType::Float: return "float";
		case ShaderDataType::Int: return "int";
		case ShaderDataType::UInt: return "uint";
		case ShaderDataType::Bool: return "bool";
		case ShaderDataType::Vec2: return "vec2";
		case ShaderDataType::IVec2: return "ivec2";
		case ShaderDataType::UVec2: return "uvec2";
		case ShaderDataType::BVec2: return "bvec2";
		case ShaderDataType::Double: return "double";
		case ShaderDataType::Vec3: return "vec3";
		case ShaderDataType::IVec3: return "ivec3";
		case ShaderDataType::UVec3: return "uvec3";
		case ShaderDataType::BVec3: return "bvec3";
		case ShaderDataType::Vec4: return "vec4";
		case ShaderDataType::IVec4: return "ivec4";
		case ShaderDataType::UVec4: return "uvec4";
		case ShaderDataType::BVec4: return "bvec4";
		case ShaderDataType::DVec2: return "dvec2";
		case ShaderDataType::DVec3: return "dvec3";
		case ShaderDataType::DVec4: return "dvec4";
		case ShaderDataType::Mat2: return "mat2";
		case ShaderDataType::Mat2x3: return "mat2x3";
		case ShaderDataType::Mat2x4: return "mat2x4";
		case ShaderDataType::Mat3: return "mat3";
		case ShaderDataType::Mat3x2: return "mat3x2";
		case ShaderDataType::Mat3x4: return "mat3x4";
		case ShaderDataType::Mat4: return "mat4";
		case ShaderDataType::Mat4x2: return "mat4x2";
		case ShaderDataType::Mat4x3: return "mat4x3";
		case ShaderDataType::DMat2: return "dmat2";
		case ShaderDataType::DMat2x3: return "dmat2x3";
		case ShaderDataType::DMat2x4: return "dmat2x4";
		case ShaderDataType::DMat3: return "dmat3";
		case ShaderDataType::DMat3x2: return "dmat3x2";
		case ShaderDataType::DMat3x4: return "dmat3x4";
		case ShaderDataType::DMat4: return "dmat4";
		case ShaderDataType::DMat4x2: return "dmat4x2";
		case ShaderDataType::DMat4x3: return "dmat4x3";
		}

		RAY_ASSERT(false, "[String] Shader Data type {0} is invalid", (uint32_t)type);
		return "INVALID SHADER DATA TYPE";
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

	uint32_t String::GetIndex(std::string_view str)
	{
		size_t startPos = str.find('[');
		size_t endPos = str.find(']');

		uint32_t index = 0;
		for (char c : str.substr(startPos + 1, endPos - startPos - 1))
		{
			RAY_MEXPECTS(std::isdigit(c), "[String] Character '{0}' is no digit", c);
			index *= 10;
			index += (uint32_t)c - 48;
		}
		return index;
	}

}  // namespace At0::Ray
