#pragma once

#include "../RlRBase.h"
#include "Drawable.h"
#include "Mesh.h"


struct aiScene;
struct aiNode;
struct aiMesh;

namespace At0::Ray
{
	class RR_API Model : public Drawable
	{
	public:
		Model() = default;
		Model(std::string_view filepath, const Renderer3D& renderer);
		virtual const std::vector<Scope<Bindable>>& GetStaticBinds() const override { return s_StaticBinds; }
		virtual void Update() override;
		virtual void Draw(Renderer3D* renderer) override;

	private:
		void AddStaticBind(Scope<Bindable> bind);
		void AddStaticIndexBuffer(Scope<IndexBuffer> bind);
		void SetIndexFromStatic();

		void ProcessNode(aiNode* pNode, const aiScene* pScene);
		Mesh ProcessMesh(aiMesh* pMesh, const aiScene* pScene);

	private:
		std::vector<Mesh> m_Meshes;
		static std::vector<Scope<Bindable>> s_StaticBinds;
	};
}