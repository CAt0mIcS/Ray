﻿#pragma once

#include "../RBase.h"


namespace At0::Ray
{
	class VertexBuffer;
	class IndexBuffer;
	class CommandBuffer;

	class RAY_EXPORT Mesh
	{
	public:
		Mesh();
		~Mesh();

		void CmdBind(const CommandBuffer& cmdBuff);
		void CmdDraw(const CommandBuffer& cmdBuff);

	private:
		Scope<VertexBuffer> vertexBuffer;
		Scope<IndexBuffer> indexBuffer;
	};
}  // namespace At0::Ray