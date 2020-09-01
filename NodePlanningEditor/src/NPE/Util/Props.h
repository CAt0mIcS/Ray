#pragma once


#define NPE_DECLARE_OPERATORS(cls, x, y)	cls& operator+(const cls& other)\
											{\
											x += other.x;\
											y += other.y;\
											return *this;\
											}\
											\
											cls& operator+=(const cls& other)\
											{\
												x += other.x;\
												y += other.y;\
												return *this;\
											}\
											\
											cls& operator-(const cls& other)\
											{\
												x += other.x;\
												y += other.y;\
												return *this;\
											}\
											\
											cls& operator-=(const cls& other)\
											{\
												x += other.x;\
												y += other.y;\
												return *this;\
											}\
											\
											cls& operator+(const int val)\
											{\
												x += val;\
												y += val;\
												return *this;\
											}\
											\
											cls& operator+=(const int val)\
											{\
												x += val;\
												y += val;\
												return *this;\
											}\
											\
											cls& operator-(const int val)\
											{\
												x += val;\
												y += val;\
												return *this;\
											}\
											\
											cls& operator-=(const int val)\
											{\
												x += val;\
												y += val;\
												return *this;\
											}



namespace NPE
{
	struct NPoint
	{
		int x, y;

		NPE_DECLARE_OPERATORS(NPoint, x, y)
	};


	struct NSize
	{
		int width, height;

		NPE_DECLARE_OPERATORS(NSize, width, height)
	};
}


