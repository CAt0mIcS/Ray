

#include "Rpch.h"
#include "RRendererLoader.h"

#include <RayBase/RException.h>

#ifdef _WIN32
    #include <Windows.h>
#else
    #include <dlfcn.h>
#endif


namespace At0::Ray
{
	namespace RendererAPI
	{
		Type API;
		RrPFNCreateBuffer CreateBuffer = nullptr;
		RrPFNBufferGetMemoryRequirements BufferGetMemoryRequirements = nullptr;
		RrPFNBindBufferMemory BindBufferMemory = nullptr;
		RrPFNDestroyBuffer DestroyBuffer = nullptr;
		RrPFNAllocateCommandBuffers AllocateCommandBuffers = nullptr;
		RrPFNBeginCommandBuffer BeginCommandBuffer = nullptr;
		RrPFNExecuteCommands ExecuteCommands = nullptr;
		RrPFNQueueSubmit QueueSubmit = nullptr;
		RrPFNQueueWaitIdle QueueWaitIdle = nullptr;
		RrPFNEndCommandBuffer EndCommandBuffer = nullptr;
		RrPFNFreeCommandBuffers FreeCommandBuffers = nullptr;
		RrPFNCreateCommandPool CreateCommandPool = nullptr;
		RrPFNDestroyCommandPool DestroyCommandPool = nullptr;
		RrPFNAllocateMemory AllocateMemory = nullptr;
		RrPFNDeviceMemoryGetMemoryTypeIndex DeviceMemoryGetMemoryTypeIndex = nullptr;
		RrPFNMapMemory MapMemory = nullptr;
		RrPFNUnmapMemory UnmapMemory = nullptr;
		RrPFNFlushMappedMemoryRanges FlushMappedMemoryRanges = nullptr;
		RrPFNFreeMemory FreeMemory = nullptr;
		RrPFNInitialize Initialize = nullptr;
		RrPFNDestroyInstance DestroyInstance = nullptr;
		RrPFNGetInstanceProcAddr GetInstanceProcAddr = nullptr;
		RrPFNCreateLogicalDevice CreateLogicalDevice = nullptr;
		RrPFNGetDeviceQueue GetDeviceQueue = nullptr;
		RrPFNDestroyLogicalDevice DestroyLogicalDevice = nullptr;
		RrPFNGetDeviceProcAddr GetDeviceProcAddr = nullptr;
		RrPFNDeviceWaitIdle DeviceWaitIdle = nullptr;
		RrPFNEnumeratePhysicalDevice EnumeratePhysicalDevice = nullptr;
		RrPFNGetPhysicalDeviceProperties GetPhysicalDeviceProperties = nullptr;
		RrPFNGetPhysicalDeviceFeatures GetPhysicalDeviceFeatures = nullptr;
		RrPFNGetPhysicalDeviceMemoryProperties GetPhysicalDeviceMemoryProperties = nullptr;
		RrPFNCreateSurface CreateSurface = nullptr;

	}  // namespace RendererAPI


    template<typename... Args>
	void* LoadFunction(Args&&... args)
	{
#ifdef _WIN32
		return GetProcAddress(std::forward<Args>(args)...);
#else
		return dlsym(std::forward<Args>(args)...);
#endif
	}

	void LoadRenderer(RendererAPI::Type type)
	{
		// RAY_TODO: Make platform independent

#ifdef _WIN32
		HMODULE lib = nullptr;
        switch (type)
		{
		case RendererAPI::OpenGL: lib = LoadLibraryA("RayRendererOpenGL.dll"); break;
		case RendererAPI::Vulkan: lib = LoadLibraryA("RayRendererVulkan.dll"); break;
		default: ThrowRuntime("Invalid renderer type {0}", (uint32_t)type);
		}
#else
        void* lib = nullptr;
        switch (type)
		{
		case RendererAPI::OpenGL: lib = dlopen("libRayRendererOpenGL.so", RTLD_LAZY); break;
		case RendererAPI::Vulkan: lib = dlopen("libRayRendererVulkan.so", RTLD_LAZY); break;
		default: ThrowRuntime("Invalid renderer type {0}", (uint32_t)type);
		}
#endif


		if (!lib)
		{
			std::string rendererStr;
			switch (type)
			{
			case RendererAPI::OpenGL: rendererStr = "OpenGL"; break;
			case RendererAPI::Vulkan: rendererStr = "Vulkan"; break;
			}

			ThrowRuntime("[Loader] Failed to load {0} renderer", rendererStr);
		}

		RendererAPI::CreateBuffer = (RrPFNCreateBuffer)LoadFunction(lib, "RrCreateBuffer");
		RendererAPI::BufferGetMemoryRequirements = (RrPFNBufferGetMemoryRequirements)LoadFunction(lib, "RrBufferGetMemoryRequirements");
		RendererAPI::BindBufferMemory = (RrPFNBindBufferMemory)LoadFunction(lib, "RrBindBufferMemory");
		RendererAPI::DestroyBuffer = (RrPFNDestroyBuffer)LoadFunction(lib, "RrDestroyBuffer");
		RendererAPI::AllocateCommandBuffers = (RrPFNAllocateCommandBuffers)LoadFunction(lib, "RrAllocateCommandBuffers");
		RendererAPI::BeginCommandBuffer = (RrPFNBeginCommandBuffer)LoadFunction(lib, "RrBeginCommandBuffer");
		RendererAPI::ExecuteCommands = (RrPFNExecuteCommands)LoadFunction(lib, "RrExecuteCommands");
		RendererAPI::QueueSubmit = (RrPFNQueueSubmit)LoadFunction(lib, "RrQueueSubmit");
		RendererAPI::QueueWaitIdle = (RrPFNQueueWaitIdle)LoadFunction(lib, "RrQueueWaitIdle");
		RendererAPI::EndCommandBuffer = (RrPFNEndCommandBuffer)LoadFunction(lib, "RrEndCommandBuffer");
		RendererAPI::FreeCommandBuffers = (RrPFNFreeCommandBuffers)LoadFunction(lib, "RrFreeCommandBuffers");
		RendererAPI::CreateCommandPool = (RrPFNCreateCommandPool)LoadFunction(lib, "RrCreateCommandPool");
		RendererAPI::DestroyCommandPool = (RrPFNDestroyCommandPool)LoadFunction(lib, "RrDestroyCommandPool");
		RendererAPI::AllocateMemory = (RrPFNAllocateMemory)LoadFunction(lib, "RrAllocateMemory");
		RendererAPI::DeviceMemoryGetMemoryTypeIndex = (RrPFNDeviceMemoryGetMemoryTypeIndex)LoadFunction(lib, "RrDeviceMemoryGetMemoryTypeIndex");
		RendererAPI::MapMemory = (RrPFNMapMemory)LoadFunction(lib, "RrMapMemory");
		RendererAPI::UnmapMemory = (RrPFNUnmapMemory)LoadFunction(lib, "RrUnmapMemory");
		RendererAPI::FlushMappedMemoryRanges = (RrPFNFlushMappedMemoryRanges)LoadFunction(lib, "RrFlushMappedMemoryRanges");
		RendererAPI::FreeMemory = (RrPFNFreeMemory)LoadFunction(lib, "RrFreeMemory");
		RendererAPI::Initialize = (RrPFNInitialize)LoadFunction(lib, "RrInitialize");
		RendererAPI::DestroyInstance = (RrPFNDestroyInstance)LoadFunction(lib, "RrDestroyInstance");
		RendererAPI::GetInstanceProcAddr = (RrPFNGetInstanceProcAddr)LoadFunction(lib, "RrGetInstanceProcAddr");
		RendererAPI::CreateLogicalDevice = (RrPFNCreateLogicalDevice)LoadFunction(lib, "RrCreateLogicalDevice");
		RendererAPI::GetDeviceQueue = (RrPFNGetDeviceQueue)LoadFunction(lib, "RrGetDeviceQueue");
		RendererAPI::DestroyLogicalDevice = (RrPFNDestroyLogicalDevice)LoadFunction(lib, "RrDestroyLogicalDevice");
		RendererAPI::GetDeviceProcAddr = (RrPFNGetDeviceProcAddr)LoadFunction(lib, "RrGetDeviceProcAddr");
		RendererAPI::DeviceWaitIdle = (RrPFNDeviceWaitIdle)LoadFunction(lib, "RrDeviceWaitIdle");
		RendererAPI::EnumeratePhysicalDevice = (RrPFNEnumeratePhysicalDevice)LoadFunction(lib, "RrEnumeratePhysicalDevice");
		RendererAPI::GetPhysicalDeviceProperties = (RrPFNGetPhysicalDeviceProperties)LoadFunction(lib, "RrGetPhysicalDeviceProperties");
		RendererAPI::GetPhysicalDeviceFeatures = (RrPFNGetPhysicalDeviceFeatures)LoadFunction(lib, "RrGetPhysicalDeviceFeatures");
		RendererAPI::GetPhysicalDeviceMemoryProperties = (RrPFNGetPhysicalDeviceMemoryProperties)LoadFunction(lib, "RrGetPhysicalDeviceMemoryProperties");
		RendererAPI::CreateSurface = (RrPFNCreateSurface)LoadFunction(lib, "RrCreateSurface");


		RendererAPI::API = type;
	}
}  // namespace At0::Ray

