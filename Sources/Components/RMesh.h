#pragma once

#include "../RBase.h"
#include "RComponent.h"
#include "../Graphics/Core/RBindable.h"


namespace At0::Ray
{
	class VertexBuffer;
	class IndexBuffer;
	class Material;


	class RAY_EXPORT Mesh : public Component, public Bindable
	{
		friend class MeshRenderer;

	private:
		struct VertexData
		{
			Ref<VertexBuffer> vertexBuffer;
			Ref<IndexBuffer> indexBuffer;
		};

	public:
		Mesh(Entity entity, VertexData vertexData);

		static VertexData Triangle(Ref<Material> material);
		static VertexData Plane(Ref<Material> material);

		/**
		 * Called by the mesh renderer to bind mesh-specific resources
		 */
		void CmdBind(const CommandBuffer& cmdBuff) const override;

	private:
		// Created by the mesh renderer
		Ref<VertexBuffer> m_VertexBuffer;
		Ref<IndexBuffer> m_IndexBuffer;
	};
}  // namespace At0::Ray


RAY_EXPORT_COMPONENT(Mesh);
