#pragma once

#include "../Core/RCore.h"
#include "RShader.h"

RR_EXTERN_C_BEG

RR_DEFINE_HANDLE(RrDescriptorSetLayout);
RR_DEFINE_HANDLE(RrDescriptorPool);
RR_DEFINE_HANDLE(RrLogicalDevice);
RR_DEFINE_HANDLE(RrSampler);

typedef enum RrDescriptorType
{
	RrDescriptorTypeSampler = 0,
	RrDescriptorTypeCombinedImageSampler = 1,
	RrDescriptorTypeSampledImage = 2,
	RrDescriptorTypeStorageImage = 3,
	RrDescriptorTypeUniformTexelBuffer = 4,
	RrDescriptorTypeStorageTexelBuffer = 5,
	RrDescriptorTypeUniformBuffer = 6,
	RrDescriptorTypeStorageBuffer = 7,
	RrDescriptorTypeUniformBufferDynamic = 8,
	RrDescriptorTypeStorageBufferDynamic = 9,
	RrDescriptorTypeInputAttachment = 10,
	RrDescriptorTypeInlineUniformBlockEXT = 1000138000,
	RrDescriptorTypeAccelerationStructureKHR = 1000150000,
	RrDescriptorTypeAccelerationStructureNV = 1000165000,
	RrDescriptorTypeUnknown = 0x7FFFFFFF
} RrDescriptorType;

typedef struct RrDescriptorSetLayoutBinding
{
	uint32_t binding;
	RrDescriptorType descriptorType;
	uint32_t descriptorCount;
	RrShaderStageFlags stageFlags;
	const RrSampler* pImmutableSamplers;
} RrDescriptorSetLayoutBinding;

typedef struct RrDescriptorPoolSize
{
	RrDescriptorType type;
	uint32_t descriptorCount;
} RrDescriptorPoolSize;

typedef enum RrDescriptorSetLayoutCreateFlagBits
{
	RrDescriptorSetLayoutCreateUpdateAfterBindPool = 0x00000002,
	RrDescriptorSetLayoutCreatePushDescriptorKHR = 0x00000001
} RrDescriptorSetLayoutCreateFlagBits;
typedef uint32_t RrDescriptorSetLayoutCreateFlags;

typedef struct RrDescriptorSetLayoutCreateInfo
{
	RrDescriptorSetLayoutCreateFlags flags;
	uint32_t bindingCount;
	const RrDescriptorSetLayoutBinding* pBindings;
} RrDescriptorSetLayoutCreateInfo;

typedef enum RrDescriptorPoolCreateFlagBits
{
	RrDescriptorPoolCreateFreeDescriptorSet = 0x00000001,
	RrDescriptorPoolCreateUpdateAfterBind = 0x00000002
} RrDescriptorPoolCreateFlagBits;
typedef uint32_t RrDescriptorPoolCreateFlags;

typedef struct RrDescriptorPoolCreateInfo
{
	RrDescriptorPoolCreateFlags flags;
	uint32_t maxSets;
	uint32_t poolSizeCount;
	const RrDescriptorPoolSize* pPoolSizes;
} RrDescriptorPoolCreateInfo;

RR_API RrError RrCreateDescriptorSetLayout(RrLogicalDevice device,
	const RrDescriptorSetLayoutCreateInfo* pCreateInfo, RrDescriptorSetLayout* pLayout);

RR_API RrError RrCreateDescriptorPool(
	RrLogicalDevice device, const RrDescriptorPoolCreateInfo* pCreateInfo, RrDescriptorPool* pPool);

RR_API void RrDestroyDescriptorPool(RrLogicalDevice device, RrDescriptorPool pool);

RR_API void RrDestroyDescriptorSetLayout(RrLogicalDevice device, RrDescriptorSetLayout layout);

RR_EXTERN_C_END
