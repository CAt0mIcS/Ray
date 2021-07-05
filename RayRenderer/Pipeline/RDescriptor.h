#pragma once

#include "../Core/RCore.h"
#include "RShader.h"

RR_EXTERN_C_BEG

RR_DEFINE_HANDLE(RrDescriptorSetLayout);
RR_DEFINE_HANDLE(RrDescriptorPool);
RR_DEFINE_HANDLE(RrDescriptorSet);
RR_DEFINE_HANDLE(RrCommandBuffer);
RR_DEFINE_HANDLE(RrLogicalDevice);
RR_DEFINE_HANDLE(RrSampler);
RR_DEFINE_HANDLE(RrImageView);
RR_DEFINE_HANDLE(RrPipelineLayout);
RR_DEFINE_HANDLE(RrBuffer);
RR_DEFINE_HANDLE(RrBufferView);

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

typedef struct RrDescriptorSetAllocateInfo
{
	RrDescriptorPool descriptorPool;
	uint32_t descriptorSetCount;
	const RrDescriptorSetLayout* pSetLayouts;
} RrDescriptorSetAllocateInfo;

typedef struct RrDescriptorImageInfo
{
	RrSampler sampler;
	RrImageView imageView;
	RrImageLayout imageLayout;
} RrDescriptorImageInfo;

typedef struct RrDescriptorBufferInfo
{
	RrBuffer buffer;
	RrDeviceSize offset;
	RrDeviceSize range;
} RrDescriptorBufferInfo;

typedef struct RrWriteDescriptorSet
{
	RrDescriptorSet dstSet;
	uint32_t dstBinding;
	uint32_t dstArrayElement;
	uint32_t descriptorCount;
	RrDescriptorType descriptorType;
	const RrDescriptorImageInfo* pImageInfo;
	const RrDescriptorBufferInfo* pBufferInfo;
	const RrBufferView* pTexelBufferView;
} RrWriteDescriptorSet;

typedef struct RrCopyDescriptorSet
{
	RrDescriptorSet srcSet;
	uint32_t srcBinding;
	uint32_t srcArrayElement;
	RrDescriptorSet dstSet;
	uint32_t dstBinding;
	uint32_t dstArrayElement;
	uint32_t descriptorCount;
} RrCopyDescriptorSet;

RR_API RrError RrCreateDescriptorSetLayout(RrLogicalDevice device,
	const RrDescriptorSetLayoutCreateInfo* pCreateInfo, RrDescriptorSetLayout* pLayout);

RR_API RrError RrCreateDescriptorPool(
	RrLogicalDevice device, const RrDescriptorPoolCreateInfo* pCreateInfo, RrDescriptorPool* pPool);

RR_API RrError RrAllocateDescriptorSets(RrLogicalDevice device,
	const RrDescriptorSetAllocateInfo* pAllocInfo, RrDescriptorSet* pDescriptorSets);

RR_API void RrUpdateDescriptorSets(RrLogicalDevice device, uint32_t descriptorWriteCount,
	const RrWriteDescriptorSet* pDescriptorWrites, uint32_t descriptorCopyCount,
	const RrCopyDescriptorSet* pDescriptorCopies);

RR_API void RrDestroyDescriptorPool(RrLogicalDevice device, RrDescriptorPool pool);

RR_API void RrDestroyDescriptorSetLayout(RrLogicalDevice device, RrDescriptorSetLayout layout);


RR_API void RrCmdBindDescriptorSets(RrCommandBuffer commandBuffer,
	RrPipelineBindPoint pipelineBindPoint, RrPipelineLayout pipelineLayout, uint32_t firstSet,
	uint32_t descriptorSetCount, const RrDescriptorSet* pDescriptorSets,
	uint32_t dynamicOffsetCount, const uint32_t* pDynamicOffsets);

RR_EXTERN_C_END
