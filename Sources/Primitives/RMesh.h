#pragma once

#include "../RBase.h"

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

		void CmdBind(const CommandBuffer& cmdBuff);
		void CmdDraw(const CommandBuffer& cmdBuff);

		Scope<GraphicsPipeline>& GetPipeline() { return graphicsPipeline; }

		Scope<UniformAccess> uniformAccess;

	private:
		Scope<VertexBuffer> vertexBuffer;
		Scope<IndexBuffer> indexBuffer;
		Scope<GraphicsPipeline> graphicsPipeline;
	};
}  // namespace At0::Ray
