#include "Rpch.h"
#include "RMaterial.h"

#include "Graphics/Pipelines/RGraphicsPipeline.h"
#include "Graphics/Pipelines/Shader/RShaderReflection.h"
#include "Graphics/Pipelines/Shader/RShader.h"
#include "Events/REventListener.h"


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
		m_Container.Set(name, std::move(texture));
		CallListeners(name, UniformType::CombinedImageSampler);
	}

	void Material::CallListeners(const std::string& name, UniformType type)
	{
		Log::Trace("[Material] Calling OnDirtyListeners (Count: {0})",
			EventDispatcher<MaterialBecameDirtyEvent>::Get().size());
		MaterialBecameDirtyEvent e{ name, type };
		for (auto listener : EventDispatcher<MaterialBecameDirtyEvent>::Get())
			listener->OnEvent(e);
	}

	UniformType Material::GetUniformType(std::string_view name) const
	{
		for (auto& [stage, reflection] : m_GraphicsPipeline->GetShader().GetReflections())
		{
			if (auto* uBlockData = reflection.TryGetUniformBlock(name.substr(0, name.find('.')));
				uBlockData)
				return uBlockData->type;
		}
		return UniformType::CombinedImageSampler;
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
		Builder::ValidateUniformExistenceAndSize(*m_GraphicsPipeline, name, -1);
		m_Container.Set(name, std::move(data));
		return *this;
	}

	Ref<Material> Material::Builder::Acquire()
	{
		return MakeRef<Material>(std::move(m_GraphicsPipeline), std::move(m_Container));
	}

	Ref<Material> Material::Builder::Build()
	{
		return MakeRef<Material>(std::move(m_GraphicsPipeline), std::move(m_Container));
	}

	void Material::Builder::ValidateUniformExistenceAndSize(
		const GraphicsPipeline& pipeline, const std::string& name, uint32_t size)
	{
		bool hasUniform = false;
		uint32_t expectedSize = 0;
		for (const auto& [stage, reflection] : pipeline.GetShader().GetReflections())
		{
			if (auto* uniform = reflection.TryGetPathedUniform(name); uniform)
			{
				hasUniform = true;
				expectedSize = uniform->size;
				break;
			}
		}

		RAY_MEXPECTS(hasUniform,
			"[Material::Builder] Uniform \"{0}\" does not exist in shaders:\n\"{1}\"\n\"{2}\"",
			name, pipeline.GetShader().GetFilepaths()[0], pipeline.GetShader().GetFilepaths()[1]);

		if (size != -1)
			RAY_MEXPECTS(expectedSize == size,
				"[Material::Builder] Uniform \"{0}\" expects size "
				"of {1} byte(s) but received {2} byte(s)",
				name, expectedSize, size);
	}
}  // namespace At0::Ray
