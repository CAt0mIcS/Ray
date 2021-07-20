#pragma once

#include "RCore.h"

RR_EXTERN_C_BEG

RR_DEFINE_HANDLE(RrSampler);
RR_DEFINE_HANDLE(RrLogicalDevice);

typedef enum RrSamplerAddressMode
{
	RrSamplerAddressModeRepeat = 0,
	RrSamplerAddressModeMirroredRepeat = 1,
	RrSamplerAddressModeClampToEdge = 2,
	RrSamplerAddressModeClampToBorder = 3,
	RrSamplerAddressModeMirrorClampToEdge = 4
} RrSamplerAddressMode;

typedef enum RrSamplerCreateFlagBits
{
	RrSamplerCreateSubsampledEXT = 0x00000001,
	RrSamplerCreateSubsampledCoarseReconstructionEXT = 0x00000002
} RrSamplerCreateFlagBits;
typedef uint32_t RrSamplerCreateFlags;

typedef enum RrSamplerMipmapMode
{
	RrSamplerMipmapModeNearest = 0,
	RrSamplerMipmapModeLinear = 1
} RrSamplerMipmapMode;

typedef enum RrBorderColor
{
	RrBorderColorFloatTransparentBlack = 0,
	RrBorderColorIntTransparentBlack = 1,
	RrBorderColorFloatOpaqueBlack = 2,
	RrBorderColorIntOpaqueBlack = 3,
	RrBorderColorFloatOpaqueWhite = 4,
	RrBorderColorIntOpaqueWhite = 5,
	RrBorderColorFloatCustomEXT = 1000287003,
	RrBorderColorIntCustomEXT = 1000287004
} RrBorderColor;

typedef struct RrSamplerCreateInfo
{
	RrSamplerCreateFlags flags;
	RrFilter magFilter;
	RrFilter minFilter;
	RrSamplerMipmapMode mipmapMode;
	RrSamplerAddressMode addressModeU;
	RrSamplerAddressMode addressModeV;
	RrSamplerAddressMode addressModeW;
	float mipLodBias;
	RrBool32 anisotropyEnable;
	float maxAnisotropy;
	RrBool32 compareEnable;
	RrCompareOp compareOp;
	float minLod;
	float maxLod;
	RrBorderColor borderColor;
	RrBool32 unnormalizedCoordinates;
} RrSamplerCreateInfo;

RR_API RrError RrCreateSampler(
	RrLogicalDevice device, const RrSamplerCreateInfo* pCreateInfo, RrSampler* pSampler);

RR_API void RrDestroySampler(RrLogicalDevice device, RrSampler sampler);

RR_EXTERN_C_END
