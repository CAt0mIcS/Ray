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

	public:
		struct VertexData
		{
			Ref<VertexBuffer> vertexBuffer;
			Ref<IndexBuffer> indexBuffer;
			std::vector<VertexData> children;
		};

	public:
		Mesh(Entity entity, VertexData vertexData);

		static VertexData Triangle(Ref<Material> material);
		static VertexData Plane(Ref<Material> material);

		/**
		 * Import a model from a file
		 * @param material Optional material. If not set, the model importer will create it's own
		 * and use any maps(diffuse/specular/...) that are contained in the model file. If this
		 * material is set, the importer will ignore said maps.
		 */
		static VertexData Import(std::string_view filepath, Ref<Material> material = nullptr);

		/**
		 * Called by the mesh renderer to bind mesh-specific resources
		 */
		void CmdBind(const CommandBuffer& cmdBuff) const override;

	private:
		void EmplaceChildren(const std::vector<VertexData>& vertexData);

	private:
		// Created by the mesh renderer
		Ref<VertexBuffer> m_VertexBuffer;
		Ref<IndexBuffer> m_IndexBuffer;
	};
}  // namespace At0::Ray


RAY_EXPORT_COMPONENT(Mesh);
