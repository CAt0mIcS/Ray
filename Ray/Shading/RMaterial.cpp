#include "Rpch.h"
#include "RMaterial.h"

#include "Graphics/Pipelines/RGraphicsPipeline.h"
#include "Graphics/Pipelines/Shader/RShaderReflection.h"
#include "Graphics/Pipelines/Shader/RShader.h"
#include "Utils/RAssert.h"


namespace At0::Ray
{
	Material::Material(Ref<GraphicsPipeline> pipeline, MaterialDataContainer container)
		: m_GraphicsPipeline(std::move(pipeline)), m_Container(std::move(container))
	{
	}

	void Material::CmdBind(const CommandBuffer& cmdBuff) const
	{
		m_GraphicsPipeline->CmdBind(cmdBuff);
	}

	bool Material::HasUniformBlock(const std::string& name) const
	{
		return m_Container.HasUniformBlock(name);
	}

	Ref<Texture2D> Material::GetTexture(const std::string& dataPath) const
	{
		return m_Container.GetTexture(dataPath);
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////// BUILDER //////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	Material::Builder::Builder(Ref<GraphicsPipeline> pipeline)
		: m_GraphicsPipeline(std::move(pipeline))
	{
	}

	Material::Builder& Material::Builder::Set(const std::string& name, Ref<Texture2D> data)
	{
		ValidateUniformExistence(name);
		m_Container.Set(name, std::move(data));
		return *this;
	}

	Ref<Material> Material::Builder::Acquire()
	{
		return MakeRef<Material>(std::move(m_GraphicsPipeline), std::move(m_Container));
	}

	void Material::Builder::ValidateUniformExistence(const std::string& name)
	{
		bool hasUniform = false;
		for (const auto& [stage, reflection] : m_GraphicsPipeline->GetShader().GetReflections())
		{
			if (reflection.HasPathedUniform(name))
			{
				hasUniform = true;
				break;
			}
		}

		RAY_MEXPECTS(hasUniform,
			"[Material::Builder] Uniform \"{0}\" does not exist in shaders:\n\"{1}\"\n\"{2}\"",
			name, m_GraphicsPipeline->GetShader().GetFilepaths()[0],
			m_GraphicsPipeline->GetShader().GetFilepaths()[1]);
	}
}  // namespace At0::Ray
