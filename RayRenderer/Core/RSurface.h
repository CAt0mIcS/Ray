#pragma once

#include "RCore.h"
#include "RInstance.h"

RR_DEFINE_HANDLE(RrSurface);

typedef struct RrSurfaceCreateInfo
{
	union
	{
		const void* hWnd;
		const void* window;
	};

	const void* display;

	// True if xcb should be used instead of xlib
	// _glfw.x11.x11xcb.handle == true == xcb
	bool xcb;
} RrSurfaceCreateInfo;


RR_API RrError RrCreateSurface(
	RrInstance instance, RrSurfaceCreateInfo* pCreateInfo, RrSurface* pSurface);
