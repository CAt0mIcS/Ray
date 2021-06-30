
#pragma once

#include "Ray/RBase.h"
#include <D:/dev/Cpp/Projects/Ray/RayRenderer/Core/RBuffer.h>
#include <D:/dev/Cpp/Projects/Ray/RayRenderer/Core/RCommandBuffer.h>
#include <D:/dev/Cpp/Projects/Ray/RayRenderer/Core/RCommandPool.h>
#include <D:/dev/Cpp/Projects/Ray/RayRenderer/Core/RDeviceMemory.h>
#include <D:/dev/Cpp/Projects/Ray/RayRenderer/Core/RInstance.h>
#include <D:/dev/Cpp/Projects/Ray/RayRenderer/Core/RLogicalDevice.h>
#include <D:/dev/Cpp/Projects/Ray/RayRenderer/Core/RPhysicalDevice.h>
#include <D:/dev/Cpp/Projects/Ray/RayRenderer/Core/RSurface.h>



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
		extern RrError (*CreateBuffer)(RrLogicalDevice pDevice, const RrBufferCreateInfo* const pCreateInfo, RrBuffer* ppBuffer);
		extern void (*BufferGetMemoryRequirements)(RrLogicalDevice pDevice, const RrBuffer pBuffer, RrMemoryRequirements* const pMemRequirements);
		extern RrError (*BindBufferMemory)(RrLogicalDevice pDevice, RrBuffer buffer, RrDeviceMemory memory, RrDeviceSize memoryOffset);
		extern void (*DestroyBuffer)(RrLogicalDevice device, RrBuffer buffer);
		extern RrError (*AllocateCommandBuffers)(RrLogicalDevice device,RrCommandBufferAllocateInfo* pAllocInfo, RrCommandBuffer* pCommandBuffers);
		extern RrError (*BeginCommandBuffer)(RrCommandBuffer commandBuffer, RrCommandBufferBeginInfo* pBeginInfo);
		extern void (*ExecuteCommands)(RrCommandBuffer commandBuffer, uint32_t secondaryCommandBufferCount,const RrCommandBuffer* pSecondaryCommandBuffers);
		extern RrError (*QueueSubmit)(RrQueue queue, uint32_t submitCount, const RrSubmitInfo* pSubmits, RrFence fence);
		extern RrError (*QueueWaitIdle)(RrQueue queue);
		extern RrError (*EndCommandBuffer)(RrCommandBuffer commandBuffer);
		extern void (*FreeCommandBuffers)(RrLogicalDevice device, RrCommandPool commandPool,uint32_t commandBufferCount, const RrCommandBuffer* pCommandBuffers);
		extern RrError (*CreateCommandPool)(RrLogicalDevice pDevice,const RrCommandPoolCreateInfo* const pCreateInfo, RrCommandPool* ppCommandPool);
		extern void (*DestroyCommandPool)(RrLogicalDevice pDevice, RrCommandPool pCommandPool);
		extern RrError (*AllocateMemory)(RrLogicalDevice pDevice,const RrMemoryAllocateInfo* const pAllocateInfo, RrDeviceMemory* ppMemory);
		extern RrError (*DeviceMemoryGetMemoryTypeIndex)(uint32_t memoryTypeBits,RrMemoryPropertyFlags memoryProperties, uint32_t memoryTypeCount,const RrMemoryType* const pMemoryTypes, uint32_t* pTypeIndex);
		extern RrError (*MapMemory)(RrLogicalDevice pDevice, RrDeviceMemory pMemory, RrDeviceSize offset,RrDeviceSize size, void** mapped);
		extern void (*UnmapMemory)(RrLogicalDevice pDevice, RrDeviceMemory pMemory);
		extern RrError (*FlushMappedMemoryRanges)(RrLogicalDevice pDevice, uint32_t memoryRangeCount, RrMappedMemoryRange* pMappedMemoryRanges);
		extern void (*FreeMemory)(RrLogicalDevice device, RrDeviceMemory memory);
		extern RrError (*Initialize)(RrInitializeInfo* const pInitInfo, RrInstance* pInstance, RrDebugMessenger* pDebugMessenger);
		extern void (*DestroyInstance)(RrInstance pInstance, RrDebugMessenger pDebugMessenger);
		extern RrPFNVoidFunction (*GetInstanceProcAddr)(RrInstance instance, const char* pName);
		extern RrError (*CreateLogicalDevice)(RrPhysicalDevice physicalDevice,RrLogicalDeviceCreateInfo* pCreateInfo, RrLogicalDevice* pDevice);
		extern void (*GetDeviceQueue)(RrLogicalDevice device, uint32_t queueFamilyIndex, uint32_t queueIndex, RrQueue* pQueue);
		extern void (*DestroyLogicalDevice)(RrLogicalDevice device);
		extern RrPFNVoidFunction (*GetDeviceProcAddr)(RrLogicalDevice device, const char* pName);
		extern RrError (*DeviceWaitIdle)(RrLogicalDevice device);
		extern RrError (*EnumeratePhysicalDevice)(RrInstance pInstance,const RrPhysicalDeviceEnumerationInfo* const pCreateInfo, RrPhysicalDevice* ppPhysicalDevice);
		extern void (*GetPhysicalDeviceProperties)(RrPhysicalDevice pDevice, RrPhysicalDeviceProperties* pProperties);
		extern void (*GetPhysicalDeviceFeatures)(RrPhysicalDevice pDevice, RrPhysicalDeviceFeatures* pFeatures);
		extern void (*GetPhysicalDeviceMemoryProperties)(RrPhysicalDevice pDevice, RrPhysicalDeviceMemoryProperties* pProperties);
		extern RrError (*CreateSurface)(RrInstance instance, RrSurfaceCreateInfo* pCreateInfo, RrSurface* pSurface);

	}  // namespace RendererAPI

	RAY_EXPORT void LoadRenderer(RendererAPI::Type type);
}  // namespace At0::Ray

