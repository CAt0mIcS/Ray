#include "Rpch.h"
#include "RSamplerUniform.h"

#include "Utils/RAssert.h"
#include "../../Images/RTexture2D.h"


namespace At0::Ray
{
	SamplerUniform::SamplerUniform(std::string_view uniformName, Shader::Stage stage,
		Ref<Texture2D> texture, const Pipeline& pipeline)
		: Uniform(uniformName), m_Texture(std::move(texture))
	{
		std::optional<uint32_t> set;

		if (auto uniforms = pipeline.GetShader().GetUniforms(stage))
		{
			if (auto uniformData = uniforms->Get(uniformName))
				set = uniformData->set;
		}

		RAY_MEXPECTS(set, "[SamplerUniform] Failed to find uniform {0} in stage {1}", uniformName,
			String::Construct(stage));

		m_Set = *set;

		m_DescriptorSet = MakeScope<DescriptorSet>(pipeline.GetDescriptorPool(),
			pipeline.GetDescriptorSetLayout(*set), pipeline.GetBindPoint(), pipeline.GetLayout(),
			*set);

		Setup();
	}

	SamplerUniform::~SamplerUniform() {}

	SamplerUniform& SamplerUniform::operator=(const SamplerUniform& other)
	{
		Uniform::operator=(other);
		m_Set = other.m_Set;
		m_Texture = other.m_Texture;
		Setup();
		return *this;
	}

	SamplerUniform::SamplerUniform(const SamplerUniform& other) : Uniform(other.m_Name)
	{
		*this = other;
	}

	void SamplerUniform::Setup()
	{
		// RAY_TODO: Check if image is in the correct format
		VkDescriptorImageInfo imageInfo{};
		imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		imageInfo.sampler = m_Texture->GetSampler();
		imageInfo.imageView = m_Texture->GetImage().GetImageView();

		VkWriteDescriptorSet descWriteImg{};
		descWriteImg.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descWriteImg.dstSet = *m_DescriptorSet;
		descWriteImg.dstBinding = m_Set;
		descWriteImg.dstArrayElement = 0;
		descWriteImg.descriptorCount = 1;
		descWriteImg.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		descWriteImg.pImageInfo = &imageInfo;
		DescriptorSet::Update({ descWriteImg });
	}
}  // namespace At0::Ray
