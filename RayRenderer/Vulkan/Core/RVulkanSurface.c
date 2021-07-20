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
typedef VkResult (*PFN_vkCreateWin32SurfaceKHR)(
	VkInstance, const VkWin32SurfaceCreateInfoKHR*, const VkAllocationCallbacks*, VkSurfaceKHR*);

#else

	#include <X11/Xlib.h>
	#include <dlfcn.h>

typedef XID xcb_window_t;
typedef XID xcb_visualid_t;
typedef struct xcb_connection_t xcb_connection_t;
typedef xcb_connection_t* (*PFN_XGetXCBConnection)(const Display*);
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

typedef VkResult (*PFN_vkCreateXlibSurfaceKHR)(
	VkInstance, const VkXlibSurfaceCreateInfoKHR*, const VkAllocationCallbacks*, VkSurfaceKHR*);
typedef VkResult (*PFN_vkCreateXcbSurfaceKHR)(
	VkInstance, const VkXcbSurfaceCreateInfoKHR*, const VkAllocationCallbacks*, VkSurfaceKHR*);

static PFN_XGetXCBConnection XGetXCBConnection = NULL;
static void* x11Handle = NULL;

#endif


RrError RrCreateSurfaceKHR(
	RrInstance instance, const RrSurfaceCreateInfoKHR* pCreateInfo, RrSurfaceKHR* pSurface)
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
		if (x11Handle == NULL)
		{
	#if defined(__CYGWIN__)
			x11Handle = dlopen("libX11-xcb-1.so", RTLD_LAZY);
	#else
			x11Handle = dlopen("libX11-xcb.so.1", RTLD_LAZY);
	#endif
		}


		if (XGetXCBConnection == NULL)
			XGetXCBConnection = (PFN_XGetXCBConnection)dlsym(x11Handle, "XGetXCBConnection");

		VkXcbSurfaceCreateInfoKHR sci;
		memset(&sci, 0, sizeof(sci));
		PFN_vkCreateXcbSurfaceKHR vkCreateXcbSurfaceKHR;
		xcb_connection_t* connection = XGetXCBConnection((Display*)pCreateInfo->display);
		if (!connection)
		{
			LogError("X11: Failed to retrieve XCB connection");
			return RrErrorExtensionNotPresent;
		}

		vkCreateXcbSurfaceKHR = (PFN_vkCreateXcbSurfaceKHR)vkGetInstanceProcAddr(
			(VkInstance)instance, "vkCreateXcbSurfaceKHR");
		if (!vkCreateXcbSurfaceKHR)
		{
			LogError("X11: Vulkan instance missing VK_KHR_xcb_surface extension");
			return RrErrorExtensionNotPresent;
		}

		sci.sType = VK_STRUCTURE_TYPE_XCB_SURFACE_CREATE_INFO_KHR;
		sci.connection = connection;
		sci.window = (Window)pCreateInfo->window;
		return GetError(
			vkCreateXcbSurfaceKHR((VkInstance)instance, &sci, NULL, (VkSurfaceKHR*)pSurface));
	}
	else
	{
		VkXlibSurfaceCreateInfoKHR sci;
		PFN_vkCreateXlibSurfaceKHR vkCreateXlibSurfaceKHR;

		vkCreateXlibSurfaceKHR = (PFN_vkCreateXlibSurfaceKHR)vkGetInstanceProcAddr(
			(VkInstance)instance, "vkCreateXlibSurfaceKHR");
		if (!vkCreateXlibSurfaceKHR)
		{
			LogError("X11: Vulkan instance missing VK_KHR_xlib_surface extension");
			return RrErrorExtensionNotPresent;
		}

		memset(&sci, 0, sizeof(sci));
		sci.sType = VK_STRUCTURE_TYPE_XLIB_SURFACE_CREATE_INFO_KHR;
		sci.dpy = (Display*)pCreateInfo->display;
		sci.window = (Window)pCreateInfo->window;

		RrError err =
			vkCreateXlibSurfaceKHR((VkInstance)instance, &sci, NULL, (VkSurfaceKHR*)pSurface);
		if (err)
		{
			LogError("X11: Failed to create Vulkan X11 surface");
			return GetError(err);
		}
	}
#endif
}

void RrDestroySurfaceKHR(RrInstance instance, RrSurfaceKHR surface)
{
	vkDestroySurfaceKHR((VkInstance)instance, (VkSurfaceKHR)surface, NULL);
}
