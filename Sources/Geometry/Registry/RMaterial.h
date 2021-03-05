#pragma once

#include "../../RBase.h"
#include "../../Scene/REntity.h"
#include "../../Graphics/Images/RImage2D.h"

#include <type_traits>


namespace At0::Ray
{
	class GraphicsPipeline;
	class VertexLayout;
	class Texture2D;

	class RAY_EXPORT Material
	{
	public:
		Material(const Float4& baseDiffuse = { 1.0f, 1.0f, 1.0f, 1.0f },
			Ref<Texture2D> imageDiffuse = nullptr, float metallic = 0.0f, float roughness = 0.0f,
			Ref<Texture2D> imageMaterial = nullptr, Ref<Texture2D> imageNormal = nullptr,
			bool useTexturedShader = false);
		~Material();

		const Float4& GetBaseDiffuse() const { return m_BaseDiffuse; }
		const Texture2D* GetDiffuseImage() const { return m_DiffuseImage.get(); }
		float GetMetallic() const { return m_Metallic; }
		float GetRoughness() const { return m_Roughness; }
		const Texture2D* GetMaterialImage() const { return m_MaterialImage.get(); }
		const Texture2D* GetNormalImage() const { return m_NormalImage.get(); }

		const GraphicsPipeline& GetGraphicsPipeline() const { return *m_GraphicsPipeline; }
		const VertexLayout& GetVertexLayout() const;


	private:
		Ref<GraphicsPipeline> m_GraphicsPipeline = nullptr;

		Float4 m_BaseDiffuse{ 1.0f, 1.0f, 1.0f, 1.0f };
		Ref<Texture2D> m_DiffuseImage = nullptr;
		float m_Metallic = 0.0f;
		float m_Roughness = 0.0f;
		Ref<Texture2D> m_MaterialImage = nullptr;
		Ref<Texture2D> m_NormalImage = nullptr;
	};


}  // namespace At0::Ray
