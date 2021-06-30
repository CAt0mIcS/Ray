
#pragma once

#include "Ray/RBase.h"
#include <D:/dev/Cpp/Projects/Ray/RayRenderer/Core/RBuffer.h>
#include <D:/dev/Cpp/Projects/Ray/RayRenderer/Core/RCommandPool.h>
#include <D:/dev/Cpp/Projects/Ray/RayRenderer/Core/RDeviceMemory.h>
#include <D:/dev/Cpp/Projects/Ray/RayRenderer/Core/RInstance.h>
#include <D:/dev/Cpp/Projects/Ray/RayRenderer/Core/RLogicalDevice.h>
#include <D:/dev/Cpp/Projects/Ray/RayRenderer/Core/RPhysicalDevice.h>



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
		extern RrPFNCreateBuffer CreateBuffer;
		extern RrPFNBufferGetMemoryRequirements BufferGetMemoryRequirements;
		extern RrPFNBindBufferMemory BindBufferMemory;
		extern RrPFNCreateCommandPool CreateCommandPool;
		extern RrPFNDestroyCommandPool DestroyCommandPool;
		extern RrPFNAllocateMemory AllocateMemory;
		extern RrPFNDeviceMemoryGetMemoryTypeIndex DeviceMemoryGetMemoryTypeIndex;
		extern RrPFNMapMemory MapMemory;
		extern RrPFNUnmapMemory UnmapMemory;
		extern RrPFNFlushMappedMemoryRanges FlushMappedMemoryRanges;
		extern RrPFNInitialize Initialize;
		extern RrPFNDestroyInstance DestroyInstance;
		extern RrPFNGetInstanceProcAddr GetInstanceProcAddr;
		extern RrPFNCreateLogicalDevice CreateLogicalDevice;
		extern RrPFNGetDeviceQueue GetDeviceQueue;
		extern RrPFNDestroyLogicalDevice DestroyLogicalDevice;
		extern RrPFNGetDeviceProcAddr GetDeviceProcAddr;
		extern RrPFNDeviceWaitIdle DeviceWaitIdle;
		extern RrPFNEnumeratePhysicalDevice EnumeratePhysicalDevice;
		extern RrPFNGetPhysicalDeviceProperties GetPhysicalDeviceProperties;
		extern RrPFNGetPhysicalDeviceFeatures GetPhysicalDeviceFeatures;
		extern RrPFNGetPhysicalDeviceMemoryProperties GetPhysicalDeviceMemoryProperties;

	}  // namespace RendererAPI

	RAY_EXPORT void LoadRenderer(RendererAPI::Type type);
}  // namespace At0::Ray

