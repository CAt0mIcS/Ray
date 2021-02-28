#pragma once

#include "RComponent.h"
#include "RTransform.h"
#include "../Core/RTime.h"

struct aiMesh;
struct aiMaterial;
struct aiNode;


namespace At0::Ray
{
	class Mesh;
	class CommandBuffer;

	class RAY_EXPORT Model : public Component
	{
	public:
		Model(std::string_view filepath);
		~Model();

		void Update(Delta ts);
		void Render(const CommandBuffer& cmdBuff) const;

		template<typename T>
		T& Get()
		{
			static_assert(false, "Template not specialized.");
		}

		template<>
		Transform& Get()
		{
			return m_Transform;
		}

		Model& operator=(Model&& other) noexcept;
		Model(Model&& other) noexcept;

	private:
		static Mesh ParseMesh(
			std::string_view base, const aiMesh& mesh, const aiMaterial* const* pMaterials);

	private:
		std::vector<Mesh> m_Meshes;
		Transform m_Transform{};
	};
}  // namespace At0::Ray
