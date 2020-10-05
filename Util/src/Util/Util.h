#pragma once

#include "UtilBase.h"
#include <iostream>


namespace Util
{
	struct UTIL_API NPoint
	{
		float x, y;

		const NPoint operator+(const NPoint& other) const
		{
			return { x + other.x, y + other.y };
		}

		NPoint& operator+=(const NPoint& other)
		{
			x += other.x;
			y += other.y;
			return *this;
		}

		const NPoint operator-(const NPoint& other) const
		{
			return { x - other.x, y - other.y };

		}

		NPoint& operator-=(const NPoint& other)
		{
			x -= other.x;
			y -= other.y;
			return *this;
		}

		NPoint& operator*=(const NPoint& other)
		{
			x *= other.x;
			y *= other.y;
			return *this;
		}

		const NPoint operator*(const NPoint& other) const
		{
			return { x * other.x, y * other.y };

		}

		const NPoint operator/(const NPoint& other) const
		{
			return { x / other.x, y / other.y };

		}

		NPoint& operator/=(const NPoint& other)
		{
			x /= other.x;
			y /= other.y;
			return *this;
		}

		const NPoint operator+(const float val) const
		{
			return { x + val, y + val };

		}

		NPoint& operator+=(const float val)
		{
			x += val;
			y += val;
			return *this;
		}

		const NPoint operator-(const float val) const
		{
			return { x - val, y - val };
		}

		NPoint& operator-=(const float val)
		{
			x -= val;
			y -= val;
			return *this;
		}

		NPoint& operator*=(const float val)
		{
			x *= val;
			y *= val;
			return *this;
		}

		const NPoint operator*(const float val) const
		{
			return { x * val, y * val };
		}

		const NPoint operator/(const float val) const
		{
			return { x / val, y / val };
		}

		NPoint& operator/=(const float val)
		{
			x /= val;
			y /= val;
			return *this;
		}

		NPoint& operator=(const float val)
		{
			x = val;
			y = val;
			return *this;
		}

		const NPoint operator+(const POINTS val) const
		{
			return { x + val.x, y + val.y };
		}

		NPoint& operator+=(const POINTS val)
		{
			x += val.x;
			y += val.y;
			return *this;
		}

		const NPoint operator-(const POINTS val) const
		{
			return { x - val.x, y - val.y };
		}

		NPoint& operator-=(const POINTS val)
		{
			x += val.x;
			y += val.y;
			return *this;
		}

		NPoint& operator*=(const POINTS val)
		{
			x *= val.x;
			y *= val.y;
			return *this;
		}

		const NPoint operator*(const POINTS val) const
		{
			return { x * val.x, y * val.y };
		}

		const NPoint operator/(const POINTS val) const
		{
			return { x / val.x, y / val.y };
		}

		NPoint& operator/=(const POINTS val)
		{
			x /= val.x;
			y /= val.y;
			return *this;
		}

		NPoint& operator=(const POINTS val)
		{
			x = val.x;
			y = val.y;
			return *this;
		}

		D2D1_POINT_2F ToD2D1Point2F() const
		{
			return { x, y };
		}

		friend std::ostream& operator<<(std::ostream& os, const NPoint& pos)
		{
			os << "{ x=" << pos.x << " y=" << pos.y << " }";
			return os;
		}
	};


	struct UTIL_API NSize
	{
		float width, height;

		const NSize operator+(const NSize& other)
		{
			return { width + other.width, height + other.height };
		}

		NSize& operator+=(const NSize& other)
		{
			width += other.width;
			height += other.height;
			return *this;
		}

		const NSize operator-(const NSize& other) const
		{
			return { width - other.width, height - other.height };
		}

		NSize& operator-=(const NSize& other)
		{
			width += other.width;
			height += other.height;
			return *this;
		}

		NSize& operator*=(const NSize& other)
		{
			width *= other.width;
			height *= other.height;
			return *this;
		}

		const NSize operator*(const NSize& other) const
		{
			return { width * other.width, height * other.height };
		}

		const NSize operator/(const NSize& other)
		{
			return { width / other.width, height / other.height };
		}

		NSize& operator/=(const NSize& other)
		{
			width /= other.width;
			height /= other.height;
			return *this;
		}

		const NSize operator+(const float val) const
		{
			return { width + val, height + val };
		}

		NSize& operator+=(const float val)
		{
			width += val;
			height += val;
			return *this;
		}

		const NSize operator-(const float val) const
		{
			return { width - val, height - val };
		}

		NSize& operator-=(const float val)
		{
			width -= val;
			height -= val;
			return *this;
		}

		NSize& operator*=(const float val)
		{
			width *= val;
			height *= val;
			return *this;
		}

		const NSize operator*(const float val) const
		{
			return { width * val, height * val };
		}

		const NSize operator/(const float val) const
		{
			return { width / val, height / val };
		}

		NSize& operator/=(const float val)
		{
			width /= val;
			height /= val;
			return *this;
		}

		friend std::ostream& operator<<(std::ostream& os, const NSize& size)
		{
			os << "{ width=" << size.width << " height=" << size.height << " }";
			return os;
		}
	};



	struct UTIL_API NColor
	{
		float r, g, b;

		NColor operator+(const NColor& other) const
		{
			return { r + other.r, g + other.g, b + other.b };
		}

		NColor& operator+=(const NColor& other)
		{
			r += other.r;
			g += other.g;
			b += other.b;
			return *this;
		}

		NColor operator-(const NColor& other) const
		{
			return { r - other.r, g - other.g, b - other.b };

		}

		NColor& operator-=(const NColor& other)
		{
			r -= other.r;
			g -= other.g;
			b -= other.b;
			return *this;
		}

		NColor& operator*=(const NColor& other)
		{
			r *= other.r;
			g *= other.g;
			b *= other.b;
			return *this;
		}

		NColor operator*(const NColor& other) const
		{
			return { r * other.r, g * other.g, b * other.b };

		}

		NColor operator/(const NColor& other) const
		{
			return { r / other.r, g / other.g, b / other.b };

		}

		NColor& operator/=(const NColor& other)
		{
			r /= other.r;
			g /= other.g;
			b /= other.b;
			return *this;
		}

		NColor operator+(const float val) const
		{
			return { r + val, g + val, b + val };

		}

		NColor& operator+=(const float val)
		{
			r += val;
			g += val;
			b += val;
			return *this;
		}

		NColor operator-(const float val) const
		{
			return { r - val, g - val, b - val };
		}

		NColor& operator-=(const float val)
		{
			r -= val;
			g -= val;
			b -= val;
			return *this;
		}

		NColor& operator*=(const float val)
		{
			r *= val;
			g *= val;
			b *= val;
			return *this;
		}

		NColor operator*(const float val) const
		{
			return { r * val, g * val, b * val };
		}

		NColor operator/(const float val) const
		{
			return { r / val, g / val, b / val };
		}

		NColor& operator/=(const float val)
		{
			r /= val;
			g /= val;
			b /= val;
			return *this;
		}

		NColor& operator=(const float val)
		{
			r = val;
			g = val;
			b = val;
			return *this;
		}

		NColor operator+(const D2D1_COLOR_F& val) const
		{
			return { r + val.r, g + val.g, b + val.b };
		}

		NColor& operator+=(const D2D1_COLOR_F& val)
		{
			r += val.r;
			g += val.g;
			b += val.b;
			return *this;
		}

		NColor operator-(const D2D1_COLOR_F& val) const
		{
			return { r - val.r, g - val.g, b - val.b };
		}

		NColor& operator-=(const D2D1_COLOR_F& val)
		{
			r -= val.r;
			g -= val.g;
			b -= val.b;
			return *this;
		}

		NColor& operator*=(const D2D1_COLOR_F& val)
		{
			r *= val.r;
			g *= val.g;
			b *= val.b;
			return *this;
		}

		NColor operator*(const D2D1_COLOR_F& val) const
		{
			return { r * val.r, g * val.g, b * val.b };
		}

		NColor operator/(const D2D1_COLOR_F& val) const
		{
			return { r / val.r, g / val.g, b / val.b };
		}

		NColor& operator/=(const D2D1_COLOR_F& val)
		{
			r /= val.r;
			g /= val.g;
			b /= val.b;
			return *this;
		}

		NColor& operator=(const D2D1_COLOR_F& val)
		{
			r = val.r;
			g = val.g;
			b = val.b;
			return *this;
		}

		D2D1_COLOR_F ToD2D1ColorF() const
		{
			return { r / 255.0f, g / 255.0f, b / 255.0f, 1.0f };
		}

		friend std::ostream& operator<<(std::ostream& os, const NColor& col)
		{
			os << "{ r=" << col.r << " g=" << col.g << " b=" << col.b << " }";
			return os;
		}
	};

	/// <summary>
	/// Converts a multi byte string (std::string) to wide character string (std::wstring)
	/// </summary>
	/// <param name="str">Is the multi byte string</param>
	/// <returns>The wide character string</returns>
	inline std::wstring MultiByteToWideChar(const std::string& str)
	{
		auto size = ::MultiByteToWideChar(CP_UTF8, MB_COMPOSITE, str.c_str(), -1, nullptr, 0);

		std::wstring buff;
		buff.resize(size);
		::MultiByteToWideChar(CP_UTF8, MB_COMPOSITE, str.c_str(), -1, buff.data(), size);
		
		return buff;
	}

	/// <summary>
	/// Converts a wide character string (std::wstring) to multi byte string (std::string)
	/// </summary>
	/// <param name="str">Is the wide character string</param>
	/// <returns>The multi byte string</returns>
	inline std::string WideCharToMultiByte(const std::wstring& str)
	{
		auto size = ::WideCharToMultiByte(CP_UTF8, WC_COMPOSITECHECK, str.c_str(), -1, nullptr, 0, NULL, NULL);
		
		std::string buff;
		buff.resize(size);
		::WideCharToMultiByte(CP_UTF8, WC_COMPOSITECHECK, str.c_str(), -1, buff.data(), size, NULL, NULL);
		
		return buff;
	}
}
