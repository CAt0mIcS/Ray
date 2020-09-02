#pragma once

#include "pch.h"


namespace NPE
{
	struct NPoint
	{
		float x, y;

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
		
		//float
		NPoint operator+(const float val)
		{
			return { x + val, y + val };

		}
			
		NPoint& operator+=(const float val)
		{
			x += val; 
			y += val; 
			return *this; 
		}
			
		NPoint operator-(const float val)
		{
			return { x - val, y - val };
		}
			
		NPoint& operator-=(const float val)
		{
			x += val; 
			y += val; 
			return *this; 
		}

		NPoint& operator*=(const float val)
		{
			x *= val;
			y *= val;
			return *this;
		}

		NPoint operator*(const float val)
		{
			return { x * val, y * val };
		}

		NPoint operator/(const float val)
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
		float width, height;

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

		//float
		NSize operator+(const float val)
		{
			return { width + val, height + val };
		}

		NSize& operator+=(const float val)
		{
			width += val;
			height += val;
			return *this;
		}

		NSize operator-(const float val)
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

		NSize operator*(const float val)
		{
			return { width * val, height * val };
		}

		NSize operator/(const float val)
		{
			return { width / val, height / val };
		}

		NSize& operator/=(const float val)
		{
			width /= val;
			height /= val;
			return *this;
		}
	};
}


