#include "Rpch.h"
#include "RMaterial.h"

#include "Graphics/RCodex.h"
#include "Graphics/RGraphics.h"
#include "Graphics/Pipelines/RGraphicsPipeline.h"
#include "Graphics/RenderPass/RRenderPass.h"
#include "Graphics/Images/RTexture2D.h"

#include "Utils/RException.h"


namespace At0::Ray
{
	Material::~Material() {}

	const VertexLayout& Material::GetVertexLayout() const
	{
		return m_GraphicsPipeline->GetVertexLayout();
	}

	void Material::CmdBind(const CommandBuffer& cmdBuff) const
	{
		m_GraphicsPipeline->CmdBind(cmdBuff);

		for (const auto& uniform : m_Uniforms)
			uniform->CmdBind(cmdBuff);
	}

	Uniform& Material::AddUniform(std::string_view tag, Scope<Uniform> uniform)
	{
		return *m_Uniforms.emplace_back(std::move(uniform));
	}

	bool Material::HasUniform(std::string_view tag) const
	{
		for (auto& uniform : m_Uniforms)
			if (tag == uniform->GetName())
				return true;
		return false;
	}

	Uniform& Material::GetUniform(std::string_view tag)
	{
		for (auto& uniform : m_Uniforms)
			if (tag == uniform->GetName())
				return *uniform;

		RAY_THROW_RUNTIME("[Mesh] Failed to get uniform with tag {0}", tag);
	}

	Material& Material::operator=(Material&& other) noexcept
	{
		m_GraphicsPipeline = std::move(other.m_GraphicsPipeline);
		m_Uniforms = std::move(other.m_Uniforms);
		return *this;
	}

	Material::Material(Material&& other) noexcept { *this = std::move(other); }

	Material::Material(const std::vector<std::string>& shaders, VertexLayout* pVertexLayout,
		VkCullModeFlags cullMode, VkPrimitiveTopology topology, VkPolygonMode polygonMode,
		float lineWidth)
	{
		CreatePipeline(shaders, pVertexLayout, cullMode, topology, polygonMode, lineWidth);
	}

	void Material::CreatePipeline(const std::vector<std::string>& shaders,
		VertexLayout* pVertexLayout, VkCullModeFlags cullMode, VkPrimitiveTopology topology,
		VkPolygonMode polygonMode, float lineWidth)
	{
		m_GraphicsPipeline = Codex::Resolve<GraphicsPipeline>(Graphics::Get().GetRenderPass(),
			shaders, pVertexLayout, Graphics::Get().GetPipelineCache(), cullMode, topology,
			polygonMode, lineWidth);
	}

	void Material::FillConfig(Config& config, PolygonMode polygonMode)
	{
		config.polygonMode = polygonMode;
	}

	void Material::FillConfig(Config& config, LineWidth lineWidth) { config.lineWidth = lineWidth; }
}  // namespace At0::Ray
