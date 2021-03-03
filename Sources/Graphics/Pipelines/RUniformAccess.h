#pragma once

#include "../../RBase.h"
#include "RShader.h"
#include "../Buffers/RBufferSynchronizer.h"
#include "../../Utils/RAssert.h"

#include <unordered_map>
#include <vector>


namespace At0::Ray
{
	class Pipeline;

	class RAY_EXPORT UniformAccess
	{
	public:
		class UniformData
		{
		public:
			UniformData(uint32_t offset) : m_OffsetInUniformBlock(offset) {}
			UniformData() : m_OffsetInUniformBlock((uint32_t)-1) {}

			template<typename T>
			UniformData& Update(T&& data, uint32_t globalOffset)
			{
				RAY_MEXPECTS(m_OffsetInUniformBlock != (uint32_t)-1,
					"[UniformData] Uniform does not exist.");

				BufferSynchronizer::Get().Update(data, globalOffset + m_OffsetInUniformBlock);
				return *this;
			}

		private:
			uint32_t m_OffsetInUniformBlock = 0;
		};


		class UniformDataAccess
		{
		public:
			UniformDataAccess(const Shader::Uniforms* uniforms,
				const Shader::UniformBlocks* uniformBlocks,
				std::pair<std::string, const Shader::UniformBlocks::UniformBlockData*>& blockCache)
				: m_Uniforms(uniforms), m_UniformBlocks(uniformBlocks), m_BlockCache(blockCache)
			{
			}

			UniformData operator()(std::string_view uniformBlockName, std::string_view uniformName);
			UniformData operator()(std::string_view uniformName);

		private:
			const Shader::Uniforms* m_Uniforms;
			const Shader::UniformBlocks* m_UniformBlocks;
			std::pair<std::string, const Shader::UniformBlocks::UniformBlockData*>& m_BlockCache;
		};

	public:
		UniformAccess(const Pipeline& pipeline);
		UniformAccess() = default;

		UniformDataAccess Resolve(Shader::Stage stage);

		template<Shader::Stage stage>
		UniformData Resolve(std::string_view uniformBlockName, std::string_view uniformName)
		{
			RAY_MEXPECTS(m_Shader, "[UniformAccess] Not created using pipeline.");
			return Resolve(stage)(uniformBlockName, uniformName);
		}

		template<Shader::Stage stage>
		UniformData Resolve(std::string_view uniformName)
		{
			RAY_MEXPECTS(m_Shader, "[UniformAccess] Not created using pipeline.");
			return Resolve(stage)(uniformName);
		}

	private:
		const Shader* m_Shader = nullptr;
		std::pair<std::string, const Shader::UniformBlocks::UniformBlockData*> m_BlockCache{};
	};
}  // namespace At0::Ray
