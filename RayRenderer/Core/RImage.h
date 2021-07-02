#pragma once

#include "RCore.h"
#include "RBuffer.h"

RR_EXTERN_C_BEG

RR_DEFINE_HANDLE(RrImage);
RR_DEFINE_HANDLE(RrDeviceMemory);

typedef enum RrImageType
{
	RrImageType1D = 0,
	RrImageType2D,
	RrImageType3D
} RrImageType;

typedef enum RrSampleCountFlagBits
{
	RrSampleCount1 = 0x00000001,
	RrSampleCount2 = 0x00000002,
	RrSampleCount4 = 0x00000004,
	RrSampleCount8 = 0x00000008,
	RrSampleCount16 = 0x00000010,
	RrSampleCount32 = 0x00000020,
	RrSampleCount64 = 0x00000040,
} RrSampleCountFlagBits;
typedef uint32_t RrSampleCountFlags;

typedef enum RrImageCreateFlagBits
{
	RrImageCreateSparseBinding = 0x00000001,
	RrImageCreateSparseResidency = 0x00000002,
	RrImageCreateSparseAliased = 0x00000004,
	RrImageCreateMutableFormat = 0x00000008,
	RrImageCreateCubeCompatible = 0x00000010,
	RrImageCreateAlias = 0x00000400,
	RrImageCreateSplitInstanceBindRegions = 0x00000040,
	RrImageCreate2DArrayCompatible = 0x00000020,
	RrImageCreateBlockTexelViewCopmatible = 0x00000080,
	RrImageCreateExtendedUsage = 0x00000100,
	RrImageCreateProtected = 0x00000800,
	RrImageCreateDisjoint = 0x00000200,
	RrImageCreateCornerSampledNV = 0x00002000,
	RrImageCreateSampleLocationsCompatibleDepthEXT = 0x00001000,
	RrImageCreateSubsampledEXT = 0x00004000,
} RrImageCreateFlagBits;
typedef uint32_t RrImageCreateFlags;

typedef enum RrImageTiling
{
	RrImageTilingOptimal,
	RrImageTilingLinear,
	RrImageTilingDRMFormatModifierEXT
} RrImageTiling;

typedef enum RrImageUsageFlagBits
{
	RrImageUsageTransferSrc = 0x00000001,
	RrImageUsageTransferDst = 0x00000002,
	RrImageUsageSampled = 0x00000004,
	RrImageUsageStorage = 0x00000008,
	RrImageUsageColorAttachment = 0x00000010,
	RrImageUsageDepthStencilAttachment = 0x00000020,
	RrImageUsageTransientAttachment = 0x00000040,
	RrImageUsageInputAttachment = 0x00000080,
	RrImageUsageShadingRateImageNV = 0x00000100,
	RrImageUsageFragmentDensityMapEXT = 0x00000200,
	RrImageUsageFragmentShadingRateAttachmentKHR = RrImageUsageShadingRateImageNV,
} RrImageUsageFlagBits;
typedef uint32_t RrImageUsageFlags;

typedef enum RrImageLayout
{
	RrImageLayoutUndefined = 0,
	RrImageLayoutGeneral = 1,
	RrImageLayoutColorAttachment = 2,
	RrImageLayoutDepthStencilAttachment = 3,
	RrImageLayoutDepthStencilReadOnly = 4,
	RrImageLayoutShaderReadOnly = 5,
	RrImageLayoutTransferSrc = 6,
	RrImageLayoutTransferDst = 7,
	RrImageLayoutPreinitialized = 8,
	RrImageLayoutDepthReadOnlyStencilAttachment = 1000117000,
	RrImageLayoutDepthAttachmentStencilReadOnly = 1000117001,
	RrImageLayoutDepthAttachment = 1000241000,
	RrImageLayoutDepthReadOnly = 1000241001,
	RrImageLayoutStencilAttachment = 1000241002,
	RrImageLayoutStencilReadOnly = 1000241003,
	RrImageLayoutPresentSrcKHR = 1000001002,
	RrImageLayoutShadedPresentSrcKHR = 1000111000,
	RrImageLayoutShadingRateNV = 1000164003,
	RrImageLayoutFragmentDensityMapEXT = 1000218000
} RrImageLayout;

typedef enum RrImageAspectFlagBits
{
	RrImageAspectColor = 0x00000001,
	RrImageAspectDepth = 0x00000002,
	RrImageAspectStencil = 0x00000004,
	RrImageAspectMetadata = 0x00000008,
	RrImageAspectPlane0 = 0x00000010,
	RrImageAspectPlane1 = 0x00000020,
	RrImageAspectPlane2 = 0x00000040,
	RrImageAspectMemoryPlane0EXT = 0x00000080,
	RrImageAspectMemoryPlane1EXT = 0x00000100,
	RrImageAspectMemoryPlane2EXT = 0x00000200,
	RrImageAspectMemoryPlane3EXT = 0x00000400,
	RrImageAspectPlane0KHR = RrImageAspectPlane0,
	RrImageAspectPlane1KHR = RrImageAspectPlane1,
	RrImageAspectPlane2KHR = RrImageAspectPlane2,
} RrImageAspectFlagBits;
typedef uint32_t RrImageAspectFlags;

typedef struct RrImageSubresourceLayers
{
	RrImageAspectFlags aspectMask;
	uint32_t mipLevel;
	uint32_t baseArrayLayer;
	uint32_t layerCount;
} RrImageSubresourceLayers;

typedef struct RrBufferImageCopy
{
	RrDeviceSize bufferOffset;
	uint32_t bufferRowLength;
	uint32_t bufferImageHeight;
	RrImageSubresourceLayers imageSubresource;
	RrOffset3D imageOffset;
	RrExtent3D imageExtent;
} RrBufferImageCopy;

typedef struct RrImageSubresource
{
	RrImageAspectFlags aspectMask;
	uint32_t mipLevel;
	uint32_t arrayLayer;
} RrImageSubresource;

typedef struct RrSubresourceLayout
{
	RrDeviceSize offset;
	RrDeviceSize size;
	RrDeviceSize rowPitch;
	RrDeviceSize arrayPitch;
	RrDeviceSize depthPitch;
} RrSubresourceLayout;

typedef struct RrImageSubresourceRange
{
	RrImageAspectFlags aspectMask;
	uint32_t baseMipLevel;
	uint32_t levelCount;
	uint32_t baseArrayLayer;
	uint32_t layerCount;
} RrImageSubresourceRange;

typedef struct RrImageMemoryBarrier
{
	RrAccessFlags srcAccessMask;
	RrAccessFlags dstAccessMask;
	RrImageLayout oldLayout;
	RrImageLayout newLayout;
	uint32_t srcQueueFamilyIndex;
	uint32_t dstQueueFamilyIndex;
	RrImage image;
	RrImageSubresourceRange subresourceRange;
} RrImageMemoryBarrier;

typedef struct RrImageCreateInfo
{
	RrImageCreateFlags flags;
	RrImageType imageType;
	RrFormat format;
	RrExtent3D extent;
	uint32_t mipLevels;
	uint32_t arrayLayers;
	RrSampleCountFlags samples;
	RrImageTiling tiling;
	RrImageUsageFlags usage;
	RrSharingMode sharingMode;
	uint32_t queueFamilyIndexCount;
	const uint32_t* pQueueFamilyIndices;
	RrImageLayout initialLayout;
} RrImageCreateInfo;


RR_API RrError RrCreateImage(
	RrLogicalDevice device, const RrImageCreateInfo* pCreateInfo, RrImage* pImage);

RR_API void RrImageGetMemoryRequirements(
	RrLogicalDevice device, RrImage image, RrMemoryRequirements* pMemRequirements);

RR_API RrError RrBindImageMemory(
	RrLogicalDevice device, RrImage image, RrDeviceMemory memory, RrDeviceSize memoryOffset);

RR_API void RrGetImageSubresourceLayout(RrLogicalDevice device, RrImage image,
	RrImageSubresource* pSubresource, RrSubresourceLayout* pLayout);

RR_API void RrDestroyImage(RrLogicalDevice device, RrImage image);

RR_EXTERN_C_END
