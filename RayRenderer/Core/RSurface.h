#pragma once

#include "RCore.h"

RR_DEFINE_HANDLE(RrSurfaceKHR);
RR_DEFINE_HANDLE(RrInstance);

typedef enum RrColorSpaceKHR
{
	RrColorSpaceSRGBNonlinearKHR = 0,
	RrColorSpaceDisplayP3NonlinearEXT = 1000104001,
	RrColorSpaceExtendedSRGBLinearEXT = 1000104002,
	RrColorSpaceDisplayP3LinearEXT = 1000104003,
	RrColorSpaceDCIP3NonlinearEXT = 1000104004,
	RrColorSpaceBT709LinearEXT = 1000104005,
	RrColorSpaceBT709NonlinearEXT = 1000104006,
	RrColorSpaceBT2020LinearEXT = 1000104007,
	RrColorSpaceHDR10ST2084EXT = 1000104008,
	RrColorSpaceDolbyvisionEXT = 1000104009,
	RrColorSpaceHDR10HLGEXT = 1000104010,
	RrColorSpaceAdobeRGBLinearEXT = 1000104011,
	RrColorSpaceAdobeRGBNonlinearEXT = 1000104012,
	RrColorSpacePassThroughEXT = 1000104013,
	RrColorSpaceExtendedSRGBNonlinearEXT = 1000104014,
	RrColorSpaceDisplayNativeAMD = 1000213000
} RrColorSpaceKHR;

typedef enum RrCompositeAlphaFlagsKHR
{
	RrCompositeAlphaOpaqueKHR = 0x00000001,
	RrCompositeAlphaPreMultipliedKHR = 0x00000002,
	RrCompositeAlphaPostMultipliedKHR = 0x00000004,
	RrCompositeAlphaInheritKHR = 0x00000008
} RrCompositeAlphaFlagsKHR;

typedef enum RrSurfaceTransformFlagsKHR
{
	RrSurfaceTransformIdentityKHR = 0x00000001,
	RrSurfaceTransformRotate90KHR = 0x00000002,
	RrSurfaceTransformRotate180KHR = 0x00000004,
	RrSurfaceTransformRotate270KHR = 0x00000008,
	RrSurfaceTransformHorizontalMirrorKHR = 0x00000010,
	RrSurfaceTransformHorizontalMirrorRotate90KHR = 0x00000020,
	RrSurfaceTransformHorizontalMirrorRotate180KHR = 0x00000040,
	RrSurfaceTransformHorizontalMirrorRotate270KHR = 0x00000080,
	RrSurfaceTransformInheritKHR = 0x00000100
} RrSurfaceTransformFlagsKHR;

typedef struct RrSurfaceCapabilitiesKHR
{
	uint32_t minImageCount;
	uint32_t maxImageCount;
	RrExtent2D currentExtent;
	RrExtent2D minImageExtent;
	RrExtent2D maxImageExtent;
	uint32_t maxImageArrayLayers;
	RrSurfaceTransformFlagsKHR supportedTransforms;
	RrSurfaceTransformFlagsKHR currentTransform;
	RrCompositeAlphaFlagsKHR supportedCompositeAlpha;
	RrImageUsageFlags supportedUsageFlags;
} RrSurfaceCapabilitiesKHR;

typedef struct RrSurfaceFormatKHR
{
	RrFormat format;
	RrColorSpaceKHR colorSpace;
} RrSurfaceFormatKHR;

typedef struct RrSurfaceCreateInfoKHR
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
} RrSurfaceCreateInfoKHR;


RR_API RrError RrCreateSurfaceKHR(
	RrInstance instance, const RrSurfaceCreateInfoKHR* pCreateInfo, RrSurfaceKHR* pSurface);
