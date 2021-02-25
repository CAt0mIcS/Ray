#pragma once

#include "../RBase.h"
#include "../Core/RMath.h"
#include "../Core/REntity.h"
#include "RComponents.h"

namespace At0::Ray
{
	class VertexBuffer;
	class IndexBuffer;
	class GraphicsPipeline;
	class CommandBuffer;
	class DescriptorSet;
	class UniformBuffer;

	class UniformAccess;

	class RAY_EXPORT Mesh
	{
	public:
		Mesh();
		~Mesh();

		void Update();

		void CmdBind(const CommandBuffer& cmdBuff);
		void CmdDraw(const CommandBuffer& cmdBuff);

		GraphicsPipeline& GetPipeline() { return *graphicsPipeline; }

		Scope<UniformAccess> uniformAccess;

		Entity GetEntity() { return m_Entity; }

	private:
		Ref<VertexBuffer> vertexBuffer = nullptr;
		Ref<IndexBuffer> indexBuffer = nullptr;
		Ref<GraphicsPipeline> graphicsPipeline = nullptr;

		Scope<DescriptorSet> descSet;
		Entity m_Entity;

		uint32_t m_GlobalUniformBufferOffset = 0;
		inline static uint32_t nextOffset = 0;
	};
}  // namespace At0::Ray
