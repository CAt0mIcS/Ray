#pragma once

#include "RCore.h"
#include "RInstance.h"

RR_EXTERN_C_BEG

RR_DEFINE_HANDLE(RrPhysicalDevice);

typedef enum RrMemoryPropertyFlagBits
{
	RrMemoryPropertyDeviceLocal = 0x00000001,
	RrMemoryPropertyHostVisible = 0x00000002,
	RrMemoryPropertyHostCoherent = 0x00000004,
	RrMemoryPropertyHostCached = 0x00000008,
	RrMemoryPropertyLazilyAllocated = 0x00000010,
	RrMemoryPropertyProtected = 0x00000020,
	RrMemoryPropertyDeviceCoherentAMD = 0x00000040,
	RrMemoryPropertyDeviceUncachedAMD = 0x00000080,
} RrMemoryPropertyFlagBits;
typedef uint32_t RrMemoryPropertyFlags;

typedef struct RrMemoryType
{
	RrMemoryPropertyFlags propertyFlags;
	uint32_t heapIndex;
} RrMemoryType;

typedef enum RrMemoryHeapFlagBits
{
	RrMemoryHeapDeviceLocal = 0x00000001,
	RrMemoryHeapMultiInstance = 0x00000002,
	RrMemoryHeapMultiInstanceKHR = RrMemoryHeapMultiInstance
} RrMemoryHeapFlagBits;
typedef uint32_t RrMemoryHeapFlags;

typedef struct RrMemoryHeap
{
	RrDeviceSize size;
	RrMemoryHeapFlags flags;
} RrMemoryHeap;

typedef enum RrPhysicalDeviceType
{
	RrPhysicalDeviceTypeOther = 0,
	RrPhysicalDeviceTypeIntegratedGPU = 1,
	RrPhysicalDeviceTypeDiscreteGPU = 2,
	RrPhysicalDeviceTypeVirtualGPU = 3,
	RrPhysicalDeviceTypeCPU = 4,
} RrPhysicalDeviceType;

typedef struct RrPhysicalDeviceMemoryProperties
{
	uint32_t memoryTypeCount;
	RrMemoryType memoryTypes[RR_MAX_MEMORY_TYPES];
	uint32_t memoryHeapCount;
	RrMemoryHeap memoryHeaps[RR_MAX_MEMORY_HEAPS];
} RrPhysicalDeviceMemoryProperties;

typedef struct RrPhysicalDeviceFeatures
{
	RrBool32 robustBufferAccess;
	RrBool32 fullDrawIndexUint32;
	RrBool32 imageCubeArray;
	RrBool32 independentBlend;
	RrBool32 geometryShader;
	RrBool32 tessellationShader;
	RrBool32 sampleRateShading;
	RrBool32 dualSrcBlend;
	RrBool32 logicOp;
	RrBool32 multiDrawIndirect;
	RrBool32 drawIndirectFirstInstance;
	RrBool32 depthClamp;
	RrBool32 depthBiasClamp;
	RrBool32 fillModeNonSolid;
	RrBool32 depthBounds;
	RrBool32 wideLines;
	RrBool32 largePoints;
	RrBool32 alphaToOne;
	RrBool32 multiViewport;
	RrBool32 samplerAnisotropy;
	RrBool32 textureCompressionETC2;
	RrBool32 textureCompressionASTC_LDR;
	RrBool32 textureCompressionBC;
	RrBool32 occlusionQueryPrecise;
	RrBool32 pipelineStatisticsQuery;
	RrBool32 vertexPipelineStoresAndAtomics;
	RrBool32 fragmentStoresAndAtomics;
	RrBool32 shaderTessellationAndGeometryPointSize;
	RrBool32 shaderImageGatherExtended;
	RrBool32 shaderStorageImageExtendedFormats;
	RrBool32 shaderStorageImageMultisample;
	RrBool32 shaderStorageImageReadWithoutFormat;
	RrBool32 shaderStorageImageWriteWithoutFormat;
	RrBool32 shaderUniformBufferArrayDynamicIndexing;
	RrBool32 shaderSampledImageArrayDynamicIndexing;
	RrBool32 shaderStorageBufferArrayDynamicIndexing;
	RrBool32 shaderStorageImageArrayDynamicIndexing;
	RrBool32 shaderClipDistance;
	RrBool32 shaderCullDistance;
	RrBool32 shaderFloat64;
	RrBool32 shaderInt64;
	RrBool32 shaderInt16;
	RrBool32 shaderResourceResidency;
	RrBool32 shaderResourceMinLod;
	RrBool32 sparseBinding;
	RrBool32 sparseResidencyBuffer;
	RrBool32 sparseResidencyImage2D;
	RrBool32 sparseResidencyImage3D;
	RrBool32 sparseResidency2Samples;
	RrBool32 sparseResidency4Samples;
	RrBool32 sparseResidency8Samples;
	RrBool32 sparseResidency16Samples;
	RrBool32 sparseResidencyAliased;
	RrBool32 variableMultisampleRate;
	RrBool32 inheritedQueries;
} RrPhysicalDeviceFeatures;

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

typedef struct RrPhysicalDeviceLimits
{
	uint32_t maxImageDimension1D;
	uint32_t maxImageDimension2D;
	uint32_t maxImageDimension3D;
	uint32_t maxImageDimensionCube;
	uint32_t maxImageArrayLayers;
	uint32_t maxTexelBufferElements;
	uint32_t maxUniformBufferRange;
	uint32_t maxStorageBufferRange;
	uint32_t maxPushConstantsSize;
	uint32_t maxMemoryAllocationCount;
	uint32_t maxSamplerAllocationCount;
	RrDeviceSize bufferImageGranularity;
	RrDeviceSize sparseAddressSpaceSize;
	uint32_t maxBoundDescriptorSets;
	uint32_t maxPerStageDescriptorSamplers;
	uint32_t maxPerStageDescriptorUniformBuffers;
	uint32_t maxPerStageDescriptorStorageBuffers;
	uint32_t maxPerStageDescriptorSampledImages;
	uint32_t maxPerStageDescriptorStorageImages;
	uint32_t maxPerStageDescriptorInputAttachments;
	uint32_t maxPerStageResources;
	uint32_t maxDescriptorSetSamplers;
	uint32_t maxDescriptorSetUniformBuffers;
	uint32_t maxDescriptorSetUniformBuffersDynamic;
	uint32_t maxDescriptorSetStorageBuffers;
	uint32_t maxDescriptorSetStorageBuffersDynamic;
	uint32_t maxDescriptorSetSampledImages;
	uint32_t maxDescriptorSetStorageImages;
	uint32_t maxDescriptorSetInputAttachments;
	uint32_t maxVertexInputAttributes;
	uint32_t maxVertexInputBindings;
	uint32_t maxVertexInputAttributeOffset;
	uint32_t maxVertexInputBindingStride;
	uint32_t maxVertexOutputComponents;
	uint32_t maxTessellationGenerationLevel;
	uint32_t maxTessellationPatchSize;
	uint32_t maxTessellationControlPerVertexInputComponents;
	uint32_t maxTessellationControlPerVertexOutputComponents;
	uint32_t maxTessellationControlPerPatchOutputComponents;
	uint32_t maxTessellationControlTotalOutputComponents;
	uint32_t maxTessellationEvaluationInputComponents;
	uint32_t maxTessellationEvaluationOutputComponents;
	uint32_t maxGeometryShaderInvocations;
	uint32_t maxGeometryInputComponents;
	uint32_t maxGeometryOutputComponents;
	uint32_t maxGeometryOutputVertices;
	uint32_t maxGeometryTotalOutputComponents;
	uint32_t maxFragmentInputComponents;
	uint32_t maxFragmentOutputAttachments;
	uint32_t maxFragmentDualSrcAttachments;
	uint32_t maxFragmentCombinedOutputResources;
	uint32_t maxComputeSharedMemorySize;
	uint32_t maxComputeWorkGroupCount[3];
	uint32_t maxComputeWorkGroupInvocations;
	uint32_t maxComputeWorkGroupSize[3];
	uint32_t subPixelPrecisionBits;
	uint32_t subTexelPrecisionBits;
	uint32_t mipmapPrecisionBits;
	uint32_t maxDrawIndexedIndexValue;
	uint32_t maxDrawIndirectCount;
	float maxSamplerLodBias;
	float maxSamplerAnisotropy;
	uint32_t maxViewports;
	uint32_t maxViewportDimensions[2];
	float viewportBoundsRange[2];
	uint32_t viewportSubPixelBits;
	size_t minMemoryMapAlignment;
	RrDeviceSize minTexelBufferOffsetAlignment;
	RrDeviceSize minUniformBufferOffsetAlignment;
	RrDeviceSize minStorageBufferOffsetAlignment;
	int32_t minTexelOffset;
	uint32_t maxTexelOffset;
	int32_t minTexelGatherOffset;
	uint32_t maxTexelGatherOffset;
	float minInterpolationOffset;
	float maxInterpolationOffset;
	uint32_t subPixelInterpolationOffsetBits;
	uint32_t maxFramebufferWidth;
	uint32_t maxFramebufferHeight;
	uint32_t maxFramebufferLayers;
	RrSampleCountFlags framebufferColorSampleCounts;
	RrSampleCountFlags framebufferDepthSampleCounts;
	RrSampleCountFlags framebufferStencilSampleCounts;
	RrSampleCountFlags framebufferNoAttachmentsSampleCounts;
	uint32_t maxColorAttachments;
	RrSampleCountFlags sampledImageColorSampleCounts;
	RrSampleCountFlags sampledImageIntegerSampleCounts;
	RrSampleCountFlags sampledImageDepthSampleCounts;
	RrSampleCountFlags sampledImageStencilSampleCounts;
	RrSampleCountFlags storageImageSampleCounts;
	uint32_t maxSampleMaskWords;
	RrBool32 timestampComputeAndGraphics;
	float timestampPeriod;
	uint32_t maxClipDistances;
	uint32_t maxCullDistances;
	uint32_t maxCombinedClipAndCullDistances;
	uint32_t discreteQueuePriorities;
	float pointSizeRange[2];
	float lineWidthRange[2];
	float pointSizeGranularity;
	float lineWidthGranularity;
	RrBool32 strictLines;
	RrBool32 standardSampleLocations;
	RrDeviceSize optimalBufferCopyOffsetAlignment;
	RrDeviceSize optimalBufferCopyRowPitchAlignment;
	RrDeviceSize nonCoherentAtomSize;
} RrPhysicalDeviceLimits;

typedef struct RrPhysicalDeviceSparseProperties
{
	RrBool32 residencyStandard2DBlockShape;
	RrBool32 residencyStandard2DMultisampleBlockShape;
	RrBool32 residencyStandard3DBlockShape;
	RrBool32 residencyAlignedMipSize;
	RrBool32 residencyNonResidentStrict;
} RrPhysicalDeviceSparseProperties;

typedef struct RrPhysicalDeviceProperties
{
	uint32_t apiVersion;
	uint32_t driverVersion;
	uint32_t vendorID;
	uint32_t deviceID;
	RrPhysicalDeviceType deviceType;
	char deviceName[RR_MAX_PHYSICAL_DEVICE_NAME_SIZE];
	uint8_t pipelineCacheUUID[RR_UUID_SIZE];
	RrPhysicalDeviceLimits limits;
	RrPhysicalDeviceSparseProperties sparseProperties;
} RrPhysicalDeviceProperties;

typedef struct RrPhysicalDeviceEnumerationInfo
{
	uint32_t deviceExtensionCount;
	const char* const* ppDeviceExtensions;
} RrPhysicalDeviceEnumerationInfo;

RR_API RrError RrEnumeratePhysicalDevice(RrInstance pInstance,
	const RrPhysicalDeviceEnumerationInfo* const pCreateInfo, RrPhysicalDevice* ppPhysicalDevice);
typedef RrError (*RrPFNEnumeratePhysicalDevice)(
	RrInstance, const RrPhysicalDeviceEnumerationInfo* const, RrPhysicalDevice*);

RR_API void RrGetPhysicalDeviceProperties(
	RrPhysicalDevice pDevice, RrPhysicalDeviceProperties* pProperties);
typedef void (*RrPFNGetPhysicalDeviceProperties)(RrPhysicalDevice, RrPhysicalDeviceProperties*);

RR_API void RrGetPhysicalDeviceFeatures(
	RrPhysicalDevice pDevice, RrPhysicalDeviceFeatures* pFeatures);
typedef void (*RrPFNGetPhysicalDeviceFeatures)(RrPhysicalDevice, RrPhysicalDeviceFeatures*);

RR_API void RrGetPhysicalDeviceMemoryProperties(
	RrPhysicalDevice pDevice, RrPhysicalDeviceMemoryProperties* pProperties);
typedef void (*RrPFNGetPhysicalDeviceMemoryProperties)(
	RrPhysicalDevice, RrPhysicalDeviceMemoryProperties*);

RR_EXTERN_C_END
