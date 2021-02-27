#pragma once

#include "RComponent.h"
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

		Model& operator=(Model&& other);
		Model(Model&& other);

	private:
		static Mesh ParseMesh(
			std::string_view base, const aiMesh& mesh, const aiMaterial* const* pMaterials);

	private:
		std::vector<Mesh> m_Meshes;
	};
}  // namespace At0::Ray
