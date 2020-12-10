#pragma once

#include <../../extern/glm/glm/glm.hpp>
#include <iostream>

#include "RlRBase.h"

namespace At0::Ray
{
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

