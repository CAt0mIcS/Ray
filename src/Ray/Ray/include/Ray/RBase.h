#pragma once

#include <memory>
#include <assert.h>

// Define so Windows.h doesnt define min, max macros
#ifndef NOMINMAX
#define NOMINMAX
#endif

#ifdef RAY_BUILD
    #if defined(_MSC_VER)
        #define RAY_API __declspec(dllexport)
    #else
        #define RAY_API
    #endif
#else
    #if defined(_MSC_VER)
        #define RAY_API __declspec(dllimport)
    #else
        #define RAY_API
    #endif
#endif



#include <../../RayUtil/include/RayUtil/RGlobalDefines.h>


