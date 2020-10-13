#pragma once

#include "UtilBase.h"
#include <iostream>

#include <d2d1.h>


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

		operator D2D1_POINT_2F() const
		{
			return { x, y };
		}

		operator POINTS() const
		{
			return { (SHORT)x, (SHORT)y };
		}

		friend std::ostream& operator<<(std::ostream& os, const NPoint& pos)
		{
			os << "{ x=" << pos.x << " y=" << pos.y << " }";
			return os;
		}

		bool operator==(const NPoint& other)
		{
			return x == other.x && y == other.y;
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

		bool operator==(const NSize& other)
		{
			return width == other.width && height == other.height;
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

		operator D2D1_COLOR_F() const
		{
			return { r / 255.0f, g / 255.0f, b / 255.0f, 1.0f };
		}

		friend std::ostream& operator<<(std::ostream& os, const NColor& col)
		{
			os << "{ r=" << col.r << " g=" << col.g << " b=" << col.b << " }";
			return os;
		}

		bool operator==(const NColor& other)
		{
			return r == other.r && g == other.g && b == other.b;
		}
	};

	struct UTIL_API NTransform
	{
		NPoint pos;
		NSize size;

		D2D1_RECT_F ToD2D1RectF()
		{
			D2D1_RECT_F rc;
			rc.left = pos.x;
			rc.top = pos.y;
			rc.right = pos.x + size.width;
			rc.bottom = pos.y + size.height;

			return rc;
		}

		bool operator==(const NTransform& other)
		{
			return pos == other.pos && size == other.size;
		}

		friend std::ostream& operator<<(std::ostream& os, const NTransform& transform)
		{
			os << "Pos :" << transform.pos << "\nSize:" << transform.size;
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

	/// <summary>
	/// Transforms a position and size into a RECT structure
	/// </summary>
	/// <param name="pos">Is the position to transform</param>
	/// <param name="size">Is the size to transform</param>
	/// <returns>The transformed RECT structure</returns>
	inline RECT ToRect(const NPoint& pos, const NSize& size)
	{
		RECT rc;
		rc.left = (LONG)floor(pos.x);
		rc.right = (LONG)floor(pos.x + size.width);
		rc.top = (LONG)floor(pos.y);
		rc.bottom = (LONG)floor(pos.y + size.height);

		return rc;
	}

	/// <summary>
	/// Transforms a RECT structure into a NTransform structure
	/// </summary>
	/// <param name="rc">Is the RECT to transform</param>
	/// <returns>The transformed NTransform structure</returns>
	inline NTransform ToTransform(const RECT& rc)
	{
		NTransform transform;
		transform.pos.x = (float)floor(rc.left);
		transform.pos.y = (float)floor(rc.top);
		transform.size.width = (float)floor(rc.right - rc.left);
		transform.size.height = (float)floor(rc.bottom - rc.top);

		return transform;
	}
}
