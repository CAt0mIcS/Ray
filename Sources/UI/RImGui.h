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
	class Texture2D;
	class Buffer;
	class SamplerUniform;
	class CommandBuffer;

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
		void DrawFrame(const CommandBuffer& cmdBuff);
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
		Ref<Texture2D> m_FontImage;

		VkDescriptorPool m_DescriptorPool;
		VkDescriptorSetLayout m_DescriptorSetLayout;
		Scope<SamplerUniform> m_FontUniform;
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
