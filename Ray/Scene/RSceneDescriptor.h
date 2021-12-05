#pragma once

#include "../RBase.h"
#include "../Core/RMath.h"
#include <vulkan/vulkan_core.h>


namespace At0::Ray
{
	class DescriptorSet;
	class BufferUniform;
	class CommandBuffer;
	class Camera;

	struct PerSceneData
	{
		Matrix View = MatrixIdentity();
		Matrix Projection = MatrixIdentity();
		Float3 ViewPos{};
		Float3 LightPos{};
	};

	class RAY_EXPORT SceneDescriptor
	{
	public:
		SceneDescriptor();
		~SceneDescriptor();

		void UpdateUniform(const Camera& camera);
		void CmdBind(const CommandBuffer& cmdBuff) const;

	private:
		VkDescriptorSetLayout m_DescriptorSetLayout;
		VkPipelineLayout m_PipelineLayout;
		VkDescriptorPool m_DescriptorPool;
		Scope<DescriptorSet> m_PerSceneDescriptor;
		Scope<BufferUniform> m_PerSceneUniform;
	};
}  // namespace At0::Ray
