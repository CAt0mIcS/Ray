#pragma once

#include <d2d1.h>


namespace NPE
{
	struct NPoint
	{
		int x, y;

		//NPoint
		NPoint operator+(const NPoint& other)
		{
			return { x + other.x, y + other.y };
		}
			
		NPoint& operator+=(const NPoint& other)
		{
			x += other.x; 
			y += other.y; 
			return *this; 
		}
			
		NPoint operator-(const NPoint& other)
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

		NPoint operator*(const NPoint& other)
		{
			return { x * other.x, y * other.y };

		}

		NPoint operator/(const NPoint& other)
		{
			return { x / other.x, y / other.y };

		}

		NPoint& operator/=(const NPoint& other)
		{
			x /= other.x;
			y /= other.y;
			return *this;
		}
		
		//int
		NPoint operator+(const int val)
		{
			return { x + val, y + val };

		}
			
		NPoint& operator+=(const int val)
		{
			x += val; 
			y += val; 
			return *this; 
		}
			
		NPoint operator-(const int val)
		{
			return { x - val, y - val };
		}
			
		NPoint& operator-=(const int val)
		{
			x -= val; 
			y -= val; 
			return *this; 
		}

		NPoint& operator*=(const int val)
		{
			x *= val;
			y *= val;
			return *this;
		}

		NPoint operator*(const int val)
		{
			return { x * val, y * val };
		}

		NPoint operator/(const int val)
		{
			return { x / val, y / val };
		}

		NPoint& operator/=(const int val)
		{
			x /= val;
			y /= val;
			return *this;
		}

		NPoint& operator=(const int val)
		{
			x = val;
			y = val;
			return *this;
		}

		//POINTS
		NPoint operator+(const POINTS val)
		{
			return { x + val.x, y + val.y };
		}

		NPoint& operator+=(const POINTS val)
		{
			x += val.x;
			y += val.y;
			return *this;
		}

		NPoint operator-(const POINTS val)
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

		NPoint operator*(const POINTS val)
		{
			return { x * val.x, y * val.y };
		}

		NPoint operator/(const POINTS val)
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
	};


	struct NSize
	{
		int width, height;

		//NSize
		NSize operator+(const NSize& other)
		{
			return { width + other.width, height + other.height };
		}

		NSize& operator+=(const NSize& other)
		{
			width += other.width;
			height += other.height;
			return *this;
		}

		NSize operator-(const NSize& other)
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

		NSize operator*(const NSize& other)
		{
			return { width * other.width, height * other.height };
		}

		NSize operator/(const NSize& other)
		{
			return { width / other.width, height / other.height };
		}

		NSize& operator/=(const NSize& other)
		{
			width /= other.width;
			height /= other.height;
			return *this;
		}

		//int
		NSize operator+(const int val)
		{
			return { width + val, height + val };
		}

		NSize& operator+=(const int val)
		{
			width += val;
			height += val;
			return *this;
		}

		NSize operator-(const int val)
		{
			return { width - val, height - val };
		}

		NSize& operator-=(const int val)
		{
			width -= val;
			height -= val;
			return *this;
		}

		NSize& operator*=(const int val)
		{
			width *= val;
			height *= val;
			return *this;
		}

		NSize operator*(const int val)
		{
			return { width * val, height * val };
		}

		NSize operator/(const int val)
		{
			return { width / val, height / val };
		}

		NSize& operator/=(const int val)
		{
			width /= val;
			height /= val;
			return *this;
		}
	};



	struct NColor
	{
		float r, g, b;

		//NColor
		NColor operator+(const NColor& other)
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

		NColor operator-(const NColor& other)
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

		NColor operator*(const NColor& other)
		{
			return { r * other.r, g * other.g, b * other.b };

		}

		NColor operator/(const NColor& other)
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

		//float
		NColor operator+(const float val)
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

		NColor operator-(const float val)
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

		NColor operator*(const float val)
		{
			return { r * val, g * val, b * val };
		}

		NColor operator/(const float val)
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

		//D2D1_COLOR_F
		NColor operator+(const D2D1_COLOR_F& val)
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

		NColor operator-(const D2D1_COLOR_F& val)
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

		NColor operator*(const D2D1_COLOR_F& val)
		{
			return { r * val.r, g * val.g, b * val.b };
		}

		NColor operator/(const D2D1_COLOR_F& val)
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
	};

}


