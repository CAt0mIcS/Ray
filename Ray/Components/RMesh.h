#pragma once

#include "RComponent.h"


namespace At0::Ray
{
	class VertexBuffer;
	class IndexBuffer;
	class Material;
	class CommandBuffer;


	class RAY_EXPORT Mesh : public Component
	{
	public:
		struct Data
		{
			Ref<VertexBuffer> vertexBuffer;
			Ref<IndexBuffer> indexBuffer;
			Ref<Material> material;
			std::vector<Entity> children;
			RAY_DEBUG_FLAG(std::string name);
		};

	public:
		Mesh(Entity entity, Data vertexData);

		static Data Triangle(Ref<Material> material);
		static Data Plane(Ref<Material> material);
		static Data HalfCircle(Ref<Material> material, int segments, float radius);
		static Data Circle(Ref<Material> material, int segments, float radius);
		static Data Cube(Ref<Material> material);
		static Data UVSphere(Ref<Material> material, float radius, int latDiv, int longDiv);
		static Data Cylinder(Ref<Material> material, int segments, float radius);
		static Data Cone(Ref<Material> material, int segments, float radius);

		/**
		 * Import a model from a file
		 * @param material Optional material. If not set, the model importer will create it's own
		 * and use any maps(diffuse/specular/...) that are contained in the model file. If this
		 * material is set, the importer will ignore said maps.
		 */
		static Mesh::Data Import(std::string_view filepath, Ref<Material> material = nullptr);

		/**
		 * Called to bind mesh-specific resources
		 */
		void CmdBind(const CommandBuffer& cmdBuff) const;

		Mesh& operator=(Mesh&& other) noexcept;
		Mesh(Mesh&& other) noexcept;

		RAY_DEBUG_FLAG(std::string_view GetName() const { return m_Name; })

	private:
		// Created by the mesh renderer
		Ref<VertexBuffer> m_VertexBuffer;
		Ref<IndexBuffer> m_IndexBuffer;

		RAY_DEBUG_FLAG(std::string m_Name);
	};
}  // namespace At0::Ray


RAY_EXPORT_COMPONENT(Mesh);
