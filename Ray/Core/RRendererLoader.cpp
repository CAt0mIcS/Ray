

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
		RrError (*CreateBuffer)(RrLogicalDevice pDevice, const RrBufferCreateInfo* const pCreateInfo, RrBuffer* ppBuffer) = nullptr;
		void (*BufferGetMemoryRequirements)(RrLogicalDevice pDevice, const RrBuffer pBuffer, RrMemoryRequirements* const pMemRequirements) = nullptr;
		RrError (*BindBufferMemory)(RrLogicalDevice pDevice, RrBuffer buffer, RrDeviceMemory memory, RrDeviceSize memoryOffset) = nullptr;
		void (*DestroyBuffer)(RrLogicalDevice device, RrBuffer buffer) = nullptr;
		RrError (*AllocateCommandBuffers)(RrLogicalDevice device,RrCommandBufferAllocateInfo* pAllocInfo, RrCommandBuffer* pCommandBuffers) = nullptr;
		RrError (*BeginCommandBuffer)(RrCommandBuffer commandBuffer, RrCommandBufferBeginInfo* pBeginInfo) = nullptr;
		void (*ExecuteCommands)(RrCommandBuffer commandBuffer, uint32_t secondaryCommandBufferCount,const RrCommandBuffer* pSecondaryCommandBuffers) = nullptr;
		RrError (*QueueSubmit)(RrQueue queue, uint32_t submitCount, const RrSubmitInfo* pSubmits, RrFence fence) = nullptr;
		RrError (*QueueWaitIdle)(RrQueue queue) = nullptr;
		RrError (*EndCommandBuffer)(RrCommandBuffer commandBuffer) = nullptr;
		void (*FreeCommandBuffers)(RrLogicalDevice device, RrCommandPool commandPool,uint32_t commandBufferCount, const RrCommandBuffer* pCommandBuffers) = nullptr;
		RrError (*CreateCommandPool)(RrLogicalDevice pDevice,const RrCommandPoolCreateInfo* const pCreateInfo, RrCommandPool* ppCommandPool) = nullptr;
		void (*DestroyCommandPool)(RrLogicalDevice pDevice, RrCommandPool pCommandPool) = nullptr;
		RrError (*AllocateMemory)(RrLogicalDevice pDevice,const RrMemoryAllocateInfo* const pAllocateInfo, RrDeviceMemory* ppMemory) = nullptr;
		RrError (*DeviceMemoryGetMemoryTypeIndex)(uint32_t memoryTypeBits,RrMemoryPropertyFlags memoryProperties, uint32_t memoryTypeCount,const RrMemoryType* const pMemoryTypes, uint32_t* pTypeIndex) = nullptr;
		RrError (*MapMemory)(RrLogicalDevice pDevice, RrDeviceMemory pMemory, RrDeviceSize offset,RrDeviceSize size, void** mapped) = nullptr;
		void (*UnmapMemory)(RrLogicalDevice pDevice, RrDeviceMemory pMemory) = nullptr;
		RrError (*FlushMappedMemoryRanges)(RrLogicalDevice pDevice, uint32_t memoryRangeCount, RrMappedMemoryRange* pMappedMemoryRanges) = nullptr;
		void (*FreeMemory)(RrLogicalDevice device, RrDeviceMemory memory) = nullptr;
		RrError (*Initialize)(RrInitializeInfo* const pInitInfo, RrInstance* pInstance, RrDebugMessenger* pDebugMessenger) = nullptr;
		void (*DestroyInstance)(RrInstance pInstance, RrDebugMessenger pDebugMessenger) = nullptr;
		RrPFNVoidFunction (*GetInstanceProcAddr)(RrInstance instance, const char* pName) = nullptr;
		RrError (*CreateLogicalDevice)(RrPhysicalDevice physicalDevice,RrLogicalDeviceCreateInfo* pCreateInfo, RrLogicalDevice* pDevice) = nullptr;
		void (*GetDeviceQueue)(RrLogicalDevice device, uint32_t queueFamilyIndex, uint32_t queueIndex, RrQueue* pQueue) = nullptr;
		void (*DestroyLogicalDevice)(RrLogicalDevice device) = nullptr;
		RrPFNVoidFunction (*GetDeviceProcAddr)(RrLogicalDevice device, const char* pName) = nullptr;
		RrError (*DeviceWaitIdle)(RrLogicalDevice device) = nullptr;
		RrError (*EnumeratePhysicalDevice)(RrInstance pInstance,const RrPhysicalDeviceEnumerationInfo* const pCreateInfo, RrPhysicalDevice* ppPhysicalDevice) = nullptr;
		void (*GetPhysicalDeviceProperties)(RrPhysicalDevice pDevice, RrPhysicalDeviceProperties* pProperties) = nullptr;
		void (*GetPhysicalDeviceFeatures)(RrPhysicalDevice pDevice, RrPhysicalDeviceFeatures* pFeatures) = nullptr;
		void (*GetPhysicalDeviceMemoryProperties)(RrPhysicalDevice pDevice, RrPhysicalDeviceMemoryProperties* pProperties) = nullptr;
		RrError (*CreateSurface)(RrInstance instance, RrSurfaceCreateInfo* pCreateInfo, RrSurface* pSurface) = nullptr;

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

		RendererAPI::CreateBuffer = (RrError (*)(RrLogicalDevice pDevice, const RrBufferCreateInfo* const pCreateInfo, RrBuffer* ppBuffer))LoadFunction(lib, "RrCreateBuffer");
		RendererAPI::BufferGetMemoryRequirements = (void (*)(RrLogicalDevice pDevice, const RrBuffer pBuffer, RrMemoryRequirements* const pMemRequirements))LoadFunction(lib, "RrBufferGetMemoryRequirements");
		RendererAPI::BindBufferMemory = (RrError (*)(RrLogicalDevice pDevice, RrBuffer buffer, RrDeviceMemory memory, RrDeviceSize memoryOffset))LoadFunction(lib, "RrBindBufferMemory");
		RendererAPI::DestroyBuffer = (void (*)(RrLogicalDevice device, RrBuffer buffer))LoadFunction(lib, "RrDestroyBuffer");
		RendererAPI::AllocateCommandBuffers = (RrError (*)(RrLogicalDevice device,RrCommandBufferAllocateInfo* pAllocInfo, RrCommandBuffer* pCommandBuffers))LoadFunction(lib, "RrAllocateCommandBuffers");
		RendererAPI::BeginCommandBuffer = (RrError (*)(RrCommandBuffer commandBuffer, RrCommandBufferBeginInfo* pBeginInfo))LoadFunction(lib, "RrBeginCommandBuffer");
		RendererAPI::ExecuteCommands = (void (*)(RrCommandBuffer commandBuffer, uint32_t secondaryCommandBufferCount,const RrCommandBuffer* pSecondaryCommandBuffers))LoadFunction(lib, "RrExecuteCommands");
		RendererAPI::QueueSubmit = (RrError (*)(RrQueue queue, uint32_t submitCount, const RrSubmitInfo* pSubmits, RrFence fence))LoadFunction(lib, "RrQueueSubmit");
		RendererAPI::QueueWaitIdle = (RrError (*)(RrQueue queue))LoadFunction(lib, "RrQueueWaitIdle");
		RendererAPI::EndCommandBuffer = (RrError (*)(RrCommandBuffer commandBuffer))LoadFunction(lib, "RrEndCommandBuffer");
		RendererAPI::FreeCommandBuffers = (void (*)(RrLogicalDevice device, RrCommandPool commandPool,uint32_t commandBufferCount, const RrCommandBuffer* pCommandBuffers))LoadFunction(lib, "RrFreeCommandBuffers");
		RendererAPI::CreateCommandPool = (RrError (*)(RrLogicalDevice pDevice,const RrCommandPoolCreateInfo* const pCreateInfo, RrCommandPool* ppCommandPool))LoadFunction(lib, "RrCreateCommandPool");
		RendererAPI::DestroyCommandPool = (void (*)(RrLogicalDevice pDevice, RrCommandPool pCommandPool))LoadFunction(lib, "RrDestroyCommandPool");
		RendererAPI::AllocateMemory = (RrError (*)(RrLogicalDevice pDevice,const RrMemoryAllocateInfo* const pAllocateInfo, RrDeviceMemory* ppMemory))LoadFunction(lib, "RrAllocateMemory");
		RendererAPI::DeviceMemoryGetMemoryTypeIndex = (RrError (*)(uint32_t memoryTypeBits,RrMemoryPropertyFlags memoryProperties, uint32_t memoryTypeCount,const RrMemoryType* const pMemoryTypes, uint32_t* pTypeIndex))LoadFunction(lib, "RrDeviceMemoryGetMemoryTypeIndex");
		RendererAPI::MapMemory = (RrError (*)(RrLogicalDevice pDevice, RrDeviceMemory pMemory, RrDeviceSize offset,RrDeviceSize size, void** mapped))LoadFunction(lib, "RrMapMemory");
		RendererAPI::UnmapMemory = (void (*)(RrLogicalDevice pDevice, RrDeviceMemory pMemory))LoadFunction(lib, "RrUnmapMemory");
		RendererAPI::FlushMappedMemoryRanges = (RrError (*)(RrLogicalDevice pDevice, uint32_t memoryRangeCount, RrMappedMemoryRange* pMappedMemoryRanges))LoadFunction(lib, "RrFlushMappedMemoryRanges");
		RendererAPI::FreeMemory = (void (*)(RrLogicalDevice device, RrDeviceMemory memory))LoadFunction(lib, "RrFreeMemory");
		RendererAPI::Initialize = (RrError (*)(RrInitializeInfo* const pInitInfo, RrInstance* pInstance, RrDebugMessenger* pDebugMessenger))LoadFunction(lib, "RrInitialize");
		RendererAPI::DestroyInstance = (void (*)(RrInstance pInstance, RrDebugMessenger pDebugMessenger))LoadFunction(lib, "RrDestroyInstance");
		RendererAPI::GetInstanceProcAddr = (RrPFNVoidFunction (*)(RrInstance instance, const char* pName))LoadFunction(lib, "RrGetInstanceProcAddr");
		RendererAPI::CreateLogicalDevice = (RrError (*)(RrPhysicalDevice physicalDevice,RrLogicalDeviceCreateInfo* pCreateInfo, RrLogicalDevice* pDevice))LoadFunction(lib, "RrCreateLogicalDevice");
		RendererAPI::GetDeviceQueue = (void (*)(RrLogicalDevice device, uint32_t queueFamilyIndex, uint32_t queueIndex, RrQueue* pQueue))LoadFunction(lib, "RrGetDeviceQueue");
		RendererAPI::DestroyLogicalDevice = (void (*)(RrLogicalDevice device))LoadFunction(lib, "RrDestroyLogicalDevice");
		RendererAPI::GetDeviceProcAddr = (RrPFNVoidFunction (*)(RrLogicalDevice device, const char* pName))LoadFunction(lib, "RrGetDeviceProcAddr");
		RendererAPI::DeviceWaitIdle = (RrError (*)(RrLogicalDevice device))LoadFunction(lib, "RrDeviceWaitIdle");
		RendererAPI::EnumeratePhysicalDevice = (RrError (*)(RrInstance pInstance,const RrPhysicalDeviceEnumerationInfo* const pCreateInfo, RrPhysicalDevice* ppPhysicalDevice))LoadFunction(lib, "RrEnumeratePhysicalDevice");
		RendererAPI::GetPhysicalDeviceProperties = (void (*)(RrPhysicalDevice pDevice, RrPhysicalDeviceProperties* pProperties))LoadFunction(lib, "RrGetPhysicalDeviceProperties");
		RendererAPI::GetPhysicalDeviceFeatures = (void (*)(RrPhysicalDevice pDevice, RrPhysicalDeviceFeatures* pFeatures))LoadFunction(lib, "RrGetPhysicalDeviceFeatures");
		RendererAPI::GetPhysicalDeviceMemoryProperties = (void (*)(RrPhysicalDevice pDevice, RrPhysicalDeviceMemoryProperties* pProperties))LoadFunction(lib, "RrGetPhysicalDeviceMemoryProperties");
		RendererAPI::CreateSurface = (RrError (*)(RrInstance instance, RrSurfaceCreateInfo* pCreateInfo, RrSurface* pSurface))LoadFunction(lib, "RrCreateSurface");


		RendererAPI::API = type;
	}
}  // namespace At0::Ray

