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

	Ref<Uniform> Material::AddUniform(std::string_view tag, Ref<Uniform> uniform)
	{
		return m_Uniforms.emplace_back(std::move(uniform));
	}

	bool Material::HasUniform(std::string_view tag) const
	{
		for (auto& uniform : m_Uniforms)
			if (tag == uniform->GetName())
				return true;
		return false;
	}

	Ref<Uniform> Material::GetUniform(std::string_view tag)
	{
		for (auto& uniform : m_Uniforms)
			if (tag == uniform->GetName())
				return uniform;

		RAY_THROW_RUNTIME("[Mesh] Failed to get uniform with tag {0}", tag);
	}

	Material& Material::operator=(Material&& other) noexcept
	{
		m_GraphicsPipeline = std::move(other.m_GraphicsPipeline);
		m_Uniforms = std::move(other.m_Uniforms);
		return *this;
	}

	Material::Material(Material&& other) noexcept { *this = std::move(other); }

	void Material::CreatePipeline(Material::Config& config)
	{
		bool customShaders = !config.shaders.value.empty();

		// Set default shaders none were specified
		if (!customShaders)
		{
			config.shaders = GetShaders(config);
		}

		m_GraphicsPipeline = Codex::Resolve<GraphicsPipeline>(Graphics::Get().GetRenderPass(),
			config.shaders, nullptr, Graphics::Get().GetPipelineCache(), config.cullMode,
			VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, config.polygonMode, config.lineWidth);


		// Add default uniforms if no custom shader was given.
		// If the client uses custom shaders, they will need to add the uniforms themselves
		if (!customShaders)
		{
			if (config.diffuseMap.value)
				AddUniform("DiffuseMap",
					MakeRef<SamplerUniform>("materialDiffuse", Shader::Stage::Fragment,
						*config.diffuseMap.value, *m_GraphicsPipeline));
			if (config.specularMap.value)
				AddUniform("SpecularMap",
					MakeRef<SamplerUniform>("materialSpecular", Shader::Stage::Fragment,
						*config.specularMap.value, *m_GraphicsPipeline));

			// If no custom shader and no map was given, the shader expects a color uniform
			if (!config.diffuseMap.value && !config.specularMap.value && !config.normalMap.value &&
				!config.color)
			{
				config.color = Float3{ 1.0f, 1.0f, 1.0f };
				BufferUniform* uShading = (BufferUniform*)AddUniform("Shading",
					MakeRef<BufferUniform>("Shading", Shader::Stage::Fragment, *m_GraphicsPipeline))
											  .get();
				(*uShading)["color"] = config.color;
			}
		}
	}

	void Material::FillConfig(Config& config, PolygonMode polygonMode)
	{
		config.polygonMode = polygonMode;
	}

	void Material::FillConfig(Config& config, LineWidth lineWidth) { config.lineWidth = lineWidth; }

	std::vector<std::string> Material::GetShaders(Material::Config& config)
	{
		std::vector<std::string> shaders;
		std::vector<std::string> shaderCodes;

		if (config.diffuseMap.value)
			shaderCodes.emplace_back("Diff");
		if (config.specularMap.value)
			shaderCodes.emplace_back("Spec");
		if (config.normalMap.value)
			shaderCodes.emplace_back("Norm");

		// Sort shader codes alphabetically
		std::sort(shaderCodes.begin(), shaderCodes.end());

		if (shaderCodes.empty())
		{
			shaders.emplace_back("Resources/Shaders/ModelShader.vert");
			shaders.emplace_back("Resources/Shaders/ModelShader.frag");
			return shaders;
		}

		shaderCodes.emplace(shaderCodes.begin(), "_");
		std::string shaderCode =
			std::accumulate(shaderCodes.begin(), shaderCodes.end(), std::string{});

		shaders.emplace_back("Resources/Shaders/ModelShader" + shaderCode + ".vert");
		shaders.emplace_back("Resources/Shaders/ModelShader" + shaderCode + ".frag");

		return shaders;
	}

	void Material::Setup(Material::Config& config)
	{
		AddUniform("PerObjectData",
			MakeRef<BufferUniform>("PerObjectData", Shader::Stage::Vertex, *m_GraphicsPipeline));
	}
}  // namespace At0::Ray
