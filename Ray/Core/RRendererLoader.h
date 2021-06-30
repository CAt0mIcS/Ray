
#pragma once

#include "Ray/RBase.h"
#include </home/simon/dev/Cpp/Projects/Ray/RayRenderer/Core/RSurface.h>
#include </home/simon/dev/Cpp/Projects/Ray/RayRenderer/Core/RCommandBuffer.h>
#include </home/simon/dev/Cpp/Projects/Ray/RayRenderer/Core/RInstance.h>
#include </home/simon/dev/Cpp/Projects/Ray/RayRenderer/Core/RPhysicalDevice.h>
#include </home/simon/dev/Cpp/Projects/Ray/RayRenderer/Core/RDeviceMemory.h>
#include </home/simon/dev/Cpp/Projects/Ray/RayRenderer/Core/RBuffer.h>
#include </home/simon/dev/Cpp/Projects/Ray/RayRenderer/Core/RCommandPool.h>
#include </home/simon/dev/Cpp/Projects/Ray/RayRenderer/Core/RLogicalDevice.h>



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
		extern RrPFNCreateSurface CreateSurface;
		extern RrPFNAllocateCommandBuffers AllocateCommandBuffers;
		extern RrPFNBeginCommandBuffer BeginCommandBuffer;
		extern RrPFNExecuteCommands ExecuteCommands;
		extern RrPFNQueueSubmit QueueSubmit;
		extern RrPFNQueueWaitIdle QueueWaitIdle;
		extern RrPFNEndCommandBuffer EndCommandBuffer;
		extern RrPFNFreeCommandBuffers FreeCommandBuffers;
		extern RrPFNInitialize Initialize;
		extern RrPFNDestroyInstance DestroyInstance;
		extern RrPFNGetInstanceProcAddr GetInstanceProcAddr;
		extern RrPFNEnumeratePhysicalDevice EnumeratePhysicalDevice;
		extern RrPFNGetPhysicalDeviceProperties GetPhysicalDeviceProperties;
		extern RrPFNGetPhysicalDeviceFeatures GetPhysicalDeviceFeatures;
		extern RrPFNGetPhysicalDeviceMemoryProperties GetPhysicalDeviceMemoryProperties;
		extern RrPFNAllocateMemory AllocateMemory;
		extern RrPFNDeviceMemoryGetMemoryTypeIndex DeviceMemoryGetMemoryTypeIndex;
		extern RrPFNMapMemory MapMemory;
		extern RrPFNUnmapMemory UnmapMemory;
		extern RrPFNFlushMappedMemoryRanges FlushMappedMemoryRanges;
		extern RrPFNFreeMemory FreeMemory;
		extern RrPFNCreateBuffer CreateBuffer;
		extern RrPFNBufferGetMemoryRequirements BufferGetMemoryRequirements;
		extern RrPFNBindBufferMemory BindBufferMemory;
		extern RrPFNDestroyBuffer DestroyBuffer;
		extern RrPFNCreateCommandPool CreateCommandPool;
		extern RrPFNDestroyCommandPool DestroyCommandPool;
		extern RrPFNCreateLogicalDevice CreateLogicalDevice;
		extern RrPFNGetDeviceQueue GetDeviceQueue;
		extern RrPFNDestroyLogicalDevice DestroyLogicalDevice;
		extern RrPFNGetDeviceProcAddr GetDeviceProcAddr;
		extern RrPFNDeviceWaitIdle DeviceWaitIdle;

	}  // namespace RendererAPI

	RAY_EXPORT void LoadRenderer(RendererAPI::Type type);
}  // namespace At0::Ray

