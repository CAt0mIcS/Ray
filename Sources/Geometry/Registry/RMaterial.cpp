#include "Rpch.h"
#include "RMaterial.h"

#include "Graphics/RCodex.h"
#include "Graphics/RGraphics.h"
#include "Graphics/Pipelines/RGraphicsPipeline.h"
#include "Graphics/RenderPass/RRenderPass.h"
#include "Graphics/Images/RTexture2D.h"


namespace At0::Ray
{
	Material::Material(const Material::Config& config)
		: m_BaseDiffuse(config.baseDiffuse), m_DiffuseMap(config.diffuseMap),
		  m_Metallic(config.metallic), m_Roughness(config.roughness),
		  m_SpecularMap(config.specularMap), m_NormalMap(config.normalMap)
	{
		m_GraphicsPipeline = Codex::Resolve<GraphicsPipeline>(Graphics::Get().GetRenderPass(),
			config.shaders, config.vertexLayout, Graphics::Get().GetPipelineCache(),
			config.cullMode, config.topology, config.polygonMode, config.lineWidth);
	}

	Material::Material(const std::vector<std::string>& shaders)
	{
		m_GraphicsPipeline = Codex::Resolve<GraphicsPipeline>(
			Graphics::Get().GetRenderPass(), shaders, nullptr, Graphics::Get().GetPipelineCache());
	}

	Material::~Material() {}

	const VertexLayout& Material::GetVertexLayout() const
	{
		return m_GraphicsPipeline->GetVertexLayout();
	}
}  // namespace At0::Ray
