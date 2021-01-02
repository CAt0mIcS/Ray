#pragma once

#include <../../RayUtil/include/RayUtil/RMath.h>

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


	using Point2 = Float2;
	using Size2 = Float2;

	using Point3 = Float3;
	using Size3 = Float3;

	inline std::wostream& operator<<(std::wostream& os, const Float2& vec)
	{
		os << L"{x=" << vec.x << L", y=" << vec.y << L"}";
		return os;
	}

	inline std::ostream& operator<<(std::ostream& os, const Float2& vec)
	{
		os << "{x=" << vec.x << ", y=" << vec.y << "}";
		return os;
	}

	inline std::wostream& operator<<(std::wostream& os, const Float3& vec)
	{
		os << L"{x=" << vec.x << L", y=" << vec.y << L", z=" << vec.z << L"}";
		return os;
	}

	inline std::ostream& operator<<(std::ostream& os, const Float3& vec)
	{
		os << "{x=" << vec.x << ", y=" << vec.y << ", z=" << vec.z << "}";
		return os;
	}

	inline std::wostream& operator<<(std::wostream& os, const Float4& vec)
	{
		os << L"{x=" << vec.x << L", y=" << vec.y << L", z=" << vec.z << L", w=" << vec.w << L"}";
		return os;
	}

	inline std::ostream& operator<<(std::ostream& os, const Float4& vec)
	{
		os << "{x=" << vec.x << ", y=" << vec.y << ", z=" << vec.z << ", w=" << vec.w << "}";
		return os;
	}
}