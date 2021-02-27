#pragma once

#include "RComponent.h"
#include "RMaterial.h"

#include "../Core/RTime.h"
#include "../Graphics/Pipelines/RUniformAccess.h"
#include "../Graphics/Pipelines/RDescriptor.h"


namespace At0::Ray
{
	class VertexBuffer;
	class IndexBuffer;
	class VertexInput;
	class CommandBuffer;


	class RAY_EXPORT Mesh : public Component
	{
	public:
		static Mesh Triangle(Material material);
		static Mesh Plane(Material material);
		static Mesh Circle(Material material, int segments = 360, float radius = 1.0f);

		static Mesh Cube(Material material);
		static Mesh IcoSphere(Material material);
		static Mesh UVSphere(Material material);


		/**
		 * Updates uniform buffers
		 */
		void Update(Delta ts);

		void Bind(const CommandBuffer& cmdBuff) const;
		void Render(const CommandBuffer& cmdBuff) const;

		const Material& GetMaterial() const { return m_Material; }

		~Mesh();

		Mesh& operator=(Mesh&& other) noexcept;
		Mesh(Mesh&& other) noexcept;

	private:
		Mesh(Ref<VertexBuffer> vertexBuffer, Ref<IndexBuffer> indexBuffer, Material material);

	private:
		Ref<VertexBuffer> m_VertexBuffer = nullptr;
		Ref<IndexBuffer> m_IndexBuffer = nullptr;

		Material m_Material;
		UniformAccess m_Uniforms;
		DescriptorSet m_DescriptorSet;

		uint32_t m_GlobalUniformBufferOffset = 0;
	};
}  // namespace At0::Ray
