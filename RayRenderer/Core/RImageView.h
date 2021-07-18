#pragma once

#include "RCore.h"
#include "RImage.h"

RR_EXTERN_C_BEG

RR_DEFINE_HANDLE(RrImageView);


typedef enum RrImageViewType
{
	RrImageViewType1D = 0,
	RrImageViewType2D = 1,
	RrImageViewType3D = 2,
	RrImageViewTypeCube = 3,
	RrImageViewType1DArray = 4,
	RrImageViewType2DArray = 5,
	RrImageViewTypeCubeArray = 6
} RrImageViewType;

typedef enum RrImageViewCreateFlagBits
{
	RrImageViewCreateFragmentDensityMapDynamicEXT = 0x00000001,
	RrImageViewCreateFragmentDensityMapDeferredEXT = 0x00000002
} RrImageViewCreateFlagBits;
typedef uint32_t RrImageViewCreateFlags;

typedef enum RrComponentSwizzle
{
	RrComponentSwizzleIdentity = 0,
	RrComponentSwizzleZero = 1,
	RrComponentSwizzleOne = 2,
	RrComponentSwizzleR = 3,
	RrComponentSwizzleG = 4,
	RrComponentSwizzleB = 5,
	RrComponentSwizzleA = 6
} RrComponentSwizzle;

typedef struct RrComponentMapping
{
	RrComponentSwizzle r;
	RrComponentSwizzle g;
	RrComponentSwizzle b;
	RrComponentSwizzle a;
} RrComponentMapping;

typedef struct RrImageViewCreateInfo
{
	RrImageViewCreateFlags flags;
	RrImage image;
	RrImageViewType viewType;
	RrFormat format;
	RrComponentMapping components;
	RrImageSubresourceRange subresourceRange;
} RrImageViewCreateInfo;


RR_API RrError RrCreateImageView(
	RrLogicalDevice device, const RrImageViewCreateInfo* pCreateInfo, RrImageView* pImageView);

RR_API void RrDestroyImageView(RrLogicalDevice device, RrImageView imageView);

RR_EXTERN_C_END
