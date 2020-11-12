#ifndef RLR_TYPEDEFINES_H
#define RLR_TYPEDEFINES_H

#include <../../RlUtilities/include/RlUtil/PlatformDetection.h>
#include <glm/glm.hpp>
#include <iostream>

#ifdef RL_PLATFORM_LINUX
//XLib Window 
class Window;
#endif

namespace At0::Reyal
{
#ifdef RL_PLATFORM_WINDOWS
	using WindowHandle = HWND;
#elif defined(RL_PLATFORM_LINUX)
	using WindowHandle = Window*;
#endif

	namespace
	{
		using Point2 = glm::vec2;
		using Size2 = glm::vec2;

		std::wostream& operator<<(std::wostream& os, const glm::vec1& vec)
		{
			os << L"{x=" << vec.x << L"}";
			return os;
		}

		std::ostream& operator<<(std::ostream& os, const glm::vec1& vec)
		{
			os << "{x=" << vec.x << "}";
			return os;
		}

		std::wostream& operator<<(std::wostream& os, const glm::vec2& vec)
		{
			os << L"{x=" << vec.x << L", y=" << vec.y << L"}";
			return os;
		}

		std::ostream& operator<<(std::ostream& os, const glm::vec2& vec)
		{
			os << "{x=" << vec.x << ", y=" << vec.y << "}";
			return os;
		}

		std::wostream& operator<<(std::wostream& os, const glm::vec3& vec)
		{
			os << L"{x=" << vec.x << L", y=" << vec.y << L", z=" << vec.z << L"}";
			return os;
		}

		std::ostream& operator<<(std::ostream& os, const glm::vec3& vec)
		{
			os << "{x=" << vec.x << ", y=" << vec.y << ", z=" << vec.z << "}";
			return os;
		}

		std::wostream& operator<<(std::wostream& os, const glm::vec4& vec)
		{
			os << L"{x=" << vec.x << L", y=" << vec.y << L", z=" << vec.z << L", w=" << vec.w << L"}";
			return os;
		}

		std::ostream& operator<<(std::ostream& os, const glm::vec4& vec)
		{
			os << "{x=" << vec.x << ", y=" << vec.y << ", z=" << vec.z << ", w=" << vec.w << "}";
			return os;
		}
	}

	
}


#endif // RLR_TYPEDEFINES_H
