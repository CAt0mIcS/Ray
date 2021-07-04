
#pragma once

#include "Ray/RBase.h"
#include <D:/dev/Cpp/Projects/Ray/RayRenderer/Core/RBuffer.h>
#include <D:/dev/Cpp/Projects/Ray/RayRenderer/Core/RCommandBuffer.h>
#include <D:/dev/Cpp/Projects/Ray/RayRenderer/Core/RCommandPool.h>
#include <D:/dev/Cpp/Projects/Ray/RayRenderer/Core/RDeviceMemory.h>
#include <D:/dev/Cpp/Projects/Ray/RayRenderer/Core/RImage.h>
#include <D:/dev/Cpp/Projects/Ray/RayRenderer/Core/RInstance.h>
#include <D:/dev/Cpp/Projects/Ray/RayRenderer/Core/RLogicalDevice.h>
#include <D:/dev/Cpp/Projects/Ray/RayRenderer/Core/RPhysicalDevice.h>
#include <D:/dev/Cpp/Projects/Ray/RayRenderer/Core/RSurface.h>
#include <D:/dev/Cpp/Projects/Ray/RayRenderer/Synchronization/RFence.h>
#include <D:/dev/Cpp/Projects/Ray/RayRenderer/Synchronization/RSemaphore.h>
#include <D:/dev/Cpp/Projects/Ray/RayRenderer/Pipeline/RDescriptor.h>
#include <D:/dev/Cpp/Projects/Ray/RayRenderer/Pipeline/RPipeline.h>



namespace At0::Ray
{
	namespace RendererAPI
	{
		enum Type
		{
			OpenGL,
			Vulkan
		};


		extern Type API;
		extern RrError (*CreateBuffer)(RrLogicalDevice device, const RrBufferCreateInfo* pCreateInfo, RrBuffer* pBuffer);
		extern void (*BufferGetMemoryRequirements)(RrLogicalDevice device, RrBuffer buffer, RrMemoryRequirements* pMemRequirements);
		extern RrError (*BindBufferMemory)(RrLogicalDevice device, RrBuffer buffer, RrDeviceMemory memory, RrDeviceSize memoryOffset);
		extern void (*DestroyBuffer)(RrLogicalDevice device, RrBuffer buffer);
		extern RrError (*AllocateCommandBuffers)(RrLogicalDevice device,RrCommandBufferAllocateInfo* pAllocInfo, RrCommandBuffer* pCommandBuffers);
		extern RrError (*BeginCommandBuffer)(RrCommandBuffer commandBuffer, RrCommandBufferBeginInfo* pBeginInfo);
		extern void (*ExecuteCommands)(RrCommandBuffer commandBuffer, uint32_t secondaryCommandBufferCount,const RrCommandBuffer* pSecondaryCommandBuffers);
		extern RrError (*QueueSubmit)(RrQueue queue, uint32_t submitCount, const RrSubmitInfo* pSubmits, RrFence fence);
		extern RrError (*QueueWaitIdle)(RrQueue queue);
		extern RrError (*EndCommandBuffer)(RrCommandBuffer commandBuffer);
		extern void (*FreeCommandBuffers)(RrLogicalDevice device, RrCommandPool commandPool,uint32_t commandBufferCount, const RrCommandBuffer* pCommandBuffers);
		extern RrError (*CreateCommandPool)(RrLogicalDevice device,const RrCommandPoolCreateInfo* pCreateInfo, RrCommandPool* pCommandPool);
		extern void (*DestroyCommandPool)(RrLogicalDevice device, RrCommandPool commandPool);
		extern RrError (*AllocateMemory)(RrLogicalDevice device, const RrMemoryAllocateInfo* pAllocateInfo, RrDeviceMemory* pMemory);
		extern RrError (*DeviceMemoryGetMemoryTypeIndex)(uint32_t memoryTypeBits,RrMemoryPropertyFlags memoryProperties, uint32_t memoryTypeCount,const RrMemoryType* pMemoryTypes, uint32_t* pTypeIndex);
		extern RrError (*MapMemory)(RrLogicalDevice device, RrDeviceMemory memory, RrDeviceSize offset,RrDeviceSize size, void** ppMapped);
		extern void (*UnmapMemory)(RrLogicalDevice device, RrDeviceMemory memory);
		extern RrError (*FlushMappedMemoryRanges)(RrLogicalDevice device, uint32_t memoryRangeCount,const RrMappedMemoryRange* pMappedMemoryRanges);
		extern void (*FreeMemory)(RrLogicalDevice device, RrDeviceMemory memory);
		extern RrError (*CreateImage)(RrLogicalDevice device, const RrImageCreateInfo* pCreateInfo, RrImage* pImage);
		extern void (*ImageGetMemoryRequirements)(RrLogicalDevice device, RrImage image, RrMemoryRequirements* pMemRequirements);
		extern RrError (*BindImageMemory)(RrLogicalDevice device, RrImage image, RrDeviceMemory memory, RrDeviceSize memoryOffset);
		extern void (*GetImageSubresourceLayout)(RrLogicalDevice device, RrImage image,RrImageSubresource* pSubresource, RrSubresourceLayout* pLayout);
		extern void (*DestroyImage)(RrLogicalDevice device, RrImage image);
		extern RrError (*Initialize)(RrInitializeInfo* pInitInfo, RrInstance* pInstance, RrDebugMessenger* pDebugMessenger);
		extern void (*DestroyInstance)(RrInstance instance, RrDebugMessenger debugMessenger);
		extern RrPFNVoidFunction (*GetInstanceProcAddr)(RrInstance instance, const char* pName);
		extern RrError (*CreateLogicalDevice)(RrPhysicalDevice physicalDevice,RrLogicalDeviceCreateInfo* pCreateInfo, RrLogicalDevice* pDevice);
		extern void (*GetDeviceQueue)(RrLogicalDevice device, uint32_t queueFamilyIndex, uint32_t queueIndex, RrQueue* pQueue);
		extern void (*DestroyLogicalDevice)(RrLogicalDevice device);
		extern RrPFNVoidFunction (*GetDeviceProcAddr)(RrLogicalDevice device, const char* pName);
		extern RrError (*DeviceWaitIdle)(RrLogicalDevice device);
		extern RrError (*EnumeratePhysicalDevice)(RrInstance instance,const RrPhysicalDeviceEnumerationInfo* pCreateInfo, RrPhysicalDevice* pPhysicalDevice);
		extern void (*GetPhysicalDeviceProperties)(RrPhysicalDevice physicalDevice, RrPhysicalDeviceProperties* pProperties);
		extern void (*GetPhysicalDeviceFeatures)(RrPhysicalDevice physicalDevice, RrPhysicalDeviceFeatures* pFeatures);
		extern void (*GetPhysicalDeviceMemoryProperties)(RrPhysicalDevice physicalDevice, RrPhysicalDeviceMemoryProperties* pProperties);
		extern void (*GetPhysicalDeviceFormatProperties)(RrPhysicalDevice physicalDevice, RrFormat format, RrFormatProperties* pProperties);
		extern RrError (*CreateSurface)(RrInstance instance, RrSurfaceCreateInfo* pCreateInfo, RrSurface* pSurface);
		extern RrError (*CreateFence)(RrLogicalDevice device, const RrFenceCreateInfo* pCreateInfo, RrFence* pFence);
		extern RrError (*WaitForFences)(RrLogicalDevice device, uint32_t fenceCount, const RrFence* pFences,RrBool32 waitAll, uint64_t timeout);
		extern RrError (*ResetFences)(RrLogicalDevice device, uint32_t fenceCount, const RrFence* pFences);
		extern void (*DestroyFence)(RrLogicalDevice device, RrFence fence);
		extern RrError (*CreateSemaphore)(RrLogicalDevice device, RrSemaphore* pSemaphore);
		extern void (*DestroySemaphore)(RrLogicalDevice device, RrSemaphore semaphore);
		extern RrError (*CreateDescriptorSetLayout)(RrLogicalDevice device,const RrDescriptorSetLayoutCreateInfo* pCreateInfo, RrDescriptorSetLayout* pLayout);
		extern RrError (*CreateDescriptorPool)(RrLogicalDevice device, const RrDescriptorPoolCreateInfo* pCreateInfo, RrDescriptorPool* pPool);
		extern void (*DestroyDescriptorPool)(RrLogicalDevice device, RrDescriptorPool pool);
		extern void (*DestroyDescriptorSetLayout)(RrLogicalDevice device, RrDescriptorSetLayout layout);
		extern RrError (*CreatePipelineLayout)(RrLogicalDevice device,const RrPipelineLayoutCreateInfo* pCreateInfo, RrPipelineLayout* pLayout);
		extern RrError (*CreateGraphicsPipeline)(RrLogicalDevice device, RrPipelineCache pipelineCache,const RrGraphicsPipelineCreateInfo* pCreateInfo, RrPipeline* pPipeline);

	}  // namespace RendererAPI

	RAY_EXPORT void LoadRenderer(RendererAPI::Type type);
}  // namespace At0::Ray

