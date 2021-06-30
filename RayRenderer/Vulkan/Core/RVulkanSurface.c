#include "Rpch.h"

#include "../Core/RSurface.h"
#include "../Core/RUtils.h"

#ifdef _WIN32

	#ifndef NOMINMAX
		#define NOMINMAX
	#endif
	#include <Windows.h>

typedef uint32_t VkWin32SurfaceCreateFlagsKHR;
typedef struct VkWin32SurfaceCreateInfoKHR
{
	VkStructureType sType;
	const void* pNext;
	VkWin32SurfaceCreateFlagsKHR flags;
	HINSTANCE hinstance;
	HWND hwnd;
} VkWin32SurfaceCreateInfoKHR;
typedef VkResult(APIENTRY* PFN_vkCreateWin32SurfaceKHR)(
	VkInstance, const VkWin32SurfaceCreateInfoKHR*, const VkAllocationCallbacks*, VkSurfaceKHR*);

#else

typedef uint32_t VkXlibSurfaceCreateFlagsKHR;
typedef uint32_t VkXcbSurfaceCreateFlagsKHR;

typedef struct VkXlibSurfaceCreateInfoKHR
{
	VkStructureType sType;
	const void* pNext;
	VkXlibSurfaceCreateFlagsKHR flags;
	Display* dpy;
	Window window;
} VkXlibSurfaceCreateInfoKHR;

typedef struct VkXcbSurfaceCreateInfoKHR
{
	VkStructureType sType;
	const void* pNext;
	VkXcbSurfaceCreateFlagsKHR flags;
	xcb_connection_t* connection;
	xcb_window_t window;
} VkXcbSurfaceCreateInfoKHR;

typedef VkResult(APIENTRY* PFN_vkCreateXlibSurfaceKHR)(
	VkInstance, const VkXlibSurfaceCreateInfoKHR*, const VkAllocationCallbacks*, VkSurfaceKHR*);
typedef VkResult(APIENTRY* PFN_vkCreateXcbSurfaceKHR)(
	VkInstance, const VkXcbSurfaceCreateInfoKHR*, const VkAllocationCallbacks*, VkSurfaceKHR*);

#endif

RrError RrCreateSurface(RrInstance instance, RrSurfaceCreateInfo* pCreateInfo, RrSurface* pSurface)
{
#ifdef _WIN32
	VkWin32SurfaceCreateInfoKHR createInfo;
	memset(&createInfo, 0, sizeof(createInfo));
	PFN_vkCreateWin32SurfaceKHR vkCreateWin32SurfaceKHR =
		(PFN_vkCreateWin32SurfaceKHR)vkGetInstanceProcAddr(
			(VkInstance)instance, "vkCreateWin32SurfaceKHR");
	if (!vkCreateWin32SurfaceKHR)
	{
		LogError("Win32: Vulkan instance missing VK_KHR_win32_surface "
				 "extension");
		return RrErrorExtensionNotPresent;
	}

	createInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
	createInfo.hinstance = GetModuleHandle(NULL);
	createInfo.hwnd = (HWND)pCreateInfo->hWnd;

	return GetError(
		vkCreateWin32SurfaceKHR((VkInstance)instance, &createInfo, NULL, (VkSurfaceKHR*)pSurface));
#else
	if (pCreateInfo->xcb)
	{
		VkXcbSurfaceCreateInfoKHR sci;
		memset(&sci, 0, sizeof(sci));
		PFN_vkCreateXcbSurfaceKHR vkCreateXcbSurfaceKHR;
		xcb_connection_t* connection = XGetXCBConnection(pCreateInfo->display);
		if (!connection)
		{
			LogError("X11: Failed to retrieve XCB connection");
			return RrErrorExtensionNotPresent;
		}

		vkCreateXcbSurfaceKHR =
			(PFN_vkCreateXcbSurfaceKHR)vkGetInstanceProcAddr(instance, "vkCreateXcbSurfaceKHR");
		if (!vkCreateXcbSurfaceKHR)
		{
			LogError("X11: Vulkan instance missing VK_KHR_xcb_surface extension");
			return RrErrorExtensionNotPresent;
		}

		sci.sType = VK_STRUCTURE_TYPE_XCB_SURFACE_CREATE_INFO_KHR;
		sci.connection = connection;
		sci.window = pCreateInfo->window;
		return GetError(vkCreateXcbSurfaceKHR(instance, &sci, NULL, surface));
	}
	else
	{
		VkXlibSurfaceCreateInfoKHR sci;
		PFN_vkCreateXlibSurfaceKHR vkCreateXlibSurfaceKHR;

		vkCreateXlibSurfaceKHR =
			(PFN_vkCreateXlibSurfaceKHR)vkGetInstanceProcAddr(instance, "vkCreateXlibSurfaceKHR");
		if (!vkCreateXlibSurfaceKHR)
		{
			LogError("X11: Vulkan instance missing VK_KHR_xlib_surface extension");
			return RrErrorExtensionNotPresent;
		}

		memset(&sci, 0, sizeof(sci));
		sci.sType = VK_STRUCTURE_TYPE_XLIB_SURFACE_CREATE_INFO_KHR;
		sci.dpy = pCreateInfo->display;
		sci.window = pCreateInfo->window;

		RrError err = vkCreateXlibSurfaceKHR(instance, &sci, NULL, surface);
		if (err)
		{
			LogError("X11: Failed to create Vulkan X11 surface");
			return GetError(err);
		}
	}
#endif
}
