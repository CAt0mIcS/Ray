#pragma once

#include "../RBase.h"
#include "../Core/RMath.h"
#include "../Core/RTime.h"
#include "../Graphics/Pipelines/RShader.h"

#include "../Events/REventListener.h"
#include "../Events/RKeyboardEvents.h"
#include "../Events/RMouseEvents.h"
#include "../Events/REngineEvents.h"


namespace At0::Ray
{

	/**
	 * @brief Encapsulates access to a Vulkan buffer backed up by device memory
	 * @note To be filled by an external source like the VulkanDevice
	 */
	struct RAY_EXPORT GuiBuffer
	{
		VkBuffer buffer = VK_NULL_HANDLE;
		VkDeviceMemory memory = VK_NULL_HANDLE;
		VkDescriptorBufferInfo descriptor;
		VkDeviceSize size = 0;
		VkDeviceSize alignment = 0;
		void* mapped = nullptr;
		/** @brief Usage flags to be filled by external source at buffer creation (to query at some
		 * later point) */
		VkBufferUsageFlags usageFlags;
		/** @brief Memory property flags to be filled by external source at buffer creation (to
		 * query at some later point) */
		VkMemoryPropertyFlags memoryPropertyFlags;
		VkResult map(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);
		void unmap();
		VkResult bind(VkDeviceSize offset = 0);
		void setupDescriptor(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);
		void copyTo(void* data, VkDeviceSize size);
		VkResult flush(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);
		VkResult invalidate(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);
		void destroy();
	};

	class Image2D;
	class TextureSampler;
	class Buffer;

	class RAY_EXPORT ImGUI :
		EventListener<FramebufferResizedEvent>,
		EventListener<MouseMovedEvent>,
		EventListener<MouseButtonPressedEvent>,
		EventListener<MouseButtonReleasedEvent>,
		EventListener<KeyPressedEvent>,
		EventListener<KeyReleasedEvent>,
		EventListener<CharEvent>,
		EventListener<ScrollLeftEvent>,
		EventListener<ScrollRightEvent>,
		EventListener<ScrollUpEvent>,
		EventListener<ScrollDownEvent>
	{
	public:
		struct PushConstBlock
		{
			Float2 scale;
			Float2 translate;
		} m_PushConstBlock;

	public:
		static ImGUI& Get();
		static void Destroy();

		void NewFrame();
		void UpdateBuffers();
		void DrawFrame(VkCommandBuffer commandBuffer);
		void Update(Delta dt);

	private:
		ImGUI();

		void InitResources();
		void CreatePipeline();
		void MapKeySpace();

		void OnEvent(FramebufferResizedEvent& e) override;
		void OnEvent(MouseMovedEvent& e) override;
		void OnEvent(MouseButtonPressedEvent& e) override;
		void OnEvent(MouseButtonReleasedEvent& e) override;
		void OnEvent(KeyPressedEvent& e) override;
		void OnEvent(KeyReleasedEvent& e) override;
		void OnEvent(CharEvent& e) override;
		void OnEvent(ScrollLeftEvent& e) override;
		void OnEvent(ScrollRightEvent& e) override;
		void OnEvent(ScrollUpEvent& e) override;
		void OnEvent(ScrollDownEvent& e) override;

	private:
		Scope<Image2D> m_FontImage;
		Scope<TextureSampler> m_Sampler;

		VkDescriptorPool m_DescriptorPool;
		VkDescriptorSetLayout m_DescriptorSetLayout;
		VkDescriptorSet m_DescriptorSet;
		VkPipelineLayout m_PipelineLayout;
		VkPipeline m_Pipeline;

		Scope<Buffer> m_VertexBuffer;
		Scope<Buffer> m_IndexBuffer;
		void* m_VertexBufferMapped = nullptr;
		void* m_IndexBufferMapped = nullptr;

		int32_t m_VertexCount = 0;
		int32_t m_IndexCount = 0;

		Shader m_Shader;
		std::vector<VkShaderModule> m_ShaderModules;
	};
}  // namespace At0::Ray
