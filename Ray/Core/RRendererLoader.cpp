

#include "Rpch.h"
#include "RRendererLoader.h"

#include "Utils/RException.h"

#ifdef _WIN32
    #include <Windows.h>
    #ifdef CreateSemaphore
        #undef CreateSemaphore
    #endif
#else
    #include <dlfcn.h>
#endif


namespace At0::Ray
{
	namespace RendererAPI
	{
		Type API;
		RrError (*CreateBuffer)(RrLogicalDevice device, const RrBufferCreateInfo* pCreateInfo, RrBuffer* pBuffer) = nullptr;
		void (*BufferGetMemoryRequirements)(RrLogicalDevice device, RrBuffer buffer, RrMemoryRequirements* pMemRequirements) = nullptr;
		RrError (*BindBufferMemory)(RrLogicalDevice device, RrBuffer buffer, RrDeviceMemory memory, RrDeviceSize memoryOffset) = nullptr;
		void (*DestroyBuffer)(RrLogicalDevice device, RrBuffer buffer) = nullptr;
		void (*CmdBindIndexBuffer)(RrCommandBuffer commandBuffer, RrBuffer buffer, RrDeviceSize offset, RrIndexType indexType) = nullptr;
		void (*CmdBindVertexBuffers)(RrCommandBuffer commandBuffer, uint32_t firstBinding,uint32_t bindingCount, const RrBuffer* pBuffers, const RrDeviceSize* pOffsets) = nullptr;
		void (*CmdCopyBuffer)(RrCommandBuffer commandBuffer, RrBuffer srcBuffer, RrBuffer dstBuffer,uint32_t regionCount, const RrBufferCopy* pRegions) = nullptr;
		RrError (*AllocateCommandBuffers)(RrLogicalDevice device,RrCommandBufferAllocateInfo* pAllocInfo, RrCommandBuffer* pCommandBuffers) = nullptr;
		RrError (*BeginCommandBuffer)(RrCommandBuffer commandBuffer, RrCommandBufferBeginInfo* pBeginInfo) = nullptr;
		void (*ExecuteCommands)(RrCommandBuffer commandBuffer, uint32_t secondaryCommandBufferCount,const RrCommandBuffer* pSecondaryCommandBuffers) = nullptr;
		RrError (*QueueSubmit)(RrQueue queue, uint32_t submitCount, const RrSubmitInfo* pSubmits, RrFence fence) = nullptr;
		RrError (*QueueWaitIdle)(RrQueue queue) = nullptr;
		RrError (*EndCommandBuffer)(RrCommandBuffer commandBuffer) = nullptr;
		void (*FreeCommandBuffers)(RrLogicalDevice device, RrCommandPool commandPool,uint32_t commandBufferCount, const RrCommandBuffer* pCommandBuffers) = nullptr;
		RrError (*CreateCommandPool)(RrLogicalDevice device,const RrCommandPoolCreateInfo* pCreateInfo, RrCommandPool* pCommandPool) = nullptr;
		void (*DestroyCommandPool)(RrLogicalDevice device, RrCommandPool commandPool) = nullptr;
		RrError (*ResetCommandPool)(RrLogicalDevice device, RrCommandPool commandPool, RrCommandPoolResetFlags flags) = nullptr;
		RrError (*AllocateMemory)(RrLogicalDevice device, const RrMemoryAllocateInfo* pAllocateInfo, RrDeviceMemory* pMemory) = nullptr;
		RrError (*DeviceMemoryGetMemoryTypeIndex)(uint32_t memoryTypeBits,RrMemoryPropertyFlags memoryProperties, uint32_t memoryTypeCount,const RrMemoryType* pMemoryTypes, uint32_t* pTypeIndex) = nullptr;
		RrError (*MapMemory)(RrLogicalDevice device, RrDeviceMemory memory, RrDeviceSize offset,RrDeviceSize size, void** ppMapped) = nullptr;
		void (*UnmapMemory)(RrLogicalDevice device, RrDeviceMemory memory) = nullptr;
		RrError (*FlushMappedMemoryRanges)(RrLogicalDevice device, uint32_t memoryRangeCount,const RrMappedMemoryRange* pMappedMemoryRanges) = nullptr;
		void (*FreeMemory)(RrLogicalDevice device, RrDeviceMemory memory) = nullptr;
		RrError (*CreateFramebuffer)(RrLogicalDevice device,const RrFramebufferCreateInfo* pCreateInfo, RrFramebuffer* pFramebuffer) = nullptr;
		void (*DestroyFramebuffer)(RrLogicalDevice device, RrFramebuffer framebuffer) = nullptr;
		RrError (*CreateImage)(RrLogicalDevice device, const RrImageCreateInfo* pCreateInfo, RrImage* pImage) = nullptr;
		void (*ImageGetMemoryRequirements)(RrLogicalDevice device, RrImage image, RrMemoryRequirements* pMemRequirements) = nullptr;
		RrError (*BindImageMemory)(RrLogicalDevice device, RrImage image, RrDeviceMemory memory, RrDeviceSize memoryOffset) = nullptr;
		void (*GetImageSubresourceLayout)(RrLogicalDevice device, RrImage image,RrImageSubresource* pSubresource, RrSubresourceLayout* pLayout) = nullptr;
		void (*DestroyImage)(RrLogicalDevice device, RrImage image) = nullptr;
		void (*CmdBlitImage)(RrCommandBuffer commandBuffer, RrImage srcImage,RrImageLayout srcImageLayout, RrImage dstImage, RrImageLayout dstImageLayout,uint32_t regionCount, const RrImageBlit* pRegions, RrFilter filter) = nullptr;
		void (*CmdCopyBufferToImage)(RrCommandBuffer commandBuffer, RrBuffer srcBuffer,RrImage dstImage, RrImageLayout dstImageLayout, uint32_t regionCount,const RrBufferImageCopy* pRegions) = nullptr;
		RrError (*CreateImageView)(RrLogicalDevice device, const RrImageViewCreateInfo* pCreateInfo, RrImageView* pImageView) = nullptr;
		void (*DestroyImageView)(RrLogicalDevice device, RrImageView imageView) = nullptr;
		RrError (*Initialize)(RrInitializeInfo* pInitInfo, RrInstance* pInstance, RrDebugMessenger* pDebugMessenger) = nullptr;
		void (*DestroyInstance)(RrInstance instance, RrDebugMessenger debugMessenger) = nullptr;
		RrPFNVoidFunction (*GetInstanceProcAddr)(RrInstance instance, const char* pName) = nullptr;
		RrError (*EnumerateInstanceExtensionProperties)(const char* pLayerName, uint32_t* pPropertyCount, RrExtensionProperties* pProperties) = nullptr;
		RrError (*CreateLogicalDevice)(RrPhysicalDevice physicalDevice,RrLogicalDeviceCreateInfo* pCreateInfo, RrLogicalDevice* pDevice) = nullptr;
		void (*GetDeviceQueue)(RrLogicalDevice device, uint32_t queueFamilyIndex, uint32_t queueIndex, RrQueue* pQueue) = nullptr;
		void (*DestroyLogicalDevice)(RrLogicalDevice device) = nullptr;
		RrPFNVoidFunction (*GetDeviceProcAddr)(RrLogicalDevice device, const char* pName) = nullptr;
		RrError (*DeviceWaitIdle)(RrLogicalDevice device) = nullptr;
		RrError (*EnumeratePhysicalDevice)(RrInstance instance,const RrPhysicalDeviceEnumerationInfo* pCreateInfo, RrPhysicalDevice* pPhysicalDevice) = nullptr;
		void (*GetPhysicalDeviceProperties)(RrPhysicalDevice physicalDevice, RrPhysicalDeviceProperties* pProperties) = nullptr;
		void (*GetPhysicalDeviceFeatures)(RrPhysicalDevice physicalDevice, RrPhysicalDeviceFeatures* pFeatures) = nullptr;
		void (*GetPhysicalDeviceMemoryProperties)(RrPhysicalDevice physicalDevice, RrPhysicalDeviceMemoryProperties* pProperties) = nullptr;
		void (*GetPhysicalDeviceFormatProperties)(RrPhysicalDevice physicalDevice, RrFormat format, RrFormatProperties* pProperties) = nullptr;
		void (*GetPhysicalDeviceSurfaceCapabilitiesKHR)(RrPhysicalDevice physicalDevice,RrSurfaceKHR surface, RrSurfaceCapabilitiesKHR* pSurfaceCapabilities) = nullptr;
		void (*GetPhysicalDeviceSurfaceFormatsKHR)(RrPhysicalDevice physicalDevice,RrSurfaceKHR surface, uint32_t* pSurfaceFormatCount, RrSurfaceFormatKHR* pSurfaceFormats) = nullptr;
		void (*GetPhysicalDeviceSurfacePresentModesKHR)(RrPhysicalDevice physicalDevice,RrSurfaceKHR surface, uint32_t* pPresentModeCount, RrPresentModeKHR* pPresentModes) = nullptr;
		void (*GetPhysicalDeviceQueueFamilyProperties)(RrPhysicalDevice physicalDevice,uint32_t* pQueueFamilyPropertyCount, RrQueueFamilyProperties* pQueueFamilyProperties) = nullptr;
		RrError (*GetPhysicalDeviceSurfaceSupportKHR)(RrPhysicalDevice physicalDevice,uint32_t queueFamilyIndex, RrSurfaceKHR surface, RrBool32* pSupported) = nullptr;
		RrError (*CreateRenderPass)(RrLogicalDevice device, const RrRenderPassCreateInfo* pCreateInfo, RrRenderPass* pRenderPass) = nullptr;
		void (*DestroyRenderPass)(RrLogicalDevice device, RrRenderPass renderPass) = nullptr;
		void (*CmdBeginRenderPass)(RrCommandBuffer commandBuffer,const RrRenderPassBeginInfo* pBeginInfo, RrSubpassContents subpassContents) = nullptr;
		void (*CmdEndRenderPass)(RrCommandBuffer commandBuffer) = nullptr;
		RrError (*CreateSampler)(RrLogicalDevice device, const RrSamplerCreateInfo* pCreateInfo, RrSampler* pSampler) = nullptr;
		void (*DestroySampler)(RrLogicalDevice device, RrSampler sampler) = nullptr;
		RrError (*CreateSurfaceKHR)(RrInstance instance, const RrSurfaceCreateInfoKHR* pCreateInfo, RrSurfaceKHR* pSurface) = nullptr;
		void (*DestroySurfaceKHR)(RrInstance instance, RrSurfaceKHR surface) = nullptr;
		RrError (*CreateSwapchainKHR)(RrLogicalDevice device,const RrSwapchainCreateInfoKHR* pCreateInfo, RrSwapchainKHR* pSwapchain) = nullptr;
		RrError (*GetSwapchainImagesKHR)(RrLogicalDevice device, RrSwapchainKHR swapchain,uint32_t* pSwapchainImageCount, RrImage* pSwapchainImages) = nullptr;
		void (*DestroySwapchainKHR)(RrLogicalDevice device, RrSwapchainKHR swapchain) = nullptr;
		RrError (*AcquireNextImageKHR)(RrLogicalDevice device, RrSwapchainKHR swapchain,uint64_t timeout, RrSemaphore semaphore, RrFence fence, uint32_t* pImageIndex) = nullptr;
		RrError (*QueuePresentKHR)(RrQueue queue, const RrPresentInfoKHR* pPresentInfo) = nullptr;
		RrError (*CreateFence)(RrLogicalDevice device, const RrFenceCreateInfo* pCreateInfo, RrFence* pFence) = nullptr;
		RrError (*WaitForFences)(RrLogicalDevice device, uint32_t fenceCount, const RrFence* pFences,RrBool32 waitAll, uint64_t timeout) = nullptr;
		RrError (*ResetFences)(RrLogicalDevice device, uint32_t fenceCount, const RrFence* pFences) = nullptr;
		void (*DestroyFence)(RrLogicalDevice device, RrFence fence) = nullptr;
		RrError (*CreateSemaphore)(RrLogicalDevice device, RrSemaphore* pSemaphore) = nullptr;
		void (*DestroySemaphore)(RrLogicalDevice device, RrSemaphore semaphore) = nullptr;
		RrError (*CreateDescriptorSetLayout)(RrLogicalDevice device,const RrDescriptorSetLayoutCreateInfo* pCreateInfo, RrDescriptorSetLayout* pLayout) = nullptr;
		RrError (*CreateDescriptorPool)(RrLogicalDevice device, const RrDescriptorPoolCreateInfo* pCreateInfo, RrDescriptorPool* pPool) = nullptr;
		RrError (*AllocateDescriptorSets)(RrLogicalDevice device,const RrDescriptorSetAllocateInfo* pAllocInfo, RrDescriptorSet* pDescriptorSets) = nullptr;
		void (*UpdateDescriptorSets)(RrLogicalDevice device, uint32_t descriptorWriteCount,const RrWriteDescriptorSet* pDescriptorWrites, uint32_t descriptorCopyCount,const RrCopyDescriptorSet* pDescriptorCopies) = nullptr;
		void (*DestroyDescriptorPool)(RrLogicalDevice device, RrDescriptorPool pool) = nullptr;
		void (*DestroyDescriptorSetLayout)(RrLogicalDevice device, RrDescriptorSetLayout layout) = nullptr;
		void (*CmdBindDescriptorSets)(RrCommandBuffer commandBuffer,RrPipelineBindPoint pipelineBindPoint, RrPipelineLayout pipelineLayout, uint32_t firstSet,uint32_t descriptorSetCount, const RrDescriptorSet* pDescriptorSets,uint32_t dynamicOffsetCount, const uint32_t* pDynamicOffsets) = nullptr;
		RrError (*CreatePipelineLayout)(RrLogicalDevice device,const RrPipelineLayoutCreateInfo* pCreateInfo, RrPipelineLayout* pLayout) = nullptr;
		RrError (*CreateGraphicsPipeline)(RrLogicalDevice device, RrPipelineCache pipelineCache,const RrGraphicsPipelineCreateInfo* pCreateInfo, RrPipeline* pPipeline) = nullptr;
		void (*DestroyPipelineLayout)(RrLogicalDevice device, RrPipelineLayout pipelineLayout) = nullptr;
		void (*DestroyPipeline)(RrLogicalDevice device, RrPipeline pipeline) = nullptr;
		void (*CmdBindPipeline)(RrCommandBuffer commandBuffer, RrPipelineBindPoint pipelineBindPoint, RrPipeline pipeline) = nullptr;
		void (*CmdSetViewport)(RrCommandBuffer commandBuffer, uint32_t firstViewport,uint32_t viewportCount, const RrViewport* pViewports) = nullptr;
		void (*CmdSetScissor)(RrCommandBuffer commandBuffer, uint32_t firstScissor,uint32_t scissorCount, const RrRect2D* pScissors) = nullptr;
		void (*CmdDrawIndexed)(RrCommandBuffer commandBuffer, uint32_t indexCount,uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance) = nullptr;
		void (*CmdPipelineBarrier)(RrCommandBuffer commandBuffer, RrPipelineStageFlags srcStageMask,RrPipelineStageFlags dstStageMask, RrDependencyFlags dependencyFlags,uint32_t memoryBarrierCount, const RrMemoryBarrier* pMemoryBarriers,uint32_t bufferMemoryBarrierCount, const RrBufferMemoryBarrier* pBufferMemoryBarriers,uint32_t imageMemoryBarrierCount, const RrImageMemoryBarrier* pImageMemoryBarriers) = nullptr;
		RrError (*CreateShaderModule)(RrLogicalDevice device,const RrShaderModuleCreateInfo* pCreateInfo, RrShaderModule* pShaderModule) = nullptr;
		void (*DestroyShaderModule)(RrLogicalDevice device, RrShaderModule shaderModule) = nullptr;
		void (*CmdPushConstants)(RrCommandBuffer commandBuffer, RrPipelineLayout pipelineLayout,RrShaderStageFlags shaderStage, uint32_t offset, uint32_t size, const void* pValues) = nullptr;

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

		RendererAPI::CreateBuffer = (RrError (*)(RrLogicalDevice device, const RrBufferCreateInfo* pCreateInfo, RrBuffer* pBuffer))LoadFunction(lib, "RrCreateBuffer");
		RendererAPI::BufferGetMemoryRequirements = (void (*)(RrLogicalDevice device, RrBuffer buffer, RrMemoryRequirements* pMemRequirements))LoadFunction(lib, "RrBufferGetMemoryRequirements");
		RendererAPI::BindBufferMemory = (RrError (*)(RrLogicalDevice device, RrBuffer buffer, RrDeviceMemory memory, RrDeviceSize memoryOffset))LoadFunction(lib, "RrBindBufferMemory");
		RendererAPI::DestroyBuffer = (void (*)(RrLogicalDevice device, RrBuffer buffer))LoadFunction(lib, "RrDestroyBuffer");
		RendererAPI::CmdBindIndexBuffer = (void (*)(RrCommandBuffer commandBuffer, RrBuffer buffer, RrDeviceSize offset, RrIndexType indexType))LoadFunction(lib, "RrCmdBindIndexBuffer");
		RendererAPI::CmdBindVertexBuffers = (void (*)(RrCommandBuffer commandBuffer, uint32_t firstBinding,uint32_t bindingCount, const RrBuffer* pBuffers, const RrDeviceSize* pOffsets))LoadFunction(lib, "RrCmdBindVertexBuffers");
		RendererAPI::CmdCopyBuffer = (void (*)(RrCommandBuffer commandBuffer, RrBuffer srcBuffer, RrBuffer dstBuffer,uint32_t regionCount, const RrBufferCopy* pRegions))LoadFunction(lib, "RrCmdCopyBuffer");
		RendererAPI::AllocateCommandBuffers = (RrError (*)(RrLogicalDevice device,RrCommandBufferAllocateInfo* pAllocInfo, RrCommandBuffer* pCommandBuffers))LoadFunction(lib, "RrAllocateCommandBuffers");
		RendererAPI::BeginCommandBuffer = (RrError (*)(RrCommandBuffer commandBuffer, RrCommandBufferBeginInfo* pBeginInfo))LoadFunction(lib, "RrBeginCommandBuffer");
		RendererAPI::ExecuteCommands = (void (*)(RrCommandBuffer commandBuffer, uint32_t secondaryCommandBufferCount,const RrCommandBuffer* pSecondaryCommandBuffers))LoadFunction(lib, "RrExecuteCommands");
		RendererAPI::QueueSubmit = (RrError (*)(RrQueue queue, uint32_t submitCount, const RrSubmitInfo* pSubmits, RrFence fence))LoadFunction(lib, "RrQueueSubmit");
		RendererAPI::QueueWaitIdle = (RrError (*)(RrQueue queue))LoadFunction(lib, "RrQueueWaitIdle");
		RendererAPI::EndCommandBuffer = (RrError (*)(RrCommandBuffer commandBuffer))LoadFunction(lib, "RrEndCommandBuffer");
		RendererAPI::FreeCommandBuffers = (void (*)(RrLogicalDevice device, RrCommandPool commandPool,uint32_t commandBufferCount, const RrCommandBuffer* pCommandBuffers))LoadFunction(lib, "RrFreeCommandBuffers");
		RendererAPI::CreateCommandPool = (RrError (*)(RrLogicalDevice device,const RrCommandPoolCreateInfo* pCreateInfo, RrCommandPool* pCommandPool))LoadFunction(lib, "RrCreateCommandPool");
		RendererAPI::DestroyCommandPool = (void (*)(RrLogicalDevice device, RrCommandPool commandPool))LoadFunction(lib, "RrDestroyCommandPool");
		RendererAPI::ResetCommandPool = (RrError (*)(RrLogicalDevice device, RrCommandPool commandPool, RrCommandPoolResetFlags flags))LoadFunction(lib, "RrResetCommandPool");
		RendererAPI::AllocateMemory = (RrError (*)(RrLogicalDevice device, const RrMemoryAllocateInfo* pAllocateInfo, RrDeviceMemory* pMemory))LoadFunction(lib, "RrAllocateMemory");
		RendererAPI::DeviceMemoryGetMemoryTypeIndex = (RrError (*)(uint32_t memoryTypeBits,RrMemoryPropertyFlags memoryProperties, uint32_t memoryTypeCount,const RrMemoryType* pMemoryTypes, uint32_t* pTypeIndex))LoadFunction(lib, "RrDeviceMemoryGetMemoryTypeIndex");
		RendererAPI::MapMemory = (RrError (*)(RrLogicalDevice device, RrDeviceMemory memory, RrDeviceSize offset,RrDeviceSize size, void** ppMapped))LoadFunction(lib, "RrMapMemory");
		RendererAPI::UnmapMemory = (void (*)(RrLogicalDevice device, RrDeviceMemory memory))LoadFunction(lib, "RrUnmapMemory");
		RendererAPI::FlushMappedMemoryRanges = (RrError (*)(RrLogicalDevice device, uint32_t memoryRangeCount,const RrMappedMemoryRange* pMappedMemoryRanges))LoadFunction(lib, "RrFlushMappedMemoryRanges");
		RendererAPI::FreeMemory = (void (*)(RrLogicalDevice device, RrDeviceMemory memory))LoadFunction(lib, "RrFreeMemory");
		RendererAPI::CreateFramebuffer = (RrError (*)(RrLogicalDevice device,const RrFramebufferCreateInfo* pCreateInfo, RrFramebuffer* pFramebuffer))LoadFunction(lib, "RrCreateFramebuffer");
		RendererAPI::DestroyFramebuffer = (void (*)(RrLogicalDevice device, RrFramebuffer framebuffer))LoadFunction(lib, "RrDestroyFramebuffer");
		RendererAPI::CreateImage = (RrError (*)(RrLogicalDevice device, const RrImageCreateInfo* pCreateInfo, RrImage* pImage))LoadFunction(lib, "RrCreateImage");
		RendererAPI::ImageGetMemoryRequirements = (void (*)(RrLogicalDevice device, RrImage image, RrMemoryRequirements* pMemRequirements))LoadFunction(lib, "RrImageGetMemoryRequirements");
		RendererAPI::BindImageMemory = (RrError (*)(RrLogicalDevice device, RrImage image, RrDeviceMemory memory, RrDeviceSize memoryOffset))LoadFunction(lib, "RrBindImageMemory");
		RendererAPI::GetImageSubresourceLayout = (void (*)(RrLogicalDevice device, RrImage image,RrImageSubresource* pSubresource, RrSubresourceLayout* pLayout))LoadFunction(lib, "RrGetImageSubresourceLayout");
		RendererAPI::DestroyImage = (void (*)(RrLogicalDevice device, RrImage image))LoadFunction(lib, "RrDestroyImage");
		RendererAPI::CmdBlitImage = (void (*)(RrCommandBuffer commandBuffer, RrImage srcImage,RrImageLayout srcImageLayout, RrImage dstImage, RrImageLayout dstImageLayout,uint32_t regionCount, const RrImageBlit* pRegions, RrFilter filter))LoadFunction(lib, "RrCmdBlitImage");
		RendererAPI::CmdCopyBufferToImage = (void (*)(RrCommandBuffer commandBuffer, RrBuffer srcBuffer,RrImage dstImage, RrImageLayout dstImageLayout, uint32_t regionCount,const RrBufferImageCopy* pRegions))LoadFunction(lib, "RrCmdCopyBufferToImage");
		RendererAPI::CreateImageView = (RrError (*)(RrLogicalDevice device, const RrImageViewCreateInfo* pCreateInfo, RrImageView* pImageView))LoadFunction(lib, "RrCreateImageView");
		RendererAPI::DestroyImageView = (void (*)(RrLogicalDevice device, RrImageView imageView))LoadFunction(lib, "RrDestroyImageView");
		RendererAPI::Initialize = (RrError (*)(RrInitializeInfo* pInitInfo, RrInstance* pInstance, RrDebugMessenger* pDebugMessenger))LoadFunction(lib, "RrInitialize");
		RendererAPI::DestroyInstance = (void (*)(RrInstance instance, RrDebugMessenger debugMessenger))LoadFunction(lib, "RrDestroyInstance");
		RendererAPI::GetInstanceProcAddr = (RrPFNVoidFunction (*)(RrInstance instance, const char* pName))LoadFunction(lib, "RrGetInstanceProcAddr");
		RendererAPI::EnumerateInstanceExtensionProperties = (RrError (*)(const char* pLayerName, uint32_t* pPropertyCount, RrExtensionProperties* pProperties))LoadFunction(lib, "RrEnumerateInstanceExtensionProperties");
		RendererAPI::CreateLogicalDevice = (RrError (*)(RrPhysicalDevice physicalDevice,RrLogicalDeviceCreateInfo* pCreateInfo, RrLogicalDevice* pDevice))LoadFunction(lib, "RrCreateLogicalDevice");
		RendererAPI::GetDeviceQueue = (void (*)(RrLogicalDevice device, uint32_t queueFamilyIndex, uint32_t queueIndex, RrQueue* pQueue))LoadFunction(lib, "RrGetDeviceQueue");
		RendererAPI::DestroyLogicalDevice = (void (*)(RrLogicalDevice device))LoadFunction(lib, "RrDestroyLogicalDevice");
		RendererAPI::GetDeviceProcAddr = (RrPFNVoidFunction (*)(RrLogicalDevice device, const char* pName))LoadFunction(lib, "RrGetDeviceProcAddr");
		RendererAPI::DeviceWaitIdle = (RrError (*)(RrLogicalDevice device))LoadFunction(lib, "RrDeviceWaitIdle");
		RendererAPI::EnumeratePhysicalDevice = (RrError (*)(RrInstance instance,const RrPhysicalDeviceEnumerationInfo* pCreateInfo, RrPhysicalDevice* pPhysicalDevice))LoadFunction(lib, "RrEnumeratePhysicalDevice");
		RendererAPI::GetPhysicalDeviceProperties = (void (*)(RrPhysicalDevice physicalDevice, RrPhysicalDeviceProperties* pProperties))LoadFunction(lib, "RrGetPhysicalDeviceProperties");
		RendererAPI::GetPhysicalDeviceFeatures = (void (*)(RrPhysicalDevice physicalDevice, RrPhysicalDeviceFeatures* pFeatures))LoadFunction(lib, "RrGetPhysicalDeviceFeatures");
		RendererAPI::GetPhysicalDeviceMemoryProperties = (void (*)(RrPhysicalDevice physicalDevice, RrPhysicalDeviceMemoryProperties* pProperties))LoadFunction(lib, "RrGetPhysicalDeviceMemoryProperties");
		RendererAPI::GetPhysicalDeviceFormatProperties = (void (*)(RrPhysicalDevice physicalDevice, RrFormat format, RrFormatProperties* pProperties))LoadFunction(lib, "RrGetPhysicalDeviceFormatProperties");
		RendererAPI::GetPhysicalDeviceSurfaceCapabilitiesKHR = (void (*)(RrPhysicalDevice physicalDevice,RrSurfaceKHR surface, RrSurfaceCapabilitiesKHR* pSurfaceCapabilities))LoadFunction(lib, "RrGetPhysicalDeviceSurfaceCapabilitiesKHR");
		RendererAPI::GetPhysicalDeviceSurfaceFormatsKHR = (void (*)(RrPhysicalDevice physicalDevice,RrSurfaceKHR surface, uint32_t* pSurfaceFormatCount, RrSurfaceFormatKHR* pSurfaceFormats))LoadFunction(lib, "RrGetPhysicalDeviceSurfaceFormatsKHR");
		RendererAPI::GetPhysicalDeviceSurfacePresentModesKHR = (void (*)(RrPhysicalDevice physicalDevice,RrSurfaceKHR surface, uint32_t* pPresentModeCount, RrPresentModeKHR* pPresentModes))LoadFunction(lib, "RrGetPhysicalDeviceSurfacePresentModesKHR");
		RendererAPI::GetPhysicalDeviceQueueFamilyProperties = (void (*)(RrPhysicalDevice physicalDevice,uint32_t* pQueueFamilyPropertyCount, RrQueueFamilyProperties* pQueueFamilyProperties))LoadFunction(lib, "RrGetPhysicalDeviceQueueFamilyProperties");
		RendererAPI::GetPhysicalDeviceSurfaceSupportKHR = (RrError (*)(RrPhysicalDevice physicalDevice,uint32_t queueFamilyIndex, RrSurfaceKHR surface, RrBool32* pSupported))LoadFunction(lib, "RrGetPhysicalDeviceSurfaceSupportKHR");
		RendererAPI::CreateRenderPass = (RrError (*)(RrLogicalDevice device, const RrRenderPassCreateInfo* pCreateInfo, RrRenderPass* pRenderPass))LoadFunction(lib, "RrCreateRenderPass");
		RendererAPI::DestroyRenderPass = (void (*)(RrLogicalDevice device, RrRenderPass renderPass))LoadFunction(lib, "RrDestroyRenderPass");
		RendererAPI::CmdBeginRenderPass = (void (*)(RrCommandBuffer commandBuffer,const RrRenderPassBeginInfo* pBeginInfo, RrSubpassContents subpassContents))LoadFunction(lib, "RrCmdBeginRenderPass");
		RendererAPI::CmdEndRenderPass = (void (*)(RrCommandBuffer commandBuffer))LoadFunction(lib, "RrCmdEndRenderPass");
		RendererAPI::CreateSampler = (RrError (*)(RrLogicalDevice device, const RrSamplerCreateInfo* pCreateInfo, RrSampler* pSampler))LoadFunction(lib, "RrCreateSampler");
		RendererAPI::DestroySampler = (void (*)(RrLogicalDevice device, RrSampler sampler))LoadFunction(lib, "RrDestroySampler");
		RendererAPI::CreateSurfaceKHR = (RrError (*)(RrInstance instance, const RrSurfaceCreateInfoKHR* pCreateInfo, RrSurfaceKHR* pSurface))LoadFunction(lib, "RrCreateSurfaceKHR");
		RendererAPI::DestroySurfaceKHR = (void (*)(RrInstance instance, RrSurfaceKHR surface))LoadFunction(lib, "RrDestroySurfaceKHR");
		RendererAPI::CreateSwapchainKHR = (RrError (*)(RrLogicalDevice device,const RrSwapchainCreateInfoKHR* pCreateInfo, RrSwapchainKHR* pSwapchain))LoadFunction(lib, "RrCreateSwapchainKHR");
		RendererAPI::GetSwapchainImagesKHR = (RrError (*)(RrLogicalDevice device, RrSwapchainKHR swapchain,uint32_t* pSwapchainImageCount, RrImage* pSwapchainImages))LoadFunction(lib, "RrGetSwapchainImagesKHR");
		RendererAPI::DestroySwapchainKHR = (void (*)(RrLogicalDevice device, RrSwapchainKHR swapchain))LoadFunction(lib, "RrDestroySwapchainKHR");
		RendererAPI::AcquireNextImageKHR = (RrError (*)(RrLogicalDevice device, RrSwapchainKHR swapchain,uint64_t timeout, RrSemaphore semaphore, RrFence fence, uint32_t* pImageIndex))LoadFunction(lib, "RrAcquireNextImageKHR");
		RendererAPI::QueuePresentKHR = (RrError (*)(RrQueue queue, const RrPresentInfoKHR* pPresentInfo))LoadFunction(lib, "RrQueuePresentKHR");
		RendererAPI::CreateFence = (RrError (*)(RrLogicalDevice device, const RrFenceCreateInfo* pCreateInfo, RrFence* pFence))LoadFunction(lib, "RrCreateFence");
		RendererAPI::WaitForFences = (RrError (*)(RrLogicalDevice device, uint32_t fenceCount, const RrFence* pFences,RrBool32 waitAll, uint64_t timeout))LoadFunction(lib, "RrWaitForFences");
		RendererAPI::ResetFences = (RrError (*)(RrLogicalDevice device, uint32_t fenceCount, const RrFence* pFences))LoadFunction(lib, "RrResetFences");
		RendererAPI::DestroyFence = (void (*)(RrLogicalDevice device, RrFence fence))LoadFunction(lib, "RrDestroyFence");
		RendererAPI::CreateSemaphore = (RrError (*)(RrLogicalDevice device, RrSemaphore* pSemaphore))LoadFunction(lib, "RrCreateSemaphore");
		RendererAPI::DestroySemaphore = (void (*)(RrLogicalDevice device, RrSemaphore semaphore))LoadFunction(lib, "RrDestroySemaphore");
		RendererAPI::CreateDescriptorSetLayout = (RrError (*)(RrLogicalDevice device,const RrDescriptorSetLayoutCreateInfo* pCreateInfo, RrDescriptorSetLayout* pLayout))LoadFunction(lib, "RrCreateDescriptorSetLayout");
		RendererAPI::CreateDescriptorPool = (RrError (*)(RrLogicalDevice device, const RrDescriptorPoolCreateInfo* pCreateInfo, RrDescriptorPool* pPool))LoadFunction(lib, "RrCreateDescriptorPool");
		RendererAPI::AllocateDescriptorSets = (RrError (*)(RrLogicalDevice device,const RrDescriptorSetAllocateInfo* pAllocInfo, RrDescriptorSet* pDescriptorSets))LoadFunction(lib, "RrAllocateDescriptorSets");
		RendererAPI::UpdateDescriptorSets = (void (*)(RrLogicalDevice device, uint32_t descriptorWriteCount,const RrWriteDescriptorSet* pDescriptorWrites, uint32_t descriptorCopyCount,const RrCopyDescriptorSet* pDescriptorCopies))LoadFunction(lib, "RrUpdateDescriptorSets");
		RendererAPI::DestroyDescriptorPool = (void (*)(RrLogicalDevice device, RrDescriptorPool pool))LoadFunction(lib, "RrDestroyDescriptorPool");
		RendererAPI::DestroyDescriptorSetLayout = (void (*)(RrLogicalDevice device, RrDescriptorSetLayout layout))LoadFunction(lib, "RrDestroyDescriptorSetLayout");
		RendererAPI::CmdBindDescriptorSets = (void (*)(RrCommandBuffer commandBuffer,RrPipelineBindPoint pipelineBindPoint, RrPipelineLayout pipelineLayout, uint32_t firstSet,uint32_t descriptorSetCount, const RrDescriptorSet* pDescriptorSets,uint32_t dynamicOffsetCount, const uint32_t* pDynamicOffsets))LoadFunction(lib, "RrCmdBindDescriptorSets");
		RendererAPI::CreatePipelineLayout = (RrError (*)(RrLogicalDevice device,const RrPipelineLayoutCreateInfo* pCreateInfo, RrPipelineLayout* pLayout))LoadFunction(lib, "RrCreatePipelineLayout");
		RendererAPI::CreateGraphicsPipeline = (RrError (*)(RrLogicalDevice device, RrPipelineCache pipelineCache,const RrGraphicsPipelineCreateInfo* pCreateInfo, RrPipeline* pPipeline))LoadFunction(lib, "RrCreateGraphicsPipeline");
		RendererAPI::DestroyPipelineLayout = (void (*)(RrLogicalDevice device, RrPipelineLayout pipelineLayout))LoadFunction(lib, "RrDestroyPipelineLayout");
		RendererAPI::DestroyPipeline = (void (*)(RrLogicalDevice device, RrPipeline pipeline))LoadFunction(lib, "RrDestroyPipeline");
		RendererAPI::CmdBindPipeline = (void (*)(RrCommandBuffer commandBuffer, RrPipelineBindPoint pipelineBindPoint, RrPipeline pipeline))LoadFunction(lib, "RrCmdBindPipeline");
		RendererAPI::CmdSetViewport = (void (*)(RrCommandBuffer commandBuffer, uint32_t firstViewport,uint32_t viewportCount, const RrViewport* pViewports))LoadFunction(lib, "RrCmdSetViewport");
		RendererAPI::CmdSetScissor = (void (*)(RrCommandBuffer commandBuffer, uint32_t firstScissor,uint32_t scissorCount, const RrRect2D* pScissors))LoadFunction(lib, "RrCmdSetScissor");
		RendererAPI::CmdDrawIndexed = (void (*)(RrCommandBuffer commandBuffer, uint32_t indexCount,uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance))LoadFunction(lib, "RrCmdDrawIndexed");
		RendererAPI::CmdPipelineBarrier = (void (*)(RrCommandBuffer commandBuffer, RrPipelineStageFlags srcStageMask,RrPipelineStageFlags dstStageMask, RrDependencyFlags dependencyFlags,uint32_t memoryBarrierCount, const RrMemoryBarrier* pMemoryBarriers,uint32_t bufferMemoryBarrierCount, const RrBufferMemoryBarrier* pBufferMemoryBarriers,uint32_t imageMemoryBarrierCount, const RrImageMemoryBarrier* pImageMemoryBarriers))LoadFunction(lib, "RrCmdPipelineBarrier");
		RendererAPI::CreateShaderModule = (RrError (*)(RrLogicalDevice device,const RrShaderModuleCreateInfo* pCreateInfo, RrShaderModule* pShaderModule))LoadFunction(lib, "RrCreateShaderModule");
		RendererAPI::DestroyShaderModule = (void (*)(RrLogicalDevice device, RrShaderModule shaderModule))LoadFunction(lib, "RrDestroyShaderModule");
		RendererAPI::CmdPushConstants = (void (*)(RrCommandBuffer commandBuffer, RrPipelineLayout pipelineLayout,RrShaderStageFlags shaderStage, uint32_t offset, uint32_t size, const void* pValues))LoadFunction(lib, "RrCmdPushConstants");


		RendererAPI::API = type;
	}
}  // namespace At0::Ray

