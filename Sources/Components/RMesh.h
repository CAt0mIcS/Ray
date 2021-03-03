#pragma once

#include "RComponent.h"
#include "Registry/RMaterial.h"

#include "../Core/RMath.h"
#include "../Core/RTime.h"
#include "../Graphics/Pipelines/RUniformAccess.h"
#include "../Graphics/Pipelines/RDescriptor.h"

#include <type_traits>


namespace At0::Ray
{
	class VertexBuffer;
	class IndexBuffer;
	class VertexInput;
	class CommandBuffer;


	class RAY_EXPORT Mesh : public Component
	{
	public:
		Mesh(Ref<VertexBuffer> vertexBuffer, Ref<IndexBuffer> indexBuffer, Material material);

		static Mesh Triangle(Material material);
		static Mesh Plane(Material material);
		static Mesh Circle(Material material, int segments = 360, float radius = 1.0f);

		static Mesh Cube(Material material);
		static Mesh IcoSphere(Material material);
		static Mesh UVSphere(Material material);

		template<typename T>
		T& Get()
		{
			return Get(std::type_identity<T>());
		}

		/**
		 * Updates uniform buffers
		 */
		void Update(Delta ts, const Transform& parentTransform = {});

		void Bind(const CommandBuffer& cmdBuff) const;
		void Render(const CommandBuffer& cmdBuff) const;

		const Material& GetMaterial() const { return m_Material; }

		~Mesh();

		Mesh& operator=(Mesh&& other) noexcept;
		Mesh(Mesh&& other) noexcept;

	private:
		Transform& Get(std::type_identity<Transform>) { return m_Transform; }

	private:
		Ref<VertexBuffer> m_VertexBuffer = nullptr;
		Ref<IndexBuffer> m_IndexBuffer = nullptr;

		Material m_Material;
		UniformAccess m_Uniforms;
		DescriptorSet m_DescriptorSet;

		uint32_t m_GlobalUniformBufferOffset = 0;

		Transform m_Transform;
	};
}  // namespace At0::Ray
