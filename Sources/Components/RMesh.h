#pragma once

#include "RComponent.h"
#include "RMaterial.h"

#include "../Core/RTime.h"
#include "../Graphics/Pipelines/RUniformAccess.h"


namespace At0::Ray
{
	class VertexBuffer;
	class IndexBuffer;
	class VertexInput;


	class RAY_EXPORT Mesh : public Component
	{
	public:
		static Mesh Triangle(Material material);

		/**
		 * Updates uniform buffers
		 */
		void Update(Delta ts);

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

		uint32_t m_GlobalUniformBufferOffset = 0;
	};
}  // namespace At0::Ray
