#include "RMaterial.h"

#include "Graphics/Pipelines/RGraphicsPipeline.h"
#include "Graphics/Pipelines/Shader/RShaderReflection.h"
#include "Graphics/Pipelines/Shader/RShader.h"
#include "Events/REventListener.h"

#include "Graphics/Images/RTexture2DAtlas.h"


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

	Ref<Texture> Material::GetTexture(const std::string& dataPath) const
	{
		return m_Container.GetTexture(dataPath);
	}

	void Material::Set(const std::string& name, Ref<Texture> texture, VkImageLayout imageLayout)
	{
		m_Container.Set(name, std::move(texture));
		CallListeners(name, UniformType::CombinedImageSampler, imageLayout);
	}

	void Material::Set(const std::string& name, Ref<Texture2DAtlas> texture)
	{
		m_Container.Set(name, std::static_pointer_cast<Texture>(std::move(texture)));
		CallListeners(name, UniformType::CombinedImageSampler);
	}

	void Material::CallListeners(
		const std::string& name, UniformType type, VkImageLayout imageLayout)
	{
		Log::Trace("[Material] Calling OnDirtyListeners (Count: {0})",
			EventDispatcher<MaterialBecameDirtyEvent>::Get().size());

		// Tells the MeshRenderer to update descriptors with name and type
		MaterialBecameDirtyEvent e{ name, type, imageLayout };
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
		: m_GraphicsPipeline(std::move(pipeline)), m_Container(m_GraphicsPipeline->GetShader())
	{
	}

	Material::Builder& Material::Builder::Set(const std::string& name, Ref<Texture> data)
	{
		Builder::ValidateUniformExistenceAndSize(*m_GraphicsPipeline, name, -1);
		m_Container.Set(name, std::move(data));
		return *this;
	}

	Material::Builder& Material::Builder::Set(const std::string& name, Ref<Texture2DAtlas> data)
	{
		Builder::ValidateUniformExistenceAndSize(*m_GraphicsPipeline, name, -1);
		m_Container.Set(name, std::static_pointer_cast<Texture>(std::move(data)));
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
		ShaderDataType expectedType;
		for (const auto& [stage, reflection] : pipeline.GetShader().GetReflections())
		{
			if (auto* uniform = reflection.TryGetPathedUniform(name); uniform)
			{
				hasUniform = true;
				expectedSize = uniform->size;
				expectedType = uniform->dataType;
				break;
			}
		}

		RAY_MEXPECTS(hasUniform,
			"[Material::Builder] Uniform \"{0}\" does not exist in shaders:\n\"{1}\"\n\"{2}\"",
			name, pipeline.GetShader().GetFilepaths()[0], pipeline.GetShader().GetFilepaths()[1]);

		if (size != -1)
			RAY_MEXPECTS(expectedSize == size,
				"[Material::Builder] Uniform \"{0}\" expects size "
				"of {1} byte(s) but received {2} byte(s) (ShaderDataType: {3})",
				name, expectedSize, size, String::Construct(expectedType));
	}
}  // namespace At0::Ray
