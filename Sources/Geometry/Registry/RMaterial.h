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
		struct Config
		{
			std::vector<std::string> shaders;
			Float4 baseDiffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
			Ref<Texture2D> diffuseMap = nullptr;
			Ref<Texture2D> specularMap = nullptr;
			Ref<Texture2D> normalMap = nullptr;
			float metallic = 0.0f;
			float roughness = 0.0f;
			VertexLayout* vertexLayout = nullptr;
			VkCullModeFlags cullMode = VK_CULL_MODE_BACK_BIT;
			VkPrimitiveTopology topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		};

	public:
		Material(const Material::Config& config);
		Material(const std::vector<std::string>& shaders);
		~Material();

		const Float4& GetBaseDiffuse() const { return m_BaseDiffuse; }
		const Texture2D* GetDiffuseMap() const { return m_DiffuseMap.get(); }
		float GetMetallic() const { return m_Metallic; }
		float GetRoughness() const { return m_Roughness; }
		const Texture2D* GetSpecularMap() const { return m_SpecularMap.get(); }
		const Texture2D* GetNormalMap() const { return m_NormalMap.get(); }

		const GraphicsPipeline& GetGraphicsPipeline() const { return *m_GraphicsPipeline; }
		const VertexLayout& GetVertexLayout() const;


	private:
		Ref<GraphicsPipeline> m_GraphicsPipeline = nullptr;

		Float4 m_BaseDiffuse{ 1.0f, 1.0f, 1.0f, 1.0f };
		Ref<Texture2D> m_DiffuseMap = nullptr;
		float m_Metallic = 0.0f;
		float m_Roughness = 0.0f;
		Ref<Texture2D> m_SpecularMap = nullptr;
		Ref<Texture2D> m_NormalMap = nullptr;
	};


}  // namespace At0::Ray
