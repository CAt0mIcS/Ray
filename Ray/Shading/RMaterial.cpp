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

	void Material::Set(const std::string& name, Ref<Texture2D> texture)
	{
		Builder::Set(name, std::move(texture), m_Container);
		CallListeners(name, UniformType::CombinedImageSampler);
	}

	uint32_t Material::AddOnDirtyListener(std::function<void(const std::string&, UniformType)> fun)
	{
		m_OnDirtyListeners.emplace_back(fun);
		return m_OnDirtyListeners.size() - 1;
	}

	void Material::RemoveOnDirtyListener(uint32_t index)
	{
		// Move element to the back
		m_OnDirtyListeners[index] = std::move(m_OnDirtyListeners.back());
		m_OnDirtyListeners.pop_back();
	}

	void Material::CallListeners(const std::string& name, UniformType type) const
	{
		for (const auto& func : m_OnDirtyListeners)
			func(name, type);
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
		ValidateUniformExistence(*m_GraphicsPipeline, name);
		Set(name, std::move(data), m_Container);
		return *this;
	}

	void Material::Builder::Set(
		const std::string& name, Ref<Texture2D> data, MaterialDataContainer& container)
	{
		container.Set(name, std::move(data));
	}

	Ref<Material> Material::Builder::Acquire()
	{
		return MakeRef<Material>(std::move(m_GraphicsPipeline), std::move(m_Container));
	}

	void Material::Builder::ValidateUniformExistence(
		const GraphicsPipeline& pipeline, const std::string& name)
	{
		bool hasUniform = false;
		for (const auto& [stage, reflection] : pipeline.GetShader().GetReflections())
		{
			if (reflection.HasPathedUniform(name))
			{
				hasUniform = true;
				break;
			}
		}

		RAY_MEXPECTS(hasUniform,
			"[Material::Builder] Uniform \"{0}\" does not exist in shaders:\n\"{1}\"\n\"{2}\"",
			name, pipeline.GetShader().GetFilepaths()[0], pipeline.GetShader().GetFilepaths()[1]);
	}
}  // namespace At0::Ray
