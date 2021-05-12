#pragma once

#include "../RBase.h"
#include "../Utils/RNonCopyable.h"
#include "../Graphics/Core/RBindable.h"
#include "../Graphics/Pipelines/RShader.h"

#include <vector>
#include <unordered_map>


namespace At0::Ray
{
	class GraphicsPipeline;
	class DescriptorSet;
	class BufferUniform;
	class Sampler2DUniform;
	class Texture2D;

	class RAY_EXPORT Material : public Bindable, NonCopyable
	{
	public:
		Material();
		~Material();

		void CmdBind(const CommandBuffer& cmdBuff) const;

		void AddBufferUniform(std::string_view name, Shader::Stage stage);
		void AddSampler2DUniform(
			std::string_view name, Shader::Stage stage, Ref<Texture2D> texture);

		Ref<BufferUniform> GetBufferUniform(std::string_view name);
		Ref<Sampler2DUniform> GetSampler2DUniform(std::string_view name);

		Material& operator=(Material&& other) noexcept;
		Material(Material&& other) noexcept;

	private:
		Ref<GraphicsPipeline> m_GraphicsPipeline = nullptr;

		std::unordered_map<uint32_t, Ref<DescriptorSet>> m_DescriptorSets;
		std::unordered_map<uint32_t, std::vector<Ref<BufferUniform>>> m_BufferUniforms;
		std::unordered_map<uint32_t, std::vector<Ref<Sampler2DUniform>>> m_Sampler2DUniforms;
	};
}  // namespace At0::Ray
