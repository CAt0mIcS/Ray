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

	Uniform& Material::AddUniform(Scope<Uniform> uniform)
	{
		return *m_Uniforms.emplace_back(std::move(uniform));
	}

	bool Material::HasUniform(std::string_view uniformName) const
	{
		for (auto& uniform : m_Uniforms)
			if (uniformName == uniform->GetName())
				return true;
		return false;
	}

	Material& Material::operator=(Material&& other) noexcept
	{
		m_GraphicsPipeline = std::move(other.m_GraphicsPipeline);
		m_Uniforms = std::move(other.m_Uniforms);
		return *this;
	}

	Material::Material(Material&& other) noexcept { *this = std::move(other); }

	Material& Material::operator=(const Material& other)
	{
		m_GraphicsPipeline = other.m_GraphicsPipeline;

		for (const Scope<Uniform>& uniform : other.m_Uniforms)
		{
			if (auto uBuff = dynamic_cast<const BufferUniform*>(uniform.get()))
			{
				m_Uniforms.emplace_back(MakeScope<BufferUniform>(*uBuff));
			}
			else if (auto uSampler = dynamic_cast<const SamplerUniform*>(uniform.get()))
			{
				m_Uniforms.emplace_back(MakeScope<SamplerUniform>(*uSampler));
			}
			else
			{
				Log::Error("[Material] Trying to copy unknown uniform type");
				RAY_ASSERT(false, "[Material] Trying to copy unknown uniform type");
			}
		}

		return *this;
	}

	Material::Material(const Material& other) { *this = other; }

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
				AddUniform(MakeScope<SamplerUniform>("materialDiffuse", Shader::Stage::Fragment,
					config.diffuseMap.value, *m_GraphicsPipeline));
			if (config.specularMap.value)
				AddUniform(MakeScope<SamplerUniform>("materialSpecular", Shader::Stage::Fragment,
					config.specularMap.value, *m_GraphicsPipeline));

			if (config.texture2D.value)
				AddUniform(MakeScope<SamplerUniform>("texSampler", Shader::Stage::Fragment,
					config.texture2D.value, *m_GraphicsPipeline));

			// If no custom shader and no maps were given, the shader expects a color uniform
			if (!config.diffuseMap.value && !config.specularMap.value && !config.normalMap.value &&
				!config.texture2D.value)
			{
				if (!config.color)
					config.color = { { 1.0f, 1.0f, 1.0f } };

				BufferUniform& uShading = (BufferUniform&)AddUniform(MakeScope<BufferUniform>(
					"Shading", Shader::Stage::Fragment, *m_GraphicsPipeline));
				uShading["color"] = *config.color;
			}
		}
	}

	void Material::Setup(Material::Config& config)
	{
		AddUniform(
			MakeScope<BufferUniform>("PerObjectData", Shader::Stage::Vertex, *m_GraphicsPipeline));
	}

	void Material::FillConfig(Config& config, PolygonMode polygonMode)
	{
		config.polygonMode = polygonMode;
	}

	void Material::FillConfig(Config& config, LineWidth lineWidth) { config.lineWidth = lineWidth; }

	void Material::FillConfig(Config& config, Material::LightingTechnique tech)
	{
		config.lightingTechnique = tech;
	}

	std::vector<std::string> Material::GetShaders(Material::Config& config)
	{
		std::vector<std::string> shaders;

		switch (config.lightingTechnique)
		{
		case LightingTechnique::Flat:
		{
			std::vector<std::string> shaderCodes;

			if (config.texture2D.value)
				shaderCodes.emplace_back("Tex");

			if (shaderCodes.empty())
			{
				shaders.emplace_back("Resources/Shaders/Flat.vert");
				shaders.emplace_back("Resources/Shaders/Flat.frag");
				return shaders;
			}

			shaderCodes.emplace(shaderCodes.begin(), "_");
			std::string shaderCode =
				std::accumulate(shaderCodes.begin(), shaderCodes.end(), std::string{});

			shaders.emplace_back("Resources/Shaders/Flat" + shaderCode + ".vert");
			shaders.emplace_back("Resources/Shaders/Flat" + shaderCode + ".frag");

			break;
		}
		case LightingTechnique::Gouraud:
		{
			break;
		}
		case LightingTechnique::Phong:
		{
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
				shaders.emplace_back("Resources/Shaders/Phong.vert");
				shaders.emplace_back("Resources/Shaders/Phong.frag");
				return shaders;
			}

			shaderCodes.emplace(shaderCodes.begin(), "_");
			std::string shaderCode =
				std::accumulate(shaderCodes.begin(), shaderCodes.end(), std::string{});

			shaders.emplace_back("Resources/Shaders/Phong" + shaderCode + ".vert");
			shaders.emplace_back("Resources/Shaders/Phong" + shaderCode + ".frag");
			break;
		}
		}

		RAY_MEXPECTS(!shaders.empty(),
			"[Material] Invalid lighting technique {0} or Material configuration",
			(uint32_t)config.lightingTechnique);
		return shaders;
	}

	std::string Material::AssertConfigCompatibility(const Material::Config& config)
	{
		if (!config.shaders.value.empty())
		{
			Log::Warn("[Material] Using custom shaders. Material properties such the color, any "
					  "maps, textures and lighting techniques will be ignored and uniforms will "
					  "need to be added manually using Material::AddUniform");
			return "";
		}

		std::string errors = "";

		switch (config.lightingTechnique)
		{
		case LightingTechnique::Flat:
		{
			if (config.diffuseMap.value)
				errors += "\n\tFlat shading is not compatible with a diffuse map";
			if (config.specularMap.value)
				errors += "\n\tFlat shading is not compatible with a specular map";
			if (config.normalMap.value)
				errors += "\n\tFlat shading is not compatible with a normal map";

			if (config.texture2D.value && config.color)
				errors += "\n\tCannot add both texture2D and color";

			break;
		}
		case LightingTechnique::Gouraud:
		{
			break;
		}
		case LightingTechnique::Phong:
		{
			if (config.texture2D.value)
				errors += "\n\tPhong shading does not take in a texture2D. Use diffuse or specular "
						  "maps for texturing";

			if (config.diffuseMap.value && config.color)
				errors += "\n\tCannot add both diffuse map and color";
			if (config.specularMap.value && config.color)
				errors += "\n\tCannot add both specular map and color";
			if (config.normalMap.value && config.color)
				errors += "\n\tCannot add both normal map and color";

			break;
		}
		}

		return errors;
	}

}  // namespace At0::Ray
