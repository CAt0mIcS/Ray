#pragma once

#include <../../extern/glm/glm/glm.hpp>
#include <iostream>
#include <memory>

namespace At0::Ray
{
	template<typename T>
	using Scope = std::unique_ptr<T>;

	template<typename T>
	using Ref = std::shared_ptr<T>;

	template<typename T, typename... Args>
	Scope<T> MakeScope(Args&&... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T, typename... Args>
	Ref<T> MakeRef(Args&&... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}


	using Point2 = glm::vec2;
	using Size2 = glm::vec2;

	using Point3 = glm::vec3;
	using Size3 = glm::vec3;

	// RAY_TODO: Learn about quaternions and add output stream operator
	using Quaternion = glm::quat;

	inline std::wostream& operator<<(std::wostream& os, const glm::vec1& vec)
	{
		os << L"{x=" << vec.x << L"}";
		return os;
	}

	inline std::ostream& operator<<(std::ostream& os, const glm::vec1& vec)
	{
		os << "{x=" << vec.x << "}";
		return os;
	}

	inline std::wostream& operator<<(std::wostream& os, const glm::vec2& vec)
	{
		os << L"{x=" << vec.x << L", y=" << vec.y << L"}";
		return os;
	}

	inline std::ostream& operator<<(std::ostream& os, const glm::vec2& vec)
	{
		os << "{x=" << vec.x << ", y=" << vec.y << "}";
		return os;
	}

	inline std::wostream& operator<<(std::wostream& os, const glm::vec3& vec)
	{
		os << L"{x=" << vec.x << L", y=" << vec.y << L", z=" << vec.z << L"}";
		return os;
	}

	inline std::ostream& operator<<(std::ostream& os, const glm::vec3& vec)
	{
		os << "{x=" << vec.x << ", y=" << vec.y << ", z=" << vec.z << "}";
		return os;
	}

	inline std::wostream& operator<<(std::wostream& os, const glm::vec4& vec)
	{
		os << L"{x=" << vec.x << L", y=" << vec.y << L", z=" << vec.z << L", w=" << vec.w << L"}";
		return os;
	}

	inline std::ostream& operator<<(std::ostream& os, const glm::vec4& vec)
	{
		os << "{x=" << vec.x << ", y=" << vec.y << ", z=" << vec.z << ", w=" << vec.w << "}";
		return os;
	}
}