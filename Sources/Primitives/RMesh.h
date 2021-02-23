#pragma once

#include "../RBase.h"
#include "../Core/RMath.h"

namespace At0::Ray
{
	class VertexBuffer;
	class IndexBuffer;
	class GraphicsPipeline;
	class CommandBuffer;

	class UniformAccess;

	class RAY_EXPORT Mesh
	{
	public:
		Mesh();
		~Mesh();

		void Update();

		void CmdBind(const CommandBuffer& cmdBuff);
		void CmdDraw(const CommandBuffer& cmdBuff);

		Scope<GraphicsPipeline>& GetPipeline() { return graphicsPipeline; }

		Scope<UniformAccess> uniformAccess;

		void Translate(Float3 translation);

	private:
		Scope<VertexBuffer> vertexBuffer;
		Scope<IndexBuffer> indexBuffer;
		Scope<GraphicsPipeline> graphicsPipeline;

		VkDescriptorSet descSet;

		Float3 translation{};
	};
}  // namespace At0::Ray
